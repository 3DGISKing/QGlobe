#include "gdm_renderview.h"
#include "GDM_SceneManager.h"
#include "GDM_DataMgr.h"
#include "GDM_TextMgr.h"

RenderView::RenderView(QWidget *parent)
: QGLWidget(QGLFormat(QGL::DoubleBuffer/* | QGL::SampleBuffers*/), parent)
{
	_RenderSceneMgr = 0;
}

RenderView::~RenderView()
{
}

void RenderView::OnInitializeGL()
{
	InitializeRenderEngine();
}


void RenderView::OnResizeGL(int width, int height )
{
	if(_RenderSceneMgr == 0)
		return;

	geo::GeoDevice* device = _RenderSceneMgr->RenderDevice();
	if ( device != 0 )
	{
		geo::core::dimension2d<geo::s32> screenSize;
		screenSize.Width = width;
		screenSize.Height = height;
		device->getVideoDriver()->OnResize( screenSize );

		geo::scene::ICameraSceneNode *cam = device->getSceneManager()->getActiveCamera();
		if ( cam != 0 )
		{
			cam->setAspectRatio( (float)screenSize.Width / screenSize.Height );
		}
	}

	gdm_GetDataMgr()->m_pTextMgr->Init(width, height);
}

void RenderView::OnPaint(GDM_RENDER_INFO *renderInfo)
{
	if(_RenderSceneMgr == 0)
		return;

	_RenderSceneMgr->renderScene();
}

bool RenderView::InitializeRenderEngine()
{
	// Don't initialize more than once!
	if ( _RenderSceneMgr == 0 ) 
		return false;

	if(false == _RenderSceneMgr->registerRenderEngine(this))
		return false;

	/*
	// Geo will clear the canvas for us, so tell Qt not to do it
	setAttribute( Qt::WA_OpaquePaintEvent );

	connect( this, SIGNAL(updateGeo(geo::GeoDevice*)),
		this, SLOT(autoUpdateGeo(geo::GeoDevice*)) );
	*/

	return true;
}

bool RenderView::GetFrameBuffer(QImage *qImage)
{
	geo::GeoDevice* device = _RenderSceneMgr->RenderDevice();
	if ( device == 0)
		return false;

	geo::video::IVideoDriver* driver = device->getVideoDriver();
	if( driver == 0 )
		return false;

   geo::core::dimension2d<geo::s32> screenSize = driver->getScreenSize();
	
   screenSize.set(screenSize.Width*4,screenSize.Height*4);

	geo::video::ITexture* texture;

	if (!driver->queryFeature(geo::video::EVDF_RENDER_TO_TARGET))
		return false;

	//[kimyi]
	//Some Computer with Low Graphics Performance does not support FRAMEBUFFER render && Texture_Not_Power_2
	//So In that case, ImageRender must be Scaled and the render quality will be so bad... 
	geo::core::dimension2d<geo::s32> textureSize = screenSize;
	if (!driver->queryFeature(geo::video::EVDF_FRAMEBUFFER_OBJECT))
	{
		textureSize = textureSize.getOptimalSize(!driver->queryFeature(geo::video::EVDF_TEXTURE_NPOT));
	}

	texture = driver->addRenderTargetTexture(textureSize, "RenderTargetTexture1");
	if(!texture)
		return false;

	// draw scene into render target

	// set render target texture
	driver->setRenderTarget(texture, true, true, geo::video::SColor(0,0,0,255));

	device->getSceneManager()->drawAll();

	// set back old render target
	// The buffer might have been distorted, so clear it
	driver->setRenderTarget(0, true, true, 0);

	void* pImageBuffer = texture->lock();
	const geo::video::IImage* image = texture->getImage();
	geo::core::dimension2d<geo::s32> sizeImage = image->getDimension();
	geo::video::ECOLOR_FORMAT colorFormat = image->getColorFormat();

	QImage::Format format;
	switch (colorFormat)
	{
	case geo::video::ECF_A1R5G5B5:
	case geo::video::ECF_R5G6B5:
		format = QImage::Format_RGB16;
		break;
	case geo::video::ECF_R8G8B8:
		format = QImage::Format_RGB888;
		break;
	case geo::video::ECF_A8R8G8B8:
		format = QImage::Format_ARGB32;
		break;
	default:
		texture->unlock();
		driver->removeTexture(texture);
		return false;
	}

	int imageSize = image->getImageDataSizeInBytes();
	void* pBuffer = malloc(imageSize);
	memcpy(pBuffer, pImageBuffer, imageSize);

	QImage imgTmp((uchar*)pBuffer, sizeImage.Width, sizeImage.Height, format);
	if(textureSize != screenSize)
		*qImage = imgTmp.scaled(screenSize.Width, screenSize.Height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	else
		*qImage = imgTmp;

	texture->unlock();
	driver->removeTexture(texture);

	return true;
}

void RenderView::UpdateCameraInfo(GDM_Coord3D& coord, GDM_ViewFrustum& viewFrustum)
{
	if(_RenderSceneMgr)
	{
		geo::core::vector3df worldPos(coord.m_org.m_tX, -coord.m_org.m_tY, coord.m_org.m_tZ);
#ifdef GDM_RENDER_APPLY_OFFSET_BODY
		coord.m_org.set(0,0,0);
#else
		worldPos.set(0,0,0);
#endif
		geo::core::vector3df camPos(coord.m_org.m_tX, -coord.m_org.m_tY, coord.m_org.m_tZ);
		geo::core::vector3df upVector(coord.m_y.m_tX, -coord.m_y.m_tY, coord.m_y.m_tZ);
		geo::core::vector3df targetPos(-coord.m_z.m_tX * EARTH_RADIUS + coord.m_org.m_tX, (coord.m_z.m_tY * EARTH_RADIUS - coord.m_org.m_tY) , (-coord.m_z.m_tZ * EARTH_RADIUS + coord.m_org.m_tZ));
		//geo::core::vector3df targetPos(-coord.m_z.m_tX, coord.m_z.m_tY , -coord.m_z.m_tZ);

		_RenderSceneMgr->setCameraPos(worldPos, camPos, targetPos, upVector);
		_RenderSceneMgr->setViewFrustum(viewFrustum.m_dNear, viewFrustum.m_dFar, viewFrustum.m_dFovy, viewFrustum.m_dAspect);
	}
}
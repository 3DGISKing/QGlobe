#include "renderview.h"
#include "SceneManager.h"

RenderView::RenderView(QWidget *parent)
: QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
{
	_RenderSceneMgr = 0;
	_updateTexture = false;
	startTimer(10);
}

RenderView::~RenderView()
{
}

void RenderView::initializeGL()
{
	makeCurrent();
	
	OnInitializeGL();
	_RenderSceneMgr->prepareSceneGraph();
}

void RenderView::resizeGL(int w, int h)
{
	OnResizeGL(w,h);
}

void RenderView::OnInitializeGL()
{
	InitializeRenderEngine();
}

void RenderView::paintGL()
{
	_RenderSceneMgr->OnBeforeRender();
	renderScene();
	_RenderSceneMgr->OnAfterRender();
}

void RenderView::timerEvent(QTimerEvent *event)
{
	repaint();
}

void RenderView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	Qt::MouseButtons mouseFlag = mouseEvent->buttons();
	if(mouseFlag == Qt::NoButton)
		return;

	geo::SEvent event;
	event.EventType = geo::EET_MOUSE_INPUT_EVENT;
	//if(mouseFlag == Qt::LeftButton)
	//{
	//	event.MouseInput.Event = 
	//}
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
}

bool RenderView::renderScene()
{
	geo::GeoDevice* device = _RenderSceneMgr->RenderDevice();
	if ( device == 0)
		return false;

	geo::video::IVideoDriver* driver = device->getVideoDriver();
	if(!driver)
		return false;

	device->getTimer()->tick();

	geo::video::SColor color (0,0,0,0);

	if(_updateTexture)
	{
		_updateTexture = false;
		_RenderSceneMgr->SetOverlayTexture(&_textureImage);
	}

	device->getVideoDriver()->beginScene( true, true, color );

	device->getSceneManager()->drawAll();

	device->getVideoDriver()->endScene();

	return true;
}

bool RenderView::InitializeRenderEngine()
{
	// Don't initialize more than once!
	if ( _RenderSceneMgr == 0 ) 
		return false;

	if(false == _RenderSceneMgr->registerRenderEngine(this))
		return false;

	return true;
}

void RenderView::SetPolygonTexture(QPixmap* pixmap)
{
	if(_RenderSceneMgr)
	{
		_textureImage = pixmap->toImage();
		_updateTexture = true;
	}
}
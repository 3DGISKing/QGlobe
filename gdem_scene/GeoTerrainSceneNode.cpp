#include "GeoTerrainSceneNode.h"
#include "GDM_TextureMgr.h"
#include "GIS_TextureMgr.h"
#include "GDM_FPSTester.h"
#include "GeoFont.h"
#include "COpenGLDriver.h"
#include "GDM_DataMgr.h"

#include "../gdem_client/gdemclient.h"
#include "../gdem_client/config.h"
#include "RasterTextureMgr.h"
#include "Layer.h"
#include "RasterRenderer.h"
#include "ColorRamp.h"

extern int	gdm_GetRenderCallBackTerrain(GDEM_RENDER_GEO_PARAM *pParam);
extern ushort TEXT_FLIGHT_SIM_NORTH [];
extern ushort TEXT_FLIGHT_SIM_EAST	 [];
extern ushort TEXT_FLIGHT_SIM_SOUTH [];
extern ushort TEXT_FLIGHT_SIM_WEST	 [];

namespace geo
{
namespace scene
{
	//! constructor
	CGeoTerrainSceneNode::CGeoTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position, const core::vector3df& rotation,
		const core::vector3df& scale)
		: ISceneNode(parent, mgr, id, position, rotation, scale), MaxTileSize(129)
	{
#ifdef _DEBUG
		setDebugName("CGeoTerrainSceneNode");
#endif

		RenderBuffer = new CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
		RenderBuffer->Material.MaterialType =video::EMT_OVERLAY_TEXTURE_BLEND;
		RenderBuffer->Material.EmissiveColor.set(255,150,150,150);
		RenderBuffer->Material.DiffuseColor.set(255,255,255,255);
		RenderBuffer->Material.Lighting=true;
		Initialize();
		m_lineColor = video::SColor(255,0,255,0);
		heightTexture = 0;
		earthTexture = 0;
		rateAngleTexture = 0;
		ratePlanTexture  = 0;
		FogEnable = false;
		FogStart = 0;
		FogEnd = 0;

		m_pFont = NULL;
		
		gdm_TileListSelector = new TreeListSelector;

		QList<IColorRamp*> ramplist=g_pGDMDataMgr->m_ColorRampList;
		
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		for(int i=0;i<ramplist.size();i++)
		{
			IColorRamp* ramp=ramplist[i];

			QString name=ramp->GetName();

			QImage symbol=ramp->GetImage();

			char textureName[256] = {0,};

			sprintf(textureName, "%s", name.toAscii().data());

			Q_ASSERT(strlen(textureName)<256);

			geo::video::ITexture* texture;

			geo::video::IImage *image = driver->createImageFromData(geo::video::ECF_A8R8G8B8, geo::core::dimension2di(symbol.width(), symbol.height()), symbol.bits(), false, true);

			Q_ASSERT(image!=NULL);

			texture = driver->addTexture(textureName, image);

			image->drop();

			Q_ASSERT(texture!=NULL);

			_RasterSymbolHash.insert(name,texture);
		}
	}

	//! destructor
	CGeoTerrainSceneNode::~CGeoTerrainSceneNode()
	{
		if(RenderBuffer)
			RenderBuffer->drop();

		delete gdm_TileListSelector;
	}

void CGeoTerrainSceneNode::OnRegisterSceneNode()
{
	SceneManager->registerNodeForRendering(this);
}

void CGeoTerrainSceneNode::CalcuFog()
{
	/*GDM_Coord3D coord;
	CGDM_Vector3DF pt1, pt2;
	double minHeight = 100;
	double fogRate = 0.25;

	gdm_GetDataMgr()->m_pCamera->GetCameraCoord(&coord);

	double angle;
	double radius = EARTH_RADIUS; 
	double dCameraDist = coord.m_org.getLength();

	if(dCameraDist < (radius + minHeight))
		dCameraDist = radius + minHeight;
	
	// end dist
	FogEnd = sqrt(dCameraDist * dCameraDist - radius * radius);

	// start dist
	fogRate = sqrt(dCameraDist - radius) / sqrt(radius * 5);
	if(fogRate > 0.9)
		fogRate = 0.9;

	angle = acos(radius / dCameraDist);
	angle = angle * fogRate;

	pt1.m_tX = radius * cos(angle);
	pt1.m_tY = radius * sin(angle);
	pt1.m_tZ = 0;
	pt2.m_tX = dCameraDist;
	pt2.m_tY = pt2.m_tZ = 0;

	FogStart = pt1.getDistanceFrom(pt2);*/

	GDM_Coord3D coord;
	
	GDM_LOCATION loc;
	gdm_GetDataMgr()->m_pCamera->GetLocation(&loc);

	double h=loc.m_dDist;

	FogEnd=sqrt(2*EARTH_RADIUS*h+h*h)*0.3;

	FogStart=h;

	qDebug("Fogstart=%f fogend=%f",FogStart,FogEnd);

}

void CGeoTerrainSceneNode::PreRender()
{
	gdm_GetPyramidDivisionTiles(&g_gdm_MeshTileArr);

	pWriteQuadList = gdm_TileListSelector->GetBufForWrite();
	pReadQuadList  = gdm_TileListSelector->GetBufForRead();
	pWriteQuadList->start();
}

void CGeoTerrainSceneNode::AfterRender()
{
	QuadTree *pReadTree = NULL;
	QuadTree *pWriteTree = NULL;

	if (pReadQuadList && pReadQuadList->LRUUsage)
	{
		pReadTree = pReadQuadList->LRUHead;	
		while(pReadTree)
		{					
			if (pReadTree->nLevel < pWriteQuadList->LRUHead->nLevel-3)
				break;
			if (!pReadTree->LRU)
			{
				pWriteTree = pWriteQuadList->add(pReadTree);
				if (!pWriteTree)
					break;
				pWriteTree->LRU = 0;
			}
			if (pWriteQuadList->m_nUsed >= GDM_MAX_TILE-3)
				break;
			pReadTree = pReadTree->pLRUNext;
		}
	}

	gdm_TileListSelector->SetReadBuf();
}

void CGeoTerrainSceneNode::render()
{
	if(g_app->GetConfig()->m_IsLicence==false)  return;
			
	PreRender();

	f32 fNear = SceneManager->getActiveCamera()->getNearValue();
	f32 fFar = SceneManager->getActiveCamera()->getFarValue();

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	video::SMaterial& material = RenderBuffer->getMaterial();
	material.FogEnable = FogEnable;

	if(FogEnable)
	{
		CalcuFog();

		driver->setFog(geo::video::SColor(0, 128, 192, 255), true, FogStart, FogEnd, 0.5f, true);
	}

	GDEM_RENDER_GEO_PARAM param;

	param.nIndex = 0;
	
	param.pVertexBuf = (geo::video::S3DVertex2TCoords *)RenderBuffer->getVertexBuffer().pointer();
	param.pIndexBuf  = (unsigned short *)RenderBuffer->getIndexBuffer().pointer();

    driver->setTransform (video::ETS_WORLD, core::IdentityMatrix);

	s32 indicesToRender;

	video::ITexture* texture;
	video::ITexture* overlay_texture;

	switch(gdm_GetDataMgr()->m_sOption.layerTextureMode)
	{
		case LTM_HEIGHT:
			texture = heightTexture;
			break;
		case LTM_RATEANGLE:
			texture = rateAngleTexture;
			break;
		case LTM_RATEPLAN:
			texture = ratePlanTexture;
			break;
	}

	while(1)
	{
		param.nIdBufCount = 0;
		param.nVertexCount = 0;

		indicesToRender = gdm_GetRenderCallBackTerrain(&param);
		gdm_GetFPSTester()->m_totalMeshCount += indicesToRender / 3;

		if(indicesToRender <= 0)
			break;

		
		if(gdm_GetDataMgr()->m_sOption.layerTextureMode==LTM_TEXTURE)
		{
			texture = (video::ITexture*)gdm_GetTextureMgr()->GetTexture(param.nTextureID);
			if(!texture)
				texture=earthTexture;
		}

		if(gdm_GetDataMgr()->m_sOption.layerTextureMode==LTM_RASTER)
		{
			texture = (video::ITexture*)gdm_GetRasterTextureMgr()->GetTexture(param.nTextureID);
			if(!texture)
				texture=rasterNodataTexture;
		}
	
		overlay_texture = (video::ITexture*)gdm_GetGISTextureMgr()->GetTexture(param.nOverlayTextureID);

		Q_ASSERT(texture!=NULL);

		if(overlay_texture != NULL)
		{
			material.setTexture(0, overlay_texture);
			material.setTexture(1, texture);
		}
		else
		{
			material.setTexture(0, texture);
			material.setTexture(1, 0);
		}

		driver->setMaterial(material);

		RenderBuffer->getIndexBuffer().set_used(indicesToRender);

		for( int i=0;i<param.nVertexCount;i++)
			param.pVertexBuf[i].Color=material.DiffuseColor;

#ifdef DEBUG
		if (g_app->GetConfig()->m_isShowTerrainInner )	
		{
			video::SMaterial debug_mat;
			debug_mat.Lighting = false;
			debug_mat.Wireframe = true;
			debug_mat.ZBuffer = true;
			driver->setMaterial(debug_mat);
			driver->drawVertexPrimitiveList(RenderBuffer->getVertices(), param.nVertexCount, RenderBuffer->getIndices(), 
				RenderBuffer->getIndexCount()/3, RenderBuffer->getVertexType(), scene::EPT_TRIANGLES, RenderBuffer->getIndexType());
		}
#endif

		if(RenderBuffer->Material.Lighting)
		{
			driver->getMeshManipulator()->recalculateNormals(RenderBuffer);

		}

		// For use with geomorphing
		driver->drawVertexPrimitiveList(RenderBuffer->getVertices(), param.nVertexCount, RenderBuffer->getIndices(), 
			RenderBuffer->getIndexCount()/3, RenderBuffer->getVertexType(), scene::EPT_TRIANGLES, RenderBuffer->getIndexType());

#ifdef DEBUG
			if (g_app->GetConfig()->m_isShowTerrainWireframe )	
			{
				video::SMaterial debug_mat;
				debug_mat.Lighting = false;
				debug_mat.Wireframe = true;
				debug_mat.ZBuffer = true;
				driver->setMaterial(debug_mat);
				driver->drawVertexPrimitiveList(RenderBuffer->getVertices(), param.nVertexCount, RenderBuffer->getIndices(), 
					RenderBuffer->getIndexCount()/3, RenderBuffer->getVertexType(), scene::EPT_TRIANGLES, RenderBuffer->getIndexType());
			}
#endif

		RenderBuffer->getIndexBuffer().set_used( RenderBuffer->getIndexBuffer().allocated_size() );
	}	

	AfterRender();

	drawMark();
	if(g_pGDMDataMgr->m_sOption.layerTextureMode==LTM_RASTER)
		DrawRasterSymbol();
}

//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CGeoTerrainSceneNode::getBoundingBox() const
{
	return Mesh.BoundingBox;
}

bool CGeoTerrainSceneNode::Initialize()
{
	const u32 numVertices = MaxTileSize * MaxTileSize;

	RenderBuffer->getIndexBuffer().setType(video::EIT_16BIT);

	RenderBuffer->getVertexBuffer().set_used(numVertices);

	RenderBuffer->getIndexBuffer().set_used(MaxTileSize*MaxTileSize*6);

	RenderBuffer->setDirty();

	return true;
}

void CGeoTerrainSceneNode::drawMark()
{
	if(gdm_GetDataMgr()->m_sOption.layerTextureMode == LTM_TEXTURE)
		return;

	QString strTop, strBot, strRight, strLeft;
	core::rect<s32> sourceRect, destRect;
	core::position2d<s32> transPos;

	// initialize
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	
	const core::dimension2d<s32>& renderTargetSize = driver->getScreenSize();


	if(gdm_GetDataMgr()->m_sOption.layerTextureMode == LTM_RATEANGLE && rateAngleTexture)
	{

		transPos.X = renderTargetSize.Width * 14 / 15;
		transPos.Y = renderTargetSize.Height / 2;

		sourceRect.UpperLeftCorner.X = 0;
		sourceRect.UpperLeftCorner.Y = 0;
		sourceRect.LowerRightCorner.X = rateAngleTexture->getOriginalSize().Width;
		sourceRect.LowerRightCorner.Y = rateAngleTexture->getOriginalSize().Height;

		destRect.UpperLeftCorner = transPos;
		destRect.LowerRightCorner.X = transPos.X + renderTargetSize.Width / 50;
		destRect.LowerRightCorner.Y = renderTargetSize.Height - 100;

		strTop.sprintf("%d%c", GDM_GIS_MAX_ANGLE , 0x00B0);
		strBot.sprintf("%d%c", 0 , 0x00B0);

		driver->draw2DImage(rateAngleTexture, destRect, sourceRect);

		glDisable(GL_TEXTURE);

		DrawOutlineString(destRect.LowerRightCorner.X + 5, destRect.UpperLeftCorner.Y + 10, strTop, 0.8f);
		DrawOutlineString(destRect.LowerRightCorner.X + 5, destRect.LowerRightCorner.Y, strBot, 0.8f);
		glEnable(GL_TEXTURE);
	}
	else if(gdm_GetDataMgr()->m_sOption.layerTextureMode == LTM_HEIGHT && heightTexture)
	{

		transPos.X = renderTargetSize.Width * 14 / 15;
		transPos.Y = renderTargetSize.Height / 2;

		sourceRect.UpperLeftCorner.X = 0;
		sourceRect.UpperLeftCorner.Y = 0;
		sourceRect.LowerRightCorner.X = heightTexture->getOriginalSize().Width;
		sourceRect.LowerRightCorner.Y = heightTexture->getOriginalSize().Height;

		destRect.UpperLeftCorner = transPos;
		destRect.LowerRightCorner.X = transPos.X + renderTargetSize.Width / 50;
		destRect.LowerRightCorner.Y = renderTargetSize.Height - 100;

		strTop.sprintf("%dkm", (int)(GDM_MAX_HEIGHT / 1000));
		strBot.sprintf("%dm", 0);

		driver->draw2DImage(heightTexture, destRect, sourceRect);

		glDisable(GL_TEXTURE);

		DrawOutlineString(destRect.LowerRightCorner.X + 5, destRect.UpperLeftCorner.Y + 10, strTop, 0.8f);
		DrawOutlineString(destRect.LowerRightCorner.X + 5, destRect.LowerRightCorner.Y, strBot, 0.8f);
		glEnable(GL_TEXTURE);
	}
	else if(gdm_GetDataMgr()->m_sOption.layerTextureMode == LTM_RATEPLAN && ratePlanTexture)
	{
		core::position2d<s32> center;

		transPos.X = renderTargetSize.Width * 9 / 10;
		transPos.Y = renderTargetSize.Height * 2 / 3;

		sourceRect.UpperLeftCorner.X = 0;
		sourceRect.UpperLeftCorner.Y = 0;
		sourceRect.LowerRightCorner.X = ratePlanTexture->getOriginalSize().Width;
		sourceRect.LowerRightCorner.Y = ratePlanTexture->getOriginalSize().Height;

		destRect.UpperLeftCorner = transPos;
		destRect.LowerRightCorner.X = transPos.X + renderTargetSize.Width / 20;
		destRect.LowerRightCorner.Y = transPos.Y + renderTargetSize.Width / 20;

		strTop = strTop.fromUtf16(TEXT_FLIGHT_SIM_NORTH);
		strBot = strBot.fromUtf16(TEXT_FLIGHT_SIM_SOUTH);
		strRight = strRight.fromUtf16(TEXT_FLIGHT_SIM_EAST);
		strLeft = strLeft.fromUtf16(TEXT_FLIGHT_SIM_WEST);

		driver->draw2DImage(ratePlanTexture, destRect, sourceRect, 0, 0, true);
		((video::COpenGLDriver*)driver)->setRenderStates2DMode(false, true, false);

		center = destRect.getCenter();

		DrawOutlineString(center.X - 7, destRect.UpperLeftCorner.Y - 5, strTop, 0.8f);
		DrawOutlineString(center.X - 7, destRect.LowerRightCorner.Y + 20, strBot, 0.8f);
		DrawOutlineString(destRect.UpperLeftCorner.X - 20, center.Y + 6, strLeft, 0.8f);
		DrawOutlineString(destRect.LowerRightCorner.X + 5, center.Y + 6, strRight, 0.8f);
		glEnable(GL_TEXTURE);
	}
	
}

void CGeoTerrainSceneNode::Draw2DLine( float x1, float y1, float x2, float y2 )
{
	glLineWidth( 2.0f );
	glBegin(GL_LINES);
	glColor3f( m_lineColor.getRed(), m_lineColor.getGreen(), m_lineColor.getBlue());
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void CGeoTerrainSceneNode::DrawOutlineString( float x, float y, QString& str, float scale )
{
	if(m_pFont)
		m_pFont->printOutlinedString( x, y, str, 0, 0, 0, m_lineColor, 0, scale);
}

void CGeoTerrainSceneNode::DrawRasterSymbol()
{
	Q_ASSERT(g_pGDMDataMgr->m_sOption.layerTextureMode==LTM_RASTER);
	
	RasterLayer* rasterlayer=g_pGDMDataMgr->m_pActiveRasterLayer;

	Q_ASSERT(rasterlayer!=NULL);

	Q_ASSERT(rasterlayer->GetVisible()==true);

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	QImage symbol=rasterlayer->GetRenderer()->GetColorRamp()->GetImage();

	QString symbolname=rasterlayer->GetRenderer()->GetColorRamp()->GetName();

	float max=rasterlayer->GetStatistics()._maxvalue;
	float min=rasterlayer->GetStatistics()._minvalue;

	char* strsymbolname=symbolname.toAscii().data();

	geo::video::ITexture* texture;

	texture=_RasterSymbolHash.value(symbolname);

	QString strTop, strBot, strRight, strLeft;
	core::rect<s32> sourceRect, destRect;
	core::position2d<s32> transPos;

	const core::dimension2d<s32>& renderTargetSize = driver->getScreenSize();

	transPos.X = renderTargetSize.Width * 14 / 15;
	transPos.Y = renderTargetSize.Height / 2;

	sourceRect.UpperLeftCorner.X = 0;
	sourceRect.UpperLeftCorner.Y = 0;
	sourceRect.LowerRightCorner.X = texture->getOriginalSize().Width;
	sourceRect.LowerRightCorner.Y = texture->getOriginalSize().Height;

	destRect.UpperLeftCorner = transPos;
	destRect.LowerRightCorner.X = transPos.X + renderTargetSize.Width / 50;
	destRect.LowerRightCorner.Y = renderTargetSize.Height - 100;

	strTop.sprintf("%f%s", max ,rasterlayer->GetUnitDescription());
	strBot.sprintf("%f%s", min,rasterlayer->GetUnitDescription());

	driver->draw2DImage(texture, destRect, sourceRect);

	glDisable(GL_TEXTURE);

	DrawOutlineString(destRect.LowerRightCorner.X + 5, destRect.UpperLeftCorner.Y + 10, strTop, 0.8f);
	DrawOutlineString(destRect.LowerRightCorner.X + 5, destRect.LowerRightCorner.Y, strBot, 0.8f);
	glEnable(GL_TEXTURE);
}

void CGeoTerrainSceneNode::enableLighting(bool enable/* =true */)
{
	if(enable)
		RenderBuffer->Material.Lighting=true;
	else
		RenderBuffer->Material.Lighting=false;
}

}//namespace scene
}//namespace geo

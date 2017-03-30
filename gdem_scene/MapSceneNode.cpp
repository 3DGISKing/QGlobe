#include <QtCore/QString>
#include <QtCore/QDir>
#include "MapSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "gdemclient.h"
#include "GDM_TextureMgr.h"
#include "config.h"
namespace geo
{
namespace scene
{
	// constructor
	MapSceneNode::MapSceneNode(ISceneNode* parent, ISceneManager* mgr, 
		s32 id, const core::vector3df& position,
		const core::vector3df& rotation, const core::vector3df& scale)
		: ISceneNode(parent, mgr, id, position)
	{
		QString workDir = gdm_GetWorkPath();

		QDir mapdir(workDir+"/maps");
		QString extension="*.coord";
		QStringList coordfiles;
		coordfiles = mapdir.entryList(QStringList(extension),QDir::Files | QDir::NoSymLinks);

		for(int i=0;i<coordfiles.size();i++)
		{
			MapInfo tmp(workDir+"/maps/"+coordfiles[i]);

			if(!tmp.isNull())
				m_MapList.push_back(tmp);
		}
		qSort(m_MapList.begin(),m_MapList.end());

		borderWidth=20;
		crossWidth=5;
		moveToTime=4000;

		material.Lighting = false;
		material.BackfaceCulling = false;
		material.FrontfaceCulling = true;
		m_pMap=NULL;
	}

	void MapSceneNode::drawCross()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		float vieworglongi=g_pGDMDataMgr->m_pCamera->m_location.m_dLongitude*gdm_RADTODEG;
		float vieworglat=g_pGDMDataMgr->m_pCamera->m_location.m_dLatitude*gdm_RADTODEG;

		int x=longitudeToScreenX(vieworglongi);
		int y=latitudeToScreenY(vieworglat);

		core::position2d<s32> p1(x-crossWidth,y);
		core::position2d<s32> p2(x+crossWidth,y);

		core::position2d<s32> p3(x,y-crossWidth);
		core::position2d<s32> p4(x,y+crossWidth);
		
		video::SColor clr=video::SColor(255, 255,0, 0);

		driver->draw2DLine(p1,p2,clr);
		driver->draw2DLine(p3,p4,clr);
	}

	void MapSceneNode::setViewExtent()
	{
		viewExtent.m_cMinPt.m_tX=g_pGDMDataMgr->m_pCamera->m_dMinLong1*gdm_RADTODEG;
		viewExtent.m_cMinPt.m_tY=g_pGDMDataMgr->m_pCamera->m_dMinLati*gdm_RADTODEG;

		viewExtent.m_cMaxPt.m_tX=g_pGDMDataMgr->m_pCamera->m_dMaxLong1*gdm_RADTODEG;
		viewExtent.m_cMaxPt.m_tY=g_pGDMDataMgr->m_pCamera->m_dMaxLati*gdm_RADTODEG;
	}

	void MapSceneNode::drawMapAndOutline()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		const core::dimension2d<s32>& renderTargetSize = driver->getScreenSize();

		core::rect<s32> sourceRect, destRect;

		sourceRect.UpperLeftCorner.X = 0;
		sourceRect.UpperLeftCorner.Y = 0;
		sourceRect.LowerRightCorner.X = m_pMap->getOriginalSize().Width;
		sourceRect.LowerRightCorner.Y = m_pMap->getOriginalSize().Height;

		destRect.UpperLeftCorner.X =borderWidth;
		destRect.UpperLeftCorner.Y =borderWidth;
		destRect.LowerRightCorner.X =borderWidth+m_pMap->getOriginalSize().Width*iconScale;
		destRect.LowerRightCorner.Y = borderWidth+m_pMap->getOriginalSize().Height*iconScale;


		const video::SColor temp[4] =
		{
			0x00FFFFFF,
			0xFFFFFFFF,
			0xFFFFFFFF,
			0xFFFFFFFF
		};
		driver->draw2DImage(m_pMap, destRect, sourceRect,0,temp,true);
		
		core::position2d<s32> upperleft(borderWidth,borderWidth);
		core::position2d<s32> lowerleft(borderWidth,destRect.LowerRightCorner.Y);

		core::position2d<s32> lowerright(destRect.LowerRightCorner.X,destRect.LowerRightCorner.Y);
		core::position2d<s32> upperright(destRect.LowerRightCorner.X,borderWidth);

		driver->draw2DLine(upperleft,lowerleft);
		driver->draw2DLine(lowerleft,lowerright);
		driver->draw2DLine(lowerright,upperright);
		driver->draw2DLine(upperright,upperleft);
	}

	int MapSceneNode::longitudeToScreenX(double longi)
	{
		return borderWidth+((longi-mapLeft)/(mapRight-mapLeft))*m_pMap->getOriginalSize().Width*iconScale;
	}

	int MapSceneNode::latitudeToScreenY(double lat)
	{
		double y=1-(lat-mapBottom)/(mapTop-mapBottom);
		y=borderWidth+y*m_pMap->getOriginalSize().Height*iconScale;
		return y;
	}

	double MapSceneNode::screenXToLongitude(int x)
	{
		double longi=x-borderWidth;
		longi=longi/(m_pMap->getOriginalSize().Width*iconScale);
		longi=mapLeft+longi*(mapRight-mapLeft);
		return longi;
	}

	double MapSceneNode::screenYToLatitude(int y)
	{
		double  lat=y-borderWidth;
		lat=lat/(m_pMap->getOriginalSize().Height*iconScale);
		lat=mapTop-lat*(mapTop-mapBottom);
		return lat;
	}

	void MapSceneNode::drawViewRect()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		int screenminx=longitudeToScreenX(viewExtentInMap.m_cMinPt.m_tX);
		int screenmaxx=longitudeToScreenX(viewExtentInMap.m_cMaxPt.m_tX);
		int screenmaxy=latitudeToScreenY(viewExtentInMap.m_cMinPt.m_tY);
		int screenminy=latitudeToScreenY(viewExtentInMap.m_cMaxPt.m_tY);

		if(screenmaxx-screenminx<crossWidth) return;
		if(screenmaxy-screenminy<crossWidth) return;

		int gap=3;
		if(screenminx<=borderWidth+gap) 
			screenminx=borderWidth+gap;
		if(screenmaxx>=borderWidth+m_pMap->getOriginalSize().Width*iconScale-gap)
			screenmaxx=borderWidth+m_pMap->getOriginalSize().Width*iconScale-gap;

		if(screenminy<=borderWidth+gap) 
			screenminy=borderWidth+gap;
		if(screenmaxy>=borderWidth+m_pMap->getOriginalSize().Height*iconScale-gap)
			screenmaxy=borderWidth+m_pMap->getOriginalSize().Height*iconScale-gap;

		video::SColor clr=video::SColor(255, 255,0, 0);

		core::position2d<s32> upperleft(screenminx,screenminy);
		core::position2d<s32> lowerleft(screenminx,screenmaxy);

		core::position2d<s32> lowerright(screenmaxx,screenmaxy);
		core::position2d<s32> upperright(screenmaxx,screenminy);

		driver->draw2DLine(upperleft,lowerleft,clr);
		driver->draw2DLine(lowerleft,lowerright,clr);
		driver->draw2DLine(lowerright,upperright,clr);
		driver->draw2DLine(upperright,upperleft,clr);
	}

	void MapSceneNode::render()
	{
		Config *conf=g_app->GetConfig();
		if(!conf->m_IsShowOverviewMap) return;

#define SCALE 0.3
		setViewExtent();
		
		for(int i=0;i<m_MapList.size();i++)
		{
			if(g_pGDMDataMgr->m_pCamera->m_location.m_dDist>m_MapList[i].getMaxHeight())
				continue;
			if(!m_MapList[i].isIntersected(viewExtent))
				continue;
			if(!m_MapList[i].isContain(g_pGDMDataMgr->m_pCamera->m_location.m_dLongitude*gdm_RADTODEG,
												g_pGDMDataMgr->m_pCamera->m_location.m_dLatitude*gdm_RADTODEG))
												continue;

			QString path;
			QString workDir = gdm_GetWorkPath();

			path=workDir+"/maps/"+m_MapList[i].getMapName()+".png";

			g_TextureListMutex.lock();
			m_pMap=SceneManager->getVideoDriver()->getTexture(path.toUtf8());
			g_TextureListMutex.unlock();

			mapLeft=m_MapList[i].left();
			mapRight=m_MapList[i].right();
			mapTop=m_MapList[i].top();
			mapBottom=m_MapList[i].bottom();
			viewExtentInMap.m_cMinPt.m_tX=mapLeft;
			viewExtentInMap.m_cMaxPt.m_tX=mapRight;
			viewExtentInMap.m_cMinPt.m_tY=mapBottom;
			viewExtentInMap.m_cMaxPt.m_tY=mapTop;

			viewExtentInMap.clipAgainst(viewExtent);
			moveToHeight=m_MapList[i].getMaxHeight()/10;
			break;
		}

		if(!m_pMap) return;
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		double scalew=driver->getScreenSize().Width*SCALE/m_pMap->getOriginalSize().Width;
		double scaleh=driver->getScreenSize().Height*SCALE/m_pMap->getOriginalSize().Height;
	
		iconScale=qMin(scalew,scaleh);

		driver->setMaterial(material);

		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		drawMapAndOutline();
		drawCross();
		drawViewRect();
	}

	void MapSceneNode::mousePressEvent(CursorState *event)
	{
		Config *conf=g_app->GetConfig();
		if(!conf->m_IsShowOverviewMap) 
			return;
		if(!m_pMap) return;

		if(event->x>borderWidth && event->x<borderWidth+m_pMap->getOriginalSize().Width*iconScale)
		if(event->y>borderWidth && event->y<borderWidth+m_pMap->getOriginalSize().Height*iconScale)
		{
			double longi=screenXToLongitude(event->x);
			double lat=screenYToLatitude(event->y);

			GDM_CAMERA_INFO cameraInfo;	
			memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
			cameraInfo.m_loc.m_dDist=moveToHeight;
			cameraInfo.m_loc.m_dLongitude=longi*gdm_DEGTORAD;
			cameraInfo.m_loc.m_dLatitude=lat*gdm_DEGTORAD;

			g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, moveToTime);
		}
	}

	MapSceneNode::~MapSceneNode(void)
	{
		

	}

	//! returns the axis aligned bounding box of this node
	const core::aabbox3d<f32>& MapSceneNode::getBoundingBox() const
	{
		return Box;
	}

	void MapSceneNode::OnRegisterSceneNode()
	{
		SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
	}
}
}


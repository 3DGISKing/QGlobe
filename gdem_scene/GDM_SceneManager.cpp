#include <QGLWidget>
#include "os.h"

#include "GDM_SceneManager.h"
#include "GeoFont.h"
#include "GDM_Define.h"
#include "GDM_DataMgr.h"
#include "GDM_TextureMgr.h"
#include "GDM_TextureIDCache.h"
#include "GeoTerrainSceneNode.h"
#include "AtmosphereSceneNode.h"
#include "GeoContourSceneNode.h"
#include "GeoNameSceneNode.h"
#include "GeoGridSceneNode.h"
#include "GeoGISSceneNode.h"
#include "GeoShapeSceneNode.h"
#include "SampleMeshSceneNode.h"
#include "CSceneManager.h"
#include "GIS_TextureMgr.h"
#include "GIS_Doc.h"
#include "GIS_TextureIDCache.h"
#include "SimSceneNode.h"
#include "MapSceneNode.h"
#include "Geo3dObjectSceneNode.h"
#include "RasterTextureMgr.h"
#include "RasterTextureIDCache.h"
#include "GeoLightSceneNode.h"

using namespace geo;
using namespace video;
using namespace scene;

extern void gdm_MakeRenderGeoNameData();
extern void gdm_PreparePlacemarkData();
extern void gdm_PrepareLinkLenAreaName();//for custom

GDM_SceneManager* g_SceneManager=NULL;

GDM_SceneManager * gdm_GetSceneManager()
{
	return g_SceneManager;
}

GDM_SceneManager::GDM_SceneManager(void)
{
	g_SceneManager=this;
	m_pDevice = NULL;
	m_pDriver = NULL;

	m_pCamera = NULL;
	m_pStarSkyBox = NULL;
	m_pBlueSkyBox = NULL;
	m_pLight = NULL;
	m_pAtmosphere = NULL;

	m_pGeoTerrainSceneNode = NULL;
	m_pGeoNameNode = NULL;
	m_pGeoShapeNode = NULL;
	m_pGeoGridNode = NULL;
	m_pGeoContourNode = NULL;
	m_pGeoGISNode = NULL;
	m_pGeoSimNode = NULL;
	m_pSimMgr = NULL;
	m_pSampleMeshNode=NULL;

	// create texturemgr
	m_pTextureMgr			= new CGDM_TextureMgr;
	m_pGisTextureMgr		= new CGIS_TextureMgr;

	// create texture id cache
	m_pTextureIdCache		= new CGDM_TextureIDCache;
	m_pGisTextureIdCache	= new CGIS_TextureIDCache;

	m_pRasterTextureMgr     = new RasterTextureMgr;
	m_pRasterTextureIdCache = new RasterTextureIDCache;

	m_pGeoMapNode =NULL;
	m_pGeo3dObjectNode=NULL;

	m_Backcolor.set(255,255,0,0);
	m_LightColor.set(1.0f,1.0f,1.0f,1.0f);
	m_AmbientLightColor.set(1.0f,0.0f,0.0f,0.0f);


	//if height of camera is larger than this, _StarSkyBox is rendered.
	//if not , _BlueSkyBox is rendered.

	m_BlueSkyHeight=10000; //10km
	m_EnableLighting=true;
}

GDM_SceneManager::~GDM_SceneManager(void)
{
	if(m_pGisTextureIdCache)
		delete m_pGisTextureIdCache;
	if(m_pTextureIdCache)
		delete m_pTextureIdCache;

	if(m_pTextureMgr)
		delete m_pTextureMgr;
	if(m_pGisTextureMgr)
		delete m_pGisTextureMgr;

	if(m_pRasterTextureMgr)
		delete m_pRasterTextureMgr;

	if(m_pRasterTextureIdCache)
		delete m_pRasterTextureIdCache;

	unregisterRenderEngine();
}

void GDM_SceneManager::Initialize()
{
	gdm_GetDataMgr()->SetTextureIDCache(m_pTextureIdCache);
}

bool GDM_SceneManager::registerRenderEngine(QGLWidget* widget)
{
	geo::SGeoCreationParameters params;

	params.DriverType = geo::video::EDT_OPENGL;;
	params.WindowId = (void*)widget->winId();
	params.WindowSize.Width = widget->width();
	params.WindowSize.Height = widget->height();
	//params.AntiAlias = true;

	m_pDevice = geo::createDeviceEx( params );

	if(m_pDevice == 0)
		return false;

	m_pSceneMgr = (CSceneManager*)m_pDevice->getSceneManager();

	m_pDriver=m_pDevice->getVideoDriver();
	
	gdm_GetTextureMgr()->SetVideoDriver(m_pDriver);
	gdm_GetGISTextureMgr()->SetVideoDriver(m_pDriver);

	gdm_GetRasterTextureMgr()->SetVideoDriver(m_pDriver);
	
	return true;
}

void GDM_SceneManager::unregisterRenderEngine()
{
	if(m_pDevice)
	{
		m_pDevice->closeDevice();
		m_pDevice->run();
		m_pDevice->drop();

		m_pDevice = 0;
	}
}

//Terrain, Shape, SkyBox등 SceneGraph를 구성한다.
bool GDM_SceneManager::prepareSceneGraph()
{
	if(!m_pSceneMgr)
		return false;

	setupEnv();

	addTerrainNode();
	if (m_pGeoTerrainSceneNode)
	{
		QString path;

		QString workDir = gdm_GetWorkPath();

		path = workDir + QString("/res/scene/earth.jpg");
		m_pGeoTerrainSceneNode->setEarthTexture(m_pDevice->getVideoDriver()->getTexture(path.toUtf8()));

		path = workDir + QString("/res/scene/nodata.jpg");
		m_pGeoTerrainSceneNode->setRasterNoDataTexture(m_pDevice->getVideoDriver()->getTexture(path.toUtf8()));

		path = workDir + QString("/res/scene/height.jpg");
		m_pGeoTerrainSceneNode->setHeightTexture(m_pDevice->getVideoDriver()->getTexture(path.toUtf8()));

		path = workDir + QString("/res/scene/rate_angle.jpg");
		m_pGeoTerrainSceneNode->setRateAngleTexture(m_pDevice->getVideoDriver()->getTexture(path.toUtf8()));

		path = workDir + QString("/res/scene/rate_plan.png");
		m_pGeoTerrainSceneNode->setRatePlanTexture(m_pDevice->getVideoDriver()->getTexture(path.toUtf8()));
	}
	
   addGeoShapeNode();

   addGeo3dObjectNode();

   addGeoNameNode();

   addAtmosphereNode();

   addGeoGridNode();

   addGeoContourNode();

   addGeoSimSceneNode();

   addGISRenderNode ();

   m_pDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true); // RYM. for render error

#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
	CGIS_TextureMgr::createTransparentTexture(m_pDriver);
#endif

	m_pGeoMapNode = new MapSceneNode(m_pSceneMgr, m_pSceneMgr, -1);
	m_pGeoMapNode->drop();

	addSampleMeshNode();
	return true;
}

void GDM_SceneManager::setupEnv()
{
	// create skybox and skydome
	m_pDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	QString path[6];
    QString workDir = gdm_GetWorkPath();

	path[0] = workDir + QString("/res/scene/sky_up.jpg");
	path[1] = workDir + QString("/res/scene/sky_dn.jpg");
	path[2] = workDir + QString("/res/scene/sky_lf.jpg");
	path[3] = workDir + QString("/res/scene/sky_rt.jpg");
	path[4] = workDir + QString("/res/scene/sky_ft.jpg");
	path[5] = workDir + QString("/res/scene/sky_bk.jpg");

	m_pStarSkyBox = m_pSceneMgr->addSkyBoxSceneNode(
                m_pDriver->getTexture(path[0].toUtf8()),
                m_pDriver->getTexture(path[1].toUtf8()),
                m_pDriver->getTexture(path[2].toUtf8()),
                m_pDriver->getTexture(path[3].toUtf8()),
                m_pDriver->getTexture(path[4].toUtf8()),
                m_pDriver->getTexture(path[5].toUtf8()));

	path[0] = workDir + QString("/res/scene/bluesky_up.jpg");
	path[1] = workDir + QString("/res/scene/bluesky_dn.jpg");
	path[2] = workDir + QString("/res/scene/bluesky_lf.jpg");
	path[3] = workDir + QString("/res/scene/bluesky_rt.jpg");
	path[4] = workDir + QString("/res/scene/bluesky_ft.jpg");
	path[5] = workDir + QString("/res/scene/bluesky_bk.jpg");

	m_pBlueSkyBox = m_pSceneMgr->addSkyBoxSceneNode(
		m_pDriver->getTexture(path[0].toUtf8()),
		m_pDriver->getTexture(path[1].toUtf8()),
		m_pDriver->getTexture(path[2].toUtf8()),
		m_pDriver->getTexture(path[3].toUtf8()),
		m_pDriver->getTexture(path[4].toUtf8()),
		m_pDriver->getTexture(path[5].toUtf8()));

    //scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("res/scene/skydome.jpg"),16,8,0.95f,2.0f);

	// add camera
	m_pCamera = m_pSceneMgr->addCameraSceneNode();

	m_pCamera->setPosition(core::vector3df(0,0,2000));
	m_pCamera->setTarget(core::vector3df(0,0,0));
	m_pCamera->setFarValue(EARTH_RADIUS * 100);

	m_pLight=new CGeoLightSceneNode(m_pSceneMgr,m_pSceneMgr,0,core::vector3df(0.0f, 0.0f,0.0f),
		m_LightColor, EARTH_RADIUS * 10);

	m_pLight->drop();

	m_pSceneMgr->setAmbientLight(m_AmbientLightColor);
}

void GDM_SceneManager::setCameraPos(geo::core::vector3df& absPos, geo::core::vector3df& pos, geo::core::vector3df& target, geo::core::vector3df& up)
{
	if(m_pCamera)
	{
		m_pCamera->setPosition(pos);
		m_pCamera->setTarget(target);
		m_pCamera->setUpVector(up);
	}

	if(m_pAtmosphere)
		m_pAtmosphere->setPosition(absPos);
}

void GDM_SceneManager::setViewFrustum(geo::f64 nearValue, geo::f64 farValue, geo::f64 fov, geo::f64 aspect)
{
	if(m_pCamera)
	{
		m_pCamera->setNearValue(nearValue);

		if(g_pGDMDataMgr->CameraHeight()<m_BlueSkyHeight)
			m_pCamera->setFarValue(farValue);
		else
			m_pCamera->setFarValue(EARTH_RADIUS*100);

		m_pCamera->setFOV(fov);
		m_pCamera->setAspectRatio(aspect);
	}
}

void GDM_SceneManager::onBeforeRender()
{
	if(g_pGDMDataMgr->CameraHeight()<m_BlueSkyHeight)
	{
		m_pBlueSkyBox->setVisible(true);

		m_pStarSkyBox->setVisible(false);
		m_pAtmosphere->setVisible(false);
	}
	else
	{
		if(m_pBlueSkyBox)
			m_pBlueSkyBox->setVisible(false);

		if(m_pStarSkyBox)
			m_pStarSkyBox->setVisible(true);

		if(m_pAtmosphere)
			m_pAtmosphere->setVisible(true);
	}

	gdm_PreparePlacemarkData();
	gdm_MakeRenderGeoNameData();
	gdm_PrepareLinkLenAreaName();//for custom

	if(gdm_GetGISDoc()->IsModified())
		gdm_GetGISTextureIDCache()->Clear();
}

void GDM_SceneManager::onAfterRender()
{
	m_pTextureMgr->DelTextureList();
	m_pGisTextureMgr->DelTextureList();
	m_pRasterTextureMgr->DelTextureList();
}

void GDM_SceneManager::addTerrainNode()
{
	m_pGeoTerrainSceneNode = new CGeoTerrainSceneNode(m_pSceneMgr, m_pSceneMgr,  -1);
	m_pGeoTerrainSceneNode->drop();
}

void GDM_SceneManager::addGeoShapeNode()
{
	m_pGeoShapeNode = new CGeoShapeSceneNode(m_pSceneMgr, m_pSceneMgr,  -1);
	m_pGeoShapeNode->drop();
}

void GDM_SceneManager::addSampleMeshNode()
{
	m_pSampleMeshNode = new CSampleMeshSceneNode(m_pSceneMgr, m_pSceneMgr,  -1);
	m_pSampleMeshNode->drop();
}

void GDM_SceneManager::	addGeoSimSceneNode()
{
	m_pGeoSimNode = new CSimSceneNode(m_pSceneMgr, m_pSceneMgr, -1);
	m_pGeoSimNode->drop();
}

void GDM_SceneManager::addGeoContourNode()
{
	m_pGeoContourNode = new CGeoContourSceneNode(m_pSceneMgr, m_pSceneMgr,  -1);
	m_pGeoContourNode->drop();
}

void GDM_SceneManager::addGeoNameNode()
{
	m_pGeoNameNode = new CGeoNameSceneNode(m_pSceneMgr, m_pSceneMgr, -1);
	m_pGeoNameNode->drop();
}

void GDM_SceneManager::addGeo3dObjectNode()
{
	m_pGeo3dObjectNode = new CGeo3dObjectSceneNode(m_pSceneMgr, m_pSceneMgr, -1);
	m_pGeo3dObjectNode->drop();
}

void GDM_SceneManager::addGeoGridNode()
{
	m_pGeoGridNode = new CGeoGridSceneNode(m_pSceneMgr, m_pSceneMgr, -1);
	m_pGeoGridNode->drop();
}

void GDM_SceneManager::addAtmosphereNode()
{
	m_pAtmosphere = new AtmosphereSceneNode(EARTH_RADIUS * 1.005f, EARTH_RADIUS*0.02f, 50, 50, m_pSceneMgr, m_pSceneMgr,  -1);
	m_pAtmosphere->setLightDirection(m_pLight->getAbsolutePosition().normalize());
	m_pAtmosphere->drop();
}

void GDM_SceneManager::addGISRenderNode()
{
	m_pGeoGISNode = new CGeoGISSceneNode(m_pSceneMgr, m_pSceneMgr, -1);
	m_pGeoGISNode->drop();
}

void GDM_SceneManager::setFont(QString szKorFileName, QString szEngFileName, unsigned int flag)
{
	if(m_pGeoNameNode)
		m_pGeoNameNode->changeFont(szKorFileName, szEngFileName, flag);return;

	if(m_pGeoSimNode && m_pGeoNameNode)
		m_pGeoSimNode->SetFont(m_pGeoNameNode->GetGeoFont());

	if(m_pGeoTerrainSceneNode && m_pGeoNameNode)
		m_pGeoTerrainSceneNode->SetFont(m_pGeoNameNode->GetGeoFont());
}
	
void GDM_SceneManager::setSimMgr(CGDM_SimMgr *pMgr)
{
	m_pSimMgr = pMgr;
	if(m_pGeoSimNode)
		m_pGeoSimNode->SetSimMgr(pMgr);
}

unsigned long gdm_GetTickTimer()
{
	return geo::os::Timer::getRealTime();
}

bool GDM_SceneManager::renderScene()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	m_pDriver->beginScene( true, true, m_Backcolor);

	m_pDevice->getSceneManager()->drawAll();

	m_pDriver->endScene();

	glPopAttrib();

   return true;
}

void GDM_SceneManager::setGlobalLightPosition(double longi,double lat,double height)
{
	GDM_LOCATION loc;

	loc.m_dLongitude=-gdm_DEGTORAD*longi;
	loc.m_dLatitude=gdm_DEGTORAD*lat;
	loc.m_dDist=height*1000;

	CGDM_Point3DF tmp;

	gdm_GetPosition(&loc,&tmp);

	m_pLight->SetLightPosition(vector3df(tmp.m_tX,tmp.m_tY,tmp.m_tZ));
}

void GDM_SceneManager::setGlobalLightColor(QColor c)
{
	SLight& s=m_pLight->getLightData();

	s.DiffuseColor.set(c.alpha()/255.0f,c.red()/255.0f,c.green()/255.0f,c.blue()/255.0f);
}

void GDM_SceneManager::setAmbientColor(QColor c)
{
	m_AmbientLightColor.set(c.alpha()/255.0f,c.red()/255.0f,c.green()/255.0f,c.blue()/255.0f);

	m_pSceneMgr->setAmbientLight(m_AmbientLightColor);
}

void GDM_SceneManager::setLightAttenuation(float c,float l,float q)
{
	m_pLight->SetLightAttenuation(c,l,q);
}

void GDM_SceneManager::enableLighting(bool enable/* =true */)
{
	m_EnableLighting=enable;
	m_pGeoTerrainSceneNode->enableLighting(enable);
	m_pSampleMeshNode->EnableLighting(enable);
	m_pGeo3dObjectNode->EnableLighting(enable);
}
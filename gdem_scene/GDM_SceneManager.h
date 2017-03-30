#ifndef __GDM_SCENEMANAGER_H__
#define __GDM_SCENEMANAGER_H__

#include <QtGui/QColor>

#include "geoEngine.h"
#include "gdem_scene_global.h"

namespace geo
{
	class GeoDevice;
	namespace scene
	{
		class ISceneManager;		
		class CGeoShapeSceneNode;
		class CGeoTerrainSceneNode;
		class AtmosphereSceneNode;
		class CGeoGridSceneNode;
		class CGeoContourSceneNode;
		class CGeoGISSceneNode;
		class CGeoNameSceneNode;
		class CSceneManager;
		class CSimSceneNode;
		class CSampleMeshSceneNode;
		class MapSceneNode;
		class CGeo3dObjectSceneNode;
		class CGeoLightSceneNode;
	}
	namespace video
	{
		class IVideoDriver;
	}
}

class QGLWidget;
class CGDM_TextureMgr;
class CGIS_TextureMgr;
class CGDM_TextureIDCache;
class CGIS_TextureIDCache;
class CGDM_SimMgr;
class RasterTextureIDCache;
class RasterTextureMgr;

class GDEM_SCENE_EXPORT GDM_SceneManager
{
public:
	GDM_SceneManager(void);
	virtual ~GDM_SceneManager(void);

	void    Initialize();
	bool    registerRenderEngine(QGLWidget* widget);
	bool    prepareSceneGraph();
	void	onBeforeRender();
	void	onAfterRender();
	bool	renderScene();

	inline  geo::GeoDevice* RenderDevice()          { return m_pDevice; }
	inline  geo::video::IVideoDriver* VideoDriver() { return m_pDriver; }

	void    setFont(QString szKorFileName, QString szEngFileName, unsigned int flag);
	void    setCameraPos(geo::core::vector3df& absPos, geo::core::vector3df& pos, geo::core::vector3df& target, geo::core::vector3df& up);
	void    setViewFrustum(geo::f64 nearValue, geo::f64 farValue, geo::f64 fov, geo::f64 aspect);
	void    setSimMgr(CGDM_SimMgr *pMgr);

	void    setGlobalLightPosition(double longi,double lat ,double height);
	void    setGlobalLightColor(QColor c);
	void    setAmbientColor(QColor c);

	void    setLightAttenuation(float c,float l,float q);
	void    enableLighting(bool enable=true);
public:
	geo::scene::CSampleMeshSceneNode*	    m_pSampleMeshNode;
	geo::scene::CGeo3dObjectSceneNode*	    m_pGeo3dObjectNode;
	geo::scene::MapSceneNode*               m_pGeoMapNode;

	CGDM_TextureMgr*			            m_pTextureMgr;			// for image render
	CGDM_TextureIDCache*		            m_pTextureIdCache;		// for image render

	CGIS_TextureMgr*			            m_pGisTextureMgr;		// for gis object render
	CGIS_TextureIDCache*	            	m_pGisTextureIdCache;	// for gis object render

	RasterTextureMgr*		                m_pRasterTextureMgr;	
	RasterTextureIDCache*		            m_pRasterTextureIdCache;

	bool                                    m_EnableLighting;
private:
	void unregisterRenderEngine();
	void setupEnv();
	void addTerrainNode();
	void addGeoShapeNode();
	void addGeoNameNode();
	void addGeo3dObjectNode();
	void addAtmosphereNode();
    void addGeoGridNode();
	void addGeoContourNode();
	void addGISRenderNode();
	void addGeoSimSceneNode();
	void addSampleMeshNode();

private:
	geo::GeoDevice*			            m_pDevice;
	geo::video::IVideoDriver*           m_pDriver;
	geo::scene::CSceneManager*          m_pSceneMgr;

	CGDM_SimMgr*			            m_pSimMgr;

	geo::video::SColor                  m_Backcolor;
	geo::video::SColorf                 m_LightColor;
	geo::video::SColorf                 m_AmbientLightColor;

	double                              m_BlueSkyHeight;

	geo::scene::CGeoTerrainSceneNode*	m_pGeoTerrainSceneNode;

	geo::scene::CGeoNameSceneNode*		m_pGeoNameNode; 
	geo::scene::CGeoShapeSceneNode*		m_pGeoShapeNode;
	geo::scene::CGeoGridSceneNode*		m_pGeoGridNode;
	geo::scene::CGeoContourSceneNode*   m_pGeoContourNode;
	geo::scene::CGeoGISSceneNode*		m_pGeoGISNode; 
	geo::scene::CSimSceneNode*			m_pGeoSimNode;

	geo::scene::ICameraSceneNode*	    m_pCamera;     //camera
	geo::scene::CGeoLightSceneNode*	    m_pLight;      // light
	geo::scene::ISceneNode*			    m_pStarSkyBox; //sky
	geo::scene::ISceneNode*			    m_pBlueSkyBox; //sky
	geo::scene::AtmosphereSceneNode*	m_pAtmosphere; //atmosphere effect node
};

GDM_SceneManager * gdm_GetSceneManager();

#endif

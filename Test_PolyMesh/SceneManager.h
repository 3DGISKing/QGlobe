#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "geoEngine.h"

namespace geo{
	class GeoDevice;
	namespace scene{
		class ISceneManager;		
		class CSceneManager;
	}
	namespace video{
		class IVideoDriver;
	}
}

class QGLWidget;

class SceneManager
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);

public:
	void Initialize();
	bool registerRenderEngine(QGLWidget* widget);
	bool prepareSceneGraph();
	void OnBeforeRender();
	void OnAfterRender();
	void OnEvent(geo::SEvent& event);

	bool SetOverlayTexture(QImage* qimage);

public:
	inline geo::GeoDevice* RenderDevice() { return _Device; }
	inline geo::scene::ISceneManager* getSceneManager()
	{
		if(_Device == 0)
			return 0;
		return _Device->getSceneManager();
	}

	inline geo::video::IVideoDriver* VideoDriver()
	{
		if(_Device == 0)
			return 0;
		return _Device->getVideoDriver();
	}

	void setCameraPos(geo::core::vector3df& absPos, geo::core::vector3df& pos, geo::core::vector3df& target, geo::core::vector3df& up);
	void setViewFrustum(geo::f64 nearValue, geo::f64 farValue, geo::f64 fov, geo::f64 aspect);

private:
	void unregisterRenderEngine();

	void setupEnv();
	
protected:
    geo::scene::ICameraSceneNode*	_Camera; //camera
    geo::scene::ILightSceneNode*	_Light;	// light
	geo::GeoDevice*			_Device;
	geo::scene::CSceneManager*	_smgr;

	geo::scene::ISceneNode* _Earth;

	geo::video::ITexture*	_MainTexture;
	geo::video::ITexture*	_OverTexture;
};

#endif //__SCENEMANAGER_H__

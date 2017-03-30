#include <QGLWidget>
#include <QtCore>
#include "os.h"

#include "SceneManager.h"
#include "CSceneManager.h"

using namespace geo;
using namespace video;
using namespace scene;


SceneManager::SceneManager(void)
{
	_Device = NULL;
	_Camera = NULL;
	_Light = NULL;
	_smgr = NULL;
	_Earth = NULL;
	_OverTexture = NULL;
}

SceneManager::~SceneManager(void)
{
	unregisterRenderEngine();
}

void SceneManager::Initialize()
{
}

bool SceneManager::registerRenderEngine(QGLWidget* widget)
{
	// Don't initialize more than once!
	if ( _Device != 0 || widget == 0) 
		return false;

	geo::SGeoCreationParameters params;
	geo::video::E_DRIVER_TYPE driverType = geo::video::EDT_OPENGL;

	params.DriverType = driverType;
	params.WindowId = (void*)widget->winId();
	params.WindowSize.Width = widget->width();
	params.WindowSize.Height = widget->height();

	_Device = geo::createDeviceEx( params );
	if(_Device == 0)
		return false;

	_smgr = (CSceneManager*)_Device->getSceneManager();

	return true;
}

void SceneManager::unregisterRenderEngine()
{
	if(_Device)
	{
		_Device->closeDevice();
		_Device->run();
		_Device->drop();

		_Device = 0;
	}
}

void ConvertToCharFromQString(QString &str, char* buffer)
{
	char *data;
	QByteArray byte = str.toAscii();
	data = byte.data();
	int len = strlen(data);

	memcpy(buffer, data, len + 1);
}

//Terrain, Shape, SkyBox등 SceneGraph를 구성한다.
bool SceneManager::prepareSceneGraph()
{
	if(!_smgr)
		return false;

	setupEnv();

	video::IVideoDriver* driver = VideoDriver();
	
	scene::IAnimatedMesh* earthMesh = _smgr->addSphereMesh("earth_mesh", 150, 64, 64);

	if (earthMesh)
	{
		//perform various task with the mesh manipulator
		scene::IMeshManipulator *manipulator = _smgr->getMeshManipulator();

		// create mesh copy with tangent informations from original earth.x mesh
		scene::IMesh* tangentSphereMesh =
			manipulator->createMeshWith2TCoords(earthMesh->getMesh(0));

		scene::ISceneNode *earth = _smgr->addMeshSceneNode(tangentSphereMesh);

		char name[255];
		QString path;
                QString workDir = QCoreApplication::applicationDirPath();
#ifdef Q_WS_MAC
                workDir += QString("/../../../");
                QDir dir(workDir);
                workDir = dir.absolutePath();
#endif
		path = workDir + QString("/res/earth.jpg");
		ConvertToCharFromQString(path, name);

		_MainTexture = driver->getTexture(name);

		earth->setMaterialTexture(0, _MainTexture);
		earth->setMaterialType(video::EMT_OVERLAY_TEXTURE_BLEND);

		// add rotation animator
		scene::ISceneNodeAnimator* anim =
			_smgr->createRotationAnimator(core::vector3df(0,0.1f,0));
		earth->addAnimator(anim);
		anim->drop();

		_Earth = earth;
	}

	return true;
}

void SceneManager::setupEnv()
{
	video::IVideoDriver* driver = _Device->getVideoDriver();

	// add camera
	_Camera = _smgr->addCameraSceneNode();

	_Camera->setPosition(core::vector3df(0,100,-300));
	_Camera->setTarget(core::vector3df(0,0,0));
	_Camera->setFarValue(1000);
	_Camera->updateAbsolutePosition();

	// add Light
	_Light = _smgr->addLightSceneNode(0, core::vector3df(300,300,-500));
	_smgr->setAmbientLight(video::SColorf(0.6f, 0.6f, 0.6f, 0.6f));
}

//카메라정보를 갱신해준다.
//Input : 카메라위치, 회전각정보
void SceneManager::setCameraPos(geo::core::vector3df& absPos, geo::core::vector3df& pos, geo::core::vector3df& target, geo::core::vector3df& up)
{
	video::IVideoDriver* driver = _Device->getVideoDriver();

	scene::ICameraSceneNode* camera = _smgr->getActiveCamera();
	if(camera)
	{
		camera->setPosition(pos);
		camera->setTarget(target);
		camera->setUpVector(up);

		camera->updateAbsolutePosition();
	}
}

void SceneManager::setViewFrustum(geo::f64 nearValue, geo::f64 farValue, geo::f64 fov, geo::f64 aspect)
{
	video::IVideoDriver* driver = _Device->getVideoDriver();

	scene::ICameraSceneNode* camera = _smgr->getActiveCamera();
	if(camera)
	{
		camera->setNearValue(nearValue);
//		camera->setFarValue(farValue);
		camera->setFOV(fov);
		camera->setAspectRatio(aspect);
	}
}

void SceneManager::OnBeforeRender()
{
	//gdm_PreparePlacemarkData();
	//gdm_MakeRenderGeoNameData();
	if(_Earth)
	{
		_Earth->OnAnimate(_Device->getTimer()->getTime());
	}
}

void SceneManager::OnAfterRender()
{
}

void SceneManager::OnEvent(geo::SEvent& event)
{
	if(event.EventType == geo::EET_MOUSE_INPUT_EVENT)
	{
		scene::ICameraSceneNode* camera = _smgr->getActiveCamera();		

	}
}

bool SceneManager::SetOverlayTexture(QImage* qimage)
{
	video::IVideoDriver* driver = VideoDriver();
	if(_OverTexture)
		driver->removeTexture(_OverTexture);

	video::IImage *image = driver->createImageFromData(video::ECF_A8R8G8B8, core::dimension2di(qimage->width(), qimage->height()), qimage->bits(), true, true);
	if(!image)
		return false;

	_OverTexture = driver->addTexture("overlay_texture_0", image);

	_Earth->setMaterialTexture(0, _OverTexture);
	_Earth->setMaterialTexture(1, _MainTexture);
	_Earth->setMaterialType(video::EMT_OVERLAY_TEXTURE_BLEND);

	return true;

}

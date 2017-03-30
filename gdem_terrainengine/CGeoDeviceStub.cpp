// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CGeoDeviceStub.h"
#include "ISceneManager.h"
#include "IEventReceiver.h"
#include "IFileSystem.h"
#include "os.h"
#include "geoCompileConfig.h"
#include "CTimer.h"
#include "CLogger.h"
#include "geoString.h"

namespace geo
{

//! constructor
CGeoDeviceStub::CGeoDeviceStub(const SGeoCreationParameters& params)
: GeoDevice(), VideoDriver(0), SceneManager(0), 
	Timer(0), CursorControl(0), UserReceiver(params.EventReceiver), Logger(0), Operator(0),
	FileSystem(0), InputReceivingSceneManager(0), CreationParams(params)
{
	Timer = new CTimer();
	if (os::Printer::Logger)
	{
		os::Printer::Logger->grab();
		Logger = (CLogger*)os::Printer::Logger;
		Logger->setReceiver(UserReceiver);
	}
	else
	{
		Logger = new CLogger(UserReceiver);
		os::Printer::Logger = Logger;
	}

	os::Printer::Logger = Logger;

	FileSystem = io::createFileSystem();
	core::stringc s = "Geo Engine version ";
	s.append(getVersion());
	os::Printer::log(s.c_str(), ELL_INFORMATION);

	checkVersion(params.SDK_version_do_not_use);
}


CGeoDeviceStub::~CGeoDeviceStub()
{
	FileSystem->drop();

	if (VideoDriver)
		VideoDriver->drop();

	if (SceneManager)
		SceneManager->drop();

	if (InputReceivingSceneManager)
		InputReceivingSceneManager->drop();

	if (CursorControl)
		CursorControl->drop();

	if (Operator)
		Operator->drop();

	CursorControl = 0;

	Timer->drop();

	if (Logger->drop())
		os::Printer::Logger = 0;
}


void CGeoDeviceStub::createGUIAndScene()
{
	// create Scene manager
	SceneManager = scene::createSceneManager(VideoDriver, FileSystem, CursorControl);

	setEventReceiver(UserReceiver);
}


//! returns the video driver
video::IVideoDriver* CGeoDeviceStub::getVideoDriver()
{
	return VideoDriver;
}



//! return file system
io::IFileSystem* CGeoDeviceStub::getFileSystem()
{
	return FileSystem;
}

//! returns the scene manager
scene::ISceneManager* CGeoDeviceStub::getSceneManager()
{
	return SceneManager;
}


//! \return Returns a pointer to the ITimer object. With it the
//! current Time can be received.
ITimer* CGeoDeviceStub::getTimer()
{
	return Timer;
}


//! Returns the version of the engine. 
const char* CGeoDeviceStub::getVersion() const
{
	return GEOENGINE_SDK_VERSION;
}

//! \return Returns a pointer to the mouse cursor control interface.
gui::ICursorControl* CGeoDeviceStub::getCursorControl()
{
	return CursorControl;
}


//! \return Returns a pointer to a list with all video modes supported
//! by the gfx adapter.
video::IVideoModeList* CGeoDeviceStub::getVideoModeList()
{
	return &VideoModeList;
}


//! checks version of sdk and prints warning if there might be a problem
bool CGeoDeviceStub::checkVersion(const char* version)
{
	if (strcmp(getVersion(), version))
	{
		core::stringc w;
		w = "Warning: The library version of the Geo Engine (";
		w += getVersion();
		w += ") does not match the version the application was compiled with (";
		w += version;
		w += "). This may cause problems.";
		os::Printer::log(w.c_str(), ELL_WARNING);
		_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return false;
	}

	return true;
}


//! send the event to the right receiver
bool CGeoDeviceStub::postEventFromUser(const SEvent& event)
{
	bool absorbed = false;

	if (UserReceiver)
		absorbed = UserReceiver->OnEvent(event);

	scene::ISceneManager* inputReceiver = InputReceivingSceneManager;
	if (!inputReceiver)
		inputReceiver = SceneManager;

	if (!absorbed && inputReceiver)
		absorbed = inputReceiver->postEventFromUser(event);

	_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return absorbed;
}


//! Sets a new event receiver to receive events
void CGeoDeviceStub::setEventReceiver(IEventReceiver* receiver)
{
	UserReceiver = receiver;
	Logger->setReceiver(receiver);
}


//! Returns poinhter to the current event receiver. Returns 0 if there is none.
IEventReceiver* CGeoDeviceStub::getEventReceiver()
{
	return UserReceiver;
}


//! \return Returns a pointer to the logger.
ILogger* CGeoDeviceStub::getLogger()
{
	return Logger;
}


//! Returns the operation system opertator object.
IOSOperator* CGeoDeviceStub::getOSOperator()
{
	return Operator;
}


//! Sets the input receiving scene manager. 
void CGeoDeviceStub::setInputReceivingSceneManager(scene::ISceneManager* sceneManager)
{
	if (InputReceivingSceneManager)
		InputReceivingSceneManager->drop();

	InputReceivingSceneManager = sceneManager;

	if (InputReceivingSceneManager)
		InputReceivingSceneManager->grab();
}


//! Checks if the window is running in fullscreen mode
bool CGeoDeviceStub::isFullscreen() const
{
	return CreationParams.Fullscreen;
}


//! returns color format
video::ECOLOR_FORMAT CGeoDeviceStub::getColorFormat() const
{
	return video::ECF_R5G6B5;
}

//! No-op in this implementation
bool CGeoDeviceStub::activateJoysticks(core::array<SJoystickInfo> & joystickInfo)
{
	return false;
}

} // end namespace geo


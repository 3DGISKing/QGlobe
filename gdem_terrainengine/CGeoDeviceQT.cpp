// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "geoCompileConfig.h"

#ifdef _GEO_USE_QT_DEVICE_

#include "CGeoDeviceQT.h"
#include "IEventReceiver.h"
#include "geoList.h"
#include "os.h"

#include "CTimer.h"
#include "geoString.h"
#include "dimension2d.h"
#include "geoEngine.h"

namespace geo
{
	namespace video
	{
		#ifdef _GEO_COMPILE_WITH_OPENGL_
		IVideoDriver* createOpenGLDriver(const geo::SGeoCreationParameters& params, io::IFileSystem* io, CGeoDeviceQT* device);
		#endif
	}
} // end namespace geo

namespace geo
{

//! constructor
CGeoDeviceQT::CGeoDeviceQT(const SGeoCreationParameters& params)
: CGeoDeviceStub(params), Widget(0), ChangedToFullScreen(false),
	IsNonNTWindows(false), Resized(false), ExternalWindow(false)
{
	#ifdef _DEBUG
	setDebugName("CGeoDeviceQT");
	#endif

	// attach external window
	Widget = static_cast<QWidget*>(CreationParams.WindowId);
	CreationParams.WindowSize.Width = params.WindowSize.Width;
	CreationParams.WindowSize.Height = params.WindowSize.Height;
	CreationParams.Fullscreen = false;
	ExternalWindow = true;

	// create driver
	createDriver();

	if (VideoDriver)
		createGUIAndScene();

}


//! destructor
CGeoDeviceQT::~CGeoDeviceQT()
{
}


//! create the driver
void CGeoDeviceQT::createDriver()
{
	switch(CreationParams.DriverType)
	{
	case video::EDT_OPENGL:

		#ifdef _GEO_COMPILE_WITH_OPENGL_

		if (CreationParams.Fullscreen)
			switchToFullScreen(CreationParams.WindowSize.Width, CreationParams.WindowSize.Height, CreationParams.Bits);

		VideoDriver = video::createOpenGLDriver(CreationParams, FileSystem, this);
		if (!VideoDriver)
		{
			os::Printer::log("Could not create OpenGL driver.", ELL_ERROR);
		}
		#else
		os::Printer::log("OpenGL driver was not compiled in.", ELL_ERROR);
		#endif
		break;

	case video::EDT_NULL:
		// create null driver
		VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
		break;

	default:
		os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
		break;
	}
}


//! runs the device. Returns false if device wants to be deleted
bool CGeoDeviceQT::run()
{
	os::Timer::tick();

	return false;
}


//! Pause the current process for the minimum time allowed only to allow other processes to execute
void CGeoDeviceQT::yield()
{
//	Sleep(1);	
}

//! Pause execution and let other processes to run for a specified amount of time.
void CGeoDeviceQT::sleep(u32 timeMs, bool pauseTimer)
{
	const bool wasStopped = Timer ? Timer->isStopped() : true;
	if (pauseTimer && !wasStopped)
		Timer->stop();
	
//	Sleep(timeMs);

	if (pauseTimer && !wasStopped)
		Timer->start();
}

//! sets the caption of the window
void CGeoDeviceQT::setWindowCaption(const wchar_t* text)
{

}


//! presents a surface in the client area
bool CGeoDeviceQT::present(video::IImage* image, void* windowId, core::rect<s32>* src)
{
	return true;
}

//! notifies the device that it should close itself
void CGeoDeviceQT::closeDevice()
{
	//DestroyWindow(HWnd);
}


//! returns if window is active. if not, nothing needs to be drawn
bool CGeoDeviceQT::isWindowActive() const
{
	return true;
}


//! returns if window has focus
bool CGeoDeviceQT::isWindowFocused() const
{
	return true;
}


//! returns if window is minimized
bool CGeoDeviceQT::isWindowMinimized() const
{
	return false;
}


//! switches to fullscreen
bool CGeoDeviceQT::switchToFullScreen(s32 width, s32 height, s32 bits)
{
	return false;
}

//! \return Returns a pointer to a list with all video modes supported
//! by the gfx adapter.
video::IVideoModeList* CGeoDeviceQT::getVideoModeList()
{
	return &VideoModeList;
}

//! Notifies the device, that it has been resized
void CGeoDeviceQT::OnResized()
{
	Resized = true;
}

//! Sets if the window should be resizeable in windowed mode.
void CGeoDeviceQT::setResizeAble(bool resize)
{
}


bool CGeoDeviceQT::activateJoysticks(core::array<SJoystickInfo> & joystickInfo)
{
	return false;
}

void CGeoDeviceQT::pollJoysticks()
{
} 

GEOENGINE_API GeoDevice* GEOCALLCONV createDeviceEx(
	const SGeoCreationParameters& parameters)
{
	CGeoDeviceQT* dev = new CGeoDeviceQT(parameters);

	return dev;
}


} // end namespace

#endif // _GEO_USE_WINDOWS_DEVICE_


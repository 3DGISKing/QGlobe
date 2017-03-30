// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_GEO_DEVICE_QT_H_INCLUDED__
#define __C_GEO_DEVICE_QT_H_INCLUDED__

#include "geoCompileConfig.h"
#ifdef _GEO_USE_QT_DEVICE_

#include "CGeoDeviceStub.h"
#include "geoEngineDevice.h"
#include "IImagePresenter.h"

class QWidget;

namespace geo
{
	class CGeoDeviceQT : public CGeoDeviceStub, video::IImagePresenter
	{
	public:

		//! constructor
		CGeoDeviceQT(const SGeoCreationParameters& params);

		//! destructor
		virtual ~CGeoDeviceQT();

		//! runs the device. Returns false if device wants to be deleted
		virtual bool run();

		//! Cause the device to temporarily pause execution and let other processes to run
		// This should bring down processor usage without major performance loss for Geo
		virtual void yield();

		//! Pause execution and let other processes to run for a specified amount of time.
		virtual void sleep(u32 timeMs, bool pauseTimer);

		//! sets the caption of the window
		virtual void setWindowCaption(const wchar_t* text);

		//! returns if window is active. if not, nothing need to be drawn
		virtual bool isWindowActive() const;

		//! returns if window has focus
		virtual bool isWindowFocused() const;

		//! returns if window is minimized
		virtual bool isWindowMinimized() const;

		//! presents a surface in the client area
		virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0);

		//! notifies the device that it should close itself
		virtual void closeDevice();

		//! \return Returns a pointer to a list with all video modes
		//! supported by the gfx adapter.
		video::IVideoModeList* getVideoModeList();

		//! Notifies the device, that it has been resized
		void OnResized();

		//! Sets if the window should be resizeable in windowed mode.
		virtual void setResizeAble(bool resize=false);

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo);

	private:

		//! create the driver
		void createDriver();

		//! switchs to fullscreen
		bool switchToFullScreen(s32 width, s32 height, s32 bits);

		void getWindowsVersion(core::stringc& version);

		void resizeIfNecessary();

		void pollJoysticks(); 

		QWidget* Widget;

		bool ChangedToFullScreen;
		bool IsNonNTWindows;
		bool Resized;
		bool ExternalWindow;
	};

} // end namespace geo

#endif //_GEO_USE_QT_DEVICE_
#endif //__C_GEO_DEVICE_QT_H_INCLUDED__


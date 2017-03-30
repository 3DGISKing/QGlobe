// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h
// This device code is based on the original SDL device implementation
// contributed by Shane Parker (sirshane).

#ifndef __C_GEO_DEVICE_SDL_H_INCLUDED__
#define __C_GEO_DEVICE_SDL_H_INCLUDED__

#include "geoCompileConfig.h"

#ifdef _GEO_USE_SDL_DEVICE_

#include "geoEngineDevice.h"
#include "CGeoDeviceStub.h"
#include "IImagePresenter.h"
#include "ICursorControl.h"

#include <SDL/SDL.h>

namespace geo
{

	class CGeoDeviceSDL : public CGeoDeviceStub, video::IImagePresenter
	{
	public:

		//! constructor
		CGeoDeviceSDL(const SGeoCreationParameters& param);

		//! destructor
		virtual ~CGeoDeviceSDL();

		//! runs the device. Returns false if device wants to be deleted
		virtual bool run();

		//! pause execution temporarily
		virtual void yield();

		//! pause execution for a specified time
		virtual void sleep(u32 timeMs, bool pauseTimer);

		//! sets the caption of the window
		virtual void setWindowCaption(const wchar_t* text);

		//! returns if window is active. if not, nothing need to be drawn
		virtual bool isWindowActive() const;

		//! returns if window has focus.
		bool isWindowFocused() const;

		//! returns if window is minimized.
		bool isWindowMinimized() const;
			
		//! returns color format of the window.
		video::ECOLOR_FORMAT getColorFormat() const;

		//! presents a surface in the client area
		virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0);

		//! notifies the device that it should close itself
		virtual void closeDevice();

		//! \return Returns a pointer to a list with all video modes supported
		video::IVideoModeList* getVideoModeList();

		//! Sets if the window should be resizeable in windowed mode.
		virtual void setResizeAble(bool resize=false);

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo);

		//! Implementation of the linux cursor control
		class CCursorControl : public gui::ICursorControl
		{
		public:

			CCursorControl(CGeoDeviceSDL* dev)
				: Device(dev), IsVisible(true)
			{
			}

			//! Changes the visible state of the mouse cursor.
			virtual void setVisible(bool visible)
			{
				IsVisible = visible;
				if ( visible )
					SDL_ShowCursor( SDL_ENABLE );
				else
					SDL_ShowCursor( SDL_DISABLE );
			}

			//! Returns if the cursor is currently visible.
			virtual bool isVisible() const
			{
				return IsVisible;
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(const core::position2d<f32> &pos)
			{
				setPosition(pos.X, pos.Y);
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(f32 x, f32 y)
			{
				setPosition((s32)(x*Device->Width), (s32)(y*Device->Height));
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(const core::position2d<s32> &pos)
			{
				setPosition(pos.X, pos.Y);
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(s32 x, s32 y)
			{
				SDL_WarpMouse( x, y );
			}

			//! Returns the current position of the mouse cursor.
			virtual core::position2d<s32> getPosition()
			{
				updateCursorPos();
				return CursorPos;
			}

			//! Returns the current position of the mouse cursor.
			virtual core::position2d<f32> getRelativePosition()
			{
				updateCursorPos();
				return core::position2d<f32>(CursorPos.X / (f32)Device->Width,
					CursorPos.Y / (f32)Device->Height);
			}

			virtual void setReferenceRect(core::rect<s32>* rect=0)
			{
			}

		private:

			void updateCursorPos()
			{
				CursorPos.X = Device->MouseX;
				CursorPos.Y = Device->MouseY;
			
				if (CursorPos.X < 0)
					CursorPos.X = 0;
				if (CursorPos.X > (s32)Device->Width)
					CursorPos.X = Device->Width;
				if (CursorPos.Y < 0)
					CursorPos.Y = 0;
				if (CursorPos.Y > (s32)Device->Height)
					CursorPos.Y = Device->Height;
			}

			CGeoDeviceSDL* Device;
			core::position2d<s32> CursorPos;
			bool IsVisible;
		};

	private:

		//! create the driver
		void createDriver();

		bool createWindow();

		void createKeyMap();

		SDL_Surface* Screen;
		int SDL_Flags;
#if defined(_GEO_COMPILE_WITH_JOYSTICK_EVENTS_)
		core::array<SDL_Joystick*> Joysticks;
#endif

		s32 MouseX, MouseY;
		
		u32 Width, Height;

		bool Close;
		bool Resizeable;
		bool WindowHasFocus;
		bool WindowMinimized;

		struct SKeyMap
		{
			SKeyMap() {}
			SKeyMap(s32 x11, s32 win32)
				: SDLKey(x11), Win32Key(win32)
			{
			}

			s32 SDLKey;
			s32 Win32Key;

			bool operator<(const SKeyMap& o) const
			{
				return SDLKey<o.SDLKey;
			}
		};

		core::array<SKeyMap> KeyMap;
	};

} // end namespace geo

#endif // _GEO_USE_SDL_DEVICE_
#endif // __C_GEO_DEVICE_SDL_H_INCLUDED__


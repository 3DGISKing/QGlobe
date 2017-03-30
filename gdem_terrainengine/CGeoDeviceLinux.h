// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_GEO_DEVICE_LINUX_H_INCLUDED__
#define __C_GEO_DEVICE_LINUX_H_INCLUDED__

#include "geoCompileConfig.h"

#ifdef _GEO_USE_LINUX_DEVICE_

#include "CGeoDeviceStub.h"
#include "geoEngineDevice.h"
#include "IImagePresenter.h"
#include "ICursorControl.h"

#ifdef _GEO_COMPILE_WITH_X11_

#ifdef _GEO_COMPILE_WITH_OPENGL_
#include <GL/gl.h>
#define GLX_GLXEXT_LEGACY 1
#include <GL/glx.h>
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
#include "glxext.h"
#endif
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef _GEO_LINUX_X11_VIDMODE_
#include <X11/extensions/xf86vmode.h>
#endif
#ifdef _GEO_LINUX_X11_RANDR_
#include <X11/extensions/Xrandr.h>
#endif
#include <X11/keysym.h>

#else
#define KeySym s32
#endif

namespace geo
{

	class CGeoDeviceLinux : public CGeoDeviceStub, public video::IImagePresenter
	{
	public:

		//! constructor
		CGeoDeviceLinux(const SGeoCreationParameters& param);

		//! destructor
		virtual ~CGeoDeviceLinux();

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

		//! returns if window has focus.
		virtual bool isWindowFocused() const;

		//! returns if window is minimized.
		virtual bool isWindowMinimized() const;

		//! returns color format of the window.
		virtual video::ECOLOR_FORMAT getColorFormat() const;

		//! presents a surface in the client area
		virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0 );

		//! notifies the device that it should close itself
		virtual void closeDevice();

		//! \return Returns a pointer to a list with all video modes
		//! supported by the gfx adapter.
		video::IVideoModeList* getVideoModeList();

		//! Sets if the window should be resizeable in windowed mode.
		virtual void setResizeAble(bool resize=false);

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo);

	private:

		//! create the driver
		void createDriver();

		bool createWindow();

		void createKeyMap();

		void pollJoysticks(); 

		//! Implementation of the linux cursor control
		class CCursorControl : public gui::ICursorControl
		{
		public:

			CCursorControl(CGeoDeviceLinux* dev, bool null)
				: Device(dev), IsVisible(true), Null(null), UseReferenceRect(false)
			{
#ifdef _GEO_COMPILE_WITH_X11_
				if (!Null)
				{
					XGCValues values;
					unsigned long valuemask = 0;

					XColor fg, bg;

					// this code, for making the cursor invisible was sent in by
					// Sirshane, thank your very much!


					Pixmap invisBitmap = XCreatePixmap(Device->display, Device->window, 32, 32, 1);
					Pixmap maskBitmap = XCreatePixmap(Device->display, Device->window, 32, 32, 1);
					Colormap screen_colormap = DefaultColormap( Device->display, DefaultScreen( Device->display ) );
					XAllocNamedColor( Device->display, screen_colormap, "black", &fg, &fg );
					XAllocNamedColor( Device->display, screen_colormap, "white", &bg, &bg );

					GC gc = XCreateGC( Device->display, invisBitmap, valuemask, &values );

					XSetForeground( Device->display, gc, BlackPixel( Device->display, DefaultScreen( Device->display ) ) );
					XFillRectangle( Device->display, invisBitmap, gc, 0, 0, 32, 32 );
					XFillRectangle( Device->display, maskBitmap, gc, 0, 0, 32, 32 );

					invisCursor = XCreatePixmapCursor( Device->display, invisBitmap, maskBitmap, &fg, &bg, 1, 1 );
					XFreeGC(Device->display, gc);
					XFreePixmap(Device->display, invisBitmap);
					XFreePixmap(Device->display, maskBitmap);
				}
#endif
			}

			//! Changes the visible state of the mouse cursor.
			virtual void setVisible(bool visible)
			{
				IsVisible = visible;
#ifdef _GEO_COMPILE_WITH_X11_
				if (!Null)
				{
					if ( !IsVisible )
						XDefineCursor( Device->display, Device->window, invisCursor );
					else
						XUndefineCursor( Device->display, Device->window );
				}
#endif
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
#ifdef _GEO_COMPILE_WITH_X11_

				if (!Null)
				{
					if (UseReferenceRect)
					{
						XWarpPointer(Device->display,
							None,
							Device->window, 0, 0,
							Device->Width,
							Device->Height,
							ReferenceRect.UpperLeftCorner.X + x,
							ReferenceRect.UpperLeftCorner.Y + y);

					}
					else
					{
						XWarpPointer(Device->display,
							None,
							Device->window, 0, 0,
							Device->Width,
							Device->Height, x, y);
					}
					XFlush(Device->display);
				}
#endif
				CursorPos.X = x;
				CursorPos.Y = y;
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

				if (!UseReferenceRect)
				{
					return core::position2d<f32>(CursorPos.X / (f32)Device->Width,
						CursorPos.Y / (f32)Device->Height);
				}

				return core::position2d<f32>(CursorPos.X / (f32)ReferenceRect.getWidth(),
						CursorPos.Y / (f32)ReferenceRect.getHeight());
			}

			virtual void setReferenceRect(core::rect<s32>* rect=0)
			{
				if (rect)
				{
					ReferenceRect = *rect;
					UseReferenceRect = true;

					// prevent division through zero and uneven sizes

					if (!ReferenceRect.getHeight() || ReferenceRect.getHeight()%2)
						ReferenceRect.LowerRightCorner.Y += 1;

					if (!ReferenceRect.getWidth() || ReferenceRect.getWidth()%2)
						ReferenceRect.LowerRightCorner.X += 1;
				}
				else
					UseReferenceRect = false;
			}

		private:

			void updateCursorPos()
			{
#ifdef _GEO_COMPILE_WITH_X11_
				if (Null)
					return;

				Window tmp;
				int itmp1, itmp2;
				unsigned  int maskreturn;
				XQueryPointer(Device->display, Device->window,
					&tmp, &tmp,
					&itmp1, &itmp2,
					&CursorPos.X, &CursorPos.Y, &maskreturn);

				if (CursorPos.X < 0)
					CursorPos.X = 0;
				if (CursorPos.X > (s32) Device->Width)
					CursorPos.X = Device->Width;
				if (CursorPos.Y < 0)
					CursorPos.Y = 0;
				if (CursorPos.Y > (s32) Device->Height)
					CursorPos.Y = Device->Height;
#endif
			}

			CGeoDeviceLinux* Device;
			core::position2d<s32> CursorPos;
			core::rect<s32> ReferenceRect;
#ifdef _GEO_COMPILE_WITH_X11_
			Cursor invisCursor;
#endif
			bool IsVisible;
			bool Null;
			bool UseReferenceRect;
		};

		friend class CCursorControl;

#ifdef _GEO_COMPILE_WITH_X11_
		Display *display;
		XVisualInfo* visual;
		int screennr;
		Window window;
		XSetWindowAttributes attributes;
		XSizeHints* StdHints;
		XImage* SoftwareImage;
		#ifdef _GEO_LINUX_X11_VIDMODE_
		XF86VidModeModeInfo oldVideoMode;
		#endif
		#ifdef _GEO_LINUX_X11_RANDR_
		SizeID oldRandrMode;
		Rotation oldRandrRotation;
		#endif
		#ifdef _GEO_COMPILE_WITH_OPENGL_
		GLXWindow glxWin;
		GLXContext Context;
		#endif
#endif
		u32 Width, Height;
		bool Close;
		bool WindowHasFocus;
		bool WindowMinimized;
		bool UseXVidMode;
		bool UseXRandR;
		bool UseGLXWindow;
		int AutorepeatSupport;

		struct SKeyMap
		{
			SKeyMap() {}
			SKeyMap(s32 x11, s32 win32)
				: X11Key(x11), Win32Key(win32)
			{
			}

			KeySym X11Key;
			s32 Win32Key;

			bool operator<(const SKeyMap& o) const
			{
				return X11Key<o.X11Key;
			}
		};

		core::array<SKeyMap> KeyMap;

#if defined(_GEO_COMPILE_WITH_JOYSTICK_EVENTS_)
		struct JoystickInfo
		{
			int	fd;
			int	axes;
			int	buttons;

			SEvent persistentData;

			JoystickInfo() : fd(-1), axes(0), buttons(0) { }
		};
		core::array<JoystickInfo> ActiveJoysticks;
#endif
	};


} // end namespace geo

#endif // _GEO_USE_LINUX_DEVICE_
#endif // __C_GEO_DEVICE_LINUX_H_INCLUDED__

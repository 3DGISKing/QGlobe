// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_GEO_DEVICE_STUB_H_INCLUDED__
#define __C_GEO_DEVICE_STUB_H_INCLUDED__

#include "geoEngineDevice.h"
#include "IImagePresenter.h"
#include "SGeoCreationParameters.h"
#include "CVideoModeList.h"

namespace geo
{
	// lots of prototypes:
	class ILogger;
	class CLogger;

	namespace scene
	{
		ISceneManager* createSceneManager(video::IVideoDriver* driver, 
			io::IFileSystem* fs, gui::ICursorControl* cc);
	}

	namespace io
	{
		IFileSystem* createFileSystem();
	}

	namespace video
	{
		IVideoDriver* createSoftwareDriver(const core::dimension2d<s32>& windowSize,
				bool fullscreen, io::IFileSystem* io,
				video::IImagePresenter* presenter);
		IVideoDriver* createSoftwareDriver2(const core::dimension2d<s32>& windowSize,
				bool fullscreen, io::IFileSystem* io,
				video::IImagePresenter* presenter);
		IVideoDriver* createNullDriver(io::IFileSystem* io, const core::dimension2d<s32>& screenSize);
	}



	//! Stub for an Geo Device implementation
	class CGeoDeviceStub : public GeoDevice
	{
	public:

		//! constructor
		CGeoDeviceStub(const SGeoCreationParameters& param);

		//! destructor
		virtual ~CGeoDeviceStub();

		//! returns the video driver
		virtual video::IVideoDriver* getVideoDriver();

		//! return file system
		virtual io::IFileSystem* getFileSystem();

		//! returns the scene manager
		virtual scene::ISceneManager* getSceneManager();

		//! \return Returns a pointer to the mouse cursor control interface.
		virtual gui::ICursorControl* getCursorControl();

		//! Returns a pointer to a list with all video modes supported by the gfx adapter.
		virtual video::IVideoModeList* getVideoModeList();

		//! Returns a pointer to the ITimer object. With it the current Time can be received.
		virtual ITimer* getTimer();

		//! Returns the version of the engine. 
		virtual const char* getVersion() const;

		//! send the event to the right receiver
		virtual bool postEventFromUser(const SEvent& event);

		//! Sets a new event receiver to receive events
		virtual void setEventReceiver(IEventReceiver* receiver);

		//! Returns pointer to the current event receiver. Returns 0 if there is none.
		virtual IEventReceiver* getEventReceiver();

		//! Sets the input receiving scene manager. 
		/** If set to null, the main scene manager (returned by GetSceneManager()) will receive the input */
		virtual void setInputReceivingSceneManager(scene::ISceneManager* sceneManager);

		//! Returns a pointer to the logger.
		virtual ILogger* getLogger();

		//! Returns the operation system opertator object.
		virtual IOSOperator* getOSOperator();

		//! Checks if the window is running in fullscreen mode.
		virtual bool isFullscreen() const;

		//! get color format of the current window
		virtual video::ECOLOR_FORMAT getColorFormat() const;

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo);

	protected:

		void createGUIAndScene();

		//! checks version of SDK and prints warning if there might be a problem
		bool checkVersion(const char* version);

		video::IVideoDriver* VideoDriver;
		scene::ISceneManager* SceneManager;
		ITimer* Timer;
		gui::ICursorControl* CursorControl;
		IEventReceiver* UserReceiver;
		CLogger* Logger;
		IOSOperator* Operator;
		io::IFileSystem* FileSystem;
		scene::ISceneManager* InputReceivingSceneManager;
		video::CVideoModeList VideoModeList;
		SGeoCreationParameters CreationParams;
	};

} // end namespace geo

#endif


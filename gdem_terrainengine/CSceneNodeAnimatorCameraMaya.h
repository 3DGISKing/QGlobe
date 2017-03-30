// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_SCENE_NODE_ANIMATOR_CAMERA_MAYA_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_CAMERA_MAYA_H_INCLUDED__

#include "ISceneNodeAnimatorCameraMaya.h"
#include "ICameraSceneNode.h"
#include "vector2d.h"

namespace geo
{

namespace gui
{
	class ICursorControl;
}

namespace scene
{

	//! Special scene node animator for FPS cameras
	/** This scene node animator can be attached to a camera to make it act
	like a 3d modelling tool camera
	*/
	class CSceneNodeAnimatorCameraMaya : public ISceneNodeAnimatorCameraMaya 
	{
	public:
		//! Constructor
		CSceneNodeAnimatorCameraMaya(gui::ICursorControl* cursor, f32 rotateSpeed = -1500.0f, 
			f32 zoomSpeed = 200.0f, f32 translationSpeed = 1500.0f);

		//! Destructor
		virtual ~CSceneNodeAnimatorCameraMaya();

		//! Animates the scene node, currently only works on cameras
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		//! Event receiver
		virtual bool OnEvent(const SEvent& event);

		//! Returns the speed of movement in units per millisecond
		virtual f32 getMoveSpeed() const;

		//! Sets the speed of movement in units per millisecond
		virtual void setMoveSpeed(f32 moveSpeed);

		//! Returns the rotation speed
		virtual f32 getRotateSpeed() const;

		//! Set the rotation speed
		virtual void setRotateSpeed(f32 rotateSpeed);

		//! Returns the zoom speed
		virtual f32 getZoomSpeed() const;

		//! Set the zoom speed
		virtual void setZoomSpeed(f32 zoomSpeed);

		//! This animator will receive events when attached to the active camera
		virtual bool isEventReceiverEnabled() const
		{
			return true;
		}

		//! Returns type of the scene node
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const 
		{
			return ESNAT_CAMERA_MAYA;
		}

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

	private:

		void allKeysUp();
		void animate();
		bool isMouseKeyDown(s32 key);

		bool MouseKeys[3];

		gui::ICursorControl *CursorControl;
		core::vector3df Pos;
		bool Zooming;
		bool Rotating;
		bool Moving;
		bool Translating;
		f32 ZoomSpeed;
		f32 RotateSpeed;
		f32 TranslateSpeed;
		core::position2df RotateStart;
		core::position2df ZoomStart;
		core::position2df TranslateStart;
		f32 CurrentZoom;
		f32 RotX, RotY;
		core::vector3df OldTarget;
		scene::ICameraSceneNode* OldCamera;

		core::position2df MousePos;
	};

} // end namespace scene
} // end namespace geo

#endif


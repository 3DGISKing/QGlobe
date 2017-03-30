// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CSceneNodeAnimatorCollisionResponse.h"
#include "ISceneCollisionManager.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "os.h"

namespace geo
{
namespace scene
{

//! constructor
CSceneNodeAnimatorCollisionResponse::CSceneNodeAnimatorCollisionResponse(
		ISceneManager* scenemanager,
		ITriangleSelector* world, ISceneNode* object,
		const core::vector3df& ellipsoidRadius,
		const core::vector3df& gravityPerSecond,
		const core::vector3df& ellipsoidTranslation,
		f32 slidingSpeed)
: Radius(ellipsoidRadius), Gravity(gravityPerSecond), Translation(ellipsoidTranslation),
	World(world), Object(object), SceneManager(scenemanager), LastTime(0),
	SlidingSpeed(slidingSpeed), Falling(false), IsCamera(false),
	AnimateCameraTarget(true)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorCollisionResponse");
	#endif
	
	if (World)
		World->grab();

	setNode(Object);
}


//! destructor
CSceneNodeAnimatorCollisionResponse::~CSceneNodeAnimatorCollisionResponse()
{
	if (World)
		World->drop();
}


//! Returns if the attached scene node is falling, which means that
//! there is no blocking wall from the scene node in the direction of
//! the gravity.
bool CSceneNodeAnimatorCollisionResponse::isFalling() const
{
	_GEO_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return Falling;
}


//! Sets the radius of the ellipsoid with which collision detection and
//! response is done.
void CSceneNodeAnimatorCollisionResponse::setEllipsoidRadius(
	const core::vector3df& radius)
{
	Radius = radius;
}


//! Returns the radius of the ellipsoid with wich the collision detection and
//! response is done.
core::vector3df CSceneNodeAnimatorCollisionResponse::getEllipsoidRadius() const
{
	return Radius;
}


//! Sets the gravity of the environment.
void CSceneNodeAnimatorCollisionResponse::setGravity(const core::vector3df& gravity)
{
	Gravity = gravity;
}


//! Returns current vector of gravity.
core::vector3df CSceneNodeAnimatorCollisionResponse::getGravity() const
{
	return Gravity;
}


//! 'Jump' the animator, by adding a jump speed opposite to its gravity
void CSceneNodeAnimatorCollisionResponse::jump(f32 jumpSpeed)
{
	FallingVelocity -= (core::vector3df(Gravity).normalize()) * jumpSpeed;
	Falling = true;
}


//! Sets the translation of the ellipsoid for collision detection.
void CSceneNodeAnimatorCollisionResponse::setEllipsoidTranslation(const core::vector3df &translation)
{
	Translation = translation;
}


//! Returns the translation of the ellipsoid for collision detection.
core::vector3df CSceneNodeAnimatorCollisionResponse::getEllipsoidTranslation() const
{
	return Translation;
}


//! Sets a triangle selector holding all triangles of the world with which
//! the scene node may collide.
void CSceneNodeAnimatorCollisionResponse::setWorld(ITriangleSelector* newWorld)
{
	Falling = false;

	LastTime = os::Timer::getTime();

	if (World)
		World->drop();

	World = newWorld;
	if (World)
		World->grab();

}


//! Returns the current triangle selector containing all triangles for
//! collision detection.
ITriangleSelector* CSceneNodeAnimatorCollisionResponse::getWorld() const
{
	return World;
}


void CSceneNodeAnimatorCollisionResponse::animateNode(ISceneNode* node, u32 timeMs)
{
	if (node != Object)
	{
		setNode(node);
		return;
	}

	if (!World)
		return;

	u32 diff = timeMs - LastTime;
	LastTime = timeMs;

	core::vector3df pos = Object->getPosition();
	core::vector3df vel = pos - LastPosition;

	FallingVelocity += Gravity * (f32)diff * 0.001f;

	core::triangle3df triangle = RefTriangle;

	core::vector3df force = vel + FallingVelocity;

	const core::vector3df nullVector ( 0.f, 0.f, 0.f );

	if ( force != nullVector )
	{
		// TODO: divide SlidingSpeed by frame time

		bool f = false;
		pos = SceneManager->getSceneCollisionManager()->getCollisionResultPosition(
				World, LastPosition-Translation,
				Radius, vel, triangle, f, SlidingSpeed, FallingVelocity);

		pos += Translation;

		if (f)//triangle == RefTriangle)
		{
			Falling = true;
		}
		else
		{
			Falling = false;
			FallingVelocity.set(0, 0, 0);
		}

		Object->setPosition(pos);
	}

	// move camera target
	if (AnimateCameraTarget && IsCamera)
	{
		const core::vector3df pdiff = Object->getPosition() - LastPosition - vel;
		ICameraSceneNode* cam = (ICameraSceneNode*)Object;
		cam->setTarget(cam->getTarget() + pdiff);
	}

	LastPosition = Object->getPosition();
}


void CSceneNodeAnimatorCollisionResponse::setNode(ISceneNode* node)
{
	Object = node;

	if (Object)
	{
		LastPosition = Object->getPosition();
		IsCamera = (Object->getType() == ESNT_CAMERA);
	}

	LastTime = os::Timer::getTime();
}


//! Writes attributes of the scene node animator.
void CSceneNodeAnimatorCollisionResponse::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addVector3d("Radius", Radius);
	out->addVector3d("Gravity", Gravity);
	out->addVector3d("Translation", Translation);
	out->addBool("AnimateCameraTarget", AnimateCameraTarget);
}


//! Reads attributes of the scene node animator.
void CSceneNodeAnimatorCollisionResponse::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Radius = in->getAttributeAsVector3d("Radius");
	Gravity = in->getAttributeAsVector3d("Gravity");
	Translation = in->getAttributeAsVector3d("Translation");
	AnimateCameraTarget = in->getAttributeAsBool("AnimateCameraTarget");
}


ISceneNodeAnimator* CSceneNodeAnimatorCollisionResponse::createClone(ISceneNode* node, ISceneManager* newManager)
{
	if (!newManager) newManager = SceneManager;

	CSceneNodeAnimatorCollisionResponse * newAnimator = 
		new CSceneNodeAnimatorCollisionResponse(newManager, World, Object, Radius, (Gravity * 1000.0f), Translation,
													SlidingSpeed);

	return newAnimator;
}


} // end namespace scene
} // end namespace geo


// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CSceneNodeAnimatorRotation.h"

namespace geo
{
namespace scene
{

//! constructor
CSceneNodeAnimatorRotation::CSceneNodeAnimatorRotation(u32 time, const core::vector3df& rotation)
: Rotation(rotation), StartTime(time)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorRotation");
	#endif
}


//! destructor
CSceneNodeAnimatorRotation::~CSceneNodeAnimatorRotation()
{
}



//! animates a scene node
void CSceneNodeAnimatorRotation::animateNode(ISceneNode* node, u32 timeMs)
{
	if (node) // thanks to warui for this fix
	{
		u32 diffTime = timeMs - StartTime;

		if (diffTime != 0)
		{
			core::vector3df NewRotation = node->getRotation(); 
			NewRotation += Rotation* ((diffTime)/10.0f); 
			node->setRotation(NewRotation); 
			StartTime=timeMs; 
		}
	}
}


//! Writes attributes of the scene node animator.
void CSceneNodeAnimatorRotation::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addVector3d("Rotation", Rotation);
}

//! Reads attributes of the scene node animator.
void CSceneNodeAnimatorRotation::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Rotation = in->getAttributeAsVector3d("Rotation");
}

ISceneNodeAnimator* CSceneNodeAnimatorRotation::createClone(ISceneNode* node, ISceneManager* newManager)
{
	CSceneNodeAnimatorRotation * newAnimator = 
		new CSceneNodeAnimatorRotation(StartTime, Rotation);

	return newAnimator;
}

} // end namespace scene
} // end namespace geo


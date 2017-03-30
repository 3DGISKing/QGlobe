// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CSceneNodeAnimatorDelete.h"
#include "ISceneManager.h"

namespace geo
{
namespace scene
{


//! constructor
CSceneNodeAnimatorDelete::CSceneNodeAnimatorDelete(ISceneManager* manager, u32 time)
: DeleteTime(time), SceneManager(manager)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorDelete");
	#endif
}



//! destructor
CSceneNodeAnimatorDelete::~CSceneNodeAnimatorDelete()
{
}



//! animates a scene node
void CSceneNodeAnimatorDelete::animateNode(ISceneNode* node, u32 timeMs)
{
	if (timeMs > DeleteTime && node && SceneManager)
	{
		// don't delete if scene manager is attached to an editor
		if (!SceneManager->getParameters()->getAttributeAsBool(GEO_SCENE_MANAGER_IS_EDITOR))
			SceneManager->addToDeletionQueue(node);
	}
}

ISceneNodeAnimator* CSceneNodeAnimatorDelete::createClone(ISceneNode* node, ISceneManager* newManager)
{
	CSceneNodeAnimatorDelete * newAnimator = 
		new CSceneNodeAnimatorDelete(SceneManager, DeleteTime);

	return newAnimator;
}

} // end namespace scene
} // end namespace geo


// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_SCENE_NODE_ANIMATOR_DELETE_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_DELETE_H_INCLUDED__

#include "ISceneNode.h"

namespace geo
{
namespace scene
{
	class CSceneNodeAnimatorDelete : public ISceneNodeAnimator
	{
	public:

		//! constructor
		CSceneNodeAnimatorDelete(ISceneManager* manager, u32 when);

		//! destructor
		virtual ~CSceneNodeAnimatorDelete();

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return ESNAT_DELETION;
		}
		
		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

	private:

		u32 DeleteTime;
		ISceneManager* SceneManager;
	};


} // end namespace scene
} // end namespace geo

#endif


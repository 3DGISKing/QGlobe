// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CEmptySceneNode.h"
#include "ISceneManager.h"

namespace geo
{
namespace scene
{

//! constructor
CEmptySceneNode::CEmptySceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
: ISceneNode(parent, mgr, id)
{
	#ifdef _DEBUG
	setDebugName("CEmptySceneNode");
	#endif

	setAutomaticCulling(scene::EAC_OFF);
}


//! pre render event
void CEmptySceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


//! render
void CEmptySceneNode::render()
{
	// do nothing
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CEmptySceneNode::getBoundingBox() const
{
	return Box;
}


//! Creates a clone of this scene node and its children.
ISceneNode* CEmptySceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent) newParent = Parent;
	if (!newManager) newManager = SceneManager;

	CEmptySceneNode* nb = new CEmptySceneNode(newParent, 
		newManager, ID);

	nb->cloneMembers(this, newManager);
	nb->Box = Box;

	nb->drop();
	return nb;
}


} // end namespace scene
} // end namespace geo

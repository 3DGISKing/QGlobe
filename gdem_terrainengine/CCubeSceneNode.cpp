// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CCubeSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "SMeshBuffer.h"
#include "os.h"

namespace geo
{
namespace scene
{

	/*
        011         111
          /6,8------/5        y
         /  |      / |        ^  z
        /   |     /  |        | /
    010 3,9-------2  |        |/
        |   7- - -10,4 101     *---->x
        |  /      |  /
        |/        | /
        0------11,1/
       000       100       
	*/

//! constructor
CCubeSceneNode::CCubeSceneNode(f32 size, ISceneNode* parent, ISceneManager* mgr,
		s32 id, const core::vector3df& position,
		const core::vector3df& rotation, const core::vector3df& scale)
	: IMeshSceneNode(parent, mgr, id, position, rotation, scale), Size(size)
{
	#ifdef _DEBUG
	setDebugName("CCubeSceneNode");
	#endif

	const u16 u[36] = {   0,2,1,   0,3,2,   1,5,4,   1,2,5,   4,6,7,   4,5,6, 
            7,3,0,   7,6,3,   9,5,2,   9,8,5,   0,11,10,   0,10,7};

	SMeshBuffer* buf = new SMeshBuffer();
	Mesh.addMeshBuffer(buf);
	buf->Indices.set_used(36);
	for (u32 i=0; i<36; ++i)
		buf->Indices[i] = u[i];
	buf->drop();

	setSize();
}


void CCubeSceneNode::setSize()
{
	// we are creating the cube mesh here. 

	// nicer texture mapping sent in by Dr Andros C Bragianos 
	// .. and then improved by jox.

	video::SColor clr(255,255,255,255);

	SMeshBuffer* buf = (SMeshBuffer*)Mesh.getMeshBuffer(0);

	buf->Vertices.reallocate(12);
	// Start setting vertices from index 0 to deal with this method being called multiple times.
	buf->Vertices.set_used(0);
	buf->Vertices.push_back(video::S3DVertex(0,0,0, -1,-1,-1, clr, 0, 1));
	buf->Vertices.push_back(video::S3DVertex(1,0,0,  1,-1,-1, clr, 1, 1));
	buf->Vertices.push_back(video::S3DVertex(1,1,0,  1, 1,-1, clr, 1, 0));
	buf->Vertices.push_back(video::S3DVertex(0,1,0, -1, 1,-1, clr, 0, 0));
	buf->Vertices.push_back(video::S3DVertex(1,0,1,  1,-1, 1, clr, 0, 1));
	buf->Vertices.push_back(video::S3DVertex(1,1,1,  1, 1, 1, clr, 0, 0));
	buf->Vertices.push_back(video::S3DVertex(0,1,1, -1, 1, 1, clr, 1, 0));
	buf->Vertices.push_back(video::S3DVertex(0,0,1, -1,-1, 1, clr, 1, 1));
	buf->Vertices.push_back(video::S3DVertex(0,1,1, -1, 1, 1, clr, 0, 1));
	buf->Vertices.push_back(video::S3DVertex(0,1,0, -1, 1,-1, clr, 1, 1));
	buf->Vertices.push_back(video::S3DVertex(1,0,1,  1,-1, 1, clr, 1, 0));
	buf->Vertices.push_back(video::S3DVertex(1,0,0,  1,-1,-1, clr, 0, 0));

	buf->BoundingBox.reset(0,0,0); 

	for (u32 i=0; i<12; ++i)
	{
		buf->Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
		buf->Vertices[i].Pos *= Size;
		buf->BoundingBox.addInternalPoint(buf->Vertices[i].Pos);
	}
}


//! renders the node.
void CCubeSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->setMaterial(Mesh.getMeshBuffer(0)->getMaterial());
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawMeshBuffer(Mesh.getMeshBuffer(0));
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CCubeSceneNode::getBoundingBox() const
{
	return Mesh.getMeshBuffer(0)->getBoundingBox();
}


void CCubeSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CCubeSceneNode::getMaterial(u32 i)
{
	return Mesh.getMeshBuffer(0)->getMaterial();
}


//! returns amount of materials used by this scene node.
u32 CCubeSceneNode::getMaterialCount() const
{
	return 1;
}


//! Writes attributes of the scene node.
void CCubeSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addFloat("Size", Size);
}


//! Reads attributes of the scene node.
void CCubeSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Size =	in->getAttributeAsFloat("Size");
	Size = core::max_(Size, 0.0001f);
	setSize();

	ISceneNode::deserializeAttributes(in, options);
}


//! Creates a clone of this scene node and its children.
ISceneNode* CCubeSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent) newParent = Parent;
	if (!newManager) newManager = SceneManager;

	CCubeSceneNode* nb = new CCubeSceneNode(Size, newParent, 
		newManager, ID, RelativeTranslation);

	nb->cloneMembers(this, newManager);
	nb->getMaterial(0) = getMaterial(0);

	nb->drop();
	return nb;
}


} // end namespace scene
} // end namespace geo


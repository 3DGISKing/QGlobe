// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CMeshSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "ICameraSceneNode.h"
#include "IMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMaterialRenderer.h"

namespace geo
{
namespace scene
{



//! constructor
CMeshSceneNode::CMeshSceneNode(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation,
			const core::vector3df& scale)
: IMeshSceneNode(parent, mgr, id, position, rotation, scale), Mesh(0), PassCount(0),
	ReadOnlyMaterials(false)
{
	#ifdef _DEBUG
	setDebugName("CMeshSceneNode");
	#endif

	setMesh(mesh);
}



//! destructor
CMeshSceneNode::~CMeshSceneNode()
{
	if (Mesh)
		Mesh->drop();
}



//! frame
void CMeshSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		// because this node supports rendering of mixed mode meshes consisting of 
		// transparent and solid material at the same time, we need to go through all 
		// materials, check of what type they are and register this node for the right
		// render pass according to that.

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		PassCount = 0;
		int transparentCount = 0;
		int solidCount = 0;

		// count transparent and solid materials in this scene node
		if (ReadOnlyMaterials && Mesh)
		{
			// count mesh materials 

			for (u32 i=0; i<Mesh->getMeshBufferCount(); ++i)
			{
				scene::IMeshBuffer* mb = Mesh->getMeshBuffer(i);
				video::IMaterialRenderer* rnd = mb ? driver->getMaterialRenderer(mb->getMaterial().MaterialType) : 0;

				if (rnd && rnd->isTransparent()) 
					++transparentCount;
				else 
					++solidCount;

				if (solidCount && transparentCount)
					break;
			}
		}
		else
		{
			// count copied materials 

			for (u32 i=0; i<Materials.size(); ++i)
			{
				video::IMaterialRenderer* rnd = 
					driver->getMaterialRenderer(Materials[i].MaterialType);

				if (rnd && rnd->isTransparent()) 
					++transparentCount;
				else 
					++solidCount;

				if (solidCount && transparentCount)
					break;
			}	
		}

		// register according to material types counted

		if (solidCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

		if (transparentCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

		ISceneNode::OnRegisterSceneNode();
	}
}



//! renders the node.
void CMeshSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!Mesh || !driver)
		return;

	bool isTransparentPass = 
		SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;

	++PassCount;

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	Box = Mesh->getBoundingBox();

	// for debug purposes only:

	bool renderMeshes = true;
	video::SMaterial mat;
	if (DebugDataVisible && PassCount==1)
	{
		// overwrite half transparency
		if ( DebugDataVisible & scene::EDS_HALF_TRANSPARENCY )
		{
			for (u32 g=0; g<Mesh->getMeshBufferCount(); ++g)
			{
				mat = Materials[g];
				mat.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
				driver->setMaterial(mat);
				driver->drawMeshBuffer(Mesh->getMeshBuffer(g));
			}
			renderMeshes = false;
		}
	}

	// render original meshes
	if ( renderMeshes )
	{
		for (u32 i=0; i<Mesh->getMeshBufferCount(); ++i)
		{
			scene::IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			if (mb)
			{
				const video::SMaterial& material = ReadOnlyMaterials ? mb->getMaterial() : Materials[i];

				video::IMaterialRenderer* rnd = driver->getMaterialRenderer(material.MaterialType);
				bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass) 
				{
					driver->setMaterial(material);
					driver->drawMeshBuffer(mb);
				}
			}
		}
	}

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	// for debug purposes only:
	if ( DebugDataVisible && PassCount==1)
	{
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);

		if ( DebugDataVisible & scene::EDS_BBOX )
		{
			driver->draw3DBox(Box, video::SColor(255,255,255,255));
		}
		if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
		{
			for (u32 g=0; g<Mesh->getMeshBufferCount(); ++g)
			{
				driver->draw3DBox(
					Mesh->getMeshBuffer(g)->getBoundingBox(),
					video::SColor(255,190,128,128));
			}
		}

		if ( DebugDataVisible & scene::EDS_NORMALS )
		{
			IAnimatedMesh * arrow = SceneManager->addArrowMesh (
					"__debugnormal", 0xFFECEC00,
					0xFF999900, 4, 8, 1.f, 0.6f, 0.05f,
					0.3f);
			IMesh *mesh = arrow->getMesh(0);

			// find a good scaling factor

			core::matrix4 m2;

			// draw normals
			for (u32 g=0; g<Mesh->getMeshBufferCount(); ++g)
			{
				const scene::IMeshBuffer* mb = Mesh->getMeshBuffer(g);
				const u32 vSize = video::getVertexPitchFromType(mb->getVertexType());
				const video::S3DVertex* v = ( const video::S3DVertex*)mb->getVertices();
				for ( u32 i=0; i != mb->getVertexCount(); ++i )
				{
					// align to v->Normal
					core::quaternion quatRot(v->Normal.X, 0.f, -v->Normal.X, 1+v->Normal.Y);
					quatRot.normalize();
					quatRot.getMatrix(m2);

					m2.setTranslation(v->Pos);
					m2*=AbsoluteTransformation;

					driver->setTransform(video::ETS_WORLD, m2);
					for (u32 a = 0; a != mesh->getMeshBufferCount(); ++a)
						driver->drawMeshBuffer(mesh->getMeshBuffer(a));

					v = (const video::S3DVertex*) ( (u8*) v + vSize );
				}
			}
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		}

		// show mesh
		if ( DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY )
		{
			m.Wireframe = true;
			driver->setMaterial(m);

			for (u32 g=0; g<Mesh->getMeshBufferCount(); ++g)
			{
				driver->drawMeshBuffer( Mesh->getMeshBuffer(g) );
			}
		}
	}
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CMeshSceneNode::getBoundingBox() const
{
	return Mesh ? Mesh->getBoundingBox() : Box;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hierarchy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CMeshSceneNode::getMaterial(u32 i)
{
	if (Mesh && ReadOnlyMaterials && i<Mesh->getMeshBufferCount())
	{
		tmpReadOnlyMaterial = Mesh->getMeshBuffer(i)->getMaterial();
		return tmpReadOnlyMaterial;
	}

	if ( i >= Materials.size())
		return ISceneNode::getMaterial(i);

	return Materials[i];
}



//! returns amount of materials used by this scene node.
u32 CMeshSceneNode::getMaterialCount() const
{
	if (Mesh && ReadOnlyMaterials)
		return Mesh->getMeshBufferCount();

	return Materials.size();
}



//! Sets a new mesh
void CMeshSceneNode::setMesh(IMesh* mesh)
{
	if (!mesh)
		return; // won't set null mesh

	if (Mesh)
		Mesh->drop();

	Mesh = mesh;
	copyMaterials();

	if (Mesh)
		Mesh->grab();
}


void CMeshSceneNode::copyMaterials()
{
	Materials.clear();

	if (Mesh)
	{
		video::SMaterial mat;

		for (u32 i=0; i<Mesh->getMeshBufferCount(); ++i)
		{
			IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			if (mb)
				mat = mb->getMaterial();

			Materials.push_back(mat);
		}
	}
}

//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
/* In this way it is possible to change the materials a mesh causing all mesh scene nodes 
referencing this mesh to change too. */
void CMeshSceneNode::setReadOnlyMaterials(bool readonly)
{
	ReadOnlyMaterials = readonly;
}

//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
bool CMeshSceneNode::isReadOnlyMaterials() const
{
	return ReadOnlyMaterials;
}


//! Creates a clone of this scene node and its children.
ISceneNode* CMeshSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent) newParent = Parent;
	if (!newManager) newManager = SceneManager;

	CMeshSceneNode* nb = new CMeshSceneNode(Mesh, newParent, 
		newManager, ID, RelativeTranslation, RelativeRotation, RelativeScale);

	nb->cloneMembers(this, newManager);
	nb->ReadOnlyMaterials = ReadOnlyMaterials;
	nb->Materials = Materials;

	nb->drop();
	return nb;
}



} // end namespace scene
} // end namespace geo


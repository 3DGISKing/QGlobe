// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "AtmosphereSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "CGeometryCreator.h"
#include "S3DVertex.h"
#include "os.h"

namespace geo
{
namespace scene
{

//! constructor
AtmosphereSceneNode::AtmosphereSceneNode(f32 radius, f32 layerDepth, u32 polyCountX, u32 polyCountY, 
										 ISceneNode* parent, ISceneManager* mgr, s32 id, const core::vector3df& position)
: IAtmosphereNode(radius, layerDepth, parent, mgr, id, position), Mesh(0),
	Radius(radius), PolyCountX(polyCountX), PolyCountY(polyCountY)
{
	#ifdef _DEBUG
	setDebugName("AtmosphereSceneNode");
	#endif

	Mesh = CGeometryCreator::createSphereMesh(radius+layerDepth, polyCountX, polyCountY);
	_AtmosphereAffector = new AtmosphereAffector(radius, layerDepth);
}



//! destructor
AtmosphereSceneNode::~AtmosphereSceneNode()
{
	if (Mesh)
		Mesh->drop();

	if(_AtmosphereAffector)
		delete _AtmosphereAffector;
}


//! renders the node.
void AtmosphereSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (Mesh && driver)
	{
		video::SMaterial& material = Mesh->getMeshBuffer(0)->getMaterial();
#if ATMOSPHERE_BUG_FIX
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
		glEnable(GL_BLEND);							// Enable Blending       (disable alpha testing)
		material.Lighting = false;
#endif

		material.BackfaceCulling = false;
		material.FrontfaceCulling = true;

		driver->setMaterial(material);

		core::matrix4 matTrans;
		matTrans.setTranslation(-getPosition());

		driver->setTransform (video::ETS_WORLD, matTrans);

		driver->drawMeshBuffer(Mesh->getMeshBuffer(0));

		//glMatrixMode(GL_PROJECTION);
		//glPopMatrix();
		//glMatrixMode(GL_MODELVIEW);
		//glPopMatrix();

		if ( DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY )
		{
			video::SMaterial debug_mat;
			debug_mat.Lighting = false;
			debug_mat.Wireframe = true;
			debug_mat.ZBuffer = true;
			driver->setMaterial(debug_mat);
			driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
		}

#if ATMOSPHERE_BUG_FIX
		glDisable(GL_BLEND);	
#endif
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& AtmosphereSceneNode::getBoundingBox() const
{
	return Mesh ? Mesh->getBoundingBox() : Box;
}


void AtmosphereSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

	if(IsVisible)
	{
		ICameraSceneNode* camera = SceneManager->getActiveCamera();

		if(camera)
		{
			core::vector3df CameraPos = getPosition();
			S3DVertex* vertexArray = (S3DVertex*)Mesh->getMeshBuffer(0)->getVertices();
			_AtmosphereAffector->ApplyColor(CameraPos, LightDirection, vertexArray, Mesh->getMeshBuffer(0)->getVertexCount());
		}
	}
	ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& AtmosphereSceneNode::getMaterial(u32 i)
{
	if (i>0 || !Mesh)
		return ISceneNode::getMaterial(i);
	else
		return Mesh->getMeshBuffer(i)->getMaterial();
}


//! returns amount of materials used by this scene node.
u32 AtmosphereSceneNode::getMaterialCount() const
{
	return 1;
}

} // end namespace scene
} // end namespace geo


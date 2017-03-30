#ifndef __C_ATMOSPHERE_SCENE_NODE_H_INCLUDED__
#define __C_ATMOSPHERE_SCENE_NODE_H_INCLUDED__

#include "IAtmosphereNode.h"
#include "IMesh.h"
#include "AtmosphereAffector.h"

namespace geo
{
namespace scene
{

	class AtmosphereSceneNode : public IAtmosphereNode
	{
	public:

		//! constructor
		AtmosphereSceneNode(f32 radius, f32 layerDepth, u32 polyCountX, u32 polyCountY, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0));

		//! destructor
		virtual ~AtmosphereSceneNode();

		virtual void setLightDirection(core::vector3df& lightDir) { LightDirection = lightDir; }

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

		//! returns amount of materials used by this scene node.
		virtual u32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SPHERE; }

		//! The mesh cannot be changed
		virtual void setMesh(IMesh* mesh) {}

		//! Returns the current mesh
		virtual IMesh* getMesh() { return Mesh; }

		//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
		/* In this way it is possible to change the materials a mesh causing all mesh scene nodes 
		referencing this mesh to change too. */
		virtual void setReadOnlyMaterials(bool readonly) {}

		//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
		virtual bool isReadOnlyMaterials() const { return false; }

	private:

		IMesh* Mesh;
		core::aabbox3d<f32> Box;
		f32 Radius;
		u32 PolyCountX;
		u32 PolyCountY;

		AtmosphereAffector* _AtmosphereAffector;
		core::vector3df LightDirection;
	};

}
}

#endif //__C_ATMOSPHERE_SCENE_NODE_H_INCLUDED__
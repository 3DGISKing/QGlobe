
#ifndef __C_GDM_GRID_SCENE_NODE_H_INCLUDED__
#define __C_GDM_GRID_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"

namespace geo
{
	namespace scene
	{

		typedef struct _grid_one_line
		{
			geo::s32	type;
			geo::core::array<geo::core::vector3df>		gridLineArray;
		} GRID_ONE_LINE;

		class CGeoGridSceneNode : public ISceneNode
		{
		public:

			//! constructor
			CGeoGridSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

			// 
			virtual ~CGeoGridSceneNode();

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			//! This method is called just before the rendering process of the whole scene.
			virtual void OnRegisterSceneNode();

			//! does nothing.
			virtual void render();

		private:
		    core::aabbox3d<f32> Box;
			core::array<geo::scene::GRID_ONE_LINE> m_gridArray;
			core::array<geo::scene::GRID_ONE_LINE> m_gridTropicArray;
			
			void* UserData;
		    video::SMaterial mat;
		};

	} // end namespace scene
} // end namespace geo

#endif

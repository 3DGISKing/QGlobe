
#ifndef __C_GDM_SHAPE_SCENE_NODE_H_INCLUDED__
#define __C_GDM_SHAPE_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
//added by RSH 2013.3.13
#include "IVideoDriver.h"
//end

#define MAX_POLYGON_COUNT 1000

struct LineSegment
{
	geo::s32	type;
	geo::core::array<geo::core::vector3df>	pts;
	int ID; 
} ;

struct PolyLinesegment
{
	int			nCount;
	LineSegment	polyList[MAX_POLYGON_COUNT];
} ;

#include "GDM_DataMgr.h"
void get2dVertexLinesegmentArray(const GDM_LOGIC_TILE *ptile, PolyLinesegment* linesegment_arr);

namespace geo
{
	namespace scene
	{

		class CGeoShapeSceneNode : public ISceneNode
		{
		public:

			//! constructor
			CGeoShapeSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

			// 
			virtual ~CGeoShapeSceneNode();

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			//! This method is called just before the rendering process of the whole scene.
			virtual void OnRegisterSceneNode();

			//! does nothing.
			virtual void render();

			geo::f32 getWidthFrom(geo::s32 nType, geo::video::SColor& color);
			void      DrawRenderTilesBoundry();

		private:

			core::aabbox3d<f32> Box;
			PolyLinesegment	_PolyList;
			
			//added by RSH 2013.3.13
			inline void ToggleColorType(int * type);
			void DrawRailway(LineSegment*line, video::IVideoDriver* driver, int level, int * lastType);
			//end addition
		};

	} // end namespace scene
} // end namespace geo

#endif

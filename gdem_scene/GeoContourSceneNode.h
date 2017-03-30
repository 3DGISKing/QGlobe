
#ifndef __C_GDM_CONTOUR_SCENE_NODE_H_INCLUDED__
#define __C_GDM_CONTOUR_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#define MAX_CONTOUR_POLYGON_COUNT 800

typedef struct _contourPolygonData
{
	int			nAltitude;
	int			nWidth;
	int			color;
	bool		close;
	geo::core::array<geo::core::vector3df>		pts;
} CONTOUR_POLYGON_DATA;

typedef struct _contourPolygonList
{
	int						nCount;
	CONTOUR_POLYGON_DATA 	polyList[MAX_CONTOUR_POLYGON_COUNT];
} CONTOUR_POLYGON_LIST;

typedef struct _contourInterFunc
{
	double a;
	double b;
	double c;
	double d;

} CONTOUR_INTERPOLATION_FUNC;

namespace geo
{
	namespace scene
	{

		class CGeoContourSceneNode : public ISceneNode
		{
		public:

			//! constructor
			CGeoContourSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

			// 
			virtual ~CGeoContourSceneNode();

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			//! This method is called just before the rendering process of the whole scene.
			virtual void OnRegisterSceneNode();

			//! does nothing.
			virtual void render();

			geo::s32 getWidthFrom(geo::s32 nType, geo::video::SColor& color);

		private:

			core::aabbox3d<f32> Box;
			
			void* UserData;
		};

	} // end namespace scene
} // end namespace geo

#endif

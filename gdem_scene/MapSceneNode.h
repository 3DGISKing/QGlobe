#pragma once

#include "ISceneNode.h"
#include "rendercursor.h"
#include "MapInfo.h"
#include <QtCore/QList>
namespace geo
{
	namespace scene
	{
		
		class MapSceneNode : public ISceneNode
		{
		public:
			//! constructor
			MapSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));
		
			//! This method is called just before the rendering process of the whole scene.
			virtual void OnRegisterSceneNode();

			virtual void render();
			//! returns the axis aligned bounding box of this node

			virtual const core::aabbox3d<f32>& getBoundingBox() const;
			void                    mousePressEvent(CursorState *event);
			~MapSceneNode(void);
		private:
			video::SMaterial        material;
			geo::video::ITexture*	m_pMap;

			QList<MapInfo>          m_MapList;

			core::aabbox3d<f32>     Box;
			float                   iconScale;
			int                     borderWidth;
			int                     crossWidth;
			int                     moveToHeight;
			int                     moveToTime;

			double                  mapLeft,mapRight,mapTop,mapBottom;
			CGDM_Rect2D             viewExtent,viewExtentInMap;

			void                    setViewExtent();
			void                    drawMapAndOutline();
			void                    drawViewRect();
			void                    drawCross();
			int                     longitudeToScreenX(double longi);
			int                     latitudeToScreenY(double lat);
			double                  screenXToLongitude(int x);
			double                  screenYToLatitude(int y);
	               
		};
	}
}


		 

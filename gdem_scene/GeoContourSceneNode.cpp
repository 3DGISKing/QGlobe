
#include "GeoContourSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "SColor.h"
#include "vector3d.h"
#include "GDM_Mem.h"
#include "../gdem_libmain/GDM_DataMgr.h"

extern int	gdm_GetGeoContourInfo(int* pIndex, CONTOUR_POLYGON_LIST* pGeoContourArray);

namespace geo
{
	namespace scene
	{
		// constructor
		CGeoContourSceneNode::CGeoContourSceneNode(ISceneNode* parent, ISceneManager* mgr, 
			s32 id, const core::vector3df& position,
			const core::vector3df& rotation, const core::vector3df& scale)
			: ISceneNode(parent, mgr, id, position)
		{
#ifdef _DEBUG
			setDebugName("CGeoContourSceneNode");
#endif
			setAutomaticCulling(scene::EAC_OFF);

			UserData = NULL;
		}

		CGeoContourSceneNode::~CGeoContourSceneNode()
		{
		}

		//! pre render event
		void CGeoContourSceneNode::OnRegisterSceneNode()
		{
			if (IsVisible)
				SceneManager->registerNodeForRendering(this);

			ISceneNode::OnRegisterSceneNode();
		}


		//! render
		void CGeoContourSceneNode::render()
		{
			if(!(gdm_GetDataMgr()->m_sOption.blContour)) 	
				return ;

			video::IVideoDriver* driver = SceneManager->getVideoDriver();
			CONTOUR_POLYGON_LIST	polyArray;
			geo::s32 nTileIndex = 0;
			geo::video::SColor color(255, 255, 255, 0);

			video::SMaterial mat;
			mat.Lighting = false;
			mat.Wireframe = true;
			mat.ZBuffer = true;
			mat.Thickness = 1;
			driver->setMaterial(mat);

			while(1)
			{
				driver->setTransform (video::ETS_WORLD, core::IdentityMatrix);

				s32 nCount = 0;
				polyArray.nCount = 0;

				nCount = gdm_GetGeoContourInfo(&nTileIndex, &polyArray);

				if(nCount <= 0)
					break;

				// For use with geomorphing
				for(s32 index = 0 ; index < polyArray.nCount ; index++)
				{
					mat.Thickness = (f32)polyArray.polyList[index].nWidth;
					driver->setMaterial(mat);
					color.set(polyArray.polyList[index].color);
					driver->draw3DPolyline(polyArray.polyList[index].pts, false, color);					
				}
				nTileIndex++;
			}
		}
		
		geo::s32 CGeoContourSceneNode::getWidthFrom(geo::s32 nType, geo::video::SColor& color)
		{
			return 1;
		}

		//! returns the axis aligned bounding box of this node
		const core::aabbox3d<f32>& CGeoContourSceneNode::getBoundingBox() const
		{
			return Box;
		}

	} // end namespace scene
} // end namespace geo

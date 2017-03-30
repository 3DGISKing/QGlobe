
#ifndef __C_DRAW_SCENE_NODE_H_INCLUDED__
#define __C_DRAW_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"

namespace geo{
	namespace video{
		class IVideoDriver;
	}
}

class CGDM_SimMgr;
class CGeoFont;
struct GDM_SIM_STATUS;

namespace geo
{
	namespace scene
	{

		class CSimSceneNode : public ISceneNode
		{
		public:

			//! constructor
			CSimSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0));
				
			// 
			virtual ~CSimSceneNode();

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			//! This method is called just before the rendering process of the whole scene.
			virtual void OnRegisterSceneNode();

			//! does nothing.
			virtual void render();


			void	SetFont(CGeoFont* pFont) {m_pFont = pFont;}
			void	SetSimMgr(CGDM_SimMgr *pMgr) {m_pSimMgr = pMgr;}

		private:
			void	PrePaint();
			void	PostPaint();

			// For flight simulation
			void	PaintFixedArea_Flight();
			void	PaintSpeedArea_Flight();
			void	PaintHeadingArea_Flight();
			void	PaintBankAngleArea_Flight();
			void	PaintVerticalSpeedArea_Flight();
			void	PaintExitButtonArea_Flight();
			void	PaintThrottleArea_Flight();
			void	PaintRudderArea_Flight();
			void	PaintAileronArea_Flight();
			void	PaintElevatorArea_Flight();
			void	PaintFlapGearArea_Flight();
			void	PaintPitchAngleArea_Flight();
			void	PaintAltitudeArea_Flight();

			// For car simulation
			void    PaintSpeedArea_Car();
			void	PaintTravelTimeArea_Car();
			void	PaintTravelDistanceArea_Car();
			void	PaintDirectionAngleArea_Car();

			// GL Drawing
			void	Draw2DLine( float x1, float y1, float x2, float y2 );
			void	Draw2DPolyLines( float pts[], int num );
			void	Draw2DPolygon( float pts[], int num );
			void	DrawOutlineString( float x, float y, QString& str, float scale = 1.0f );



		private:

			core::aabbox3d<f32>		Box;
			video::SMaterial		mat;

		private:
			CGeoFont*				m_pFont;
			CGDM_SimMgr*			m_pSimMgr;
			GDM_SIM_STATUS*			m_simStatus;
			video::IVideoDriver*	m_painter;
		};

	} // end namespace scene
} // end namespace geo

#endif

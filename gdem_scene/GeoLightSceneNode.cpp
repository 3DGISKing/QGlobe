#include "GeoLightSceneNode.h"
#include "ISceneManager.h"
#include "IMeshSceneNode.h"
#include "IMesh.h"
#include "IMeshBuffer.h"

#include "../gdem_libmain/GDM_DataMgr.h" //for use of g_OffsetBodyPt

#define DEBUG_SHOW_POSITION

namespace geo
{
	using namespace core;

	namespace scene
	{
		// constructor
		CGeoLightSceneNode::CGeoLightSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, video::SColorf color, f32 range)
			: CLightSceneNode(parent, mgr, id, position,color,range)
		{
#ifdef _DEBUG
			setDebugName("CGeoLightSceneNode");
#endif

#ifdef DEBUG_SHOW_POSITION
			sphere=SceneManager->addSphereSceneNode(1000,16,NULL,-1,position);
			sphere->getMesh()->getMeshBuffer(0)->getMaterial().Lighting=false;
			sphere->getMesh()->getMeshBuffer(0)->getMaterial().DiffuseColor.set(255,255,0,0);
			sphere->getMesh()->getMeshBuffer(0)->getMaterial().EmissiveColor.set(255,255,0,0);
#endif
			LightPosition=position;

			video::SLight& lightdata=getLightData();

			lightdata.Attenuation.X=1.0f;
		
			SetLightPosition(vector3df(0.0f,0.0f,0.0f));
		}

		void CGeoLightSceneNode::render()
		{	
			vector3df pos=LightPosition;
#ifdef DEBUG_SHOW_POSITION
			pos.set(pos.X-g_OffsetBodyPt.m_tX,pos.Y+g_OffsetBodyPt.m_tY,pos.Z-g_OffsetBodyPt.m_tZ);
			sphere->setPosition(pos);
			sphere->updateAbsolutePosition();
#endif
			setPosition(pos);
			updateAbsolutePosition();

			setAutomaticCulling(scene::EAC_OFF);

			CLightSceneNode::render();
		}


		void CGeoLightSceneNode::SetLightAttenuation(f32 kc,f32 kl,f32 kq)
		{
			video::SLight& data=this->getLightData();

			data.Attenuation.X=kc;
			data.Attenuation.Y=kl;
			data.Attenuation.Z=kq;
		}

	} // end namespace scene
} // end namespace geo

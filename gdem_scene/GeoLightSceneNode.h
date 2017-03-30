#ifndef CAMERA_SCENE_NODE_H
#define CAMERA_SCENE_NODE_H

#include "CLightSceneNode.h"

namespace geo
{
	namespace scene
	{
		class IMeshSceneNode;

		class CGeoLightSceneNode : public CLightSceneNode
		{
		public:
			//! constructor
			CGeoLightSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position, video::SColorf color, f32 range);
			
			virtual void render();

			inline void            SetLightPosition(const core::vector3df pos)
			{
				LightPosition=pos;
				setPosition(pos);
				updateAbsolutePosition();
			};
			inline core::vector3df GetLightPosition(){return LightPosition;};

			void   SetLightAttenuation(f32 kc,f32 kl,f32 kq);

		private:
			IMeshSceneNode*       sphere;
			core::vector3df       LightPosition;
		};

	} // end namespace scene
} // end namespace geo

#endif

#ifndef _IATMOSPHERENODE_H_
#define _IATMOSPHERENODE_H_

#include "IMeshSceneNode.h"

namespace geo
{
namespace scene
{
	
	class IAtmosphereNode : public IMeshSceneNode
	{
	public:
		IAtmosphereNode(f32 radius, f32 layerDepth, ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0))
				: IMeshSceneNode(parent, mgr, id, position, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0)) {}
		virtual ~IAtmosphereNode() {}

	public:
		virtual void setLightDirection(core::vector3df& lightDir) = 0;

	};
}
}
#endif //_IATMOSPHERENODE_H_
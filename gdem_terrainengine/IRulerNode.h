
#ifndef __I_GEO_RULER_NODE_H_INCLUDED__
#define __I_GEO_RULER_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "GeoRenderCallBack.h"

namespace geo
{
	namespace scene
	{

		class IRulerNode : public ISceneNode 
		{
		public:
			//
			IRulerNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
				: ISceneNode(parent, mgr, id) {}
			virtual ~IRulerNode() {}

			virtual void setRulerCalcCB(GEO_CALLBACK_GETDATA shapeCB, void* pUserData) = 0;
		};

	}//namespace scene
}//namespace geo

#endif

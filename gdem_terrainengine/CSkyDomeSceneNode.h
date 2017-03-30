// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h
// Code for this scene node has been contributed by Anders la Cour-Harbo (alc)

#ifndef __C_SKY_DOME_SCENE_NODE_H_INCLUDED__
#define __C_SKY_DOME_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "SMeshBuffer.h"

namespace geo
{
namespace scene
{

class CSkyDomeSceneNode : public ISceneNode
{
	public:
		CSkyDomeSceneNode(video::ITexture* texture, u32 horiRes, u32 vertRes,
			f64 texturePercentage, f64 spherePercentage,
			ISceneNode* root, ISceneManager* smgr, s32 id);
		virtual ~CSkyDomeSceneNode();
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		virtual video::SMaterial& getMaterial(u32 i);
		virtual u32 getMaterialCount() const;
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SKY_BOX; }

	private:
		SMeshBuffer* Buffer;
};


}
}

#endif


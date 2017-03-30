// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_GEOMETRY_CREATOR_H_INCLUDED__
#define __C_GEOMETRY_CREATOR_H_INCLUDED__

#include "IMesh.h"
#include "IImage.h"



namespace geo
{
namespace video
{
	class IVideoDriver;
	class SMaterial;
}

namespace scene
{

//! class for creating geometry on the fly
class CGeometryCreator
{
public:

	static IMesh* createHillPlaneMesh(
		const core::dimension2d<f32>& tileSize, const core::dimension2d<u32>& tileCount,
		video::SMaterial* material, f32 hillHeight, const core::dimension2d<f32>& countHills,
		const core::dimension2d<f32>& textureRepeatCount);

	static IMesh* createTerrainMesh(video::IImage* texture,
		video::IImage* heightmap, const core::dimension2d<f32>& stretchSize,
		f32 maxHeight, video::IVideoDriver* driver,
		const core::dimension2d<s32>& defaultVertexBlockSize,
		bool debugBorders=false);

	static IMesh* createArrowMesh(const u32 tesselationCylinder,
			const u32 tesselationCone, const f32 height,
			const f32 cylinderHeight, const f32 width0,
			const f32 width1, const video::SColor vtxColor0,
			const video::SColor vtxColor1);

	static IMesh* createSphereMesh(f32 radius, u32 polyCountX, u32 polyCountY);

	static IMesh* createCylinderMesh(f32 radius, f32 length, u32 tesselation, const video::SColor& color=video::SColor(0xffffffff), bool closeTop=true, f32 oblique=0.f);

	static IMesh* createConeMesh(f32 radius, f32 length, u32 tesselation, const video::SColor& colorTop=video::SColor(0xffffffff), const video::SColor& colorBottom=video::SColor(0xffffffff), f32 oblique=0.f);
};


} // end namespace scene
} // end namespace geo

#endif


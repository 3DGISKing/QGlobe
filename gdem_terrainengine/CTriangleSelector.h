// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_TRIANGLE_SELECTOR_H_INCLUDED__
#define __C_TRIANGLE_SELECTOR_H_INCLUDED__

#include "ITriangleSelector.h"
#include "IMesh.h"
#include "geoArray.h"

namespace geo
{
namespace scene
{

class ISceneNode;

//! Stupid triangle selector without optimization
class CTriangleSelector : public ITriangleSelector
{
public:

	//! Constructs a selector based on a mesh
	CTriangleSelector(const ISceneNode* node);

	//! Constructs a selector based on a mesh
	CTriangleSelector(const IMesh* mesh, const ISceneNode* node);

	//! Constructs a selector based on a bounding box
	CTriangleSelector(const core::aabbox3d<f32>& box, const ISceneNode* node);

	//! Gets all triangles.
	void getTriangles(core::triangle3df* triangles, s32 arraySize, s32& outTriangleCount, 
		const core::matrix4* transform=0) const;

	//! Gets all triangles which lie within a specific bounding box.
	void getTriangles(core::triangle3df* triangles, s32 arraySize, s32& outTriangleCount, 
		const core::aabbox3d<f32>& box, const core::matrix4* transform=0) const;

	//! Gets all triangles which have or may have contact with a 3d line.
	virtual void getTriangles(core::triangle3df* triangles, s32 arraySize,
		s32& outTriangleCount, const core::line3d<f32>& line, 
		const core::matrix4* transform=0) const;

	//! Returns amount of all available triangles in this selector
	virtual s32 getTriangleCount() const;

protected:

	const ISceneNode* SceneNode;
	mutable core::array<core::triangle3df> Triangles;
};

} // end namespace scene
} // end namespace geo


#endif


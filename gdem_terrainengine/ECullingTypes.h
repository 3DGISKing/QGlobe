// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __E_CULLING_TYPES_H_INCLUDED__
#define __E_CULLING_TYPES_H_INCLUDED__

#include "geoTypes.h"

namespace geo
{
namespace scene
{

	//! An enumeration for all types of automatic culling for built-in scene nodes
	enum E_CULLING_TYPE
	{
		EAC_OFF = 0,
		EAC_BOX = 1,
		EAC_FRUSTUM_BOX = 2,
		EAC_FRUSTUM_SPHERE = 4
	};

	//! Names for culling type
	const c8* const AutomaticCullingNames[] =
	{
		"false",
		"box",			// camera box against node box
		"frustum_box",		// camera frustum against node box
		"frustum_sphere",	// camera frustum against node sphere
		0
	};

} // end namespace scene
} // end namespace geo


#endif // __E_CULLING_TYPES_H_INCLUDED__


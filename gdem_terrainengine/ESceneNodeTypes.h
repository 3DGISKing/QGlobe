// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __E_SCENE_NODE_TYPES_H_INCLUDED__
#define __E_SCENE_NODE_TYPES_H_INCLUDED__

#include "geoTypes.h"

namespace geo
{
namespace scene
{

	//! An enumeration for all types of built-in scene nodes
	/** A scene node type is represented by a four character code
	such as 'cube' or 'mesh' instead of simple numbers, to avoid
	name clashes with external scene nodes.*/
	enum ESCENE_NODE_TYPE
	{
		//! simple cube scene node
		ESNT_CUBE           = MAKE_GEO_ID('c','u','b','e'),

		//! Sphere scene node
		ESNT_SPHERE         = MAKE_GEO_ID('s','p','h','r'),

		//! Text Scene Node
		ESNT_TEXT           = MAKE_GEO_ID('t','e','x','t'),

		//! Water Surface Scene Node
		ESNT_WATER_SURFACE  = MAKE_GEO_ID('w','a','t','r'),

		//! Terrain Scene Node
		ESNT_TERRAIN        = MAKE_GEO_ID('t','e','r','r'),

		//! Sky Box Scene Node
		ESNT_SKY_BOX        = MAKE_GEO_ID('s','k','y','_'),

		//! Shadow Volume Scene Node
		ESNT_SHADOW_VOLUME  = MAKE_GEO_ID('s','h','d','w'),

		//! OctTree Scene Node
		ESNT_OCT_TREE       = MAKE_GEO_ID('o','c','t','t'),

		//! Mesh Scene Node
		ESNT_MESH           = MAKE_GEO_ID('m','e','s','h'),

		//! Light Scene Node
		ESNT_LIGHT          = MAKE_GEO_ID('l','g','h','t'),

		//! Empty Scene Node
		ESNT_EMPTY          = MAKE_GEO_ID('e','m','t','y'),

		//! Dummy Transformation Scene Node
		ESNT_DUMMY_TRANSFORMATION = MAKE_GEO_ID('d','m','m','y'),

		//! Camera Scene Node
		ESNT_CAMERA         = MAKE_GEO_ID('c','a','m','_'),

		//! Billboard Scene Node
		ESNT_BILLBOARD      = MAKE_GEO_ID('b','i','l','l'),

		//! Animated Mesh Scene Node
		ESNT_ANIMATED_MESH  = MAKE_GEO_ID('a','m','s','h'),

		//! Particle System Scene Node
		ESNT_PARTICLE_SYSTEM = MAKE_GEO_ID('p','t','c','l'),

		//! Quake3 Model Scene Node ( has tag to link to )
		ESNT_MD3_SCENE_NODE  = MAKE_GEO_ID('m','d','3','_'),

		//! Maya Camera Scene Node
		//! Legacy, for loading version <= 1.4.x .irr files
		ESNT_CAMERA_MAYA    = MAKE_GEO_ID('c','a','m','M'),

		//! First Person Shooter Camera
		//! Legacy, for loading version <= 1.4.x .irr files
		ESNT_CAMERA_FPS     = MAKE_GEO_ID('c','a','m','F'),

		//! Unknown scene node
		ESNT_UNKNOWN        = MAKE_GEO_ID('u','n','k','n'),

		//! Will match with any scene node when checking types
		ESNT_ANY            = MAKE_GEO_ID('a','n','y','_'),

		ESNT_MEASURE        = MAKE_GEO_ID('r','u','l','e'),
	};



} // end namespace scene
} // end namespace geo


#endif


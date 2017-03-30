// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __E_MESH_WRITER_ENUMS_H_INCLUDED__
#define __E_MESH_WRITER_ENUMS_H_INCLUDED__

#include "geoTypes.h"

namespace geo
{
namespace scene
{

	//! An enumeration for all supported types of built-in mesh writers
	/** A scene mesh writers is represented by a four character code
	such as 'irrm' or 'coll' instead of simple numbers, to avoid
	name clashes with external mesh writers.*/
	enum EMESH_WRITER_TYPE
	{
		//! Geo Native mesh writer, for static .irrmesh files.
		EMWT_GEO_MESH     = MAKE_GEO_ID('i','r','r','m'),

		//! COLLADA mesh writer for .dae and .xml files
		EMWT_COLLADA      = MAKE_GEO_ID('c','o','l','l'),

		//! STL mesh writer for .stl files
		EMWT_STL          = MAKE_GEO_ID('s','t','l',0),

		//! OBJ mesh writer for .obj files
		EMWT_OBJ          = MAKE_GEO_ID('o','b','j',0)
	};


	//! flags configuring mesh writing
	enum E_MESH_WRITER_FLAGS
	{
		//! no writer flags
		EMWF_NONE = 0,

		//! write lightmap textures out if possible
		EMWF_WRITE_LIGHTMAPS = 0x1,

		//! write in a way that does consume less disk space
		EMWF_WRITE_COMPRESSED = 0x2
	};

} // end namespace scene
} // end namespace geo


#endif // __E_MESH_WRITER_ENUMS_H_INCLUDED__


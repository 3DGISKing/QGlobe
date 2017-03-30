// Copyright (C) 2004 Murphy McCauley
// Copyright (C) 2007-2008 Christian Stehno
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h
/*
 Thanks to:
 Max McGuire for his Flipcode article about WAL textures
 Nikolaus Gebhardt for the Geo 3D engine
*/

#ifndef __C_IMAGE_LOADER_WAL_H_INCLUDED__
#define __C_IMAGE_LOADER_WAL_H_INCLUDED__

#include "IImageLoader.h"

namespace geo 
{
namespace video
{

#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

	 struct SWALHeader {
		char	FrameName[32];
		
		u32	ImageWidth;
		u32	ImageHeight;

		s32	MipmapOffset[4];

		char	NextFrameName[32];

		u32	Flags;		// surface properties, i.e. slick, sky, nodraw
		u32	Contents;	// i.e. solid, clip, area portal
		u32	Value;		// light
    } PACK_STRUCT;

#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif
#undef PACK_STRUCT

//! An Geo image loader for Quake engine WAL textures
class CImageLoaderWAL : public geo::video::IImageLoader
{
public:
	virtual bool isALoadableFileExtension(const c8* fileName) const;

	virtual bool isALoadableFileFormat(geo::io::IReadFile* file) const;

	virtual geo::video::IImage* loadImage(geo::io::IReadFile* file) const;

	virtual IImage* loadImage(void* buffer, s32 size) const { return 0; }

private:
	static s32 DefaultPaletteQ2[256];
};

}
}

#endif

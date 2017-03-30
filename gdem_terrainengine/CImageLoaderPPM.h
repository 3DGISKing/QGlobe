// Copyright (C) 2007-2008 Christian Stehno
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_IMAGE_LOADER_PPM_H_INCLUDED__
#define __C_IMAGE_LOADER_PPM_H_INCLUDED__

#include "geoCompileConfig.h"

#ifdef _GEO_COMPILE_WITH_PPM_LOADER_

#include "IImageLoader.h"
#include "geoString.h"


namespace geo
{
namespace video
{


/*!
	Surface Loader for SUN Pixmaps
*/
class CImageLoaderPPM : public IImageLoader
{
public:

	//! constructor
	CImageLoaderPPM();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	virtual bool isALoadableFileExtension(const c8* fileName) const;

	//! returns true if the file maybe is able to be loaded by this class
	virtual bool isALoadableFileFormat(io::IReadFile* file) const;

	//! creates a surface from the file
	virtual IImage* loadImage(io::IReadFile* file) const;

	virtual IImage* loadImage(void* buffer, s32 size) const { return 0; }


private:
	//! read the next token from file
	void getNextToken(io::IReadFile* file, core::stringc& token) const;
	//! skip to next token (skip whitespace)
	void skipToNextToken(io::IReadFile* file) const;
};

} // end namespace video
} // end namespace geo


#endif
#endif


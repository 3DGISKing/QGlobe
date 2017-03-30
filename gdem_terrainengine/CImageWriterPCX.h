// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef _C_IMAGE_WRITER_PCX_H_INCLUDED__
#define _C_IMAGE_WRITER_PCX_H_INCLUDED__

#include "geoCompileConfig.h"

#ifdef _GEO_COMPILE_WITH_PCX_WRITER_

#include "IImageWriter.h"

namespace geo
{
namespace video
{

class CImageWriterPCX : public IImageWriter
{
public:
	//! constructor
	CImageWriterPCX();

	//! return true if this writer can write a file with the given extension
	virtual bool isAWriteableFileExtension(const c8* fileName) const;

	//! write image to file
	virtual bool writeImage(io::IWriteFile *file, IImage *image, u32 param) const;
};

} // namespace video
} // namespace geo

#endif
#endif // _C_IMAGE_WRITER_PCX_H_INCLUDED__


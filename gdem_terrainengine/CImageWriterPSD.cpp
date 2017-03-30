// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CImageWriterPSD.h"

#ifdef _GEO_COMPILE_WITH_PSD_WRITER_

#include "CImageLoaderPSD.h"
#include "IWriteFile.h"
#include "os.h" // for logging
#include "geoString.h"

namespace geo
{
namespace video
{

IImageWriter* createImageWriterPSD()
{
	return new CImageWriterPSD;
}

CImageWriterPSD::CImageWriterPSD()
{
#ifdef _DEBUG
	setDebugName("CImageWriterPSD");
#endif
}

bool CImageWriterPSD::isAWriteableFileExtension(const c8* fileName) const
{
	return strstr(fileName, ".psd") != 0;
}

bool CImageWriterPSD::writeImage(io::IWriteFile *file, IImage *image,u32 param) const
{
	os::Printer::log("PSD writer not yet implemented. Image not written.", ELL_WARNING);
	return false;
}

} // namespace video
} // namespace geo

#endif


// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CImageWriterPPM.h"

#ifdef _GEO_COMPILE_WITH_PPM_WRITER_

#include "IWriteFile.h"
#include "IImage.h"
#include "dimension2d.h"
#include "geoString.h"

namespace geo
{
namespace video
{


IImageWriter* createImageWriterPPM()
{
	return new CImageWriterPPM;
}


CImageWriterPPM::CImageWriterPPM()
{
#ifdef _DEBUG
	setDebugName("CImageWriterPPM");
#endif
}


bool CImageWriterPPM::isAWriteableFileExtension(const c8* fileName) const
{
	return strstr(fileName, ".ppm") != 0;
}


bool CImageWriterPPM::writeImage(io::IWriteFile *file, IImage *image, u32 param) const
{
	char cache[70];
	int size;

	const core::dimension2d<s32>& imageSize = image->getDimension();

	const bool binary = false;

	if (binary)
		size = snprintf(cache, 70, "P6\n");
	else
		size = snprintf(cache, 70, "P3\n");

	if (file->write(cache, size) != size)
		return false;

	size = snprintf(cache, 70, "%d %d\n", imageSize.Width, imageSize.Height);
	if (file->write(cache, size) != size)
		return false;

	size = snprintf(cache, 70, "255\n");
	if (file->write(cache, size) != size)
		return false;

	if (binary)
	{
		for (s32 h = 0; h < imageSize.Height; ++h)
		{
			for (s32 c = 0; c < imageSize.Width; ++c)
			{
				const video::SColor& pixel = image->getPixel(c, h);
				const u8 r = (u8)(pixel.getRed() & 0xff);
				const u8 g = (u8)(pixel.getGreen() & 0xff);
				const u8 b = (u8)(pixel.getBlue() & 0xff);
				file->write(&r, 1);
				file->write(&g, 1);
				file->write(&b, 1);
			}
		}
	}
	else
	{
		s32 n = 0;

		for (s32 h = 0; h < imageSize.Height; ++h)
		{
			for (s32 c = 0; c < imageSize.Width; ++c, ++n)
			{
				const video::SColor& pixel = image->getPixel(c, h);
				size = snprintf(cache, 70, "%.3u %.3u %.3u%s", pixel.getRed(), pixel.getGreen(), pixel.getBlue(), n % 5 == 4 ? "\n" : "  ");	
				if (file->write(cache, size) != size)
					return false;
			}
		}
	}

	return true;
}


} // namespace video
} // namespace geo

#endif


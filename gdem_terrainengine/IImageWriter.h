// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef _I_IMAGE_WRITER_H_INCLUDED__
#define _I_IMAGE_WRITER_H_INCLUDED__

#include "IReferenceCounted.h"

namespace geo
{
namespace io
{
	class IWriteFile;
} // end namespace io

namespace video
{
	class IImage;


//! Interface for writing software image data.
class IImageWriter : public IReferenceCounted
{
public:
	//! Check if this writer can write a file with the given extension
	/** \param fileName Name of the file to check.
	\return True if file extension specifies a writable type. */
	virtual bool isAWriteableFileExtension(const c8* fileName) const = 0;

	//! Write image to file
	/** \param file File handle to write to.
	\param image Image to write into file.
	\param param Writer specific parameter, influencing e.g. quality.
	\return True if image was successfully written. */
	virtual bool writeImage(io::IWriteFile *file, IImage *image, u32 param = 0) const = 0;
};

} // namespace video
} // namespace geo

#endif // _I_IMAGE_WRITER_H_INCLUDED__


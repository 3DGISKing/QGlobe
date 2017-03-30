// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __I_SURFACE_LOADER_H_INCLUDED__
#define __I_SURFACE_LOADER_H_INCLUDED__

#include "IReferenceCounted.h"
#include "IImage.h"

namespace geo
{
namespace io
{
	class IReadFile;
} // end namespace io
namespace video
{

//! Class which is able to create a image from a file.
/** If you want the Geo Engine be able to load textures of
currently unsupported file formats (e.g .gif), then implement
this and add your new Surface loader with
IVideoDriver::addExternalImageLoader() to the engine. */
class IImageLoader : public virtual IReferenceCounted
{
public:

	//! Check if the file might be loaded by this class
	/** Check is based on the file extension (e.g. ".tga")
	\param fileName Name of file to check.
	\return True if file seems to be loadable. */
	virtual bool isALoadableFileExtension(const c8* fileName) const = 0;

	//! Check if the file might be loaded by this class
	/** Check might look into the file.
	\param file File handle to check.
	\return True if file seems to be loadable. */
	virtual bool isALoadableFileFormat(io::IReadFile* file) const = 0;

	//! Creates a surface from the file
	/** \param file File handle to check.
	\return Pointer to newly created image, or 0 upon error. */
	virtual IImage* loadImage(io::IReadFile* file) const = 0;

	//! kimyi : Creates a surface from buffer
	/** \param image buffer.
	\return Pointer to newly created image, or 0 upon error. */
	virtual IImage* loadImage(void* buffer, s32 size) const = 0;
};


} // end namespace video
} // end namespace geo

#endif


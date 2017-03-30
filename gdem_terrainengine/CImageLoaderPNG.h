// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

// this file was created by rt (www.tomkorp.com), based on ttk's png-reader
// i wanted to be able to read in PNG images with irrlicht :)
// why?  lossless compression with 8-bit alpha channel!

#ifndef __C_IMAGE_LOADER_PNG_H_INCLUDED__
#define __C_IMAGE_LOADER_PNG_H_INCLUDED__

#include "geoCompileConfig.h"

#ifdef _GEO_COMPILE_WITH_PNG_LOADER_

#include "IImageLoader.h"

namespace geo
{
namespace video
{

//!  Surface Loader for PNG files
class CImageLoaderPng : public IImageLoader
{
public:

   //! returns true if the file maybe is able to be loaded by this class
   //! based on the file extension (e.g. ".png")
   virtual bool isALoadableFileExtension(const c8* fileName) const;

   //! returns true if the file maybe is able to be loaded by this class
   virtual bool isALoadableFileFormat(io::IReadFile* file) const;

   //! creates a surface from the file
   virtual IImage* loadImage(io::IReadFile* file) const;

   virtual IImage* loadImage(void* buffer, s32 size) const { return 0; }
};


} // end namespace video
} // end namespace geo

#endif
#endif


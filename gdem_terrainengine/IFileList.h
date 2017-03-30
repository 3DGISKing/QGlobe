// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __I_FILE_LIST_H_INCLUDED__
#define __I_FILE_LIST_H_INCLUDED__

#include "IReferenceCounted.h"

namespace geo
{
namespace io
{

//! The Filelist lists all files in a directory.
class IFileList : public virtual IReferenceCounted
{
public:
	//! Get the number of files in the filelist.
	/** \return Amount of files and directories in the file list. */
	virtual u32 getFileCount() const = 0;

	//! Gets the name of a file in the list, based on an index.
	/** The path is not included in this name. Use getFullFileName for this.
	\param index is the zero based index of the file which name should
	be returned. The index has to be smaller than the amount getFileCount() returns.
	\return File name of the file. Returns 0, if an error occured. */
	virtual const c8* getFileName(u32 index) const = 0;

	//! Gets the full name of a file in the list, path included, based on an index.
	/** \param index is the zero based index of the file which name should
	be returned. The index has to be smaller than the amount getFileCount() returns.
	\return File name of the file. Returns 0, if an error occured. */
	virtual const c8* getFullFileName(u32 index) = 0;

	//! Check if the file is a directory
	/** \param index The zero based index of the file whose name shall
	be returned. The index has to be smaller than the amount getFileCount() returns.
	\return True if the file is a directory, else false. */
	virtual bool isDirectory(u32 index) const = 0;
};

} // end namespace geo
} // end namespace io


#endif


// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_FILE_LIST_H_INCLUDED__
#define __C_FILE_LIST_H_INCLUDED__

#include "IFileList.h"
#include "geoString.h"
#include "geoArray.h"

namespace geo
{
namespace io
{

/*!
	FileSystem, which manages where files are, so that modules which
	use the the io do not need to know where every file is located.
*/
class CFileList : public IFileList
{
public:

	//! constructor
	CFileList();

	//! Returns the amount of files in the filelist.
	/** \return Amount of files and directories in the file list. */
	virtual u32 getFileCount() const;

	//! Gets the name of a file in the list, based on an index.
	/** \param index is the zero based index of the file which name should
	be returned. The index has to be smaller than the amount getFileCount() returns.
	\return The file name of the file. Returns 0, if an error occured. */
	virtual const c8* getFileName(u32 index) const;

	//! Gets the full name of a file in the list, path included, based on an index.
	virtual const c8* getFullFileName(u32 index);

	//! Returns of the file is a directory
	/** \param index is the zero based index of the file which name should
	be returned. The index has to be smaller than the amount getFileCount() returns.
	\return True if the file is a directory, else false. */
	virtual bool isDirectory(u32 index) const;

private:

	struct FileEntry
	{
		core::stringc Name;
		core::stringc FullName;
		long Size;
		bool isDirectory;

		bool operator <(const struct FileEntry& other) const
		{
			if ( isDirectory ^ other.isDirectory )
				return isDirectory;

			return Name.lower_ignore_case ( other.Name );
		}
	};

	core::stringc Path;
	core::array< FileEntry > Files;
};

} // end namespace geo
} // end namespace io


#endif


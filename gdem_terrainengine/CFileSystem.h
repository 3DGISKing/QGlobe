// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_FILE_SYSTEM_H_INCLUDED__
#define __C_FILE_SYSTEM_H_INCLUDED__

#include "IFileSystem.h"
#include "geoArray.h"

namespace geo
{
namespace io
{

	class CZipReader;
	class CPakReader;
	class CUnZipReader;
	const s32 FILE_SYSTEM_MAX_PATH = 1024;

/*!
	FileSystem which uses normal files and one zipfile
*/
class CFileSystem : public IFileSystem
{
public:

	//! constructor
	CFileSystem();

	//! destructor
	virtual ~CFileSystem();

	//! opens a file for read access
	virtual IReadFile* createAndOpenFile(const c8* filename);

	//! Creates an IReadFile interface for accessing memory like a file.
	virtual IReadFile* createMemoryReadFile(void* memory, s32 len, const c8* fileName, bool deleteMemoryWhenDropped = false);

	//! Opens a file for write access.
	virtual IWriteFile* createAndWriteFile(const c8* filename, bool append=false);

	//! adds an zip archive to the filesystem
	virtual bool addZipFileArchive(const c8* filename, bool ignoreCase = true, bool ignorePaths = true);
	virtual bool addFolderFileArchive(const c8* filename, bool ignoreCase = true, bool ignorePaths = true);

	//! adds an pak archive to the filesystem
	virtual bool addPakFileArchive(const c8* filename, bool ignoreCase = true, bool ignorePaths = true);

	//! Returns the string of the current working directory
	virtual const c8* getWorkingDirectory();

	//! Changes the current Working Directory to the string given.
	//! The string is operating system dependent. Under Windows it will look
	//! like this: "drive:\directory\sudirectory\"
	virtual bool changeWorkingDirectoryTo(const c8* newDirectory);

	//! Converts a relative path to an absolute (unique) path, resolving symbolic links
	virtual core::stringc getAbsolutePath(const core::stringc& filename) const;

	//! Returns the directory a file is located in.
	/** \param filename: The file to get the directory from */
	virtual core::stringc getFileDir(const core::stringc& filename) const;

	//! Returns the base part of a filename, i.e. the name without the directory
	//! part. If no directory is prefixed, the full name is returned.
	/** \param filename: The file to get the basename from */
	core::stringc getFileBasename(const core::stringc& filename, bool keepExtension=true) const;

	//! Creates a list of files and directories in the current working directory 
	//! and returns it.
	virtual IFileList* createFileList() const;

	//! determinates if a file exists and would be able to be opened.
	virtual bool existFile(const c8* filename) const;

	//! determines if a file exists and would be able to be opened.
	bool existFile(const core::stringc& filename) const
	{
		return existFile(filename.c_str());
	}

	//! Creates a XML Reader from a file.
	virtual IXMLReader* createXMLReader(const c8* filename);

	//! Creates a XML Reader from a file.
	virtual IXMLReader* createXMLReader(IReadFile* file);

	//! Creates a XML Reader from a file.
	virtual IXMLReaderUTF8* createXMLReaderUTF8(const c8* filename);

	//! Creates a XML Reader from a file.
	virtual IXMLReaderUTF8* createXMLReaderUTF8(IReadFile* file);

	//! Creates a XML Writer from a file.
	virtual IXMLWriter* createXMLWriter(const c8* filename);

	//! Creates a XML Writer from a file.
	virtual IXMLWriter* createXMLWriter(IWriteFile* file);

	//! Creates a new empty collection of attributes, usable for serialization and more.
	virtual IAttributes* createEmptyAttributes(video::IVideoDriver* driver);

private:

	core::array<CZipReader*> ZipFileSystems;
	core::array<CPakReader*> PakFileSystems;
	core::array<CUnZipReader*> UnZipFileSystems;
	c8 WorkingDirectory[FILE_SYSTEM_MAX_PATH];
};


} // end namespace geo
} // end namespace io

#endif


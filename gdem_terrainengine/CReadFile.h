// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_READ_FILE_H_INCLUDED__
#define __C_READ_FILE_H_INCLUDED__

#include <stdio.h>
#include "IReadFile.h"
#include "geoString.h"

namespace geo
{

namespace io
{

	/*!
		Class for reading a real file from disk.
	*/
	class CReadFile : public IReadFile
	{
	public:

		CReadFile(const wchar_t* fileName);
		CReadFile(const c8* fileName);

		virtual ~CReadFile();

		//! returns how much was read
		virtual s32 read(void* buffer, u32 sizeToRead);

		//! changes position in file, returns true if successful
		virtual bool seek(long finalPos, bool relativeMovement = false);

		//! returns size of file
		virtual long getSize() const;

		//! returns if file is open
		virtual bool isOpen() const
		{
			return File != 0;
		}

		//! returns where in the file we are.
		virtual long getPos() const;

		//! returns name of file
		virtual const c8* getFileName() const;

	private:

		//! opens the file
		void openFile();

		FILE* File;
		long FileSize;
		core::stringc Filename;
	};

} // end namespace io
} // end namespace geo

#endif


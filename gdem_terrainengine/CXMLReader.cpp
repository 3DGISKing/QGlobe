// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CXMLReaderImpl.h"
#include "CXMLReader.h"
#include "IReadFile.h"

namespace geo
{
namespace io
{
	//! Geo implementation of the file read callback for the xml parser
	class CGeoXMLFileReadCallBack : public IFileReadCallBack
	{
	public:

		//! construct from FILE pointer
		CGeoXMLFileReadCallBack(IReadFile* file)
			: ReadFile(file)
		{
			ReadFile->grab();
		}

		//! destructor
		virtual ~CGeoXMLFileReadCallBack()
		{
			ReadFile->drop();
		}

		//! Reads an amount of bytes from the file.
		virtual int read(void* buffer, int sizeToRead)
		{
			return ReadFile->read(buffer, sizeToRead);
		}

		//! Returns size of file in bytes
		virtual long getSize() const
		{
			return ReadFile->getSize();
		}

	private:

		IReadFile* ReadFile;
	}; // end class CMyXMLFileReadCallBack


	// now create an implementation for IXMLReader using irrXML.

	//! Creates an instance of a wide character xml parser. 
	IXMLReader* createIXMLReader(IReadFile* file)
	{
		if (!file)
			return 0;

		return new CXMLReaderImpl<wchar_t, IReferenceCounted>(new CGeoXMLFileReadCallBack(file)); 
	}

	//! Creates an instance of an UFT-8 or ASCII character xml parser. 
	IXMLReaderUTF8* createIXMLReaderUTF8(IReadFile* file)
	{
		if (!file)
			return 0;

		return new CXMLReaderImpl<char, IReferenceCounted>(new CGeoXMLFileReadCallBack(file)); 
	}

} // end namespace
} // end namespace

// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __I_XML_READER_H_INCLUDED__
#define __I_XML_READER_H_INCLUDED__

#include "IReferenceCounted.h"
#include "geoXML.h"

namespace geo
{
namespace io
{
	//! An xml reader for wide characters, derived from IReferenceCounted.
	/** This XML Parser can read any type of text files from any source
	Geo can read. Just call IFileSystem::createXMLReader(). For more
	informations on how to use the parser, see IGeoXMLReader */
	typedef IGeoXMLReader<wchar_t, IReferenceCounted> IXMLReader;

	//! An xml reader for ASCII or UTF-8 characters, derived from IReferenceCounted.
	/** This XML Parser can read any type of text files from any source
	Geo can read. Just call IFileSystem::createXMLReaderUTF8(). For
	more informations on how to use the parser, see IGeoXMLReader */
	typedef IGeoXMLReader<c8, IReferenceCounted> IXMLReaderUTF8;

} // end namespace io
} // end namespace geo

#endif


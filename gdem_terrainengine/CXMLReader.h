// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_XML_READER_H_INCLUDED__
#define __C_XML_READER_H_INCLUDED__

#include "IXMLReader.h"

namespace geo
{
namespace io
{
	class IReadFile;

	//! creates an IXMLReader
	IXMLReader* createIXMLReader(IReadFile* file);

	//! creates an IXMLReader
	IXMLReaderUTF8* createIXMLReaderUTF8(IReadFile* file);

} // end namespace geo
} // end namespace io

#endif


// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "CImageLoaderBMP.h"

#ifdef _GEO_COMPILE_WITH_BMP_LOADER_

#include "IReadFile.h"
#include "SColor.h"
#include "CColorConverter.h"
#include "CImage.h"
#include "os.h"
#include "geoString.h"

namespace geo
{
namespace video
{


//! constructor
CImageLoaderBMP::CImageLoaderBMP()
{
	#ifdef _DEBUG
	setDebugName("CImageLoaderBMP");
	#endif
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".tga")
bool CImageLoaderBMP::isALoadableFileExtension(const c8* fileName) const
{
	return strstr(fileName, ".bmp") != 0;
}


//! returns true if the file maybe is able to be loaded by this class
bool CImageLoaderBMP::isALoadableFileFormat(io::IReadFile* file) const
{
	u16 headerID;
	file->read(&headerID, sizeof(u16));
#ifdef __BIG_ENDIAN__
	headerID = os::Byteswap::byteswap(headerID);
#endif
	return headerID == 0x4d42;
}


void CImageLoaderBMP::decompress8BitRLE(u8*& bmpData, s32 size, s32 width, s32 height, s32 pitch) const
{
	u8* p = bmpData;
	u8* newBmp = new u8[(width+pitch)*height];
	u8* d = newBmp;
	u8* destEnd = newBmp + (width+pitch)*height;
	s32 line = 0;

	while (bmpData - p < size && d < destEnd)
	{
		if (*p == 0)
		{
			++p;

			switch(*p)
			{
			case 0: // end of line
				++p;
				++line;
				d = newBmp + (line*(width+pitch));
				break;
			case 1: // end of bmp
				delete [] bmpData;
				bmpData = newBmp;
				return;
			case 2:
				++p; d +=(u8)*p;  // delta
				++p; d += ((u8)*p)*(width+pitch);
				++p;
				break;
			default:
				{
					// absolute mode
					s32 count = (u8)*p; ++p;
					s32 readAdditional = ((2-(count%2))%2);
					s32 i;

					for (i=0; i<count; ++i)
					{
						*d = *p;
						++p;
						++d;
					}

					for (i=0; i<readAdditional; ++i)
						++p;
				}
			}
		}
		else
		{
			s32 count = (u8)*p; ++p;
			u8 color = *p; ++p;
			for (s32 i=0; i<count; ++i)
			{
				*d = color;
				++d;
			}
		}
	}

	delete [] bmpData;
	bmpData = newBmp;
}


void CImageLoaderBMP::decompress4BitRLE(u8*& bmpData, s32 size, s32 width, s32 height, s32 pitch) const
{
	s32 lineWidth = (width+1)/2+pitch;
	u8* p = bmpData;
	u8* newBmp = new u8[lineWidth*height];
	u8* d = newBmp;
	u8* destEnd = newBmp + lineWidth*height;
	s32 line = 0;
	s32 shift = 4;

	while (bmpData - p < size && d < destEnd)
	{
		if (*p == 0)
		{
			++p;

			switch(*p)
			{
			case 0: // end of line
				++p;
				++line;
				d = newBmp + (line*lineWidth);
				shift = 4;
				break;
			case 1: // end of bmp
				delete [] bmpData;
				bmpData = newBmp;
				return;
			case 2:
				{
					++p;
					s32 x = (u8)*p; ++p;
					s32 y = (u8)*p; ++p;
					d += x/2 + y*lineWidth;
					shift = x%2==0 ? 4 : 0;
				}
				break;
			default:
				{
					// absolute mode
					s32 count = (u8)*p; ++p;
					s32 readAdditional = ((2-((count)%2))%2);
					s32 readShift = 4;
					s32 i;

					for (i=0; i<count; ++i)
					{
						s32 color = (((u8)*p) >> readShift) & 0x0f;
						readShift -= 4;
						if (readShift < 0)
						{
							++*p;
							readShift = 4;
						}

						u8 mask = 0x0f << shift;
						*d = (*d & (~mask)) | ((color << shift) & mask);

						shift -= 4;
						if (shift < 0)
						{
							shift = 4;
							++d;
						}

					}

					for (i=0; i<readAdditional; ++i)
						++p;
				}
			}
		}
		else
		{
			s32 count = (u8)*p; ++p;
			s32 color1 = (u8)*p; color1 = color1 & 0x0f;
			s32 color2 = (u8)*p; color2 = (color2 >> 4) & 0x0f;
			++p;

			for (s32 i=0; i<count; ++i)
			{
				u8 mask = 0x0f << shift;
				u8 toSet = (shift==0 ? color1 : color2) << shift;
				*d = (*d & (~mask)) | (toSet & mask);

				shift -= 4;
				if (shift < 0)
				{
					shift = 4;
					++d;
				}
			}
		}
	}

	delete [] bmpData;
	bmpData = newBmp;
}

IImage* CImageLoaderBMP::loadImage(void* buffer, s32 size) const
{
	u8* pReadBuffer = (u8*)buffer;
	SBMPHeader header;

	memcpy(&header, pReadBuffer, sizeof(header));
	pReadBuffer += sizeof(header);

#ifdef __BIG_ENDIAN__
	header.Id = os::Byteswap::byteswap(header.Id);
	header.FileSize = os::Byteswap::byteswap(header.FileSize);
	header.BitmapDataOffset = os::Byteswap::byteswap(header.BitmapDataOffset);
	header.BitmapHeaderSize = os::Byteswap::byteswap(header.BitmapHeaderSize);
	header.Width = os::Byteswap::byteswap(header.Width);
	header.Height = os::Byteswap::byteswap(header.Height);
	header.Planes = os::Byteswap::byteswap(header.Planes);
	header.BPP = os::Byteswap::byteswap(header.BPP);
	header.Compression = os::Byteswap::byteswap(header.Compression);
	header.BitmapDataSize = os::Byteswap::byteswap(header.BitmapDataSize);
	header.PixelPerMeterX = os::Byteswap::byteswap(header.PixelPerMeterX);
	header.PixelPerMeterY = os::Byteswap::byteswap(header.PixelPerMeterY);
	header.Colors = os::Byteswap::byteswap(header.Colors);
	header.ImportantColors = os::Byteswap::byteswap(header.ImportantColors);
#endif

	s32 pitch = 0;

	//! return if the header is false

	if (header.Id != 0x4d42)
		return 0;

	if (header.Compression > 2) // we'll only handle RLE-Compression
	{
		os::Printer::log("Compression mode not supported.", ELL_ERROR);
		return 0;
	}

	// adjust bitmap data size to dword boundary
	header.BitmapDataSize += (4-(header.BitmapDataSize%4))%4;

	// read palette

	long pos = pReadBuffer - (u8*)buffer;
	s32 paletteSize = (header.BitmapDataOffset - pos) / 4;

	s32* paletteData = 0;
	if (paletteSize)
	{
		paletteData = new s32[paletteSize];
		memcpy(paletteData, pReadBuffer, paletteSize * sizeof(s32));
		pReadBuffer += paletteSize * sizeof(s32);
#ifdef __BIG_ENDIAN__
		for (s32 i=0; i<paletteSize; ++i)
			paletteData[i] = os::Byteswap::byteswap(paletteData[i]);
#endif
	}

	// read image data

	if (!header.BitmapDataSize)
	{
		// okay, lets guess the size
		// some tools simply don't set it
		header.BitmapDataSize = static_cast<u32>(size) - header.BitmapDataOffset;
	}

	pReadBuffer = (u8*)buffer + header.BitmapDataOffset;

	f32 t = (header.Width) * (header.BPP / 8.0f);
	s32 widthInBytes = (s32)t;
	t -= widthInBytes;
	if (t!=0.0f)
		++widthInBytes;

	s32 lineData = widthInBytes + ((4-(widthInBytes%4)))%4;
	pitch = lineData - widthInBytes;

	u8* bmpData = new u8[header.BitmapDataSize];
	memcpy(bmpData, pReadBuffer, header.BitmapDataSize);

	// decompress data if needed
	switch(header.Compression)
	{
	case 1: // 8 bit rle
		decompress8BitRLE(bmpData, header.BitmapDataSize, header.Width, header.Height, pitch);
		break;
	case 2: // 4 bit rle
		decompress4BitRLE(bmpData, header.BitmapDataSize, header.Width, header.Height, pitch);
		break;
	}

	// create surface

	IImage* image = 0;
	switch(header.BPP)
	{
	case 1:
		image = new CImage(ECF_A1R5G5B5, core::dimension2d<s32>(header.Width, header.Height));
		if (image)
			CColorConverter::convert1BitTo16Bit(bmpData, (s16*)image->lock(), header.Width, header.Height, pitch, true);
		break;
	case 4:
		image = new CImage(ECF_A1R5G5B5, core::dimension2d<s32>(header.Width, header.Height));
		if (image)
			CColorConverter::convert4BitTo16Bit(bmpData, (s16*)image->lock(), header.Width, header.Height, paletteData, pitch, true);
		break;
	case 8:
		image = new CImage(ECF_A1R5G5B5, core::dimension2d<s32>(header.Width, header.Height));
		if (image)
			CColorConverter::convert8BitTo16Bit(bmpData, (s16*)image->lock(), header.Width, header.Height, paletteData, pitch, true);
		break;
	case 16:
		image = new CImage(ECF_A1R5G5B5, core::dimension2d<s32>(header.Width, header.Height));
		if (image)
			CColorConverter::convert16BitTo16Bit((s16*)bmpData, (s16*)image->lock(), header.Width, header.Height, pitch, true);
		break;
	case 24:
		image = new CImage(ECF_R8G8B8, core::dimension2d<s32>(header.Width, header.Height));
		if (image)
			CColorConverter::convert24BitTo24Bit(bmpData, (u8*)image->lock(), header.Width, header.Height, pitch, true, true);
		break;
	case 32: // thx to Reinhard Ostermeier
		image = new CImage(ECF_A8R8G8B8, core::dimension2d<s32>(header.Width, header.Height));
		if (image)
			CColorConverter::convert32BitTo32Bit((s32*)bmpData, (s32*)image->lock(), header.Width, header.Height, pitch, true);
		break;
	};
	if (image)
		image->unlock();

	// clean up

	delete [] paletteData;
	delete [] bmpData;

	return image;
}


//! creates a surface from the file
IImage* CImageLoaderBMP::loadImage(io::IReadFile* file) const
{
	s32 filesize = file->getSize();
	u8* input = new u8[filesize];
	file->read(input, filesize);

	IImage* image = loadImage(input, filesize);

	delete [] input;

	return image;
}


//! creates a loader which is able to load windows bitmaps
IImageLoader* createImageLoaderBMP()
{
	return new CImageLoaderBMP;
}


} // end namespace video
} // end namespace geo

#endif


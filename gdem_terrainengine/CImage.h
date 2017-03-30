// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_IMAGE_H_INCLUDED__
#define __C_IMAGE_H_INCLUDED__

#include "IImage.h"
#include "rect.h"

namespace geo
{
namespace video
{

//! IImage implementation with a lot of special image operations for
//! 16 bit A1R5G5B5/32 Bit A8R8G8B8 images, which are used by the SoftwareDevice.
class CImage : public IImage
{
public:

	//! constructor from another image with format change
	CImage(ECOLOR_FORMAT format, IImage* imageToCopy);

	//! constructor from raw image data
	//! \param useForeignMemory: If true, the image will use the data pointer
	//! directly and own it from now on, which means it will also try to delete [] the
	//! data when the image will be destructed. If false, the memory will by copied.
	CImage(ECOLOR_FORMAT format, const core::dimension2d<s32>& size,
		void* data, bool ownForeignMemory=true, bool deleteMemory = true);

	//! constructor for empty image
	CImage(ECOLOR_FORMAT format, const core::dimension2d<s32>& size);

	//! constructor using a part from another image
	CImage(IImage* imageToCopy,
		const core::position2d<s32>& pos, const core::dimension2d<s32>& size);

	//! destructor
	virtual ~CImage();

	//! Lock function.
	virtual void* lock()
	{
		return Data;
	}

	//! Unlock function.
	virtual void unlock() {}

	//! Returns width and height of image data.
	virtual const core::dimension2d<s32>& getDimension() const;

	//! Returns bits per pixel.
	virtual u32 getBitsPerPixel() const;

	//! Returns bytes per pixel
	virtual u32 getBytesPerPixel() const;

	//! Returns image data size in bytes
	virtual u32 getImageDataSizeInBytes() const;

	//! Returns image data size in pixels
	virtual u32 getImageDataSizeInPixels() const;

	//! returns mask for red value of a pixel
	virtual u32 getRedMask() const;

	//! returns mask for green value of a pixel
	virtual u32 getGreenMask() const;

	//! returns mask for blue value of a pixel
	virtual u32 getBlueMask() const;

	//! returns mask for alpha value of a pixel
	virtual u32 getAlphaMask() const;

	//! returns a pixel
	virtual SColor getPixel(u32 x, u32 y) const;

	//! sets a pixel
	virtual void setPixel(u32 x, u32 y, const SColor &color );

	//! returns the color format
	virtual ECOLOR_FORMAT getColorFormat() const;

	//! returns pitch of image
	virtual u32 getPitch() const { return Pitch; }

	//! copies this surface into another, scaling it to fit.
	virtual void copyToScaling(void* target, s32 width, s32 height, ECOLOR_FORMAT format, u32 pitch=0);

	//! copies this surface into another, scaling it to fit.
	virtual void copyToScaling(IImage* target);

	//! copies this surface into another
	virtual void copyTo(IImage* target, const core::position2d<s32>& pos=core::position2d<s32>(0,0));

	//! copies this surface into another
	virtual void copyTo(IImage* target, const core::position2d<s32>& pos, const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect=0);

	//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
	virtual void copyToWithAlpha(IImage* target, const core::position2d<s32>& pos,
			const core::rect<s32>& sourceRect, const SColor &color,
			const core::rect<s32>* clipRect = 0);

	//! copies this surface into another, scaling it to fit, appyling a box filter
	void copyToScalingBoxFilter(IImage* target, s32 bias = 0);

	//! fills the surface with black or white
	virtual void fill(const SColor &color);

	//! draws a rectangle
	void drawRectangle(const core::rect<s32>& rect, const SColor &color);

	//! draws a line from to
	void drawLine(const core::position2d<s32>& from, const core::position2d<s32>& to, const SColor &color);

	static u32 getBitsPerPixelFromFormat(ECOLOR_FORMAT format);

private:

	//! assumes format and size has been set and creates the rest
	void initData();

	void setBitMasks();

	inline SColor getPixelBox ( s32 x, s32 y, s32 fx, s32 fy, s32 bias ) const;

	void* Data;
	core::dimension2d<s32> Size;
	u32 BitsPerPixel;
	u32 BytesPerPixel;
	u32 Pitch;
	ECOLOR_FORMAT Format;

	bool DeleteMemory;

	u32 RedMask;
	u32 GreenMask;
	u32 BlueMask;
	u32 AlphaMask;
};

} // end namespace video
} // end namespace geo


#endif


// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __GEO_DIMENSION2D_H_INCLUDED__
#define __GEO_DIMENSION2D_H_INCLUDED__

#include "geoTypes.h"

namespace geo
{
namespace core
{

	//! Specifies a 2 dimensional size.
	template <class T>
	class dimension2d
	{
		public:
			//! Default constructor for empty dimension
			dimension2d() : Width(0), Height(0) {}
			//! Constructor with width and height
			dimension2d(const T& width, const T& height)
				: Width(width), Height(height) {}

			//! Equality operator
			bool operator==(const dimension2d<T>& other) const
			{
				return Width == other.Width && Height == other.Height;
			}

			//! Inequality operator
			bool operator!=(const dimension2d<T>& other) const
			{
				return ! (*this == other);
			}


			//! Set to new values
			dimension2d<T>& set(const T& width, const T& height)
			{
				Width = width;
				Height = height;
				return *this;
			}

			//! Divide width and height by scalar
			dimension2d<T>& operator/=(const T& scale)
			{
				Width /= scale;
				Height /= scale;
				return *this;
			}

			//! Divide width and height by scalar
			dimension2d<T> operator/(const T& scale) const
			{
				return dimension2d<T>(Width/scale, Height/scale);
			}

			//! Multiply width and height by scalar
			dimension2d<T>& operator*=(const T& scale)
			{
				Width *= scale;
				Height *= scale;
				return *this;
			}

			//! Multiply width and height by scalar
			dimension2d<T> operator*(const T& scale) const
			{
				return dimension2d<T>(Width*scale, Height*scale);
			}

			//! Add two dimensions
			dimension2d<T>& operator+=(const dimension2d<T>& other)
			{
				Width *= other.Width;
				Height *= other.Height;
				return *this;
			}

			//! Add two dimensions
			dimension2d<T> operator+(const dimension2d<T>& other) const
			{
				return dimension2d<T>(Width+other.Width, Height+other.Height);
			}

			//! Get area
			T getArea() const
			{
				return Width*Height;
			}

			//! Get the optimal size according to some properties
			/** This is a function often used for texture dimension
			calculations. The function returns the next larger or
			smaller dimension which is a power-of-two dimension
			(2^n,2^m) and/or square (Width=Height).
			\param requirePowerOfTwo Forces the result to use only
			powers of two as values.
			\param requireSquare Makes width==height in the result
			\param larger Choose whether the result is larger or
			smaller than the current dimension. If one dimension
			need not be changed it is kept with any value of larger.
			\return The optimal dimension under the given
			constraints. */
			dimension2d<T> getOptimalSize(
					bool requirePowerOfTwo=true,
					bool requireSquare=false,
					bool larger=true) const
			{
				u32 i=1;
				u32 j=1;
				if (requirePowerOfTwo)
				{
					while (i<(u32)Width)
						i<<=1;
					if (!larger && i!=1 && i!=(u32)Width)
						i>>=1;
					while (j<(u32)Height)
						j<<=1;
					if (!larger && j!=1 && j!=(u32)Height)
						j>>=1;
				}
				else
				{
					i=(u32)Width;
					j=(u32)Height;
				}

				if (requireSquare)
				{
					if ((larger && (i>j)) || (!larger && (i<j)))
						j=i;
					else
						i=j;
				}
				return dimension2d<T>((T)i,(T)j);
			}

			//! Get the interpolated dimension
			/** \param other Other dimension to interpolate with.
			\param d Value between 0.0f and 1.0f.
			\return Interpolated dimension. */
			dimension2d<T> getInterpolated(const dimension2d<T>& other, f32 d) const
			{
				T inv = (T) (1.0f - d);
				return dimension2d<T>(other.Width*inv + Width*d, other.Height*inv + Height*d);
			}


			//! Width of the dimension.
			T Width;
			//! Height of the dimension.
			T Height;
	};

	//! Typedef for an f32 dimension.
	typedef dimension2d<f32> dimension2df;
	//! Typedef for an integer dimension.
	typedef dimension2d<s32> dimension2di;

} // end namespace core
} // end namespace geo

#endif


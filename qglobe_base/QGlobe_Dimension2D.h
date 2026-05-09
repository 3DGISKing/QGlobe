#ifndef __QGLOBE_DIMENSION_2D_H_INCLUDED__
#define __QGLOBE_DIMENSION_2D_H_INCLUDED__


#include "QGlobe_Define.h"

//! Specifies a 2 dimensional size.
template <class T>
class CQGlobe_Dimension2D
{
	public:
		//! Default constructor for empty dimension
		CQGlobe_Dimension2D() : m_tWidth(0), m_tHeight(0) {}
		//! Constructor with a_tWidth and a_tHeight
		CQGlobe_Dimension2D(const T& a_tWidth, const T& a_tHeight)
			: m_tWidth(a_tWidth), m_tHeight(a_tHeight) {}

		//! Equality operator
		bool operator==(const CQGlobe_Dimension2D<T>& a_cOther) const
		{
			return m_tWidth == a_cOther.m_tWidth && m_tHeight == a_cOther.m_tHeight;
		}

		//! Inequality operator
		bool operator!=(const CQGlobe_Dimension2D<T>& a_cOther) const
		{
			return ! (*this == a_cOther);
		}


		//! Set to new values
		CQGlobe_Dimension2D<T>& set(const T& a_tWidth, const T& a_tHeight)
		{
			m_tWidth = a_tWidth;
			m_tHeight = a_tHeight;
			return *this;
		}

		//! Divide a_tWidth and a_tHeight by scalar
		CQGlobe_Dimension2D<T>& operator/=(const T& a_tScale)
		{
			m_tWidth /= a_tScale;
			m_tHeight /= a_tScale;
			return *this;
		}

		//! Divide a_tWidth and a_tHeight by scalar
		CQGlobe_Dimension2D<T> operator/(const T& a_tScale) const
		{
			return CQGlobe_Dimension2D<T>(m_tWidth/a_tScale, m_tHeight/a_tScale);
		}

		//! Multiply a_tWidth and a_tHeight by scalar
		CQGlobe_Dimension2D<T>& operator*=(const T& a_tScale)
		{
			m_tWidth *= a_tScale;
			m_tHeight *= a_tScale;
			return *this;
		}

		//! Multiply a_tWidth and a_tHeight by scalar
		CQGlobe_Dimension2D<T> operator*(const T& a_tScale) const
		{
			return CQGlobe_Dimension2D<T>(m_tWidth*a_tScale, m_tHeight*a_tScale);
		}

		//! Add two dimensions
		CQGlobe_Dimension2D<T>& operator+=(const CQGlobe_Dimension2D<T>& a_cOther)
		{
			m_tWidth *= a_cOther.m_tWidth;
			m_tHeight *= a_cOther.m_tHeight;
			return *this;
		}

		//! Add two dimensions
		CQGlobe_Dimension2D<T> operator+(const CQGlobe_Dimension2D<T>& a_cOther) const
		{
			return CQGlobe_Dimension2D<T>(m_tWidth+a_cOther.m_tWidth, m_tHeight+a_cOther.m_tHeight);
		}

		//! Get area
		T getArea() const
		{
			return m_tWidth*m_tHeight;
		}

		//! Get the optimal size according to some properties
		/** This is a function often used for texture dimension
		calculations. The function returns the next larger or
		smaller dimension which is a power-of-two dimension
		(2^n,2^m) and/or square (m_tWidth=m_tHeight).
		\param requirePowerOfTwo Forces the result to use only
		powers of two as values.
		\param requireSquare Makes a_tWidth==a_tHeight in the result
		\param larger Choose whether the result is larger or
		smaller than the current dimension. If one dimension
		need not be changed it is kept with any value of larger.
		\return The optimal dimension under the given
		constraints. */
		CQGlobe_Dimension2D<T> getOptimalSize(
				bool requirePowerOfTwo=true,
				bool requireSquare=false,
				bool larger=true) const
		{
			QGLOBE_UINT32 i=1;
			QGLOBE_UINT32 j=1;
			if (requirePowerOfTwo)
			{
				while (i<(QGLOBE_UINT32)m_tWidth)
					i<<=1;
				if (!larger && i!=1 && i!=(QGLOBE_UINT32)m_tWidth)
					i>>=1;
				while (j<(QGLOBE_UINT32)m_tHeight)
					j<<=1;
				if (!larger && j!=1 && j!=(QGLOBE_UINT32)m_tHeight)
					j>>=1;
			}
			else
			{
				i=(QGLOBE_UINT32)m_tWidth;
				j=(QGLOBE_UINT32)m_tHeight;
			}

			if (requireSquare)
			{
				if ((larger && (i>j)) || (!larger && (i<j)))
					j=i;
				else
					i=j;
			}
			return CQGlobe_Dimension2D<T>((T)i,(T)j);
		}

		//! Get the interpolated dimension
		/** \param a_cOther Other dimension to interpolate with.
		\param d Value between 0.0f and 1.0f.
		\return Interpolated dimension. */
		CQGlobe_Dimension2D<T> getInterpolated(const CQGlobe_Dimension2D<T>& a_cOther, double d) const
		{
			T inv = (T) (1.0f - d);
			return CQGlobe_Dimension2D<T>(a_cOther.m_tWidth*inv + m_tWidth*d, a_cOther.m_tHeight*inv + m_tHeight*d);
		}


		//! m_tWidth of the dimension.
		T m_tWidth;
		//! m_tHeight of the dimension.
		T m_tHeight;
};

	//! Typedef for an double dimension.
	typedef CQGlobe_Dimension2D<double> CQGlobe_Dimension2DF;
	//! Typedef for an integer dimension.
	typedef CQGlobe_Dimension2D<QGLOBE_INT> CQGlobe_Dimension2DI;

#endif // __QGLOBE_DIMENSION_2D_H_INCLUDED__

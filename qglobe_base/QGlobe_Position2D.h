#ifndef __QGLOBE_POSITION_2D_H_INCLUDED__
#define __QGLOBE_POSITION_2D_H_INCLUDED__

#include "QGlobe_Dimension2D.h"


//! Simple class for holding 2d coordinates.
/** Not supposed for doing geometric calculations.
use vector2d instead for things like that.
*/
template <class T>
class CQGlobe_Position2D
{
	public:
		//! Default constructor for (0,0).
		CQGlobe_Position2D() : m_tX(0), m_tY(0) {}
		CQGlobe_Position2D(T x, T y) : m_tX(x), m_tY(y) {}
		CQGlobe_Position2D(const CQGlobe_Position2D<T>& a_cOther)
			: m_tX(a_cOther.m_tX), m_tY(a_cOther.m_tY) {}

		bool operator == (const CQGlobe_Position2D<T>& a_cOther) const
		{
			return m_tX == a_cOther.m_tX && m_tY == a_cOther.m_tY;
		}

		bool operator != (const CQGlobe_Position2D<T>& a_cOther) const
		{
			return m_tX != a_cOther.m_tX || m_tY != a_cOther.m_tY;
		}

		const CQGlobe_Position2D<T>& operator+=(const CQGlobe_Position2D<T>& a_cOther)
		{
			m_tX += a_cOther.m_tX;
			m_tY += a_cOther.m_tY;
			return *this;
		}

		const CQGlobe_Position2D<T>& operator-=(const CQGlobe_Position2D<T>& a_cOther)
		{
			m_tX -= a_cOther.m_tX;
			m_tY -= a_cOther.m_tY;
			return *this;
		}

		const CQGlobe_Position2D<T>& operator+=(const CQGlobe_Dimension2D<T>& a_cOther)
		{
			m_tX += a_cOther.m_tWidth;
			m_tY += a_cOther.m_tHeight;
			return *this;
		}

		const CQGlobe_Position2D<T>& operator-=(const CQGlobe_Dimension2D<T>& a_cOther)
		{
			m_tX -= a_cOther.m_tWidth;
			m_tY -= a_cOther.m_tHeight;
			return *this;
		}

		CQGlobe_Position2D<T> operator-(const CQGlobe_Position2D<T>& a_cOther) const
		{
			return CQGlobe_Position2D<T>(m_tX-a_cOther.m_tX, m_tY-a_cOther.m_tY);
		}

		CQGlobe_Position2D<T> operator+(const CQGlobe_Position2D<T>& a_cOther) const
		{
			return CQGlobe_Position2D<T>(m_tX+a_cOther.m_tX, m_tY+a_cOther.m_tY);
		}

		CQGlobe_Position2D<T> operator*(const CQGlobe_Position2D<T>& a_cOther) const
		{
			return CQGlobe_Position2D<T>(m_tX*a_cOther.m_tX, m_tY*a_cOther.m_tY);
		}

		CQGlobe_Position2D<T> operator*(const T& scalar) const
		{
			return CQGlobe_Position2D<T>(m_tX*scalar, m_tY*scalar);
		}

		CQGlobe_Position2D<T> operator+(const CQGlobe_Dimension2D<T>& a_cOther) const
		{
			return CQGlobe_Position2D<T>(m_tX+a_cOther.m_tWidth, m_tY+a_cOther.m_tHeight);
		}

		CQGlobe_Position2D<T> operator-(const CQGlobe_Dimension2D<T>& a_cOther) const
		{
			return CQGlobe_Position2D<T>(m_tX-a_cOther.m_tWidth, m_tY-a_cOther.m_tHeight);
		}

		const CQGlobe_Position2D<T>& operator=(const CQGlobe_Position2D<T>& a_cOther)
		{
			m_tX = a_cOther.m_tX;
			m_tY = a_cOther.m_tY;
			return *this;
		}

		//! m_tX coordinate of the position.
		T m_tX;
		//! m_tY coordinate of the position.
		T m_tY;
};

	//! Typedef for an double position.
	typedef CQGlobe_Position2D<double> CQGlobe_Position2DF;
	//! Typedef for an integer position.
	typedef CQGlobe_Position2D<QGLOBE_INT> CQGlobe_Position2DI;


#endif // __QGLOBE_POSITION_2D_H_INCLUDED__

#ifndef __GDEM_POSITION_2D_H_INCLUDED__
#define __GDEM_POSITION_2D_H_INCLUDED__

#include "GDM_Dimension2D.h"


//! Simple class for holding 2d coordinates.
/** Not supposed for doing geometric calculations.
use vector2d instead for things like that.
*/
template <class T>
class CGDM_Position2D
{
	public:
		//! Default constructor for (0,0).
		CGDM_Position2D() : m_tX(0), m_tY(0) {}
		CGDM_Position2D(T x, T y) : m_tX(x), m_tY(y) {}
		CGDM_Position2D(const CGDM_Position2D<T>& a_cOther)
			: m_tX(a_cOther.m_tX), m_tY(a_cOther.m_tY) {}

		bool operator == (const CGDM_Position2D<T>& a_cOther) const
		{
			return m_tX == a_cOther.m_tX && m_tY == a_cOther.m_tY;
		}

		bool operator != (const CGDM_Position2D<T>& a_cOther) const
		{
			return m_tX != a_cOther.m_tX || m_tY != a_cOther.m_tY;
		}

		const CGDM_Position2D<T>& operator+=(const CGDM_Position2D<T>& a_cOther)
		{
			m_tX += a_cOther.m_tX;
			m_tY += a_cOther.m_tY;
			return *this;
		}

		const CGDM_Position2D<T>& operator-=(const CGDM_Position2D<T>& a_cOther)
		{
			m_tX -= a_cOther.m_tX;
			m_tY -= a_cOther.m_tY;
			return *this;
		}

		const CGDM_Position2D<T>& operator+=(const CGDM_Dimension2D<T>& a_cOther)
		{
			m_tX += a_cOther.m_tWidth;
			m_tY += a_cOther.m_tHeight;
			return *this;
		}

		const CGDM_Position2D<T>& operator-=(const CGDM_Dimension2D<T>& a_cOther)
		{
			m_tX -= a_cOther.m_tWidth;
			m_tY -= a_cOther.m_tHeight;
			return *this;
		}

		CGDM_Position2D<T> operator-(const CGDM_Position2D<T>& a_cOther) const
		{
			return CGDM_Position2D<T>(m_tX-a_cOther.m_tX, m_tY-a_cOther.m_tY);
		}

		CGDM_Position2D<T> operator+(const CGDM_Position2D<T>& a_cOther) const
		{
			return CGDM_Position2D<T>(m_tX+a_cOther.m_tX, m_tY+a_cOther.m_tY);
		}

		CGDM_Position2D<T> operator*(const CGDM_Position2D<T>& a_cOther) const
		{
			return CGDM_Position2D<T>(m_tX*a_cOther.m_tX, m_tY*a_cOther.m_tY);
		}

		CGDM_Position2D<T> operator*(const T& scalar) const
		{
			return CGDM_Position2D<T>(m_tX*scalar, m_tY*scalar);
		}

		CGDM_Position2D<T> operator+(const CGDM_Dimension2D<T>& a_cOther) const
		{
			return CGDM_Position2D<T>(m_tX+a_cOther.m_tWidth, m_tY+a_cOther.m_tHeight);
		}

		CGDM_Position2D<T> operator-(const CGDM_Dimension2D<T>& a_cOther) const
		{
			return CGDM_Position2D<T>(m_tX-a_cOther.m_tWidth, m_tY-a_cOther.m_tHeight);
		}

		const CGDM_Position2D<T>& operator=(const CGDM_Position2D<T>& a_cOther)
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
	typedef CGDM_Position2D<double> CGDM_Position2DF;
	//! Typedef for an integer position.
	typedef CGDM_Position2D<GDEM_INT> CGDM_Position2DI;


#endif // __GDEM_POSITION_2D_H_INCLUDED__

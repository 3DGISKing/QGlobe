#ifndef __GDEM_RECT_H_INCLUDED__
#define __GDEM_RECT_H_INCLUDED__

#include "GDM_Dimension2D.h"
#include "GDM_Position2D.h"

//! Rectangle template.
/** Mostly used by 2D GUI elements and for 2D drawing methods.
It has 2 positions instead of position and dimension and a fast
method for collision detection with other rectangles and points.

Coordinates are (0,0) for top-left corner, and increasing to the right
and to the bottom.
*/
template <class T>
class CGDM_Rect
{
public:

	CGDM_Rect() : m_cMinPt(0,0), m_cMaxPt(0,0) {}

	CGDM_Rect(T x, T y, T x2, T y2)
		: m_cMinPt(x,y), m_cMaxPt(x2,y2) {}

	CGDM_Rect(const CGDM_Position2D<T>& upperLeft, const CGDM_Position2D<T>& lowerRight)
		: m_cMinPt(upperLeft), m_cMaxPt(lowerRight) {}

	CGDM_Rect(const CGDM_Position2D<T>& pos, const CGDM_Dimension2D<T>& size)
		: m_cMinPt(pos), m_cMaxPt(pos.m_tX + size.Width, pos.m_tY + size.Height) {}


	CGDM_Rect<T> operator+(const CGDM_Position2D<T>& pos) const
	{
		CGDM_Rect<T> ret(*this);
		return ret+=pos;
	}

	CGDM_Rect<T>& operator+=(const CGDM_Position2D<T>& pos)
	{
		m_cMinPt += pos;
		m_cMaxPt += pos;
		return *this;
	}

	CGDM_Rect<T> operator-(const CGDM_Position2D<T>& pos) const
	{
		CGDM_Rect<T> ret(*this);
		return ret-=pos;
	}

	CGDM_Rect<T>& operator-=(const CGDM_Position2D<T>& pos)
	{
		m_cMinPt -= pos;
		m_cMaxPt -= pos;
		return *this;
	}

	bool operator==(const CGDM_Rect<T>& other) const
	{
		return (m_cMinPt == other.m_cMinPt &&
			m_cMaxPt == other.m_cMaxPt);
	}


	bool operator!=(const CGDM_Rect<T>& other) const
	{
		return (m_cMinPt != other.m_cMinPt ||
			m_cMaxPt != other.m_cMaxPt);
	}

	// compares size of rectangles
	bool operator<(const CGDM_Rect<T>& other) const
	{
		return getArea() < other.getArea();
	}

	//! Returns size of rectangle
	T getArea() const
	{
		return getWidth() * getHeight();
	}

	//! Returns if a 2d point is within this rectangle.
	/** \param pos Position to test if it lies within this rectangle.
	\return True if the position is within the rectangle, false if not. */
	bool isPointInside(const CGDM_Position2D<T>& pos) const
	{
		return (m_cMinPt.m_tX <= pos.m_tX &&
			m_cMinPt.m_tY <= pos.m_tY &&
			m_cMaxPt.m_tX >= pos.m_tX &&
			m_cMaxPt.m_tY >= pos.m_tY);
	}


	bool isPointInside(const T &tX, const T &tY) const
	{
		return (m_cMinPt.m_tX <= tX &&
			m_cMinPt.m_tY <= tY &&
			m_cMaxPt.m_tX >= tX &&
			m_cMaxPt.m_tY >= tY);
	}

	//! Returns if the rectangle collides with another rectangle.
	bool isRectCollided(const CGDM_Rect<T>& other) const
	{
		return (m_cMaxPt.m_tY > other.m_cMinPt.m_tY &&
			m_cMinPt.m_tY < other.m_cMaxPt.m_tY &&
			m_cMaxPt.m_tX > other.m_cMinPt.m_tX &&
			m_cMinPt.m_tX < other.m_cMaxPt.m_tX);
	}

	//Returns true if the given point is inside or on the edge of the rectangle; otherwise returns false
	bool isContained(const CGDM_Rect<T>& other) const
	{
		T l1 = m_cMinPt.m_tX;
		T r1 = m_cMinPt.m_tX;
		T w=getWidth();
		if (w < 0)
			l1 += w;
		else
			r1 += w;
		if (l1 == r1) // null rect
			return false;

		qreal l2 = other.m_cMinPt.m_tX;
		qreal r2 = other.m_cMinPt.m_tX;
		if (other.getWidth() < 0)
			l2 += other.getWidth();
		else
			r2 +=  other.getWidth();
		if (l2 == r2) // null rect
			return false;

		if (l2 < l1 || r2 > r1)
			return false;

		qreal t1 = m_cMinPt.m_tY;
		qreal b1 = m_cMinPt.m_tY;
		T h=getHeight();
		if (h < 0)
			t1 += h;
		else
			b1 += h;
		if (t1 == b1) // null rect
			return false;

		qreal t2 = other.m_cMinPt.m_tY;
		qreal b2 = other.m_cMinPt.m_tY;
		if (other.getHeight() < 0)
			t2 += other.getHeight() ;
		else
			b2 += other.getHeight() ;
		if (t2 == b2) // null rect
			return false;

		if (t2 < t1 || b2 > b1)
			return false;

		return true;
	}


	//! Clips this rectangle with another one.
	void clipAgainst(const CGDM_Rect<T>& other)
	{
		if (other.m_cMaxPt.m_tX < m_cMaxPt.m_tX)
			m_cMaxPt.m_tX = other.m_cMaxPt.m_tX;
		if (other.m_cMaxPt.m_tY < m_cMaxPt.m_tY)
			m_cMaxPt.m_tY = other.m_cMaxPt.m_tY;

		if (other.m_cMinPt.m_tX > m_cMinPt.m_tX)
			m_cMinPt.m_tX = other.m_cMinPt.m_tX;
		if (other.m_cMinPt.m_tY > m_cMinPt.m_tY)
			m_cMinPt.m_tY = other.m_cMinPt.m_tY;

		// correct possible invalid CGDM_Rect resulting from clipping
		if (m_cMinPt.m_tY > m_cMaxPt.m_tY)
			m_cMinPt.m_tY = m_cMaxPt.m_tY;
		if (m_cMinPt.m_tX > m_cMaxPt.m_tX)
			m_cMinPt.m_tX = m_cMaxPt.m_tX;
	}

	//! Moves this rectangle to fit inside another one.
	/** \return True on success, false if not possible */
	bool constrainTo(const CGDM_Rect<T>& other)
	{
		if (other.getWidth() < getWidth() || other.getHeight() < getHeight())
			return false;

		T diff = other.m_cMaxPt.m_tX - m_cMaxPt.m_tX;
		if (diff < 0)
		{
			m_cMaxPt.m_tX += diff;
			m_cMinPt.m_tX  += diff;
		}

		diff = other.m_cMaxPt.m_tY - m_cMaxPt.m_tY;
		if (diff < 0)
		{
			m_cMaxPt.m_tY += diff;
			m_cMinPt.m_tY  += diff;
		}

		diff = m_cMinPt.m_tX - other.m_cMinPt.m_tX;
		if (diff < 0)
		{
			m_cMinPt.m_tX  -= diff;
			m_cMaxPt.m_tX -= diff;
		}

		diff = m_cMinPt.m_tY - other.m_cMinPt.m_tY;
		if (diff < 0)
		{
			m_cMinPt.m_tY  -= diff;
			m_cMaxPt.m_tY -= diff;
		}

		return true;
	}

	//! Returns width of rectangle.
	T getWidth() const
	{
		return m_cMaxPt.m_tX - m_cMinPt.m_tX;
	}

	//! Returns height of rectangle.
	T getHeight() const
	{
		return m_cMaxPt.m_tY - m_cMinPt.m_tY;
	}

	//! If the lower right corner of the CGDM_Rect is smaller then the upper left, the points are swapped.
	void repair()
	{
		if (m_cMaxPt.m_tX < m_cMinPt.m_tX)
		{
			T t = m_cMaxPt.m_tX;
			m_cMaxPt.m_tX = m_cMinPt.m_tX;
			m_cMinPt.m_tX = t;
		}

		if (m_cMaxPt.m_tY < m_cMinPt.m_tY)
		{
			T t = m_cMaxPt.m_tY;
			m_cMaxPt.m_tY = m_cMinPt.m_tY;
			m_cMinPt.m_tY = t;
		}
	}

	//! Returns if the CGDM_Rect is valid to draw.
	/** It would be invalid if the m_cMinPt is lower or more
	right than the m_cMaxPt. */
	bool isValid() const
	{
		return ((m_cMaxPt.m_tX >= m_cMinPt.m_tX) &&
				(m_cMaxPt.m_tY >= m_cMinPt.m_tY));
	}

	//! Returns the center of the rectangle
	CGDM_Position2D<T> getCenter() const
	{
		return CGDM_Position2D<T>(
				(m_cMinPt.m_tX + m_cMaxPt.m_tX) / 2,
				(m_cMinPt.m_tY + m_cMaxPt.m_tY) / 2);
	}

	//! Returns the dimensions of the rectangle
	CGDM_Dimension2D<T> getSize() const
	{
		return CGDM_Dimension2D<T>(getWidth(), getHeight());
	}


	//! Adds a point to the rectangle
	/** Cause the rectangle to grow bigger, if point is outside of
	the box
	\param p Point to add into the box. */
	void addInternalPoint(const CGDM_Position2D<T>& p)
	{
		addInternalPoint(p.m_tX, p.m_tY);
	}

	//! Adds a point to the bounding rectangle
	/** Cause the rectangle to grow bigger, if point is outside of
	\param x X Coordinate of the point to add to this box.
	\param y Y Coordinate of the point to add to this box. */
	void addInternalPoint(T x, T y)
	{
		if (x>m_cMaxPt.m_tX)
			m_cMaxPt.m_tX = x;
		if (y>m_cMaxPt.m_tY)
			m_cMaxPt.m_tY = y;

		if (x<m_cMinPt.m_tX)
			m_cMinPt.m_tX = x;
		if (y<m_cMinPt.m_tY)
			m_cMinPt.m_tY = y;
	}

	CGDM_Position2D<T> m_cMinPt;
	CGDM_Position2D<T> m_cMaxPt;
};

	//! Typedef for an double Rect.
	typedef CGDM_Rect<double> CGDM_Rect2D;
	//! Typedef for an integer Rect.
	typedef CGDM_Rect<GDEM_INT> CGDM_RectI;

#endif // __GDEM_RECT_H_INCLUDED__

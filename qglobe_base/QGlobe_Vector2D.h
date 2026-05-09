#ifndef __QGLOBE_VECTOR_2D_H_INCLUDED__
#define __QGLOBE_VECTOR_2D_H_INCLUDED__

#include "QGlobe_Define.h"
#include "QGlobe_Math.h"

//! 2d vector template class with lots of operators and methods.
template <class T>
class CQGlobe_Vector2D
{
public:
	//! Default constructor (null vector)
	CQGlobe_Vector2D(void) : m_tX(0), m_tY(0) {}
	//! Constructor with two different values
	CQGlobe_Vector2D(T a_tX, T a_tY) : m_tX(a_tX), m_tY(a_tY) {}
	//! Constructor with the same value for both members
	explicit CQGlobe_Vector2D(T a_tV) : m_tX(a_tV), m_tY(a_tV) {}
	//! Copy constructor
	CQGlobe_Vector2D(const CQGlobe_Vector2D<T>& a_cOther) : m_tX(a_cOther.m_tX), m_tY(a_cOther.m_tY) {}


	// operators

	CQGlobe_Vector2D<T> operator-() const { return CQGlobe_Vector2D<T>(-m_tX, -m_tY); }

	CQGlobe_Vector2D<T>& operator=(const CQGlobe_Vector2D<T>& a_cOther) { m_tX = a_cOther.m_tX; m_tY = a_cOther.m_tY; return *this; }

	CQGlobe_Vector2D<T> operator+(const CQGlobe_Vector2D<T>& a_cOther) const { return CQGlobe_Vector2D<T>(m_tX + a_cOther.m_tX, m_tY + a_cOther.m_tY); }
	CQGlobe_Vector2D<T>& operator+=(const CQGlobe_Vector2D<T>& a_cOther) { m_tX+=a_cOther.m_tX; m_tY+=a_cOther.m_tY; return *this; }
	CQGlobe_Vector2D<T> operator+(const T a_tV) const { return CQGlobe_Vector2D<T>(m_tX + a_tV, m_tY + a_tV); }
	CQGlobe_Vector2D<T>& operator+=(const T a_tV) { m_tX+=a_tV; m_tY+=a_tV; return *this; }

	CQGlobe_Vector2D<T> operator-(const CQGlobe_Vector2D<T>& a_cOther) const { return CQGlobe_Vector2D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY); }
	CQGlobe_Vector2D<T>& operator-=(const CQGlobe_Vector2D<T>& a_cOther) { m_tX-=a_cOther.m_tX; m_tY-=a_cOther.m_tY; return *this; }
	CQGlobe_Vector2D<T> operator-(const T a_tV) const { return CQGlobe_Vector2D<T>(m_tX - a_tV, m_tY - a_tV); }
	CQGlobe_Vector2D<T>& operator-=(const T a_tV) { m_tX-=a_tV; m_tY-=a_tV; return *this; }

	CQGlobe_Vector2D<T> operator*(const CQGlobe_Vector2D<T>& a_cOther) const { return CQGlobe_Vector2D<T>(m_tX * a_cOther.m_tX, m_tY * a_cOther.m_tY); }
	CQGlobe_Vector2D<T>& operator*=(const CQGlobe_Vector2D<T>& a_cOther) { m_tX*=a_cOther.m_tX; m_tY*=a_cOther.m_tY; return *this; }
	CQGlobe_Vector2D<T> operator*(const T a_tV) const { return CQGlobe_Vector2D<T>(m_tX * a_tV, m_tY * a_tV); }
	CQGlobe_Vector2D<T>& operator*=(const T a_tV) { m_tX*=a_tV; m_tY*=a_tV; return *this; }

	CQGlobe_Vector2D<T> operator/(const CQGlobe_Vector2D<T>& a_cOther) const { return CQGlobe_Vector2D<T>(m_tX / a_cOther.m_tX, m_tY / a_cOther.m_tY); }
	CQGlobe_Vector2D<T>& operator/=(const CQGlobe_Vector2D<T>& a_cOther) { m_tX/=a_cOther.m_tX; m_tY/=a_cOther.m_tY; return *this; }
	CQGlobe_Vector2D<T> operator/(const T a_tV) const { return CQGlobe_Vector2D<T>(m_tX / a_tV, m_tY / a_tV); }
	CQGlobe_Vector2D<T>& operator/=(const T a_tV) { m_tX/=a_tV; m_tY/=a_tV; return *this; }

	bool operator<=(const CQGlobe_Vector2D<T>&a_cOther) const { return m_tX<=a_cOther.m_tX && m_tY<=a_cOther.m_tY; }
	bool operator>=(const CQGlobe_Vector2D<T>&a_cOther) const { return m_tX>=a_cOther.m_tX && m_tY>=a_cOther.m_tY; }

	bool operator<(const CQGlobe_Vector2D<T>&a_cOther) const { return m_tX<a_cOther.m_tX && m_tY<a_cOther.m_tY; }
	bool operator>(const CQGlobe_Vector2D<T>&a_cOther) const { return m_tX>a_cOther.m_tX && m_tY>a_cOther.m_tY; }

	bool operator==(const CQGlobe_Vector2D<T>& a_cOther) const { return equals(a_cOther); }
	bool operator!=(const CQGlobe_Vector2D<T>& a_cOther) const { return !equals(a_cOther); }

	// functions

	//! Checks if this vector equals the a_cOther one.
	/** Takes floating point rounding errors into account.
	\param a_cOther Vector to compare with.
	\return True if the two vector are (almost) equal, else false. */
	bool equals(const CQGlobe_Vector2D<T>& a_cOther) const
	{
		return qglobe_equals(m_tX, a_cOther.m_tX) && qglobe_equals(m_tY, a_cOther.m_tY);
	}

	CQGlobe_Vector2D<T>& set(T a_tX, T a_tY) {m_tX=a_tX; m_tY=a_tY; return *this; }
	CQGlobe_Vector2D<T>& set(const CQGlobe_Vector2D<T>& p) { m_tX=p.m_tX; m_tY=p.m_tY; return *this; }

	//! Gets the length of the vector.
	/** \return The length of the vector. */
	T getLength() const { return (T)sqrt((QGLOBE_DOUBLE)(m_tX*m_tX + m_tY*m_tY)); }

	//! Get the squared length of this vector
	/** This is useful because it is much faster than getLength().
	\return The squared length of the vector. */
	T getLengthSQ() const { return m_tX*m_tX + m_tY*m_tY; }

	//! Get the dot product of this vector with another.
	/** \param a_cOther Other vector to take dot product with.
	\return The dot product of the two vectors. */
	T dotProduct(const CQGlobe_Vector2D<T>& a_cOther) const
	{
		return m_tX*a_cOther.m_tX + m_tY*a_cOther.m_tY;
	}

	//! Gets distance from another point.
	/** Here, the vector is interpreted as a point in 2-dimensional space.
	\param a_cOther Other vector to measure from.
	\return Distance from a_cOther point. */
	T getDistanceFrom(const CQGlobe_Vector2D<T>& a_cOther) const
	{
		return CQGlobe_Vector2D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY).getLength();
	}

	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as a point in 2-dimensional space.
	\param a_cOther Other vector to measure from.
	\return Squared distance from a_cOther point. */
	T getDistanceFromSQ(const CQGlobe_Vector2D<T>& a_cOther) const
	{
		return CQGlobe_Vector2D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY).getLengthSQ();
	}

	//! rotates the point anticlockwise around a center by an amount of degrees.
	/** \param degrees Amount of degrees to rotate by, anticlockwise.
	\param center Rotation center.
	\return This vector after transformation. */
	CQGlobe_Vector2D<T>& rotateBy(QGLOBE_DOUBLE degrees, const CQGlobe_Vector2D<T>& center=CQGlobe_Vector2D<T>())
	{
		degrees *= qglobe_DEGTORAD64;
		const QGLOBE_DOUBLE cs = cos(degrees);
		const QGLOBE_DOUBLE sn = sin(degrees);

		m_tX -= center.m_tX;
		m_tY -= center.m_tY;

		set((T)(m_tX*cs - m_tY*sn), (T)(m_tX*sn + m_tY*cs));

		m_tX += center.m_tX;
		m_tY += center.m_tY;
		return *this;
	}

	//! Normalize the vector.
	/** The null vector is left untouched.
	\return Reference to this vector, after normalization. */
	CQGlobe_Vector2D<T>& normalize()
	{
	/*
		double length = (double)(m_tX*m_tX + m_tY*m_tY);
		if (qglobe_equals(length, 0.f))
			return *this;
		length = qglobe_reciprocal_squareroot ( length );
		m_tX = (T)(m_tX * length);
		m_tY = (T)(m_tY * length);
	*/
		return *this;
	}

	//! Calculates the angle of this vector in degrees in the trigonometric sense.
	/** 0 is to the left (9 o'clock), values increase clockwise.
	This method has been suggested by Pr3t3nd3r.
	\return Returns a value between 0 and 360. */
	QGLOBE_DOUBLE getAngleTrig() const
	{
	//	if (m_tX == 0)
			return m_tY < 0 ? 270 : 90;
	/*
		else
		if (m_tY == 0)
			return m_tX < 0 ? 180 : 0;

		if ( m_tY > 0)
			if (m_tX > 0)
				return atan(m_tY/m_tX) * qglobe_RADTODEG64;
			else
				return 180.0-atan(m_tY/-m_tX) * qglobe_RADTODEG64;
		else
			if (m_tX > 0)
				return 360.0-atan(-m_tY/m_tX) * qglobe_RADTODEG64;
			else
				return 180.0+atan(-m_tY/-m_tX) * qglobe_RADTODEG64;
		*/
	}

	//! Calculates the angle of this vector in degrees in the counter trigonometric sense.
	/** 0 is to the right (3 o'clock), values increase counter-clockwise.
	\return Returns a value between 0 and 360. */
	inline QGLOBE_DOUBLE getAngle() const
	{
	//	if (m_tY == 0) // corrected thanks to a suggestion by Jox
	return m_tX < 0 ? 180 : 0;
/*
		else if (m_tX == 0)
			return m_tY < 0 ? 90 : 270;

		QGLOBE_DOUBLE tmp = m_tY / getLength();
		tmp = atan(sqrt(1 - tmp*tmp) / tmp) * qglobe_RADTODEG64;

		if (m_tX>0 && m_tY>0)
			return tmp + 270;
		else
		if (m_tX>0 && m_tY<0)
			return tmp + 90;
		else
		if (m_tX<0 && m_tY<0)
			return 90 - tmp;
		else
		if (m_tX<0 && m_tY>0)
			return 270 - tmp;

		return tmp;
*/
	}

	//! Calculates the angle between this vector and another one in degree.
	/** \param b Other vector to test with.
	\return Returns a value between 0 and 90. */
	inline QGLOBE_DOUBLE getAngleWith(const CQGlobe_Vector2D<T>& b) const
	{
		return 90.0;
	/*
		QGLOBE_DOUBLE tmp = m_tX*b.m_tX + m_tY*b.m_tY;

		if (tmp == 0.0)
			return 90.0;

		tmp = tmp / sqrt((QGLOBE_DOUBLE)((m_tX*m_tX + m_tY*m_tY) * (b.m_tX*b.m_tX + b.m_tY*b.m_tY)));
		if (tmp < 0.0)
			tmp = -tmp;

		return atan(sqrt(1 - tmp*tmp) / tmp) * qglobe_RADTODEG64;
	*/
	}

	//! Returns if this vector interpreted as a point is on a line between two a_cOther points.
	/** It is assumed that the point is on the line.
	\param begin Beginning vector to compare between.
	\param end Ending vector to compare between.
	\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const CQGlobe_Vector2D<T>& begin, const CQGlobe_Vector2D<T>& end) const
	{
		if (begin.m_tX != end.m_tX)
		{
			return ((begin.m_tX <= m_tX && m_tX <= end.m_tX) ||
				(begin.m_tX >= m_tX && m_tX >= end.m_tX));
		}
		else
		{
			return ((begin.m_tY <= m_tY && m_tY <= end.m_tY) ||
				(begin.m_tY >= m_tY && m_tY >= end.m_tY));
		}
	}

	//! Creates an interpolated vector between this vector and another vector.
	/** \param a_cOther The a_cOther vector to interpolate with.
	\param d Interpolation value between 0.0f (all the a_cOther vector) and 1.0f (all this vector).
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	\return An interpolated vector.  This vector is not modified. */
	CQGlobe_Vector2D<T> getInterpolated(const CQGlobe_Vector2D<T>& a_cOther, QGLOBE_DOUBLE d) const
	{
		QGLOBE_DOUBLE inv = 1.0f - d;
		return CQGlobe_Vector2D<T>((T)(a_cOther.m_tX*inv + m_tX*d), (T)(a_cOther.m_tY*inv + m_tY*d));
	}

	//! Creates a quadratically interpolated vector between this and two a_cOther vectors.
	/** \param v2 Second vector to interpolate with.
	\param v3 Third vector to interpolate with (maximum at 1.0f)
	\param d Interpolation value between 0.0f (all this vector) and 1.0f (all the 3rd vector).
	Note that this is the opposite direction of interpolation to getInterpolated() and interpolate()
	\return An interpolated vector. This vector is not modified. */
	CQGlobe_Vector2D<T> getInterpolated_quadratic(const CQGlobe_Vector2D<T>& v2, const CQGlobe_Vector2D<T>& v3, QGLOBE_DOUBLE d) const
	{
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		const QGLOBE_DOUBLE inv = 1.0f - d;
		const QGLOBE_DOUBLE mul0 = inv * inv;
		const QGLOBE_DOUBLE mul1 = 2.0f * d * inv;
		const QGLOBE_DOUBLE mul2 = d * d;

		return CQGlobe_Vector2D<T> ( (T)(m_tX * mul0 + v2.m_tX * mul1 + v3.m_tX * mul2),
					(T)(m_tY * mul0 + v2.m_tY * mul1 + v3.m_tY * mul2));
	}

	//! Sets this vector to the linearly interpolated vector between a and b.
	/** \param a first vector to interpolate with, maximum at 1.0f
	\param b second vector to interpolate with, maximum at 0.0f
	\param d Interpolation value between 0.0f (all vector b) and 1.0f (all vector a)
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	*/
	CQGlobe_Vector2D<T>& interpolate(const CQGlobe_Vector2D<T>& a, const CQGlobe_Vector2D<T>& b, QGLOBE_DOUBLE d)
	{
		m_tX = (T)((QGLOBE_DOUBLE)b.m_tX + ( ( a.m_tX - b.m_tX ) * d ));
		m_tY = (T)((QGLOBE_DOUBLE)b.m_tY + ( ( a.m_tY - b.m_tY ) * d ));
		return *this;
	}

	//! m_tX coordinate of vector.
	T m_tX;
	//! m_tY coordinate of vector.
	T m_tY;
};

	//! Typedef for double 2d vector.
	typedef CQGlobe_Vector2D<double> CQGlobe_Vector2DF;
	//! Typedef for integer 2d vector.
	typedef CQGlobe_Vector2D<QGLOBE_INT> CQGlobe_Vector2DI;

	template<class S, class T>
	CQGlobe_Vector2D<T> operator*(const S scalar, const CQGlobe_Vector2D<T>& vector) { return vector*scalar; }


#endif // __QGLOBE_VECTOR_2D_H_INCLUDED__

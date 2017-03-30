#ifndef __GDEM_VECTOR_2D_H_INCLUDED__
#define __GDEM_VECTOR_2D_H_INCLUDED__

#include "GDM_Define.h"
#include "GDM_Math.h"

//! 2d vector template class with lots of operators and methods.
template <class T>
class CGDM_Vector2D
{
public:
	//! Default constructor (null vector)
	CGDM_Vector2D(void) : m_tX(0), m_tY(0) {}
	//! Constructor with two different values
	CGDM_Vector2D(T a_tX, T a_tY) : m_tX(a_tX), m_tY(a_tY) {}
	//! Constructor with the same value for both members
	explicit CGDM_Vector2D(T a_tV) : m_tX(a_tV), m_tY(a_tV) {}
	//! Copy constructor
	CGDM_Vector2D(const CGDM_Vector2D<T>& a_cOther) : m_tX(a_cOther.m_tX), m_tY(a_cOther.m_tY) {}


	// operators

	CGDM_Vector2D<T> operator-() const { return CGDM_Vector2D<T>(-m_tX, -m_tY); }

	CGDM_Vector2D<T>& operator=(const CGDM_Vector2D<T>& a_cOther) { m_tX = a_cOther.m_tX; m_tY = a_cOther.m_tY; return *this; }

	CGDM_Vector2D<T> operator+(const CGDM_Vector2D<T>& a_cOther) const { return CGDM_Vector2D<T>(m_tX + a_cOther.m_tX, m_tY + a_cOther.m_tY); }
	CGDM_Vector2D<T>& operator+=(const CGDM_Vector2D<T>& a_cOther) { m_tX+=a_cOther.m_tX; m_tY+=a_cOther.m_tY; return *this; }
	CGDM_Vector2D<T> operator+(const T a_tV) const { return CGDM_Vector2D<T>(m_tX + a_tV, m_tY + a_tV); }
	CGDM_Vector2D<T>& operator+=(const T a_tV) { m_tX+=a_tV; m_tY+=a_tV; return *this; }

	CGDM_Vector2D<T> operator-(const CGDM_Vector2D<T>& a_cOther) const { return CGDM_Vector2D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY); }
	CGDM_Vector2D<T>& operator-=(const CGDM_Vector2D<T>& a_cOther) { m_tX-=a_cOther.m_tX; m_tY-=a_cOther.m_tY; return *this; }
	CGDM_Vector2D<T> operator-(const T a_tV) const { return CGDM_Vector2D<T>(m_tX - a_tV, m_tY - a_tV); }
	CGDM_Vector2D<T>& operator-=(const T a_tV) { m_tX-=a_tV; m_tY-=a_tV; return *this; }

	CGDM_Vector2D<T> operator*(const CGDM_Vector2D<T>& a_cOther) const { return CGDM_Vector2D<T>(m_tX * a_cOther.m_tX, m_tY * a_cOther.m_tY); }
	CGDM_Vector2D<T>& operator*=(const CGDM_Vector2D<T>& a_cOther) { m_tX*=a_cOther.m_tX; m_tY*=a_cOther.m_tY; return *this; }
	CGDM_Vector2D<T> operator*(const T a_tV) const { return CGDM_Vector2D<T>(m_tX * a_tV, m_tY * a_tV); }
	CGDM_Vector2D<T>& operator*=(const T a_tV) { m_tX*=a_tV; m_tY*=a_tV; return *this; }

	CGDM_Vector2D<T> operator/(const CGDM_Vector2D<T>& a_cOther) const { return CGDM_Vector2D<T>(m_tX / a_cOther.m_tX, m_tY / a_cOther.m_tY); }
	CGDM_Vector2D<T>& operator/=(const CGDM_Vector2D<T>& a_cOther) { m_tX/=a_cOther.m_tX; m_tY/=a_cOther.m_tY; return *this; }
	CGDM_Vector2D<T> operator/(const T a_tV) const { return CGDM_Vector2D<T>(m_tX / a_tV, m_tY / a_tV); }
	CGDM_Vector2D<T>& operator/=(const T a_tV) { m_tX/=a_tV; m_tY/=a_tV; return *this; }

	bool operator<=(const CGDM_Vector2D<T>&a_cOther) const { return m_tX<=a_cOther.m_tX && m_tY<=a_cOther.m_tY; }
	bool operator>=(const CGDM_Vector2D<T>&a_cOther) const { return m_tX>=a_cOther.m_tX && m_tY>=a_cOther.m_tY; }

	bool operator<(const CGDM_Vector2D<T>&a_cOther) const { return m_tX<a_cOther.m_tX && m_tY<a_cOther.m_tY; }
	bool operator>(const CGDM_Vector2D<T>&a_cOther) const { return m_tX>a_cOther.m_tX && m_tY>a_cOther.m_tY; }

	bool operator==(const CGDM_Vector2D<T>& a_cOther) const { return equals(a_cOther); }
	bool operator!=(const CGDM_Vector2D<T>& a_cOther) const { return !equals(a_cOther); }

	// functions

	//! Checks if this vector equals the a_cOther one.
	/** Takes floating point rounding errors into account.
	\param a_cOther Vector to compare with.
	\return True if the two vector are (almost) equal, else false. */
	bool equals(const CGDM_Vector2D<T>& a_cOther) const
	{
		return gdm_equals(m_tX, a_cOther.m_tX) && gdm_equals(m_tY, a_cOther.m_tY);
	}

	CGDM_Vector2D<T>& set(T a_tX, T a_tY) {m_tX=a_tX; m_tY=a_tY; return *this; }
	CGDM_Vector2D<T>& set(const CGDM_Vector2D<T>& p) { m_tX=p.m_tX; m_tY=p.m_tY; return *this; }

	//! Gets the length of the vector.
	/** \return The length of the vector. */
	T getLength() const { return (T)sqrt((GDEM_DOUBLE)(m_tX*m_tX + m_tY*m_tY)); }

	//! Get the squared length of this vector
	/** This is useful because it is much faster than getLength().
	\return The squared length of the vector. */
	T getLengthSQ() const { return m_tX*m_tX + m_tY*m_tY; }

	//! Get the dot product of this vector with another.
	/** \param a_cOther Other vector to take dot product with.
	\return The dot product of the two vectors. */
	T dotProduct(const CGDM_Vector2D<T>& a_cOther) const
	{
		return m_tX*a_cOther.m_tX + m_tY*a_cOther.m_tY;
	}

	//! Gets distance from another point.
	/** Here, the vector is interpreted as a point in 2-dimensional space.
	\param a_cOther Other vector to measure from.
	\return Distance from a_cOther point. */
	T getDistanceFrom(const CGDM_Vector2D<T>& a_cOther) const
	{
		return CGDM_Vector2D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY).getLength();
	}

	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as a point in 2-dimensional space.
	\param a_cOther Other vector to measure from.
	\return Squared distance from a_cOther point. */
	T getDistanceFromSQ(const CGDM_Vector2D<T>& a_cOther) const
	{
		return CGDM_Vector2D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY).getLengthSQ();
	}

	//! rotates the point anticlockwise around a center by an amount of degrees.
	/** \param degrees Amount of degrees to rotate by, anticlockwise.
	\param center Rotation center.
	\return This vector after transformation. */
	CGDM_Vector2D<T>& rotateBy(GDEM_DOUBLE degrees, const CGDM_Vector2D<T>& center=CGDM_Vector2D<T>())
	{
		degrees *= gdm_DEGTORAD64;
		const GDEM_DOUBLE cs = cos(degrees);
		const GDEM_DOUBLE sn = sin(degrees);

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
	CGDM_Vector2D<T>& normalize()
	{
	/*
		double length = (double)(m_tX*m_tX + m_tY*m_tY);
		if (gdm_equals(length, 0.f))
			return *this;
		length = gdm_reciprocal_squareroot ( length );
		m_tX = (T)(m_tX * length);
		m_tY = (T)(m_tY * length);
	*/
		return *this;
	}

	//! Calculates the angle of this vector in degrees in the trigonometric sense.
	/** 0 is to the left (9 o'clock), values increase clockwise.
	This method has been suggested by Pr3t3nd3r.
	\return Returns a value between 0 and 360. */
	GDEM_DOUBLE getAngleTrig() const
	{
	//	if (m_tX == 0)
			return m_tY < 0 ? 270 : 90;
	/*
		else
		if (m_tY == 0)
			return m_tX < 0 ? 180 : 0;

		if ( m_tY > 0)
			if (m_tX > 0)
				return atan(m_tY/m_tX) * gdm_RADTODEG64;
			else
				return 180.0-atan(m_tY/-m_tX) * gdm_RADTODEG64;
		else
			if (m_tX > 0)
				return 360.0-atan(-m_tY/m_tX) * gdm_RADTODEG64;
			else
				return 180.0+atan(-m_tY/-m_tX) * gdm_RADTODEG64;
		*/
	}

	//! Calculates the angle of this vector in degrees in the counter trigonometric sense.
	/** 0 is to the right (3 o'clock), values increase counter-clockwise.
	\return Returns a value between 0 and 360. */
	inline GDEM_DOUBLE getAngle() const
	{
	//	if (m_tY == 0) // corrected thanks to a suggestion by Jox
	return m_tX < 0 ? 180 : 0;
/*
		else if (m_tX == 0)
			return m_tY < 0 ? 90 : 270;

		GDEM_DOUBLE tmp = m_tY / getLength();
		tmp = atan(sqrt(1 - tmp*tmp) / tmp) * gdm_RADTODEG64;

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
	inline GDEM_DOUBLE getAngleWith(const CGDM_Vector2D<T>& b) const
	{
		return 90.0;
	/*
		GDEM_DOUBLE tmp = m_tX*b.m_tX + m_tY*b.m_tY;

		if (tmp == 0.0)
			return 90.0;

		tmp = tmp / sqrt((GDEM_DOUBLE)((m_tX*m_tX + m_tY*m_tY) * (b.m_tX*b.m_tX + b.m_tY*b.m_tY)));
		if (tmp < 0.0)
			tmp = -tmp;

		return atan(sqrt(1 - tmp*tmp) / tmp) * gdm_RADTODEG64;
	*/
	}

	//! Returns if this vector interpreted as a point is on a line between two a_cOther points.
	/** It is assumed that the point is on the line.
	\param begin Beginning vector to compare between.
	\param end Ending vector to compare between.
	\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const CGDM_Vector2D<T>& begin, const CGDM_Vector2D<T>& end) const
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
	CGDM_Vector2D<T> getInterpolated(const CGDM_Vector2D<T>& a_cOther, GDEM_DOUBLE d) const
	{
		GDEM_DOUBLE inv = 1.0f - d;
		return CGDM_Vector2D<T>((T)(a_cOther.m_tX*inv + m_tX*d), (T)(a_cOther.m_tY*inv + m_tY*d));
	}

	//! Creates a quadratically interpolated vector between this and two a_cOther vectors.
	/** \param v2 Second vector to interpolate with.
	\param v3 Third vector to interpolate with (maximum at 1.0f)
	\param d Interpolation value between 0.0f (all this vector) and 1.0f (all the 3rd vector).
	Note that this is the opposite direction of interpolation to getInterpolated() and interpolate()
	\return An interpolated vector. This vector is not modified. */
	CGDM_Vector2D<T> getInterpolated_quadratic(const CGDM_Vector2D<T>& v2, const CGDM_Vector2D<T>& v3, GDEM_DOUBLE d) const
	{
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		const GDEM_DOUBLE inv = 1.0f - d;
		const GDEM_DOUBLE mul0 = inv * inv;
		const GDEM_DOUBLE mul1 = 2.0f * d * inv;
		const GDEM_DOUBLE mul2 = d * d;

		return CGDM_Vector2D<T> ( (T)(m_tX * mul0 + v2.m_tX * mul1 + v3.m_tX * mul2),
					(T)(m_tY * mul0 + v2.m_tY * mul1 + v3.m_tY * mul2));
	}

	//! Sets this vector to the linearly interpolated vector between a and b.
	/** \param a first vector to interpolate with, maximum at 1.0f
	\param b second vector to interpolate with, maximum at 0.0f
	\param d Interpolation value between 0.0f (all vector b) and 1.0f (all vector a)
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	*/
	CGDM_Vector2D<T>& interpolate(const CGDM_Vector2D<T>& a, const CGDM_Vector2D<T>& b, GDEM_DOUBLE d)
	{
		m_tX = (T)((GDEM_DOUBLE)b.m_tX + ( ( a.m_tX - b.m_tX ) * d ));
		m_tY = (T)((GDEM_DOUBLE)b.m_tY + ( ( a.m_tY - b.m_tY ) * d ));
		return *this;
	}

	//! m_tX coordinate of vector.
	T m_tX;
	//! m_tY coordinate of vector.
	T m_tY;
};

	//! Typedef for double 2d vector.
	typedef CGDM_Vector2D<double> CGDM_Vector2DF;
	//! Typedef for integer 2d vector.
	typedef CGDM_Vector2D<GDEM_INT> CGDM_Vector2DI;

	template<class S, class T>
	CGDM_Vector2D<T> operator*(const S scalar, const CGDM_Vector2D<T>& vector) { return vector*scalar; }


#endif // __GDEM_VECTOR_2D_H_INCLUDED__

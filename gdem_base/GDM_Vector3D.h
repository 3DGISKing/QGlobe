#ifndef __GDEM_VECTOR3D_H_INCLUDED__
#define __GDEM_VECTOR3D_H_INCLUDED__

#include "GDM_Define.h"
#include "GDM_Math.h"
#include "GDM_Array.h"

//! 3d vector template class with lots of operators and methods.
/** The CGDM_Vector3D class is used in Geo for three main purposes: 
	1) As a direction vector (most of the methods assume this).
	2) As a position in 3d space (which is synonymous with a direction vector from the origin to this position).
	3) To hold three Euler rotations, where m_tX is pitch, m_tY is yaw and m_tZ is roll.
*/
template <class T>
class CGDM_Vector3D
{
public:
	//! Default constructor (null vector).
	CGDM_Vector3D() : m_tX(0), m_tY(0), m_tZ(0) {}
	//! Constructor with three different values
	CGDM_Vector3D(T a_tX, T a_tY, T a_tZ) : m_tX(a_tX), m_tY(a_tY), m_tZ(a_tZ) {}
	//! Constructor with the same value for all elements
	explicit CGDM_Vector3D(T a_tV) : m_tX(a_tV), m_tY(a_tV), m_tZ(a_tV) {}
	//! Copy constructor
	CGDM_Vector3D(const CGDM_Vector3D<T>& a_cOther) : m_tX(a_cOther.m_tX), m_tY(a_cOther.m_tY), m_tZ(a_cOther.m_tZ) {}

	// operators

	CGDM_Vector3D<T> operator-() const { return CGDM_Vector3D<T>(-m_tX, -m_tY, -m_tZ); }

	CGDM_Vector3D<T>& operator=(const CGDM_Vector3D<T>& a_cOther) { m_tX = a_cOther.m_tX; m_tY = a_cOther.m_tY; m_tZ = a_cOther.m_tZ; return *this; }

	CGDM_Vector3D<T> operator+(const CGDM_Vector3D<T>& a_cOther) const { return CGDM_Vector3D<T>(m_tX + a_cOther.m_tX, m_tY + a_cOther.m_tY, m_tZ + a_cOther.m_tZ); }
	CGDM_Vector3D<T>& operator+=(const CGDM_Vector3D<T>& a_cOther) { m_tX+=a_cOther.m_tX; m_tY+=a_cOther.m_tY; m_tZ+=a_cOther.m_tZ; return *this; }
	CGDM_Vector3D<T> operator+(const T a_tV) const { return CGDM_Vector3D<T>(m_tX + a_tV, m_tY + a_tV, m_tZ + a_tV); }
	CGDM_Vector3D<T>& operator+=(const T a_tV) { m_tX+=a_tV; m_tY+=a_tV; m_tZ+=a_tV; return *this; }

	CGDM_Vector3D<T> operator-(const CGDM_Vector3D<T>& a_cOther) const { return CGDM_Vector3D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY, m_tZ - a_cOther.m_tZ); }
	CGDM_Vector3D<T>& operator-=(const CGDM_Vector3D<T>& a_cOther) { m_tX-=a_cOther.m_tX; m_tY-=a_cOther.m_tY; m_tZ-=a_cOther.m_tZ; return *this; }
	CGDM_Vector3D<T> operator-(const T a_tV) const { return CGDM_Vector3D<T>(m_tX - a_tV, m_tY - a_tV, m_tZ - a_tV); }
	CGDM_Vector3D<T>& operator-=(const T a_tV) { m_tX-=a_tV; m_tY-=a_tV; m_tZ-=a_tV; return *this; }

	CGDM_Vector3D<T> operator*(const CGDM_Vector3D<T>& a_cOther) const { return CGDM_Vector3D<T>(m_tX * a_cOther.m_tX, m_tY * a_cOther.m_tY, m_tZ * a_cOther.m_tZ); }
	CGDM_Vector3D<T>& operator*=(const CGDM_Vector3D<T>& a_cOther) { m_tX*=a_cOther.m_tX; m_tY*=a_cOther.m_tY; m_tZ*=a_cOther.m_tZ; return *this; }
	CGDM_Vector3D<T> operator*(const T a_tV) const { return CGDM_Vector3D<T>(m_tX * a_tV, m_tY * a_tV, m_tZ * a_tV); }
	CGDM_Vector3D<T>& operator*=(const T a_tV) { m_tX*=a_tV; m_tY*=a_tV; m_tZ*=a_tV; return *this; }

	CGDM_Vector3D<T> operator/(const CGDM_Vector3D<T>& a_cOther) const { return CGDM_Vector3D<T>(m_tX / a_cOther.m_tX, m_tY / a_cOther.m_tY, m_tZ / a_cOther.m_tZ); }
	CGDM_Vector3D<T>& operator/=(const CGDM_Vector3D<T>& a_cOther) { m_tX/=a_cOther.m_tX; m_tY/=a_cOther.m_tY; m_tZ/=a_cOther.m_tZ; return *this; }
	CGDM_Vector3D<T> operator/(const T a_tV) const { T i=(T)1.0/a_tV; return CGDM_Vector3D<T>(m_tX * i, m_tY * i, m_tZ * i); }
	CGDM_Vector3D<T>& operator/=(const T a_tV) { T i=(T)1.0/a_tV; m_tX*=i; m_tY*=i; m_tZ*=i; return *this; }

	bool operator<=(const CGDM_Vector3D<T>&a_cOther) const { return m_tX<=a_cOther.m_tX && m_tY<=a_cOther.m_tY && m_tZ<=a_cOther.m_tZ;}
	bool operator>=(const CGDM_Vector3D<T>&a_cOther) const { return m_tX>=a_cOther.m_tX && m_tY>=a_cOther.m_tY && m_tZ>=a_cOther.m_tZ;}
	bool operator<(const CGDM_Vector3D<T>&a_cOther) const { return m_tX<a_cOther.m_tX && m_tY<a_cOther.m_tY && m_tZ<a_cOther.m_tZ;}
	bool operator>(const CGDM_Vector3D<T>&a_cOther) const { return m_tX>a_cOther.m_tX && m_tY>a_cOther.m_tY && m_tZ>a_cOther.m_tZ;}

	//! use weak double compare
	bool operator==(const CGDM_Vector3D<T>& a_cOther) const
	{
		return this->equals(a_cOther);
	}

	bool operator!=(const CGDM_Vector3D<T>& a_cOther) const
	{
		return !this->equals(a_cOther);
	}

	// functions

	//! returns if this vector equals the a_cOther one, taking floating point rounding errors into account
	bool equals(const CGDM_Vector3D<T>& a_cOther, const T tolerance = (T)gdm_ROUNDING_ERROR_32 ) const
	{
		return gdm_equals(m_tX, a_cOther.m_tX, tolerance) &&
			gdm_equals(m_tY, a_cOther.m_tY, tolerance) &&
			gdm_equals(m_tZ, a_cOther.m_tZ, tolerance);
	}

	CGDM_Vector3D<T>& set(const T a_tX, const T a_tY, const T a_tZ) {m_tX=a_tX; m_tY=a_tY; m_tZ=a_tZ; return *this;}
	CGDM_Vector3D<T>& set(const CGDM_Vector3D<T>& a_cVector) {m_tX=a_cVector.m_tX; m_tY=a_cVector.m_tY; m_tZ=a_cVector.m_tZ;return *this;}

	//! Get length of the vector.
	T getLength() const { return (T) sqrt((GDEM_DOUBLE)(m_tX*m_tX + m_tY*m_tY + m_tZ*m_tZ)); }

	//! Get squared length of the vector.
	/** This is useful because it is much faster than getLength().
	\return Squared length of the vector. */
	T getLengthSQ() const { return m_tX*m_tX + m_tY*m_tY + m_tZ*m_tZ; }

	//! Get the dot product with another vector.
	T dotProduct(const CGDM_Vector3D<T>& a_cOther) const
	{
		return m_tX*a_cOther.m_tX + m_tY*a_cOther.m_tY + m_tZ*a_cOther.m_tZ;
	}

	//! Get distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistanceFrom(const CGDM_Vector3D<T>& a_cOther) const
	{
		return CGDM_Vector3D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY, m_tZ - a_cOther.m_tZ).getLength();
	}

	//! Get distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistance(const T &x, const T &y, const T &z) const
	{
		return CGDM_Vector3D<T>(m_tX - x, m_tY - y, m_tZ - z).getLength();
	}
	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistanceFromSQ(const CGDM_Vector3D<T>& a_cOther) const
	{
		return CGDM_Vector3D<T>(m_tX - a_cOther.m_tX, m_tY - a_cOther.m_tY, m_tZ - a_cOther.m_tZ).getLengthSQ();
	}

	//! Calculates the cross product with another vector.
	/** \param a_cVector Vector to multiply with.
	\return Crossproduct of this vector with a_cVector. */
	CGDM_Vector3D<T> crossProduct(const CGDM_Vector3D<T>& a_cVector) const
	{
		return CGDM_Vector3D<T>(m_tY * a_cVector.m_tZ - m_tZ * a_cVector.m_tY, m_tZ * a_cVector.m_tX - m_tX * a_cVector.m_tZ, m_tX * a_cVector.m_tY - m_tY * a_cVector.m_tX);
	}

	//! Returns if this vector interpreted as a point is on a line between two a_cOther points.
	/** It is assumed that the point is on the line.
	\param begin Beginning vector to compare between.
	\param end Ending vector to compare between.
	\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const CGDM_Vector3D<T>& begin, const CGDM_Vector3D<T>& end) const
	{
		const T f = (end - begin).getLengthSQ();
		return getDistanceFromSQ(begin) <= f &&
			getDistanceFromSQ(end) <= f;
	}

	//! Normalizes the vector.
	/** In case of the 0 vector the result is still 0, otherwise
	the length of the vector will be 1.
	\return Reference to this vector after normalization. */
	CGDM_Vector3D<T>& normalize()
	{
		double length = (double)(m_tX*m_tX + m_tY*m_tY + m_tZ*m_tZ);
		if (gdm_equals(length, (double)0 , (double)1E-30)) // chaki
			return *this;
		length = gdm_reciprocal_squareroot ( (double)length );
		m_tX = (T)(m_tX * length);
		m_tY = (T)(m_tY * length);
		m_tZ = (T)(m_tZ * length);
		return *this;
	}

	//! Sets the length of the vector to a new value
	CGDM_Vector3D<T>& setLength(T newlength)
	{
		normalize();
		return (*this *= newlength);
	}

	//! Inverts the vector.
	CGDM_Vector3D<T>& invert()
	{
		m_tX *= -1.0f;
		m_tY *= -1.0f;
		m_tZ *= -1.0f;
		return *this;
	}

	//! Rotates the vector by a specified number of degrees around the m_tY axis and the specified center.
	/** \param degrees Number of degrees to rotate around the m_tY axis.
	\param center The center of the rotation. */
	void rotateXZBy(GDEM_DOUBLE degrees, const CGDM_Vector3D<T>& center=CGDM_Vector3D<T>())
	{
		degrees *= gdm_DEGTORAD64;
		GDEM_DOUBLE cs = cos(degrees);
		GDEM_DOUBLE sn = sin(degrees);
		m_tX -= center.m_tX;
		m_tZ -= center.m_tZ;
		set((T)(m_tX*cs - m_tZ*sn), m_tY, (T)(m_tX*sn + m_tZ*cs));
		m_tX += center.m_tX;
		m_tZ += center.m_tZ;
	}

	//! Rotates the vector by a specified number of degrees around the m_tZ axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the m_tZ axis.
	\param center: The center of the rotation. */
	void rotateXYBy(GDEM_DOUBLE degrees, const CGDM_Vector3D<T>& center=CGDM_Vector3D<T>())
	{
		degrees *= gdm_DEGTORAD64;
		GDEM_DOUBLE cs = cos(degrees);
		GDEM_DOUBLE sn = sin(degrees);
		m_tX -= center.m_tX;
		m_tY -= center.m_tY;
		set((T)(m_tX*cs - m_tY*sn), (T)(m_tX*sn + m_tY*cs), m_tZ);
		m_tX += center.m_tX;
		m_tY += center.m_tY;
	}

	//! Rotates the vector by a specified number of degrees around the m_tX axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the m_tX axis.
	\param center: The center of the rotation. */
	void rotateYZBy(GDEM_DOUBLE degrees, const CGDM_Vector3D<T>& center=CGDM_Vector3D<T>())
	{
		degrees *= gdm_DEGTORAD64;
		GDEM_DOUBLE cs = cos(degrees);
		GDEM_DOUBLE sn = sin(degrees);
		m_tZ -= center.m_tZ;
		m_tY -= center.m_tY;
		set(m_tX, (T)(m_tY*cs - m_tZ*sn), (T)(m_tY*sn + m_tZ*cs));
		m_tZ += center.m_tZ;
		m_tY += center.m_tY;
	}

	//! Creates an interpolated vector between this vector and another vector.
	/** \param a_cOther The a_cOther vector to interpolate with.
	\param d Interpolation value between 0.0f (all the a_cOther vector) and 1.0f (all this vector).
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	\return An interpolated vector.  This vector is not modified. */
	CGDM_Vector3D<T> getInterpolated(const CGDM_Vector3D<T>& a_cOther, GDEM_DOUBLE d) const
	{
		const GDEM_DOUBLE inv = 1.0 - d;
		return CGDM_Vector3D<T>((T)(a_cOther.m_tX*inv + m_tX*d), (T)(a_cOther.m_tY*inv + m_tY*d), (T)(a_cOther.m_tZ*inv + m_tZ*d));
	}

	//! Creates a quadratically interpolated vector between this and two a_cOther vectors.
	/** \param v2 Second vector to interpolate with.
	\param v3 Third vector to interpolate with (maximum at 1.0f)
	\param d Interpolation value between 0.0f (all this vector) and 1.0f (all the 3rd vector).
	Note that this is the opposite direction of interpolation to getInterpolated() and interpolate()
	\return An interpolated vector. This vector is not modified. */
	CGDM_Vector3D<T> getInterpolated_quadratic(const CGDM_Vector3D<T>& v2, const CGDM_Vector3D<T>& v3, GDEM_DOUBLE d) const
	{
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		const GDEM_DOUBLE inv = (T) 1.0 - d;
		const GDEM_DOUBLE mul0 = inv * inv;
		const GDEM_DOUBLE mul1 = (T) 2.0 * d * inv;
		const GDEM_DOUBLE mul2 = d * d;

		return CGDM_Vector3D<T> ((T)(m_tX * mul0 + v2.m_tX * mul1 + v3.m_tX * mul2),
				(T)(m_tY * mul0 + v2.m_tY * mul1 + v3.m_tY * mul2),
				(T)(m_tZ * mul0 + v2.m_tZ * mul1 + v3.m_tZ * mul2));
	}

	//! Sets this vector to the linearly interpolated vector between a and b.
	/** \param a first vector to interpolate with, maximum at 1.0f
	\param b second vector to interpolate with, maximum at 0.0f
	\param d Interpolation value between 0.0f (all vector b) and 1.0f (all vector a)
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	*/
	CGDM_Vector3D<T>& interpolate(const CGDM_Vector3D<T>& a, const CGDM_Vector3D<T>& b, GDEM_DOUBLE d)
	{
		m_tX = (T)((GDEM_DOUBLE)b.m_tX + ( ( a.m_tX - b.m_tX ) * d ));
		m_tY = (T)((GDEM_DOUBLE)b.m_tY + ( ( a.m_tY - b.m_tY ) * d ));
		m_tZ = (T)((GDEM_DOUBLE)b.m_tZ + ( ( a.m_tZ - b.m_tZ ) * d ));
		return *this;
	}


	//! Get the rotations that would make a (0,0,1) direction vector point in the same direction as this direction vector.
	/** Thanks to Arras on the Geo forums for this method.  This utility method is very useful for
	orienting scene nodes towards specific targets.  For example, if this vector represents the difference
	between two scene nodes, then applying the result of getHorizontalAngle() to one scene node will point
	it at the a_cOther one.
	Example code:
	// Where target and seeker are of type ISceneNode*
	const vector3df toTarget(target->getAbsolutePosition() - seeker->getAbsolutePosition());
	const vector3df requiredRotation = toTarget.getHorizontalAngle();
	seeker->setRotation(requiredRotation); 

	\return A rotation vector containing the m_tX (pitch) and m_tY (raw) rotations (in degrees) that when applied to a 
	+m_tZ (e.g. 0, 0, 1) direction vector would make it point in the same direction as this vector. The m_tZ (roll) rotation 
	is always 0, since two Euler rotations are sufficient to point in any given direction. */
	CGDM_Vector3D<T> getHorizontalAngle() const
	{
		CGDM_Vector3D<T> angle;

		angle.m_tY = (T)(atan2(m_tX, m_tZ) * gdm_RADTODEG64);

		if (angle.m_tY < 0.0f)
			angle.m_tY += 360.0f;
		if (angle.m_tY >= 360.0f)
			angle.m_tY -= 360.0f;

		const GDEM_DOUBLE z1 = sqrt(m_tX*m_tX + m_tZ*m_tZ);

		angle.m_tX = (T)(atan2(z1, (GDEM_DOUBLE)m_tY) * gdm_RADTODEG64 - 90.0);

		if (angle.m_tX < 0.0f)
			angle.m_tX += 360.0f;
		if (angle.m_tX >= 360.0f)
			angle.m_tX -= 360.0f;

		return angle;
	}	

	//! Builds a direction vector from (this) rotation vector.
	/** This vector is assumed to be a rotation vector composed of 3 Euler angle rotations, in degrees.
	The implementation performs the same calculations as using a matrix to do the rotation.

	\param[in] forwards  The direction representing "forwards" which will be rotated by this vector. 
	If you do not provide a direction, then the +m_tZ axis (0, 0, 1) will be assumed to be forwards.
	\return A direction vector calculated by rotating the forwards direction by the 3 Euler angles 
	(in degrees) represented by this vector. */
	CGDM_Vector3D<T> rotationToDirection(const CGDM_Vector3D<T> & forwards = CGDM_Vector3D<T>(0, 0, 1)) const
	{
		const GDEM_DOUBLE cr = cos( gdm_DEGTORAD64 * m_tX );
		const GDEM_DOUBLE sr = sin( gdm_DEGTORAD64 * m_tX );
		const GDEM_DOUBLE cp = cos( gdm_DEGTORAD64 * m_tY );
		const GDEM_DOUBLE sp = sin( gdm_DEGTORAD64 * m_tY );
		const GDEM_DOUBLE cy = cos( gdm_DEGTORAD64 * m_tZ );
		const GDEM_DOUBLE sy = sin( gdm_DEGTORAD64 * m_tZ );

		const GDEM_DOUBLE srsp = sr*sp;
		const GDEM_DOUBLE crsp = cr*sp;

		const GDEM_DOUBLE pseudoMatrix[] = {
			( cp*cy ), ( cp*sy ), ( -sp ),
			( srsp*cy-cr*sy ), ( srsp*sy+cr*cy ), ( sr*cp ),
			( crsp*cy+sr*sy ), ( crsp*sy-sr*cy ), ( cr*cp )};

		return CGDM_Vector3D<T>(
			(T)(forwards.m_tX * pseudoMatrix[0] +
				forwards.m_tY * pseudoMatrix[3] +
				forwards.m_tZ * pseudoMatrix[6]),
			(T)(forwards.m_tX * pseudoMatrix[1] +
				forwards.m_tY * pseudoMatrix[4] +
				forwards.m_tZ * pseudoMatrix[7]),
			(T)(forwards.m_tX * pseudoMatrix[2] +
				forwards.m_tY * pseudoMatrix[5] +
				forwards.m_tZ * pseudoMatrix[8]));
	}

	//! Fills an array of 4 values with the vector data (usually floats).
	/** Useful for setting in shader constants for example. The fourth value
	will always be 0. */
	void getAs4Values(T* array) const
	{
		array[0] = m_tX;
		array[1] = m_tY;
		array[2] = m_tZ;
		array[3] = 0;
	}

	//! m_tX coordinate of the vector
	T m_tX;
	//! m_tY coordinate of the vector
	T m_tY;
	//! m_tZ coordinate of the vector
	T m_tZ;
};

	typedef CGDM_Vector3D<double> Vector3DF;
	//! Typedef for a double 3d vector.
	typedef CGDM_Vector3D<double> CGDM_Vector3DF;
	//! Typedef for a double 3d vector.
	typedef CGDM_Vector3D<double> CGDM_Location3D;	// (longititude, latitude, height)
	//! Typedef for an integer 3d vector.
	typedef CGDM_Vector3D<GDEM_INT> CGDM_Vector3DI;
	
	//! Typedef for a double 3d point.
	typedef CGDM_Vector3DF	CGDM_Point3DF;
	//! Typedef for an integer 3d vector.
	typedef CGDM_Vector3DI	CGDM_Point3DI;

	//! Function multiplying a scalar and a vector component-wise.
	template<class S, class T>
	CGDM_Vector3D<T> operator*(const S scalar, const CGDM_Vector3D<T>& vector) { return vector*scalar; }

	typedef gdm_array<CGDM_Vector3DF>	CGDM_VectorArr;
	typedef gdm_array<CGDM_Location3D>	CGDM_LocationArr;
	typedef gdm_array<CGDM_LocationArr>	CGDM_LocationPolygonArr;	// the array of Polygon

	typedef struct _gdem_mesh
	{
		CGDM_Location3D	pt1; // location point
		CGDM_Location3D	pt2; // location point	
		CGDM_Location3D	pt3; // location point	
	}GDEM_MESH;

	typedef gdm_array<GDEM_MESH>	CGDM_MeshArr;
	typedef gdm_array<GDEM_MESH>	Triangle3DArray;

#endif // __GDEM_VECTOR3D_H_INCLUDED__

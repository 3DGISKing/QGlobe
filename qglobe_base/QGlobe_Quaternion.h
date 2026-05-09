#ifndef __QGLOBE_QUATERNION_H_INCLUDED__
#define __QGLOBE_QUATERNION_H_INCLUDED__

#include "QGlobe_Define.h"
#include "QGlobe_Math.h"
#include "QGlobe_Vector3D.h"
#include "QGlobe_Matrix4.h"

//! Quaternion class for representing rotations.
/** It provides cheap combinations and avoids gimbal locks.
Also useful for interpolations. */
class CQGlobe_Quaternion
{
	public:

		//! Default Constructor
		CQGlobe_Quaternion() : m_fX(0.0f), m_fY(0.0f), m_fZ(0.0f), m_fW(1.0f) {}

		//! Constructor
		CQGlobe_Quaternion(double x, double y, double z, double w) : m_fX(x), m_fY(y), m_fZ(z), m_fW(w) { }

		//! Constructor which converts euler angles (radians) to a CQGlobe_Quaternion
		CQGlobe_Quaternion(double x, double y, double z);

		//! Constructor which converts euler angles (radians) to a CQGlobe_Quaternion
		CQGlobe_Quaternion(const CQGlobe_Vector3DF& vec);

		//! Constructor which converts a matrix to a CQGlobe_Quaternion
		CQGlobe_Quaternion(const CQGlobe_Matrix4& mat);

		//! Equalilty operator
		bool operator==(const CQGlobe_Quaternion& other) const;

		//! Assignment operator
		inline CQGlobe_Quaternion& operator=(const CQGlobe_Quaternion& other);

		//! Matrix assignment operator
		inline CQGlobe_Quaternion& operator=(const CQGlobe_Matrix4& other);

		//! Add operator
		CQGlobe_Quaternion operator+(const CQGlobe_Quaternion& other) const;

		//! Multiplication operator
		CQGlobe_Quaternion operator*(const CQGlobe_Quaternion& other) const;

		//! Multiplication operator with scalar
		CQGlobe_Quaternion operator*(double s) const;

		//! Multiplication operator with scalar
		CQGlobe_Quaternion& operator*=(double s);

		//! Multiplication operator
		CQGlobe_Vector3DF operator*(const CQGlobe_Vector3DF& v) const;

		//! Multiplication operator
		CQGlobe_Quaternion& operator*=(const CQGlobe_Quaternion& other);

		//! Calculates the dot product
		inline double dotProduct(const CQGlobe_Quaternion& other) const;

		//! Sets new CQGlobe_Quaternion
		inline CQGlobe_Quaternion& set(double x, double y, double z, double w);

		//! Sets new CQGlobe_Quaternion based on euler angles (radians)
		inline CQGlobe_Quaternion& set(double x, double y, double z);

		//! Sets new CQGlobe_Quaternion based on euler angles (radians)
		inline CQGlobe_Quaternion& set(const CQGlobe_Vector3DF& vec);

		//! Normalizes the CQGlobe_Quaternion
		inline CQGlobe_Quaternion& normalize();

		//! Creates a matrix from this CQGlobe_Quaternion
		CQGlobe_Matrix4 getMatrix() const;

		//! Creates a matrix from this CQGlobe_Quaternion
		void getMatrix( CQGlobe_Matrix4 &dest ) const;

		//! Creates a matrix from this CQGlobe_Quaternion
		inline void getMatrix_transposed( CQGlobe_Matrix4 &dest ) const;

		//! Inverts this CQGlobe_Quaternion
		CQGlobe_Quaternion& makeInverse();

		//! Set this CQGlobe_Quaternion to the result of the interpolation between two quaternions
		CQGlobe_Quaternion& slerp( CQGlobe_Quaternion q1, CQGlobe_Quaternion q2, double interpolate );

		//! Create CQGlobe_Quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The CQGlobe_Quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		CQGlobe_Quaternion& fromAngleAxis (double angle, const CQGlobe_Vector3DF& axis);

		//! Fills an angle (radians) around an axis (unit vector)
		void toAngleAxis (double &angle, CQGlobe_Vector3DF& axis) const;

		//! Output this CQGlobe_Quaternion to an euler angle (radians)
		void toEuler(CQGlobe_Vector3DF& euler) const;

		//! Set CQGlobe_Quaternion to identity
		CQGlobe_Quaternion& makeIdentity();

		//! Set CQGlobe_Quaternion to represent a rotation from one vector to another.
		CQGlobe_Quaternion& rotationFromTo(const CQGlobe_Vector3DF& from, const CQGlobe_Vector3DF& to);

		//! Quaternion elements.
		double m_fX, m_fY, m_fZ, m_fW;
};


// Constructor which converts euler angles to a CQGlobe_Quaternion
inline CQGlobe_Quaternion::CQGlobe_Quaternion(double x, double y, double z)
{
	set(x,y,z);
}


// Constructor which converts euler angles to a CQGlobe_Quaternion
inline CQGlobe_Quaternion::CQGlobe_Quaternion(const CQGlobe_Vector3DF& vec)
{
	set(vec.m_tX,vec.m_tY,vec.m_tX);
}


// Constructor which converts a matrix to a CQGlobe_Quaternion
inline CQGlobe_Quaternion::CQGlobe_Quaternion(const CQGlobe_Matrix4& mat)
{
	(*this) = mat;
}


// equal operator
inline bool CQGlobe_Quaternion::operator==(const CQGlobe_Quaternion& other) const
{
	return ((m_fX == other.m_fX) &&
		(m_fY == other.m_fY) &&
		(m_fZ == other.m_fZ) &&
		(m_fW == other.m_fW));
}


// assignment operator
inline CQGlobe_Quaternion& CQGlobe_Quaternion::operator=(const CQGlobe_Quaternion& other)
{
	m_fX = other.m_fX;
	m_fY = other.m_fY;
	m_fZ = other.m_fZ;
	m_fW = other.m_fW;
	return *this;
}


// matrix assignment operator
inline CQGlobe_Quaternion& CQGlobe_Quaternion::operator=(const CQGlobe_Matrix4& m)
{
	const double diag = m(0,0) + m(1,1) + m(2,2) + 1;

	if( diag > 0.0f )
	{
		const double scale = sqrtf(diag) * 2.0f; // get scale from diagonal

		// TODO: speed this up
		m_fX = ( m(2,1) - m(1,2)) / scale;
		m_fY = ( m(0,2) - m(2,0)) / scale;
		m_fZ = ( m(1,0) - m(0,1)) / scale;
		m_fW = 0.25f * scale;
	}
	else
	{
		if ( m(0,0) > m(1,1) && m(0,0) > m(2,2))
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			const double scale = sqrtf( 1.0f + m(0,0) - m(1,1) - m(2,2)) * 2.0f;

			// TODO: speed this up
			m_fX = 0.25f * scale;
			m_fY = (m(0,1) + m(1,0)) / scale;
			m_fZ = (m(2,0) + m(0,2)) / scale;
			m_fW = (m(2,1) - m(1,2)) / scale;
		}
		else if ( m(1,1) > m(2,2))
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const double scale = sqrtf( 1.0f + m(1,1) - m(0,0) - m(2,2)) * 2.0f;

			// TODO: speed this up
			m_fX = (m(0,1) + m(1,0) ) / scale;
			m_fY = 0.25f * scale;
			m_fZ = (m(1,2) + m(2,1) ) / scale;
			m_fW = (m(0,2) - m(2,0) ) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			const double scale = sqrtf( 1.0f + m(2,2) - m(0,0) - m(1,1)) * 2.0f;

			// TODO: speed this up
			m_fX = (m(0,2) + m(2,0)) / scale;
			m_fY = (m(1,2) + m(2,1)) / scale;
			m_fZ = 0.25f * scale;
			m_fW = (m(1,0) - m(0,1)) / scale;
		}
	}

	return normalize();
}


// multiplication operator
inline CQGlobe_Quaternion CQGlobe_Quaternion::operator*(const CQGlobe_Quaternion& other) const
{
	CQGlobe_Quaternion tmp;

	tmp.m_fW = (other.m_fW * m_fW) - (other.m_fX * m_fX) - (other.m_fY * m_fY) - (other.m_fZ * m_fZ);
	tmp.m_fX = (other.m_fW * m_fX) + (other.m_fX * m_fW) + (other.m_fY * m_fZ) - (other.m_fZ * m_fY);
	tmp.m_fY = (other.m_fW * m_fY) + (other.m_fY * m_fW) + (other.m_fZ * m_fX) - (other.m_fX * m_fZ);
	tmp.m_fZ = (other.m_fW * m_fZ) + (other.m_fZ * m_fW) + (other.m_fX * m_fY) - (other.m_fY * m_fX);

	return tmp;
}


// multiplication operator
inline CQGlobe_Quaternion CQGlobe_Quaternion::operator*(double s) const
{
	return CQGlobe_Quaternion(s*m_fX, s*m_fY, s*m_fZ, s*m_fW);
}

// multiplication operator
inline CQGlobe_Quaternion& CQGlobe_Quaternion::operator*=(double s)
{
	m_fX*=s;
	m_fY*=s;
	m_fZ*=s;
	m_fW*=s;
	return *this;
}

// multiplication operator
inline CQGlobe_Quaternion& CQGlobe_Quaternion::operator*=(const CQGlobe_Quaternion& other)
{
	return (*this = other * (*this));
}

// add operator
inline CQGlobe_Quaternion CQGlobe_Quaternion::operator+(const CQGlobe_Quaternion& b) const
{
	return CQGlobe_Quaternion(m_fX+b.m_fX, m_fY+b.m_fY, m_fZ+b.m_fZ, m_fW+b.m_fW);
}


// Creates a matrix from this CQGlobe_Quaternion
inline CQGlobe_Matrix4 CQGlobe_Quaternion::getMatrix() const
{
	CQGlobe_Matrix4 m;
	getMatrix_transposed(m);
	return m;
}


// Creates a matrix from this CQGlobe_Quaternion
inline void CQGlobe_Quaternion::getMatrix( CQGlobe_Matrix4 &dest ) const
{
	dest[0] = 1.0f - 2.0f*m_fY*m_fY - 2.0f*m_fZ*m_fZ;
	dest[1] = 2.0f*m_fX*m_fY + 2.0f*m_fZ*m_fW;
	dest[2] = 2.0f*m_fX*m_fZ - 2.0f*m_fY*m_fW;
	dest[3] = 0.0f;

	dest[4] = 2.0f*m_fX*m_fY - 2.0f*m_fZ*m_fW;
	dest[5] = 1.0f - 2.0f*m_fX*m_fX - 2.0f*m_fZ*m_fZ;
	dest[6] = 2.0f*m_fZ*m_fY + 2.0f*m_fX*m_fW;
	dest[7] = 0.0f;

	dest[8] = 2.0f*m_fX*m_fZ + 2.0f*m_fY*m_fW;
	dest[9] = 2.0f*m_fZ*m_fY - 2.0f*m_fX*m_fW;
	dest[10] = 1.0f - 2.0f*m_fX*m_fX - 2.0f*m_fY*m_fY;
	dest[11] = 0.0f;

	dest[12] = 0.f;
	dest[13] = 0.f;
	dest[14] = 0.f;
	dest[15] = 1.f;
}

// Creates a matrix from this CQGlobe_Quaternion
inline void CQGlobe_Quaternion::getMatrix_transposed( CQGlobe_Matrix4 &dest ) const
{
	dest[0] = 1.0f - 2.0f*m_fY*m_fY - 2.0f*m_fZ*m_fZ;
	dest[4] = 2.0f*m_fX*m_fY + 2.0f*m_fZ*m_fW;
	dest[8] = 2.0f*m_fX*m_fZ - 2.0f*m_fY*m_fW;
	dest[12] = 0.0f;

	dest[1] = 2.0f*m_fX*m_fY - 2.0f*m_fZ*m_fW;
	dest[5] = 1.0f - 2.0f*m_fX*m_fX - 2.0f*m_fZ*m_fZ;
	dest[9] = 2.0f*m_fZ*m_fY + 2.0f*m_fX*m_fW;
	dest[13] = 0.0f;

	dest[2] = 2.0f*m_fX*m_fZ + 2.0f*m_fY*m_fW;
	dest[6] = 2.0f*m_fZ*m_fY - 2.0f*m_fX*m_fW;
	dest[10] = 1.0f - 2.0f*m_fX*m_fX - 2.0f*m_fY*m_fY;
	dest[14] = 0.0f;

	dest[3] = 0.f;
	dest[7] = 0.f;
	dest[11] = 0.f;
	dest[15] = 1.f;
}



// Inverts this CQGlobe_Quaternion
inline CQGlobe_Quaternion& CQGlobe_Quaternion::makeInverse()
{
	m_fX = -m_fX; m_fY = -m_fY; m_fZ = -m_fZ;
	return *this;
}

// sets new CQGlobe_Quaternion
inline CQGlobe_Quaternion& CQGlobe_Quaternion::set(double x, double y, double z, double w)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fW = w;
	return *this;
}


// sets new CQGlobe_Quaternion based on euler angles
inline CQGlobe_Quaternion& CQGlobe_Quaternion::set(double x, double y, double z)
{
	QGLOBE_DOUBLE angle;

	angle = x * 0.5;
	const QGLOBE_DOUBLE sr = sin(angle);
	const QGLOBE_DOUBLE cr = cos(angle);

	angle = y * 0.5;
	const QGLOBE_DOUBLE sp = sin(angle);
	const QGLOBE_DOUBLE cp = cos(angle);

	angle = z * 0.5;
	const QGLOBE_DOUBLE sy = sin(angle);
	const QGLOBE_DOUBLE cy = cos(angle);

	const QGLOBE_DOUBLE cpcy = cp * cy;
	const QGLOBE_DOUBLE spcy = sp * cy;
	const QGLOBE_DOUBLE cpsy = cp * sy;
	const QGLOBE_DOUBLE spsy = sp * sy;

	m_fX = (double)(sr * cpcy - cr * spsy);
	m_fY = (double)(cr * spcy + sr * cpsy);
	m_fZ = (double)(cr * cpsy - sr * spcy);
	m_fW = (double)(cr * cpcy + sr * spsy);

	return normalize();
}

// sets new CQGlobe_Quaternion based on euler angles
inline CQGlobe_Quaternion& CQGlobe_Quaternion::set(const CQGlobe_Vector3DF& vec)
{
	return set(vec.m_tX, vec.m_tY, vec.m_tZ);
}

// normalizes the CQGlobe_Quaternion
inline CQGlobe_Quaternion& CQGlobe_Quaternion::normalize()
{
	const double n = m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ + m_fW*m_fW;

	if (n == 1)
		return *this;

	//n = 1.0f / sqrtf(n);
	return (*this *= qglobe_reciprocal_squareroot ( n ));
}


// set this CQGlobe_Quaternion to the result of the interpolation between two quaternions
inline CQGlobe_Quaternion& CQGlobe_Quaternion::slerp(CQGlobe_Quaternion q1, CQGlobe_Quaternion q2, double time)
{
	double angle = q1.dotProduct(q2);

	if (angle < 0.0f)
	{
		q1 *= -1.0f;
		angle *= -1.0f;
	}

	double scale;
	double invscale;

	if ((angle + 1.0f) > 0.05f)
	{
		if ((1.0f - angle) >= 0.05f) // spherical interpolation
		{
			const double theta = acosf(angle);
			const double invsintheta = qglobe_reciprocal(sinf(theta));
			scale = sinf(theta * (1.0f-time)) * invsintheta;
			invscale = sinf(theta * time) * invsintheta;
		}
		else // linear interploation
		{
			scale = 1.0f - time;
			invscale = time;
		}
	}
	else
	{
		q2.set(-q1.m_fY, q1.m_fX, -q1.m_fW, q1.m_fZ);
		scale = sinf(qglobe_PI * (0.5f - time));
		invscale = sinf(qglobe_PI * time);
	}

	return (*this = (q1*scale) + (q2*invscale));
}


// calculates the dot product
inline double CQGlobe_Quaternion::dotProduct(const CQGlobe_Quaternion& q2) const
{
	return (m_fX * q2.m_fX) + (m_fY * q2.m_fY) + (m_fZ * q2.m_fZ) + (m_fW * q2.m_fW);
}


//! axis must be unit length
//! angle in radians
inline CQGlobe_Quaternion& CQGlobe_Quaternion::fromAngleAxis(double angle, const CQGlobe_Vector3DF& axis)
{
	const double fHalfAngle = 0.5f*angle;
	const double fSin = sinf(fHalfAngle);
	m_fW = cosf(fHalfAngle);
	m_fX = fSin*axis.m_tX;
	m_fY = fSin*axis.m_tY;
	m_fZ = fSin*axis.m_tZ;
	return *this;
}


inline void CQGlobe_Quaternion::toAngleAxis(double &angle, CQGlobe_Vector3DF &axis) const
{
	const double scale = sqrtf(m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ);

	if (qglobe_iszero(scale) || m_fW > 1.0f || m_fW < -1.0f)
	{
		angle = 0.0f;
		axis.m_tX = 0.0f;
		axis.m_tY = 1.0f;
		axis.m_tZ = 0.0f;
	}
	else
	{
		const double invscale = qglobe_reciprocal(scale);
		angle = 2.0f * acosf(m_fW);
		axis.m_tX = m_fX * invscale;
		axis.m_tY = m_fY * invscale;
		axis.m_tZ = m_fZ * invscale;
	}
}

inline void CQGlobe_Quaternion::toEuler(CQGlobe_Vector3DF& euler) const
{
	const QGLOBE_DOUBLE sqw = m_fW*m_fW;
	const QGLOBE_DOUBLE sqx = m_fX*m_fX;
	const QGLOBE_DOUBLE sqy = m_fY*m_fY;
	const QGLOBE_DOUBLE sqz = m_fZ*m_fZ;

	// heading = rotation about z-axis
	euler.m_tZ = (double) (atan2(2.0 * (m_fX*m_fY +m_fZ*m_fW),(sqx - sqy - sqz + sqw)));

	// bank = rotation about x-axis
	euler.m_tX = (double) (atan2(2.0 * (m_fY*m_fZ +m_fX*m_fW),(-sqx - sqy + sqz + sqw)));

	// attitude = rotation about y-axis
	euler.m_tY = asin( qglobe_clamp(-2.0 * (m_fX*m_fZ - m_fY*m_fW), -1.0, 1.0) );
}


inline CQGlobe_Vector3DF CQGlobe_Quaternion::operator* (const CQGlobe_Vector3DF& v) const
{
	// nVidia SDK implementation

	CQGlobe_Vector3DF uv, uuv;
	CQGlobe_Vector3DF qvec(m_fX, m_fY, m_fZ);
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * m_fW);
	uuv *= 2.0f;

	return v + uv + uuv;
}

// set CQGlobe_Quaternion to identity
inline CQGlobe_Quaternion& CQGlobe_Quaternion::makeIdentity()
{
	m_fW = 1.f;
	m_fX = 0.f;
	m_fY = 0.f;
	m_fZ = 0.f;
	return *this;
}

inline CQGlobe_Quaternion& CQGlobe_Quaternion::rotationFromTo(const CQGlobe_Vector3DF& from, const CQGlobe_Vector3DF& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	CQGlobe_Vector3DF v0 = from;
	CQGlobe_Vector3DF v1 = to;
	v0.normalize();
	v1.normalize();

	const double d = v0.dotProduct(v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return makeIdentity();
	}

	const double s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
	const double invs = 1.f / s;
	const CQGlobe_Vector3DF c = v0.crossProduct(v1)*invs;

	m_fX = c.m_tX;
	m_fY = c.m_tY;
	m_fZ = c.m_tZ;
	m_fW = s * 0.5f;

	return *this;
}

#endif // __QGLOBE_QUATERNION_H_INCLUDED__


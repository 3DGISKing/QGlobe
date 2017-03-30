#ifndef __GDEM_QUATERNION_H_INCLUDED__
#define __GDEM_QUATERNION_H_INCLUDED__

#include "GDM_Define.h"
#include "GDM_Math.h"
#include "GDM_Vector3D.h"
#include "GDM_Matrix4.h"

//! Quaternion class for representing rotations.
/** It provides cheap combinations and avoids gimbal locks.
Also useful for interpolations. */
class CGDM_Quaternion
{
	public:

		//! Default Constructor
		CGDM_Quaternion() : m_fX(0.0f), m_fY(0.0f), m_fZ(0.0f), m_fW(1.0f) {}

		//! Constructor
		CGDM_Quaternion(double x, double y, double z, double w) : m_fX(x), m_fY(y), m_fZ(z), m_fW(w) { }

		//! Constructor which converts euler angles (radians) to a CGDM_Quaternion
		CGDM_Quaternion(double x, double y, double z);

		//! Constructor which converts euler angles (radians) to a CGDM_Quaternion
		CGDM_Quaternion(const CGDM_Vector3DF& vec);

		//! Constructor which converts a matrix to a CGDM_Quaternion
		CGDM_Quaternion(const CGDM_Matrix4& mat);

		//! Equalilty operator
		bool operator==(const CGDM_Quaternion& other) const;

		//! Assignment operator
		inline CGDM_Quaternion& operator=(const CGDM_Quaternion& other);

		//! Matrix assignment operator
		inline CGDM_Quaternion& operator=(const CGDM_Matrix4& other);

		//! Add operator
		CGDM_Quaternion operator+(const CGDM_Quaternion& other) const;

		//! Multiplication operator
		CGDM_Quaternion operator*(const CGDM_Quaternion& other) const;

		//! Multiplication operator with scalar
		CGDM_Quaternion operator*(double s) const;

		//! Multiplication operator with scalar
		CGDM_Quaternion& operator*=(double s);

		//! Multiplication operator
		CGDM_Vector3DF operator*(const CGDM_Vector3DF& v) const;

		//! Multiplication operator
		CGDM_Quaternion& operator*=(const CGDM_Quaternion& other);

		//! Calculates the dot product
		inline double dotProduct(const CGDM_Quaternion& other) const;

		//! Sets new CGDM_Quaternion
		inline CGDM_Quaternion& set(double x, double y, double z, double w);

		//! Sets new CGDM_Quaternion based on euler angles (radians)
		inline CGDM_Quaternion& set(double x, double y, double z);

		//! Sets new CGDM_Quaternion based on euler angles (radians)
		inline CGDM_Quaternion& set(const CGDM_Vector3DF& vec);

		//! Normalizes the CGDM_Quaternion
		inline CGDM_Quaternion& normalize();

		//! Creates a matrix from this CGDM_Quaternion
		CGDM_Matrix4 getMatrix() const;

		//! Creates a matrix from this CGDM_Quaternion
		void getMatrix( CGDM_Matrix4 &dest ) const;

		//! Creates a matrix from this CGDM_Quaternion
		inline void getMatrix_transposed( CGDM_Matrix4 &dest ) const;

		//! Inverts this CGDM_Quaternion
		CGDM_Quaternion& makeInverse();

		//! Set this CGDM_Quaternion to the result of the interpolation between two quaternions
		CGDM_Quaternion& slerp( CGDM_Quaternion q1, CGDM_Quaternion q2, double interpolate );

		//! Create CGDM_Quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The CGDM_Quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		CGDM_Quaternion& fromAngleAxis (double angle, const CGDM_Vector3DF& axis);

		//! Fills an angle (radians) around an axis (unit vector)
		void toAngleAxis (double &angle, CGDM_Vector3DF& axis) const;

		//! Output this CGDM_Quaternion to an euler angle (radians)
		void toEuler(CGDM_Vector3DF& euler) const;

		//! Set CGDM_Quaternion to identity
		CGDM_Quaternion& makeIdentity();

		//! Set CGDM_Quaternion to represent a rotation from one vector to another.
		CGDM_Quaternion& rotationFromTo(const CGDM_Vector3DF& from, const CGDM_Vector3DF& to);

		//! Quaternion elements.
		double m_fX, m_fY, m_fZ, m_fW;
};


// Constructor which converts euler angles to a CGDM_Quaternion
inline CGDM_Quaternion::CGDM_Quaternion(double x, double y, double z)
{
	set(x,y,z);
}


// Constructor which converts euler angles to a CGDM_Quaternion
inline CGDM_Quaternion::CGDM_Quaternion(const CGDM_Vector3DF& vec)
{
	set(vec.m_tX,vec.m_tY,vec.m_tX);
}


// Constructor which converts a matrix to a CGDM_Quaternion
inline CGDM_Quaternion::CGDM_Quaternion(const CGDM_Matrix4& mat)
{
	(*this) = mat;
}


// equal operator
inline bool CGDM_Quaternion::operator==(const CGDM_Quaternion& other) const
{
	return ((m_fX == other.m_fX) &&
		(m_fY == other.m_fY) &&
		(m_fZ == other.m_fZ) &&
		(m_fW == other.m_fW));
}


// assignment operator
inline CGDM_Quaternion& CGDM_Quaternion::operator=(const CGDM_Quaternion& other)
{
	m_fX = other.m_fX;
	m_fY = other.m_fY;
	m_fZ = other.m_fZ;
	m_fW = other.m_fW;
	return *this;
}


// matrix assignment operator
inline CGDM_Quaternion& CGDM_Quaternion::operator=(const CGDM_Matrix4& m)
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
inline CGDM_Quaternion CGDM_Quaternion::operator*(const CGDM_Quaternion& other) const
{
	CGDM_Quaternion tmp;

	tmp.m_fW = (other.m_fW * m_fW) - (other.m_fX * m_fX) - (other.m_fY * m_fY) - (other.m_fZ * m_fZ);
	tmp.m_fX = (other.m_fW * m_fX) + (other.m_fX * m_fW) + (other.m_fY * m_fZ) - (other.m_fZ * m_fY);
	tmp.m_fY = (other.m_fW * m_fY) + (other.m_fY * m_fW) + (other.m_fZ * m_fX) - (other.m_fX * m_fZ);
	tmp.m_fZ = (other.m_fW * m_fZ) + (other.m_fZ * m_fW) + (other.m_fX * m_fY) - (other.m_fY * m_fX);

	return tmp;
}


// multiplication operator
inline CGDM_Quaternion CGDM_Quaternion::operator*(double s) const
{
	return CGDM_Quaternion(s*m_fX, s*m_fY, s*m_fZ, s*m_fW);
}

// multiplication operator
inline CGDM_Quaternion& CGDM_Quaternion::operator*=(double s)
{
	m_fX*=s;
	m_fY*=s;
	m_fZ*=s;
	m_fW*=s;
	return *this;
}

// multiplication operator
inline CGDM_Quaternion& CGDM_Quaternion::operator*=(const CGDM_Quaternion& other)
{
	return (*this = other * (*this));
}

// add operator
inline CGDM_Quaternion CGDM_Quaternion::operator+(const CGDM_Quaternion& b) const
{
	return CGDM_Quaternion(m_fX+b.m_fX, m_fY+b.m_fY, m_fZ+b.m_fZ, m_fW+b.m_fW);
}


// Creates a matrix from this CGDM_Quaternion
inline CGDM_Matrix4 CGDM_Quaternion::getMatrix() const
{
	CGDM_Matrix4 m;
	getMatrix_transposed(m);
	return m;
}


// Creates a matrix from this CGDM_Quaternion
inline void CGDM_Quaternion::getMatrix( CGDM_Matrix4 &dest ) const
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

// Creates a matrix from this CGDM_Quaternion
inline void CGDM_Quaternion::getMatrix_transposed( CGDM_Matrix4 &dest ) const
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



// Inverts this CGDM_Quaternion
inline CGDM_Quaternion& CGDM_Quaternion::makeInverse()
{
	m_fX = -m_fX; m_fY = -m_fY; m_fZ = -m_fZ;
	return *this;
}

// sets new CGDM_Quaternion
inline CGDM_Quaternion& CGDM_Quaternion::set(double x, double y, double z, double w)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fW = w;
	return *this;
}


// sets new CGDM_Quaternion based on euler angles
inline CGDM_Quaternion& CGDM_Quaternion::set(double x, double y, double z)
{
	GDEM_DOUBLE angle;

	angle = x * 0.5;
	const GDEM_DOUBLE sr = sin(angle);
	const GDEM_DOUBLE cr = cos(angle);

	angle = y * 0.5;
	const GDEM_DOUBLE sp = sin(angle);
	const GDEM_DOUBLE cp = cos(angle);

	angle = z * 0.5;
	const GDEM_DOUBLE sy = sin(angle);
	const GDEM_DOUBLE cy = cos(angle);

	const GDEM_DOUBLE cpcy = cp * cy;
	const GDEM_DOUBLE spcy = sp * cy;
	const GDEM_DOUBLE cpsy = cp * sy;
	const GDEM_DOUBLE spsy = sp * sy;

	m_fX = (double)(sr * cpcy - cr * spsy);
	m_fY = (double)(cr * spcy + sr * cpsy);
	m_fZ = (double)(cr * cpsy - sr * spcy);
	m_fW = (double)(cr * cpcy + sr * spsy);

	return normalize();
}

// sets new CGDM_Quaternion based on euler angles
inline CGDM_Quaternion& CGDM_Quaternion::set(const CGDM_Vector3DF& vec)
{
	return set(vec.m_tX, vec.m_tY, vec.m_tZ);
}

// normalizes the CGDM_Quaternion
inline CGDM_Quaternion& CGDM_Quaternion::normalize()
{
	const double n = m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ + m_fW*m_fW;

	if (n == 1)
		return *this;

	//n = 1.0f / sqrtf(n);
	return (*this *= gdm_reciprocal_squareroot ( n ));
}


// set this CGDM_Quaternion to the result of the interpolation between two quaternions
inline CGDM_Quaternion& CGDM_Quaternion::slerp(CGDM_Quaternion q1, CGDM_Quaternion q2, double time)
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
			const double invsintheta = gdm_reciprocal(sinf(theta));
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
		scale = sinf(gdm_PI * (0.5f - time));
		invscale = sinf(gdm_PI * time);
	}

	return (*this = (q1*scale) + (q2*invscale));
}


// calculates the dot product
inline double CGDM_Quaternion::dotProduct(const CGDM_Quaternion& q2) const
{
	return (m_fX * q2.m_fX) + (m_fY * q2.m_fY) + (m_fZ * q2.m_fZ) + (m_fW * q2.m_fW);
}


//! axis must be unit length
//! angle in radians
inline CGDM_Quaternion& CGDM_Quaternion::fromAngleAxis(double angle, const CGDM_Vector3DF& axis)
{
	const double fHalfAngle = 0.5f*angle;
	const double fSin = sinf(fHalfAngle);
	m_fW = cosf(fHalfAngle);
	m_fX = fSin*axis.m_tX;
	m_fY = fSin*axis.m_tY;
	m_fZ = fSin*axis.m_tZ;
	return *this;
}


inline void CGDM_Quaternion::toAngleAxis(double &angle, CGDM_Vector3DF &axis) const
{
	const double scale = sqrtf(m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ);

	if (gdm_iszero(scale) || m_fW > 1.0f || m_fW < -1.0f)
	{
		angle = 0.0f;
		axis.m_tX = 0.0f;
		axis.m_tY = 1.0f;
		axis.m_tZ = 0.0f;
	}
	else
	{
		const double invscale = gdm_reciprocal(scale);
		angle = 2.0f * acosf(m_fW);
		axis.m_tX = m_fX * invscale;
		axis.m_tY = m_fY * invscale;
		axis.m_tZ = m_fZ * invscale;
	}
}

inline void CGDM_Quaternion::toEuler(CGDM_Vector3DF& euler) const
{
	const GDEM_DOUBLE sqw = m_fW*m_fW;
	const GDEM_DOUBLE sqx = m_fX*m_fX;
	const GDEM_DOUBLE sqy = m_fY*m_fY;
	const GDEM_DOUBLE sqz = m_fZ*m_fZ;

	// heading = rotation about z-axis
	euler.m_tZ = (double) (atan2(2.0 * (m_fX*m_fY +m_fZ*m_fW),(sqx - sqy - sqz + sqw)));

	// bank = rotation about x-axis
	euler.m_tX = (double) (atan2(2.0 * (m_fY*m_fZ +m_fX*m_fW),(-sqx - sqy + sqz + sqw)));

	// attitude = rotation about y-axis
	euler.m_tY = asin( gdm_clamp(-2.0 * (m_fX*m_fZ - m_fY*m_fW), -1.0, 1.0) );
}


inline CGDM_Vector3DF CGDM_Quaternion::operator* (const CGDM_Vector3DF& v) const
{
	// nVidia SDK implementation

	CGDM_Vector3DF uv, uuv;
	CGDM_Vector3DF qvec(m_fX, m_fY, m_fZ);
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * m_fW);
	uuv *= 2.0f;

	return v + uv + uuv;
}

// set CGDM_Quaternion to identity
inline CGDM_Quaternion& CGDM_Quaternion::makeIdentity()
{
	m_fW = 1.f;
	m_fX = 0.f;
	m_fY = 0.f;
	m_fZ = 0.f;
	return *this;
}

inline CGDM_Quaternion& CGDM_Quaternion::rotationFromTo(const CGDM_Vector3DF& from, const CGDM_Vector3DF& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	CGDM_Vector3DF v0 = from;
	CGDM_Vector3DF v1 = to;
	v0.normalize();
	v1.normalize();

	const double d = v0.dotProduct(v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return makeIdentity();
	}

	const double s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
	const double invs = 1.f / s;
	const CGDM_Vector3DF c = v0.crossProduct(v1)*invs;

	m_fX = c.m_tX;
	m_fY = c.m_tY;
	m_fZ = c.m_tZ;
	m_fW = s * 0.5f;

	return *this;
}

#endif // __GDEM_QUATERNION_H_INCLUDED__


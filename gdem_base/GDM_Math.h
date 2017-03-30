#ifndef __GDEM_MATH_H_INCLUDED__
#define __GDEM_MATH_H_INCLUDED__

#include "GDM_Define.h"
#include <math.h>
#include <stdlib.h> // for abs() etc.
#include "gdem_base_global.h"

#if defined(_GEO_SOLARIS_PLATFORM_) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__) || defined (_WIN32_WCE)
	#define sqrtf(X) (f32)sqrt((f64)(X))
	#define sinf(X) (f32)sin((f64)(X))
	#define cosf(X) (f32)cos((f64)(X))
	#define asinf(X) (f32)asin((f64)(X))
	#define acosf(X) (f32)acos((f64)(X))
	#define atan2f(X,Y) (f32)atan2((f64)(X),(f64)(Y))
	#define ceilf(X) (f32)ceil((f64)(X))
	#define floorf(X) (f32)floor((f64)(X))
	#define powf(X,Y) (f32)pow((f64)(X),(f64)(Y))
	#define fmodf(X,Y) (f32)fmod((f64)(X),(f64)(Y))
	#define fabsf(X) (f32)fabs((f64)(X))
#endif


#ifdef GDEM_FAST_MATH
	const double gdm_ROUNDING_ERROR_32 = 0.00005f;
	const GDEM_DOUBLE gdm_ROUNDING_ERROR_64 = 0.000005;
#else
	const double gdm_ROUNDING_ERROR_32 = 0.00000001; //0.000001f;
	const GDEM_DOUBLE gdm_ROUNDING_ERROR_64 = 0.00000001;
#endif

#ifndef REALINLINE
	#ifdef _MSC_VER
		#define REALINLINE __forceinline
	#else
		#define REALINLINE inline
	#endif
#endif

	//! Constant for PI.
	const double gdm_PI		= 3.1415926535897932384626433832795028841971693993751;

	const double gdm_TwoPI		= (2 * gdm_PI);

	//! Constant for reciprocal of PI.
	const double gdm_RECIPROCAL_PI	= 0.31830988618379067153776752674508; //1.0/gdm_PI;

	//! Constant for half of PI.
	const double gdm_HALF_PI	= gdm_PI/2.0f;

	//! Constant for 64bit PI.
	const GDEM_DOUBLE gdm_PI64		= gdm_PI;

	//! Constant for 64bit reciprocal of PI.
	const GDEM_DOUBLE gdm_RECIPROCAL_PI64 = gdm_RECIPROCAL_PI;

	//! 64bit Constant for converting from degrees to radians
	const double gdm_DEGTORAD = 0.017453292519943295769236907684883; // gdm_PI / 180.0f;

	//! 64bit constant for converting from radians to degrees (formally known as GRAD_PI)
	const double gdm_RADTODEG   = 57.295779513082320876798154814114; //180.0f / gdm_PI;

	//! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
	const GDEM_DOUBLE gdm_DEGTORAD64 = gdm_DEGTORAD;

	//! 64bit constant for converting from radians to degrees
	const GDEM_DOUBLE gdm_RADTODEG64 = gdm_RADTODEG;



	//! Utility function to convert a radian value to degrees
	/** Provided as it can be clearer to write radToDeg(X) than RADTODEG * X
	\param radians	The radians value to convert to degrees.
	*/
	inline double gdm_radToDeg(double radians)
	{
		return gdm_RADTODEG * radians;
	}

	////! Utility function to convert a radian value to degrees
	///** Provided as it can be clearer to write radToDeg(X) than RADTODEG * X
	//\param radians	The radians value to convert to degrees.
	//*/
	//inline GDEM_DOUBLE gdm_radToDeg(GDEM_DOUBLE radians)
	//{
	//	return gdm_RADTODEG64 * radians;
	//}

	//! Utility function to convert a degrees value to radians
	/** Provided as it can be clearer to write degToRad(X) than DEGTORAD * X
	\param degrees	The degrees value to convert to radians.
	*/
	inline double gdm_degToRad(double degrees)
	{
		return gdm_DEGTORAD * degrees;
	}

	////! Utility function to convert a degrees value to radians
	///** Provided as it can be clearer to write degToRad(X) than DEGTORAD * X
	//\param degrees	The degrees value to convert to radians.
	//*/
	//inline GDEM_DOUBLE gdm_degToRad(GDEM_DOUBLE degrees)
	//{
	//	return gdm_DEGTORAD64 * degrees;
	//}



	//! returns minimum of two values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline const T& gdm_min_(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	//! returns minimum of three values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline const T& gdm_min_(const T& a, const T& b, const T& c)
	{
		return a < b ? min_(a, c) : min_(b, c);
	}

	//! returns maximum of two values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline const T& gdm_max_(const T& a, const T& b)
	{
		return a < b ? b : a;
	}

	//! returns maximum of three values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline const T& gdm_max_(const T& a, const T& b, const T& c)
	{
		return a < b ? gdm_max_(b, c) : max_(a, c);
	}

	//! returns abs of two values. Own implementation to get rid of STL (VS6 problems)
	template<class T>
	inline T gdm_abs_(const T& a)
	{
		return a < (T)0 ? -a : a;
	}

	//! returns linear interpolation of a and b with ratio t
	//! \return: a if t==0, b if t==1, and the linear interpolation else
	template<class T>
	inline T gdm_lerp(const T& a, const T& b, const double t)
	{
		return (T)(a*(1.f-t)) + (b*t);
	}

	//! clamps a value between low and high
	template <class T>
	inline const T gdm_clamp (const T& value, const T& low, const T& high)
	{
		return gdm_min_ (gdm_max_(value,low), high);
	}


	REALINLINE double gdm_reciprocal_squareroot(const double x)
	{
#ifdef GEOENGINE_FAST_MATH
		// comes from Nvidia
#if 1
		GDEM_UINT tmp = (GDEM_UINT(IEEE_1_0 << 1) + IEEE_1_0 - *(GDEM_UINT*)&x) >> 1;
		double y = *(double*)&tmp;
		return y * (1.47f - 0.47f * x * y * y);
#elif defined(_MSC_VER)
		// an sse2 version
		__asm
		{
			movss	xmm0, x
			rsqrtss	xmm0, xmm0
			movss	x, xmm0
		}
		return x;
#endif
#else // no fast math
		return 1.f / sqrt ( x );
#endif
	}



	REALINLINE double gdm_reciprocal ( const double f )
	{
#ifdef GEOENGINE_FAST_MATH
		//! i do not divide through 0.. (fpu expection)
		// instead set f to a high value to get a return value near zero..
		// -1000000000000.f.. is use minus to stay negative..
		// must test's here (plane.normal dot anything ) checks on <= 0.f
		return 1.f / f;
		//GDEM_UINT x = (-(AIR(f) != 0 ) >> 31 ) & ( IR(f) ^ 0xd368d4a5 ) ^ 0xd368d4a5;
		//return 1.f / FR ( x );
#else // no fast math
		return 1.f / f;
#endif
	}

	//! returns if a equals b, taking possible rounding errors into account
        inline GDEM_BASE_EXPORT bool gdm_equals(const float a, const float b, const float tolerance = gdm_ROUNDING_ERROR_64)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals b, taking possible rounding errors into account
        inline GDEM_BASE_EXPORT bool gdm_equals(const double a, const double b, const double tolerance = gdm_ROUNDING_ERROR_32)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals b, taking possible rounding errors into account
        inline GDEM_BASE_EXPORT bool gdm_equals(const GDEM_INT a, const GDEM_INT b, const GDEM_INT tolerance = 0)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals b, taking possible rounding errors into account
        inline GDEM_BASE_EXPORT bool gdm_equals(const GDEM_UINT a, const GDEM_UINT b, const GDEM_UINT tolerance = 0)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

        inline GDEM_BASE_EXPORT bool gdm_equals(const GDEM_UINT32 a, const double b, const GDEM_UINT tolerance = 0)
        {
                return (a + tolerance >= b) && (a - tolerance <= b);
        }

	//! returns if a equals zero, taking rounding errors into account
	inline bool gdm_iszero(const double a, const double tolerance = gdm_ROUNDING_ERROR_32)
	{
		return fabsf ( a ) <= tolerance;
	}

	//! returns if a equals zero, taking rounding errors into account
	inline bool gdm_iszero(const GDEM_INT a, const GDEM_INT tolerance = 0)
	{
		return ( a & 0x7ffffff ) <= tolerance;
	}

	//! returns if a equals zero, taking rounding errors into account
	inline bool gdm_iszero(const GDEM_UINT a, const GDEM_UINT tolerance = 0)
	{
		return a <= tolerance;
	}

	inline GDEM_INT gdm_s32_min ( GDEM_INT a, GDEM_INT b)
	{
		GDEM_INT mask = (a - b) >> 31;
		return (a & mask) | (b & ~mask);
	}

	inline GDEM_INT gdm_s32_max ( GDEM_INT a, GDEM_INT b)
	{
		GDEM_INT mask = (a - b) >> 31;
		return (b & mask) | (a & ~mask);
	}

	inline GDEM_INT gdm_s32_clamp (GDEM_INT value, GDEM_INT low, GDEM_INT high)
	{
		return gdm_s32_min (gdm_s32_max(value,low), high);
	}

	inline double gdm_f32_max3(const double a, const double b, const double c)
	{
		return a > b ? (a > c ? a : c) : (b > c ? b : c);
	}

	inline double gdm_f32_min3(const double a, const double b, const double c)
	{
		return a < b ? (a < c ? a : c) : (b < c ? b : c);
	}

	inline double gdm_fract ( double x )
	{
		return x - floorf ( x );
	}


	#define F_32_VALUE_0		0x00000000
	#define F_32_VALUE_1		0x3f800000
	#define	F_32_LOWER_EQUAL_0(n)	((n) <= 0.0f)
        typedef union { unsigned int u; int s; double f; } _inttofloat;
        inline unsigned int _IR(double x) {_inttofloat tmp; tmp.f=x; return tmp.u;}
#endif // __GDEM_MATH_H_INCLUDED__

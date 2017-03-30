#ifndef __GDM_FLIGHT_SIM_TYPES_H__
#define __GDM_FLIGHT_SIM_TYPES_H__

#include "GDM_Vector3D.h"
#include "GDM_Coord3D.h"

// typedefs
typedef	double			FS_Kilogram;
typedef	double			FS_NewtonMag;
typedef	double			FS_Meter;
typedef	double			FS_Millisecond;
typedef	double			FS_Speed;
typedef	double			FS_SpeedKPH;

typedef	float			FS_Degree;
typedef	double			FS_Radian;

typedef	double			FS_Factor;
typedef	double			FS_Fraction;	// [-1.0 ~ +1.0]

typedef	CGDM_Vector3DF	FS_Vector3;
typedef	FS_Vector3		FS_Poistion;
typedef FS_Vector3		FS_Velocity;
typedef FS_Vector3		FS_Acceleration;
typedef FS_Vector3		FS_Newton;
typedef FS_Vector3		FS_Torque;
typedef CGDM_Matrix4	FS_Tensor;

typedef	GDM_Coord3D		FS_Coord;

// defines
#define FS_DEG2RAD			gdm_DEGTORAD
#define FS_RAD2DEG			gdm_RADTODEG

// constants
const	FS_Speed		FS_SPEED_MINIMAL		= 0.01f; // m/s
const	FS_Fraction		FS_MAX_CS_FRACTION		= 1.0f; // max fraction for Control Surface.
const	FS_Millisecond	FS_MIN_SIMULATE_TIME	= 0.0f;	// minimum time interval of simulation in milliseconds.
const	FS_Millisecond	FS_COLLID_DELAY_TIME	= 2000.0f;	// delay time in milliseconds when collision occurs.
const	FS_Radian		FS_ZERO_RADIAN			= 0.0000f;	
const	int				FS_THRUST_DIR			= -1;
const	int				FS_LIFT_DIR				= 1;
const	FS_Radian		FS_RAD_90				= 90 * gdm_DEGTORAD;
const	double			FS_COLLISION_MIN_HEIGHT	= 1;	// meter
const	double			FS_INIT_MAX_HEIGHT		= 10000; // meter
const	double			FS_INIT_MIN_HEIGHT		= 100; // meter
const	double			FS_INIT_RESTART_HEIGHT	= 1500; // meter
const	FS_NewtonMag	FS_GRAVITY_ACCELERATION_SCALAR	= 9.81f;


// enums
enum	FS_AXIS
{
	eFS_AXIS_PITCH,		eFS_AXIS_X = eFS_AXIS_PITCH,	
	eFS_AXIS_YAW,		eFS_AXIS_Y = eFS_AXIS_YAW,		eFS_AXIS_LIFT   = eFS_AXIS_YAW,
	eFS_AXIS_ROLL,		eFS_AXIS_Z = eFS_AXIS_ROLL,		eFS_AXIS_THRUST = eFS_AXIS_ROLL
};

// functions
template<typename T> inline
T FS_Clamp( T minVal, T maxVal, T val )
{
	if ( val < minVal )
		return minVal;
	if ( val > maxVal )
		return maxVal;
	return val;
}


template<typename T> inline FS_Fraction
FS_FractionInRange( T n, T low, T high )
{
	// Negative range undefined.
	if ( n <= low )
	{
		return 0.0f;
	}
	else if ( n >= high )
	{
		return 1.0f;
	}
	else  // inside range
	{
		return (n - low) / (high - low);
	}
}

template<typename T> inline
T FS_Abs( T val )
{
	return val > 0.0f? val : -val;
}

template<typename T> inline
T FS_Min( T val1, T val2 )
{
	return val1 > val2 ? val2 : val1;
}

template<typename T> inline
T FS_Max( T val1, T val2 )
{
	return val1 > val2 ? val1 : val2;
}

inline FS_SpeedKPH
FS_Speed2KPH( const FS_Speed speed )
{
	FS_Speed  meterPerHour = speed * 3600.0f;
	FS_SpeedKPH kilometerPerHour = meterPerHour / 1000.f;
	return kilometerPerHour;
}

inline FS_Speed
FS_KPH2Speed( const FS_SpeedKPH kph )
{
	FS_Speed meterPerHour   = kph * 1000.0f;
	FS_Speed meterPerSecond = meterPerHour / 3600.0f;
	return meterPerSecond;
}

inline bool
FS_IsZeroRadian( FS_Radian rad )
{
	return rad < FS_ZERO_RADIAN && rad > -FS_ZERO_RADIAN;
}

inline double
FS_DotProduct( FS_Vector3& vec1, FS_Vector3& vec2 )
{
	return vec1.dotProduct(vec2);
}

inline FS_Vector3
FS_CrossProduct( FS_Vector3& vec1, FS_Vector3& vec2 )
{
	return vec1.crossProduct(vec2);
}

inline FS_Radian
FS_GetAngleBetweenNormalVectors( FS_Vector3& vec1, FS_Vector3& vec2 )
{
	return acos( FS_DotProduct( vec1, vec2 ) );
}

inline void
FS_Normalize( FS_Vector3& vec )
{
	vec.normalize();
}
#endif
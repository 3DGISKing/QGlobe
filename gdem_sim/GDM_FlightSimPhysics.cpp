#include "GDM_FlightSim.h"

//#define _FS_ENABLE_DEBUG

FS_Factor
CGDM_FlightSim::ComputeRotationFactor( void )
{

	FS_SpeedKPH speed     = FS_Speed2KPH( m_Speed );
	FS_SpeedKPH speedLow  = 0;
	FS_SpeedKPH speedHigh = 60;

	if ( speed < 10 )
	{
		return 0.0f; 
	}
	else if ( speed >= speedHigh )
	{
		return 1.0f;
	}
	else
	{
		FS_Factor rotationFactor = FS_FractionInRange( speed, speedLow, speedHigh );
		return rotationFactor;
	}
}

void
CGDM_FlightSim::RotateAircraft( FS_Millisecond a_Milliseconds )
{
	FS_Fraction aileronFraction, elevatorFraction, rudderFraction;
	FS_Radian	rollRad, pitchRad, yawRad, radFactor;

	// Get fractions of aileron, elevator, rudder on control surface
	aileronFraction		= m_ControlSurface.GetAileron() * (-1.0f);
	elevatorFraction	= m_ControlSurface.GetElevator() * (-1.0f);
	rudderFraction		= m_ControlSurface.GetRudder() * (-1.0f);

	// Compute rotation factor from speed, milliseconds, rad
	radFactor =  ComputeRotationFactor(); 
	radFactor *= a_Milliseconds / 1000.0f;	// angular velocity is degree/sec
	radFactor *= FS_DEG2RAD;

	// Compute rotation radian angles of roll, pitch, yaw.
	// It is a function of fraction of control surface, 
	// angle velocity of spec  and radian factor 
	rollRad  = aileronFraction  * m_AircraftSpecs.m_RollRate  * radFactor;
	pitchRad = elevatorFraction * m_AircraftSpecs.m_PitchRate * radFactor;
	yawRad   = rudderFraction   * m_AircraftSpecs.m_YawRate   * radFactor;

	// Rotate aircraft coordinate system
	if ( !FS_IsZeroRadian( rollRad ) )
		RotateAircraft( eFS_AXIS_ROLL, rollRad );

	if ( !FS_IsZeroRadian( pitchRad ) )
		RotateAircraft( eFS_AXIS_PITCH, pitchRad );

	if ( !FS_IsZeroRadian( yawRad ) )
			RotateAircraft( eFS_AXIS_YAW, yawRad );

	FS_Factor factor = 0.9f;
	if ( m_LastInputDevice == eFS_INPUT_DEVICE_KEYBOARD )
	{
		m_ControlSurface.SetAileronFactor( factor );
		m_ControlSurface.SetElevatorFactor( factor );
	}
	m_ControlSurface.SetRudderFactor( factor );
}

void
CGDM_FlightSim::ComuteTotalForce()
{
	FS_Newton		thrustForce, dragForce, liftForce, weightForce;
	FS_NewtonMag	thrustForceMag, dragForceMag, liftForceMag, weightForceMag;
	FS_Factor		thrustFactor, dragFactor, liftFactor, weightFactor, densityFctor;
	FS_Factor		liftFactorSpeed, liftFactorAoA;

	FS_SpeedKPH		speedKph		= FS_Speed2KPH( m_Speed );
	bool			hasVel			= HasVelocity();
	FS_Degree		angleOfAttack	= ComputeAngleOfAttack( hasVel );

	// Density factor
	densityFctor	= ComputeAltitudeFromSeaLevel() / 20000;
	densityFctor	= FS_Clamp<FS_Factor>( 0.0f, 1.0f, densityFctor );
	densityFctor	= 1.0f - densityFctor;
	//
	//	Compute thrust force
	//
	thrustFactor	= m_ControlSurface.GetThrust();
	thrustFactor	*= densityFctor;
	thrustForceMag	= m_AircraftSpecs.m_MaxThrustMag;
	thrustForce		= m_AircraftAxis[eFS_AXIS_THRUST] * FS_THRUST_DIR;
	thrustForce		*= (thrustForceMag * thrustFactor);

	//
	// Compute drag force
	//
	if ( hasVel )
	{
		// speed
		dragFactor		= speedKph / m_AircraftSpecs.m_MaxSpeed;
		// AoA
		dragFactor		+= FS_Min<FS_Factor>( m_AircraftSpecs.m_DragAofALimit, 
											FS_Abs(angleOfAttack) / m_AircraftSpecs.m_DragAofADegree );
		dragFactor		*= densityFctor;
		// max thrust
		dragForceMag	= m_AircraftSpecs.m_MaxThrustMag ;
		// drag force direction is opposition to velocity
		dragForce		= -m_Velocity;	FS_Normalize( dragForce );
		dragForce		*= (dragForceMag * dragFactor);
	}

	//
	// Compute lift force
	//
	if ( hasVel )
	{
		// speed factor
		liftFactorSpeed	= FS_Min<FS_Factor>( 1.5f, speedKph / m_AircraftSpecs.m_LiftoffSpeed );
		liftFactorSpeed = FS_Clamp( -m_AircraftSpecs.m_RangeOfLiftFactorSpeed,
									 m_AircraftSpecs.m_RangeOfLiftFactorSpeed,
									 liftFactorSpeed );
		// AoA factor
		liftFactorAoA	= angleOfAttack / m_AircraftSpecs.m_MaxAofA;
		liftFactorAoA	= FS_Clamp( -m_AircraftSpecs.m_RangeOfLiftFactorAofA,
									 m_AircraftSpecs.m_RangeOfLiftFactorAofA,
									 liftFactorAoA );
		// combine
		liftFactor		= liftFactorSpeed * ( 0.7f - 2.0f / m_AircraftSpecs.m_LiftoffSpeed + liftFactorAoA);
		liftFactor		= FS_Clamp( -m_AircraftSpecs.m_RangeOfLiftFactor,
									 m_AircraftSpecs.m_RangeOfLiftFactor,
									 liftFactor );
		liftFactor		*= densityFctor;
		// weight 
		liftForceMag	= m_AircraftSpecs.m_Mass * FS_GRAVITY_ACCELERATION_SCALAR;
		// lift force
		liftForce		= m_AircraftAxis[eFS_AXIS_LIFT];
		liftForce		*= (liftForceMag * liftFactor);
	}

	//
	// Compute weight
	//
	weightFactor	= 1.0f;
	weightForceMag	= liftForceMag;
	weightForce		= ComputeGravityDir();
	weightForce		*= (weightForceMag * weightFactor );

	// Compute acceleration
	m_TotalForce		= thrustForce + dragForce + liftForce + weightForce;

#ifdef _FS_ENABLE_DEBUG	// debug
	GDM_Coord3D coord;
	coord.m_x = m_AircraftAxis[eFS_AXIS_X];
	coord.m_y = m_AircraftAxis[eFS_AXIS_Y];
	coord.m_z = m_AircraftAxis[eFS_AXIS_Z];
	gdm_VectorSCStoTCS( &coord, &thrustForce );
	gdm_VectorSCStoTCS( &coord, &dragForce );
	gdm_VectorSCStoTCS( &coord, &liftForce );
	gdm_VectorSCStoTCS( &coord, &weightForce );
	qDebug("------>Flight Sim : thrust[%f,%f,%f] drag[%f,%f,%f] lift[%f,%f,%f] weight[%f,%f,%f]",
		thrustForce.m_tX, thrustForce.m_tY, thrustForce.m_tZ,
		dragForce.m_tX, dragForce.m_tY, dragForce.m_tZ,
		liftForce.m_tX, liftForce.m_tY, liftForce.m_tZ,
		weightForce.m_tX, weightForce.m_tY, weightForce.m_tZ
		);
#endif
}

void
CGDM_FlightSim::ApplyForce( FS_Millisecond a_Milliseconds )
{
	FS_Acceleration	accel;
	FS_Torque		torque; 
	FS_Vector3		cg, accelAngle, angle;
	FS_Radian		angleMag, angleMax;
	FS_Factor		timeFactor		= a_Milliseconds / 1000.0f;

	accel			= m_TotalForce / m_AircraftSpecs.m_Mass;

	//
	// Compute Velocity and position
	//
	m_Velocity		+= (accel * timeFactor);

	// Compute additional velocity and add it to velocity
	// to prevent get away too far
	FS_Meter	speed = m_Velocity.getLength();
	if ( speed > 1 )
	{
		FS_Velocity	additionalVel;
		additionalVel	= m_AircraftAxis[eFS_AXIS_THRUST] * FS_THRUST_DIR;
		additionalVel	*= speed;
		additionalVel	-= m_Velocity;
		additionalVel.setLength( additionalVel.getLength() / 20 );
		m_Velocity		+= additionalVel;
	}

	// Change position
	m_AircraftPosition	+= (m_Velocity * (timeFactor * m_simulationSpeed));
	m_Speed			= m_Velocity.getLength();

	//
	// Compute rotating angle
	//
	// Compute torque
	cg = m_AircraftAxis[eFS_AXIS_THRUST] * FS_THRUST_DIR * m_AircraftSpecs.m_Length * 0.25;
	torque = FS_CrossProduct( cg, m_TotalForce );

	// compute angle accelerator and angle from torque
	GDM_Coord3D coord;
	coord.m_x = m_AircraftAxis[eFS_AXIS_X];
	coord.m_y = m_AircraftAxis[eFS_AXIS_Y];
	coord.m_z = m_AircraftAxis[eFS_AXIS_Z];

	gdm_SCStoTCS( &coord, &torque );
	accelAngle.m_tX = torque.m_tX / m_InertiaTensor(0,0);
	accelAngle.m_tY = torque.m_tY / m_InertiaTensor(1,1);
	accelAngle.m_tZ = torque.m_tZ / m_InertiaTensor(2,2);
	gdm_TCStoSCS( &coord, &accelAngle );

	angle = 0.5f * accelAngle * timeFactor * timeFactor;

	// rotate the coordinate system
	angleMax = m_AircraftSpecs.m_RotateRate * FS_DEG2RAD * timeFactor;
	angleMag = angle.getLength() * 1.0 ;
	//if ( angleMag > 0.1 * FS_DEG2RAD )
	{
		angleMag = FS_Clamp( -angleMax, angleMax, angleMag );

		gdm_RotateAroundVector( &coord, angleMag, angle );
	}

	m_AircraftAxis[eFS_AXIS_X] = coord.m_x;
	m_AircraftAxis[eFS_AXIS_Y] = coord.m_y;
	m_AircraftAxis[eFS_AXIS_Z] = coord.m_z;
#ifdef _FS_ENABLE_DEBUG	// debug
	FS_Vector3 forceV, accelV, velocityV, angleV;
	forceV		= m_TotalForce;
	accelV		= accel;
	velocityV	= m_Velocity;
	angleV		= angle;
	gdm_VectorSCStoTCS( &coord, &forceV );
	gdm_VectorSCStoTCS( &coord, &accelV );
	gdm_VectorSCStoTCS( &coord, &velocityV );
	gdm_VectorSCStoTCS( &coord, &angleV );
	qDebug("------>Flight Sim : Force[%f,%f,%f] Accel[%f,%f,%f] Velocity[%f,%f,%f] Angle[%f,%f,%f : %f]",
		forceV.m_tX, forceV.m_tY, forceV.m_tZ,
		accelV.m_tX, accelV.m_tY, accelV.m_tZ,
		velocityV.m_tX, velocityV.m_tY, velocityV.m_tZ,
		angleV.m_tX, angleV.m_tY, angleV.m_tZ, angleMag * FS_RAD2DEG
		);
#endif

}

bool
CGDM_FlightSim::HasVelocity( void )
{
	return FS_Speed2KPH( m_Speed ) > FS_SPEED_MINIMAL;
}

FS_Degree
CGDM_FlightSim::ComputeAngleOfAttack( bool a_hasVelocity )
{
	if ( a_hasVelocity )
	{
		FS_Vector3 normVel = m_Velocity;
		FS_Normalize( normVel );
		FS_Radian rad = FS_GetAngleBetweenNormalVectors( m_AircraftAxis[eFS_AXIS_Y], normVel );
		return (rad - FS_RAD_90) * FS_RAD2DEG;
	}
	else
		return 0.0f;
}

void
CGDM_FlightSim::HandleCollision()
{
	FS_Meter	aircraftAltitude = 0, demHeight = 0, seaLevelHeight = 0;
	ComputeAircraftHeights( & seaLevelHeight, &demHeight, &aircraftAltitude );
	if ( aircraftAltitude <= FS_COLLISION_MIN_HEIGHT )
	{ 
		m_AircraftPosition.setLength( demHeight + 2 + EARTH_RADIUS );
		m_Collided = true;
	}
}

void
CGDM_FlightSim::HandleStall()
{
	FS_Degree	aoa = ComputeAngleOfAttack( HasVelocity() );
	FS_Factor	stallFaction =  aoa  / m_AircraftSpecs.m_MaxAofA;
	if ( stallFaction > 1.0f || stallFaction < -1.0f)
	{
		m_Stall	= true;
// 		FS_Radian	pitch, roll, yaw;
// 		double	randFactor = (double) rand() / (double) RAND_MAX * 0.35f + 0.1f;
// 
// 		stallFaction	= sin( stallFaction * FS_RAD_90 );
// 		stallFaction	*= randFactor;
// 		stallFaction	*= FS_DEG2RAD;
// 		pitch	= stallFaction * 3.0f;	
// 		roll	= stallFaction * 0.2f;	
// 		yaw		= stallFaction * 0.1f;	
// 		//RotateAircraft( eFS_AXIS_ROLL,	-roll );
// 		RotateAircraft( eFS_AXIS_PITCH, -pitch );
// 		//RotateAircraft( eFS_AXIS_YAW,	yaw );
	}
	else
		m_Stall	= false;

}


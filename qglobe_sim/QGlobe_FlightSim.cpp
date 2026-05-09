#include "QGlobe_FlightSim.h"

#define FS_INIT_CAMERA_HEIGHT(x)			((x) + FS_INIT_MAX_HEIGHT + EARTH_RADIUS)
#define FS_INIT_CAMERA_RESTART_HEIGHT(x)	((x) + FS_INIT_RESTART_HEIGHT + EARTH_RADIUS)

CQGlobe_FlightSim::CQGlobe_FlightSim(CQGlobe_SimMgr* a_pSimMgr) : CQGlobe_Simulator(a_pSimMgr)
{
	m_simulationSpeed	= 1.0f;
}

CQGlobe_FlightSim::~CQGlobe_FlightSim()
{

}

void	
CQGlobe_FlightSim::InitSimulation()
{
	// aircraft specs
	m_AircraftSpecs.ChooseAircraftType( eFS_AIRCRAFT_TYPE_A4 );

	// control surface 
	m_ControlSurface.Reset();
	m_ControlSurface.SetThrust( 0.5f );

	// Three axises of aircraft
	QGlobe_Coord3D coord;
	m_pCamera->GetCameraCoord( &coord );

	m_AircraftAxis[eFS_AXIS_X] = coord.m_x;
	m_AircraftAxis[eFS_AXIS_Y] = coord.m_y;
	m_AircraftAxis[eFS_AXIS_Z] = coord.m_z;

	// position of aircraft
	m_AircraftPosition.set( coord.m_org.m_tX, coord.m_org.m_tY, coord.m_org.m_tZ );
	double	altitude = 0, demHeight = 0, sealevel = 0;
	ComputeAircraftHeights( &sealevel, &demHeight, &altitude );
	if ( altitude > FS_INIT_MAX_HEIGHT )
		m_AircraftPosition.setLength( FS_INIT_CAMERA_HEIGHT(demHeight) );
	else if ( altitude < FS_INIT_MIN_HEIGHT )
		m_AircraftPosition.setLength( FS_INIT_CAMERA_RESTART_HEIGHT(demHeight) );


	// Initial speed and velocity
	m_Speed		= FS_KPH2Speed( m_AircraftSpecs.m_LiftoffSpeed ) * 1.5f;
	m_Velocity	= m_AircraftAxis[eFS_AXIS_THRUST] * FS_THRUST_DIR;
	m_Velocity	*= m_Speed;

	// force and inertial tensor
	FS_Meter r = m_AircraftSpecs.m_Height * 0.5f;
	FS_Meter h = m_AircraftSpecs.m_Length;
	FS_NewtonMag m = m_AircraftSpecs.m_Mass;

	m_TotalForce.set(0,0,0);
	m_InertiaTensor(0, 0) = 0.25 * m * r * r + 1/12 * m * h * h;
	m_InertiaTensor(1, 1) = m_InertiaTensor(0, 0);
	m_InertiaTensor(2, 2) = 0.5 * m * r * r * 1.5;

	// others...
	m_PrevMilliseconds	= 0;
	m_LastInputDevice	= eFS_INPUT_DEVICE_KEYBOARD;

	// Flags
	m_UseMouse	= false;
	m_Paused	= false;
	m_Ended		= false;
	m_Collided	= false;
	m_Stall		= false;
	m_ShowHelp	= true;
}

void
CQGlobe_FlightSim::RestartSimulation()
{
	InitSimulation();
	QGlobe_LOCATION	location;
	QGlobe_Coord3D		coord;

	m_pCamera->GetLocation( &location );
	qglobe_GetStandardCoord( &location, &coord );
	qglobe_RotateAroundXAxis( &coord, qglobe_PI / 2 );

	m_AircraftAxis[eFS_AXIS_X] = coord.m_x;
	m_AircraftAxis[eFS_AXIS_Y] = coord.m_y;
	m_AircraftAxis[eFS_AXIS_Z] = coord.m_z;

}

bool	
CQGlobe_FlightSim::AdvanceSimulation(IN double a_dt)
{
	// if simulation is ended.
	if ( m_Ended )
		return true;

	// if simulation is paused.
	if ( m_Paused )
	{
		m_PrevMilliseconds = 0.0f;
		return true;
	}

	// if collision occurs.
	if ( m_Collided )
	{
		m_simStatus.m_collided = true;
		return true;
	}

	FS_Millisecond milliseconds = m_PrevMilliseconds + a_dt;

	// if not enough interval time.
	if ( milliseconds < FS_MIN_SIMULATE_TIME )
	{
		m_PrevMilliseconds = milliseconds;
		return true;
	}

	// simulation processing....
	m_PrevMilliseconds = 0.0f;

	RotateAircraft( milliseconds );

	ComuteTotalForce( );

	ApplyForce( milliseconds );

	HandleStall();

	HandleCollision();

	//if ( !m_Collided )
	{
		UpdateCamera();
		ComputeSimulationStatus();
	}

	return true;
}

void
CQGlobe_FlightSim::EndSimulation()
{
	if ( !m_Collided )
		return;

	QGlobe_LOCATION	location;
	QGlobe_Coord3D		standardCoord;
	double			altitude = 0, demHeight = 0, sealevel = 0;

	ComputeAircraftHeights( &sealevel, &demHeight, &altitude );

	m_pCamera->GetLocation( &location );
	qglobe_GetStandardCoord( &location, &standardCoord );
	qglobe_RotateAroundXAxis( &standardCoord, qglobe_PI / 2 );

	m_pCamera->SetCameraCoord( &standardCoord , true );
	standardCoord.m_org.setLength( FS_INIT_CAMERA_RESTART_HEIGHT(demHeight) );
}

void	
CQGlobe_FlightSim::GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus)
{
	// speed
	a_pSimStatus->m_speed			= m_simStatus.m_speed;

	// paused
	a_pSimStatus->m_paused			= m_Paused;

	// ended
	a_pSimStatus->m_ended			= m_Ended;

	// show help
	a_pSimStatus->m_showHelp		= m_ShowHelp;

	// vertical speed	
	a_pSimStatus->m_verticalspeed	= m_simStatus.m_verticalspeed;

	// altitude
	a_pSimStatus->m_altitude		= m_simStatus.m_altitude;

	// heading
	a_pSimStatus->m_heading			= m_simStatus.m_heading;

	// bank angle
	a_pSimStatus->m_bankangle		= m_simStatus.m_bankangle;

	// pitch 
	a_pSimStatus->m_pitchangle		= m_simStatus.m_pitchangle;

	// throttle
	a_pSimStatus->m_throttle		= m_simStatus.m_throttle;

	// aileron
	a_pSimStatus->m_aileron			= m_simStatus.m_aileron;

	// rudder
	a_pSimStatus->m_rudder			= m_simStatus.m_rudder;

	// elevator
	a_pSimStatus->m_elevator		= m_simStatus.m_elevator;

	// flap
	a_pSimStatus->m_flap			= m_simStatus.m_flap;

	// gear
	a_pSimStatus->m_gear			= m_simStatus.m_gear;

	// stall
	a_pSimStatus->m_stall			= m_simStatus.m_stall;

	// collided
	a_pSimStatus->m_collided		= m_Collided;

	// auto move
	a_pSimStatus->m_autoMove		= false;

	// use mouse
	a_pSimStatus->m_useMouse		= m_UseMouse;
}

void
CQGlobe_FlightSim::RotateAircraft( FS_AXIS a_Axis, FS_Radian a_Rad )
{
	QGlobe_Coord3D	coord;
	coord.m_x = m_AircraftAxis[eFS_AXIS_X];
	coord.m_y = m_AircraftAxis[eFS_AXIS_Y];
	coord.m_z = m_AircraftAxis[eFS_AXIS_Z];

	switch (a_Axis)
	{
	case eFS_AXIS_X:
		qglobe_RotateAroundXAxis( &coord, a_Rad );
		break;
	case eFS_AXIS_Y:
		qglobe_RotateAroundYAxis( &coord, a_Rad );
		break;
	case eFS_AXIS_Z:
		qglobe_RotateAroundZAxis( &coord, a_Rad );
		break;
	}
	m_AircraftAxis[eFS_AXIS_X] = coord.m_x;
	m_AircraftAxis[eFS_AXIS_Y] = coord.m_y;
	m_AircraftAxis[eFS_AXIS_Z] = coord.m_z;

}

FS_Vector3
CQGlobe_FlightSim::ComputeGravityDir( void )
{
	FS_Vector3 dir = -m_AircraftPosition;;
	FS_Normalize( dir );
	return dir;	
}

void
CQGlobe_FlightSim::UpdateCamera( void )
{
	QGlobe_Coord3D coord;
	coord.m_org.set( m_AircraftPosition.m_tX,
					 m_AircraftPosition.m_tY,
					 m_AircraftPosition.m_tZ );
	coord.m_x = m_AircraftAxis[eFS_AXIS_X];
	coord.m_y = m_AircraftAxis[eFS_AXIS_Y];
	coord.m_z = m_AircraftAxis[eFS_AXIS_Z];
	m_pCamera->SetCameraCoord( &coord, false );
}

CQGlobe_Vector3DF
GetRollPitchYawDegrees( CQGlobe_Matrix4& mat )
{
	double P = asin(mat(2,1));
	const double C = cos(P);
	P *= qglobe_RADTODEG64;

	double rotx, roty, R, Y;

	if (fabs(C)>qglobe_ROUNDING_ERROR_64)
	{
		double invC = 1.0 / C;
		rotx = mat(2,2) * invC;
		roty = mat(2,0) * invC;
		R = atan2( -roty, rotx ) * qglobe_RADTODEG64;
		rotx = mat(1,1) * invC;
		roty = mat(0,1) * invC;
		Y = atan2( -roty, rotx ) * qglobe_RADTODEG64;
	}
	else
	{
		Y = 0.0;
		rotx = mat(0,0);
		roty = -mat(1,0);
		R = atan2( roty, rotx ) * qglobe_RADTODEG64;
	}

// 	if (R < 0.0) R += 360.0;
// 	if (P < 0.0) P += 360.0;
// 	if (Y < 0.0) Y += 360.0;

	// Transform order of used formula is Yaw/Pitch/Roll
	// our order is Roll/Pitch/Yaw
	return CQGlobe_Vector3DF(Y,P,R);	
}

void
CQGlobe_FlightSim::ComputeSimulationStatus()
{
	FS_Radian	pitchRad, bankRad, headingRad;

	FS_Vector3	gravityDir = ComputeGravityDir();

	QGlobe_LOCATION	location;
	QGlobe_Coord3D		standardCoord, cameraCoord;
	CQGlobe_Vector3DF	vecAngle;

	m_pCamera->GetLocation( &location );
	qglobe_GetStandardCoord( &location, &standardCoord );
	m_pCamera->GetCameraCoord( &cameraCoord );
	// set org to zero
	standardCoord.m_org.set(0.0f, 0.0f, 0.0f);
	cameraCoord.m_org.set(0.0f, 0.0f, 0.0f);
	// z axis of initial standard coord is opposit direction
	qglobe_RotateAroundXAxis( &standardCoord, FS_RAD_90);

	
	CQGlobe_Matrix4	mat;
	// get transform matrix from scs to tcs of standard coordinate system.
	qglobe_MatrixSCStoTCS( &standardCoord, &mat );
	// transform camera coord to standard coord with transform matrix
	mat.transformVect( cameraCoord.m_x );	cameraCoord.m_x.normalize();
	mat.transformVect( cameraCoord.m_y );	cameraCoord.m_y.normalize();
	mat.transformVect( cameraCoord.m_z );	cameraCoord.m_z.normalize();
	// get transform matrix from scs of standard coordinate system to tss of camera
	qglobe_MatrixTCStoSCS( &cameraCoord, &mat );
	// get euler angles : returnVector(roll, pitch, yaw)
	vecAngle	= GetRollPitchYawDegrees( mat );
	pitchRad	= vecAngle.m_tY;
	bankRad		= vecAngle.m_tX;
	headingRad	= vecAngle.m_tZ;



	// speed
	m_simStatus.m_speed			= FS_Speed2KPH( m_Speed );

	// paused
	m_simStatus.m_paused		= m_Paused;

	// ended
	m_simStatus.m_ended			= m_Ended;

	// show help
	m_simStatus.m_showHelp		= m_ShowHelp;

	// vertical speed	
	m_simStatus.m_verticalspeed	= -FS_DotProduct( gravityDir, m_Velocity ) ;

	// altitude
	m_simStatus.m_altitude		= (int)(m_AircraftPosition.getLength() - EARTH_RADIUS);

	// heading
	m_simStatus.m_heading		= headingRad;

	// bank angle
	m_simStatus.m_bankangle		= bankRad ;

	// pitch 
	m_simStatus.m_pitchangle	= -pitchRad ;


	// throttle
	m_simStatus.m_throttle		= m_ControlSurface.GetThrust();

	// aileron
	m_simStatus.m_aileron		= m_ControlSurface.GetAileron();

	// rudder
	m_simStatus.m_rudder		= m_ControlSurface.GetRudder();

	// elevator
	m_simStatus.m_elevator		= m_ControlSurface.GetElevator();

	// flap
	m_simStatus.m_flap			= 0;

	// gear
	m_simStatus.m_gear			= false;

	// stall
	m_simStatus.m_stall			= m_Stall;

	// Use mouse
	m_simStatus.m_useMouse		= m_UseMouse;

	// collided
	m_simStatus.m_collided		= m_Collided;

}

void
CQGlobe_FlightSim::ComputeAircraftHeights( double* a_seaLevelHeight, double* a_demHeight, double* a_alititude )
{
	QGlobe_LOCATION	location;
	double			demHeight;
	double			altitude;

	qglobe_GetLocation( &m_AircraftPosition, &location );
 	demHeight	= qglobe_IntersectHeight( location.m_dLongitude, location.m_dLatitude );
 	if ( demHeight == 0 )
		demHeight = qglobe_getExactHeight( location.m_dLongitude, location.m_dLatitude );

	altitude	= location.m_dDist - demHeight;

	// According to altitude of camera, adjust the speed of simulation
	m_simulationSpeed = altitude / FS_INIT_MAX_HEIGHT * 2.0f + 2.0f;
	if ( m_simulationSpeed < 1.0f )
		m_simulationSpeed = 1.0f;
	if ( m_simulationSpeed > 4.0f )
		m_simulationSpeed = 4.0f;

	*a_seaLevelHeight	= location.m_dDist;
	*a_demHeight		= demHeight;
	*a_alititude		= altitude;
}

double
CQGlobe_FlightSim::ComputeAltitudeFromSeaLevel()
{
	QGlobe_LOCATION	location;
	double			demHeight;
	double			altitude;

	qglobe_GetLocation( &m_AircraftPosition, &location );
	return location.m_dDist;
}

void
CQGlobe_FlightSim::VectorWorld2Aircraft( FS_Vector3* a_vector )
{
	QGlobe_Coord3D coord;
	coord.m_x = m_AircraftAxis[eFS_AXIS_X];
	coord.m_y = m_AircraftAxis[eFS_AXIS_Y];
	coord.m_z = m_AircraftAxis[eFS_AXIS_Z];
	qglobe_VectorSCStoTCS( &coord, a_vector );
}

void
CQGlobe_FlightSim::VectorAircraft2World( FS_Vector3* a_vector )
{
	QGlobe_Coord3D coord;
	coord.m_x = m_AircraftAxis[eFS_AXIS_X];
	coord.m_y = m_AircraftAxis[eFS_AXIS_Y];
	coord.m_z = m_AircraftAxis[eFS_AXIS_Z];
	qglobe_VectorTCStoSCS( &coord, a_vector );
}
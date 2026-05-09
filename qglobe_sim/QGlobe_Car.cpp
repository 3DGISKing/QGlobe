
#include "QGlobe_Car.h"

#define CAR_HEIGHT					1

#define	CS_ACCEL_SPEED_STEP			1.0f
#define	CS_ACCEL_ANGLE_STEP			4.0f
#define	CS_ACCEL_CONVEGENCE_COUNT	10	

#define CS_APPROACH_TIME			2.0f 

const	double	zeroDistance		= 0.0001f;
const	double	minAngleSpeed		= 8;

//#define ENABLE_LOG
//#define ENABLE_DEBUG

CQGlobe_Car::CQGlobe_Car(void)
{

}

CQGlobe_Car::~CQGlobe_Car(void)
{
	if ( m_firstAdvance )
		ComputeStartPosition();
	else
		m_pCamera->SetCameraCoord( &m_cameraCoord, true );
}

void CQGlobe_Car::InitCar(IN CQGlobe_Camera* a_pCamera)
{
	m_speed			= 0;
	m_frictionFactor= 1.5f;
	m_dragFactor	= 0.1f;

	m_accelSpeed	= 0.0f;
	m_accelAngle	= 0.0f;
	m_maxAccelSpeed	= 10.0f;
	m_maxAccelAngle	= 80;

	m_maxSpeed		= 50;
	m_maxBackSpeed	= m_maxSpeed / 2;
	m_maxTurnSpeed	= m_maxSpeed / 2;

	m_travelTime	= 0;
	m_travelDistance = 0;
	m_directionAngle = 0;

	m_carMotion		= CAR_MOTION_NONE;
	m_carSteer		= CAR_STEER_MID;

	m_pCamera		= a_pCamera;

	m_firstAdvance	= true;
	m_secondAdvance	= true;
	m_approachInit	= true;
	m_approachSpeed	= 0;
	m_approachTime	= 0;
	
	m_logIndex		= 0;

	m_carHeight		= CAR_HEIGHT;

}

void CQGlobe_Car::SetCarMotion(IN QGlobe_CAR_MOTION carMotion)
{
	m_carMotion = carMotion;
	m_accelSpeed += (m_carMotion - 1) * CS_ACCEL_SPEED_STEP;
	m_accelSpeed = qglobe_clamp( m_accelSpeed , -m_maxAccelSpeed, m_maxAccelSpeed );
}

void CQGlobe_Car::SetCarSteer(IN QGlobe_CAR_STEER carSteer)
{
	m_carSteer = carSteer;
	m_accelAngle += (m_carSteer - 1) * CS_ACCEL_ANGLE_STEP;
	m_accelAngle = qglobe_clamp( m_accelAngle , -m_maxAccelAngle, m_maxAccelAngle );
}


void CQGlobe_Car::GetCarStatus(OUT QGlobe_CAR_STATUS* a_pCarStatus)
{
	a_pCarStatus->m_speed			= m_speed;
	a_pCarStatus->m_travelTime		= m_travelTime;
	a_pCarStatus->m_travelDistance	= m_travelDistance;
	a_pCarStatus->m_directionAngle	= m_directionAngle;
	a_pCarStatus->m_maxSpeed		= m_maxSpeed;
	a_pCarStatus->m_maxBackSpeed	= m_maxBackSpeed;
}

void
CQGlobe_Car::AdvanceCar( double a_dt )
{
	if ( m_firstAdvance )
	{
		ComputeStartPosition();
		m_firstAdvance = false;

		return;
	}
	else if ( m_secondAdvance )
	{
		if ( ApproachGround( a_dt ) )
		{
			m_secondAdvance = false;
		}

		return;
	}


	m_travelTime += a_dt;


	m_accelSpeed -= m_accelSpeed / CS_ACCEL_CONVEGENCE_COUNT;
	m_accelAngle -= m_accelAngle / CS_ACCEL_CONVEGENCE_COUNT;

	ComputeSpeed( a_dt );

	TurnCar( a_dt );

	m_travelDistance += MoveCar( a_dt );

	m_pCamera->SetCameraCoord( &m_cameraCoord, false );

	ComputeStatus();

	m_carMotion = CAR_MOTION_NONE;
	m_carSteer = CAR_STEER_MID;

}

void
CQGlobe_Car::TurnCar( double a_dt )
{
	double dOffset, angle;
	double speedFactor = 0.0f;

	speedFactor = m_speed / m_maxTurnSpeed;
	if ( speedFactor > 1.0f )
		speedFactor = 1.0f;
	else if ( speedFactor < -1.0f )
		speedFactor = -1.0f;
	
	angle = m_accelAngle * a_dt * speedFactor;
	dOffset = angle * qglobe_DEGTORAD;

	qglobe_RotateAroundYAxis( &m_cameraCoord, dOffset );

}
double 
CQGlobe_Car::MoveCar( double a_dt )
{
	double distance = m_speed * a_dt;
	
	if ( qglobe_abs_( distance ) < zeroDistance )
		return 0;

	CQGlobe_Vector3DF startPos, endPos, upVect, stepV;
	double endHeight = 0.0f;

	// Calculate start and end position
	startPos.set( m_cameraCoord.m_org.m_tX,
				m_cameraCoord.m_org.m_tY,
				m_cameraCoord.m_org.m_tZ );

	stepV		= m_cameraCoord.m_z * (-distance);
	endPos		= startPos + stepV;

	endHeight	= GetDEMHeight( endPos );

#ifdef ENABLE_LOG
	m_endHeightLogs[m_logIndex] = endHeight;
	endHeight = 0.0f;
	for ( int i = 0; i < LOG_NUM; i++ )
	{
		endHeight	+= m_endHeightLogs[i];
	}
	endHeight	/= LOG_NUM;
#endif
	endPos.setLength( endHeight + EARTH_RADIUS + m_carHeight );

	// Compute up vector
	ComputeUpVector( startPos, upVect );

	// Move camera coord
	MoveCameraCoord( startPos, endPos, upVect, a_dt );

#ifdef ENABLE_LOG
	// Increase log index
	m_logIndex	= (m_logIndex + 1) % LOG_NUM;
#endif

	return qglobe_abs_(distance);
}

void
CQGlobe_Car::MoveCameraCoord( CQGlobe_Vector3DF& a_startPos, CQGlobe_Vector3DF& a_endPos, CQGlobe_Vector3DF& a_upVector, double a_dt )
{
	CQGlobe_Vector3DF upV, forwardV;
	double angle, angleMin, angleMax, angleSpeed;
	
	// Compute climb angle between start and end positions
	angle = ComputeClimbAngle( a_startPos, a_endPos );

	// Adjust the climb angle to prevent to change sharply
	angleSpeed = minAngleSpeed;
	angleMax = angleSpeed * qglobe_DEGTORAD * a_dt;
	angleMin = (angleSpeed - 1.0f) * qglobe_DEGTORAD * a_dt;

	angle = qglobe_clamp( angle, -angleMax	, angleMax );

	if ( qglobe_abs_( angle ) < angleMin )
			angle = 0.0f;

#ifdef ENABLE_LOG
 	m_angleLogs[m_logIndex]	= angle;
 
 	angle = 0.0f;
 	for ( int i = 0; i < LOG_NUM; i++)
 		angle += m_angleLogs[i];
 	angle /= LOG_NUM;
#endif

	angle = m_speed > 0.0f ? angle : -angle;
	qglobe_RotateAroundXAxis( &m_cameraCoord, -angle );
#ifdef ENABLE_DEBUG
	qDebug("--->CarSim : Angle{%f]", angle);
#endif

	m_cameraCoord.m_org.set( a_endPos.m_tX, a_endPos.m_tY, a_endPos.m_tZ );
	m_cameraCoord.m_x = a_upVector.crossProduct( m_cameraCoord.m_z );	
	m_cameraCoord.m_y = m_cameraCoord.m_z.crossProduct( m_cameraCoord.m_x );

	m_cameraCoord.m_x.normalize();
	m_cameraCoord.m_y.normalize();
	m_cameraCoord.m_z.normalize();
}

void
CQGlobe_Car::ComputeStartPosition()
{	
	m_pCamera->GetCameraCoord( &m_cameraCoord );
	CQGlobe_Vector3DF pos;
	double h;
	pos.set( m_cameraCoord.m_org.m_tX, m_cameraCoord.m_org.m_tY, m_cameraCoord.m_org.m_tZ );
	h = GetDEMHeight( pos );
	for ( int i = 0; i < LOG_NUM; i++ )
	{
		m_endHeightLogs[i]		= h;
		m_angleLogs[i]			= 0.0f;
	}
	m_speed = 10.0f;
	for (int i = 0; i < LOG_NUM; i++)
	{
		//MoveCar( 0.1f );
	}
	m_speed = 0.0f;

	m_logIndex = 0;


}

void
CQGlobe_Car::ComputeUpVector( CQGlobe_Vector3DF& a_pos, CQGlobe_Vector3DF& a_upVector )
{
	a_upVector = a_pos;
	a_upVector.normalize();
}

void
CQGlobe_Car::GetDEMPosition( CQGlobe_Vector3DF& a_worldPos, CQGlobe_Vector3DF& a_demPos, double a_carHeight )
{
	QGlobe_LOCATION loc;
	double demHeight;

	qglobe_GetLocation( &a_worldPos, &loc );
	demHeight = qglobe_getExactHeight( loc.m_dLongitude, loc.m_dLatitude );
	a_demPos = a_worldPos;
	a_demPos.setLength( demHeight + EARTH_RADIUS + a_carHeight );
}

double
CQGlobe_Car::GetDEMHeight( CQGlobe_Vector3DF& a_worldPos )
{
	QGlobe_LOCATION loc;
	double height = 0.0f;
	qglobe_GetLocation( &a_worldPos, &loc );
	height = qglobe_IntersectHeight( loc.m_dLongitude, loc.m_dLatitude );
	if ( height == 0)
		height = qglobe_getExactHeight( loc.m_dLongitude, loc.m_dLatitude );

#ifdef ENABLE_DEBUG
	qDebug("--->CarSim : Height[%f], Lon[%2.16f], Lat[%2.16f]", height, loc.m_dLongitude, loc.m_dLatitude );
#endif
	return height;
}

void
CQGlobe_Car::ComputeSpeed( double a_dt )
{
	double accelFactor, resistFactor;

	accelFactor  = m_accelSpeed;
	resistFactor = m_frictionFactor * m_dragFactor * m_speed;

	m_speed += (accelFactor - resistFactor ) * a_dt;
#if 0
	CQGlobe_Vector3DF posV;
	double gravityFactor;
	posV = m_cameraCoord.m_org;
	posV.normalize();
	gravityFactor = acos( posV.dotProduct( m_cameraCoord.m_z ));
	gravityFactor *= 0.1;
	m_speed += gravityFactor * a_dt;
#endif	

	m_speed =qglobe_clamp( m_speed, -m_maxBackSpeed, m_maxSpeed );

#ifdef ENABLE_DEBUG
	qDebug("--->CarSim : speed[%f]", m_speed);
#endif
}
void
CQGlobe_Car::ComputeStatus()
{
	m_directionAngle = 0.0f;
	QGlobe_CAMERA_INFO	cameraInfo;
	m_pCamera->GetInfo( &cameraInfo );
	m_directionAngle = -cameraInfo.m_roll * qglobe_RADTODEG;
}

double
CQGlobe_Car::ComputeClimbAngle( CQGlobe_Vector3DF& a_startPos, CQGlobe_Vector3DF& a_endPos )
{
	CQGlobe_Vector3DF upV, forwardV;
	double angle;
	upV = m_cameraCoord.m_y;
	forwardV = a_endPos - a_startPos;
	forwardV.normalize();

	angle = acos( forwardV.dotProduct( upV ));
	angle -= (qglobe_DEGTORAD * 90);

	return angle;
}

bool
CQGlobe_Car::ApproachGround( double a_dt )
{	
	CQGlobe_Vector3DF cameraPos;
	double cameraHeight, targetHeight, offset;	

	cameraPos.set( m_cameraCoord.m_org.m_tX,
					m_cameraCoord.m_org.m_tY,
					m_cameraCoord.m_org.m_tZ );

	cameraHeight = cameraPos.getLength();
	targetHeight = GetDEMHeight( cameraPos ) + EARTH_RADIUS + m_carHeight;

	if ( m_approachInit )
	{
		m_approachSpeed = ( cameraHeight - targetHeight ) / CS_APPROACH_TIME;
		m_approachInit = false;
	}
	
	m_approachTime += a_dt;

	offset = cameraHeight - targetHeight;	

	if ( offset > m_approachSpeed * a_dt )
		cameraPos.setLength( cameraHeight - m_approachSpeed * a_dt );
	else
		cameraPos.setLength( targetHeight );

	m_cameraCoord.m_org.set( cameraPos.m_tX, cameraPos.m_tY, cameraPos.m_tZ );

	m_pCamera->SetCameraCoord( &m_cameraCoord, false );

	if ( m_approachTime > CS_APPROACH_TIME || offset <= 0 )
	{		
		return true;
	}

	return false;
}
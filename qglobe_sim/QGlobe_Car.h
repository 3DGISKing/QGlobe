#ifndef __QGlobe_CAR_H__
#define __QGlobe_CAR_H__

#include "QGlobe_Camera.h"

#define	LOG_NUM				1

enum QGlobe_CAR_MOTION
{
	CAR_MOTION_BRAKE	= 0,
	CAR_MOTION_NONE,
	CAR_MOTION_ACCELERATION,	
};

enum QGlobe_CAR_STEER
{	
	CAR_STEER_RIGHT	= 0,
	CAR_STEER_MID,
	CAR_STEER_LEFT,
};

typedef struct
{
	double		m_speed;			// m/s^2
	double		m_travelTime;		// second
	double		m_travelDistance;	// meter
	double		m_directionAngle;	// degree
	double		m_maxSpeed;			// m/s^2
	double		m_maxBackSpeed;		// m/s^2


} QGlobe_CAR_STATUS;

class CQGlobe_Car
{
public:
	CQGlobe_Car(void);
	virtual ~CQGlobe_Car(void);

public:
	void			InitCar(IN CQGlobe_Camera* a_pCamera);

	void			SetCarMotion(IN QGlobe_CAR_MOTION carMotion);
	void			SetCarSteer(IN QGlobe_CAR_STEER carSteer);
	
	void			GetCarStatus(OUT QGlobe_CAR_STATUS* a_pCarStatus);

	void			SetCarHeight(IN double a_carHeight ) { m_carHeight = a_carHeight; }

public:
	void			AdvanceCar( double a_dt );

private:
	void			TurnCar( double a_dt );
	double			MoveCar( double a_dt );

	void			GetDEMPosition( CQGlobe_Vector3DF& a_worldPos, CQGlobe_Vector3DF& a_demPos, double a_carHeight );
	double			GetDEMHeight( CQGlobe_Vector3DF& a_worldPos );
	void			ComputeStartPosition();
	void			ComputeUpVector( CQGlobe_Vector3DF& a_pos, CQGlobe_Vector3DF& a_upVector );
    double			ComputeClimbAngle( CQGlobe_Vector3DF& a_startPos, CQGlobe_Vector3DF& a_endPos );
	void			MoveCameraCoord( CQGlobe_Vector3DF& a_startPos, CQGlobe_Vector3DF& a_forwardVector, CQGlobe_Vector3DF& a_upVector, double a_dt );

	void			ComputeSpeed( double a_dt );
	void			ComputeStatus();

	bool			ApproachGround( double a_dt );

private:	
	double			m_speed;

	double			m_travelTime;
	double			m_travelDistance;


	double			m_accelSpeed;		// m/s^2
	double			m_accelAngle;		// degree/s
	double			m_maxAccelSpeed;
	double			m_maxAccelAngle;
	double			m_maxSpeed;
	double			m_maxBackSpeed;
	double			m_maxTurnSpeed;
	double			m_frictionFactor;
	double			m_dragFactor;

	double			m_directionAngle;

	QGlobe_CAR_MOTION	m_carMotion;
	QGlobe_CAR_STEER	m_carSteer;

	CQGlobe_Camera*	m_pCamera;

	double			m_carHeight;
	

	QGlobe_Coord3D		m_cameraCoord;
	bool			m_firstAdvance;
	bool			m_secondAdvance;
	bool			m_approachInit;
	double			m_approachSpeed;
	double			m_approachTime;
	double			m_endHeightLogs[LOG_NUM];
	double			m_angleLogs[LOG_NUM];
	int				m_logIndex;
};
#endif

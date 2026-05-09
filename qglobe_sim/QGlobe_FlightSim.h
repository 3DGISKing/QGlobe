#ifndef __QGlobe_FLIGHT_SIMULATOR_H__
#define __QGlobe_FLIGHT_SIMULATOR_H__

#include "QGlobe_Simulator.h"
#include "QGlobe_FlightSimTypes.h"
#include "QGlobe_ControlSurface.h"
#include "QGlobe_AircraftSpecs.h"

enum FS_INPUT_DEVICE
{
	eFS_INPUT_DEVICE_KEYBOARD,
	eFS_INPUT_DEVICE_MOUSE
};
class CQGlobe_FlightSim : public CQGlobe_Simulator
{
public:
	CQGlobe_FlightSim (CQGlobe_SimMgr* a_pSimMgr);
	~CQGlobe_FlightSim (void);

public:
	virtual	void	InitSimulation();
	virtual bool	AdvanceSimulation(IN double a_dt);
	virtual void	GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus);	
	virtual void	EndSimulation();
	virtual void	RestartSimulation();

	virtual bool	OnMousePress(CursorState* mouseEvent);
	virtual bool	OnMouseMove(CursorState* mouseEvent);
	virtual void	OnKeyPressEvent(KeyState keyState);

private:
	void			UpdateCamera( void );
	void			ComputeSimulationStatus();
	void			ComputeAircraftHeights( double* a_seaLevelHeight, double* a_demHeight, double* a_alititude );

private:
	void			RotateAircraft( FS_Millisecond a_Milliseconds );
	void			RotateAircraft( FS_AXIS a_Axis, FS_Radian a_Rad );
	void			ApplyForce( FS_Millisecond a_Milliseconds );
	void			ComuteTotalForce( void );
	void			HandleCollision( void );
	void			HandleStall( void );
	void			HandleKeyEvent( int a_key, FS_Fraction* a_aileron, FS_Fraction* a_elevator, 
									FS_Fraction* a_rudder, FS_Fraction* a_a_thrust );

private:
	FS_Factor		ComputeRotationFactor( void );
	bool			HasVelocity( void );
	FS_Degree		ComputeAngleOfAttack( bool a_hasVelocity );
	FS_Vector3		ComputeGravityDir( void );
	void			ComputeSimulationSpeed();
	double			ComputeAltitudeFromSeaLevel( void );
	void			VectorWorld2Aircraft( FS_Vector3* a_vector );
	void			VectorAircraft2World( FS_Vector3* a_vector );

private:
	FS_Poistion					m_AircraftPosition;
	FS_Vector3					m_AircraftAxis[3];
	CQGlobe_ControlSurface			m_ControlSurface;
	CQGlobe_AircraftSpecs			m_AircraftSpecs;

private:
	FS_Velocity		m_Velocity;
	FS_Speed		m_Speed;
	FS_Millisecond	m_PrevMilliseconds;
	FS_Newton		m_TotalForce;
	FS_Tensor		m_InertiaTensor;

	FS_INPUT_DEVICE	m_LastInputDevice;

	bool			m_UseMouse;
	bool			m_Paused;
	bool			m_Ended;
	bool			m_ShowHelp;
	bool			m_Collided;
	bool			m_Stall;

	QGlobe_SIM_STATUS	m_simStatus;

};
#endif
#ifndef __QGlobe_SIMULATOR_MANAGER_H__
#define __QGlobe_SIMULATOR_MANAGER_H__

#include "../qglobe_libmain/QGlobe_Camera.h"

class CQGlobe_Camera;
class CGIS_Path;
class CQGlobe_Simulator;

enum QGlobe_SIM_TYPE
{
	SIM_TYPE_FLIGHT,
	SIM_TYPE_CAR,
	SIM_TYPE_PATH,

	SIM_TYPE_NONE
};

struct QGlobe_SIM_STATUS
{
	// SimMgr status
	QGlobe_SIM_TYPE	m_type;
	int				m_screenWidth;
	int				m_screenHeight;

	// common
	double		m_speed;
	bool		m_paused;
	bool		m_ended;
	bool		m_showHelp;
	bool		m_autoMove;
	
	// Flight Simulation
	double		m_heading;
	double		m_bankangle;
	double		m_verticalspeed;
	double		m_throttle;
	double		m_rudder;
	double		m_aileron;
	double		m_elevator;
	int			m_flap;
	bool		m_gear;
	double		m_pitchangle;
	int			m_altitude;
	bool		m_stall;
	bool		m_useMouse;
	bool		m_collided;

	// for Path simulation
	double		m_pathtotaltime;
	double		m_pathcurrenttime;
	int			m_pathreplay;
	int		    m_pathplaystatus;
	double      m_pathcameradistance;
	double      m_pathcameraslopangle;

	// for Car simulation
	double		m_travelTime;
	double		m_travelDistance;
	double		m_maxSpeed;
	double		m_maxBackSpeed;
	double		m_directionAngle;


	//...
};

// for Path simulation
enum QGlobe_SIM_CONTROL
{
	SIM_CONTROL_PLAY,
	SIM_CONTROL_STOP,
	SIM_CONTROL_FORWARD,
	SIM_CONTROL_BACK,
	SIM_CONTROL_REPEAT,
	SIM_CONTROL_SCROLL,

	SIM_CONTROL_NONE
};

class CQGlobe_SimMgr
{
public:
	CQGlobe_SimMgr(void);
	~CQGlobe_SimMgr(void);

	void	StartSimulation(IN QGlobe_SIM_TYPE a_type, IN bool a_restart = false );
	bool	AdvanceSimulation();
	void	EndSimulation();

	void	SetCamera(IN CQGlobe_Camera* a_pCamera);
	void	SetScreenSize(IN int a_width, IN int a_height);
	void	SetGISPath(IN CGIS_Path* a_pGISPath ,OUT QGlobe_CAMERA_INFO* a_pCameraInfo);
	void	ControlPlay(IN QGlobe_SIM_CONTROL a_type, double a_dtime = 0.0f);
	void	SetCarHeight(IN double a_carHeight ) ;

	void	GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus);
	void	SetSimulationStatus(IN  QGlobe_SIM_STATUS* a_pSimStatus);	

	double	GetSimulationStartTime();
	CQGlobe_Simulator* GetCurSimulator() {return m_pSimulator;}


private:

private:
	QGlobe_SIM_TYPE	m_type;
	double			m_lastTime;

	CQGlobe_Simulator*	m_pSimulator;
	CQGlobe_Camera*	m_pCamera;
	int				m_screenWidth;
	int				m_screenHeight;

};
#endif
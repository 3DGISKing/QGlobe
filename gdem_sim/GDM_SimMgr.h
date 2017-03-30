#ifndef __GDM_SIMULATOR_MANAGER_H__
#define __GDM_SIMULATOR_MANAGER_H__

#include "../gdem_libmain/GDM_Camera.h"

class CGDM_Camera;
class CGIS_Path;
class CGDM_Simulator;

enum GDM_SIM_TYPE
{
	SIM_TYPE_FLIGHT,
	SIM_TYPE_CAR,
	SIM_TYPE_PATH,

	SIM_TYPE_NONE
};

struct GDM_SIM_STATUS
{
	// SimMgr status
	GDM_SIM_TYPE	m_type;
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
enum GDM_SIM_CONTROL
{
	SIM_CONTROL_PLAY,
	SIM_CONTROL_STOP,
	SIM_CONTROL_FORWARD,
	SIM_CONTROL_BACK,
	SIM_CONTROL_REPEAT,
	SIM_CONTROL_SCROLL,

	SIM_CONTROL_NONE
};

class CGDM_SimMgr
{
public:
	CGDM_SimMgr(void);
	~CGDM_SimMgr(void);

	void	StartSimulation(IN GDM_SIM_TYPE a_type, IN bool a_restart = false );
	bool	AdvanceSimulation();
	void	EndSimulation();

	void	SetCamera(IN CGDM_Camera* a_pCamera);
	void	SetScreenSize(IN int a_width, IN int a_height);
	void	SetGISPath(IN CGIS_Path* a_pGISPath ,OUT GDM_CAMERA_INFO* a_pCameraInfo);
	void	ControlPlay(IN GDM_SIM_CONTROL a_type, double a_dtime = 0.0f);
	void	SetCarHeight(IN double a_carHeight ) ;

	void	GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus);
	void	SetSimulationStatus(IN  GDM_SIM_STATUS* a_pSimStatus);	

	double	GetSimulationStartTime();
	CGDM_Simulator* GetCurSimulator() {return m_pSimulator;}


private:

private:
	GDM_SIM_TYPE	m_type;
	double			m_lastTime;

	CGDM_Simulator*	m_pSimulator;
	CGDM_Camera*	m_pCamera;
	int				m_screenWidth;
	int				m_screenHeight;

};
#endif
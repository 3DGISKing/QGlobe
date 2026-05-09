#include "QGlobe_SimMgr.h"
#include "QGlobe_Command.h"
#include "QGlobe_CarSim.h"
#include "QGlobe_FlightSim.h"
#include "QGlobe_PathSim.h"

CQGlobe_SimMgr::CQGlobe_SimMgr()
{
	m_pSimulator	= NULL;
	m_type			= SIM_TYPE_NONE;
	m_pCamera		= NULL;
	m_lastTime		= 0;
	m_screenWidth	= 0;
	m_screenHeight	= 0;
}

CQGlobe_SimMgr::~CQGlobe_SimMgr()
{
	EndSimulation();
}

void
CQGlobe_SimMgr::StartSimulation(IN QGlobe_SIM_TYPE a_type, IN bool a_restart)
{
	if ( a_type < SIM_TYPE_FLIGHT || a_type > SIM_TYPE_NONE )
		return;

	EndSimulation();

	m_type = a_type;
	switch (a_type)
	{
	case SIM_TYPE_CAR:
			m_pSimulator = (CQGlobe_Simulator*) new CQGlobe_CarSim(this);
		break;
	case SIM_TYPE_FLIGHT:
		m_pSimulator = (CQGlobe_Simulator*) new CQGlobe_FlightSim(this);
		break;
	case SIM_TYPE_PATH:
		m_pSimulator = (CQGlobe_Simulator*) new CQGlobe_PathSim(this);
		break;
	default:
		{
			m_type = SIM_TYPE_NONE;
		}
		break;
	}
	m_pSimulator->SetCamera( m_pCamera );
	m_pSimulator->SetScreenSize( m_screenWidth, m_screenHeight );
	m_pSimulator->InitSimulation();
	if ( a_restart )
		m_pSimulator->RestartSimulation();
	m_lastTime	= qglobe_GetTickTimer();
}

void
CQGlobe_SimMgr::EndSimulation()
{
	if (!m_pSimulator)
		return;
	m_pSimulator->EndSimulation();

	switch (m_type)
	{
	case SIM_TYPE_CAR:
		{
			CQGlobe_CarSim* pSim = (CQGlobe_CarSim*) m_pSimulator;
			delete pSim;
		}
		break;
	case SIM_TYPE_FLIGHT:
		{
			CQGlobe_FlightSim* pSim = (CQGlobe_FlightSim*) m_pSimulator;
			delete pSim;
		}
		break;
	case SIM_TYPE_PATH:
		{
			CQGlobe_PathSim* pSim = (CQGlobe_PathSim*) m_pSimulator;
			delete pSim;
		}
		break;
	default:
		break;
	}

	m_pSimulator	= NULL;
	m_type			= SIM_TYPE_NONE;	
}

bool
CQGlobe_SimMgr::AdvanceSimulation()
{
	if (m_pSimulator == NULL)
		return false;

	double newTime, dt;
	newTime = qglobe_GetTickTimer();
	dt = newTime - m_lastTime;
	m_lastTime = newTime;

	return m_pSimulator->AdvanceSimulation(dt);

}

void	
CQGlobe_SimMgr::SetCamera(IN CQGlobe_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;

	if (m_pSimulator)
		m_pSimulator->SetCamera( m_pCamera );
}

void	
CQGlobe_SimMgr::SetScreenSize(IN int a_width, IN int a_height)
{
	m_screenWidth	= a_width;
	m_screenHeight	= a_height;

	if (m_pSimulator)
		m_pSimulator->SetScreenSize( m_screenWidth, m_screenHeight );
}

void	
CQGlobe_SimMgr::SetGISPath(IN CGIS_Path* a_pGISPath ,OUT QGlobe_CAMERA_INFO* a_pCameraInfo)
{
	if (m_type != SIM_TYPE_PATH || !m_pSimulator)
		return;

	CQGlobe_PathSim* pSim = (CQGlobe_PathSim*)m_pSimulator;
	pSim->SetGISPath( a_pGISPath , a_pCameraInfo);
}
void
CQGlobe_SimMgr::ControlPlay(IN QGlobe_SIM_CONTROL a_type, double a_dtime)
{
	if (m_type != SIM_TYPE_PATH || !m_pSimulator)
		return;

	CQGlobe_PathSim* pSim = (CQGlobe_PathSim*)m_pSimulator;
	pSim->ControlPlay( a_type, a_dtime);
}

void	
CQGlobe_SimMgr::GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus)
{
	if ( a_pSimStatus == NULL )
		return;

	a_pSimStatus->m_type		= m_type;
	a_pSimStatus->m_screenWidth	= m_screenWidth;
	a_pSimStatus->m_screenHeight= m_screenHeight;

	if (m_pSimulator)
		m_pSimulator->GetSimulationStatus( a_pSimStatus );
}
void	
CQGlobe_SimMgr::SetSimulationStatus(IN QGlobe_SIM_STATUS* a_pSimStatus)
{
	if (m_type != SIM_TYPE_PATH || !m_pSimulator)
		return;

	CQGlobe_PathSim* pSim = (CQGlobe_PathSim*)m_pSimulator;
	pSim->SetSimulationStatus( a_pSimStatus);
}

double	
CQGlobe_SimMgr::GetSimulationStartTime()
{
	return 0;
}

void
CQGlobe_SimMgr::SetCarHeight(IN double a_carHeight )
{
	if ( m_type != SIM_TYPE_CAR || !m_pSimulator )
		return;
	CQGlobe_CarSim* pSim = (CQGlobe_CarSim*)m_pSimulator;
	pSim->SetCarHeight( a_carHeight );
}
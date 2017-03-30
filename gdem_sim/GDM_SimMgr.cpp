#include "GDM_SimMgr.h"
#include "GDM_Command.h"
#include "GDM_CarSim.h"
#include "GDM_FlightSim.h"
#include "GDM_PathSim.h"

CGDM_SimMgr::CGDM_SimMgr()
{
	m_pSimulator	= NULL;
	m_type			= SIM_TYPE_NONE;
	m_pCamera		= NULL;
	m_lastTime		= 0;
	m_screenWidth	= 0;
	m_screenHeight	= 0;
}

CGDM_SimMgr::~CGDM_SimMgr()
{
	EndSimulation();
}

void
CGDM_SimMgr::StartSimulation(IN GDM_SIM_TYPE a_type, IN bool a_restart)
{
	if ( a_type < SIM_TYPE_FLIGHT || a_type > SIM_TYPE_NONE )
		return;

	EndSimulation();

	m_type = a_type;
	switch (a_type)
	{
	case SIM_TYPE_CAR:
			m_pSimulator = (CGDM_Simulator*) new CGDM_CarSim(this);
		break;
	case SIM_TYPE_FLIGHT:
		m_pSimulator = (CGDM_Simulator*) new CGDM_FlightSim(this);
		break;
	case SIM_TYPE_PATH:
		m_pSimulator = (CGDM_Simulator*) new CGDM_PathSim(this);
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
	m_lastTime	= gdm_GetTickTimer();
}

void
CGDM_SimMgr::EndSimulation()
{
	if (!m_pSimulator)
		return;
	m_pSimulator->EndSimulation();

	switch (m_type)
	{
	case SIM_TYPE_CAR:
		{
			CGDM_CarSim* pSim = (CGDM_CarSim*) m_pSimulator;
			delete pSim;
		}
		break;
	case SIM_TYPE_FLIGHT:
		{
			CGDM_FlightSim* pSim = (CGDM_FlightSim*) m_pSimulator;
			delete pSim;
		}
		break;
	case SIM_TYPE_PATH:
		{
			CGDM_PathSim* pSim = (CGDM_PathSim*) m_pSimulator;
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
CGDM_SimMgr::AdvanceSimulation()
{
	if (m_pSimulator == NULL)
		return false;

	double newTime, dt;
	newTime = gdm_GetTickTimer();
	dt = newTime - m_lastTime;
	m_lastTime = newTime;

	return m_pSimulator->AdvanceSimulation(dt);

}

void	
CGDM_SimMgr::SetCamera(IN CGDM_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;

	if (m_pSimulator)
		m_pSimulator->SetCamera( m_pCamera );
}

void	
CGDM_SimMgr::SetScreenSize(IN int a_width, IN int a_height)
{
	m_screenWidth	= a_width;
	m_screenHeight	= a_height;

	if (m_pSimulator)
		m_pSimulator->SetScreenSize( m_screenWidth, m_screenHeight );
}

void	
CGDM_SimMgr::SetGISPath(IN CGIS_Path* a_pGISPath ,OUT GDM_CAMERA_INFO* a_pCameraInfo)
{
	if (m_type != SIM_TYPE_PATH || !m_pSimulator)
		return;

	CGDM_PathSim* pSim = (CGDM_PathSim*)m_pSimulator;
	pSim->SetGISPath( a_pGISPath , a_pCameraInfo);
}
void
CGDM_SimMgr::ControlPlay(IN GDM_SIM_CONTROL a_type, double a_dtime)
{
	if (m_type != SIM_TYPE_PATH || !m_pSimulator)
		return;

	CGDM_PathSim* pSim = (CGDM_PathSim*)m_pSimulator;
	pSim->ControlPlay( a_type, a_dtime);
}

void	
CGDM_SimMgr::GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus)
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
CGDM_SimMgr::SetSimulationStatus(IN GDM_SIM_STATUS* a_pSimStatus)
{
	if (m_type != SIM_TYPE_PATH || !m_pSimulator)
		return;

	CGDM_PathSim* pSim = (CGDM_PathSim*)m_pSimulator;
	pSim->SetSimulationStatus( a_pSimStatus);
}

double	
CGDM_SimMgr::GetSimulationStartTime()
{
	return 0;
}

void
CGDM_SimMgr::SetCarHeight(IN double a_carHeight )
{
	if ( m_type != SIM_TYPE_CAR || !m_pSimulator )
		return;
	CGDM_CarSim* pSim = (CGDM_CarSim*)m_pSimulator;
	pSim->SetCarHeight( a_carHeight );
}
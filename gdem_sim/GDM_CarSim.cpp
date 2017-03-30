#include "stable.h"
//#include "../../gdem_client/gdemclient.h"
#include "GDM_CarSim.h"
#include "GDM_DataMgr.h"

#define START_ANIMATION_TIME	5000
#define CS_KEY_ESC				 Qt::Key_Escape
#define CS_KEY_PUASE			 Qt::Key_Space
#define CS_KEY_HELP				 Qt::Key_F1
#define CS_KEY_LEFT				 Qt::Key_Left
#define CS_KEY_RIGHT			 Qt::Key_Right
#define CS_KEY_UP				 Qt::Key_Up
#define CS_KEY_DOWN				 Qt::Key_Down

CGDM_CarSim::CGDM_CarSim(CGDM_SimMgr* a_pSimMgr) : CGDM_Simulator(a_pSimMgr)
{
	m_pCar = new CGDM_Car();	
	gdm_TileListSelector->SetLRUUsage(true);
}

CGDM_CarSim::~CGDM_CarSim()
{
}

void	CGDM_CarSim::InitSimulation()
{
	m_pCar->InitCar( m_pCamera );

	m_pCar->GetCarStatus( &m_carStatus );

	m_startAnimationTime = 0.0f;

	m_ended		= false;
	m_paused	= false;
	m_showHelp  = true;
}

bool	CGDM_CarSim::AdvanceSimulation(IN double a_dt)
{
	if ( m_startAnimationTime < START_ANIMATION_TIME )
	{
		m_startAnimationTime += a_dt;

		if ( m_ended )
			return true;
		else
			return false;
	}

	if ( m_ended )
		return true;

	if ( m_paused )
		return true;


	//m_pCar->Play(a_dt * 0.001);
	m_pCar->AdvanceCar( a_dt * 0.001f );

	return true;

}

void CGDM_CarSim::EndSimulation()
{
	if (m_pCar)
	{
		delete m_pCar;
	}	
	gdm_TileListSelector->SetLRUUsage(false);

}

void CGDM_CarSim::GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus)
{

	m_pCar->GetCarStatus(&m_carStatus);

	// type
	a_pSimStatus->m_type			= SIM_TYPE_CAR;
	// speed
	a_pSimStatus->m_speed			= m_carStatus.m_speed;

	// traveling time
	a_pSimStatus->m_travelTime		= m_carStatus.m_travelTime;

	// traveling distance
	a_pSimStatus->m_travelDistance	= m_carStatus.m_travelDistance;

	// direction angle
	a_pSimStatus->m_directionAngle	= m_carStatus.m_directionAngle;

	// max speeds
	a_pSimStatus->m_maxSpeed		= m_carStatus.m_maxSpeed;
	a_pSimStatus->m_maxBackSpeed	= m_carStatus.m_maxBackSpeed;

	// ended
	a_pSimStatus->m_ended			= m_ended;

	// paused
	a_pSimStatus->m_paused			= m_paused;

	// help
	a_pSimStatus->m_showHelp		= m_showHelp;

	// auto move
	a_pSimStatus->m_autoMove		= m_startAnimationTime < START_ANIMATION_TIME;

}

void CGDM_CarSim::OnKeyPressEvent(KeyState keyState)
{
	GDM_CAR_MOTION carMotion = CAR_MOTION_NONE;
	GDM_CAR_STEER carSteer = CAR_STEER_MID;

	if (keyState.key1 == CS_KEY_ESC)
	{
		m_ended = true;
		return;
	}

	if (keyState.key1 == CS_KEY_PUASE)
	{
		m_paused = !m_paused;
		if (m_paused)
			return;
	}

	if (keyState.key1 == CS_KEY_HELP)
	{
		m_showHelp = !m_showHelp;
		return;
	}

	if(keyState.key1 == CS_KEY_RIGHT || keyState.key1 == CS_KEY_LEFT)
	{
		carSteer = keyState.key1 == CS_KEY_RIGHT? CAR_STEER_RIGHT: CAR_STEER_LEFT;
	}

	if(keyState.key2 == CS_KEY_RIGHT || keyState.key2 == CS_KEY_LEFT)
	{
		carSteer = keyState.key2 == CS_KEY_RIGHT? CAR_STEER_RIGHT: CAR_STEER_LEFT;
	}

	if(keyState.key1 == CS_KEY_UP || keyState.key1 == CS_KEY_DOWN)
	{
		carMotion = keyState.key1 == CS_KEY_UP ? CAR_MOTION_ACCELERATION: CAR_MOTION_BRAKE;
	}		

	if(keyState.key2 == CS_KEY_UP || keyState.key2 == CS_KEY_DOWN)
	{
		carMotion = keyState.key2 == CS_KEY_UP ? CAR_MOTION_ACCELERATION: CAR_MOTION_BRAKE;
	}		

	m_pCar->SetCarMotion(carMotion);
	m_pCar->SetCarSteer(carSteer);
}

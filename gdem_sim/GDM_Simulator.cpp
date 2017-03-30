#include "GDM_Simulator.h"

CGDM_Simulator::CGDM_Simulator(CGDM_SimMgr* a_pSimMgr)
{
	m_pSimMgr		= a_pSimMgr;
	m_pCamera		= NULL;
	m_screenWidth	= 0;
	m_screenHeight	= 0;
	m_simulationSpeed = 1.0f;
}

CGDM_Simulator::~CGDM_Simulator()
{

}

void 
CGDM_Simulator::SetCamera(IN CGDM_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;
}

void
CGDM_Simulator::SetScreenSize(IN int a_width, IN int a_height)
{
	m_screenWidth = a_width;
	m_screenHeight = a_height;
}

void
CGDM_Simulator::SetSimulationSpeed( IN double a_simulatinSpeed )
{
	m_simulationSpeed = a_simulatinSpeed;
}


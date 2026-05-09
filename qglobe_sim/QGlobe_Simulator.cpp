#include "QGlobe_Simulator.h"

CQGlobe_Simulator::CQGlobe_Simulator(CQGlobe_SimMgr* a_pSimMgr)
{
	m_pSimMgr		= a_pSimMgr;
	m_pCamera		= NULL;
	m_screenWidth	= 0;
	m_screenHeight	= 0;
	m_simulationSpeed = 1.0f;
}

CQGlobe_Simulator::~CQGlobe_Simulator()
{

}

void 
CQGlobe_Simulator::SetCamera(IN CQGlobe_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;
}

void
CQGlobe_Simulator::SetScreenSize(IN int a_width, IN int a_height)
{
	m_screenWidth = a_width;
	m_screenHeight = a_height;
}

void
CQGlobe_Simulator::SetSimulationSpeed( IN double a_simulatinSpeed )
{
	m_simulationSpeed = a_simulatinSpeed;
}


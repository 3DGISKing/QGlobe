#ifndef __GDM_SIMULATOR_H__
#define __GDM_SIMULATOR_H__

#include "GDM_SimMgr.h"

class CGDM_Simulator
{
public:
	CGDM_Simulator(CGDM_SimMgr* a_pSimMgr);
	virtual ~CGDM_Simulator();

public:
	virtual	void	InitSimulation() = 0;
	virtual bool	AdvanceSimulation(IN double a_dt) = 0;
	virtual void	EndSimulation() = 0;
	virtual void	GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus) = 0;	
	virtual	void	RestartSimulation() {}
	//virtual void	SetSimulationStatus(IN GDM_SIM_STATUS* a_pSimStatus) = 0;	

	virtual bool	OnMousePress(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseMove(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseRelease(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseDblClick(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseWheelEvent(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseClickEvent(CursorState* mouseEvent) {return true;}
	virtual void	OnKeyPressEvent(KeyState keyState){} 


	void	SetCamera(IN CGDM_Camera* a_pCamera);
	void	SetScreenSize(IN int a_width, IN int a_height);
	void	SetSimulationSpeed( IN double a_simulatinSpeed );

protected:
	CGDM_SimMgr*	m_pSimMgr;
	CGDM_Camera*	m_pCamera;
	int				m_screenWidth;
	int				m_screenHeight;
	double			m_simulationSpeed;
};

#endif
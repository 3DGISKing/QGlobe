#ifndef __QGlobe_SIMULATOR_H__
#define __QGlobe_SIMULATOR_H__

#include "QGlobe_SimMgr.h"

class CQGlobe_Simulator
{
public:
	CQGlobe_Simulator(CQGlobe_SimMgr* a_pSimMgr);
	virtual ~CQGlobe_Simulator();

public:
	virtual	void	InitSimulation() = 0;
	virtual bool	AdvanceSimulation(IN double a_dt) = 0;
	virtual void	EndSimulation() = 0;
	virtual void	GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus) = 0;	
	virtual	void	RestartSimulation() {}
	//virtual void	SetSimulationStatus(IN QGlobe_SIM_STATUS* a_pSimStatus) = 0;	

	virtual bool	OnMousePress(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseMove(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseRelease(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseDblClick(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseWheelEvent(CursorState* mouseEvent) {return true;}
	virtual bool	OnMouseClickEvent(CursorState* mouseEvent) {return true;}
	virtual void	OnKeyPressEvent(KeyState keyState){} 


	void	SetCamera(IN CQGlobe_Camera* a_pCamera);
	void	SetScreenSize(IN int a_width, IN int a_height);
	void	SetSimulationSpeed( IN double a_simulatinSpeed );

protected:
	CQGlobe_SimMgr*	m_pSimMgr;
	CQGlobe_Camera*	m_pCamera;
	int				m_screenWidth;
	int				m_screenHeight;
	double			m_simulationSpeed;
};

#endif
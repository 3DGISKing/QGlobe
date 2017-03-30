#ifndef __GDM_CAR_SIMULATOR_H__
#define __GDM_CAR_SIMULATOR_H__

#include "GDM_Simulator.h"
#include "GDM_Car.h"

class CGDM_CarSim : public CGDM_Simulator
{
public:
	CGDM_CarSim(CGDM_SimMgr* a_pSimMgr);
	~CGDM_CarSim(void);

public:
	virtual	void	InitSimulation() ;
	virtual bool	AdvanceSimulation(IN double a_dt);
	virtual void	GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus);	
	virtual void	EndSimulation();

	virtual void	OnKeyPressEvent(KeyState keyState);

	void			SetCarHeight(IN double a_carHeight ) { m_pCar->SetCarHeight( a_carHeight); }

private:

private:
	CGDM_Car*		m_pCar;
	double			m_startAnimationTime;
	bool			m_ended;
	bool			m_paused;
	bool			m_showHelp;
	GDM_CAR_STATUS	m_carStatus;


};
#endif
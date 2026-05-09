#ifndef __QGlobe_CAR_SIMULATOR_H__
#define __QGlobe_CAR_SIMULATOR_H__

#include "QGlobe_Simulator.h"
#include "QGlobe_Car.h"

class CQGlobe_CarSim : public CQGlobe_Simulator
{
public:
	CQGlobe_CarSim(CQGlobe_SimMgr* a_pSimMgr);
	~CQGlobe_CarSim(void);

public:
	virtual	void	InitSimulation() ;
	virtual bool	AdvanceSimulation(IN double a_dt);
	virtual void	GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus);	
	virtual void	EndSimulation();

	virtual void	OnKeyPressEvent(KeyState keyState);

	void			SetCarHeight(IN double a_carHeight ) { m_pCar->SetCarHeight( a_carHeight); }

private:

private:
	CQGlobe_Car*		m_pCar;
	double			m_startAnimationTime;
	bool			m_ended;
	bool			m_paused;
	bool			m_showHelp;
	QGlobe_CAR_STATUS	m_carStatus;


};
#endif
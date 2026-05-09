#ifndef __QGlobe_PATH_SIMULATOR_H__
#define __QGlobe_PATH_SIMULATOR_H__

#include "QGlobe_Simulator.h"

enum CQGlobe_PATHSIM_STATUS
{
	SIM_PATH_STARTMOVE,
	SIM_PATH_START,
	SIM_PATH_END,
	SIM_PATH_PLAY,
	SIM_PATH_STOP,
	SIM_PATH_NONE,
};

class CQGlobe_PathSim : public CQGlobe_Simulator
{
public:
	CQGlobe_PathSim (CQGlobe_SimMgr* a_pSimMgr);
	~CQGlobe_PathSim (void);

	virtual	void	InitSimulation();
	virtual bool	AdvanceSimulation(IN double a_dt);
	virtual void	GetSimulationStatus(OUT QGlobe_SIM_STATUS* a_pSimStatus);	
	virtual void	EndSimulation();
	virtual void	OnKeyPressEvent(KeyState keyState);


	void			SetGISPath(IN CGIS_Path* a_pGISPath , OUT QGlobe_CAMERA_INFO* a_pCameraInfo);
	void			ControlPlay(IN QGlobe_SIM_CONTROL a_type, double a_dtime = 0.0f);
	void			SetSimulationStatus(IN QGlobe_SIM_STATUS* a_pSimStatus);
	
private:

	bool	StartAnimation();
	void	Cameramove();
	bool	GetSpeedVector(IN int a_nIndex, INOUT CQGlobe_Vector3DF* a_pvSpeed);
	bool	MoveStartPosition();
	bool	MoveEndPosition();
	void    GetDrawSegPoints(CQGlobe_VectorArr* pVetorArr, QGlobe_LOCATION* pStart, QGlobe_LOCATION* pEnd, double nDiv);
	void    ResetDivisionPoints();

private:
	CGIS_Path*		 m_pGISPath;
	CQGlobe_VectorArr   m_pArrTargetPos;
	CQGlobe_VectorArr   m_pArrCameraPos;
	qglobe_array<double>	 m_pArrdTimes;
	CQGlobe_LocationArr m_pArrLongLat;
	
	QGlobe_SIM_STATUS  m_sCurrentStatus;
	double			m_dStartMovetime;

	double			m_dCameraSpeed;
	double			m_dTargetSpeed;
	double			m_dSlopeAngle;	
	double			m_dCamerDistance;	
	double			m_dTotalTime;	
	double			m_dCurrentTime;	

	int				m_nSpeedStep;
	int			    m_nReplay;
	CQGlobe_PATHSIM_STATUS m_eStatus;

	CQGlobe_Vector3DF  vStartUp;
	CQGlobe_Vector3DF  vEndUp;
	
	bool		m_ended;

};
#endif
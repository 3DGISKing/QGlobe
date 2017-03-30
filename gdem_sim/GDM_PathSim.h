#ifndef __GDM_PATH_SIMULATOR_H__
#define __GDM_PATH_SIMULATOR_H__

#include "GDM_Simulator.h"

enum CGDM_PATHSIM_STATUS
{
	SIM_PATH_STARTMOVE,
	SIM_PATH_START,
	SIM_PATH_END,
	SIM_PATH_PLAY,
	SIM_PATH_STOP,
	SIM_PATH_NONE,
};

class CGDM_PathSim : public CGDM_Simulator
{
public:
	CGDM_PathSim (CGDM_SimMgr* a_pSimMgr);
	~CGDM_PathSim (void);

	virtual	void	InitSimulation();
	virtual bool	AdvanceSimulation(IN double a_dt);
	virtual void	GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus);	
	virtual void	EndSimulation();
	virtual void	OnKeyPressEvent(KeyState keyState);


	void			SetGISPath(IN CGIS_Path* a_pGISPath , OUT GDM_CAMERA_INFO* a_pCameraInfo);
	void			ControlPlay(IN GDM_SIM_CONTROL a_type, double a_dtime = 0.0f);
	void			SetSimulationStatus(IN GDM_SIM_STATUS* a_pSimStatus);
	
private:

	bool	StartAnimation();
	void	Cameramove();
	bool	GetSpeedVector(IN int a_nIndex, INOUT CGDM_Vector3DF* a_pvSpeed);
	bool	MoveStartPosition();
	bool	MoveEndPosition();
	void    GetDrawSegPoints(CGDM_VectorArr* pVetorArr, GDM_LOCATION* pStart, GDM_LOCATION* pEnd, double nDiv);
	void    ResetDivisionPoints();

private:
	CGIS_Path*		 m_pGISPath;
	CGDM_VectorArr   m_pArrTargetPos;
	CGDM_VectorArr   m_pArrCameraPos;
	gdm_array<double>	 m_pArrdTimes;
	CGDM_LocationArr m_pArrLongLat;
	
	GDM_SIM_STATUS  m_sCurrentStatus;
	double			m_dStartMovetime;

	double			m_dCameraSpeed;
	double			m_dTargetSpeed;
	double			m_dSlopeAngle;	
	double			m_dCamerDistance;	
	double			m_dTotalTime;	
	double			m_dCurrentTime;	

	int				m_nSpeedStep;
	int			    m_nReplay;
	CGDM_PATHSIM_STATUS m_eStatus;

	CGDM_Vector3DF  vStartUp;
	CGDM_Vector3DF  vEndUp;
	
	bool		m_ended;

};
#endif
#include "GDM_PathSim.h"
#include "GIS_Path.h"

#define  CAMERA_ANGLE_MIN				0.0
#define  CAMERA_ANGLE_MAX				80.0
#define  CAMERA_ANGLE_DEFAULT			45.0

#define  CAMERA_DISTANCE_MIN			150.0
#define  CAMERA_DISTANCE_MAX			5000.0
#define  CAMERA_DISTANCE_DEFAULT		1000.0

#define  CAMERA_SPEED_MIN				50.0
#define  CAMERA_SPEED_MAX				1000.0
#define  CAMERA_SPEED_DEFAULT			250.0

#define  CAMERA_ANIMATION_TIME			7000.0

#define  CAMERA_SMOOTH_TIME			    1500.0
#define  CAMERA_LIMITE_DIS			    50.0


CGDM_PathSim::CGDM_PathSim(CGDM_SimMgr* a_pSimMgr) : CGDM_Simulator(a_pSimMgr)
{
	m_dStartMovetime = 0;
	m_dCameraSpeed = CAMERA_SPEED_DEFAULT;
	m_dSlopeAngle = CAMERA_ANGLE_DEFAULT;
	m_dCamerDistance = CAMERA_DISTANCE_DEFAULT;	

	m_pGISPath = NULL;
	
	m_dTotalTime = 0.0;	
	m_dCurrentTime = 0.0;	
	m_nSpeedStep = 0;

	m_dTargetSpeed =  m_dCameraSpeed * EARTH_RADIUS/(m_dCamerDistance*cos(m_dSlopeAngle) + EARTH_RADIUS);

	m_eStatus = SIM_PATH_NONE;

	m_nReplay = 0;

	m_ended  = false;

}

CGDM_PathSim::~CGDM_PathSim()
{
	if(m_pGISPath) 
		m_pGISPath = NULL;

	m_pArrTargetPos.clear();
	m_pArrCameraPos.clear();
	m_pArrdTimes.clear();
}

void	
CGDM_PathSim::InitSimulation()
{	
	
	//m_dStarttime = gdm_GetTickCount();
	//m_eStatus == SIM_PATH_START;

}

bool	
CGDM_PathSim::AdvanceSimulation(IN double a_dt)
{
	if ( m_ended )
		return true;

	switch (m_eStatus)
	{
	case SIM_PATH_STARTMOVE:
		{
			m_dStartMovetime += a_dt;
			if (m_dStartMovetime < (CAMERA_ANIMATION_TIME+1000))
			{
				return false;
			}
			else
			{
				m_eStatus = SIM_PATH_START;
				m_nSpeedStep = 1;
			}			
			
		}
	case SIM_PATH_START:
		{
			return MoveStartPosition();
		}
	break;
	case SIM_PATH_PLAY:
		{
			m_dCurrentTime += a_dt * m_nSpeedStep;
			Cameramove();
		}
		break;
	case SIM_PATH_END:
		{
			return MoveEndPosition();
		}
		break;
	default:
		break;		
	}

	
	return true;
}
void	
CGDM_PathSim::GetSimulationStatus(OUT GDM_SIM_STATUS* a_pSimStatus)
{
	a_pSimStatus->m_type = SIM_TYPE_PATH;
	a_pSimStatus->m_speed  = m_dCameraSpeed;
	a_pSimStatus->m_pathcurrenttime = m_dCurrentTime;
	a_pSimStatus->m_pathtotaltime = m_dTotalTime;	
	a_pSimStatus->m_pathreplay = m_nReplay;
	a_pSimStatus->m_pathplaystatus = m_nSpeedStep;

	// ended
	a_pSimStatus->m_ended = m_ended;
	a_pSimStatus->m_autoMove = (m_eStatus == SIM_PATH_STARTMOVE);
}

void
CGDM_PathSim::EndSimulation()
{
	m_pArrTargetPos.clear();
	m_pArrCameraPos.clear();
	m_pArrdTimes.clear();

	m_dStartMovetime = 0;
	m_eStatus = SIM_PATH_NONE;
}




void
CGDM_PathSim::SetGISPath(IN CGIS_Path* a_pGISPath , OUT GDM_CAMERA_INFO* a_pCameraInfo)
{

	if (!a_pGISPath || (a_pGISPath->GetCount() < 2) )
		return;

	m_pGISPath = a_pGISPath;

	// Set Camera Target Position

	CGDM_Vector3DF vPosition;
	GDM_LOCATION   sLatLong;

	ResetDivisionPoints();

	int nCount = m_pArrTargetPos.size();

	m_dTotalTime = 0;

	// Set  Camera  Position

	CGDM_Vector3DF vSpeed;
	CGDM_Vector3DF vN1 , vN2 ;
	CGDM_Vector3DF vPos ;
	CGDM_Vector3DF vTemp ;
	double dtime = 0;

	for (int i = 0; i < nCount ;i++)
	{		

		if(GetSpeedVector(i, &vSpeed)== false)
			return;

		vN1 = m_pArrTargetPos[i].crossProduct(vSpeed);
		vN2 = (m_pArrTargetPos[i].crossProduct(vN1)).normalize();

		vTemp = m_pArrTargetPos[i];

		vPos =  m_pArrTargetPos[i] + (vTemp.normalize())*(m_dCamerDistance*cos(m_dSlopeAngle*gdm_DEGTORAD)) 
			+ vN2*(m_dCamerDistance*sin(m_dSlopeAngle*gdm_DEGTORAD));		

		m_pArrCameraPos.push_back(vPos);
	}

	for (int i = 0; i< nCount-1 ;i++)
	{		
		vSpeed = m_pArrCameraPos[i+1] - m_pArrCameraPos[i];
		dtime = (vSpeed.getLength())/m_dCameraSpeed * 1000;

		// for Smooth Move
		if (dtime < CAMERA_SMOOTH_TIME)
		{
			if (nCount > 2)
			{
				dtime = CAMERA_SMOOTH_TIME;
			}			
		}
		m_pArrdTimes.push_back(dtime);
		m_dTotalTime += dtime;	
	}

	//for (int i = 0; i< nCount-2 ;i++)
	//{
	//	vSpeed = m_pArrCameraPos[i+1] - m_pArrCameraPos[i];
	//	dtime = (vSpeed.getLength())/m_dCameraSpeed * 1000;

	//	if(dtime < 1000.f)
	//	{
	//		m_pArrCameraPos.erase(i+1);
	//		m_pArrTargetPos.erase(i+1);
	//		nCount --;
	//		i--;
	//	}
	//	else
	//	{
	//		m_pArrdTimes.push_back(dtime);
	//		m_dTotalTime += dtime;	
	//	}
	//}

	//vSpeed = m_pArrCameraPos[nCount-1] - m_pArrCameraPos[nCount-2];
	//dtime = (vSpeed.getLength())/m_dCameraSpeed * 1000;
	//m_pArrdTimes.push_back(dtime);
	//m_dTotalTime += dtime;	
	//StartAnimation();

	GDM_CAMERA_INFO cameraInfo;	
	GDM_Coord3D		coord;
	CGDM_Vector3DF vLookAt;

	vLookAt = m_pArrTargetPos[0] - m_pArrCameraPos[0];
	GetSpeedVector(0, &vSpeed);		
	vStartUp = vLookAt.crossProduct(vSpeed.crossProduct(vLookAt));	

	coord.m_org = m_pArrCameraPos[0];
	coord.m_z =   -(vLookAt.normalize());
	coord.m_y =   vStartUp.normalize();
	coord.m_x =   coord.m_y.crossProduct(coord.m_z);

	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));

	gdm_GetCoordInfo(&coord, &cameraInfo, 0);

	*a_pCameraInfo = cameraInfo;

	m_dStartMovetime = 0;

	m_eStatus = SIM_PATH_STARTMOVE;	
}


void 
CGDM_PathSim::ControlPlay(IN GDM_SIM_CONTROL a_type, double a_dtime)
{
	if( (m_pGISPath == NULL) || (m_eStatus == SIM_PATH_STARTMOVE))
		return;

	switch (a_type)
	{
	case SIM_CONTROL_PLAY:
		{
			m_eStatus = SIM_PATH_PLAY;
			m_nSpeedStep = 1;
		}
		break;
	case SIM_CONTROL_STOP:
		{
			m_eStatus = SIM_PATH_STOP;
			m_nSpeedStep = 0;
		}
		break;
	case SIM_CONTROL_FORWARD:
		{
			if((m_eStatus == SIM_PATH_START) || (m_eStatus == SIM_PATH_STOP))			
			{
				m_eStatus = SIM_PATH_PLAY;
			}
			if(m_nSpeedStep <= 0) m_nSpeedStep = 1;
			m_nSpeedStep *= 2; 
		}
		break;
	case SIM_CONTROL_BACK:
		{
			if((m_eStatus == SIM_PATH_END) || (m_eStatus == SIM_PATH_STOP))
			{
				m_eStatus = SIM_PATH_PLAY;
			}
			if(m_nSpeedStep >= 0) 
			{
				m_nSpeedStep = -1;
			}
			else
			{
				m_nSpeedStep *= 2;
			}	
		}
		break;
	case SIM_CONTROL_REPEAT:
		{
			m_nReplay ^= 0x01;
		}
		break;
	case SIM_CONTROL_SCROLL:
		{
			m_dCurrentTime = a_dtime;
			Cameramove();
			m_eStatus = SIM_PATH_STOP;
			m_nSpeedStep = 0;
		}
		break;
	default:
		break;
	}
}

void
CGDM_PathSim::SetSimulationStatus(IN GDM_SIM_STATUS* a_pSimStatus)
{
	if ( a_pSimStatus->m_pathcameradistance < CAMERA_DISTANCE_MIN || 
		a_pSimStatus->m_pathcameradistance > CAMERA_DISTANCE_MAX)
	{
		m_dCamerDistance = CAMERA_DISTANCE_DEFAULT;
	}
	else
	{
		m_dCamerDistance = a_pSimStatus->m_pathcameradistance;
	}

	if ( a_pSimStatus->m_pathcameraslopangle < CAMERA_ANGLE_MIN || 
		a_pSimStatus->m_pathcameraslopangle > CAMERA_ANGLE_MAX)
	{
		m_dSlopeAngle = CAMERA_ANGLE_DEFAULT;
	}
	else
	{
		m_dSlopeAngle = a_pSimStatus->m_pathcameraslopangle;
	}

	if ( a_pSimStatus->m_speed < CAMERA_SPEED_MIN || 
		a_pSimStatus->m_speed > CAMERA_SPEED_MAX)
	{
		m_dCameraSpeed = CAMERA_SPEED_DEFAULT;
	}
	else
	{
		m_dCameraSpeed = a_pSimStatus->m_speed;
	}

	m_dTargetSpeed =  m_dCameraSpeed * EARTH_RADIUS/(m_dCamerDistance*cos(m_dSlopeAngle) + EARTH_RADIUS);
}

bool
CGDM_PathSim::StartAnimation()
{
	if(!m_pGISPath) 
		return false;

	GDM_CAMERA_INFO cameraInfo;	
	GDM_Coord3D		coord;
	CGDM_Vector3DF vLookAt;
	CGDM_Vector3DF vSpeed;

	vLookAt = m_pArrTargetPos[0] - m_pArrCameraPos[0];
	GetSpeedVector(0, &vSpeed);		
	vStartUp = vLookAt.crossProduct(vSpeed.crossProduct(vLookAt));	

	coord.m_org = m_pArrCameraPos[0];
	coord.m_z =   -(vLookAt.normalize());
	coord.m_y =   vStartUp.normalize();
	coord.m_x =   coord.m_y.crossProduct(coord.m_z);

	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));

	gdm_GetCoordInfo(&coord, &cameraInfo, 0);

	m_dStartMovetime = 0;

	return true;
}

bool	
CGDM_PathSim::MoveStartPosition()
{
	CGDM_Vector3DF vLookAt;
	CGDM_Vector3DF vSpeed;

	vLookAt = m_pArrTargetPos[0] - m_pArrCameraPos[0];
	GetSpeedVector(0, &vSpeed);		
	vStartUp = vLookAt.crossProduct(vSpeed.crossProduct(vLookAt));	

	CGDM_Point3DF  CameraPosition(m_pArrCameraPos[0].m_tX,m_pArrCameraPos[0].m_tY,m_pArrCameraPos[0].m_tZ);
	m_pCamera->LookAt(&CameraPosition,&m_pArrTargetPos[0],&vStartUp);


	m_eStatus = SIM_PATH_PLAY;

	m_dCurrentTime = 0.0;


	return true;
}
bool	
CGDM_PathSim::MoveEndPosition()
{
	CGDM_Vector3DF vLookAt;
	CGDM_Vector3DF vSpeed;
	int Index = m_pArrTargetPos.size()-1;
	if(Index < 0) return false;

	vLookAt = m_pArrTargetPos[Index] - m_pArrCameraPos[Index];
	GetSpeedVector(Index, &vSpeed);		
	vEndUp = vLookAt.crossProduct(vSpeed.crossProduct(vLookAt));	

	CGDM_Point3DF  CameraPosition(m_pArrCameraPos[Index].m_tX,m_pArrCameraPos[Index].m_tY,m_pArrCameraPos[Index].m_tZ);
	m_pCamera->LookAt(&CameraPosition,&m_pArrTargetPos[Index],&vEndUp);

	m_eStatus = SIM_PATH_END;

	m_dCurrentTime = m_dTotalTime;

	return true;
}




#define		GDEM_MIN_PATH_DIVISION_ANGLE		0.03 // ~PAI / 30

inline void ConvertVector3D( IN CGDM_Location3D *pLoc ,OUT CGDM_Point3DF* pPosF)
{
	double dist = pLoc->m_tZ + EARTH_RADIUS;
	double projXY = dist * cos(pLoc->m_tY);
	pPosF->m_tZ = dist * sin(pLoc->m_tY);
	pPosF->m_tX = projXY * cos(pLoc->m_tX);
	pPosF->m_tY = projXY * sin(pLoc->m_tX);
}

void 
CGDM_PathSim::GetDrawSegPoints(CGDM_VectorArr* pVetorArr, GDM_LOCATION* pStart, GDM_LOCATION* pEnd, double nDiv)
{
	double	dHeight = 0;

	CGDM_Point3DF	ptStart, ptEnd;

	gdm_GetPosition (pStart, &ptStart);
	gdm_GetPosition (pEnd,	 &ptEnd);

	double			dAngle	 = 0;
	CGDM_Vector3DF	rotAxis = ptStart.crossProduct (ptEnd);

	gdm_GetAngleBetweenTwoVector (ptStart, ptEnd, rotAxis, dAngle);

	if (fabs (dAngle) > gdm_PI)
	{
		if (dAngle < 0)
			dAngle += gdm_TwoPI;
		else
			dAngle -= gdm_TwoPI;
	}

	CGDM_Location3D	locNode;

	double			dAngStep = dAngle / nDiv;
	CGDM_Point3DF	point	 = ptStart;

	int		nPts = nDiv / 2;

	if (nPts > nDiv / 6) nPts = nDiv / 6;

	double	dAngNode = dAngStep / 2;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptStart;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		
		pVetorArr->push_back(point);
	}

	for (int i=nPts+1; i<nDiv - nPts; i++)
	{
		point = ptStart;
		dAngNode += dAngStep;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		
		pVetorArr->push_back(point);
	}

	dAngNode = dAngStep/2 - nPts * dAngStep;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptEnd;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		
		pVetorArr->push_back(point);
	}

	if(pVetorArr->getLast() != ptEnd)
		pVetorArr->push_back(ptEnd);

	return;
}

void	GetSegPoints(CGDM_LocationArr* pLocationArr, GDM_LOCATION *pStart, GDM_LOCATION *pEnd, int nDiv)
{
	double	dHeight = 0;

	CGDM_Point3DF	ptStart, ptEnd;

	gdm_GetPosition (pStart, &ptStart);
	gdm_GetPosition (pEnd,	 &ptEnd);

	double			dAngle	 = 0;
	CGDM_Vector3DF	rotAxis = ptStart.crossProduct (ptEnd);

	gdm_GetAngleBetweenTwoVector (ptStart, ptEnd, rotAxis, dAngle);
	if (fabs (dAngle) > gdm_PI)
	{
		if (dAngle < 0)
			dAngle += gdm_TwoPI;
		else
			dAngle -= gdm_TwoPI;
	}

	CGDM_Location3D	locNode;

	double			dAngStep = dAngle / nDiv;
	CGDM_Point3DF	point	 = ptStart;

	int		nPts = nDiv / 2;

	if (nPts > nDiv / 6) nPts = nDiv / 6;

	double	dAngNode = dAngStep / 2;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptStart;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		gdm_GetLocation (&point, &locNode);
		pLocationArr->push_back(locNode);
	}

	for (int i=nPts+1; i<nDiv - nPts; i++)
	{
		point = ptStart;
		dAngNode += dAngStep;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		gdm_GetLocation (&point, &locNode);
		pLocationArr->push_back(locNode);
	}

	dAngNode = dAngStep/2 - nPts * dAngStep;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptEnd;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		gdm_GetLocation (&point, &locNode);
		pLocationArr->push_back(locNode);
	}

	gdm_GetLocation (&ptEnd, &locNode);
	if(pLocationArr->getLast() != locNode)
		pLocationArr->push_back(locNode);
}

void CGDM_PathSim::ResetDivisionPoints()
{
	if(!m_pGISPath)
		return;

	unsigned int i, count, allCount, nDiv;
	CGDM_Location3D pt1, pt2;
	GDM_LOCATION start, end;

	CGDM_Vector3DF vPosition;

	m_pArrLongLat.clear();

	count = m_pGISPath->m_PtArr.size();

	if(count <= 1)
		return;

	pt1 = m_pGISPath->m_PtArr[0];
	m_pArrLongLat.push_back(pt1);
	//ConvertVector3D(&pt1 , &vPosition);

	//m_pArrTargetPos.push_back(vPosition);

	allCount = count;

	for(i = 1; i < allCount; i++)
	{
		pt1 = m_pGISPath->m_PtArr[i - 1];
		if(i < count)
			pt2 = m_pGISPath->m_PtArr[i];
		else
			pt2 = m_pGISPath->m_PtArr[0];

		pt1.m_tZ = 0;
		pt2.m_tZ = 0;
		double dist = pt1.getDistanceFrom(pt2);

		if(dist < GDEM_MIN_PATH_DIVISION_ANGLE)
		{
			//ConvertVector3D(&pt2, &vPosition);		
			//m_pArrTargetPos.push_back(vPosition);

			m_pArrLongLat.push_back(pt2);

			continue;
		}

		// division
		nDiv = (int)(dist / GDEM_MIN_PATH_DIVISION_ANGLE + 1);
		start.m_dLongitude	= pt1.m_tX;
		start.m_dLatitude	= pt1.m_tY;
		start.m_dDist		= pt1.m_tZ;
		end.m_dLongitude	= pt2.m_tX;
		end.m_dLatitude		= pt2.m_tY;
		end.m_dDist			= pt2.m_tZ;

		//GetDrawSegPoints(&m_pArrTargetPos, &start, &end, nDiv);
		GetSegPoints(&m_pArrLongLat, &start, &end, nDiv);
	}


	int nTargetCount = m_pArrLongLat.size();	

	CGDM_Vector3DF vTemp;
	double         dLimite = m_dTargetSpeed / 2 ;

	for(i = 0; i < nTargetCount; i++)
	{		
		m_pGISPath->SetHeightValue(m_pArrLongLat[i]);
		ConvertVector3D(&m_pArrLongLat[i] , &vPosition);
		if (i > 0)
		{
			vTemp = vPosition - m_pArrTargetPos.getLast();

			if(vTemp.getLength() < dLimite)
				continue;
		}
		m_pArrTargetPos.push_back(vPosition);
	}
	if(m_pArrTargetPos.size() == 1)
	{
		m_pArrTargetPos.push_back(vPosition);
	}
	m_pArrLongLat.clear();
}

bool
CGDM_PathSim::GetSpeedVector(IN int a_nIndex, INOUT CGDM_Vector3DF* a_pvSpeed)
{
	int nCount = m_pArrTargetPos.size();

	if( nCount < 1 || a_nIndex >= nCount)
		return false;

	CGDM_Vector3DF vTemp, vN;
	if(a_nIndex == 0)
	{
		vTemp = m_pArrTargetPos[a_nIndex+1] - m_pArrTargetPos[a_nIndex];

		//*a_pvSpeed = m_pArrTargetPos[a_nIndex+1] - m_pArrTargetPos[a_nIndex];
	}
	//else if(a_nIndex == (nCount-1)) 
	//{
	//	*a_pvSpeed = m_pArrTargetPos[a_nIndex] - m_pArrTargetPos[a_nIndex-1];
	//}
	else
	{
		vTemp = m_pArrTargetPos[a_nIndex] - m_pArrTargetPos[a_nIndex -1];
		//*a_pvSpeed = m_pArrTargetPos[a_nIndex] - m_pArrTargetPos[a_nIndex -1];
	}

	double dSpeed = vTemp.getLength()/1.3;

	if((a_nIndex > 0) && (a_nIndex < (nCount - 1)))
	{
		CGDM_Vector3DF vTempNext = m_pArrTargetPos[a_nIndex+1] - m_pArrTargetPos[a_nIndex];
		
		dSpeed = vTempNext.getLength()/1.3;

	}

	vN = m_pArrTargetPos[a_nIndex].crossProduct(vTemp);
	
	*a_pvSpeed = (vN.crossProduct(m_pArrTargetPos[a_nIndex])).normalize() * dSpeed;

	return true;
}


void
CGDM_PathSim::Cameramove()
{		

	if(m_dCurrentTime > m_dTotalTime)
	{
		if(m_eStatus == SIM_PATH_PLAY)	
			MoveEndPosition();

		if(m_nReplay && m_eStatus != SIM_PATH_STOP) 
		{
			m_dCurrentTime = 0;
			m_eStatus = SIM_PATH_START;
		}
		else
		{
			m_nSpeedStep = 0;
		}

		return;
	}
	if(m_dCurrentTime < 0)
	{
		if(m_eStatus == SIM_PATH_PLAY)
		{
			MoveStartPosition();
			if(m_nReplay)
			{
				MoveEndPosition();
				m_dCurrentTime = m_dTotalTime;
				m_eStatus = SIM_PATH_PLAY;
			}
			else
			{
				m_eStatus = SIM_PATH_STOP;
				m_nSpeedStep = 0;
			}
		}		
	}

	const int nPointCount = (int)m_pArrTargetPos.size();

	if (nPointCount < 2)
		return;		

	double dtime = 0;
	int		idx = 0;

	for (idx = 0 ; idx< m_pArrdTimes.size();idx++)
	{		
		dtime += m_pArrdTimes[idx];
		if(m_dCurrentTime < dtime )
		{
			dtime -= m_pArrdTimes[idx];
			break;
		}
	}

	const double	u = (m_dCurrentTime - dtime)/m_pArrdTimes[idx];

	CGDM_Vector3DF  vTargetP0 = m_pArrTargetPos[idx];
	CGDM_Vector3DF  vTargetP1 = m_pArrTargetPos[idx + 1];

	CGDM_Vector3DF  vCameraP0 = m_pArrCameraPos[idx];
	CGDM_Vector3DF  vCameraP1 = m_pArrCameraPos[idx + 1];


	// hermite polynomials
	double h1 = 2.0f * u * u * u - 3.0f * u * u + 1.0f;
	double h2 = -2.0f * u * u * u + 3.0f * u * u;
	double h3 = u * u * u - 2.0f * u * u + u;
	double h4 = u * u * u - u * u;

	CGDM_Vector3DF V0 , V1;

	GetSpeedVector(idx , &V0);
	GetSpeedVector(idx+1 , &V1);
		
	 // interpolated Target position
	 CGDM_Vector3DF vTargetPosition  = vTargetP0 * h1 + vTargetP1 * h2 + V0 * h3 + V1 * h4;	

	 // interpolated Camera position
	 V0 = V0 * (m_dCameraSpeed /  m_dTargetSpeed);
	 V1 = V1 * (m_dCameraSpeed /  m_dTargetSpeed);
	CGDM_Vector3DF vCameraPosition  = vCameraP0 * h1 + vCameraP1 * h2 + V0 * h3 + V1 * h4;
		
	if(m_dSlopeAngle < 1.0)
	{
		//// Calculate Current Speed Vector
		//h1 = 6.0f * u * u  - 6.0f * u;
		//h2 = -6.0f * u * u  + 6.0f * u;
		//h3 = 3.0f * u * u - 4.0f * u + 1.0f;
		//h4 = 3.0f * u * u - 2.0f * u;

		//CGDM_Vector3DF vSpeed  = vCameraP0 * h1 + vCameraP1 * h2 + V0 * h3 + V1 * h4;

		CGDM_Vector3DF vSpeed  = V0 * (1.0 - u) + V1 * u;


		//change camera position
		CGDM_Vector3DF vLookAt = vTargetPosition - vCameraPosition;

		CGDM_Vector3DF UpVector = vLookAt.crossProduct(vSpeed.crossProduct(vLookAt));

		if(m_pCamera)
		{
			CGDM_Point3DF  CameraPosition(vCameraPosition.m_tX,vCameraPosition.m_tY,vCameraPosition.m_tZ);
			m_pCamera->LookAt(&CameraPosition,&vTargetPosition,&UpVector);
		}

	}
	else
	{
		CGDM_Vector3DF vLookAt;

		vLookAt = vTargetPosition - vCameraPosition;
		CGDM_Vector3DF vLookAtCenter = (-vCameraPosition);

		if(m_pCamera)
		{
			CGDM_Vector3DF UpVector = vLookAt.crossProduct(vLookAt.crossProduct(vLookAtCenter));
			CGDM_Point3DF  CameraPosition(vCameraPosition.m_tX,vCameraPosition.m_tY,vCameraPosition.m_tZ);
			m_pCamera->LookAt(&CameraPosition,&vTargetPosition,&UpVector);
		}


	}

}

void CGDM_PathSim::OnKeyPressEvent(KeyState keyState)
{

	if (keyState.key1 == Qt::Key_Escape)
	{
		m_ended = true;
		return;
	}
	////if(keyState.bMoveKey)
	//{		 
	//	if(keyState.key1 == 68 ) //d
	//	{	
	//		if((m_eStatus == SIM_PATH_START) || (m_eStatus == SIM_PATH_STOP))			
	//		{
	//			m_eStatus = SIM_PATH_PLAY;
	//		}
	//		if(m_nSpeedStep <= 0) m_nSpeedStep = 0;
	//		m_nSpeedStep += 1; 		
	//	}
	//	if(keyState.key1 == 65 ) //a
	//	{
	//		if((m_eStatus == SIM_PATH_END) || (m_eStatus == SIM_PATH_STOP))
	//		{
	//			m_eStatus = SIM_PATH_PLAY;
	//		}
	//		if(m_nSpeedStep >= 0) m_nSpeedStep = 0;
	//		m_nSpeedStep -= 1; 		
	//	}
	//	if(keyState.key1 == 87 ) //w
	//	{			
	//		m_nReplay ^= 0x01;
	//	}
	//	if(keyState.key1 == 83 ) //s
	//	{			
	//		m_eStatus = SIM_PATH_STOP;
	//		m_nSpeedStep = 0;

	//	}
	//	if(keyState.key1 == 88 ) //x
	//	{			
	//		m_eStatus = SIM_PATH_PLAY;
	//		m_nSpeedStep = 1;
	//		
	//	}

	//}
}

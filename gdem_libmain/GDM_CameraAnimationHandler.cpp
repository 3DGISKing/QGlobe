#include "GDM_CameraAnimationHandler.h"
#include "GDM_Camera.h"
#include "GDM_Coord3D.h"
#include "GDM_DataMgr.h"
#include "GDM_CollisionHandler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Animation Handler Base Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define T_STEP_COUNT					20
#define GDM_MAX_DOUBLE					1E+20



CGDM_CameraAnimationHandler::CGDM_CameraAnimationHandler(CGDM_Camera* pCamera)
{
	m_pCamera = pCamera;
}
CGDM_CameraAnimationHandler::~CGDM_CameraAnimationHandler()
{
	EndAnimation();
}

void CGDM_CameraAnimationHandler::StartAnimation()
{
	if(m_eAniType == ANI_NONE)
		return;
	EndAnimation();
}

double CGDM_CameraAnimationHandler::GetRepRate(double totalTime , double midTimeRate , double curTime, char RepType)
{
	double retVal;
	double a1 , a2 , v , t;
	double midTime = totalTime * midTimeRate;
	if( curTime > totalTime)
		curTime = totalTime;
	if(RepType > 0) // accelate
	{
		if(midTimeRate == 0)
		{
			a1 = 0;
			a2 = 2 / (totalTime * totalTime);
			v = a2 * totalTime;
		}
		else
		{
			a1 = 2 / (totalTime * totalTime * midTimeRate);
			a2 = a1 * midTimeRate / (1 - midTimeRate);
			v = a1 * midTime;
		}


		if(curTime < midTime)
		{
			t = curTime;
			retVal = a1 * t * t / 2;
		}
		else 
		{
			t = curTime - midTime;
			retVal = midTimeRate + v * t - a2 * t * t / 2;
		}

	}
	else
	{
		retVal = curTime / totalTime;
	}
	if(retVal > 1)
		retVal = 1;
	if(retVal < 0)
		retVal = 0;
	return retVal;
}

void CGDM_CameraAnimationHandler::EndAnimation()
{
	m_eAniType = ANI_NONE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Move Forward Animation Handler Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGDM_CameraMoveForwardAnimationHandler::CGDM_CameraMoveForwardAnimationHandler(CGDM_Camera* pCamera) : CGDM_CameraAnimationHandler(pCamera)
{
	m_eAniType = ANI_ROTATE;
}

CGDM_CameraMoveForwardAnimationHandler::~CGDM_CameraMoveForwardAnimationHandler()
{

}

void CGDM_CameraMoveForwardAnimationHandler::SetAniInfo(GDM_MOVE_FORWARD_ANI_INFO* pInfo)
{
	m_aniInfo = *pInfo;
	m_aniInfo.m_curhour = 0;
	m_orgCoord = m_pCamera->m_coord;
	m_totalOffset = 0;
	m_nStep = 0;
}

bool CGDM_CameraMoveForwardAnimationHandler::OnAnimation(double deltaHour)
{
	CGDM_Point3DF newOrg;
	CGDM_Vector3DF viewVt;
	GDM_Coord3D coord;
	m_aniInfo.m_curhour = m_aniInfo.m_curhour + deltaHour;
	if(m_aniInfo.m_curhour >= m_aniInfo.m_hour)
	{
		m_totalOffset = m_aniInfo.m_fOffsetLen;
		m_aniInfo.m_curhour = m_aniInfo.m_hour;
	}	
	if(m_aniInfo.m_curhour >= m_aniInfo.m_hour)
	{
		coord = m_orgCoord;
		m_pCamera->GetViewVector(&viewVt);
		coord.m_org = m_orgCoord.m_org + viewVt * m_aniInfo.m_fOffsetLen;
		m_pCamera->SetCameraCoord(&coord);
		EndAnimation();
		return false;
	}
	double t = GetRepRate(m_aniInfo.m_hour , 0 , m_aniInfo.m_curhour , (char)m_aniInfo.m_moveType);
	m_totalOffset = m_aniInfo.m_fOffsetLen * t;
	//if(m_nStep < 0)
	//	m_totalOffset += (m_aniInfo.m_fOffsetLen - m_totalOffset ) * 0.25;
	//else
	//	m_totalOffset += (m_aniInfo.m_fOffsetLen - m_totalOffset ) / 5;

	coord = m_orgCoord;
	m_pCamera->GetViewVector(&viewVt);
	coord.m_org = m_orgCoord.m_org + viewVt * m_totalOffset;
	m_pCamera->SetCameraCoord(&coord);
	m_nStep++;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Rotate Animation Handler Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGDM_CameraRotAnimationHandler::CGDM_CameraRotAnimationHandler(CGDM_Camera* pCamera) : CGDM_CameraAnimationHandler(pCamera)
{
	m_eAniType = ANI_ROTATE;
}

CGDM_CameraRotAnimationHandler::~CGDM_CameraRotAnimationHandler()
{

}

void CGDM_CameraRotAnimationHandler::SetAniInfo(GDM_ROT_ANI_INFO* pInfo)
{
	m_aniInfo = *pInfo;
	m_aniInfo.m_curhour = 0;
	m_orgCoord = m_pCamera->m_coord;
	m_totalOffset = 0;
}

bool CGDM_CameraRotAnimationHandler::OnAnimation(double deltaHour)
{
	if(m_aniInfo.m_blAuto)
	{
		double curHour = m_aniInfo.m_curhour + deltaHour;
		double t = curHour / m_aniInfo.m_hour;
		double offset = m_aniInfo.m_fOffsetAngle * t;
		if(offset >= gdm_TwoPI)
		{
			offset = offset - ((int)(offset / gdm_TwoPI)) * gdm_TwoPI;
		}
		else if(offset <= -gdm_TwoPI)
		{
			offset = -offset;
			offset = offset - ((int)(offset / gdm_TwoPI)) * gdm_TwoPI;
			offset = -offset;
		}
		CGDM_Vector3DF vt = m_aniInfo.m_aroundAxis;
		vt.normalize();
		GDM_Coord3D coord = m_orgCoord;
		GDM_Coord3D curCoord;
		double curLen , dstLen;
		m_pCamera->GetCameraCoord(&curCoord);
		gdm_RotateAroundVector(&coord , offset , vt);
		g_pGDMDataMgr->m_pCollisionHandler->ValidatePos(&coord.m_org);
		curLen = curCoord.m_org.getLength();
		dstLen = coord.m_org.getLength();
		if(dstLen < curLen)
		{
			double len = (curLen - dstLen) * 0.9 + dstLen;
			coord.m_org.setLength(len);
		}
		m_pCamera->SetCameraCoord(&coord,false);		
		m_aniInfo.m_curhour = curHour;
	}
	else
	{
		if(m_aniInfo.m_curhour == m_aniInfo.m_hour)
			return false;
		double curHour = m_aniInfo.m_curhour + deltaHour;
		m_totalOffset += (m_aniInfo.m_fOffsetAngle - m_totalOffset ) / 2;
		if(curHour >= m_aniInfo.m_hour)
		{
			m_totalOffset = m_aniInfo.m_fOffsetAngle;
			curHour = m_aniInfo.m_hour;
		}
		//double t = curHour / m_aniInfo.m_hour;
		//double offset = m_aniInfo.m_fOffsetAngle * t;

		CGDM_Vector3DF vt = m_aniInfo.m_aroundAxis;
		vt.normalize();
		GDM_Coord3D coord = m_orgCoord;
		gdm_RotateAroundVector(&coord , m_totalOffset , vt);
		m_pCamera->SetCameraCoord(&coord);
		m_aniInfo.m_curhour = curHour;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Move To Animation Handler Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGDM_CameraMoveToAnimationHandler::CGDM_CameraMoveToAnimationHandler(CGDM_Camera* pRender) : CGDM_CameraAnimationHandler(pRender)
{
	m_eAniType = ANI_MOVETO;
	m_nStepCount = 0;
	memset(m_cameraInfo , 0 , sizeof(GDM_CAMERA_INFO) * ANI_MAX_STEP);
}

CGDM_CameraMoveToAnimationHandler::~CGDM_CameraMoveToAnimationHandler()
{


}

void CGDM_CameraMoveToAnimationHandler::SetAniInfo(GDM_MOVETO_ANI_INFO* pInfo)
{
	memset(m_cameraInfo , 0 , sizeof(GDM_CAMERA_INFO) * ANI_MAX_STEP);
	memset(m_hour , 0 , sizeof(double) * ANI_MAX_STEP);
	m_aniInfo = *pInfo;
	m_aniInfo.m_curhour = 0;

	double height = m_aniInfo.m_dstLocation.m_loc.m_dDist * 2 + m_aniInfo.m_dstLocation.m_orgDist;
	double curHeight;

	m_nCurStep = 0;
	int nCurStep = 0;
	m_orgCoord = m_pCamera->m_coord;
	m_pCamera->GetInfo(&m_cameraInfo[nCurStep]);	

	double dist = 0;
	gdm_GetLength(g_pGDMDataMgr , &m_cameraInfo[0].m_loc , &m_aniInfo.m_dstLocation.m_loc , &dist);
	if(g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
		dist *= g_pGDMDataMgr->m_sOption.dem_detail_rate;// 고도 배률을 고려
	curHeight = m_cameraInfo[0].m_loc.m_dDist + m_cameraInfo[0].m_orgDist;

	if(m_aniInfo.m_pathType == 0)
	{
		height = height < dist ? dist : height;
	}
	else
	{
		
	}

	nCurStep++;
	if(m_aniInfo.m_pathType == 0) //by search
	{
		if(curHeight < height)
		{
			m_nStepCount = 3;
			// seecond node
			m_cameraInfo[nCurStep] = m_cameraInfo[nCurStep -1];
			m_cameraInfo[nCurStep].m_loc.m_dDist = height;
			m_cameraInfo[nCurStep].m_pitch = 0;
			nCurStep++;
		}
		else
		{
			m_nStepCount = 2;
		}
		m_cameraInfo[nCurStep].m_loc = pInfo->m_dstLocation.m_loc;
		m_cameraInfo[nCurStep].m_loc.m_dDist = height;
		m_cameraInfo[nCurStep].m_orgDist = m_aniInfo.m_dstLocation.m_orgDist;
		m_cameraInfo[nCurStep].m_pitch = m_aniInfo.m_dstLocation.m_pitch;
		m_cameraInfo[nCurStep].m_roll = m_aniInfo.m_dstLocation.m_roll;
		nCurStep++;

	}
	else if(m_aniInfo.m_pathType == 2) // by placemark modify
	{
		height = m_aniInfo.m_dstLocation.m_loc.m_dDist + m_aniInfo.m_dstLocation.m_orgDist;
		if(curHeight < height)
		{
			m_nStepCount = 2;
			// seecond node
			m_cameraInfo[nCurStep].m_loc = pInfo->m_dstLocation.m_loc;
			m_cameraInfo[nCurStep].m_loc.m_dDist = height;
			m_cameraInfo[nCurStep].m_orgDist = m_aniInfo.m_dstLocation.m_orgDist;
			m_cameraInfo[nCurStep].m_pitch = m_aniInfo.m_dstLocation.m_pitch;
			m_cameraInfo[nCurStep].m_roll = m_aniInfo.m_dstLocation.m_roll;
			nCurStep++;
		}
		else
		{
			m_nStepCount = 1;
		}
	}
	else // by mouse dblclick
	{
		m_nStepCount = 1;
		// seecond node
	}
	m_cameraInfo[nCurStep] = m_cameraInfo[nCurStep -1];
	m_cameraInfo[nCurStep].m_loc = pInfo->m_dstLocation.m_loc;
	m_cameraInfo[nCurStep].m_orgDist = m_aniInfo.m_dstLocation.m_orgDist;
	m_cameraInfo[nCurStep].m_pitch = m_aniInfo.m_dstLocation.m_pitch;
	m_cameraInfo[nCurStep].m_roll = m_aniInfo.m_dstLocation.m_roll;
	m_cameraInfo[nCurStep].m_yaw = 0;

	// time assign
	double totalDist = 0;
	double stepDist[ANI_MAX_STEP] = {0};
	CGDM_Point3DF pt1 , pt2;
	gdm_GetPosition(&m_cameraInfo[0].m_loc , &pt1);
	for( int i = 0; i < m_nStepCount; i++)
	{
		gdm_GetPosition(&m_cameraInfo[i+1].m_loc , &pt2);
		stepDist[i] = (pt1 - pt2).getLength();
		totalDist += stepDist[i];
		pt1 = pt2;
	}
	double val = 0;
	m_hour[0] = 0;
	m_hour[m_nStepCount] = m_aniInfo.m_hour;

	double interval = m_aniInfo.m_hour / m_nStepCount;
	for( int i = 1; i < m_nStepCount; i++)
	{
		m_hour[i] = m_hour[0] + interval * i;
		//val += stepDist[i];
		//if(i == (m_nStepCount - 1))
		//	m_hour[i+1] = m_aniInfo.m_hour;
		//else
		//	m_hour[i+1] = m_aniInfo.m_hour *  val / totalDist;
	}
}

void CGDM_CameraMoveToAnimationHandler::LerpCameraInfo(double t , GDM_CAMERA_INFO* resultInfo)
{
	memset(resultInfo , 0 , sizeof(GDM_CAMERA_INFO));

	// orientation
	resultInfo->m_pitch = m_endInfo.m_pitch * t + m_startInfo.m_pitch * (1-t);
	resultInfo->m_roll = m_endInfo.m_roll * t + m_startInfo.m_roll * (1-t);
	resultInfo->m_yaw = 0;
	// position
	double dtLongitude = fabs(m_startInfo.m_loc.m_dLongitude - m_endInfo.m_loc.m_dLongitude);
	if(dtLongitude <= gdm_PI)
	{
		resultInfo->m_loc.m_dLatitude = m_startInfo.m_loc.m_dLatitude * (1 - t) + m_endInfo.m_loc.m_dLatitude * t;
		resultInfo->m_loc.m_dLongitude = m_startInfo.m_loc.m_dLongitude * (1 - t) + m_endInfo.m_loc.m_dLongitude * t;
		resultInfo->m_loc.m_dDist = m_startInfo.m_loc.m_dDist * (1 - t) + m_endInfo.m_loc.m_dDist * t;
		resultInfo->m_orgDist = m_startInfo.m_orgDist * (1 - t) + m_endInfo.m_orgDist * t;
	}
	else
	{
		int nSign = m_startInfo.m_loc.m_dLongitude >= m_endInfo.m_loc.m_dLongitude ? 1 : -1;
		double dt = (gdm_TwoPI - dtLongitude) * nSign * t;
		double longitude = m_startInfo.m_loc.m_dLongitude  + dt;
		if(fabs(longitude) > gdm_PI)
			longitude -= nSign * gdm_TwoPI;
		if(t == 1)
			longitude = m_endInfo.m_loc.m_dLongitude;
		resultInfo->m_loc.m_dLongitude = longitude;
		resultInfo->m_loc.m_dLatitude = m_startInfo.m_loc.m_dLatitude * (1 - t) + m_endInfo.m_loc.m_dLatitude * t;
		resultInfo->m_loc.m_dDist = m_startInfo.m_loc.m_dDist * (1 - t) + m_endInfo.m_loc.m_dDist * t;
		resultInfo->m_orgDist = m_startInfo.m_orgDist * (1 - t) + m_endInfo.m_orgDist * t;
	}
}

bool CGDM_CameraMoveToAnimationHandler::OnAnimation(double deltaHour)
{
	GDM_Coord3D coord;
	CGDM_Quaternion q;
	CGDM_Vector3DF offsetVt;
	double t;
	double stepDeltaHour = 0 , stepHour;
	double curhour = m_aniInfo.m_curhour + deltaHour;
	if(m_nCurStep >= m_nStepCount || m_aniInfo.m_curhour > m_aniInfo.m_hour)
	{
		m_pCamera->SetInfo(&m_cameraInfo[m_nStepCount]);
		m_pCamera->Update();
		EndAnimation();
		return false;
	}
	if(curhour >= m_hour[m_nCurStep+1])
		curhour = m_hour[m_nCurStep+1];

	stepDeltaHour = curhour - m_hour[m_nCurStep];
	m_startInfo = m_cameraInfo[m_nCurStep];
	m_endInfo = m_cameraInfo[m_nCurStep + 1];
	stepHour = m_hour[m_nCurStep + 1] - m_hour[m_nCurStep];
	t = GetRepRate(stepHour , 0.5 , stepDeltaHour , (char) m_aniInfo.m_moveType);
	// angle interpolation
	GDM_CAMERA_INFO cameraInfo;
	LerpCameraInfo(t , &cameraInfo);

	m_pCamera->SetInfo(&cameraInfo);
	m_pCamera->Update();
	m_aniInfo.m_curhour = curhour;
	if(curhour == m_hour[m_nCurStep+1])
	{
		m_nCurStep++;
	}
	return true;
}


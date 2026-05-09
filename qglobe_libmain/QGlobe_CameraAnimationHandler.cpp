#include "QGlobe_CameraAnimationHandler.h"
#include "QGlobe_Camera.h"
#include "QGlobe_Coord3D.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_CollisionHandler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Animation Handler Base Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define T_STEP_COUNT					20
#define QGlobe_MAX_DOUBLE					1E+20



CQGlobe_CameraAnimationHandler::CQGlobe_CameraAnimationHandler(CQGlobe_Camera* pCamera)
{
	m_pCamera = pCamera;
}
CQGlobe_CameraAnimationHandler::~CQGlobe_CameraAnimationHandler()
{
	EndAnimation();
}

void CQGlobe_CameraAnimationHandler::StartAnimation()
{
	if(m_eAniType == ANI_NONE)
		return;
	EndAnimation();
}

double CQGlobe_CameraAnimationHandler::GetRepRate(double totalTime , double midTimeRate , double curTime, char RepType)
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

void CQGlobe_CameraAnimationHandler::EndAnimation()
{
	m_eAniType = ANI_NONE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Move Forward Animation Handler Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CQGlobe_CameraMoveForwardAnimationHandler::CQGlobe_CameraMoveForwardAnimationHandler(CQGlobe_Camera* pCamera) : CQGlobe_CameraAnimationHandler(pCamera)
{
	m_eAniType = ANI_ROTATE;
}

CQGlobe_CameraMoveForwardAnimationHandler::~CQGlobe_CameraMoveForwardAnimationHandler()
{

}

void CQGlobe_CameraMoveForwardAnimationHandler::SetAniInfo(QGlobe_MOVE_FORWARD_ANI_INFO* pInfo)
{
	m_aniInfo = *pInfo;
	m_aniInfo.m_curhour = 0;
	m_orgCoord = m_pCamera->m_coord;
	m_totalOffset = 0;
	m_nStep = 0;
}

bool CQGlobe_CameraMoveForwardAnimationHandler::OnAnimation(double deltaHour)
{
	CQGlobe_Point3DF newOrg;
	CQGlobe_Vector3DF viewVt;
	QGlobe_Coord3D coord;
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

CQGlobe_CameraRotAnimationHandler::CQGlobe_CameraRotAnimationHandler(CQGlobe_Camera* pCamera) : CQGlobe_CameraAnimationHandler(pCamera)
{
	m_eAniType = ANI_ROTATE;
}

CQGlobe_CameraRotAnimationHandler::~CQGlobe_CameraRotAnimationHandler()
{

}

void CQGlobe_CameraRotAnimationHandler::SetAniInfo(QGlobe_ROT_ANI_INFO* pInfo)
{
	m_aniInfo = *pInfo;
	m_aniInfo.m_curhour = 0;
	m_orgCoord = m_pCamera->m_coord;
	m_totalOffset = 0;
}

bool CQGlobe_CameraRotAnimationHandler::OnAnimation(double deltaHour)
{
	if(m_aniInfo.m_blAuto)
	{
		double curHour = m_aniInfo.m_curhour + deltaHour;
		double t = curHour / m_aniInfo.m_hour;
		double offset = m_aniInfo.m_fOffsetAngle * t;
		if(offset >= qglobe_TwoPI)
		{
			offset = offset - ((int)(offset / qglobe_TwoPI)) * qglobe_TwoPI;
		}
		else if(offset <= -qglobe_TwoPI)
		{
			offset = -offset;
			offset = offset - ((int)(offset / qglobe_TwoPI)) * qglobe_TwoPI;
			offset = -offset;
		}
		CQGlobe_Vector3DF vt = m_aniInfo.m_aroundAxis;
		vt.normalize();
		QGlobe_Coord3D coord = m_orgCoord;
		QGlobe_Coord3D curCoord;
		double curLen , dstLen;
		m_pCamera->GetCameraCoord(&curCoord);
		qglobe_RotateAroundVector(&coord , offset , vt);
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

		CQGlobe_Vector3DF vt = m_aniInfo.m_aroundAxis;
		vt.normalize();
		QGlobe_Coord3D coord = m_orgCoord;
		qglobe_RotateAroundVector(&coord , m_totalOffset , vt);
		m_pCamera->SetCameraCoord(&coord);
		m_aniInfo.m_curhour = curHour;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  Move To Animation Handler Class  ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CQGlobe_CameraMoveToAnimationHandler::CQGlobe_CameraMoveToAnimationHandler(CQGlobe_Camera* pRender) : CQGlobe_CameraAnimationHandler(pRender)
{
	m_eAniType = ANI_MOVETO;
	m_nStepCount = 0;
	memset(m_cameraInfo , 0 , sizeof(QGlobe_CAMERA_INFO) * ANI_MAX_STEP);
}

CQGlobe_CameraMoveToAnimationHandler::~CQGlobe_CameraMoveToAnimationHandler()
{


}

void CQGlobe_CameraMoveToAnimationHandler::SetAniInfo(QGlobe_MOVETO_ANI_INFO* pInfo)
{
	memset(m_cameraInfo , 0 , sizeof(QGlobe_CAMERA_INFO) * ANI_MAX_STEP);
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
	qglobe_GetLength(g_pGDMDataMgr , &m_cameraInfo[0].m_loc , &m_aniInfo.m_dstLocation.m_loc , &dist);
	if(g_pGDMDataMgr->IsVisible(E_QGlobe_SUBDATA_TYPE_DEM))
		dist *= g_pGDMDataMgr->m_sOption.dem_detail_rate;// ���� ����� ����
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
	CQGlobe_Point3DF pt1 , pt2;
	qglobe_GetPosition(&m_cameraInfo[0].m_loc , &pt1);
	for( int i = 0; i < m_nStepCount; i++)
	{
		qglobe_GetPosition(&m_cameraInfo[i+1].m_loc , &pt2);
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

void CQGlobe_CameraMoveToAnimationHandler::LerpCameraInfo(double t , QGlobe_CAMERA_INFO* resultInfo)
{
	memset(resultInfo , 0 , sizeof(QGlobe_CAMERA_INFO));

	// orientation
	resultInfo->m_pitch = m_endInfo.m_pitch * t + m_startInfo.m_pitch * (1-t);
	resultInfo->m_roll = m_endInfo.m_roll * t + m_startInfo.m_roll * (1-t);
	resultInfo->m_yaw = 0;
	// position
	double dtLongitude = fabs(m_startInfo.m_loc.m_dLongitude - m_endInfo.m_loc.m_dLongitude);
	if(dtLongitude <= qglobe_PI)
	{
		resultInfo->m_loc.m_dLatitude = m_startInfo.m_loc.m_dLatitude * (1 - t) + m_endInfo.m_loc.m_dLatitude * t;
		resultInfo->m_loc.m_dLongitude = m_startInfo.m_loc.m_dLongitude * (1 - t) + m_endInfo.m_loc.m_dLongitude * t;
		resultInfo->m_loc.m_dDist = m_startInfo.m_loc.m_dDist * (1 - t) + m_endInfo.m_loc.m_dDist * t;
		resultInfo->m_orgDist = m_startInfo.m_orgDist * (1 - t) + m_endInfo.m_orgDist * t;
	}
	else
	{
		int nSign = m_startInfo.m_loc.m_dLongitude >= m_endInfo.m_loc.m_dLongitude ? 1 : -1;
		double dt = (qglobe_TwoPI - dtLongitude) * nSign * t;
		double longitude = m_startInfo.m_loc.m_dLongitude  + dt;
		if(fabs(longitude) > qglobe_PI)
			longitude -= nSign * qglobe_TwoPI;
		if(t == 1)
			longitude = m_endInfo.m_loc.m_dLongitude;
		resultInfo->m_loc.m_dLongitude = longitude;
		resultInfo->m_loc.m_dLatitude = m_startInfo.m_loc.m_dLatitude * (1 - t) + m_endInfo.m_loc.m_dLatitude * t;
		resultInfo->m_loc.m_dDist = m_startInfo.m_loc.m_dDist * (1 - t) + m_endInfo.m_loc.m_dDist * t;
		resultInfo->m_orgDist = m_startInfo.m_orgDist * (1 - t) + m_endInfo.m_orgDist * t;
	}
}

bool CQGlobe_CameraMoveToAnimationHandler::OnAnimation(double deltaHour)
{
	QGlobe_Coord3D coord;
	CQGlobe_Quaternion q;
	CQGlobe_Vector3DF offsetVt;
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
	QGlobe_CAMERA_INFO cameraInfo;
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



#include "cameracontroller.h"
#include "gdem_protocol.h"
#include "rendersrv.h"
#include "gdm_renderview.h"
#include "GDM_CommandMgr.h"
#include "GDM_DataMgr.h"
#include "GIS_Doc.h"

extern GDM_RENDER_HANDLE	    g_pRender;

#define LOOKVIEW_OFFSET			2 // degree
#define TILT_OFFSET				1.5 // degree
#define AUTO_MAX_SPEED_RATE		0.5
#define SPEED_RATE				0.15 // rotate
#define SPEED_RATE1				0.75 // dbl click
#define SPEED_RATE2				0.2 // forward
#define SPEED_STEP				9
#define CAMERA_TILT_LIMIT		1

#define INIT_LATI_KOREA			39.02 
#define INIT_LONGI_KOREA		125.75 
#define INIT_DST_KOREA			(600000)

#define INIT_LATI_PYONGSONG		39.27 
#define INIT_LONGI_PYONGSONG	125.87 
#define INIT_DST_PYONGSONG		(15000)

#define INIT_LATI_MTKUMGANG		38.685
#define INIT_LONGI_MTKUMKANG	128.2 
#define INIT_DST_KUMKANG	    10000

#define INIT_LATI		        INIT_LATI_KOREA
#define INIT_LONGI		        INIT_LONGI_KOREA 
#define INIT_DST			    INIT_DST_KOREA
#define INIT_PITCH              0 //degree
#define INIT_ROLL               0  //degreee

#define DEFAULT_MOVETO_RATE		0.1
#define AUTO_ROT_TIME			100
#define ROT_TIME				300
#define FAST_MOVE_TO_TIME		500
#define FASTEST_MOVE_TO_TIME	10
#define MOVE_TO_TIME			8000
#define MOVE_TO_BY_MOUSE_TIME   3000
#define MOVE_TO_BY_RESET	    1000
#define MOVE_FORWARD_TIME		300

// define select range
#define GDEM_SELECT_RANGE		10

CameraController::CameraController(RenderSrv	*pRenderSrv)
{
	m_bMode = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_curX = m_curX = 0;
	m_prevX = m_prevY = 0;
	m_aniType = ANI_NONE;
	m_dTickCount = 0;
	m_pRenderSrv = pRenderSrv;
	m_tiltRotOrg.set(EARTH_RADIUS , 0 , 0);
	m_bFromTilt = false;
	m_blFirstMoveTo = true;

	m_initCameraInfo.m_loc.m_dDist = INIT_DST;
	m_initCameraInfo.m_loc.m_dLongitude = INIT_LONGI * gdm_DEGTORAD;
	m_initCameraInfo.m_loc.m_dLatitude =  INIT_LATI * gdm_DEGTORAD;
	m_initCameraInfo.m_orgDist =gdm_getHeight(INIT_LONGI,INIT_LATI);
	m_initCameraInfo.m_pitch = INIT_PITCH*gdm_DEGTORAD;
	m_initCameraInfo.m_roll = INIT_ROLL*gdm_DEGTORAD;
	m_initCameraInfo.m_yaw=0.0;
	m_sCursorPt.rangeInScreen = GDEM_SELECT_RANGE;
	m_sCursorPt.rangeInWorld = 0;
	m_sCursorPt.altitude = 0;
	m_sCursorPt.blEnable = false;
	m_sCursorPt.dX = 0;
	m_sCursorPt.dY = 0;
	m_sCursorPt.dZ = 0;
	m_sCursorPt.latitude = 0;
	m_sCursorPt.longitude = 0;
	m_sCursorPt.x = 0;
	m_sCursorPt.y = 0;
	m_pt.set(0,0,0);
	m_dAutoRotateTickCount = 0;
}

CameraController::~CameraController()
{
	Release();
}

bool CameraController::Init()
{
	gdm_CameraInit(g_pRender , 1 , 1);
    return true;
}

void CameraController::Release()
{

}

void CameraController::MoveInitPos()
{
	AutoMoveTo(0 , m_initCameraInfo, MOVE_TO_TIME); // reset pitch / roll
}

void CameraController::OnChangedViewSize(int w, int h)
{
	m_nWidth = w;
	m_nHeight = h;
	gdm_CameraSetViewPortSize(g_pRender , w , h);
	m_pRenderSrv->m_pView->OnResizeGL(w, h);
	UpdateTiltRotAxis();
	m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
}

// --------------- Rotate control ---------------------
bool CameraController::GetRotateInfo(int x1 , int y1 , int x2, int y2 , double& fOffsetAngle , CGDM_Vector3DF *pRotAxis)
{
	EndAnimation();
	int x = x2 - x1;
	int y = -(y2 - y1);
	if( x == 0 && y == 0)
		return false;
	CGDM_Vector3DF rotAxis(x , y , 0);
	CGDM_Vector3DF vt(0 , 0 , 1) , vt1 , vt2 , upVt, hitVt;
	GDM_Coord3D coord;
	CGDM_Point3DF pt1(0,0,0) , pt2(0,0,0);
	GDM_ERROR_NO retVal1 , retVal2;
	double fAngle;
	GDM_LOCATION loc;
	double fMaxOffset;
	bool blCameraIn = false;

	gdm_CameraGetCoord(g_pRender , &coord);
	gdm_CameraGetHitVector(g_pRender, x1, y1, &hitVt);
	retVal1 = gdm_HitPoint(coord.m_org, hitVt, m_rad, &pt1);
	if((coord.m_org.getLength() - m_rad) < 0)
	{
		blCameraIn = true;
	}
	if( blCameraIn && GDM_SUCCESS == retVal1)
	{
		y2 = y1 - (y2 - y1);
	}
	gdm_CameraGetHitVector(g_pRender, x2, y2, &hitVt);
	retVal2 = gdm_HitPoint(coord.m_org, hitVt, m_rad, &pt2);
	vt1 = pt1; // - coord.m_org;
	vt2 = pt2; // - coord.m_org;
	
	vt1.normalize();
	vt2.normalize();

	rotAxis = vt1.crossProduct(vt2);
	rotAxis.normalize();

	if(gdm_iszero(rotAxis.getLength()))
	{
		printf("[Camera::GetRotateInfo] rotAxis error1!");
		return false;
	}

	gdm_GetAngleBetweenTwoVector(vt2 , vt1 , rotAxis , fAngle);
#ifdef DEBUG
	qDebug("[GetRotateInfo angle = %f , dx = %d , dy = %d] \n" , fAngle , x2 - x1 , y2 - y1);
#endif	
	*pRotAxis = rotAxis;
	fOffsetAngle = fAngle;

	gdm_CameraGetLocation(g_pRender , &loc);
	fMaxOffset = (loc.m_dDist - gdm_getHeight(loc.m_dLongitude, loc.m_dLatitude));
	if(fMaxOffset < 60)
		fMaxOffset = 60;
	fMaxOffset /= (loc.m_dDist + EARTH_RADIUS);
	if(fabs(fOffsetAngle) > fMaxOffset)
		fOffsetAngle = fOffsetAngle > 0 ? fMaxOffset : -fMaxOffset;

	return true;
}

void CameraController::AutoRotateView(int x1 , int y1 , int x2, int y2, double time)
{
	CGDM_Vector3DF rotAxis;
	CGDM_Point3DF pt1 , pt2;
	double fOffsetAngle , fMaxOffset;
	// rotation angle
	GDM_ROT_ANI_INFO tInfo;
	GDM_LOCATION loc;

	if(!GetRotateInfo(x1 , y1 , x2 , y2 , fOffsetAngle , &rotAxis))
		return;

	gdm_CameraGetLocation(g_pRender , &loc);
	fMaxOffset = loc.m_dDist * AUTO_MAX_SPEED_RATE;
	fMaxOffset /= (loc.m_dDist + EARTH_RADIUS);
	if(fabs(fOffsetAngle) > fMaxOffset)
		fOffsetAngle = fOffsetAngle > 0 ? fMaxOffset : -fMaxOffset;

	tInfo.m_aroundAxis = rotAxis;
	tInfo.m_fOffsetAngle = fOffsetAngle;
	tInfo.m_blAuto = true;
	tInfo.m_hour = AUTO_ROT_TIME;
	gdm_CameraStartAutoRotate(g_pRender , &tInfo);
	StartAnimation(ANI_ROTATE);

	// prevent warning error : must be remove
	fOffsetAngle = time;
}

void CameraController::FreeRotateView(int x1 , int y1 , int x2, int y2)
{
	EndAnimation();
	CGDM_Vector3DF rotAxis;
	CGDM_Point3DF pt1 , pt2;
	double fOffsetAngle;
	if(GetRotateInfo(x1 , y1 , x2 , y2 , fOffsetAngle , &rotAxis))
	{
		gdm_CameraRotate(g_pRender , fOffsetAngle , &rotAxis);
		m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
	}
}

void CameraController::MoveView(int x , int y)
{
	GDM_Coord3D coord;
	GDM_Coord3D orgCoord;
	GDM_LOCATION loc;
	double fOffset, angle;
	double fHeight;
	CGDM_Vector3DF vt;
	GDM_ROT_ANI_INFO tInfo;
	double rate;
	EndAnimation();
	gdm_CameraGetCoord(g_pRender , &coord);
	orgCoord = coord;
	vt = coord.m_org;
	gdm_GetAngleBetweenTwoVector(coord.m_z,vt, coord.m_x, angle);
	gdm_RotateAroundVector(&coord, angle, coord.m_x);
	gdm_CameraGetLocation(g_pRender , &loc);
	fHeight = loc.m_dDist - gdm_getHeight(loc.m_dLongitude, loc.m_dLatitude);
	if(fHeight < 60)
		fHeight = 60;
	rate = fabs(SPEED_RATE * sin(angle));
	if(rate < SPEED_RATE/2.0f)
		rate = SPEED_RATE/2.0f;
	fOffset = fHeight * rate;
	fOffset /= (loc.m_dDist + EARTH_RADIUS);
	vt = coord.m_y * x + coord.m_x * (-y);
	vt.normalize();

	gdm_RotateAroundVector(&orgCoord, fOffset, vt);
	gdm_CameraSetCoord(g_pRender, orgCoord);
	UpdateTiltRotAxis();
	m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
}

// --------------- Move control ---------------------
void CameraController::AutoMoveTo(int pathType , GDM_CAMERA_INFO cameraInfo , double moveTime , unsigned char moveType)
{
	GDM_MOVETO_ANI_INFO tInfo;
	GDM_OPTION option;
	double movetoRate;
	movetoRate = DEFAULT_MOVETO_RATE;
	gdm_GetRenderOption(g_pRender , &option);
	if(option.nav_moveto_rate)
		movetoRate =  movetoRate / option.nav_moveto_rate;
	EndAnimation();
	tInfo.m_pathType = pathType;
	tInfo.m_hour = moveTime;
	tInfo.m_hour *= movetoRate;
	tInfo.m_curhour = 0;
	tInfo.m_dstLocation = cameraInfo;
	tInfo.m_moveType = moveType;
	gdm_CameraStartAutoMoveTo(g_pRender , &tInfo);
	StartAnimation(ANI_MOVETO);
}

void CameraController::MoveForward(int speed , int nFrom)
{
	GDM_MOVE_FORWARD_ANI_INFO tInfo;
	GDM_LOCATION loc;
	double fDistance;
	double fOffset;
	double rate;
	GDM_CAMERA_INFO cameraInfo;
	CGDM_Point3DF targetPt , dstPt , orgPt;
	GDM_Coord3D		coord;
	double speedRate;

	EndAnimation();
	gdm_CameraGetInfo(g_pRender , &cameraInfo);
	loc = cameraInfo.m_loc;
    gdm_CameraGetLocation(g_pRender , &loc);
	fDistance = loc.m_dDist;
	gdm_CameraGetCoord(g_pRender , &coord);
	
	if(nFrom == 0)
		speedRate = SPEED_RATE2 * 1.2;
	else
		speedRate = SPEED_RATE2 * 0.3;

	rate = speedRate * (double)speed / SPEED_STEP;

	if(rate < 0)
	{
		if(rate < -0.9)
			rate = -0.9;
		rate = rate / (1 + rate);
	}
	else if(rate > 0.9)
	{
		rate = 0.9;
	}

	if( GDM_SUCCESS == gdm_HitPoint(coord.m_org , -(coord.m_z), &targetPt))
	{
		fOffset = (coord.m_org - targetPt).getLength() * rate;
	}
	else
	{
		fOffset = fDistance * rate;

		double pitch = 0.5;

		if(!gdm_equals(cameraInfo.m_pitch , gdm_HALF_PI , pitch))
		{
			fOffset /= cos(cameraInfo.m_pitch);
		}
		else
			fOffset /= cos(fabs(gdm_HALF_PI) - pitch);
	}
	if(nFrom != 0)
	{
		coord.m_org = coord.m_org + -coord.m_z * fOffset;
		gdm_CameraSetCoord(g_pRender , coord);
		m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
	}
	else
	{
		tInfo.m_hour = nFrom == 0 ? MOVE_FORWARD_TIME : 1;
		tInfo.m_fOffsetLen = -fOffset;
		tInfo.m_curhour = 0;
		tInfo.m_moveType = nFrom == 0 ? 1 : 0;
		gdm_CameraStartAutoMoveForward(g_pRender , &tInfo);
		StartAnimation(ANI_MOVE_FORWARD);

	}
}

// --------------- Pitch , Roll control ---------------------
void CameraController::FreeRotatePitchRoll(int x , int y , int x1, int y1)
{
	int dx = x1 - x;
	int dy = y1 - y;
	GDM_Coord3D coord , tmpCoord;
	CGDM_Vector3DF rotAxis;
	CGDM_Point3DF pt;
	double offsetRoll , offsetPitch;

	EndAnimation();
	gdm_CameraGetCoord(g_pRender , &coord);
	if(dx)
	{
		rotAxis = -m_pt;
		offsetRoll = gdm_PI * dx / m_nWidth;
		gdm_RotateAroundVector(&coord , offsetRoll , rotAxis);
	}
	if(dy)
	{
		offsetPitch = gdm_PI * dy / m_nHeight;
		tmpCoord = coord;
		gdm_RotateAroundLine(&tmpCoord , offsetPitch , m_pt , coord.m_x);
		if(IsValidCoord(tmpCoord , offsetPitch , 1))
		{
			coord = tmpCoord;			
		}
	}
	gdm_CameraSetCoord(g_pRender , coord);
	// prevent warning error : must be remove
	offsetRoll = x;
	offsetRoll = y;
	m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
}

bool CameraController::IsValidCoord(GDM_Coord3D coord , double offsetPitch , char bMode)
{
	bool blRet;
	GDM_CAMERA_INFO cameraInfo;
	double maxPitch = gdm_HALF_PI;
	if(bMode == 0)
		maxPitch = 1.5;
	gdm_GetCoordInfo(&coord , &cameraInfo , bMode);
	if(offsetPitch > 0 && cameraInfo.m_pitch >= maxPitch)
	{
		blRet = false;
	}
	else if( offsetPitch < 0 && cameraInfo.m_pitch <= 0)
	{
		blRet = false;

	}
	else
	{
		blRet = true;
	}
	return blRet;
}

void CameraController::OffsetOrientation(double offsetPitchAngle, double offsetRollAngle, double offsetYawAngle, char blMode)
{
	GDM_Coord3D coord;
	CGDM_Vector3DF vt;
	GDM_LOCATION loc;
	GDM_CAMERA_INFO cameraInfo;
	EndAnimation();
	if(blMode == 1)
	{
		gdm_CameraGetInfo(g_pRender, &cameraInfo, blMode);
		cameraInfo.m_roll += offsetRollAngle;
		if(cameraInfo.m_roll < 0)
			cameraInfo.m_roll += gdm_TwoPI;
		if(cameraInfo.m_roll > gdm_TwoPI)
			cameraInfo.m_roll -= gdm_TwoPI;
		cameraInfo.m_pitch += offsetPitchAngle;
		if(cameraInfo.m_pitch < 0)
			cameraInfo.m_pitch = 0;
		if(cameraInfo.m_pitch > gdm_HALF_PI)
			cameraInfo.m_pitch = gdm_HALF_PI;
		gdm_GetCoord(&cameraInfo, &coord, blMode);
		gdm_CameraSetCoord(g_pRender, coord);
		UpdateTiltRotAxis();
	}
	else
	{
		m_bFromTilt = true;
		gdm_CameraGetCoord(g_pRender , &coord);
		vt = coord.m_x;
		GDM_Coord3D tempCoord = coord;
		gdm_RotateAroundLine(&tempCoord , offsetPitchAngle , m_tiltRotOrg , vt);
		if(IsValidCoord(tempCoord , offsetPitchAngle , 0) && !gdm_IsPtInDem(tempCoord.m_org))
		{
			coord = tempCoord;
		}
		gdm_RotateAroundVector(&coord , offsetRollAngle , m_tiltRotOrg);
		gdm_CameraSetCoord(g_pRender, coord);
	}
	m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
}
// roll
void CameraController::RotateView(int direct, char blMode)
{
	double fOffset;
	fOffset = LOOKVIEW_OFFSET * gdm_DEGTORAD * direct;
	OffsetOrientation(0, fOffset, 0, blMode);
}

// pitch
void CameraController::TiltView(int direct, char blMode)
{
	double fOffset;
	fOffset = TILT_OFFSET * gdm_DEGTORAD * direct;
	if(blMode == 1)
		fOffset /= 3;
	OffsetOrientation(fOffset, 0, 0, blMode);
}


void CameraController::LookView(int x , int y)
{
	double fOffsetRoll, fOffsetPitch;
	double rate;
	rate =  x / 21.0;
	fOffsetRoll = LOOKVIEW_OFFSET * gdm_DEGTORAD * rate;
	rate =  -y / 21.0;
	fOffsetPitch = LOOKVIEW_OFFSET * gdm_DEGTORAD * rate / 2;
	OffsetOrientation(fOffsetPitch, fOffsetRoll, 0, 1);
}

void CameraController::MousePress(CursorState* mouseEvent)
{
	GDM_ERROR_NO resultVal;
	CGDM_Point3DF pt;
	GDM_Coord3D coord;
	EndAnimation();
	bool	    ret;

	SetCursorPt(mouseEvent);

	m_x = m_curX = mouseEvent->x;
	m_y = m_curY = mouseEvent->y;
	m_prevX = m_x;
	m_prevY = m_y;
	gdm_CameraGetCoord(g_pRender , &coord);

	ret = GetCursorPt(pt);

	if(mouseEvent->buttons & MBS_MID)
	{
	
		if( ret)
		{
			m_bMode = 2; // pitch & roll
		}
	}
	else
	{
		m_bMode = 1; // free rotate
	}

	if(m_bMode != 2)
	{
		GDM_CAMERA_INFO cameraInfo , cameraInfo1;
		gdm_CameraGetInfo(g_pRender , &cameraInfo);
		if(cameraInfo.m_pitch == gdm_HALF_PI)
		{
			gdm_GetCoordInfo(&coord , &cameraInfo1 , 1);
			if(cameraInfo1.m_pitch <= gdm_HALF_PI * CAMERA_TILT_LIMIT)
			{
				gdm_GetCoord(&cameraInfo , &coord);
				gdm_CameraSetCoord(g_pRender , coord);
			}
		}

	}
	m_pt = pt;

	if(ret)
	{
		m_rad = m_pt.getLength();
	}
	else
	{
		m_rad = EARTH_RADIUS;
	}
}
void CameraController::MouseMove(CursorState* mouseEvent)
{
	m_prevX = m_curX;
	m_prevY = m_curY;
	if(m_bMode == 1)
	{
		FreeRotateView(m_curX , m_curY , mouseEvent->x , mouseEvent->y);
		m_dAutoRotateTickCount = gdm_GetTickTimer();
	}
	else if(m_bMode == 2)
	{
		FreeRotatePitchRoll(m_curX , m_curY , mouseEvent->x , mouseEvent->y);
	}
	m_curX = mouseEvent->x;
	m_curY = mouseEvent->y;
}
void CameraController::MouseRelease(CursorState* mouseEvent)
{
	if(m_bMode == 1)
	{
		int dx = mouseEvent->x - m_prevX;
		int dy = mouseEvent->y - m_prevY;
		long curTickCount = gdm_GetTickTimer();
		long delta = curTickCount - m_dAutoRotateTickCount;
		if( delta < 100 && (abs(dx) > 1 || abs(dy) > 1))
		{
			AutoRotateView(m_prevX , m_prevY , mouseEvent->x , mouseEvent->y , 1);
		}
	}
	UpdateTiltRotAxis();
	m_bMode = 0;
}
void CameraController::MouseDblClick(CursorState* mouseEvent)
{
    CGDM_Point3DF targetPt , dstPt , orgPt , orgVt;
	SetCursorPt(mouseEvent);

	if(GetCursorPt(targetPt))
	{
		GDM_CAMERA_INFO cameraInfo;
		CGDM_Vector3DF rotAxis , rotAxis1;
		GDM_Coord3D		coord;
		double rate;
		double dst;
		memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
		gdm_CameraGetCoord(g_pRender , &coord);
		orgVt = -coord.m_z;
		orgPt = coord.m_org;
		gdm_CameraGetInfo(g_pRender , &cameraInfo);

		rate = SPEED_RATE1;

		if(mouseEvent->buttons & MBS_RIGHT)
		{
			rate = -rate/(1-rate);
		}
		dst = (targetPt - orgPt).getLength();
		if(cameraInfo.m_pitch < gdm_HALF_PI)
		{
			GDM_LOCATION loc;
			gdm_GetLocation(&targetPt , &loc);
			cameraInfo.m_loc.m_dLongitude = loc.m_dLongitude;
			cameraInfo.m_loc.m_dLatitude = loc.m_dLatitude;
			cameraInfo.m_loc.m_dDist = dst * (1 - rate);
			cameraInfo.m_orgDist = (double) gdm_getHeight(loc.m_dLongitude , loc.m_dLatitude);
		}
		else
		{
			dstPt.m_tX = orgPt.m_tX * (1 - rate) + targetPt.m_tX * rate;
			dstPt.m_tY = orgPt.m_tY * (1 - rate) + targetPt.m_tY * rate;
			dstPt.m_tZ = orgPt.m_tZ * (1 - rate) + targetPt.m_tZ * rate;
			coord.m_org = dstPt;
			gdm_GetCoordInfo(&coord , &cameraInfo);
		}
		AutoMoveTo(1 , cameraInfo , MOVE_TO_BY_MOUSE_TIME); // unchange roll , pitch
	}
}

void CameraController::GetStatusInfo(StatusBarInfo* pInfo)
{
	CGDM_Point3DF pt;
	GDM_LOCATION loc , cameraLoc;
	GDM_DIRECTION_INFO rateDirInfo;


	gdm_CameraGetLocation(g_pRender , &cameraLoc);

	// reset cursor pt for moving state
	SetCursorPt(m_sCursorPt.x, m_sCursorPt.y);

	// get cursor information
	if(m_sCursorPt.blEnable)
	{
		pInfo->latitude = m_sCursorPt.latitude;
		pInfo->longitude = m_sCursorPt.longitude;
		pInfo->distance = m_sCursorPt.altitude / gdm_GetDataMgr()->m_sOption.dem_detail_rate;
	}
	gdm_GetFSP(g_pRender , pInfo->frames, pInfo->real_frames, pInfo->meshCount, pInfo->lineCount);
	pInfo->height = cameraLoc.m_dDist;

	// get rate /direction angle
	rateDirInfo = gdm_getRateInfo(m_sCursorPt.longitude, m_sCursorPt.latitude);
	pInfo->rate_angle = rateDirInfo.angle;
	pInfo->dir_angle  = (rateDirInfo.direction - 90);
	if(pInfo->dir_angle < 0)
		pInfo->dir_angle += 360;

	ResetSelectRange();
}

void CameraController::StartAnimation(GDM_ANIMATION_TYPE aniType)
{
	EndAnimation();
	m_dTickCount = gdm_GetTickTimer();
	m_aniType = aniType;
	m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_START_ANIMATION, m_aniType);
}

void CameraController::EndAnimation()
{
	if(m_aniType)
	{
		m_dTickCount = 0;
		m_aniType = ANI_NONE;
		if(!m_bFromTilt)
			UpdateTiltRotAxis();
		m_bFromTilt = false;
	}
}
void CameraController::OnAnimation()
{
	static int cameraChangedNotifyCount = 0;
	if(m_aniType)
	{
		double tickCount = gdm_GetTickTimer();
		double deltaHour = tickCount - m_dTickCount;
		if(deltaHour > 0)
		{
			if(gdm_CameraOnAnimation(g_pRender , deltaHour) != GDM_SUCCESS)
			{
				// Animation이 끝났음을 통보
				m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_END_ANIMATION, m_aniType);
				m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
				EndAnimation();
			}
			else
			{
				m_dTickCount = tickCount;
				cameraChangedNotifyCount++;
				if(cameraChangedNotifyCount > 0)
				{
					cameraChangedNotifyCount = 0;
					m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
				}
			}
		}
	}
}

void CameraController::ResetTilt()
{
	GDM_CAMERA_INFO cameraInfo;
	GDM_MOVETO_ANI_INFO tInfo;
	EndAnimation();
	gdm_CameraGetInfo(g_pRender , &cameraInfo);
	cameraInfo.m_pitch = 0;
	tInfo.m_dstLocation = cameraInfo;
	tInfo.m_pathType = 1;
	tInfo.m_hour = MOVE_TO_BY_RESET;
	tInfo.m_curhour = 0;
	tInfo.m_moveType = 1;
	m_bFromTilt = true;
	gdm_CameraStartAutoMoveTo(g_pRender , &tInfo);
	StartAnimation(ANI_MOVETO);
}

void CameraController::ResetAll()
{
	GDM_CAMERA_INFO cameraInfo;
	GDM_MOVETO_ANI_INFO tInfo;
	EndAnimation();
	gdm_CameraGetInfo(g_pRender , &cameraInfo);
	cameraInfo.m_pitch = cameraInfo.m_roll = cameraInfo.m_yaw = 0;
	tInfo.m_dstLocation = cameraInfo;
	tInfo.m_pathType = 1;
	tInfo.m_hour = MOVE_TO_BY_RESET;
	tInfo.m_curhour = 0;
	tInfo.m_moveType = 1;
	m_bFromTilt = true;
	gdm_CameraStartAutoMoveTo(g_pRender , &tInfo);
	StartAnimation(ANI_MOVETO);
}

void CameraController::StopView()
{
	EndAnimation();
}

void CameraController::ResetView()
{
	GDM_CAMERA_INFO cameraInfo;
	GDM_MOVETO_ANI_INFO tInfo;
	EndAnimation();
	gdm_CameraGetInfo(g_pRender , &cameraInfo);
	cameraInfo.m_roll = cameraInfo.m_yaw = 0;
	tInfo.m_dstLocation = cameraInfo;
	tInfo.m_pathType = 1;
	tInfo.m_hour = MOVE_TO_BY_RESET;
	tInfo.m_curhour = 0;
	tInfo.m_moveType = 1;
	m_bFromTilt = true;
	gdm_CameraStartAutoMoveTo(g_pRender , &tInfo);
	StartAnimation(ANI_MOVETO);
}

void CameraController::ChangedCompassAngle(double* angle)
{
	GDM_CAMERA_INFO cameraInfo;
	double resultAngle;
	GDM_LOCATION loc;
	GDM_Coord3D coord;
	double offsetAngle;
	EndAnimation();
	gdm_CameraGetInfo(g_pRender , &cameraInfo);
	resultAngle = cameraInfo.m_roll;
	resultAngle = -(*angle);
	offsetAngle = resultAngle - cameraInfo.m_roll;
	OffsetOrientation(0, offsetAngle, 0, 0);
	m_pRenderSrv->GetCommandMgr()->OnNotifyEvent(GDM_RENDER_NITIFY_CAMERA_CHANGED);
}

void CameraController::GetCompassAngle(double* angle)
{
	GDM_CAMERA_INFO cameraInfo;
	gdm_CameraGetInfo(g_pRender , &cameraInfo , 1);
	*angle = -cameraInfo.m_roll;
}

bool CameraController::IsSamePlace(GDM_CAMERA_INFO info1 , GDM_CAMERA_INFO info2)
{
	double PosTal = 1E-8;
	double DistTal = 1;
	double angleTal = 1E-5;
	if(!gdm_equals(info1.m_loc.m_dLatitude , info2.m_loc.m_dLatitude , PosTal))
		return false;
	if(!gdm_equals(info1.m_loc.m_dLatitude , info2.m_loc.m_dLatitude , PosTal))
		return false;
	if(!gdm_equals(info1.m_loc.m_dDist , info2.m_loc.m_dDist , DistTal))
		return false;
	if(!gdm_equals(info1.m_orgDist , info2.m_orgDist , DistTal))
		return false;
	if(!gdm_equals(info1.m_pitch , info2.m_pitch , angleTal))
		return false;
	if(!gdm_equals(info1.m_roll , info2.m_roll , angleTal))
		return false;
	if(!gdm_equals(info1.m_yaw , info2.m_yaw , angleTal))
		return false;
	return true;
}
void CameraController::MoveToBound(CGDM_Rect2D bound)
{
	double minVal = 15 / EARTH_RADIUS;
	if( bound.getWidth() < minVal)
	{
		bound.m_cMinPt.m_tX -= minVal;
		bound.m_cMaxPt.m_tX += minVal;
	}
	if( bound.getHeight() < minVal)
	{
		bound.m_cMinPt.m_tY -= minVal;
		bound.m_cMaxPt.m_tY += minVal;
	}
	// find serveral object
	PLACEMARK_DATA sendData = {0};
	GDM_LOCATION ltLoc = {0} , rbLoc = {0};
	GDM_Coord3D coord;
	GDM_CAMERA_INFO cameraInfo;
	ltLoc.m_dLongitude = bound.m_cMinPt.m_tX;
	ltLoc.m_dLatitude = bound.m_cMinPt.m_tY;
	rbLoc.m_dLongitude = bound.m_cMaxPt.m_tX;
	rbLoc.m_dLatitude = bound.m_cMaxPt.m_tY;
	gdm_GetOptimzeCameraCoord(ltLoc, rbLoc, &coord);
	gdm_GetCoordInfo(&coord, &cameraInfo);
	sendData.snapshot = true;
	sendData.view_longitude = cameraInfo.m_loc.m_dLongitude;
	sendData.view_latitude = cameraInfo.m_loc.m_dLatitude;
	sendData.view_range = cameraInfo.m_loc.m_dDist;
	MoveToSnapshot(&sendData);
}

void CameraController::MoveToSnapshot(PLACEMARK_DATA* placeData)
{
	GDM_CAMERA_INFO curCameraInfo;
	GDM_CAMERA_INFO cameraInfo;
	memset(&curCameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
	if(placeData)
	{
		gdm_CameraGetInfo(g_pRender , &curCameraInfo);
		cameraInfo.m_loc.m_dDist = INIT_DST;
		cameraInfo.m_loc.m_dLongitude = placeData->view_longitude;
		cameraInfo.m_loc.m_dLatitude =  placeData->view_latitude;
		cameraInfo.m_loc.m_dDist =  GetMoveToHeight(placeData);
		cameraInfo.m_orgDist = gdm_getHeight(placeData->view_longitude , placeData->view_latitude);
		cameraInfo.m_pitch = placeData->view_tilt;
		cameraInfo.m_roll = placeData->view_heading;
		cameraInfo.m_yaw = 0;
		if(!m_blFirstMoveTo)
		{
			if(!IsSamePlace(curCameraInfo , cameraInfo))
				AutoMoveTo(0 , cameraInfo , MOVE_TO_TIME); // reset pitch / roll
		}
		else
			m_initCameraInfo = cameraInfo;
	}
	
	m_blFirstMoveTo = false;
}


void CameraController::MoveToPlace(PLACEMARK_DATA* placeData)
{
	if(placeData == NULL || placeData->snapshot)
	{
		MoveToSnapshot(placeData);
		return;
	}
	else
	{
		CGDM_Rect2D bound;
		CGIS_Node *pNode = gdm_GetGISDoc()->GetNodeFromKey(placeData->key);
		if(pNode)
		{
			bound = pNode->m_cBound;
			MoveToBound(bound);
		}
	}
}

void CameraController::FastMoveToPlace(PLACEMARK_DATA* placeData)
{
	GDM_CAMERA_INFO curCameraInfo;
	GDM_CAMERA_INFO cameraInfo;
	memset(&curCameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
	gdm_CameraGetInfo(g_pRender , &curCameraInfo);
	cameraInfo.m_loc.m_dDist = INIT_DST;
	cameraInfo.m_loc.m_dLongitude = placeData->view_longitude;
	cameraInfo.m_loc.m_dLatitude =  placeData->view_latitude;
	cameraInfo.m_loc.m_dDist =  GetMoveToHeight(placeData);
	if(placeData->orgDist == PLACEMARK_DEMHEIGHT_INVALID)
		cameraInfo.m_orgDist = gdm_getHeight(cameraInfo.m_loc.m_dLongitude, cameraInfo.m_loc.m_dLatitude);
	else
		cameraInfo.m_orgDist = placeData->orgDist;
	cameraInfo.m_pitch = placeData->view_tilt;
	cameraInfo.m_roll = placeData->view_heading;
	cameraInfo.m_yaw = 0;

	if(!IsSamePlace(curCameraInfo , cameraInfo))
		AutoMoveTo(2 , cameraInfo , FAST_MOVE_TO_TIME); // reset pitch / roll
}

double CameraController::GetMoveToHeight(PLACEMARK_DATA* placeData)
{
	return placeData->view_range;
}

bool CameraController::HitPoint(int x , int y , CGDM_Point3DF* pPt, bool isDem /* = true*/)
{
	if(m_sCursorPt.x == x && m_sCursorPt.y == y)
	{
		GetCursorPt(*pPt);
		return m_sCursorPt.blEnable;
	}
	CGDM_Vector3DF hitVt;
	GDM_Coord3D coord;
	GDM_ERROR_NO retVal;
	gdm_CameraGetCoord(g_pRender , &coord);
	gdm_CameraGetHitVector(g_pRender, x , y, &hitVt);
	retVal = gdm_HitPoint(coord.m_org, hitVt, pPt, false/*isDem*/);

	if(retVal == GDM_SUCCESS)
		return	true;

	return	false;
}

void CameraController::UpdateTiltRotAxis()
{
	GDM_Coord3D coord;
	CGDM_Point3DF pt;
	CGDM_Vector3DF vt , vt1 , vt2;
	GDM_ViewFrustum frustum;
	GDM_CAMERA_INFO cameraInfo;
	gdm_CameraGetCoord(g_pRender , &coord);
	vt = -coord.m_z;
	if(GDM_SUCCESS != gdm_HitPoint(coord.m_org , vt, &pt))
	{
		gdm_GetCoordInfo(&coord , &cameraInfo , 1);
		if(cameraInfo.m_pitch > gdm_HALF_PI * CAMERA_TILT_LIMIT)
		{
			vt1 = pt - coord.m_org;
			vt1.normalize();
			gdm_CameraGetFrustumInfo(g_pRender , frustum);
			gdm_RotateAroundVector(&vt , -frustum.m_dFovy / 2 , coord.m_x);
			vt2 = vt + vt1;
			if(GDM_SUCCESS != gdm_HitPoint(coord.m_org , vt2, &pt))
			{
				pt = pt;
			}
		}
	}
	m_tiltRotOrg = pt;
	
}

void CameraController::SetCursorPt(CursorState* mouseEvent)
{
	SetCursorPt(mouseEvent->x, mouseEvent->y);
}

void CameraController::SetCursorPt(int x, int y)
{
	CGDM_Location3D pt;

	m_sCursorPt.x = x;
	m_sCursorPt.y = y;

	m_sCursorPt.blEnable = (gdm_GetHitLocation(m_sCursorPt.x, m_sCursorPt.y, &pt) == GDM_SUCCESS); 

	m_sCursorPt.longitude	= pt.m_tX;
	m_sCursorPt.latitude	= pt.m_tY;
	m_sCursorPt.altitude	= pt.m_tZ;

	double dRadius, dXRadius;

	dRadius	   = EARTH_RADIUS + m_sCursorPt.altitude;

	dXRadius = dRadius * cos(m_sCursorPt.latitude);
	m_sCursorPt.dZ = dRadius * sin(m_sCursorPt.latitude);
	m_sCursorPt.dX = dXRadius * cos(m_sCursorPt.longitude);
	m_sCursorPt.dY = dXRadius * sin(m_sCursorPt.longitude);
}

void CameraController::ResetSelectRange()
{
	double range;

	m_sCursorPt.rangeInScreen = GDEM_SELECT_RANGE;
	m_sCursorPt.rangeInWorld  = 0;

	GDM_ERROR_NO result1 , result2;
	CGDM_Point3DF startPt , endPt;
	CGDM_Vector3DF hitVt1, hitVt2;
	GDM_Coord3D coord;
	GDM_ERROR_NO retVal;
	gdm_CameraGetCoord(g_pRender , &coord);
	gdm_CameraGetHitVector(g_pRender, m_sCursorPt.x , m_sCursorPt.y, &hitVt1);
	gdm_CameraGetHitVector(g_pRender, m_sCursorPt.x + m_sCursorPt.rangeInScreen , m_sCursorPt.y + m_sCursorPt.rangeInScreen, &hitVt2);

	result1 = gdm_HitPoint(coord.m_org, hitVt1, &startPt, false);
	result2 = gdm_HitPoint(coord.m_org, hitVt2, &endPt, false);
	if(result1 == GDM_SUCCESS && result2 == GDM_SUCCESS)
	{
		GDM_LOCATION startLoc , endLoc;
		gdm_GetLocation(&startPt , &startLoc);
		gdm_GetLocation(&endPt , &endLoc);

		range = fabs(startLoc.m_dLongitude - endLoc.m_dLongitude);
		if(range < fabs(startLoc.m_dLongitude - endLoc.m_dLongitude))
			range = fabs(startLoc.m_dLongitude - endLoc.m_dLongitude);
		m_sCursorPt.rangeInWorld = range;
	}
}

bool CameraController::GetCursorPt(CGDM_Point3DF &pt)
{
	if(!m_sCursorPt.blEnable)
		return false;

	pt.m_tX = m_sCursorPt.dX;
	pt.m_tY = m_sCursorPt.dY;
	pt.m_tZ = m_sCursorPt.dZ;

	return true;
}

void CameraController::SetTopViewState(double longi,double lat)
{
	GDM_CAMERA_INFO cameraInfo;
	GDM_MOVETO_ANI_INFO tInfo;
	EndAnimation();

	gdm_CameraGetInfo(g_pRender , &cameraInfo);
	cameraInfo.m_loc.m_dLongitude=longi;
	cameraInfo.m_loc.m_dLatitude=lat;	
	cameraInfo.m_pitch = cameraInfo.m_roll = cameraInfo.m_yaw = 0;
	tInfo.m_dstLocation = cameraInfo;
	tInfo.m_pathType = 1;
	tInfo.m_hour = MOVE_TO_BY_RESET;
	tInfo.m_curhour = 0;
	tInfo.m_moveType = 1;
	m_bFromTilt = true;
	gdm_CameraStartAutoMoveTo(g_pRender , &tInfo);
	StartAnimation(ANI_MOVETO);
}

void CameraController::CheckRequest()
{
	if(m_aniType!=ANI_NONE)
		return;

	gdm_CheckRequest();
}
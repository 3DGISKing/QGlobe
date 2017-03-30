
#include "GDM_Camera.h"
#include "GDM_Define.h"
#include "GDM_Coord3D.h"
#include "GDM_CollisionHandler.h"
#include "GDM_PyramidMgr.h"
#include "GDM_DataMgr.h"



//	[2009.08.19][CameraMan][NearFar]
#define CAMERA_NEAR		1.0
#define CAMERA_FAR		1E+10
#define CAMERA_FOVY		50 * gdm_DEGTORAD
#define	MAX_CAMERA_DIST	10 * EARTH_RADIUS

void gdm_GetStandardCoord(GDM_LOCATION *pLoc , GDM_Coord3D *pCoord)
{
	gdm_GetPosition(pLoc , &pCoord->m_org);
	CGDM_Vector3DF vx , vy , vz;
	CGDM_Vector3DF vt(0 , 0 , 1);
	double scalar;
	vz = pCoord->m_org;
	vz.normalize();
	scalar = vz.dotProduct(vt);
	if(gdm_equals(scalar , 1.0 , 0.0001))
	{
		vx = CGDM_Vector3DF(0, 1 , 0);// same with world y
	}
	else
	{
		vx = vt.crossProduct(vz);
		vx.normalize();
	}
	vy = vz.crossProduct(vx).normalize();
	pCoord->m_x = vx;
	pCoord->m_y = vy;
	pCoord->m_z = vz;
}

void gdm_GetCoord(GDM_CAMERA_INFO *pCameraInfo , GDM_Coord3D *pCoord , char bMode )
{
	GDM_Coord3D coord;
	CGDM_Point3DF org;
	CGDM_Vector3DF vt;
	GDM_LOCATION loc;
	double height = 0;
	if(bMode == 0)
	{
		loc = pCameraInfo->m_loc;
		loc.m_dDist += pCameraInfo->m_orgDist;
		gdm_GetStandardCoord(&loc, &coord);
		org.set(0 , 0 , 0);
		vt = coord.m_org;
		gdm_RotateAroundLine(&coord , pCameraInfo->m_roll , org , vt);

		org = coord.m_org;
//		loc.m_dDist = pCameraInfo->m_orgDist;
//		gdm_GetPosition(&loc , &org);
		org = coord.m_org - coord.m_z * pCameraInfo->m_loc.m_dDist;
		
		vt = coord.m_x;
		gdm_RotateAroundLine(&coord , pCameraInfo->m_pitch , org , vt);
		*pCoord = coord;
	}
	else
	{
		loc = pCameraInfo->m_loc;
		loc.m_dDist += pCameraInfo->m_orgDist;
		gdm_GetStandardCoord(&loc , &coord);
		org.set(0 , 0 , 0);
		vt = coord.m_org;
		gdm_RotateAroundLine(&coord , pCameraInfo->m_roll , org , vt);

		org = coord.m_org;
		vt = coord.m_x;
		gdm_RotateAroundLine(&coord , pCameraInfo->m_pitch , org , vt);
		*pCoord = coord;
	}
}

void gdm_GetCoordInfo(GDM_Coord3D* pCoord , GDM_CAMERA_INFO *pCameraInfo , char bMode)
{
	GDM_Coord3D standCoord;
	CGDM_Vector3DF vt , vt1;
	CGDM_Point3DF pt;
	double len;
	GDM_ERROR_NO retVal;
	if(bMode == 0)
	{
		retVal = gdm_HitPoint(pCoord->m_org , -(pCoord->m_z) , &pt);
		vt1 = pCoord->m_org - pt;
		len = fabs(vt1.dotProduct(pCoord->m_z));
		pt = pCoord->m_org + -pCoord->m_z * len;
		gdm_GetLocation(&pt , &pCameraInfo->m_loc);
		gdm_GetStandardCoord(&pCameraInfo->m_loc , &standCoord);
		if(retVal == GDM_SUCCESS)
		{
			pCameraInfo->m_orgDist = (double)pCameraInfo->m_loc.m_dDist; // gdm_getHeight(pCameraInfo->m_loc.m_dLongitude , pCameraInfo->m_loc.m_dLatitude);
			pCameraInfo->m_loc.m_dDist = len;
			vt = pCoord->m_x;
			gdm_GetAngleBetweenTwoVector(standCoord.m_z , vt1 , vt , pCameraInfo->m_pitch);
		}
		else
		{
			pCameraInfo->m_loc.m_dDist = len;
			pCameraInfo->m_orgDist = 0;
			pCameraInfo->m_pitch = gdm_HALF_PI;
		}
		vt = standCoord.m_z;
		gdm_GetAngleBetweenTwoVector(standCoord.m_x , pCoord->m_x , vt , pCameraInfo->m_roll);
		pCameraInfo->m_yaw = 0;
	}
	else
	{

		gdm_GetLocation(&pCoord->m_org , &pCameraInfo->m_loc);
		gdm_GetStandardCoord(&pCameraInfo->m_loc , &standCoord);
		vt = pCoord->m_x;
		gdm_GetAngleBetweenTwoVector(standCoord.m_z , pCoord->m_z , vt , pCameraInfo->m_pitch);
		vt = pCoord->m_org;
		vt.normalize();
		gdm_GetAngleBetweenTwoVector(standCoord.m_x , pCoord->m_x , vt , pCameraInfo->m_roll);
		pCameraInfo->m_yaw = 0;
		pCameraInfo->m_orgDist = 0;
	}
}

CGDM_Camera::CGDM_Camera(void)
{
	m_pRotAnimationHandler = new CGDM_CameraRotAnimationHandler(this);
	m_pMoveToAnimationHandler = new CGDM_CameraMoveToAnimationHandler(this);
	m_pMoveForwardAnimationHandler = new CGDM_CameraMoveForwardAnimationHandler(this);
	m_anyType = ANI_NONE;
	m_fScale  = 1.0f;
	m_fScaleW = 1.0f;
	m_fScaleH = 1.0f;
	m_fScalarOfDir = 1.0;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bEnabled = false;
}

CGDM_Camera::~CGDM_Camera(void)
{
	if(m_pRotAnimationHandler)
		delete m_pRotAnimationHandler;
	if(m_pMoveToAnimationHandler)
		delete m_pMoveToAnimationHandler;
	if(m_pMoveForwardAnimationHandler)
		delete m_pMoveForwardAnimationHandler;
}

void CGDM_Camera::Initialize(int nWidth , int nHeight)
{
	// View Frustum
	m_frustum.m_dNear = CAMERA_NEAR;
	m_frustum.m_dFar =  CAMERA_FAR;
	m_frustum.m_dFovy = CAMERA_FOVY;
	m_frustum.m_dAspect = ((double)nWidth) / nHeight;

	GDM_LOCATION loc;

	loc.m_dDist = EARTH_RADIUS * 3;
	loc.m_dLatitude = gdm_DEGTORAD * 0 ;
	loc.m_dLongitude = gdm_DEGTORAD * 0;

	SetCameraLocation(&loc);

	Update();
}

//set camera's location from B,L,H 
//and then set camera coordinate 

void CGDM_Camera::SetCameraLocation(GDM_LOCATION *pLoc)
{	
	m_location = *pLoc;
	
	GDM_Coord3D coord;

	gdm_GetStandardCoord(pLoc , &coord);
	SetCameraCoord(&coord);
}

//set camera coordinates considering collision

bool CGDM_Camera::SetCameraCoord(GDM_Coord3D* pCoord , bool blCheckCollision)
{
	double dist = pCoord->m_org.getLength();

	if(dist > MAX_CAMERA_DIST)
		return false;

	if (blCheckCollision)
		g_pGDMDataMgr->m_pCollisionHandler->ValidatePos(&pCoord->m_org);

	if(IsSameCoord( &m_coord, pCoord))
		return false;

	m_coord = *pCoord;

	Update();             //???
	return true;
}

//set screen width and height and determine frustum 's aspect and fScale,m_fScaleW,m_fScaleH

GDM_ERROR_NO CGDM_Camera::SetViewPortSize(int w , int h)
{
	m_nScreenWidth = w;
	m_nScreenHeight = h;

	m_frustum.m_dAspect =  ((double)w) / h;

	double height = 2 * m_frustum.m_dNear * tan(m_frustum.m_dFovy / 2);
	double width = height * m_frustum.m_dAspect;
	
	m_fScaleW = (m_nScreenWidth - 0.75f) / width;   //???
	m_fScaleH = (m_nScreenHeight - 0.75f) / height; //???

	m_fScale =qMax(m_fScaleW,m_fScaleH);

	return GDM_SUCCESS;
}


void CGDM_Camera::Update()
{
	//1 determine camera location
	gdm_GetLocation(&m_coord.m_org , &m_location);

	g_pGDMDataMgr->m_pPyramidMgr->Update(); //???

	//2 update frustum 's near and far
	
	m_frustum.m_dNear = ComputeNearClipDistance();
	m_frustum.m_dFar  = ComputeFarClipDistance();

	//3 update m_fScale,m_fScaleW,m_fScaleW
	UpdateScaleParamerters();

	//4 determine m_fScalarOfDir
	// set scalar value of direction

	CGDM_Vector3DF org;

	org = m_coord.m_org;
	org.normalize();

	m_fScalarOfDir = fabs(org.dotProduct(m_coord.m_z));

	CalculateViewRange();
	g_pGDMDataMgr->SetModifyFlag();   //???
}

double CGDM_Camera::ComputeNearClipDistance()
{
	double SurfaceHeight = gdm_getMaxHeight(m_location.m_dLongitude, m_location.m_dLatitude);
	
	//original mode
	//double near = ((m_location.m_dDist - SurfaceHeight) * cos(m_frustum.m_dFovy * 1.2 / 2)) / 10.0;

	double near=(m_location.m_dDist - SurfaceHeight)*cos(m_frustum.m_dFovy/2)/2;

	if( near < CAMERA_NEAR)
		near = CAMERA_NEAR;

	if(m_location.m_dDist < SurfaceHeight)
		near = CAMERA_NEAR;

	return near;
}

double CGDM_Camera::ComputeFarClipDistance()
{
	/* original mode
	CGDM_Vector3DF vt;

	vt = -m_coord.m_z;

	CGDM_Point3DF crossPt , orgPt(0 , 0 , 0);

	gdm_GetPerpendPt(&crossPt, &orgPt , &m_coord.m_org , &vt);

	double lenToCenter = (crossPt - m_coord.m_org).getLength();

    return lenToCenter * 2;*/

	double h=m_location.m_dDist;

	double far=sqrt(2*EARTH_RADIUS*h+h*h);

	return far;
}

void CGDM_Camera::UpdateScaleParamerters()
{
	double height = 2 * m_frustum.m_dNear * tan(m_frustum.m_dFovy / 2);
	double width = height * m_frustum.m_dAspect;

	m_fScaleW = (m_nScreenWidth - 0.75f) / width;   //???
	m_fScaleH = (m_nScreenHeight - 0.75f) / height; //???

	m_fScale =qMax(m_fScaleW,m_fScaleH);
}

void CGDM_Camera::CalculateViewRange()
{
	double angle;
	double radius = EARTH_RADIUS; 
	double dCameraDist = m_location.m_dDist + radius;
	double fovy = m_frustum.m_dFovy * 1.7;
	double dirAngle;

	// calculate angle
	angle = acos(radius / dCameraDist);

	if(m_fScalarOfDir > 1.0) //???
		dirAngle = 0;
	else
		dirAngle = acos(m_fScalarOfDir);

	if((dirAngle + fovy / 2.0) < (gdm_HALF_PI - angle))
	{
		dirAngle = dirAngle + fovy / 2.0;
		double angTmp = dCameraDist / radius * sin(dirAngle);
		if(angTmp > 1.0)
			angTmp = 1.0;
		angTmp = asin(angTmp);
		angTmp = gdm_HALF_PI + (gdm_HALF_PI - angTmp);
		angle = gdm_PI - angTmp - dirAngle;
	}

	double dMinLati = m_location.m_dLatitude - angle;
	double dMaxLati = m_location.m_dLatitude + angle;

	if(dMaxLati >  gdm_HALF_PI)
		dMaxLati = gdm_HALF_PI;
	if(dMinLati < (-gdm_HALF_PI))
		dMinLati = -gdm_HALF_PI;

	// calulate longititude
	double longi = m_location.m_dLongitude;
	double radius1 = radius * cos(m_location.m_dLatitude);
	double radius2 = radius * angle;
	double dMinLongi, dMidMinLongi;
	double dMidMaxLongi, dMaxLongi;

	if(radius1 <= radius2)
	{
		dMinLongi = -gdm_PI;
		dMaxLongi = gdm_PI;
		dMidMinLongi = dMaxLongi;
		dMidMaxLongi = dMaxLongi;
	}
	else
	{
		angle = asin(radius2 / radius1);
		if(longi + angle > gdm_PI)
		{
			dMinLongi = -gdm_PI;
			dMaxLongi = gdm_PI;
			dMidMinLongi = longi + angle - (2 * gdm_PI);
			dMidMaxLongi = longi - angle;
		}
		else if(longi - angle < (-gdm_PI))
		{
			dMinLongi = -gdm_PI;
			dMaxLongi = gdm_PI;
			dMidMinLongi = longi + angle;
			dMidMaxLongi = longi - angle + (2 * gdm_PI);
		}
		else
		{
			dMinLongi = longi - angle;
			dMaxLongi = longi + angle;
			dMidMinLongi = dMaxLongi;
			dMidMaxLongi = dMaxLongi;
		}
	}

	m_dMinLong1 = dMinLongi;
	m_dMaxLong1 = dMidMinLongi;	
	m_dMinLong2 = dMidMaxLongi;	
	m_dMaxLong2 = dMaxLongi;
	m_dMinLati = dMinLati;
	m_dMaxLati = dMaxLati;
}

//get camera coordinates
void CGDM_Camera::GetCameraCoord(GDM_Coord3D* pCoord)
{
	*pCoord = m_coord;
}

bool CGDM_Camera::LookAt(CGDM_Point3DF* pOrg , CGDM_Point3DF* pTargetPos, CGDM_Vector3DF* pUpVt)
{
	GDM_Coord3D coord;
	coord.m_org = *pOrg;
	coord.m_z = (-(*pTargetPos - *pOrg)).normalize();
	coord.m_y = pUpVt->normalize();
	coord.m_x = coord.m_y.crossProduct(coord.m_z);
	SetCameraCoord(&coord);
	return true;
}

void CGDM_Camera::SetInfo(GDM_CAMERA_INFO *pCameraInfo, char bMode)
{
	GDM_Coord3D coord;
	gdm_GetCoord(pCameraInfo , &coord, bMode);
	SetCameraCoord(&coord);
}

void CGDM_Camera::GetInfo(GDM_CAMERA_INFO *pCameraInfo, char bMode)
{
	gdm_GetCoordInfo(&m_coord, pCameraInfo, bMode);
}

//get vector representing view direction i.e Z axis
void CGDM_Camera::GetViewVector(CGDM_Vector3DF* pVt)
{
	GDM_Coord3D coord;
	GetCameraCoord(&coord);
	*pVt = coord.m_z;
}

GDM_ERROR_NO CGDM_Camera::WorldToScreen(INOUT CGDM_Vector3DF* pPt)
{
	gdm_SCStoTCS(&m_coord , pPt);
	double depth = fabs(pPt->m_tZ);

	if(gdm_equals(depth , 0.0 , gdm_ROUNDING_ERROR_32))
		depth = gdm_ROUNDING_ERROR_32;

	pPt->m_tX = pPt->m_tX * m_frustum.m_dNear / depth;
	pPt->m_tY = pPt->m_tY * m_frustum.m_dNear / depth;
	CameraToScreen(pPt);
	if(pPt->m_tZ >= -m_frustum.m_dNear)
	{
		pPt->m_tZ = -1;
		return GDM_FAIL;
	}
	else
		pPt->m_tZ = 0;
	return GDM_SUCCESS;
}

GDM_ERROR_NO CGDM_Camera::ScreenToCamera(double* pScalar)//???
{
	*pScalar = *pScalar / m_fScale;
	return GDM_SUCCESS;
}

GDM_ERROR_NO CGDM_Camera::ScreenToCamera(INOUT CGDM_Vector3DF *pVector)
{
	pVector->m_tX = (pVector->m_tX - m_nScreenWidth / 2.f) / m_fScaleW;
	pVector->m_tY = (m_nScreenHeight / 2.f - pVector->m_tY) / m_fScaleH;
	return GDM_SUCCESS;
}

GDM_ERROR_NO CGDM_Camera::CameraToScreen(INOUT CGDM_Vector3DF *pPt)
{
	pPt->m_tX = pPt->m_tX * m_fScaleW + m_nScreenWidth / 2.f + 0.5f;
	pPt->m_tY =  m_nScreenHeight / 2.f - pPt->m_tY * m_fScaleH + 0.5f;
	return GDM_SUCCESS;
}

// around pVt
void CGDM_Camera::RotateAroundVector(double angle , CGDM_Vector3DF* pVt)
{
	if(angle == 0.0f)
		return;

	CGDM_Vector3DF vt = *pVt;
	vt.normalize();
	GDM_Coord3D coord = m_coord;
	CGDM_Point3DF oldOrg = m_coord.m_org;
	gdm_RotateAroundVector(&coord , angle , vt);
	SetCameraCoord(&coord);
}

void CGDM_Camera::StartAutoMoveForward(GDM_MOVE_FORWARD_ANI_INFO* pInfo)
{
	m_pMoveForwardAnimationHandler->SetAniInfo(pInfo);
	m_anyType = ANI_MOVE_FORWARD;
}

void CGDM_Camera::StartAutoMoveTo(GDM_MOVETO_ANI_INFO* pInfo)
{
	m_pMoveToAnimationHandler->SetAniInfo(pInfo);
	m_anyType = ANI_MOVETO;
}

void CGDM_Camera::StartAutoRotate(GDM_ROT_ANI_INFO *pInfo)
{
	m_pRotAnimationHandler->SetAniInfo(pInfo);
	m_anyType = ANI_ROTATE;
}

bool CGDM_Camera::OnAnimation(double deltaHour)
{
	switch(m_anyType)
	{
	case ANI_ROTATE:
		return m_pRotAnimationHandler->OnAnimation(deltaHour);
	case ANI_MOVETO:
		return m_pMoveToAnimationHandler->OnAnimation(deltaHour);
	case ANI_MOVE_FORWARD:
		return m_pMoveForwardAnimationHandler->OnAnimation(deltaHour);
	}
	return false;
}

void CGDM_Camera::EndAnimation()
{
	m_anyType = ANI_NONE;
}

GDM_ERROR_NO CGDM_Camera::GetHitVector(int x , int y , CGDM_Vector3DF* pVt)
{
	GDM_Coord3D coord;
	GDM_ERROR_NO ret = GDM_SUCCESS;

	GetCameraCoord(&coord);
	pVt->set(x , y , 0);
	ScreenToCamera(pVt);
	CGDM_Vector3DF vt = *pVt;
	vt.m_tZ = -m_frustum.m_dNear;
	vt.normalize();
	gdm_VectorTCStoSCS(&coord , &vt);
	*pVt = vt;
	return ret;
}
//¼ÑÊÌ¼ê ½Û·Í»ô¶®Áìµá¹¢À¾ Ì¡°Ò´ª»ô¶®Áì¶¦ ±Ëº¤ÂÙ³Þ.
GDM_ERROR_NO CGDM_Camera::GetHitLocation(int x , int y , CGDM_Location3D* pPt)
{
	GDM_Coord3D coord;
	GDM_ERROR_NO ret = GDM_SUCCESS;
	CGDM_Point3DF cPt;

	// for fast calcu
	if(!m_blNeedCalcuHitPos)
	{
		if(m_sMousePos.x == x || m_sMousePos.y == y)
		{
			pPt->m_tX = m_sMousePos.longitude;
			pPt->m_tY = m_sMousePos.latitude;
			pPt->m_tZ = m_sMousePos.altitude;
			return m_bEnabled ? GDM_SUCCESS : GDM_FAIL;
		}
	}

	GetCameraCoord(&coord);
	cPt.set(x , y , 0);
	ScreenToCamera(&cPt);
	CGDM_Vector3DF vt = cPt;
	vt.m_tZ = -m_frustum.m_dNear;
	vt.normalize();
	gdm_VectorTCStoSCS(&coord , &vt);

	ret = gdm_HitPoint(m_coord.m_org , vt , &cPt );
	gdm_GetLocation(&cPt , pPt);

	m_sMousePos.longitude	= pPt->m_tX;
	m_sMousePos.latitude	= pPt->m_tY;

	if(pPt->m_tZ < 1)
		pPt->m_tZ = gdm_getHeight(m_sMousePos.longitude, m_sMousePos.latitude);

	m_sMousePos.altitude	= pPt->m_tZ;

	if(m_sMousePos.altitude < 10)
	{
		vt.m_tX = 0;
	}
	m_sMousePos.x			= x;
	m_sMousePos.y			= y;

	m_blNeedCalcuHitPos = false;
	m_bEnabled = ret == GDM_SUCCESS;

	return ret;
}

GDM_ERROR_NO CGDM_Camera::GetHitLocation(int x , int y , double radius, CGDM_Location3D* pPt)
{
	CGDM_Vector3DF vt;
	CGDM_Vector3DF interPt;
	
	gdm_CameraGetHitVector(g_pGDMDataMgr, x, y, &vt);
    gdm_HitPoint(m_coord.m_org, vt, radius, &interPt);
	gdm_GetLocation(&interPt , pPt);

	return GDM_SUCCESS;
}

// get screen point from location
GDM_ERROR_NO CGDM_Camera::WorldToScreen(double Longitude, double Latitude, CGDM_Vector3DF* pPt)
{
	gdm_get3DWorldPoint(Longitude, Latitude, *pPt, 0, NULL,true);

	WorldToScreen(pPt);

	return GDM_SUCCESS;
}

// get screen point from location
GDM_ERROR_NO CGDM_Camera::WorldToScreen(double Longitude, double Latitude, double height, CGDM_Vector3DF* pPt)
{
	GDM_ERROR_NO ret;

	gdm_get3DWorldPoint(Longitude, Latitude, *pPt, height, NULL, false);

	ret = WorldToScreen(pPt);

	return ret;
}


double CGDM_Camera::GetAreaInScreen(double minLongitude, double maxLongitude, double minLatitude, double maxLatitude)
{
	double area1,area2;
	double maxHeight = 3000;
	CGDM_Vector3DF tmpPt[8];

	if(maxHeight > m_location.m_dDist)
		maxHeight = m_location.m_dDist;

	WorldToScreen(minLongitude, minLatitude, 0, &tmpPt[0]);
	WorldToScreen(minLongitude, maxLatitude, 0, &tmpPt[1]);
	WorldToScreen(maxLongitude, minLatitude, 0, &tmpPt[2]);
	WorldToScreen(maxLongitude, maxLatitude, 0, &tmpPt[3]);

	//WorldToScreen(minLongitude, minLatitude, maxHeight, &tmpPt[4]);
	//WorldToScreen(minLongitude, maxLatitude, maxHeight, &tmpPt[5]);
	//WorldToScreen(maxLongitude, minLatitude, maxHeight, &tmpPt[6]);
	//WorldToScreen(maxLongitude, maxLatitude, maxHeight, &tmpPt[7]);

	//WorldToScreen(minLongitude, minLatitude, maxHeight, &tmpPt[0]);
	//WorldToScreen(minLongitude, maxLatitude, maxHeight, &tmpPt[1]);
	//WorldToScreen(maxLongitude, minLatitude, maxHeight, &tmpPt[2]);
	//WorldToScreen(maxLongitude, maxLatitude, maxHeight, &tmpPt[3]);

	//if(!IsInScreen(tmpPt, 8))
	//{
	//	return 5;
	//}

	area1 = (tmpPt[2].m_tX - tmpPt[0].m_tX) * (tmpPt[1].m_tY - tmpPt[0].m_tY) - 
		(tmpPt[2].m_tY - tmpPt[0].m_tY) * (tmpPt[1].m_tX - tmpPt[0].m_tX);
	area2 = (tmpPt[2].m_tX - tmpPt[3].m_tX) * (tmpPt[1].m_tY - tmpPt[3].m_tY) - 
		(tmpPt[2].m_tY - tmpPt[3].m_tY) * (tmpPt[1].m_tX - tmpPt[3].m_tX);
	return (fabs(area1) + fabs(area2))/2.0;
}

void CGDM_Camera::UpdateCameraPos()
{
	GDM_Coord3D coord = m_coord;
	SetCameraCoord(&coord);
}

bool CGDM_Camera::IsSameCoord(GDM_Coord3D* pCoord1 , GDM_Coord3D* pCoord2)
{
	if(!pCoord1 || !pCoord2)
		return false;
	if(!gdm_equals(pCoord1->m_org.m_tX, pCoord2->m_org.m_tX))
		return false;

	if(!gdm_equals(pCoord1->m_org.m_tY, pCoord2->m_org.m_tY))
		return false;

	if(!gdm_equals(pCoord1->m_org.m_tZ, pCoord2->m_org.m_tZ))
		return false;

	if(!gdm_equals(pCoord1->m_x.m_tX, pCoord2->m_x.m_tX))
		return false;

	if(!gdm_equals(pCoord1->m_x.m_tY, pCoord2->m_x.m_tY))
		return false;

	if(!gdm_equals(pCoord1->m_y.m_tX, pCoord2->m_y.m_tX))
		return false;

	if(!gdm_equals(pCoord1->m_y.m_tY, pCoord2->m_y.m_tY))
		return false;

	return true;
}

void CGDM_Camera::SetFOV(double fov)
{
	m_frustum.m_dFovy=fov*gdm_DEGTORAD;
	 Update();
}

/*

//*/

#include "QGlobe_Camera.h"
#include "QGlobe_Define.h"
#include "QGlobe_Coord3D.h"
#include "QGlobe_CollisionHandler.h"
#include "QGlobe_PyramidMgr.h"
#include "QGlobe_DataMgr.h"



//	[2009.08.19][CameraMan][NearFar]
#define CAMERA_NEAR		1.0
#define CAMERA_FAR		1E+10
#define CAMERA_FOVY		50 * qglobe_DEGTORAD
#define	MAX_CAMERA_DIST	10 * EARTH_RADIUS

void qglobe_GetStandardCoord(QGlobe_LOCATION *pLoc , QGlobe_Coord3D *pCoord)
{
	qglobe_GetPosition(pLoc , &pCoord->m_org);
	CQGlobe_Vector3DF vx , vy , vz;
	CQGlobe_Vector3DF vt(0 , 0 , 1);
	double scalar;
	vz = pCoord->m_org;
	vz.normalize();
	scalar = vz.dotProduct(vt);
	if(qglobe_equals(scalar , 1.0 , 0.0001))
	{
		vx = CQGlobe_Vector3DF(0, 1 , 0);// same with world y
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

void qglobe_GetCoord(QGlobe_CAMERA_INFO *pCameraInfo , QGlobe_Coord3D *pCoord , char bMode )
{
	QGlobe_Coord3D coord;
	CQGlobe_Point3DF org;
	CQGlobe_Vector3DF vt;
	QGlobe_LOCATION loc;
	double height = 0;
	if(bMode == 0)
	{
		loc = pCameraInfo->m_loc;
		loc.m_dDist += pCameraInfo->m_orgDist;
		qglobe_GetStandardCoord(&loc, &coord);
		org.set(0 , 0 , 0);
		vt = coord.m_org;
		qglobe_RotateAroundLine(&coord , pCameraInfo->m_roll , org , vt);

		org = coord.m_org;
//		loc.m_dDist = pCameraInfo->m_orgDist;
//		qglobe_GetPosition(&loc , &org);
		org = coord.m_org - coord.m_z * pCameraInfo->m_loc.m_dDist;
		
		vt = coord.m_x;
		qglobe_RotateAroundLine(&coord , pCameraInfo->m_pitch , org , vt);
		*pCoord = coord;
	}
	else
	{
		loc = pCameraInfo->m_loc;
		loc.m_dDist += pCameraInfo->m_orgDist;
		qglobe_GetStandardCoord(&loc , &coord);
		org.set(0 , 0 , 0);
		vt = coord.m_org;
		qglobe_RotateAroundLine(&coord , pCameraInfo->m_roll , org , vt);

		org = coord.m_org;
		vt = coord.m_x;
		qglobe_RotateAroundLine(&coord , pCameraInfo->m_pitch , org , vt);
		*pCoord = coord;
	}
}

void qglobe_GetCoordInfo(QGlobe_Coord3D* pCoord , QGlobe_CAMERA_INFO *pCameraInfo , char bMode)
{
	QGlobe_Coord3D standCoord;
	CQGlobe_Vector3DF vt , vt1;
	CQGlobe_Point3DF pt;
	double len;
	QGlobe_ERROR_NO retVal;
	if(bMode == 0)
	{
		retVal = qglobe_HitPoint(pCoord->m_org , -(pCoord->m_z) , &pt);
		vt1 = pCoord->m_org - pt;
		len = fabs(vt1.dotProduct(pCoord->m_z));
		pt = pCoord->m_org + -pCoord->m_z * len;
		qglobe_GetLocation(&pt , &pCameraInfo->m_loc);
		qglobe_GetStandardCoord(&pCameraInfo->m_loc , &standCoord);
		if(retVal == QGlobe_SUCCESS)
		{
			pCameraInfo->m_orgDist = (double)pCameraInfo->m_loc.m_dDist; // qglobe_getHeight(pCameraInfo->m_loc.m_dLongitude , pCameraInfo->m_loc.m_dLatitude);
			pCameraInfo->m_loc.m_dDist = len;
			vt = pCoord->m_x;
			qglobe_GetAngleBetweenTwoVector(standCoord.m_z , vt1 , vt , pCameraInfo->m_pitch);
		}
		else
		{
			pCameraInfo->m_loc.m_dDist = len;
			pCameraInfo->m_orgDist = 0;
			pCameraInfo->m_pitch = qglobe_HALF_PI;
		}
		vt = standCoord.m_z;
		qglobe_GetAngleBetweenTwoVector(standCoord.m_x , pCoord->m_x , vt , pCameraInfo->m_roll);
		pCameraInfo->m_yaw = 0;
	}
	else
	{

		qglobe_GetLocation(&pCoord->m_org , &pCameraInfo->m_loc);
		qglobe_GetStandardCoord(&pCameraInfo->m_loc , &standCoord);
		vt = pCoord->m_x;
		qglobe_GetAngleBetweenTwoVector(standCoord.m_z , pCoord->m_z , vt , pCameraInfo->m_pitch);
		vt = pCoord->m_org;
		vt.normalize();
		qglobe_GetAngleBetweenTwoVector(standCoord.m_x , pCoord->m_x , vt , pCameraInfo->m_roll);
		pCameraInfo->m_yaw = 0;
		pCameraInfo->m_orgDist = 0;
	}
}

CQGlobe_Camera::CQGlobe_Camera(void)
{
	m_pRotAnimationHandler = new CQGlobe_CameraRotAnimationHandler(this);
	m_pMoveToAnimationHandler = new CQGlobe_CameraMoveToAnimationHandler(this);
	m_pMoveForwardAnimationHandler = new CQGlobe_CameraMoveForwardAnimationHandler(this);
	m_anyType = ANI_NONE;
	m_fScale  = 1.0f;
	m_fScaleW = 1.0f;
	m_fScaleH = 1.0f;
	m_fScalarOfDir = 1.0;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bEnabled = false;
}

CQGlobe_Camera::~CQGlobe_Camera(void)
{
	if(m_pRotAnimationHandler)
		delete m_pRotAnimationHandler;
	if(m_pMoveToAnimationHandler)
		delete m_pMoveToAnimationHandler;
	if(m_pMoveForwardAnimationHandler)
		delete m_pMoveForwardAnimationHandler;
}

void CQGlobe_Camera::Initialize(int nWidth , int nHeight)
{
	// View Frustum
	m_frustum.m_dNear = CAMERA_NEAR;
	m_frustum.m_dFar =  CAMERA_FAR;
	m_frustum.m_dFovy = CAMERA_FOVY;
	m_frustum.m_dAspect = ((double)nWidth) / nHeight;

	QGlobe_LOCATION loc;

	loc.m_dDist = EARTH_RADIUS * 3;
	loc.m_dLatitude = qglobe_DEGTORAD * 0 ;
	loc.m_dLongitude = qglobe_DEGTORAD * 0;

	SetCameraLocation(&loc);

	Update();
}

//set camera's location from B,L,H 
//and then set camera coordinate 

void CQGlobe_Camera::SetCameraLocation(QGlobe_LOCATION *pLoc)
{	
	m_location = *pLoc;
	
	QGlobe_Coord3D coord;

	qglobe_GetStandardCoord(pLoc , &coord);
	SetCameraCoord(&coord);
}

//set camera coordinates considering collision

bool CQGlobe_Camera::SetCameraCoord(QGlobe_Coord3D* pCoord , bool blCheckCollision)
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

QGlobe_ERROR_NO CQGlobe_Camera::SetViewPortSize(int w , int h)
{
	m_nScreenWidth = w;
	m_nScreenHeight = h;

	m_frustum.m_dAspect =  ((double)w) / h;

	double height = 2 * m_frustum.m_dNear * tan(m_frustum.m_dFovy / 2);
	double width = height * m_frustum.m_dAspect;
	
	m_fScaleW = (m_nScreenWidth - 0.75f) / width;   //???
	m_fScaleH = (m_nScreenHeight - 0.75f) / height; //???

	m_fScale =qMax(m_fScaleW,m_fScaleH);

	return QGlobe_SUCCESS;
}


void CQGlobe_Camera::Update()
{
	//1 determine camera location
	qglobe_GetLocation(&m_coord.m_org , &m_location);

	g_pGDMDataMgr->m_pPyramidMgr->Update(); //???

	//2 update frustum 's near and far
	
	m_frustum.m_dNear = ComputeNearClipDistance();
	m_frustum.m_dFar  = ComputeFarClipDistance();

	//3 update m_fScale,m_fScaleW,m_fScaleW
	UpdateScaleParamerters();

	//4 determine m_fScalarOfDir
	// set scalar value of direction

	CQGlobe_Vector3DF org;

	org = m_coord.m_org;
	org.normalize();

	m_fScalarOfDir = fabs(org.dotProduct(m_coord.m_z));

	CalculateViewRange();
	g_pGDMDataMgr->SetModifyFlag();   //???
}

double CQGlobe_Camera::ComputeNearClipDistance()
{
	double SurfaceHeight = qglobe_getMaxHeight(m_location.m_dLongitude, m_location.m_dLatitude);
	
	//original mode
	//double near = ((m_location.m_dDist - SurfaceHeight) * cos(m_frustum.m_dFovy * 1.2 / 2)) / 10.0;

	double near=(m_location.m_dDist - SurfaceHeight)*cos(m_frustum.m_dFovy/2)/2;

	if( near < CAMERA_NEAR)
		near = CAMERA_NEAR;

	if(m_location.m_dDist < SurfaceHeight)
		near = CAMERA_NEAR;

	return near;
}

double CQGlobe_Camera::ComputeFarClipDistance()
{
	/* original mode
	CQGlobe_Vector3DF vt;

	vt = -m_coord.m_z;

	CQGlobe_Point3DF crossPt , orgPt(0 , 0 , 0);

	qglobe_GetPerpendPt(&crossPt, &orgPt , &m_coord.m_org , &vt);

	double lenToCenter = (crossPt - m_coord.m_org).getLength();

    return lenToCenter * 2;*/

	double h=m_location.m_dDist;

	double far=sqrt(2*EARTH_RADIUS*h+h*h);

	return far;
}

void CQGlobe_Camera::UpdateScaleParamerters()
{
	double height = 2 * m_frustum.m_dNear * tan(m_frustum.m_dFovy / 2);
	double width = height * m_frustum.m_dAspect;

	m_fScaleW = (m_nScreenWidth - 0.75f) / width;   //???
	m_fScaleH = (m_nScreenHeight - 0.75f) / height; //???

	m_fScale =qMax(m_fScaleW,m_fScaleH);
}

void CQGlobe_Camera::CalculateViewRange()
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

	if((dirAngle + fovy / 2.0) < (qglobe_HALF_PI - angle))
	{
		dirAngle = dirAngle + fovy / 2.0;
		double angTmp = dCameraDist / radius * sin(dirAngle);
		if(angTmp > 1.0)
			angTmp = 1.0;
		angTmp = asin(angTmp);
		angTmp = qglobe_HALF_PI + (qglobe_HALF_PI - angTmp);
		angle = qglobe_PI - angTmp - dirAngle;
	}

	double dMinLati = m_location.m_dLatitude - angle;
	double dMaxLati = m_location.m_dLatitude + angle;

	if(dMaxLati >  qglobe_HALF_PI)
		dMaxLati = qglobe_HALF_PI;
	if(dMinLati < (-qglobe_HALF_PI))
		dMinLati = -qglobe_HALF_PI;

	// calulate longititude
	double longi = m_location.m_dLongitude;
	double radius1 = radius * cos(m_location.m_dLatitude);
	double radius2 = radius * angle;
	double dMinLongi, dMidMinLongi;
	double dMidMaxLongi, dMaxLongi;

	if(radius1 <= radius2)
	{
		dMinLongi = -qglobe_PI;
		dMaxLongi = qglobe_PI;
		dMidMinLongi = dMaxLongi;
		dMidMaxLongi = dMaxLongi;
	}
	else
	{
		angle = asin(radius2 / radius1);
		if(longi + angle > qglobe_PI)
		{
			dMinLongi = -qglobe_PI;
			dMaxLongi = qglobe_PI;
			dMidMinLongi = longi + angle - (2 * qglobe_PI);
			dMidMaxLongi = longi - angle;
		}
		else if(longi - angle < (-qglobe_PI))
		{
			dMinLongi = -qglobe_PI;
			dMaxLongi = qglobe_PI;
			dMidMinLongi = longi + angle;
			dMidMaxLongi = longi - angle + (2 * qglobe_PI);
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
void CQGlobe_Camera::GetCameraCoord(QGlobe_Coord3D* pCoord)
{
	*pCoord = m_coord;
}

bool CQGlobe_Camera::LookAt(CQGlobe_Point3DF* pOrg , CQGlobe_Point3DF* pTargetPos, CQGlobe_Vector3DF* pUpVt)
{
	QGlobe_Coord3D coord;
	coord.m_org = *pOrg;
	coord.m_z = (-(*pTargetPos - *pOrg)).normalize();
	coord.m_y = pUpVt->normalize();
	coord.m_x = coord.m_y.crossProduct(coord.m_z);
	SetCameraCoord(&coord);
	return true;
}

void CQGlobe_Camera::SetInfo(QGlobe_CAMERA_INFO *pCameraInfo, char bMode)
{
	QGlobe_Coord3D coord;
	qglobe_GetCoord(pCameraInfo , &coord, bMode);
	SetCameraCoord(&coord);
}

void CQGlobe_Camera::GetInfo(QGlobe_CAMERA_INFO *pCameraInfo, char bMode)
{
	qglobe_GetCoordInfo(&m_coord, pCameraInfo, bMode);
}

//get vector representing view direction i.e Z axis
void CQGlobe_Camera::GetViewVector(CQGlobe_Vector3DF* pVt)
{
	QGlobe_Coord3D coord;
	GetCameraCoord(&coord);
	*pVt = coord.m_z;
}

QGlobe_ERROR_NO CQGlobe_Camera::WorldToScreen(INOUT CQGlobe_Vector3DF* pPt)
{
	qglobe_SCStoTCS(&m_coord , pPt);
	double depth = fabs(pPt->m_tZ);

	if(qglobe_equals(depth , 0.0 , qglobe_ROUNDING_ERROR_32))
		depth = qglobe_ROUNDING_ERROR_32;

	pPt->m_tX = pPt->m_tX * m_frustum.m_dNear / depth;
	pPt->m_tY = pPt->m_tY * m_frustum.m_dNear / depth;
	CameraToScreen(pPt);
	if(pPt->m_tZ >= -m_frustum.m_dNear)
	{
		pPt->m_tZ = -1;
		return QGlobe_FAIL;
	}
	else
		pPt->m_tZ = 0;
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO CQGlobe_Camera::ScreenToCamera(double* pScalar)//???
{
	*pScalar = *pScalar / m_fScale;
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO CQGlobe_Camera::ScreenToCamera(INOUT CQGlobe_Vector3DF *pVector)
{
	pVector->m_tX = (pVector->m_tX - m_nScreenWidth / 2.f) / m_fScaleW;
	pVector->m_tY = (m_nScreenHeight / 2.f - pVector->m_tY) / m_fScaleH;
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO CQGlobe_Camera::CameraToScreen(INOUT CQGlobe_Vector3DF *pPt)
{
	pPt->m_tX = pPt->m_tX * m_fScaleW + m_nScreenWidth / 2.f + 0.5f;
	pPt->m_tY =  m_nScreenHeight / 2.f - pPt->m_tY * m_fScaleH + 0.5f;
	return QGlobe_SUCCESS;
}

// around pVt
void CQGlobe_Camera::RotateAroundVector(double angle , CQGlobe_Vector3DF* pVt)
{
	if(angle == 0.0f)
		return;

	CQGlobe_Vector3DF vt = *pVt;
	vt.normalize();
	QGlobe_Coord3D coord = m_coord;
	CQGlobe_Point3DF oldOrg = m_coord.m_org;
	qglobe_RotateAroundVector(&coord , angle , vt);
	SetCameraCoord(&coord);
}

void CQGlobe_Camera::StartAutoMoveForward(QGlobe_MOVE_FORWARD_ANI_INFO* pInfo)
{
	m_pMoveForwardAnimationHandler->SetAniInfo(pInfo);
	m_anyType = ANI_MOVE_FORWARD;
}

void CQGlobe_Camera::StartAutoMoveTo(QGlobe_MOVETO_ANI_INFO* pInfo)
{
	m_pMoveToAnimationHandler->SetAniInfo(pInfo);
	m_anyType = ANI_MOVETO;
}

void CQGlobe_Camera::StartAutoRotate(QGlobe_ROT_ANI_INFO *pInfo)
{
	m_pRotAnimationHandler->SetAniInfo(pInfo);
	m_anyType = ANI_ROTATE;
}

bool CQGlobe_Camera::OnAnimation(double deltaHour)
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

void CQGlobe_Camera::EndAnimation()
{
	m_anyType = ANI_NONE;
}

QGlobe_ERROR_NO CQGlobe_Camera::GetHitVector(int x , int y , CQGlobe_Vector3DF* pVt)
{
	QGlobe_Coord3D coord;
	QGlobe_ERROR_NO ret = QGlobe_SUCCESS;

	GetCameraCoord(&coord);
	pVt->set(x , y , 0);
	ScreenToCamera(pVt);
	CQGlobe_Vector3DF vt = *pVt;
	vt.m_tZ = -m_frustum.m_dNear;
	vt.normalize();
	qglobe_VectorTCStoSCS(&coord , &vt);
	*pVt = vt;
	return ret;
}
//���̼� �۷ͻ�����Ṣ�� ̡�Ҵ������춦 �˺��ٳ�.
QGlobe_ERROR_NO CQGlobe_Camera::GetHitLocation(int x , int y , CQGlobe_Location3D* pPt)
{
	QGlobe_Coord3D coord;
	QGlobe_ERROR_NO ret = QGlobe_SUCCESS;
	CQGlobe_Point3DF cPt;

	// for fast calcu
	if(!m_blNeedCalcuHitPos)
	{
		if(m_sMousePos.x == x || m_sMousePos.y == y)
		{
			pPt->m_tX = m_sMousePos.longitude;
			pPt->m_tY = m_sMousePos.latitude;
			pPt->m_tZ = m_sMousePos.altitude;
			return m_bEnabled ? QGlobe_SUCCESS : QGlobe_FAIL;
		}
	}

	GetCameraCoord(&coord);
	cPt.set(x , y , 0);
	ScreenToCamera(&cPt);
	CQGlobe_Vector3DF vt = cPt;
	vt.m_tZ = -m_frustum.m_dNear;
	vt.normalize();
	qglobe_VectorTCStoSCS(&coord , &vt);

	ret = qglobe_HitPoint(m_coord.m_org , vt , &cPt );
	qglobe_GetLocation(&cPt , pPt);

	m_sMousePos.longitude	= pPt->m_tX;
	m_sMousePos.latitude	= pPt->m_tY;

	if(pPt->m_tZ < 1)
		pPt->m_tZ = qglobe_getHeight(m_sMousePos.longitude, m_sMousePos.latitude);

	m_sMousePos.altitude	= pPt->m_tZ;

	if(m_sMousePos.altitude < 10)
	{
		vt.m_tX = 0;
	}
	m_sMousePos.x			= x;
	m_sMousePos.y			= y;

	m_blNeedCalcuHitPos = false;
	m_bEnabled = ret == QGlobe_SUCCESS;

	return ret;
}

QGlobe_ERROR_NO CQGlobe_Camera::GetHitLocation(int x , int y , double radius, CQGlobe_Location3D* pPt)
{
	CQGlobe_Vector3DF vt;
	CQGlobe_Vector3DF interPt;
	
	qglobe_CameraGetHitVector(g_pGDMDataMgr, x, y, &vt);
    qglobe_HitPoint(m_coord.m_org, vt, radius, &interPt);
	qglobe_GetLocation(&interPt , pPt);

	return QGlobe_SUCCESS;
}

// get screen point from location
QGlobe_ERROR_NO CQGlobe_Camera::WorldToScreen(double Longitude, double Latitude, CQGlobe_Vector3DF* pPt)
{
	qglobe_get3DWorldPoint(Longitude, Latitude, *pPt, 0, NULL,true);

	WorldToScreen(pPt);

	return QGlobe_SUCCESS;
}

// get screen point from location
QGlobe_ERROR_NO CQGlobe_Camera::WorldToScreen(double Longitude, double Latitude, double height, CQGlobe_Vector3DF* pPt)
{
	QGlobe_ERROR_NO ret;

	qglobe_get3DWorldPoint(Longitude, Latitude, *pPt, height, NULL, false);

	ret = WorldToScreen(pPt);

	return ret;
}


double CQGlobe_Camera::GetAreaInScreen(double minLongitude, double maxLongitude, double minLatitude, double maxLatitude)
{
	double area1,area2;
	double maxHeight = 3000;
	CQGlobe_Vector3DF tmpPt[8];

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

void CQGlobe_Camera::UpdateCameraPos()
{
	QGlobe_Coord3D coord = m_coord;
	SetCameraCoord(&coord);
}

bool CQGlobe_Camera::IsSameCoord(QGlobe_Coord3D* pCoord1 , QGlobe_Coord3D* pCoord2)
{
	if(!pCoord1 || !pCoord2)
		return false;
	if(!qglobe_equals(pCoord1->m_org.m_tX, pCoord2->m_org.m_tX))
		return false;

	if(!qglobe_equals(pCoord1->m_org.m_tY, pCoord2->m_org.m_tY))
		return false;

	if(!qglobe_equals(pCoord1->m_org.m_tZ, pCoord2->m_org.m_tZ))
		return false;

	if(!qglobe_equals(pCoord1->m_x.m_tX, pCoord2->m_x.m_tX))
		return false;

	if(!qglobe_equals(pCoord1->m_x.m_tY, pCoord2->m_x.m_tY))
		return false;

	if(!qglobe_equals(pCoord1->m_y.m_tX, pCoord2->m_y.m_tX))
		return false;

	if(!qglobe_equals(pCoord1->m_y.m_tY, pCoord2->m_y.m_tY))
		return false;

	return true;
}

void CQGlobe_Camera::SetFOV(double fov)
{
	m_frustum.m_dFovy=fov*qglobe_DEGTORAD;
	 Update();
}

/*

//*/
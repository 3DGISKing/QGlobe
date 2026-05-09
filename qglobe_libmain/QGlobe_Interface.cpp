#include "QGlobe_Interface.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_Camera.h"
#include "QGlobe_ThreadMgr.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_FileCacheMgr.h"
#include "QGlobe_FPSTester.h"
#include "QGlobe_TextMgr.h"
#include "QGlobe_RenderTile.h"
#include "QGlobe_CollisionHandler.h"
#include "QGlobe_SimpleBuildingCache.h"
#include "qglobe_config.h"

extern CQGlobe_FPSTester g_FPSTester;

extern void qglobe_GetPlaceCameraInfo(PLACEMARK_DATA* pData);

QGlobe_ERROR_NO qglobe_CameraInit(QGlobe_RENDER_HANDLE handle , int nWidth , int nHeight)
{
	if(handle == NULL)
		return QGlobe_FAIL;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->Initialize(nWidth , nHeight);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraSetViewPortSize(QGlobe_RENDER_HANDLE handle , int w , int h)
{
	if(handle == NULL)
		return QGlobe_FAIL;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->SetViewPortSize(w , h);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraGetCoord(QGlobe_RENDER_HANDLE handle , QGlobe_Coord3D *pCoord)
{
	if(handle == NULL)
		return QGlobe_FAIL;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->GetCameraCoord(pCoord);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraSetCoord(QGlobe_RENDER_HANDLE handle , QGlobe_Coord3D coord)
{
	if(handle == NULL)
		return QGlobe_FAIL;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->SetCameraCoord(&coord);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO	qglobe_CameraGetLocation(QGlobe_RENDER_HANDLE handle , QGlobe_LOCATION *pLoc)
{
	if(handle == NULL)
		return QGlobe_FAIL;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->GetLocation(pLoc);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraRotate(QGlobe_RENDER_HANDLE handle , double fOffsetAngle , CQGlobe_Vector3DF *pAxis)
{
	if(handle == NULL)
		return QGlobe_FAIL;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->RotateAroundVector(fOffsetAngle , pAxis);
	return QGlobe_SUCCESS;

}

QGlobe_ERROR_NO qglobe_CameraStopAnimation(QGlobe_RENDER_HANDLE handle)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->StopAnimation();
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraStartAutoRotate(QGlobe_RENDER_HANDLE handle , QGlobe_ROT_ANI_INFO* pInfo)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->StartAutoRotate(pInfo);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraStartAutoMoveForward(QGlobe_RENDER_HANDLE handle , QGlobe_MOVE_FORWARD_ANI_INFO* pInfo)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->StartAutoMoveForward(pInfo);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraStartAutoMoveTo(QGlobe_RENDER_HANDLE handle , QGlobe_MOVETO_ANI_INFO* pInfo)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->StartAutoMoveTo(pInfo);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraOnAnimation(QGlobe_RENDER_HANDLE handle , double deltaTime)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	if(pMainMgr->m_pCamera->OnAnimation(deltaTime))
		return QGlobe_SUCCESS;
	else
		return QGlobe_FAIL;
}
QGlobe_ERROR_NO qglobe_CameraStopAuto(QGlobe_RENDER_HANDLE handle)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->StopAnimation();
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_LocationToScreen(double longitude, double latitude, double height, CQGlobe_Point3DF* pOutPt)
{
	QGlobe_POINT3D pt;

	if(g_pGDMDataMgr == NULL)
		return QGlobe_SUCCESS;

	qglobe_get3DWorldPoint(longitude, latitude, pt, height);
	pOutPt->m_tX = pt.m_X;
	pOutPt->m_tY = pt.m_Y;
	pOutPt->m_tZ = pt.m_Z;

	g_pGDMDataMgr->m_pCamera->WorldToScreen(pOutPt);

	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_WorldToScreen(INOUT CQGlobe_Point3DF* pOutPt)
{
	if(g_pGDMDataMgr == NULL)
		return QGlobe_SUCCESS;
	g_pGDMDataMgr->m_pCamera->WorldToScreen(pOutPt);
	return QGlobe_SUCCESS;
}


QGlobe_ERROR_NO qglobe_ScreenToCamera(QGlobe_RENDER_HANDLE handle , double* pScalar)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->ScreenToCamera(pScalar);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_ScreenToCamera(QGlobe_RENDER_HANDLE handle , CQGlobe_Vector3DF *pVector)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->ScreenToCamera(pVector);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_GetHitLocation(int x , int y , CQGlobe_Location3D* pPt)
{
	if(g_pGDMDataMgr == NULL)
		return QGlobe_SUCCESS;
	return g_pGDMDataMgr->m_pCamera->GetHitLocation(x , y , pPt);
}

QGlobe_ERROR_NO qglobe_CameraGetInfo(QGlobe_RENDER_HANDLE handle , QGlobe_CAMERA_INFO *pCameraInfo, char bMode)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->GetInfo(pCameraInfo, bMode);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraSetInfo(QGlobe_RENDER_HANDLE handle , QGlobe_CAMERA_INFO *pCameraInfo, char bMode)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCamera->SetInfo(pCameraInfo, bMode);
	return QGlobe_SUCCESS;
}


QGlobe_ERROR_NO qglobe_CameraGetHitVector(QGlobe_RENDER_HANDLE handle , int x , int y , CQGlobe_Vector3DF* pVt)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	return pMainMgr->m_pCamera->GetHitVector( x, y, pVt);
}


/********************************************************************************
*								Cache Processing								*
********************************************************************************/

QGlobe_ERROR_NO qglobe_CacheMemClear(QGlobe_RENDER_HANDLE handle)
{
	if(handle == NULL)
		return QGlobe_SUCCESS;
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCacheMgr->Clear();
	pMainMgr->SetModifyFlag();
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CacheChangeSize(QGlobe_RENDER_HANDLE handle, int nMemSize, int nFileSize)
{

	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pFileCacheMgr->SetCacheFileSize(nFileSize);
	pMainMgr->m_pCacheMgr->SetMaxSize(nMemSize);
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CacheFileClear(QGlobe_RENDER_HANDLE handle)
{
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pFileCacheMgr->Clear();
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_SimpleBuildingCacheClear(QGlobe_RENDER_HANDLE handle)
{
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	pMainMgr->m_pCacheMgr->GetCacheMgr(E_QGlobe_MAINDATA_TYPE_3D)->Clear();
	pMainMgr->m_pSimpleBuildingCache->Clear();
	return QGlobe_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

void qglobe_GetFSP(QGlobe_RENDER_HANDLE handle , int& fsp, int &real_fsp, int &meshs, int &lines)
{
	fsp = g_FPSTester.m_curFrame;
	real_fsp = g_FPSTester.m_realFrame;
	meshs = g_FPSTester.m_totalMeshCount;
	lines = g_FPSTester.m_totalLineCount;
}

void qglobe_GetRenderOption(QGlobe_RENDER_HANDLE handle , QGlobe_OPTION* option)
{
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	if(pMainMgr == NULL)
		return;
	*option = pMainMgr->m_sOption;
}

void qglobe_GetPlacemarkViewerData(QGlobe_RENDER_HANDLE handle, PLACEMARK_DATA* pData)
{
	qglobe_GetPlaceCameraInfo(pData);
}

QGlobe_ERROR_NO qglobe_GetLength(QGlobe_RENDER_HANDLE handle , QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd , double* length)
{
	double	dblLength = 0;
	double	rA	= 6378137,
			rB	= 6356752.314245;
	double	e2	= 0.006739496742337,
			f	= 0.003352810664747;

	double	fdLambda = pStart->m_dLongitude - pEnd->m_dLongitude;
	double	fdPhi	 = pStart->m_dLatitude - pEnd->m_dLatitude;
	double	fPhimean = (pStart->m_dLatitude + pEnd->m_dLatitude) / 2.0;
	double	fPhimean_Sin = sin(fPhimean);

	double	fTemp	= 1 - e2 * pow(fPhimean_Sin, 2);

	if (fTemp == 0)
	{
		int	a = 0;
	}

	double	fRho	= (rA * (1 - e2)) / pow(fTemp, 1.5);
	double	fNu		= rA / sqrt(fTemp);

	double	fLatEnd_Cos		= cos(pEnd->m_dLatitude),
			fLatStart_Cos	= cos(pStart->m_dLatitude);

	double	fz		= sqrt(pow(sin(fdPhi / 2.0), 2) + fLatEnd_Cos * fLatStart_Cos * pow(sin(fdLambda / 2.0), 2));
	fz	= 2 * asin(fz);

	double	fzSin = sin(fz);

	if (fzSin != 0)
	{
		double	fAlpha	= fLatEnd_Cos * sin(fdLambda) * 1 / fzSin;
		fAlpha = asin(fAlpha);

		double	fValue	= fRho * pow(sin(fAlpha), 2) + fNu * pow(cos(fAlpha), 2);

		if (fValue == 0)
		{
			int	a = 0;
		}

		double	fR = (fRho * fNu) / fValue;

		dblLength = (fz * fR);
	}

	*length = dblLength;

	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_HitPoint(CQGlobe_Point3DF orgPt , CQGlobe_Vector3DF vt , double rad , CQGlobe_Point3DF* pPt)
{
	double alpha , alpha1 , alpha2 ,cosVal;
	CQGlobe_Vector3DF vt1 , vt2 , vt3;
	QGlobe_Coord3D coord;
	long double l;
	vt.normalize();
	QGlobe_ERROR_NO retVal = QGlobe_SUCCESS;
	long double a , b , c , d;
	double t, t1 , t2;
	a = 1;
	b = 2 * (orgPt.m_tX * vt.m_tX + orgPt.m_tY * vt.m_tY + orgPt.m_tZ * vt.m_tZ);
	c = orgPt.getLengthSQ() - rad * rad;
	d = b * b - 4 * a * c;
	t = 0;
	t1 = 0;
	t2 = 0;
	if(d < 0)
		retVal = QGlobe_FAIL;
	else
	{
		t1 = (-b - sqrt(d)) / (2 * a);
		t2 = (-b + sqrt(d)) / (2 * a);
		if(t1 > 0 && t2 > 0)
		{
			t = t1 < t2 ? t1 : t2;
			if(qglobe_iszero(t))
			{
				t = t1 < t2 ? t2 : t1;
			}
		}
		else 
			t = t1 < t2 ? t2 : t1;

		if( (d < 0) || (t < 0))
		{
			retVal = QGlobe_FAIL;
		}
		else
		{
			*pPt = orgPt + vt * t;
			retVal = QGlobe_SUCCESS;
		}
	}
	if(retVal == QGlobe_FAIL)
	{
		qglobe_CameraGetCoord(g_pGDMDataMgr , &coord);
		vt1 = -coord.m_org; // org
		vt1.normalize();
		vt2 = vt; // hit vector
		vt3 = vt1.crossProduct(vt2).normalize();
		if(vt3.getLength() > 0.8) // if not vt1 // vt2 ->
		{
			qglobe_GetAngleBetweenTwoVector(vt1 , vt2 , vt3 , alpha);
			l = sqrt(orgPt.getLengthSQ() - EARTH_RADIUS * EARTH_RADIUS);
			cosVal = l / orgPt.getLength();
			if(cosVal > 1)
				cosVal = 1;
			alpha1 = acos(cosVal);
			alpha2 = alpha - alpha1;
#ifdef DEBUG
			//qDebug("alpha1 = %.5f , alpha2 = %.5f , alpha = %.5f\n" , alpha1 , alpha2 , alpha);
#endif
			vt2 = vt2.normalize() * l;
			qglobe_RotateAroundVector(&vt2 , -alpha2 , vt3);
			*pPt = coord.m_org + vt2;

		}
		else
		{
			QGlobe_LOCATION loc;
			loc.m_dDist = EARTH_RADIUS;
			loc.m_dLatitude = loc.m_dLongitude = 0;
			qglobe_GetPosition(&loc , pPt);
		}
	}
	return retVal;
}

QGlobe_ERROR_NO qglobe_HitPoint(CQGlobe_Point3DF orgPt , CQGlobe_Vector3DF vt , CQGlobe_Point3DF* pPt, bool isDem)
{
	QGlobe_ERROR_NO retVal;
	vt.normalize();
	retVal = QGlobe_FAIL;
#if 0
	if(isDem && g_pGDMDataMgr->m_sOption.is_LayerTerrain)
	{
		int x , y;
		double pos3D[3]; 
		CQGlobe_Point3DF targetPt = orgPt + vt * EARTH_RADIUS;
		qglobe_WorldToScreen(g_pGDMDataMgr , &targetPt);
		x = targetPt.m_tX;
		y = targetPt.m_tY;
		g_pGDMDataMgr->GetGDMSceneManager()->RenderDevice()->getVideoDriver()->screenToWorld(x, y, pos3D);
		g_pGDMDataMgr->m_pCamera->GetCameraCoord(&coord);
		CQGlobe_Vector3DF pt(pos3D[0], -pos3D[1], pos3D[2]);
		pt += coord.m_org;
		if (pt.getLength() > EARTH_RADIUS * 1.3)
			retVal = QGlobe_FAIL;
		else
		{
			pPt->set(pt.m_tX, pt.m_tY, pt.m_tZ);
			retVal = QGlobe_SUCCESS;
		}

	}
	else
#endif

		if(isDem && g_pGDMDataMgr->IsVisible(E_QGlobe_SUBDATA_TYPE_DEM))
		{			
#if USE_FAST_HITPOINT			
			retVal = qglobe_intersectRayDEM2(orgPt, vt, pPt);
#else
			retVal = qglobe_intersectRayDEM(orgPt, vt, pPt);
#endif
		}

		if(retVal == QGlobe_FAIL)
			retVal = qglobe_HitPoint(orgPt, vt , EARTH_RADIUS, pPt);
		return retVal;
}

void qglobe_SetMoveToSpeed(QGlobe_RENDER_HANDLE handle , double speed)
{
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	if(pMainMgr == NULL)
		return;
	pMainMgr->m_sOption.nav_moveto_rate = speed;
}

void qglobe_GetRenderTextLableSize(QGlobe_RENDER_HANDLE handle, int nSize)
{
	CQGlobe_DataMgr *pMainMgr = (CQGlobe_DataMgr *) handle;
	if(pMainMgr == NULL)
		return;

	pMainMgr->m_pTextMgr->SetLableSize(nSize);
}

void qglobe_ServerChange()
{
	//
}

QGlobe_ERROR_NO qglobe_IsModified(OUT bool& blModified)
{
	if(g_pGDMDataMgr == NULL)
		return QGlobe_FAIL;
	blModified = g_pGDMDataMgr->IsModified();
	return QGlobe_SUCCESS;
}

QGlobe_ERROR_NO qglobe_CameraGetFrustumInfo(QGlobe_RENDER_HANDLE handle , QGlobe_ViewFrustum& frustumInfo)
{
	if(g_pGDMDataMgr == NULL)
		return QGlobe_FAIL;
	frustumInfo = g_pGDMDataMgr->m_pCamera->m_frustum;
	return QGlobe_SUCCESS;

}

void qglobe_GetOptimzeCameraCoord(IN QGlobe_LOCATION ltPos, IN QGlobe_LOCATION rbPos, OUT QGlobe_Coord3D *pCoord)
{
	QGlobe_ViewFrustum frustumInfo;
	CQGlobe_Point3DF ltPt, rbPt, centerPt, orgPt;
	QGlobe_LOCATION orgPos;
	double len , totalLen, fovy, hFovy;
	qglobe_GetPosition(&ltPos , &ltPt);
	qglobe_GetPosition(&rbPos , &rbPt);
	centerPt = (ltPt + rbPt) * 0.5;
	// determin orientation
	qglobe_GetLocation(&centerPt, &orgPos);
	qglobe_GetStandardCoord(&orgPos, pCoord);
	// determin distance
	qglobe_CameraGetFrustumInfo(g_pGDMDataMgr, frustumInfo);
	fovy = frustumInfo.m_dFovy;
	hFovy = atan(tan(fovy) * frustumInfo.m_dAspect);
	fovy = fovy > hFovy ? hFovy : fovy;
	len = (ltPt - rbPt).getLength() / tan(fovy) * 1.3;
	totalLen = len + centerPt.getLength();
	if(totalLen < (EARTH_RADIUS + 1))
		totalLen = EARTH_RADIUS + 1;
	centerPt.normalize();
	pCoord->m_org = centerPt * totalLen ;
}

bool qglobe_IsBackFace(double x , double y, double z)
{
	if(g_pGDMDataMgr == NULL)
		return false;
	QGlobe_POINT3D vt;
	QGlobe_Coord3D coord;
	qglobe_CameraGetCoord(g_pGDMDataMgr, &coord);

	vt.m_X = coord.m_org.m_tX - x;
	vt.m_Y = coord.m_org.m_tY - y;
	vt.m_Z = coord.m_org.m_tZ - z;
	double scalra = x * vt.m_X + y * vt.m_Y + z * vt.m_Z;

	return scalra <= 0;	
}

bool qglobe_IsPtInDem(CQGlobe_Point3DF pt)
{
	if(g_pGDMDataMgr == NULL)
		return false;
	return g_pGDMDataMgr->m_pCollisionHandler->IsInvaildPos(&pt);
}

#include "QGlobe_RequestMgr.h"
void qglobe_CheckRequest()
{
	int offsetlevl=g_pGDMDataMgr->m_pRequestMgr->GetOffsetLevel();

	if(offsetlevl>0)
		g_pGDMDataMgr->m_pRequestMgr->UpdateRequest();
}
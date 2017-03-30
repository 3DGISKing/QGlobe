#include "GDM_Interface.h"
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_ThreadMgr.h"
#include "GDM_CacheMgr.h"
#include "GDM_FileCacheMgr.h"
#include "GDM_FPSTester.h"
#include "GDM_TextMgr.h"
#include "GDM_RenderTile.h"
#include "GDM_CollisionHandler.h"
#include "GDM_SimpleBuildingCache.h"
#include "gdem_config.h"

extern CGDM_FPSTester g_FPSTester;

extern void gdm_GetPlaceCameraInfo(PLACEMARK_DATA* pData);

GDM_ERROR_NO gdm_CameraInit(GDM_RENDER_HANDLE handle , int nWidth , int nHeight)
{
	if(handle == NULL)
		return GDM_FAIL;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->Initialize(nWidth , nHeight);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraSetViewPortSize(GDM_RENDER_HANDLE handle , int w , int h)
{
	if(handle == NULL)
		return GDM_FAIL;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->SetViewPortSize(w , h);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraGetCoord(GDM_RENDER_HANDLE handle , GDM_Coord3D *pCoord)
{
	if(handle == NULL)
		return GDM_FAIL;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->GetCameraCoord(pCoord);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraSetCoord(GDM_RENDER_HANDLE handle , GDM_Coord3D coord)
{
	if(handle == NULL)
		return GDM_FAIL;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->SetCameraCoord(&coord);
	return GDM_SUCCESS;
}

GDM_ERROR_NO	gdm_CameraGetLocation(GDM_RENDER_HANDLE handle , GDM_LOCATION *pLoc)
{
	if(handle == NULL)
		return GDM_FAIL;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->GetLocation(pLoc);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraRotate(GDM_RENDER_HANDLE handle , double fOffsetAngle , CGDM_Vector3DF *pAxis)
{
	if(handle == NULL)
		return GDM_FAIL;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->RotateAroundVector(fOffsetAngle , pAxis);
	return GDM_SUCCESS;

}

GDM_ERROR_NO gdm_CameraStopAnimation(GDM_RENDER_HANDLE handle)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->StopAnimation();
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraStartAutoRotate(GDM_RENDER_HANDLE handle , GDM_ROT_ANI_INFO* pInfo)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->StartAutoRotate(pInfo);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraStartAutoMoveForward(GDM_RENDER_HANDLE handle , GDM_MOVE_FORWARD_ANI_INFO* pInfo)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->StartAutoMoveForward(pInfo);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraStartAutoMoveTo(GDM_RENDER_HANDLE handle , GDM_MOVETO_ANI_INFO* pInfo)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->StartAutoMoveTo(pInfo);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraOnAnimation(GDM_RENDER_HANDLE handle , double deltaTime)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	if(pMainMgr->m_pCamera->OnAnimation(deltaTime))
		return GDM_SUCCESS;
	else
		return GDM_FAIL;
}
GDM_ERROR_NO gdm_CameraStopAuto(GDM_RENDER_HANDLE handle)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->StopAnimation();
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_LocationToScreen(double longitude, double latitude, double height, CGDM_Point3DF* pOutPt)
{
	GDM_POINT3D pt;

	if(g_pGDMDataMgr == NULL)
		return GDM_SUCCESS;

	gdm_get3DWorldPoint(longitude, latitude, pt, height);
	pOutPt->m_tX = pt.m_X;
	pOutPt->m_tY = pt.m_Y;
	pOutPt->m_tZ = pt.m_Z;

	g_pGDMDataMgr->m_pCamera->WorldToScreen(pOutPt);

	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_WorldToScreen(INOUT CGDM_Point3DF* pOutPt)
{
	if(g_pGDMDataMgr == NULL)
		return GDM_SUCCESS;
	g_pGDMDataMgr->m_pCamera->WorldToScreen(pOutPt);
	return GDM_SUCCESS;
}


GDM_ERROR_NO gdm_ScreenToCamera(GDM_RENDER_HANDLE handle , double* pScalar)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->ScreenToCamera(pScalar);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_ScreenToCamera(GDM_RENDER_HANDLE handle , CGDM_Vector3DF *pVector)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->ScreenToCamera(pVector);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_GetHitLocation(int x , int y , CGDM_Location3D* pPt)
{
	if(g_pGDMDataMgr == NULL)
		return GDM_SUCCESS;
	return g_pGDMDataMgr->m_pCamera->GetHitLocation(x , y , pPt);
}

GDM_ERROR_NO gdm_CameraGetInfo(GDM_RENDER_HANDLE handle , GDM_CAMERA_INFO *pCameraInfo, char bMode)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->GetInfo(pCameraInfo, bMode);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraSetInfo(GDM_RENDER_HANDLE handle , GDM_CAMERA_INFO *pCameraInfo, char bMode)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCamera->SetInfo(pCameraInfo, bMode);
	return GDM_SUCCESS;
}


GDM_ERROR_NO gdm_CameraGetHitVector(GDM_RENDER_HANDLE handle , int x , int y , CGDM_Vector3DF* pVt)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	return pMainMgr->m_pCamera->GetHitVector( x, y, pVt);
}


/********************************************************************************
*								Cache Processing								*
********************************************************************************/

GDM_ERROR_NO gdm_CacheMemClear(GDM_RENDER_HANDLE handle)
{
	if(handle == NULL)
		return GDM_SUCCESS;
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCacheMgr->Clear();
	pMainMgr->SetModifyFlag();
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CacheChangeSize(GDM_RENDER_HANDLE handle, int nMemSize, int nFileSize)
{

	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pFileCacheMgr->SetCacheFileSize(nFileSize);
	pMainMgr->m_pCacheMgr->SetMaxSize(nMemSize);
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CacheFileClear(GDM_RENDER_HANDLE handle)
{
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pFileCacheMgr->Clear();
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_SimpleBuildingCacheClear(GDM_RENDER_HANDLE handle)
{
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	pMainMgr->m_pCacheMgr->GetCacheMgr(E_GDM_MAINDATA_TYPE_3D)->Clear();
	pMainMgr->m_pSimpleBuildingCache->Clear();
	return GDM_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

void gdm_GetFSP(GDM_RENDER_HANDLE handle , int& fsp, int &real_fsp, int &meshs, int &lines)
{
	fsp = g_FPSTester.m_curFrame;
	real_fsp = g_FPSTester.m_realFrame;
	meshs = g_FPSTester.m_totalMeshCount;
	lines = g_FPSTester.m_totalLineCount;
}

void gdm_GetRenderOption(GDM_RENDER_HANDLE handle , GDM_OPTION* option)
{
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	if(pMainMgr == NULL)
		return;
	*option = pMainMgr->m_sOption;
}

void gdm_GetPlacemarkViewerData(GDM_RENDER_HANDLE handle, PLACEMARK_DATA* pData)
{
	gdm_GetPlaceCameraInfo(pData);
}

GDM_ERROR_NO gdm_GetLength(GDM_RENDER_HANDLE handle , GDM_LOCATION *pStart, GDM_LOCATION *pEnd , double* length)
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

	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_HitPoint(CGDM_Point3DF orgPt , CGDM_Vector3DF vt , double rad , CGDM_Point3DF* pPt)
{
	double alpha , alpha1 , alpha2 ,cosVal;
	CGDM_Vector3DF vt1 , vt2 , vt3;
	GDM_Coord3D coord;
	long double l;
	vt.normalize();
	GDM_ERROR_NO retVal = GDM_SUCCESS;
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
		retVal = GDM_FAIL;
	else
	{
		t1 = (-b - sqrt(d)) / (2 * a);
		t2 = (-b + sqrt(d)) / (2 * a);
		if(t1 > 0 && t2 > 0)
		{
			t = t1 < t2 ? t1 : t2;
			if(gdm_iszero(t))
			{
				t = t1 < t2 ? t2 : t1;
			}
		}
		else 
			t = t1 < t2 ? t2 : t1;

		if( (d < 0) || (t < 0))
		{
			retVal = GDM_FAIL;
		}
		else
		{
			*pPt = orgPt + vt * t;
			retVal = GDM_SUCCESS;
		}
	}
	if(retVal == GDM_FAIL)
	{
		gdm_CameraGetCoord(g_pGDMDataMgr , &coord);
		vt1 = -coord.m_org; // org
		vt1.normalize();
		vt2 = vt; // hit vector
		vt3 = vt1.crossProduct(vt2).normalize();
		if(vt3.getLength() > 0.8) // if not vt1 // vt2 ->
		{
			gdm_GetAngleBetweenTwoVector(vt1 , vt2 , vt3 , alpha);
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
			gdm_RotateAroundVector(&vt2 , -alpha2 , vt3);
			*pPt = coord.m_org + vt2;

		}
		else
		{
			GDM_LOCATION loc;
			loc.m_dDist = EARTH_RADIUS;
			loc.m_dLatitude = loc.m_dLongitude = 0;
			gdm_GetPosition(&loc , pPt);
		}
	}
	return retVal;
}

GDM_ERROR_NO gdm_HitPoint(CGDM_Point3DF orgPt , CGDM_Vector3DF vt , CGDM_Point3DF* pPt, bool isDem)
{
	GDM_ERROR_NO retVal;
	vt.normalize();
	retVal = GDM_FAIL;
#if 0
	if(isDem && g_pGDMDataMgr->m_sOption.is_LayerTerrain)
	{
		int x , y;
		double pos3D[3]; 
		CGDM_Point3DF targetPt = orgPt + vt * EARTH_RADIUS;
		gdm_WorldToScreen(g_pGDMDataMgr , &targetPt);
		x = targetPt.m_tX;
		y = targetPt.m_tY;
		g_pGDMDataMgr->GetGDMSceneManager()->RenderDevice()->getVideoDriver()->screenToWorld(x, y, pos3D);
		g_pGDMDataMgr->m_pCamera->GetCameraCoord(&coord);
		CGDM_Vector3DF pt(pos3D[0], -pos3D[1], pos3D[2]);
		pt += coord.m_org;
		if (pt.getLength() > EARTH_RADIUS * 1.3)
			retVal = GDM_FAIL;
		else
		{
			pPt->set(pt.m_tX, pt.m_tY, pt.m_tZ);
			retVal = GDM_SUCCESS;
		}

	}
	else
#endif

		if(isDem && g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
		{			
#if USE_FAST_HITPOINT			
			retVal = gdm_intersectRayDEM2(orgPt, vt, pPt);
#else
			retVal = gdm_intersectRayDEM(orgPt, vt, pPt);
#endif
		}

		if(retVal == GDM_FAIL)
			retVal = gdm_HitPoint(orgPt, vt , EARTH_RADIUS, pPt);
		return retVal;
}

void gdm_SetMoveToSpeed(GDM_RENDER_HANDLE handle , double speed)
{
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	if(pMainMgr == NULL)
		return;
	pMainMgr->m_sOption.nav_moveto_rate = speed;
}

void gdm_GetRenderTextLableSize(GDM_RENDER_HANDLE handle, int nSize)
{
	CGDM_DataMgr *pMainMgr = (CGDM_DataMgr *) handle;
	if(pMainMgr == NULL)
		return;

	pMainMgr->m_pTextMgr->SetLableSize(nSize);
}

void gdm_ServerChange()
{
	//
}

GDM_ERROR_NO gdm_IsModified(OUT bool& blModified)
{
	if(g_pGDMDataMgr == NULL)
		return GDM_FAIL;
	blModified = g_pGDMDataMgr->IsModified();
	return GDM_SUCCESS;
}

GDM_ERROR_NO gdm_CameraGetFrustumInfo(GDM_RENDER_HANDLE handle , GDM_ViewFrustum& frustumInfo)
{
	if(g_pGDMDataMgr == NULL)
		return GDM_FAIL;
	frustumInfo = g_pGDMDataMgr->m_pCamera->m_frustum;
	return GDM_SUCCESS;

}

void gdm_GetOptimzeCameraCoord(IN GDM_LOCATION ltPos, IN GDM_LOCATION rbPos, OUT GDM_Coord3D *pCoord)
{
	GDM_ViewFrustum frustumInfo;
	CGDM_Point3DF ltPt, rbPt, centerPt, orgPt;
	GDM_LOCATION orgPos;
	double len , totalLen, fovy, hFovy;
	gdm_GetPosition(&ltPos , &ltPt);
	gdm_GetPosition(&rbPos , &rbPt);
	centerPt = (ltPt + rbPt) * 0.5;
	// determin orientation
	gdm_GetLocation(&centerPt, &orgPos);
	gdm_GetStandardCoord(&orgPos, pCoord);
	// determin distance
	gdm_CameraGetFrustumInfo(g_pGDMDataMgr, frustumInfo);
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

bool gdm_IsBackFace(double x , double y, double z)
{
	if(g_pGDMDataMgr == NULL)
		return false;
	GDM_POINT3D vt;
	GDM_Coord3D coord;
	gdm_CameraGetCoord(g_pGDMDataMgr, &coord);

	vt.m_X = coord.m_org.m_tX - x;
	vt.m_Y = coord.m_org.m_tY - y;
	vt.m_Z = coord.m_org.m_tZ - z;
	double scalra = x * vt.m_X + y * vt.m_Y + z * vt.m_Z;

	return scalra <= 0;	
}

bool gdm_IsPtInDem(CGDM_Point3DF pt)
{
	if(g_pGDMDataMgr == NULL)
		return false;
	return g_pGDMDataMgr->m_pCollisionHandler->IsInvaildPos(&pt);
}

#include "GDM_RequestMgr.h"
void gdm_CheckRequest()
{
	int offsetlevl=g_pGDMDataMgr->m_pRequestMgr->GetOffsetLevel();

	if(offsetlevl>0)
		g_pGDMDataMgr->m_pRequestMgr->UpdateRequest();
}
#include "GDM_Ruler.h"
#include "GDM_Interface.h"
#include "CSceneManager.h"
#include "geoEngine.h"
#include "GDM_DataMgr.h"

#define	DEBUG_TERRAIN

#ifdef	DEBUG_TERRAIN
	#include "GDM_LibMain.h"
#endif

void CGDM_Ruler::GetMeasureSceneNode(GDM_SceneManager* sceneMgr)
{
	geo::GeoDevice			*pDevice	= sceneMgr->RenderDevice();
	geo::scene::CSceneManager	*pSceneMgr	= (geo::scene::CSceneManager *)pDevice->getSceneManager();

	m_pRulerNode = (geo::scene::CRulerSceneNode *)pSceneMgr->GetRulerNode ();
}

void CGDM_Ruler::Clear ()
{
	if (m_pRulerNode)
		m_pRulerNode->Clear ();
}

void CGDM_Ruler::StepPoint(GDM_LOCATION *a_pLocation, int nStep, bool blLine)
{
	if (m_pRulerNode == NULL)
		return;

	if (blLine)
	{
		if (nStep == 0)
			m_pRulerNode->m_locStart = *a_pLocation;

		m_pRulerNode->m_locEnd = *a_pLocation;
	}
	else
	{
		m_pRulerNode->m_nStepPath = nStep;

		if (nStep == 0)
		{
			m_pRulerNode->m_aryLocation.push_back (*a_pLocation);
		}

		m_pRulerNode->m_aryLocation.push_back (*a_pLocation);
	}
}

void CGDM_Ruler::MovePoint(GDM_LOCATION *a_pLocation, int nStep, bool blLine)
{
	if (blLine)
	{
		if (nStep == 1)
			m_pRulerNode->m_locEnd = *a_pLocation;
	}
	else
	{
		if (nStep < (int)m_pRulerNode->m_aryLocation.size ())
			m_pRulerNode->m_aryLocation[nStep] = *a_pLocation;
	}
}

void CGDM_Ruler::Release (int nStep, bool blLine)
{
	if (blLine)
	{
		if (nStep == 1)
			m_pRulerNode->m_locEnd = m_pRulerNode->m_locStart;
	}
	else
	{
		if (nStep > 1)
			m_pRulerNode->m_aryLocation[nStep] = m_pRulerNode->m_aryLocation[nStep - 1];
	}
}

bool CGDM_Ruler::GetPointFromLocation (GDM_LOCATION *a_pLocation, CGDM_Point3DF	*a_pPoint, double dHeight)
{
	GDM_POINT3D		ptWorld;

	gdm_get3DBodyPoint (a_pLocation->m_dLongitude, a_pLocation->m_dLatitude, ptWorld, dHeight, NULL, true);

	a_pPoint->m_tX = ptWorld.m_X;
	a_pPoint->m_tY = ptWorld.m_Y;
	a_pPoint->m_tZ = ptWorld.m_Z;

	return	true;
}

double CGDM_Ruler::CountAllLength (bool blLine)
{
	double	dblLength = 0;

	if (blLine)
	{
		double	dblValue = 0;
		if (gdm_GetLength (NULL, &m_pRulerNode->m_locStart, &m_pRulerNode->m_locEnd, &dblValue) == GDM_SUCCESS)
			dblLength += dblValue;
	}
	else
	{
		int	nPoint = (int)m_pRulerNode->m_aryLocation.size ();

		for (int i=1; i<nPoint; i++)
		{
			GDM_LOCATION	*pStart = &m_pRulerNode->m_aryLocation[i - 1];
			GDM_LOCATION	*pEnd	= &m_pRulerNode->m_aryLocation[i];

			double	dblValue = 0;
			if (gdm_GetLength (NULL, pStart, pEnd, &dblValue) == GDM_SUCCESS)
				dblLength += dblValue;
		}
	}

	return	dblLength;
}

double CGDM_Ruler::CountAngle (bool blLine)
{
	double	dblAngle = 0;

	if (blLine)
	{
	/*
		CGDM_Point3DF	ptStart, ptEnd, rotAxis;

		gdm_GetPosition (&m_pRulerNode->m_locStart,	&ptStart);
		gdm_GetPosition (&m_pRulerNode->m_locEnd,	&ptEnd);
		rotAxis = ptStart.crossProduct (ptEnd);

		gdm_GetAngleBetweenTwoVector (ptStart, ptEnd, rotAxis, dblAngle);
		dblAngle *= gdm_RADTODEG;
	*/

		double	dLatitude = m_pRulerNode->m_locEnd.m_dLatitude - m_pRulerNode->m_locStart.m_dLatitude;
		double	dLongitude = m_pRulerNode->m_locEnd.m_dLongitude - m_pRulerNode->m_locStart.m_dLongitude;

		if(gdm_iszero(dLongitude))
			dblAngle = 0;
		else
			dblAngle = -atan (dLatitude / dLongitude);
		dblAngle += gdm_HALF_PI;
		if (dLongitude < 0)
			dblAngle += gdm_PI;
		dblAngle *= gdm_RADTODEG;
	}

	return	dblAngle;
}


#ifdef	DEBUG_TERRAIN
	extern CGDM_DataMgr*	g_pGDMDataMgr;
	extern CGDM_Vector3DF	g_OffsetBodyPt;
#endif

void CGDM_Ruler::AddPoint (geo::core::array<geo::core::vector3df> *a_pPointAry, GDM_LOCATION *a_pLocation, double a_dHeight, double a_fRatio)
{
	CGDM_Point3DF	ptWorld;

#ifndef	DEBUG_TERRAIN
	GetPointFromLocation (a_pLocation, &ptWorld, a_dHeight);
#else
	{
		GDM_POINT3D	ptWork;
		bool		bHeight		= true;
		double		dLatitude	= a_pLocation->m_dLatitude,
					dLongitude	= a_pLocation->m_dLongitude;
		double		dHeight		= a_dHeight;

		if(bHeight && g_pGDMDataMgr->m_sOption.is_LayerTerrain)
		{
			GDM_DEM_TILE* pTile = NULL;
			double	dValue = 0;

			if(pTile == NULL)
				dValue = gdm_getHeight(dLongitude, dLatitude);
			else
				dValue = gdm_getHeightInTile(dLongitude, dLatitude, pTile);

			if (a_dHeight != 0)
				dValue += sqrt (m_pCamera->m_location.m_dDist) * 2;
			if (dHeight < a_dHeight)
				dValue = a_dHeight;

			dHeight += dValue * a_fRatio;
		}

		double	dRadius = EARTH_RADIUS + dHeight;
		double	dXRadius = dRadius * cos(dLatitude);
		ptWork.m_Z = dRadius * sin(dLatitude);
		ptWork.m_X = dXRadius * cos(dLongitude);
		ptWork.m_Y = dXRadius * sin(dLongitude);

	#ifdef GDM_RENDER_APPLY_OFFSET_BODY
		ptWork.m_Z -= g_OffsetBodyPt.m_tZ;
		ptWork.m_X -= g_OffsetBodyPt.m_tX;
		ptWork.m_Y -= g_OffsetBodyPt.m_tY;
	#endif

		ptWorld.m_tX = ptWork.m_X;
		ptWorld.m_tY = ptWork.m_Y;
		ptWorld.m_tZ = ptWork.m_Z;
	}
#endif

	geo::core::vector3df	irrPoint;

	irrPoint.X =  ptWorld.m_tX;
	irrPoint.Y = -ptWorld.m_tY;
	irrPoint.Z =  ptWorld.m_tZ;

	a_pPointAry->push_back (irrPoint);
}

int	CGDM_Ruler::GetRulerSegPoints(void *pPoint, GDM_LOCATION *pStart, GDM_LOCATION *pEnd)
{
	geo::core::array<geo::core::vector3df> *pPointAry = (geo::core::array<geo::core::vector3df> *)pPoint;
	pPointAry->clear ();

	double	dHeight = 0;
	int		nDiv = GetValidDivid (pStart, pEnd, dHeight);

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

	GDM_LOCATION	locNode;
	AddPoint (pPointAry, pStart);

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
		AddPoint (pPointAry, &locNode, dHeight, sin (gdm_HALF_PI * i / nPts));
	}

	for (int i=nPts+1; i<nDiv - nPts; i++)
	{
		point = ptStart;
		dAngNode += dAngStep;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		gdm_GetLocation (&point, &locNode);
		AddPoint (pPointAry, &locNode, dHeight);
	}

	dAngNode = dAngStep/2 - nPts * dAngStep;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptEnd;

		gdm_RotateAroundVector (&point, dAngNode, rotAxis);
		gdm_GetLocation (&point, &locNode);
		AddPoint (pPointAry, &locNode, dHeight, sin (gdm_HALF_PI * (nPts - i) / nPts));
	}

	AddPoint (pPointAry, pEnd);

	return	(int)pPointAry->size ();
}

int	CGDM_Ruler::GetValidDivid(GDM_LOCATION *pStart, GDM_LOCATION *pEnd, double	&dHeight)
{
	#define		MIN_DIVIDE		2
	#define		MAX_DIVIDE		1024

	dHeight	= 0;

	double	dblDist = m_pCamera->m_location.m_dDist;
	double	dL		= dblDist * EARTH_RADIUS / (MIN_DIVIDE * CAMERA_DIST_MAX * 10);
	double	dAngUnit = atan(dL / EARTH_RADIUS);

	double			dAngle = 0;
	CGDM_Point3DF	ptStart, ptEnd, rotAxis;

	gdm_GetPosition (pStart, &ptStart);
	gdm_GetPosition (pEnd,	 &ptEnd);
	rotAxis = ptStart.crossProduct (ptEnd);

	gdm_GetAngleBetweenTwoVector (ptStart, ptEnd, rotAxis, dAngle);

	if (fabs(dAngle) > gdm_PI)
	{
		if (dAngle < 0)
			dAngle += gdm_PI;
		else
			dAngle -= gdm_PI;
	}

	double	nDiv = dAngle / dAngUnit;

	if (nDiv < MIN_DIVIDE)
		nDiv = MIN_DIVIDE;
	if (nDiv > MAX_DIVIDE)
		nDiv = MAX_DIVIDE;

	dHeight	= EARTH_RADIUS / cos (dAngle / nDiv) - EARTH_RADIUS;

	return	nDiv;
}
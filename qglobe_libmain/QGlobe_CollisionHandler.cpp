#include "QGlobe_CollisionHandler.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_Camera.h"
#include "QGlobe_Interface.h"

#define COLLISION_RADIUS	4

CQGlobe_CollisionHandler::CQGlobe_CollisionHandler(void)
{
}

CQGlobe_CollisionHandler::~CQGlobe_CollisionHandler(void)
{
}

//if given point is inside earth surface ,get new point on earth surface

QGlobe_ERROR_NO CQGlobe_CollisionHandler::ValidatePos(INOUT CQGlobe_Point3DF *Pt)
{
	double maxHeight = g_pGDMDataMgr->m_sOption.dem_detail_rate * QGlobe_MAX_HEIGHT;

	if(g_pGDMDataMgr->m_pCamera->m_location.m_dDist < maxHeight)
	{
		QGlobe_LOCATION loc;

		qglobe_GetLocation(Pt, &loc);

	    int height  = 0;
		height = qglobe_IntersectHeight(loc.m_dLongitude, loc.m_dLatitude); //???
		
		double CollisionRad = COLLISION_RADIUS;

		if(loc.m_dDist< height+CollisionRad)
		{
			g_pGDMDataMgr->m_pCamera->m_frustum.m_dNear = 1;
			loc.m_dDist = height + COLLISION_RADIUS;

			qglobe_GetPosition(&loc , Pt);
			return QGlobe_SUCCESS;
		}
	}

	return QGlobe_FAIL; // NO COLLISION
}

//test whether given point is inside earth surface

bool CQGlobe_CollisionHandler::IsInvaildPos(CQGlobe_Point3DF *Pt)
{
	double maxHeight = g_pGDMDataMgr->m_sOption.dem_detail_rate * QGlobe_MAX_HEIGHT;

	if(g_pGDMDataMgr->m_pCamera->m_location.m_dDist < maxHeight)
	{
		QGlobe_LOCATION loc;

		qglobe_GetLocation(Pt, &loc);

		int height  = 0;
		height = qglobe_getMaxHeight(loc.m_dLongitude, loc.m_dLatitude);

		double CollisionRad = COLLISION_RADIUS;

		if (loc.m_dDist<height+CollisionRad)
			return true;
	}

	return false;
}

#include "GDM_CollisionHandler.h"
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_Interface.h"

#define COLLISION_RADIUS	4

CGDM_CollisionHandler::CGDM_CollisionHandler(void)
{
}

CGDM_CollisionHandler::~CGDM_CollisionHandler(void)
{
}

//if given point is inside earth surface ,get new point on earth surface

GDM_ERROR_NO CGDM_CollisionHandler::ValidatePos(INOUT CGDM_Point3DF *Pt)
{
	double maxHeight = g_pGDMDataMgr->m_sOption.dem_detail_rate * GDM_MAX_HEIGHT;

	if(g_pGDMDataMgr->m_pCamera->m_location.m_dDist < maxHeight)
	{
		GDM_LOCATION loc;

		gdm_GetLocation(Pt, &loc);

	    int height  = 0;
		height = gdm_IntersectHeight(loc.m_dLongitude, loc.m_dLatitude); //???
		
		double CollisionRad = COLLISION_RADIUS;

		if(loc.m_dDist< height+CollisionRad)
		{
			g_pGDMDataMgr->m_pCamera->m_frustum.m_dNear = 1;
			loc.m_dDist = height + COLLISION_RADIUS;

			gdm_GetPosition(&loc , Pt);
			return GDM_SUCCESS;
		}
	}

	return GDM_FAIL; // NO COLLISION
}

//test whether given point is inside earth surface

bool CGDM_CollisionHandler::IsInvaildPos(CGDM_Point3DF *Pt)
{
	double maxHeight = g_pGDMDataMgr->m_sOption.dem_detail_rate * GDM_MAX_HEIGHT;

	if(g_pGDMDataMgr->m_pCamera->m_location.m_dDist < maxHeight)
	{
		GDM_LOCATION loc;

		gdm_GetLocation(Pt, &loc);

		int height  = 0;
		height = gdm_getMaxHeight(loc.m_dLongitude, loc.m_dLatitude);

		double CollisionRad = COLLISION_RADIUS;

		if (loc.m_dDist<height+CollisionRad)
			return true;
	}

	return false;
}

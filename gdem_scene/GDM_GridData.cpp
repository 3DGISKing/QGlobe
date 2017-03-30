#include <QString>
#include <string.h>
#include "GDM_Define.h"
#include "GDM_PyramidMgr.h"
#include "GDM_Camera.h"
#include "GDM_CacheMgr.h"
#include "GDM_RequestMgr.h"
#include "GeoGridSceneNode.h"
#include "GDM_TextMgr.h"
#include "GDM_FPSTester.h"
#include "GDM_DataMgr.h"

#define MAX_RESOLUTION gdm_DEGTORAD*10 /* degress */

#define EQUATOR					0*gdm_DEGTORAD
#define TROPIC_OF_CANCER		23.5*gdm_DEGTORAD
#define TROPIC_OF_CAPRICORN		-23.5*gdm_DEGTORAD
#define ARCTIC_CIRCLE			66.5*gdm_DEGTORAD
#define ANTARCTIC_CIRCLE		-66.5*gdm_DEGTORAD
#define FIRST_MERIDIAN			0*gdm_DEGTORAD
#define MERIDIAN				180*gdm_DEGTORAD

#define MIN_RADIUS 				gdm_DEGTORAD*0.0036
#define MAX_RADIUS				gdm_DEGTORAD*10.0

#define  MAX_RESOLUTION_ANGLE		0.05	// radian

#define gGRID_POLE_MAX_LONGITUDE_RESOLUTION 37
#define gGRID_POLE_MAX_LATITUDE_RESOLUTION 22
#define gGRID_POLE_MIN_LONGITUDE_RESOLUTION 16
#define gGRID_POLE_MIN_LATITUDE_RESOLUTION 7

#define gGRID_GEN_MAX_LONGITUDE_RESOLUTION 17
#define gGRID_GEN_MAX_LATITUDE_RESOLUTION 17

#define gGRID_GEN_MIN_LONGITUDE_RESOLUTION 7
#define gGRID_GEN_MIN_LATITUDE_RESOLUTION 7

double gGRID_OFFSET_RADIUS;
#define GRID_MAX_OFFSET_RADIUS			4000

#define GRID_UNKNOWN 200.0

// speed
#define MAX_COUNT 36

GDM_GRID_VIEW_INFO g_gridViewInfo = {
		MAX_RADIUS,
		MAX_RADIUS,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0,
		0.0, 0.0, 0.0, 0.0,
		0, 0,
		gGRID_POLE_MAX_LONGITUDE_RESOLUTION,gGRID_POLE_MAX_LATITUDE_RESOLUTION,
		gGRID_POLE_MIN_LONGITUDE_RESOLUTION,gGRID_POLE_MIN_LATITUDE_RESOLUTION,
};

typedef struct _GridVtx
{
	double dLongi;
	double dLati;
}GridVtx;

enum{
	NONE_DIV = 0x00,	
	LONGI_DIV=0x01,
	LATI_DIV=0x02,
};

/* copy in guiuty.cpp */
#define OPTION_LL_DMS				0
#define OPTION_LL_10NOTATION		1
#define OPTION_LL_10NOTATION_DM		2

#define DELTA				(1E-8)
#define GEO_NOT_TYPE		-100
#define GEO_TYPE_LONGITUDE	1
#define GEO_TYPE_LATITUDE	2

static void degree2DMS(double& degree, bool& sign, unsigned int& d, unsigned int& m, double& s)
{
	sign = true;
	if (degree < 0)
	{
		degree *= -1;
		sign = false;
	}

	d = (unsigned int) (degree + DELTA);
	m = (unsigned int)((degree - d) * 60  + DELTA);
	s = qAbs(((degree - d) * 3600) - (60 * m));
}

static void degree2DM(double& degree, bool& sign, unsigned int& d, double& m)
{
	sign = true;
	if (degree < 0)
	{
		degree *= -1;
		sign = false;
	}

	d = (unsigned int) (degree + DELTA);
	m = (degree - d) * 60;
}

static void AddDrawingText(double longi, double lati, const unsigned short* str)
{
	CGDM_Location3D loc;

	loc.m_tX = longi;
	loc.m_tY = lati;
	loc.m_tZ = 0;

	gdm_GetDataMgr()->m_pTextMgr->AddText(E_GDM_SUBDATA_TYPE_NONE,loc, RTT_GRID, str);		
}

QString grid_Degree2DMS(double degree, unsigned int type)
{
	bool sign;
	unsigned int d,m;
	double s;
	unsigned ss;

	degree2DMS(degree, sign, d, m, s);

	ss = (int)s;

	QString res;
	if(ss != 0)
	{
		switch(type)
		{
			case GEO_TYPE_LONGITUDE:
				res.sprintf("%c%d%c%d\'%2d\"", (sign?'E':'W'),d, 0x00B0, m, ss);
				break;
			case GEO_TYPE_LATITUDE:
				res.sprintf("%c%d%c%d\'%2d\"", (sign?'N':'S'), d, 0x00B0, m, ss);
				break;
			default:
				break;
		}
	}
	else if(m != 0)
	{
		switch(type)
		{
			case GEO_TYPE_LONGITUDE:
				res.sprintf("%c%d%c%d\'", (sign?'E':'W'),d, 0x00B0, m);
				break;
			case GEO_TYPE_LATITUDE:
				res.sprintf("%c%d%c%d\'", (sign?'N':'S'), d, 0x00B0, m);
				break;
			default:
				break;
		}
	}
	else
	{
		switch(type)
		{
			case GEO_TYPE_LONGITUDE:
				res.sprintf("%c%d%c", (sign?'E':'W'),d, 0x00B0);
				break;
			case GEO_TYPE_LATITUDE:
				res.sprintf("%c%d%c", (sign?'N':'S'), d, 0x00B0);
				break;
			default:
				break;
		}
	}
	return res;
}

QString grid_Degree2DM(double degree, unsigned int type)
{
	bool sign;
	unsigned int d;
	double m;

	degree2DM(degree, sign, d, m);

	QString res;
	switch(type)
	{
		case GEO_TYPE_LONGITUDE:
			res.sprintf("%c%d%c%.3lf\'", (sign?'E':'W'),d, 0x00B0, m );
			break;
		case GEO_TYPE_LATITUDE:
			res.sprintf("%c%d%c%.3lf\'", (sign?'N':'S'), d, 0x00B0, m);
			break;
		default:
			break;
	}
	return res;
}

QString grid_Degree2Degree(double degree, unsigned int type)
{
	bool sign;
	unsigned int d;
	double m;

	degree2DM(degree, sign, d, m);

	QString res;
	switch(type)
	{
		case GEO_TYPE_LONGITUDE:
			res.sprintf("%c%.4lf%c", (sign?'E':'W'),degree,0x00B0);
			break;
		case GEO_TYPE_LATITUDE:
			res.sprintf("%c%.4lf%c", (sign?'N':'S'),degree,0x00B0);			
			break;
	}
	return res;
}


//==SDOC========================================================================
//==
//==  grid_CalcDistanceBetweenTwoGridVtx
//==
//==============================================================================
//
//  Parameters:
//    vtx1:
//      one point of two points.
//    vtx2:
//      two point of two points.
//
//  Description:
//      Calculates of distance between two grid vtx.
//
//==EDOC========================================================================
double grid_CalcDistanceBetweenTwoGridVtx(GridVtx& vtx1, GridVtx& vtx2)
{
	double dDistance = acos(sin(vtx1.dLati)*sin(vtx2.dLati) + 
                  cos(vtx1.dLati)*cos(vtx2.dLati) *
                  cos(vtx2.dLongi-vtx1.dLongi)) * EARTH_RADIUS;
	return dDistance;
}

//==SDOC========================================================================
//==
//==  grid_CanDiviable
//==
//==============================================================================
//
//  Parameters:
//    pdLongiInterval:
//      Current Longitude Interval.
//    pdLatiInterval:
//      Current Latitude Interval.
//
//  Description:
//      Longitude, Latitude diviable check.
//      --- dDistance1-----(Latitude)
//                                   |
//                                   |
//                                   |
//                           dDistance2(Longitude)
//                                   |
//                                   |
//
//==EDOC========================================================================
int grid_CanDiviable(double* pdLongiInterval, double* pdLatiInterval)
{
	int nRet = NONE_DIV;
	double dCurLongiInterval = *pdLongiInterval;
	double dCurLatiInterval = *pdLatiInterval;

	// Checks hori/vert of tile
	double dCameraLongitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLongitude;
	double dCameraLatitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLatitude;

	double dLongitudeInCamera1 = ((int)(dCameraLongitude/ dCurLongiInterval))*dCurLongiInterval;
	double dLatitudeInCamera1= ((int)(dCameraLatitude/ dCurLatiInterval))*dCurLatiInterval;
	double dLongitudeInCamera2 = ((int)(dCameraLongitude/ dCurLongiInterval)+1)*dCurLongiInterval;
	double dLatitudeInCamera2= ((int)(dCameraLatitude/ dCurLatiInterval))*dCurLatiInterval;

	GridVtx vtx1, vtx2;
	vtx1.dLongi = dLongitudeInCamera1;
	vtx1.dLati = dLatitudeInCamera1;	
	vtx2.dLongi = dLongitudeInCamera1;
	vtx2.dLati = dLatitudeInCamera1+dCurLatiInterval;

	double dDistance1 = grid_CalcDistanceBetweenTwoGridVtx(vtx1, vtx2);
	vtx1 = vtx2;
	vtx2.dLongi += dCurLongiInterval;	
	double dDistance2 = grid_CalcDistanceBetweenTwoGridVtx(vtx1, vtx2);

	double dRatio = dDistance1 / dDistance2;
	if( dRatio < 0.5)
	{
		*pdLongiInterval = dCurLongiInterval / 2.0;		
		nRet |= LONGI_DIV;
	}
	else if( dRatio > 2.0)
	{	
		*pdLatiInterval = dCurLatiInterval / 2.0;
		nRet |= LATI_DIV;
	}
	
	return nRet;
}

double grid_IsBackVtx(GDM_POINT3D& cameraPos, GDM_POINT3D& gridVtx)
{
	GDM_POINT3D vt;

#ifdef GDM_RENDER_APPLY_OFFSET_BODY
	vt = gridVtx;
	gridVtx.m_X = gridVtx.m_X + cameraPos.m_X;
	gridVtx.m_Y = gridVtx.m_Y + cameraPos.m_Y;
	gridVtx.m_Z = gridVtx.m_Z + cameraPos.m_Z;
#else
	vt.m_X = gridVtx.m_X - cameraPos.m_org.m_tX;
	vt.m_Y = gridVtx.m_Y - cameraPos.m_org.m_tY;
	vt.m_Z = gridVtx.m_Z - cameraPos.m_org.m_tZ;
#endif
	double scalra = gridVtx.m_X * vt.m_X + gridVtx.m_Y * vt.m_Y + gridVtx.m_Z * vt.m_Z;

	return scalra;
}


int grid_CheckLotsOfGridVtx(double* pdLongiInterval, double* pdLatiInterval)
{
	int nRet = NONE_DIV;
	double dCurLongiInterval = *pdLongiInterval;
	double dCurLatiInterval = *pdLatiInterval;
	
	int nLongiCount = g_gridViewInfo.nLongiCount;
	int nLatiCount =  g_gridViewInfo.nLatiCount;

	if( nLongiCount > g_gridViewInfo.nMaxLongiResolution )
	{
		*pdLongiInterval = 2.0*dCurLongiInterval;
		nRet |= LONGI_DIV;
	}
	if( nLatiCount > g_gridViewInfo.nMaxLatiResolution )
	{
		*pdLatiInterval = 2.0*dCurLatiInterval;
		nRet |= LATI_DIV;
	}
	
	return nRet;
}

int grid_CheckFewOfGridVtx(double* pdLongiInterval, double* pdLatiInterval)
{
	int nRet = NONE_DIV;
	double dCurLongiInterval = *pdLongiInterval;
	double dCurLatiInterval = *pdLatiInterval;
	
	int nLongiCount = g_gridViewInfo.nLongiCount;
	int nLatiCount =  g_gridViewInfo.nLatiCount;

	if( nLongiCount < g_gridViewInfo.nMinLongiResolution )
	{
		*pdLongiInterval = 0.5*dCurLongiInterval;
		nRet |= LONGI_DIV;
	}
	if( nLatiCount < g_gridViewInfo.nMinLatiResolution )
	{
		*pdLatiInterval = 0.5*dCurLatiInterval;
		nRet |= LATI_DIV;
	}
	
	return nRet;
}

double grid_FindFitAngle(double dAngle)
{
	double dMaxAngle = MAX_RADIUS;
	double dCurAngle;

	bool sign;
	unsigned int d,m;
	double s;

	dCurAngle = dAngle * gdm_RADTODEG;

	degree2DMS(dCurAngle, sign, d, m, s);

	if(dAngle >= MAX_RADIUS)
		return MAX_RADIUS;

	if(d > 0)
	{
		s = 0;
		m = 0;
		if(d < 2)
			d = 2;
		else if(d < 5)
			d = 5;
		else if(d < 10)
			d = 10;
	}
	else if(m > 0)
	{
		s = 0;
		if (m < 2)
			m = 2;
		else if (m < 5)
			m = 5;
		else if (m < 10)
			m = 10;
		else if (m < 30)
			m = 30;
		else
		{
			d = 1;
			m = 0;
		}
	}
	else 
	{
		if(s < 30)
			s = 30;
		else
		{
			d = 0;
			m = 1;
			s = 0;
		}

	}

	dCurAngle = (d + m / 60.0 + s / 3600.0) * gdm_DEGTORAD;

	return dCurAngle;
}

void grid_GetAngles(double* pdLongiInterval, double* pdLatiInterval)
{
	double dCurLongiInterval = *pdLongiInterval;
	double dCurLatiInterval = *pdLatiInterval;

#if 1 /* Speed */
      if( g_gridViewInfo.dMidMinLongi != -GRID_UNKNOWN)
      {
      		double dRange = g_gridViewInfo.dRealMidMinLongi - g_gridViewInfo.dRealLongiStart;
		dRange += ( g_gridViewInfo.dRealLongiEnd - g_gridViewInfo.dMidMaxLongi);
	
		dCurLongiInterval = dRange / MAX_COUNT;
		*pdLongiInterval = grid_FindFitAngle(dCurLongiInterval);
      }
      else
      {
      		double dRange = ( g_gridViewInfo.dRealLongiEnd - g_gridViewInfo.dRealLongiStart);
		dCurLongiInterval = dRange / MAX_COUNT;
		*pdLongiInterval = grid_FindFitAngle(dCurLongiInterval);
      }

	double dRange = ( g_gridViewInfo.dRealLatiEnd - g_gridViewInfo.dRealLatiStart);
	dCurLatiInterval = dRange / MAX_COUNT;
	*pdLatiInterval = grid_FindFitAngle(dCurLatiInterval);
#else
	grid_UpdateVisibleLines(dCurLongiInterval, dCurLatiInterval);
	int nDiv = NONE_DIV;

	if( (nDiv = grid_CheckFewOfGridVtx(&dCurLongiInterval, &dCurLatiInterval)))
	{
		if( nDiv & LONGI_DIV )
			g_gridViewInfo.nLongiCount = g_gridViewInfo.nLongiCount *2;
		if( nDiv & LATI_DIV )
			g_gridViewInfo.nLatiCount = g_gridViewInfo.nLatiCount *2;				
	}
	else if( (nDiv = grid_CheckLotsOfGridVtx(&dCurLongiInterval, &dCurLatiInterval)) )
	{
		if( nDiv & LONGI_DIV )
			g_gridViewInfo.nLongiCount = g_gridViewInfo.nLongiCount /2;
		if( nDiv & LATI_DIV )
			g_gridViewInfo.nLatiCount = g_gridViewInfo.nLatiCount /2;			
	}
	else
	{
		if( (nDiv = grid_CanDiviable(&dCurLongiInterval, &dCurLatiInterval)) )
		{
			if( nDiv & LONGI_DIV )
				g_gridViewInfo.nLongiCount = 2*g_gridViewInfo.nLongiCount;
			
			if( nDiv & LATI_DIV)
				g_gridViewInfo.nLatiCount = 2*g_gridViewInfo.nLatiCount;

			if( (nDiv = grid_CheckLotsOfGridVtx(&dCurLongiInterval, &dCurLatiInterval)) )
			{
				if( nDiv & LONGI_DIV )
					g_gridViewInfo.nLongiCount = g_gridViewInfo.nLongiCount /2;
				if( nDiv & LATI_DIV )
					g_gridViewInfo.nLatiCount = g_gridViewInfo.nLatiCount /2;			
			}		
			else if( (nDiv = grid_CheckFewOfGridVtx(&dCurLongiInterval, &dCurLatiInterval)) )
			{
				if( nDiv & LONGI_DIV )
					g_gridViewInfo.nLongiCount = g_gridViewInfo.nLongiCount *2;
				if( nDiv & LATI_DIV )
					g_gridViewInfo.nLatiCount = g_gridViewInfo.nLatiCount *2;				
			}
			else
			{
				if( nDiv & LONGI_DIV )
					dCurLongiInterval = 2*dCurLongiInterval;
			
				if( nDiv & LATI_DIV)
					dCurLatiInterval = 2*dCurLatiInterval;
			}
		}
	}

	*pdLongiInterval = dCurLongiInterval;
	*pdLatiInterval = dCurLatiInterval;
			
	if( *pdLongiInterval > MAX_RADIUS )
		*pdLongiInterval = MAX_RADIUS;
	else if( *pdLongiInterval  < MIN_RADIUS )
		*pdLongiInterval = MIN_RADIUS;

	if( *pdLatiInterval > MAX_RADIUS )
		*pdLatiInterval = MAX_RADIUS;
	else if( *pdLatiInterval  < MIN_RADIUS )
		*pdLatiInterval = MIN_RADIUS;
#endif

}

double grid_CalcOffsetRadius(double dLongiInterval, double dLatiInterval)
{	
	double dCenterLongitude, dCenterLatitude;
	if( g_gridViewInfo.dMidMinLongi != -GRID_UNKNOWN)
	{
		dCenterLongitude = g_gridViewInfo.dMinLongi;
		dCenterLatitude = (g_gridViewInfo.dMinLati + g_gridViewInfo.dMaxLati)/2.0;
	}
	else
	{
		dCenterLongitude = (g_gridViewInfo.dMinLongi + g_gridViewInfo.dMaxLongi)/2.0;	
		dCenterLatitude = (g_gridViewInfo.dMinLati + g_gridViewInfo.dMaxLati)/2.0;	
	}
	double dTmpLongitude = dCenterLongitude + dLongiInterval;
	double dTmpLatitude = dCenterLatitude + dLatiInterval;
		
	GDM_POINT3D pt1;	
	gdm_get3DWorldPoint( dCenterLongitude, dCenterLatitude, pt1, 0);
	GDM_POINT3D pt2;	
	gdm_get3DWorldPoint( dCenterLongitude, dTmpLatitude, pt2, 0);
	double dScalar = pt1.m_X*pt2.m_X + pt1.m_Y*pt2.m_Y + pt1.m_Z*pt2.m_Z;
	dScalar = dScalar / (EARTH_RADIUS*EARTH_RADIUS);
	if( dScalar > 1.0)
		dScalar = 1.0;
	double dAngle = acos(dScalar);
	double dOffSet =  EARTH_RADIUS - EARTH_RADIUS * cos(dAngle/2);		
	return dOffSet;
}

bool grid_UpdateGeoInfo(void)
{

	CGDM_Camera *pCamera = gdm_GetDataMgr()->m_pCamera;

	g_gridViewInfo.dMinLongi = pCamera->m_dMinLong1;
	g_gridViewInfo.dMidMinLongi = pCamera->m_dMaxLong1;
	g_gridViewInfo.dMaxLongi = pCamera->m_dMaxLong2;
	g_gridViewInfo.dMidMaxLongi = pCamera->m_dMinLong2;	
	g_gridViewInfo.dMinLati = pCamera->m_dMinLati;
	g_gridViewInfo.dMaxLati = pCamera->m_dMaxLati;

	double dLongiResolution, dLatiResolution;
	
	dLongiResolution =	g_gridViewInfo.dLongiResolution;
	dLatiResolution = g_gridViewInfo.dLatiResolution;	

	g_gridViewInfo.dRealLongiStart = ((int)((g_gridViewInfo.dMinLongi / g_gridViewInfo.dLongiResolution)))*g_gridViewInfo.dLongiResolution;
	g_gridViewInfo.dRealLatiStart = ((int)((g_gridViewInfo.dMinLati / g_gridViewInfo.dLatiResolution)))*g_gridViewInfo.dLatiResolution;
	g_gridViewInfo.dRealLongiEnd = ((int)((g_gridViewInfo.dMaxLongi / g_gridViewInfo.dLongiResolution)+2.0))*g_gridViewInfo.dLongiResolution;
	g_gridViewInfo.dRealLatiEnd = ((int)((g_gridViewInfo.dMaxLati / g_gridViewInfo.dLatiResolution)+2.0))*g_gridViewInfo.dLatiResolution;
	g_gridViewInfo.dRealMidMinLongi = ((int)(g_gridViewInfo.dMidMinLongi/g_gridViewInfo.dLongiResolution))*g_gridViewInfo.dLongiResolution;	
	g_gridViewInfo.dRealMidMaxLongi = ((int)(g_gridViewInfo.dMidMaxLongi/g_gridViewInfo.dLongiResolution)+1.0)*g_gridViewInfo.dLongiResolution;	
	
	//g_gridViewInfo.dRealLongiStart = g_gridViewInfo.dMinLongi;
	//g_gridViewInfo.dRealLatiStart = g_gridViewInfo.dMinLati;
	//g_gridViewInfo.dRealLongiEnd = g_gridViewInfo.dMaxLongi;
	//g_gridViewInfo.dRealLatiEnd = g_gridViewInfo.dMaxLati;
	//g_gridViewInfo.dRealMidMinLongi = g_gridViewInfo.dMidMinLongi;
	//g_gridViewInfo.dRealMidMaxLongi = g_gridViewInfo.dMidMaxLongi;
	
	grid_GetAngles(&dLongiResolution, &dLatiResolution); 	

	g_gridViewInfo.dLongiResolution = dLongiResolution;
	g_gridViewInfo.dLatiResolution = dLatiResolution;
	
//	gGRID_OFFSET_RADIUS = grid_CalcOffsetRadius(dLongiResolution, dLatiResolution);
	gGRID_OFFSET_RADIUS = gdm_GetDataMgr()->m_pCamera->m_location.m_dDist / 10.0;
	if(gGRID_OFFSET_RADIUS > GRID_MAX_OFFSET_RADIUS)
		gGRID_OFFSET_RADIUS = GRID_MAX_OFFSET_RADIUS;
	//gGRID_OFFSET_RADIUS = 0.0;
	return true;
}

void grid_getGeoLongiVtx(geo::core::array<geo::scene::GRID_ONE_LINE>* pGeoLongiArray)
{
	GDM_POINT3D pt;	
	double dLongi, dLati;
	double dRealLongiStart = g_gridViewInfo.dRealLongiStart;
	double dRealLatiStart = g_gridViewInfo.dRealLatiStart;
	double dRealLongiEnd = g_gridViewInfo.dRealLongiEnd;
	double dRealLatiEnd = g_gridViewInfo.dRealLatiEnd;	
	double dLongiResoultion = g_gridViewInfo.dLongiResolution;
	double dLatiResoultion = g_gridViewInfo.dLatiResolution;

	//if(dLongiResoultion > MAX_RESOLUTION_ANGLE)
	//{
	//	while(dLongiResoultion > MAX_RESOLUTION_ANGLE)
	//		dLongiResoultion = dLongiResoultion / 2.0;
	//}
	if(dLatiResoultion > MAX_RESOLUTION_ANGLE)
	{
		while(dLatiResoultion > MAX_RESOLUTION_ANGLE)
			dLatiResoultion = dLatiResoultion / 2.0;
	}

	GDM_POINT3D ptCamera;
	ptCamera.m_X = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tX;
	ptCamera.m_Y = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tY;
	ptCamera.m_Z = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tZ;

	if( g_gridViewInfo.dMidMinLongi != -GRID_UNKNOWN)
	{
		for( double dLongi = dRealLongiStart ; dLongi <= g_gridViewInfo.dRealMidMinLongi; dLongi+= dLongiResoultion )
		{
			geo::scene::GRID_ONE_LINE gridLine;
			geo::core::vector3df vertex;
			for( double dLati = dRealLatiStart; dLati <= dRealLatiEnd; dLati+=dLatiResoultion )
			{
				gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
				vertex.X = pt.m_X;
				vertex.Y = -pt.m_Y;
				vertex.Z = pt.m_Z;
				double dScalar = grid_IsBackVtx(ptCamera, pt);
				if(dScalar < 0)
					gridLine.gridLineArray.push_back(vertex);
			}
			pGeoLongiArray->push_back(gridLine);			
		}

		for( double dLongi = g_gridViewInfo.dRealMidMaxLongi ; dLongi <= g_gridViewInfo.dRealLongiEnd; dLongi+= dLongiResoultion )
		{
			geo::scene::GRID_ONE_LINE gridLine;
			geo::core::vector3df vertex;
			for( double dLati = dRealLatiStart; dLati <= dRealLatiEnd; dLati+=dLatiResoultion )
			{
				gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
				vertex.X = pt.m_X;
				vertex.Y = -pt.m_Y;
				vertex.Z = pt.m_Z;
				double dScalar = grid_IsBackVtx(ptCamera, pt);
				if(dScalar < 0)				
					gridLine.gridLineArray.push_back(vertex);
			}
			pGeoLongiArray->push_back(gridLine);			
		}
	}
	else
	{		
		for(dLongi = dRealLongiStart ; dLongi <= dRealLongiEnd; dLongi+=dLongiResoultion)
		{
			geo::scene::GRID_ONE_LINE gridLine;
			geo::core::vector3df vertex;			
			for(dLati = dRealLatiStart; dLati <= dRealLatiEnd; dLati+=dLatiResoultion)
			{
				gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
				vertex.X = pt.m_X;
				vertex.Y = -pt.m_Y;
				vertex.Z = pt.m_Z;
				double dScalar = grid_IsBackVtx(ptCamera, pt);
				if(dScalar < 0)				
					gridLine.gridLineArray.push_back(vertex);
				if( gdm_iszero(g_gridViewInfo.dMaxLati - dLati) || dLati > g_gridViewInfo.dMaxLati)
					break;
			}
			pGeoLongiArray->push_back(gridLine);
			if( gdm_iszero(g_gridViewInfo.dMaxLongi - dLongi) || dLongi > g_gridViewInfo.dMaxLongi)
				break;
	       }	   	
	}
}

void AddTropicArray(double dLati, geo::core::array<geo::scene::GRID_ONE_LINE>* pGeoTropicArray)
{
	double dRealLongiStart = g_gridViewInfo.dRealLongiStart;
	double dRealLongiEnd = g_gridViewInfo.dRealLongiEnd;
	double dLongiResoultion = g_gridViewInfo.dLongiResolution;
	double dLatiResolution = g_gridViewInfo.dLatiResolution;
	
	double dCameraLongitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLongitude;
	double dCameraLatitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLatitude;

	GDM_POINT3D ptCamera;
	ptCamera.m_X = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tX;
	ptCamera.m_Y = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tY;
	ptCamera.m_Z = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tZ;
	
	geo::scene::GRID_ONE_LINE gridLine;
	geo::core::vector3df vertex;
	GDM_POINT3D pt;	

	if(dLongiResoultion > MAX_RESOLUTION_ANGLE)
	{
		while(dLongiResoultion > MAX_RESOLUTION_ANGLE)
			dLongiResoultion = dLongiResoultion / 2.0;
	}
	if(dLatiResolution > MAX_RESOLUTION_ANGLE)
	{
		while(dLatiResolution > MAX_RESOLUTION_ANGLE)
			dLatiResolution = dLatiResolution / 2.0;
	}

	if( g_gridViewInfo.dMidMinLongi != -GRID_UNKNOWN)
	{
		for(double dLongi = dRealLongiStart; dLongi <= g_gridViewInfo.dMidMinLongi; dLongi+=dLongiResoultion)
		{
			gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
			vertex.X = pt.m_X;
			vertex.Y = -pt.m_Y;
			vertex.Z = pt.m_Z;
			double dScalar = grid_IsBackVtx(ptCamera, pt);
			if(dScalar < 0)
				gridLine.gridLineArray.push_back(vertex);
		}
		for(double dLongi = g_gridViewInfo.dMidMaxLongi; dLongi <= g_gridViewInfo.dMaxLongi; dLongi+=dLongiResoultion)
		{
			gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
			vertex.X = pt.m_X;
			vertex.Y = -pt.m_Y;
			vertex.Z = pt.m_Z;
			double dScalar = grid_IsBackVtx(ptCamera, pt);
			if(dScalar < 0)			
				gridLine.gridLineArray.push_back(vertex);
		}
		pGeoTropicArray->push_back(gridLine);
	}
	else
	{
		for(double dLongi = dRealLongiStart; dLongi <= dRealLongiEnd; dLongi+=dLongiResoultion)
		{
			gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);	
			vertex.X = pt.m_X;
			vertex.Y = -pt.m_Y;
			vertex.Z = pt.m_Z;
			double dScalar = grid_IsBackVtx(ptCamera, pt);
			if(dScalar < 0)
				gridLine.gridLineArray.push_back(vertex);
			if( gdm_iszero(g_gridViewInfo.dMaxLongi - dLongi) || dLongi > g_gridViewInfo.dMaxLongi)
				break;
		}
		pGeoTropicArray->push_back(gridLine);
	}
}

void AddParallelArray(double dLongi, geo::core::array<geo::scene::GRID_ONE_LINE>* pGeoTropicArray)
{
	double dRealLatiStart = g_gridViewInfo.dRealLatiStart;
	double dRealLatiEnd = g_gridViewInfo.dRealLatiEnd;
	double dLatiResoultion = g_gridViewInfo.dLatiResolution;
	double dLongiResoultion = g_gridViewInfo.dLongiResolution;
	double dCameraLongitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLongitude;
	double dCameraLatitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLatitude;

	GDM_POINT3D ptCamera;
	ptCamera.m_X = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tX;
	ptCamera.m_Y = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tY;
	ptCamera.m_Z = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tZ;
	
	geo::scene::GRID_ONE_LINE gridLine;
	geo::core::vector3df vertex;
	GDM_POINT3D pt;	

	if(dLongiResoultion > MAX_RESOLUTION_ANGLE)
	{
		while(dLongiResoultion > MAX_RESOLUTION_ANGLE)
			dLongiResoultion = dLongiResoultion / 2.0;
	}
	if(dLatiResoultion > MAX_RESOLUTION_ANGLE)
	{
		while(dLatiResoultion > MAX_RESOLUTION_ANGLE)
			dLatiResoultion = dLatiResoultion / 2.0;
	}

	for(double dLati = dRealLatiStart; dLati <= dRealLatiEnd; dLati+=dLatiResoultion)
	{
		gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;
		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
			gridLine.gridLineArray.push_back(vertex);
		if( gdm_iszero(g_gridViewInfo.dMaxLati - dLati) || dLati > g_gridViewInfo.dMaxLati)
			break;
	}
	pGeoTropicArray->push_back(gridLine);
}

bool grid_IsInLongiRange(double dLongi)
{
	bool bRet = false;
	if( g_gridViewInfo.dMidMinLongi != -GRID_UNKNOWN )
	{
		if( dLongi >= g_gridViewInfo.dMinLongi && dLongi <= g_gridViewInfo.dMidMinLongi )
			bRet = true;

		if( dLongi >= g_gridViewInfo.dMidMaxLongi && dLongi <= g_gridViewInfo.dMaxLongi )
			bRet = true;
	}
	else
	{
		if( dLongi >= g_gridViewInfo.dMinLongi && dLongi <= (g_gridViewInfo.dMaxLongi + 0.00001) )
			bRet = true;
	}
	return bRet;
}

bool grid_IsInLatiRange(double dLati)
{
	bool bRet = false;
	if( dLati >= g_gridViewInfo.dMinLati && dLati <= g_gridViewInfo.dMaxLongi )
		bRet = true;
	return bRet;
}

void grid_getGeoParallelVtx(geo::core::array<geo::scene::GRID_ONE_LINE>* pGeoTropicArray)
{
	double dRealLongiStart = g_gridViewInfo.dRealLongiStart;
	double dRealLatiStart = g_gridViewInfo.dRealLatiStart;
	double dRealLongiEnd = g_gridViewInfo.dRealLongiEnd;
	double dRealLatiEnd = g_gridViewInfo.dRealLatiEnd;	

	double dCameraLongitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLongitude;
	double dCameraLatitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLatitude;

	unsigned short gridText[32];

	GDM_POINT3D ptCamera;
	GDM_POINT3D pt;
	ptCamera.m_X = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tX;
	ptCamera.m_Y = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tY;
	ptCamera.m_Z = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tZ;
	
	/* MERIDIAN = GYONGSON */
	if( grid_IsInLongiRange(MERIDIAN))
	{
		AddParallelArray(MERIDIAN, pGeoTropicArray);			
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( MERIDIAN, dCameraLatitude, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));

			gridText[0] = 0xB0A0;
			gridText[1] = 0xC790;
			gridText[2] = 0xBC14;
			gridText[3] = 0xB01C;
			gridText[4] = 0xC120;
			AddDrawingText(MERIDIAN, dCameraLatitude, (const unsigned short*)gridText);		
		}
	}
	
	/* FIRST MERIDIAN = BONCHOJAOSON */
	if( grid_IsInLongiRange(FIRST_MERIDIAN))
	{
		AddParallelArray(FIRST_MERIDIAN,pGeoTropicArray);	
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( FIRST_MERIDIAN, dCameraLatitude, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));
			gridText[0] = 0xBCF8;
			gridText[1] = 0xCD08;
			gridText[2] = 0xC790;
			gridText[3] = 0xC624;
			gridText[4] = 0xC120;		
			AddDrawingText(FIRST_MERIDIAN, dCameraLatitude, (const unsigned short*)gridText);				
		}
	}

	/* EQUATOR = JOKDO */
	if(grid_IsInLatiRange(EQUATOR))
	{
		AddTropicArray(EQUATOR,pGeoTropicArray);
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dCameraLongitude, EQUATOR, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));
			gridText[0] = 0xC801;
			gridText[1] = 0xB3C4;
			AddDrawingText(dCameraLongitude, EQUATOR, (const unsigned short*)gridText);								
		}
	}

	/* TROPIC_OF_CANCER = PUKHOIGUISON */
	if(grid_IsInLatiRange(TROPIC_OF_CANCER))
	{
		AddTropicArray(TROPIC_OF_CANCER,pGeoTropicArray);	
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dCameraLongitude, TROPIC_OF_CANCER, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));
			gridText[0] = 0xBD81;
			gridText[1] = 0xD68C;
			gridText[2] = 0xADC0;
			gridText[3] = 0xC120;
			AddDrawingText(dCameraLongitude, TROPIC_OF_CANCER, (const unsigned short*)gridText);						
		}		
	}

	/* TROPIC_OF_CAPRICORN = NAMHOIGUISON */
	if(grid_IsInLatiRange(TROPIC_OF_CAPRICORN))
	{
		AddTropicArray(TROPIC_OF_CAPRICORN,pGeoTropicArray);	
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dCameraLongitude, TROPIC_OF_CAPRICORN, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));
			gridText[0] = 0xB0A8;
			gridText[1] = 0xD68C;
			gridText[2] = 0xADC0;
			gridText[3] = 0xC120;		
			AddDrawingText(dCameraLongitude, TROPIC_OF_CAPRICORN, (const unsigned short*)gridText);								
		}
	}	

	/* ARCTIC_CIRCLE = PUKKUKKWON */
	if(grid_IsInLatiRange(ARCTIC_CIRCLE))
	{
		AddTropicArray(ARCTIC_CIRCLE,pGeoTropicArray);	
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dCameraLongitude, ARCTIC_CIRCLE, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));
			gridText[0] = 0xBD81;
			gridText[1] = 0xADF9;
			gridText[2] = 0xAD8C;
			AddDrawingText(dCameraLongitude, ARCTIC_CIRCLE, (const unsigned short*)gridText);												
		}
	}

	/* ANTARCTIC_CIRCLE = NAMKUKKWON */
	if(grid_IsInLatiRange(ANTARCTIC_CIRCLE))
	{
		AddTropicArray(ANTARCTIC_CIRCLE,pGeoTropicArray);	
		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dCameraLongitude, ANTARCTIC_CIRCLE, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar < 0)
		{
			memset(gridText, 0, sizeof(gridText));	
			gridText[0] = 0xB0A8;
			gridText[1] = 0xADF9;
			gridText[2] = 0xAD8C;	
			AddDrawingText(dCameraLongitude, ANTARCTIC_CIRCLE, (const unsigned short*)gridText);												
		}
	}	
}

void grid_getGeoLatiVtx(geo::core::array<geo::scene::GRID_ONE_LINE>* pGeoLatiArray)
{
       GDM_POINT3D pt;	
	double dLongi, dLati;
	double dRealLongiStart = g_gridViewInfo.dRealLongiStart;
	double dRealLatiStart = g_gridViewInfo.dRealLatiStart;
	double dRealLongiEnd = g_gridViewInfo.dRealLongiEnd;
	double dRealLatiEnd = g_gridViewInfo.dRealLatiEnd;	
	double dLongiResoultion = g_gridViewInfo.dLongiResolution;
	double dLatiResoultion = g_gridViewInfo.dLatiResolution;

	GDM_POINT3D ptCamera;
	ptCamera.m_X = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tX;
	ptCamera.m_Y = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tY;
	ptCamera.m_Z = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tZ;
	
	if(dLongiResoultion > MAX_RESOLUTION_ANGLE)
	{
		while(dLongiResoultion > MAX_RESOLUTION_ANGLE)
			dLongiResoultion = dLongiResoultion / 2.0;
	}
	//if(dLatiResoultion > MAX_RESOLUTION_ANGLE)
	//{
	//	while(dLatiResoultion > MAX_RESOLUTION_ANGLE)
	//		dLatiResoultion = dLatiResoultion / 2.0;
	//}

	for(dLati = dRealLatiStart; dLati <= dRealLatiEnd; dLati+=dLatiResoultion)	
	{
		geo::scene::GRID_ONE_LINE gridLine;
		geo::core::vector3df vertex;
		if( g_gridViewInfo.dMidMinLongi != -GRID_UNKNOWN)
		{
			for(double dLongi = dRealLongiStart; dLongi <= g_gridViewInfo.dRealMidMinLongi; dLongi+=dLongiResoultion)
			{
				gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
				vertex.X = pt.m_X;
				vertex.Y = -pt.m_Y;
				vertex.Z = pt.m_Z;
				double dScalar = grid_IsBackVtx(ptCamera, pt);
				if(dScalar < 0)
					gridLine.gridLineArray.push_back(vertex);
			}			
						
			for(double dLongi = g_gridViewInfo.dRealMidMaxLongi; dLongi <= dRealLongiEnd; dLongi+=dLongiResoultion)
			{
				gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
				vertex.X = pt.m_X;
				vertex.Y = -pt.m_Y;
				vertex.Z = pt.m_Z;
				double dScalar = grid_IsBackVtx(ptCamera, pt);
				if(dScalar < 0)
					gridLine.gridLineArray.push_back(vertex);
			}
			pGeoLatiArray->push_back(gridLine);
		}
		else
		{
			for(dLongi = dRealLongiStart ; dLongi <= dRealLongiEnd; dLongi+=dLongiResoultion)
			{
				gdm_get3DBodyPoint( dLongi, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
				vertex.X = pt.m_X;
				vertex.Y = -pt.m_Y;
				vertex.Z = pt.m_Z;
				double dScalar = grid_IsBackVtx(ptCamera, pt);
				if(dScalar < 0)
					gridLine.gridLineArray.push_back(vertex);
				if( gdm_iszero(g_gridViewInfo.dMaxLongi - dLongi) || dLongi > g_gridViewInfo.dMaxLongi)
					break;
			}
			pGeoLatiArray->push_back(gridLine);
		}		
		if( gdm_iszero(g_gridViewInfo.dMaxLati - dLati) || dLati > g_gridViewInfo.dMaxLati)
			break;
       }	
}

void	grid_getGeoTextVtx(void)
{
	double dCameraLongitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLongitude;
	double dCameraLatitude = gdm_GetDataMgr()->m_pCamera->m_location.m_dLatitude;
	double dLongi, dLati;

	double dRealLongiStart = g_gridViewInfo.dRealLongiStart;
	double dRealLatiStart = g_gridViewInfo.dRealLatiStart;
	double dRealLongiEnd = g_gridViewInfo.dRealLongiEnd;
	double dRealLatiEnd = g_gridViewInfo.dRealLatiEnd;	
	double dLongiResoultion = g_gridViewInfo.dLongiResolution*2.0;
	double dLatiResoultion = g_gridViewInfo.dLatiResolution*2.0;
	GDM_POINT3D pt;
	GDM_POINT3D ptCamera;
	ptCamera.m_X = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tX;
	ptCamera.m_Y = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tY;
	ptCamera.m_Z = gdm_GetDataMgr()->m_pCamera->m_coord.m_org.m_tZ;

	dLongi = - gdm_PI;
	while(dLongi <= dRealLongiEnd)
	{
		dLongi+=dLongiResoultion;
		if(dLongi < dRealLongiStart)
			continue;

		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dLongi, dCameraLatitude, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar > 0)
			continue;

		if( fabs(dLongi) > 180.0*gdm_DEGTORAD)
			continue;	

		QString longi;
		switch( gdm_GetDataMgr()->m_sOption.gridViewInfo.gridMode)
		{
			case OPTION_LL_10NOTATION:
				longi = grid_Degree2Degree(dLongi*gdm_RADTODEG, GEO_TYPE_LONGITUDE);
				break;
			case OPTION_LL_DMS:
				longi = grid_Degree2DMS(dLongi*gdm_RADTODEG, GEO_TYPE_LONGITUDE);
				break;
			case OPTION_LL_10NOTATION_DM:
				longi = grid_Degree2DM(dLongi*gdm_RADTODEG, GEO_TYPE_LONGITUDE);
				break;
		}
		
		AddDrawingText(dLongi, dCameraLatitude, (const unsigned short*)longi.data());
	}

	dLati = - gdm_PI;
	while(dLati <= dRealLatiEnd)
	{
		dLati+=dLatiResoultion;
		if(dLati < dRealLatiStart)
			continue;

		geo::core::vector3df vertex;		
		gdm_get3DBodyPoint( dCameraLongitude, dLati, pt, gGRID_OFFSET_RADIUS, NULL, true);			
		vertex.X = pt.m_X;
		vertex.Y = -pt.m_Y;
		vertex.Z = pt.m_Z;

		double dScalar = grid_IsBackVtx(ptCamera, pt);
		if(dScalar > 0)
			continue;

		if( fabs(dLati) > 90.0*gdm_DEGTORAD)
			continue;

		QString lati;
		switch( gdm_GetDataMgr()->m_sOption.gridViewInfo.gridMode)
		{
			case OPTION_LL_10NOTATION:
				lati = grid_Degree2Degree(dLati*gdm_RADTODEG, GEO_TYPE_LATITUDE);
				break;
			case OPTION_LL_DMS:
				lati = grid_Degree2DMS(dLati*gdm_RADTODEG, GEO_TYPE_LATITUDE);
				break;
			case OPTION_LL_10NOTATION_DM:
				lati = grid_Degree2DM(dLati*gdm_RADTODEG, GEO_TYPE_LATITUDE);
				break;
		}		
		AddDrawingText(dCameraLongitude, dLati, (const unsigned short*)lati.data());
	}
}

void gdm_GetGeoGridVtx(geo::core::array<geo::scene::GRID_ONE_LINE>* pGridArray,
							geo::core::array<geo::scene::GRID_ONE_LINE>* pGridTropicArray )
{
	if( !gdm_GetDataMgr()->m_sOption.gridViewInfo.gridView)
		return;

	/* Updates grid info */
	if( grid_UpdateGeoInfo() )
	{
		/* Get Latitude */
		grid_getGeoLatiVtx(pGridArray);

		/* Get Longitude */
		grid_getGeoLongiVtx(pGridArray);
		/* Get Tropic */
		grid_getGeoParallelVtx(pGridTropicArray);
		/* Get Texts */
		grid_getGeoTextVtx();		
	}
}

int	gdm_CallBackGetGridVtx(void* pUserData, void *param1, void *param2, void *param3)
{
	int ret = 1;
	geo::core::array<geo::scene::GRID_ONE_LINE>*	pGridArray = (geo::core::array<geo::scene::GRID_ONE_LINE>*)param1;
	geo::core::array<geo::scene::GRID_ONE_LINE>*	pGridTropicArray = (geo::core::array<geo::scene::GRID_ONE_LINE>*)param2;
	
	if( pGridArray != NULL && pGridTropicArray != NULL )
	{
		gdm_GetGeoGridVtx(pGridArray, pGridTropicArray);

		// check line count
		for(unsigned int i = 0; i < pGridArray->size(); i++)
		{
			geo::core::array<geo::core::vector3df>* pArray = &(*pGridArray)[i].gridLineArray;
			gdm_GetFPSTester()->m_totalLineCount += pArray->size();
		}
	}
	return ret;
}

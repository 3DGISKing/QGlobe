#include <string.h>
#include "GDM_Define.h"
#include "GDM_Camera.h"
#include "GDM_PyramidMgr.h"
#include "GDM_CacheMgr.h"
#include "GDM_RequestMgr.h"
#include "GDM_DataMgr.h"
#include "GeoShapeSceneNode.h"
#include "GDM_FPSTester.h"

//added by RSH 2013.3.18
#include "../gdem_base/GDM_Line2D.h"
//#include "../gdem_base/GDM_Line3D.h"
//end addition

#include <QList>

#define OFFSET_RADIUS_COUNTRY			500
#define OFFSET_RADIUS_FIRST_ADMIN		0
#define SHAPE_MAX_OFFSET_RADIUS			2000
#define INTERSECTING_TOLERANCE          0.000000000001
#define DIAGONAL_TOLERANCE              0.12
#define SORTING_LONGITUDE_COEFF         10
#define SHAPE_EXACT_RENDER_LEVEL        15

using namespace  geo;
using namespace  core;
void gdm_AddVertexToPolygon(double longitude, double latitude, double offsetRadius, LineSegment* polygonData, GDM_DEM_TILE* pDemTile)
{
	GDM_POINT3D pt;
	gdm_get3DBodyPoint(longitude, latitude, pt, offsetRadius, pDemTile, true);

	geo::core::vector3df vertex;
	vertex.X = pt.m_X;
	vertex.Y = - pt.m_Y;
	vertex.Z = pt.m_Z;
	polygonData->pts.push_back(vertex);
	gdm_GetFPSTester()->m_totalLineCount++;
}

inline double getDistance(double x1,double y1,double x2,double y2)
{
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
//added by RSH 2013.3.17
///*
/******
hypothesis: logic tile explicitly contains starting and ending points.
******/


struct CROSSINGPOINT
{
	CGDM_Vector3DF pt;
	double sortingVal;
};

bool hasBiggerZValue(const CROSSINGPOINT &pt1, const CROSSINGPOINT pt2)
{
	return (pt1.sortingVal < pt2.sortingVal);
}

inline void GetExactPtsInLogicTile(const GDM_LOGIC_TILE *ptile, CGDM_Line2DF* line, 
								   geo::core::array<geo::core::vector3df> *ptArr)
{
	GDM_DEM_TILE * pDem;
	pDem = ptile->pDemTile;
	unsigned int prevX = (unsigned int)((line->m_cStart.m_tX - pDem->minLongitude)/pDem->gridWidth);
	unsigned int prevY = (unsigned int)((line->m_cStart.m_tY - pDem->minLatitude)/pDem->gridWidth);
	unsigned int curX  = (unsigned int)((line->m_cEnd.m_tX - pDem->minLongitude)/pDem->gridWidth);
	unsigned int curY  = (unsigned int)((line->m_cEnd.m_tY - pDem->minLatitude)/pDem->gridWidth);
	

	
	bool longAscend, latAscend, fstInside, sndInside;
	longAscend = (line->m_cStart.m_tX<line->m_cEnd.m_tX);
	latAscend = (line->m_cStart.m_tY<line->m_cEnd.m_tY);

	fstInside = (prevX >= 0 && prevX < GDEM_MAX_DEM_PT_COUNT && prevY >= 0 && prevY < GDEM_MAX_DEM_PT_COUNT);
	sndInside = (curX >= 0 && curX < GDEM_MAX_DEM_PT_COUNT && curY >= 0 && curY < GDEM_MAX_DEM_PT_COUNT);

	int startX = qMin(prevX, curX); startX = qMax(startX+1, 0);
	int startY = qMin(prevY, curY); startY = qMax(startY+1, 0);
	int endX = qMax(prevX, curX); endX = qMin(endX, GDEM_MAX_DEM_DIVISION);
	int endY = qMax(prevY, curY); endY = qMin(endY, GDEM_MAX_DEM_DIVISION);

	QList<CROSSINGPOINT> inPtArr;
	CGDM_Vector2DF startPt, endPt, crossPt;
	CGDM_Line2DF tstLine;
	CROSSINGPOINT crossingPt;
	GDM_POINT3D pos;
	double d, h1=0, h2=0;
	int iX, iY;
	double rate = 1.0, OFFSET_POLYGON = 0.3f;

	if (gdm_GetDataMgr()) rate = gdm_GetDataMgr()->m_sOption.dem_detail_rate;

	//Get intersecting points with latitude lines.
	for (iX = startX; iX < (endX+1); iX++)
	{
		startPt.m_tX = endPt.m_tX = pDem->minLongitude + pDem->gridWidth*iX;
		startPt.m_tY = pDem->minLatitude; endPt.m_tY = pDem->maxLatitude;
		tstLine.setLine(startPt, endPt);
		if (line->intersectWith(tstLine, crossPt, INTERSECTING_TOLERANCE))
		{		
			crossingPt.pt.m_tX = crossPt.m_tX; crossingPt.pt.m_tY = crossPt.m_tY;
			crossingPt.sortingVal = SORTING_LONGITUDE_COEFF*crossPt.m_tX;
			if (!longAscend) crossingPt.sortingVal = -crossingPt.sortingVal;
			if (latAscend) crossingPt.sortingVal += crossPt.m_tY;
			else crossingPt.sortingVal -= crossPt.m_tY;

			iY = (int)((crossPt.m_tY - pDem->minLatitude)/pDem->gridWidth);
			h1 = pDem->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX];
			h2 = pDem->m_pData[(iY + 1)*GDEM_MAX_DEM_PT_COUNT + iX];
			crossingPt.pt.m_tZ = h1 + (h2 - h1)*(crossPt.m_tY - pDem->minLatitude - iY*pDem->gridWidth)/pDem->gridWidth;
			crossingPt.pt.m_tZ *= rate;
			crossingPt.pt.m_tZ += OFFSET_POLYGON;

			inPtArr.push_back(crossingPt);
		}
	}
	//calculate crossing points with longitudinal lines.
	for (iY = startY; iY < (endY+1); iY++)
	{
		startPt.m_tY = endPt.m_tY = pDem->minLatitude + pDem->gridWidth*iY;
		startPt.m_tX = pDem->minLongitude; endPt.m_tX = pDem->maxLongitude;
		tstLine.setLine(startPt, endPt);
		if (line->intersectWith(tstLine, crossPt, INTERSECTING_TOLERANCE))
		{
			crossingPt.pt.m_tX = crossPt.m_tX; crossingPt.pt.m_tY = crossPt.m_tY;
			crossingPt.sortingVal = SORTING_LONGITUDE_COEFF*crossPt.m_tX;
			if (!longAscend) crossingPt.sortingVal = -crossingPt.sortingVal;
			if (latAscend) crossingPt.sortingVal += crossPt.m_tY;
			else crossingPt.sortingVal -= crossPt.m_tY;

			iX = (int)((crossPt.m_tX - pDem->minLongitude)/pDem->gridWidth);
			h1 = pDem->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX];
			h2 = pDem->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX + 1];
			crossingPt.pt.m_tZ = h1 + (h2 - h1)*(crossPt.m_tX - pDem->minLongitude - iX*pDem->gridWidth)/pDem->gridWidth;
			crossingPt.pt.m_tZ *= rate;
			crossingPt.pt.m_tZ += OFFSET_POLYGON;

			inPtArr.push_back(crossingPt);
		}
	}

	//Followings accounts for anti-diagonal lines with equation of y = x + i.
	int startTri, endTri;
	startTri = qMin(prevY - prevX,curY - curX); startTri = qMax(startTri, -GDEM_MAX_DEM_DIVISION);
	endTri   = qMax(prevY - prevX,curY - curX); endTri   = qMin(endTri, GDEM_MAX_DEM_DIVISION);
	//startTri = startTri/2; endTri = endTri/2;

	for (int i=startTri; i< endTri+2; i++)
	{
		if (i%2 != 0) continue;

		startPt.m_tX = pDem->minLongitude; endPt.m_tY = pDem->maxLatitude;
		startPt.m_tY = pDem->minLatitude  + i*pDem->gridWidth;
		endPt.m_tX =   pDem->maxLongitude - i*pDem->gridWidth;
		tstLine.setLine(startPt, endPt);
		if (line->intersectWith(tstLine, crossPt, INTERSECTING_TOLERANCE))
		{
			iX = (int)((crossPt.m_tX - pDem->minLongitude)/pDem->gridWidth);
			iY = (int)((crossPt.m_tY - pDem->minLatitude)/pDem->gridWidth);
			
			float dX = crossPt.m_tX - pDem->minLongitude - iX*pDem->gridWidth;
			if (dX < pDem->gridWidth*DIAGONAL_TOLERANCE) continue;
			dX = pDem->minLongitude + (iX + 1)*pDem->gridWidth - crossPt.m_tX;
			if (dX < pDem->gridWidth*DIAGONAL_TOLERANCE) continue;

			crossingPt.pt.m_tX = crossPt.m_tX; crossingPt.pt.m_tY = crossPt.m_tY;
			crossingPt.sortingVal = SORTING_LONGITUDE_COEFF*crossPt.m_tX;
			if (!longAscend) crossingPt.sortingVal = -crossingPt.sortingVal;
			if (latAscend) crossingPt.sortingVal += crossPt.m_tY;
			else crossingPt.sortingVal -= crossPt.m_tY;

			h1 = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
			h2 = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
			d = (crossPt.m_tX - pDem->minLongitude - iX*pDem->gridWidth)*(crossPt.m_tX - pDem->minLongitude - iX*pDem->gridWidth);
			d += (crossPt.m_tY - pDem->minLatitude - iY*pDem->gridWidth)*(crossPt.m_tY - pDem->minLatitude - iY*pDem->gridWidth);
			d = abs(sqrt(d));
			crossingPt.pt.m_tZ = h1 + (h2 - h1)*d/1.41421356/pDem->gridWidth;
			crossingPt.pt.m_tZ *= rate;
			crossingPt.pt.m_tZ += OFFSET_POLYGON;

			inPtArr.push_back(crossingPt);
		}
	}
	//Major diagonal lines are treated here. The equation is y = -x + i.
	startTri = qMin(prevX + prevY, curX + curY); startTri = qMax(startTri, 0);
	endTri   = qMax(prevX + prevY, curX + curY); endTri   = qMin(endTri, 2*GDEM_MAX_DEM_DIVISION);
	
	for (int i=startTri; i<endTri+2; i++)
	{
		if (i%2 != 0) continue;
		startPt.m_tX = pDem->minLongitude + i*pDem->gridWidth; startPt.m_tY = pDem->minLatitude;
		endPt.m_tX = pDem->minLongitude; endPt.m_tY = pDem->minLatitude + i*pDem->gridWidth;
		tstLine.setLine(startPt, endPt);
		if (line->intersectWith(tstLine, crossPt, INTERSECTING_TOLERANCE))
		{
			iX = (int)((crossPt.m_tX - pDem->minLongitude)/pDem->gridWidth);
			iY = (int)((crossPt.m_tY - pDem->minLatitude)/pDem->gridWidth);

			float dX = crossPt.m_tX - pDem->minLongitude - iX*pDem->gridWidth;
			if (dX < pDem->gridWidth*DIAGONAL_TOLERANCE) continue;
			dX = pDem->minLongitude + (iX + 1)*pDem->gridWidth - crossPt.m_tX;
			if (dX < pDem->gridWidth*DIAGONAL_TOLERANCE) continue;

			crossingPt.pt.m_tX = crossPt.m_tX; crossingPt.pt.m_tY = crossPt.m_tY;
			crossingPt.sortingVal = SORTING_LONGITUDE_COEFF*crossPt.m_tX;
			if (!longAscend) crossingPt.sortingVal = -crossingPt.sortingVal;
			if (latAscend) crossingPt.sortingVal += crossPt.m_tY;
			else crossingPt.sortingVal -= crossPt.m_tY;

			h1 = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
			h2 = pDem->m_pData[(iY + 1)*GDEM_MAX_DEM_PT_COUNT + iX];
			d = (crossPt.m_tX - pDem->minLongitude - (iX + 1)*pDem->gridWidth)*(crossPt.m_tX - pDem->minLongitude - (iX + 1)*pDem->gridWidth);
			d += (crossPt.m_tY - pDem->minLatitude -iY * pDem->gridWidth)*(crossPt.m_tY - pDem->minLatitude -iY * pDem->gridWidth);
			d = abs(sqrt(d));
			crossingPt.pt.m_tZ = h1 + (h2 - h1)*d/1.41421356/pDem->gridWidth;
			crossingPt.pt.m_tZ *= rate;
			crossingPt.pt.m_tZ += OFFSET_POLYGON;

			inPtArr.push_back(crossingPt);
		}
	}
	//If first point is in dem tile, it should be considered.
	if (fstInside)
	{
		crossingPt.pt.m_tX = line->m_cStart.m_tX; crossingPt.pt.m_tY = line->m_cStart.m_tY;
		crossingPt.sortingVal = SORTING_LONGITUDE_COEFF*line->m_cStart.m_tX;
		if (!longAscend) crossingPt.sortingVal = -crossingPt.sortingVal;
		if (latAscend) crossingPt.sortingVal += line->m_cStart.m_tY;
		else crossingPt.sortingVal -= line->m_cStart.m_tY;

		iX = (line->m_cStart.m_tX - pDem->minLongitude)/pDem->gridWidth;
		iY = (line->m_cStart.m_tY - pDem->minLatitude)/pDem->gridWidth;
		float height[3], xcoord[3], ycoord[3], dX, dY;
		
		dX = line->m_cStart.m_tX - pDem->minLongitude - iX*pDem->gridWidth;
		dY = line->m_cStart.m_tY - pDem->minLatitude - iY*pDem->gridWidth;

		if ((iX % 2 == 0 && iY % 2 == 0) || (iX % 2 != 0 && iY %2 != 0))
		{
			if (dX > dY)
			{
				xcoord[0] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[0] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[1] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[2] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				height[0] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
				height[1] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[2] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
			}
			else
			{
				xcoord[0] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[0] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[1] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[2] = pDem->minLatitude + iY*pDem->gridWidth;
				height[0] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[1] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX];
				height[2] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
			}
		}
		else
		{
			if ((dX + dY) < pDem->gridWidth)
			{
				xcoord[0] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[0] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[1] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[2] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				height[0] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[1] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
				height[2] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX];
			}
			else
			{
				xcoord[0] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[0] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[1] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[2] = pDem->minLatitude + iY*pDem->gridWidth;
				height[0] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX];
				height[1] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[2] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
			}
		}
		d = line->m_cStart.m_tX - xcoord[0]; d = abs(d);
		crossingPt.pt.m_tZ = height[0] + ( d*(height[1] - height[0]) + abs(line->m_cStart.m_tY - ycoord[0])*(height[2] - height[1]) )/pDem->gridWidth;
		crossingPt.pt.m_tZ *= rate;
		crossingPt.pt.m_tZ += OFFSET_POLYGON;

		inPtArr.push_back(crossingPt);
	}

	//Provided that last point is belonged to dem tile, it needs same operation.
	if (sndInside)
	{
		crossingPt.pt.m_tX = line->m_cEnd.m_tX; crossingPt.pt.m_tY = line->m_cEnd.m_tY;
		crossingPt.sortingVal = SORTING_LONGITUDE_COEFF*line->m_cEnd.m_tX;
		if (!longAscend) crossingPt.sortingVal = -crossingPt.sortingVal;
		if (latAscend) crossingPt.sortingVal += line->m_cEnd.m_tY;
		else crossingPt.sortingVal -= line->m_cEnd.m_tY;

		iX = (line->m_cEnd.m_tX - pDem->minLongitude)/pDem->gridWidth;
		iY = (line->m_cEnd.m_tY - pDem->minLatitude)/pDem->gridWidth;
		float height[3], xcoord[3], ycoord[3], dX, dY;

		dX = line->m_cEnd.m_tX - pDem->minLongitude - iX*pDem->gridWidth;
		dY = line->m_cEnd.m_tY - pDem->minLatitude - iY*pDem->gridWidth;

		if ((iX % 2 == 0 && iY % 2 == 0) || (iX % 2 != 0 && iY %2 != 0))
		{
			if (dX > dY)
			{
				xcoord[0] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[0] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[1] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[2] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				height[0] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
				height[1] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[2] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
			}
			else
			{
				xcoord[0] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[0] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[1] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[2] = pDem->minLatitude + iY*pDem->gridWidth;
				height[0] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[1] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX];
				height[2] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
			}
		}
		else
		{
			if ((dX + dY) < pDem->gridWidth)
			{
				xcoord[0] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[0] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[1] = pDem->minLatitude + iY*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[2] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				height[0] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[1] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX];
				height[2] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX];
			}
			else
			{
				xcoord[0] = pDem->minLongitude + iX*pDem->gridWidth;      ycoord[0] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[1] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[1] = pDem->minLatitude + (iY+1)*pDem->gridWidth;
				xcoord[2] = pDem->minLongitude + (iX+1)*pDem->gridWidth;  ycoord[2] = pDem->minLatitude + iY*pDem->gridWidth;
				height[0] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX];
				height[1] = pDem->m_pData[(iY+1)*GDEM_MAX_DEM_PT_COUNT + iX + 1];
				height[2] = pDem->m_pData[iY*GDEM_MAX_DEM_PT_COUNT + iX + 1];
			}
		}
		d = line->m_cEnd.m_tX - xcoord[0]; d = abs(d);
		crossingPt.pt.m_tZ = height[0] + ( d*(height[1] - height[0]) + abs(line->m_cEnd.m_tY - ycoord[0])*(height[2] - height[1]) )/pDem->gridWidth;
		crossingPt.pt.m_tZ *= rate;
		crossingPt.pt.m_tZ += OFFSET_POLYGON;

		inPtArr.push_back(crossingPt);
	}
	
	qSort(inPtArr.begin(), inPtArr.end(), hasBiggerZValue);
	
	for (int i = 0; i<inPtArr.size(); i++)
	{
		gdm_get3DBodyPoint(inPtArr.at(i).pt.m_tX, inPtArr.at(i).pt.m_tY, pos, inPtArr.at(i).pt.m_tZ);
		ptArr->push_back(geo::core::vector3df(pos.m_X, -pos.m_Y, pos.m_Z));
	}
}
//*/
//end addition by RSH
void	convert3d(const GDM_LOGIC_TILE *ptile, LineSegment* linesegment)
{   	
	if (linesegment->pts.size() < 2) return;

	GDM_POINT3D pt;
	double offsetheight = gdm_GetDataMgr()->m_pCamera->m_location.m_dDist / 100.0;
	
	double height;
	double firstnonheight=0.0;   //°¡»þ ½åËÁËæ 0ËË Ê­³¥ ²ÚËË°ª
	double latestnonheight=0.0;      //°¡»þ ¾×°ýËæ ±½»¦´ô 0ËË Ê­³¥ ²ÚËË°ª
	
	//added by RSH 2013.317
	///*
	geo::core::array<geo::core::vector3df> ptArr;
	ptArr.set_used(0);

	if (ptile->nLevel > SHAPE_EXACT_RENDER_LEVEL)
	{
		CGDM_Vector2DF prevPt, curPt;
		CGDM_Line2DF line;
		//geo::core::vector2df prevPt,curPt;
		prevPt.m_tX = linesegment->pts[0].X;
		prevPt.m_tY = linesegment->pts[0].Y;
		
		for (unsigned int i = 1; i<linesegment->pts.size(); i++)
		{
			curPt.m_tX = linesegment->pts[i].X; curPt.m_tY = linesegment->pts[i].Y;
			line.setLine(prevPt, curPt);
			GetExactPtsInLogicTile(ptile, &line, &ptArr);
			prevPt.m_tX = curPt.m_tX; prevPt.m_tY = curPt.m_tY;
		}
		linesegment->pts.set_used(0);
		for (unsigned int k = 0; k<ptArr.size(); k++)
		{
			linesegment->pts.push_back(ptArr[k]);
		}
		ptArr.set_used(0);
		return;
	}
	//*/
	//end addition by RSH



	if(ptile->nLevel<14)
	{		
		for(unsigned int i=0;i<linesegment->pts.size();i++)
		{
		   height=gdm_getHeight(linesegment->pts[i].X, linesegment->pts[i].Y);
		   
		   if(firstnonheight==0.0 && height>0.0)
				firstnonheight=height;

		   if(height>0.0)	   latestnonheight=height;

		   if(height==0.0)		height=latestnonheight;

		    linesegment->pts[i].Z=height;
		}

		if(linesegment->pts[0].Z==0.0) 	linesegment->pts[0].Z=firstnonheight;

		for(unsigned int i=0;i<linesegment->pts.size();i++)
		{
			 gdm_get3DBodyPoint(linesegment->pts[i].X, linesegment->pts[i].Y, pt, linesegment->pts[i].Z+offsetheight);
			 
			linesegment->pts[i].X=pt.m_X;
			linesegment->pts[i].Y=-pt.m_Y;
			linesegment->pts[i].Z=pt.m_Z;
		}
		return;
	 }

	array<vector3df> arrdivpt;//»½·ëÂ×±Â ±ý²æ¼°

	double deltalong,deltalat;
	int level=ptile->pDemTile->m_sInfo.nLevel;
	if(level>13) level=13;

	double tilewidth=gdm_TwoPI/(1<<level);
	double divisionangle=tilewidth/128.0;	

	arrdivpt.push_back(linesegment->pts[0]);  //Ë§ºº ½ó±¨¼°Ë¾ ²Ä²÷³Þ.
	double dist;
	vector3df tmp;

	int ndiv;
	firstnonheight=0.0;
	latestnonheight=0.0;

	for(unsigned int i=1;i<linesegment->pts.size();i++)
	{
		dist=getDistance(linesegment->pts[i-1].X,linesegment->pts[i-1].Y,
			             linesegment->pts[i].X,linesegment->pts[i].Y);//³ÞËÁ¼°Ä´¼èÌ© °º¶®¶¦ ±Ëº¤ÂÙ³Þ.
		dist = abs(dist);

		if(dist<=divisionangle)//»½·ëÂ×±Â ±ý²è Â­Êý°¡ ÊÖ²÷ °ÒË§
		{
			double x=linesegment->pts[i].X;
			double y=linesegment->pts[i].Y;
			height=gdm_getExactHeight(x,y);
			linesegment->pts[i].Z=height;
			arrdivpt.push_back(linesegment->pts[i]);
			continue;
		}

		ndiv=(int)(dist / divisionangle + 1);
		deltalong=linesegment->pts[i].X-linesegment->pts[i-1].X;
		deltalong=deltalong/ndiv;
		deltalat=linesegment->pts[i].Y-linesegment->pts[i-1].Y;
		deltalat=deltalat/ndiv;

		for(int j=1;j<=ndiv;j++)  //³Þ»¤ ¼¿·ëÂ×±Â ±ý²ä±Ê³Þ.
		{
			tmp.X=linesegment->pts[i-1].X+j*deltalong;
			tmp.Y=linesegment->pts[i-1].Y+j*deltalat;
			height=gdm_getExactHeight(tmp.X,tmp.Y);
			
			if(firstnonheight==0.0 && height>0.0)
				firstnonheight=height;

			if(height>0.0)	   latestnonheight=height;

			if(height<=0.0)	
				height=latestnonheight;
			tmp.Z=height;
			
			//qDebug("%f", height);

			arrdivpt.push_back(tmp);
		}
		
		for(unsigned int k=arrdivpt.size()-ndiv-1;k<arrdivpt.size();k++)
		{
			if(arrdivpt[k].Z<=0)
				arrdivpt[k].Z=firstnonheight;
		}

	}

	height=gdm_getExactHeight(arrdivpt[0].X,arrdivpt[0].Y);


	if(height!=0.0)
		arrdivpt[0].Z=height;
	else
		arrdivpt[0].Z=firstnonheight;

	linesegment->pts.set_used(0);
	
	for(unsigned int i=0;i<arrdivpt.size();i++)
	{
		gdm_get3DBodyPoint(arrdivpt[i].X, arrdivpt[i].Y, pt, arrdivpt[i].Z+offsetheight);
		
		tmp.X=pt.m_X;
		tmp.Y=-pt.m_Y;
		tmp.Z=pt.m_Z;
		linesegment->pts.push_back(tmp);
	}
}

void get2dVertexLinesegmentArray(const GDM_LOGIC_TILE *ptile, PolyLinesegment* linesegment_arr)
{
	GDM_POINT2D *vertexbuf;
	const GDM_SHAPE_TILE* pShapeTile;
	LineSegment* be_writing_linesegment; 
	GDM_POLYGON *current_source_linesegment;

	double longitude, latitude;//,preLongitude,preLatitude;

	int linesegment_index = 0;

	for(int i = 0; i < ptile->nShapeTileCnt; i++)
	{
		if( ptile->sArrShapeTile[i]==NULL) continue;

		pShapeTile = ptile->sArrShapeTile[i];

		const char *current_shapetile_pos = (char*)pShapeTile->m_pData;

		for(int j = 0; j < pShapeTile->m_nPolygonCount; j++)
		{
			if(linesegment_index >= MAX_POLYGON_COUNT) 	break;

			be_writing_linesegment= &linesegment_arr->polyList[linesegment_index];
			current_source_linesegment = (GDM_POLYGON*)current_shapetile_pos;

			vertexbuf = (GDM_POINT2D*)((int*)current_source_linesegment + 1); // count;

			be_writing_linesegment->type = pShapeTile->m_sInfo.eSubType;

			for(unsigned int k = 0; k < current_source_linesegment->m_nCount; k++)
			{
				if(linesegment_index >= MAX_POLYGON_COUNT)		  break;

				longitude = vertexbuf[k].m_X * PAI;
				latitude = vertexbuf[k].m_Y * PAI;

				//if(longitude < ptile->minLongitude || longitude > ptile->maxLongitude
				//	|| latitude < ptile->minLatitude || latitude > ptile->maxLatitude)
				//{						
				//	if(be_writing_linesegment->pts.size() > 0)
				//	{
				//		be_writing_linesegment->pts.push_back(vector3df(longitude+gdm_PI,gdm_PI-latitude,0))	;

				//		linesegment_index++;
				//		if(linesegment_index >= MAX_POLYGON_COUNT)	 	break;

				//		be_writing_linesegment = &linesegment_arr->polyList[linesegment_index];

				//		be_writing_linesegment->type = pShapeTile->m_sInfo.eSubType;
				//	}		

				//	preLongitude = longitude;
				//	preLatitude = latitude;
				//	continue;
				//}

				//if (k > 0 && be_writing_linesegment->pts.size() == 0)
				//	be_writing_linesegment->pts.push_back(vector3df(preLongitude+gdm_PI, gdm_PI-preLatitude,0));

				be_writing_linesegment->pts.push_back(vector3df(longitude+gdm_PI, gdm_PI-latitude,0));

				/*preLongitude = longitude;
				preLatitude = latitude;*/
			}

			if(be_writing_linesegment->pts.size() > 1)
				linesegment_index++;
			else if (be_writing_linesegment->pts.size() > 0)
				be_writing_linesegment->pts.set_used(0);

			current_shapetile_pos += 4/*count*/ + sizeof(GDM_POINT2D) * current_source_linesegment->m_nCount;
		}
	}
	linesegment_arr->nCount = linesegment_index;
}

///////****
//addition by RSH 2013.3.2
//this is copy from terrain engine
inline bool intersects(double lptx1, double lpty1,double lptx2,double lpty2, double rptx1, double rpty1, double rptx2, double rpty2)
{
	double commonDenominator = (lpty2 - lpty1)*(rptx2 - rptx1) - (lptx2 - lptx1)*(rpty2 - rpty1);
	double numeratorA =        (lptx2 - lptx1)*(rpty1 - lpty1) - (lpty2 - lpty1)*(rptx1 - lptx1);
	double numeratorB =        (rptx2 - rptx1)*(rpty1 - lpty1) - (rpty2 - rpty1)*(rptx1 - lptx1); 

	if(abs(commonDenominator) < INTERSECTING_TOLERANCE)
		return false; // parallel

	double uA = numeratorA / commonDenominator;
	if(uA < 0.f || uA > 1.f) return false; 

	double uB = numeratorB / commonDenominator;
	if(uB < 0.f || uB > 1.f) return false; 

	return true; 
}
//end addition by RSH
//////*/////

void	get3dVertexLinesegmentArray(const GDM_LOGIC_TILE *ptile, PolyLinesegment* linesegment_arr)
{
	GDM_POINT2D *vertexbuf;
	const GDM_SHAPE_TILE* pShapeTile;
	LineSegment* be_writing_linesegment; 
	GDM_POLYGON *current_source_linesegment;

	double longitude, latitude,preLongitude,preLatitude;
	int linesegment_index = 0;
	

	CGDM_Vector2DF crossPt;
	bool isOutPt = false;
	CGDM_Line2DF bndLines[4], line;
	bndLines[0].setLine(CGDM_Vector2DF(ptile->minLongitude, ptile->minLatitude), CGDM_Vector2DF(ptile->minLongitude, ptile->maxLatitude));
	bndLines[1].setLine(CGDM_Vector2DF(ptile->minLongitude, ptile->minLatitude), CGDM_Vector2DF(ptile->maxLongitude, ptile->minLatitude));
	bndLines[2].setLine(CGDM_Vector2DF(ptile->maxLongitude, ptile->minLatitude), CGDM_Vector2DF(ptile->maxLongitude, ptile->maxLatitude));
	bndLines[3].setLine(CGDM_Vector2DF(ptile->minLongitude, ptile->maxLatitude), CGDM_Vector2DF(ptile->maxLongitude, ptile->maxLatitude));

	//ÂÙ±¶Ì© µã¶®À°ËÎËæ ÊÞµÍ±¶Ì© shapeÀ°ËÎ´ÉËË ¼Á¼õÂÙ³Þ.
   	for(int i = 0; i < ptile->nShapeTileCnt; i++)
	{
		if( ptile->sArrShapeTile[i]==NULL) continue;

		pShapeTile = ptile->sArrShapeTile[i];

		//À°ËÎËæº· »¨¼è Áç¶®°Ø»ôµê°¡ ´ÉÊÌËØ²÷ ±¨ÊÍ±¨Ì© ¼è¼¬»ô¶¦ °¡¶®¿Ö³Þ.
		const char *current_shapetile_pos = (char*)pShapeTile->m_pData;

		//ÂÙ±¶Ì© shapeÀ°ËÎËæº· Áç¶®°Ø±¶ºã¶í¿Ñ ºåÃûË¾ ¼êÃÔÂÙ³Þ. 
		for(int j = 0; j < pShapeTile->m_nPolygonCount; j++)
		{
			if(linesegment_index >= MAX_POLYGON_COUNT) 	break;

			be_writing_linesegment= &linesegment_arr->polyList[linesegment_index];
			current_source_linesegment = (GDM_POLYGON*)current_shapetile_pos;
			be_writing_linesegment->ID=	current_source_linesegment->ID;
			
			//added by RSH 2013.3.3 for bounding box
			double polyMinX, polyMinY, polyMaxX, polyMaxY;
			polyMinX = current_source_linesegment->m_bMinX;
			polyMinY = current_source_linesegment->m_bMinY;
			polyMaxX = current_source_linesegment->m_bMaxX;
			polyMaxY = current_source_linesegment->m_bMaxY;
			
			vertexbuf = (GDM_POINT2D*)((int*)current_source_linesegment + 2 + 8/*added by RSH 2013.3.3 for bound. */); // count,id, bound;
			be_writing_linesegment->type = pShapeTile->m_sInfo.eSubType;

			if(polyMaxX < ptile->minLongitude || polyMinX > ptile->maxLongitude
							|| polyMaxY < ptile->minLatitude || polyMinY > ptile->maxLatitude)
			{
				current_shapetile_pos += 4/*count*/+4/*id*/ + 32/*bounding box: 4 * double*/ + sizeof(GDM_POINT2D) * current_source_linesegment->m_nCount;
				continue;
			}	
			//end addition by RSH
			isOutPt = false;
			for(unsigned int k = 0; k < current_source_linesegment->m_nCount; k++)
			{
				if(linesegment_index >= MAX_POLYGON_COUNT)		  break;

				longitude = vertexbuf[k].m_X;
				latitude = vertexbuf[k].m_Y;

				if(longitude < ptile->minLongitude || longitude > ptile->maxLongitude
					|| latitude < ptile->minLatitude || latitude > ptile->maxLatitude)
				{	
					isOutPt = true;
					///*
					if (k == 0) 
					{
						preLongitude = longitude;
						preLatitude = latitude;
						continue;
					}
					if ((preLongitude < ptile->minLongitude && longitude < ptile->minLongitude)	||
						(preLongitude > ptile->maxLongitude && longitude > ptile->maxLongitude) ||
						(preLatitude < ptile->minLatitude   && latitude  < ptile->minLatitude ) ||
						(preLatitude > ptile->maxLatitude   && latitude  > ptile->maxLatitude))
					{
						preLongitude = longitude;
						preLatitude = latitude;
						continue;
					}
					
					int inx[4];
					geo::core::array<geo::core::vector3df> ptArr;

					line.setLine(CGDM_Vector2DF(vertexbuf[k-1].m_X, vertexbuf[k-1].m_Y), CGDM_Vector2DF(vertexbuf[k].m_X, vertexbuf[k].m_Y));
					if (preLongitude < longitude)
					{
						if (preLatitude < latitude)
						{
							inx[0] = 0; inx[1] = 1; inx[2] = 2; inx[3] = 3;
						}
						else
						{
							inx[0] = 3; inx[1] = 0; inx[2] = 1; inx[3] = 2;
						}
					}
					else
					{
						if (preLatitude < latitude)
						{
							inx[0] = 1; inx[1] = 2; inx[2] = 3; inx[3] = 0;
						}
						else
						{
							inx[0] = 2; inx[1] = 3; inx[2] = 0; inx[3] = 1;
						}
					}
					for (int lineInx = 0; lineInx<4; lineInx++)
					{
						int index = inx[lineInx];
						if (line.intersectWith(bndLines[index], crossPt, INTERSECTING_TOLERANCE))
							ptArr.push_back(vector3df(crossPt.m_tX, crossPt.m_tY, 0));
					}
					if (ptArr.size() > 1)
					{
						be_writing_linesegment->type = pShapeTile->m_sInfo.eSubType;
						be_writing_linesegment->ID = current_source_linesegment->ID;
						for (unsigned int nInx = 0; nInx<ptArr.size(); nInx++)
							be_writing_linesegment->pts.push_back(ptArr[nInx]);
						
						linesegment_index ++;
						if (linesegment_index >= MAX_POLYGON_COUNT) break;
						be_writing_linesegment = &linesegment_arr->polyList[linesegment_index];
						be_writing_linesegment->pts.set_used(0);
					}
					else if (ptArr.size() > 0)
					{
						be_writing_linesegment->pts.push_back(ptArr[0]);
						linesegment_index++;
						if (linesegment_index >= MAX_POLYGON_COUNT) break;
						be_writing_linesegment = &linesegment_arr->polyList[linesegment_index];
						be_writing_linesegment->pts.set_used(0);
						be_writing_linesegment->type = pShapeTile->m_sInfo.eSubType;
						be_writing_linesegment->ID = current_source_linesegment->ID;
					}
					//*/
					/*
					
					if(be_writing_linesegment->pts.size() > 0)
					{
						CGDM_Line2DF line;
						line.setLine(CGDM_Vector2DF(preLongitude, preLatitude), CGDM_Vector2DF(longitude, latitude));

						be_writing_linesegment->pts.push_back(vector3df(longitude,latitude,0))	;

						linesegment_index++;
						if(linesegment_index >= MAX_POLYGON_COUNT)	 	break;

						be_writing_linesegment = &linesegment_arr->polyList[linesegment_index];

						be_writing_linesegment->type = pShapeTile->m_sInfo.eSubType;
						be_writing_linesegment->ID=	current_source_linesegment->ID;
					}		
					
					///*********
					//by RSH 2013.3.2 to append 4 crossing points with boundary
					else if (k != current_source_linesegment->m_nCount-1)	//¶ë¼è¶ì¼°ËË Ê­³£µ¹·²
					{
						// ³ÞËÁ¼°Ë¾ °ÖµØÂÙ³Þ.
						double nlongi = vertexbuf[k + 1].m_X, nlati = vertexbuf[k + 1].m_Y ;

						double r1x, r1y, r2x, r2y;
						//bottomline

						r1x = ptile->maxLongitude; r1y = ptile->minLatitude; r2x = ptile->maxLongitude; r2y = ptile->maxLatitude;
						if (intersects(longitude, latitude, nlongi, nlati, r1x, r1y, r2x, r2y))
						{
							be_writing_linesegment->pts.push_back(vector3df(longitude, latitude, 0));
						}	
						else
						{
							r1x = ptile->minLongitude; r1y = ptile->minLatitude; r2x = ptile->minLongitude; r2y = ptile->maxLatitude;
							if (intersects(longitude, latitude, nlongi, nlati, r1x, r1y, r2x, r2y))
							{
								be_writing_linesegment->pts.push_back(vector3df(longitude, latitude, 0));
							}
							else
							{	
								r1x = ptile->minLongitude; r1y = ptile->maxLatitude; r2x = ptile->maxLongitude; r2y = ptile->maxLatitude;
								if (intersects(longitude, latitude, nlongi, nlati, r1x, r1y, r2x, r2y))
								{
									be_writing_linesegment->pts.push_back(vector3df(longitude, latitude, 0));
								}
								else
								{
									
									r1x = ptile->minLongitude; r1y = ptile->minLatitude; r2x = ptile->maxLongitude; r2y = ptile->minLatitude;
									if (intersects(longitude, latitude, nlongi, nlati, r1x, r1y, r2x, r2y))
									{
										be_writing_linesegment->pts.push_back(vector3df(longitude, latitude, 0));
									}
								}
							}
						}
					}
					//end addition by RSH
					/////******/////

					preLongitude = longitude;
					preLatitude = latitude;
					continue;
				}

				// commented out by RSH 2013.3.2: because  pre-entering point was added to be_writing_linesegment
				///******************
				//if (k > 0 && be_writing_linesegment->pts.size() ==0)
				//	be_writing_linesegment->pts.push_back(vector3df(preLongitude, preLatitude,0));
				//end comment by RSH
				////////////////*/
				if (isOutPt)
				{
					line.setLine(CGDM_Vector2DF(preLongitude, preLatitude), CGDM_Vector2DF(longitude, latitude));
					for (int nInx = 0; nInx < 4; nInx++)
					{
						if (line.intersectWith(bndLines[nInx], crossPt, INTERSECTING_TOLERANCE))
							be_writing_linesegment->pts.push_back(vector3df(crossPt.m_tX, crossPt.m_tY, 0));
					}
				}
				be_writing_linesegment->pts.push_back(vector3df(longitude, latitude,0));
				isOutPt = false;

				preLongitude = longitude;
				preLatitude = latitude;
			}

			if(be_writing_linesegment->pts.size() > 1)
				linesegment_index++;
			else if (be_writing_linesegment->pts.size() > 0)
				be_writing_linesegment->pts.set_used(0);

			//revised by RSH 2013.3.3 for bounding box
			current_shapetile_pos += 4/*count*/+4/*id*/ + 32/*bounding box: 4 * double*/ + sizeof(GDM_POINT2D) * current_source_linesegment->m_nCount;
		}
	}
	linesegment_arr->nCount = linesegment_index;
	
	for(int i=0;i<linesegment_arr->nCount;i++)
		convert3d(ptile,&linesegment_arr->polyList[i]);
}

//Ìº¶·Ì© ¿¸´Å
int	gdm_GetGeoShapeInfo(int* pIndex, PolyLinesegment* pGeoShapeArray)
{
	int nTileIndex;	
	const GDM_LOGIC_TILE *pTile;	
	const GDM_LOGIC_TILE_ARR *p_shape_tile_list;
	double baseOffset;
	GDM_POINT2D *pVertex;

	p_shape_tile_list = gdm_GetRenderTiles();

	nTileIndex = *pIndex;

	if(nTileIndex >= p_shape_tile_list->nCount)
	{
		*pIndex = 0;
		return 0;
	}

	GDM_LOCATION location;
	gdm_CameraGetLocation(gdm_GetDataMgr(), &location);

	pTile		= p_shape_tile_list->pData + nTileIndex;
	pGeoShapeArray->nCount = 0;	

	int polyIndex = 0;

	// get offset
	baseOffset = gdm_GetDataMgr()->m_pCamera->m_location.m_dDist / 50.0;

	if(baseOffset > SHAPE_MAX_OFFSET_RADIUS)
		baseOffset = SHAPE_MAX_OFFSET_RADIUS;

	for(int i = 0; i < pTile->nShapeTileCnt; i++)
	{
		LineSegment* polygonData = NULL;
		const GDM_SHAPE_TILE* pShapeTile = pTile->sArrShapeTile[i];
		if(pShapeTile == NULL)
			continue;

		// for debug add tile line
		if(0)
		{
			polygonData = &pGeoShapeArray->polyList[polyIndex];
			polyIndex++;
			double tileWidth = gdm_TwoPI / (1 << pShapeTile->m_sInfo.nLevel);

			double minLongitude = tileWidth * pShapeTile->m_sInfo.nX - gdm_PI;
			double minLatitude	= tileWidth * pShapeTile->m_sInfo.nY - gdm_PI;
			gdm_AddVertexToPolygon(minLongitude, minLatitude, baseOffset, polygonData, NULL);
			gdm_AddVertexToPolygon(minLongitude + tileWidth, minLatitude, baseOffset, polygonData, NULL);
			gdm_AddVertexToPolygon(minLongitude + tileWidth, minLatitude + tileWidth, baseOffset, polygonData, NULL);
			gdm_AddVertexToPolygon(minLongitude, minLatitude + tileWidth, baseOffset + 100, polygonData, NULL);
			polygonData->type = pShapeTile->m_sInfo.eSubType;
		}

		if(pShapeTile != NULL)
		{		
			const char *pData = (char*)pShapeTile->m_pData;
			
			for(int j = 0; j < pShapeTile->m_nPolygonCount; j++)
			{
				if(polyIndex >= MAX_POLYGON_COUNT)
					break;

				polygonData = &pGeoShapeArray->polyList[polyIndex];
				GDM_POLYGON *pPolygon = (GDM_POLYGON*)pData;
				
				pVertex = (GDM_POINT2D*)((int*)pPolygon + 1); // count;

				polygonData->type = pShapeTile->m_sInfo.eSubType;

				double preLongitude = 0;
				double preLatitude = 0;

				for(unsigned int k = 0; k < pPolygon->m_nCount; k++)
				{
					if(polyIndex >= MAX_POLYGON_COUNT)
						break;

					double longitude = pVertex[k].m_X * PAI;
					double latitude = pVertex[k].m_Y * PAI;
					if (0)
					{
						double minLon, maxLon, minLat, maxLat;
						minLon = 76.122619 * gdm_DEGTORAD;
						maxLon = 77.736738 * gdm_DEGTORAD;
						minLat = 31.598381 * gdm_DEGTORAD;
						maxLat = 33.118941 * gdm_DEGTORAD;
						if ( minLon > pTile->minLongitude && maxLon < pTile->maxLongitude &&
							 minLat > pTile->minLatitude  && maxLat < pTile->maxLatitude )
						if ( longitude > minLon && longitude < maxLon &&
							 latitude  > minLat && latitude  < maxLat )
							 qDebug( "---->Shape Error : Yes : Level[%d] X[%d] Y[%d]", pTile->nLevel, pTile->nX, pTile->nY);

					}

					
					if(longitude < pTile->minLongitude || longitude > pTile->maxLongitude
						|| latitude < pTile->minLatitude || latitude > pTile->maxLatitude)
					{						
						if(polygonData->pts.size() > 0)
						{
							gdm_AddVertexToPolygon(longitude, latitude, baseOffset, polygonData, NULL);

							polyIndex++;
							if(polyIndex >= MAX_POLYGON_COUNT)
								break;
							polygonData = &pGeoShapeArray->polyList[polyIndex];
							//polygonData->close = false;
							
							polygonData->type = pShapeTile->m_sInfo.eSubType;
						}		
						
						preLongitude = longitude;
						preLatitude = latitude;
						continue;
					}

					if (k > 0 && polygonData->pts.size() == 0)
					{
						gdm_AddVertexToPolygon(preLongitude, preLatitude, baseOffset, polygonData, NULL);
					}

					gdm_AddVertexToPolygon(longitude, latitude, baseOffset, polygonData, pTile->pDemTile);
					preLongitude = longitude;
					preLatitude = latitude;
				}

				if(polygonData == NULL)
					continue;

				if(polygonData->pts.size() > 1)
					polyIndex++;
				else if (polygonData->pts.size() > 0)
					polygonData->pts.clear();
				pData += 4/*count*/ + sizeof(GDM_POINT2D) * pPolygon->m_nCount;
			}
			pGeoShapeArray->nCount = polyIndex;
		}
	}

	return 1;
}

#include "GIS_funcs.h"
#include "GDM_Triangulate.h"

#define  GDEM_MAX_TILE_COUNTS  5000
#define	 GDEM_MAX_VERTEX_COUNTS 4
#define  SMALL_NUM  0.0000000000000001 // anything that avoids division overflow

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).m_tX * (v).m_tX + (u).m_tY * (v).m_tY + (u).m_tZ * (v).m_tZ)
#define cross(u,v,n)  n.m_tX = (u).m_tY * (v).m_tZ - (u).m_tZ * (v).m_tY;\
	n.m_tY = (u).m_tZ * (v).m_tX - (u).m_tX * (v).m_tZ;\
	n.m_tZ = (u).m_tX * (v).m_tY - (u).m_tY * (v).m_tX;
#define  gdm_swap(u, v) {double t = v; v = u; u = t;}

int sign(float value) {
	if (value > 0)
		return 1;
	else if (value == 0) 
		return 0;
	else
		return -1;
}

//위도, 경도자리표로부터 지구중심자리표(x, y, z) 얻는 함수.
void gdem_Get3DPointFromLocation(CGDM_Location3D &i_pt, CGDM_Vector3DF &o_pt, double &height)
{
	double a= 6378137.0;
	double b = 6356752.3142;
	double e = 0.08181919;
	double e2 = 0.006739496742337;
	double f = 0.0035281;
	double c = a / (sqrt((1 - e2 * pow(sin(i_pt.m_tY), 2))));
	o_pt.m_tX = (c + i_pt.m_tZ) * cos(i_pt.m_tY) * cos(i_pt.m_tX);
	o_pt.m_tY = (c + i_pt.m_tZ) * cos(i_pt.m_tY) * sin(i_pt.m_tX);
	o_pt.m_tZ = (c * pow(b, 2) / pow(a, 2) + i_pt.m_tZ) * sin(i_pt.m_tY);

}

void gdem_GetLocationFrom3DPoint(CGDM_Vector3DF &i_pt, CGDM_Location3D &o_pt)
{
}

//  Dem Tile내에서 점이 놓인 그리드의 4개정점의 높이를 얻는 함수 
double gdem_GetHeightFromDemTile(CGDM_Location3D &i_pt, GDM_DEM_TILE* pTile)
{
	short	h[4];		// height
	double	high = 0;
	double	d[4];		// distance from loc

	if(pTile == NULL)
		return 0;

	memset(h, 0, sizeof(h));
	if(i_pt.m_tX < pTile->minLongitude || i_pt.m_tX > pTile->maxLongitude || 
		i_pt.m_tY < pTile->minLatitude || i_pt.m_tY > pTile->maxLatitude)
		return 0;

	i_pt.m_tZ = 0;

	double dBaseLongitude = pTile->tileWidth * pTile->m_sInfo.nX - gdm_PI;
	double dBaseLatitude  = pTile->tileWidth * pTile->m_sInfo.nY - gdm_PI;

	int iX = ((i_pt.m_tX - dBaseLongitude) / pTile->gridWidth);
	int iY = ((i_pt.m_tY - dBaseLatitude ) / pTile->gridWidth);

	if(iX >= GDEM_MAX_DEM_PT_COUNT || iY >= GDEM_MAX_DEM_PT_COUNT)
		return 0;

	h[0] = pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX];
	h[1] = pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX + 1];
	h[2] = pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX + 1];
	h[3] = pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX];

	d[0] = i_pt.getDistance(dBaseLongitude + iX * pTile->gridWidth, dBaseLatitude + iY * pTile->gridWidth, 0);
	d[1] = i_pt.getDistance(dBaseLongitude + (iX + 1) * pTile->gridWidth, dBaseLatitude + iY * pTile->gridWidth, 0);
	d[2] = i_pt.getDistance(dBaseLongitude + (iX + 1) * pTile->gridWidth, dBaseLatitude + (iY + 1) * pTile->gridWidth, 0);
	d[3] = i_pt.getDistance(dBaseLongitude + iX * pTile->gridWidth, dBaseLatitude + (iY + 1) * pTile->gridWidth, 0);

	high = (h[0] * d[1] * d[2] * d[3] + d[0] * h[1] * d[2] * d[3] + d[0] * d[1] * h[2] * d[3] + d[0] * d[1] * d[2] * h[3]) /
		(d[1] * d[2] * d[3] + d[0] * d[2] * d[3] + d[0] * d[1] * d[3] + d[0] * d[1] * d[2]);

	i_pt.m_tZ = high;

	return high;
}

//Polygon내부의 수평면적계산.
double	gdem_Get2DAreaWithPolygon(CGDM_VectorArr &i_arrPt)
{	
	double dArea = 0;
	int nPtCounts = i_arrPt.size();

	for (int i = 0; i< nPtCounts; i++)
	{
		dArea += (i_arrPt[i].m_tX * i_arrPt[i + 1].m_tY - i_arrPt[i + 1].m_tX * i_arrPt[i].m_tY) / 2;
	}
	return dArea;
}

int	gdem_Sign2DAreaWithPolygon(CGDM_VectorArr &i_arrPt, int &a_nCounts)
{	
	double dArea1 = 0;
	double dArea2 = 0;

	if (a_nCounts <= 3) return 1;

	a_nCounts -= 2;

	for (int i = 0; i< a_nCounts; i++)
	{
		dArea1 += (i_arrPt[i].m_tX * i_arrPt[i + 1].m_tY - i_arrPt[i + 1].m_tX * i_arrPt[i].m_tY) / 2;
		if (i == (a_nCounts - 2) ) dArea2 = dArea1;
	}
	if (dArea1 >= dArea2)
		return 1;
	else 
		return -1;
	
}

//지형고려안한 Polygon의 면적계산.
double		gdem_GetAreaWithoutTerrain(CGDM_LocationArr &i_arrPt, int i_Level)
{
	float area = 0;
	int sig = 1;
	int signal = 1;
	unsigned int i, nCounts = i_arrPt.size();
	if (nCounts <= 2)
	{
		return 0;
	}

	nCounts = nCounts - 2;

	for (i = 0; i < nCounts; i++)
	{		
		float angle = 0;
		unsigned int j = i + 2;
		double x1 = i_arrPt[i +1].m_tX - i_arrPt[0].m_tX;
		double y1 = i_arrPt[i + 1].m_tY - i_arrPt[0 ].m_tY;
		double x2 = i_arrPt[i + 2].m_tX - i_arrPt[0].m_tX;
		double y2 = i_arrPt[i + 2].m_tY - i_arrPt[0].m_tY;

		double meanLatitude = (i_arrPt[0].m_tY + i_arrPt[i + 1].m_tY) / 2;		
		double diffLongitude = sqrt((i_arrPt[0].m_tX - i_arrPt[i + 1].m_tX) * (i_arrPt[0].m_tX - i_arrPt[i + 1].m_tX) + (i_arrPt[0].m_tY - i_arrPt[i + 1].m_tY) * (i_arrPt[0].m_tY - i_arrPt[i + 1].m_tY));

		if (diffLongitude == 0) 
			continue;

                double diffLatitude = ((x1 * y2) - (x2 * y1)) / diffLongitude;
                diffLatitude = fabs(diffLatitude);

		angle =  sign((x1 * y2) - (x2 * y1));

		area += sign(angle) * gdem_GetAreaWithoutTerrain(meanLatitude, diffLatitude, diffLongitude);
	}


	area = area / 2;
        return fabs(area) ;
	
}

//delta_latitude, delta_longitude에 따라 지형고려안한 Polygon의 면적계산.
double		gdem_GetAreaWithoutTerrain(double latitude, double delta_latitude, double delta_longitude)
{
	double A, B, C, D, E, F;
	double dArea;
	double a= 6378137.0;
	double b = 6356752.3142;
	double e = 0.08181919;
	double e2 = 0.006739496742337;
	double e4 = e2 * e2;
	double e6 = e2 * e4;
	double e8 = e6 * e2;
	double e10 = e8 * e2;
	double f = 0.0035281;


	A = 1.0 + e2 * 0.5 + 3.0 / 8 * e4 + 5.0 / 16 * e6 + 35.0 / 128 * e8 + 63.0 / 256 * e10;
	B = 1.0 / 6 * e2 + 3.0 / 16 * e4 + 3.0 / 16 * e6 + 35.0 / 192 * e8 + 45.0 / 256  * e10;
	C = 3.0 / 80 * e4 + 1.0 / 16 * e6 + 5.0 / 64 * e8 + 45.0 / 512 * e10;
	D = 1.0 / 112 * e6 + 5.0 / 256 * e8 + 15.0 / 512 * e10;
	E = 5.0 / 2304 * e8 + 3.0 / 512 * e10;
	F = 3.0 / 5632 * e10;

	dArea =  b * b * 2 * delta_longitude * (A * cos(latitude) * sin(delta_latitude / 2) - B * cos(3 * latitude) * sin(3 / 2 * delta_latitude) + C * cos(5 * latitude) * sin(5 / 2 * delta_latitude) - D * cos(7 * latitude) * sin(3.5 * delta_latitude) + E * cos(9 * latitude) * sin(4.5 * delta_latitude) - F * cos(11 * latitude) * sin(5.5 * delta_latitude));

    return fabs(dArea);
}


//지구겉면자리표계의 점이 놓이는 타일의 INFO 얻기.
GDM_TILE_INFO gdem_GetTileFromPt(CGDM_Location3D i_Pt, int i_Level)
{
	GDM_TILE_INFO pTileInfo;
	CGDM_Vector3DF o_Pt;
	double dHeight = 0.0;

	double tileWidth = gdm_TwoPI / (1 << i_Level);

	pTileInfo.nX = (int)((i_Pt.m_tX + gdm_PI) / tileWidth);
	pTileInfo.nY = (int)((i_Pt.m_tY + gdm_PI) / tileWidth);
	pTileInfo.nLevel = i_Level;
	gdem_Get3DPointFromLocation(i_Pt, o_Pt,dHeight);

	return pTileInfo;
}

//Polygon내부에 점이 놓이는가를 판정.
bool gdem_PtIsInPolygon(CGDM_Location3D &i_pt, CGDM_LocationArr &i_arrPt)
{
	unsigned int index, count, crossCount;
	CGDM_Location3D *pPt1, *pPt2;
	double crossX;
	double x = i_pt.m_tX;
	double y = i_pt.m_tY;

	count = i_arrPt.size();
	crossCount = 0;
	for(index = 0; index < count; index++)
	{
		pPt1 = &i_arrPt[index];
		if(index == i_arrPt.size() - 1)
			pPt2 = &i_arrPt[0];
		else
			pPt2 = &i_arrPt[index + 1];

		if((pPt1->m_tY > y && pPt2->m_tY > y) || (pPt1->m_tY < y && pPt2->m_tY < y))
			continue;
		if(pPt1->m_tX < x && pPt2->m_tX < x)
			continue;

		crossX = ((pPt1->m_tX - pPt2->m_tX) * y + (pPt1->m_tY * pPt2->m_tX - pPt1->m_tX * pPt2->m_tY)) / (pPt1->m_tY - pPt2->m_tY);
		if(crossX > x)
			crossCount++;
	}

	if(crossCount % 2 != 0)
		return true;
	return false;

}


// 선분과 삼각형이 사귀는가를 판정하는 모쥴
/************************************************************************
gdem_LineIsInIntersectWithTriangle(): intersect a ray with a 3D triangle
Input:  i_startPt, i_endPt, i_Pt1, i_Pt2, i_Pt3
Output: *destPt = intersection point (when it exists)
Return: -1 = triangle is degenerate (a segment or point)
0 = disjoint (no intersect)
1 = intersect in unique point I1
2 = are in the same plane
************************************************************************/
int gdem_LineIsInIntersectWithTriangle(CGDM_Location3D &i_startPt, CGDM_Location3D &i_endPt, CGDM_Vector3DF *i_Pt1 , CGDM_Vector3DF *i_Pt2 , CGDM_Vector3DF *i_Pt3, CGDM_Vector3DF &i_destPt)
{

	CGDM_Location3D    u, v, n;             // triangle vectors
	CGDM_Vector3DF    w0, w;          // ray vectors
	float     r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = (CGDM_Location3D)(i_Pt2 - i_Pt1);
	v = (CGDM_Location3D)(i_Pt3 - i_Pt1);	
	cross(u,v,n);
	if (n == CGDM_Vector3DF(0))            // triangle is degenerate
		return -1;                 // do not deal with this case

	a = -dot(n,(i_startPt - *i_Pt1));
	b = dot(n,i_endPt);
	if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
		if (a == 0)                // ray lies in triangle plane
			return 2;
		else 
			return 0;             // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                   // ray goes away from triangle
		return 0;                  // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	i_destPt = (CGDM_Vector3DF)(i_startPt + r * i_endPt);           // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = dot(u,u);
	uv = dot(u,v);
	vv = dot(v,v);
	w = i_destPt - *i_Pt1;
	wu = dot(w,u);
	wv = dot(w,v);
	D = 1/(uv * uv - uu * vv);

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) * D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) * D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                      // I is in T
}


//두 점을 맺은 직선이 사귀는 타일들의 정보 얻기.
GDM_TILE_INFO *	gdem_GetTilesFromLine(CGDM_Location3D startPos, CGDM_Location3D endPos , int i_Level, int *o_TileCount)
{
	int tileCounts = 0;
	GDM_TILE_INFO* pTileInfo = NULL;
	GDM_TILE_INFO tileInfo1 , tileInfo2;
	pTileInfo = (GDM_TILE_INFO *)gdmMemMalloc(GDEM_MAX_TILE_COUNTS * sizeof(GDM_TILE_INFO));
	tileInfo1 = gdem_GetTileFromPt(startPos, i_Level);
	tileInfo2 = gdem_GetTileFromPt(endPos, i_Level);

	int x = tileInfo1.nX;
	int y = tileInfo1.nY;
	int delta_x = abs(tileInfo2.nX - tileInfo1.nX);
	int delta_y = abs(tileInfo2.nY - tileInfo1.nY);
	int s1 = sign(tileInfo2.nX - tileInfo1.nX);
	int s2 = sign(tileInfo2.nY - tileInfo1.nY);
	// interchange delta_x and delta_y, depending on the slope
	//   of the line
	bool Interchange;
	if (delta_y > delta_x) {
		int Temp = delta_x;
		delta_x = delta_y;
		delta_y = Temp;
		Interchange = true;
	}
	// initialize the error term to compensate for a nonzero intercept
	int e = 2 * delta_y - delta_x;
	// begin the main loop
	for (int i = 0; i <= delta_x; i++) {
		tileCounts++;
		if (tileCounts >= GDEM_MAX_TILE_COUNTS)
		{
			break;
		}
		pTileInfo[tileCounts - 1].nX = x; 
		pTileInfo[tileCounts - 1].nY = y;
		while (e > 0) {
			if (Interchange) {
				x += s1;
			} else {
				y += s2;
			}
			e -= 2 * delta_x;
		}
		if (Interchange) {
			y += s2;
		} else {
			x += s1;
		}
		e += 2 * delta_y;
	}

	o_TileCount = &tileCounts;
	return pTileInfo;
}

//Polyline과 사귀는 Tile들의 정보 얻기.
GDM_TILE_INFO *	gdem_GetTilesFromPolyLine(CGDM_LocationArr &i_arrPt,  int i_Level, int &o_TileCount)
{
	int tileCounts = 0;
	GDM_TILE_INFO* pTileInfo = NULL;

	pTileInfo = (GDM_TILE_INFO *)gdmMemMalloc(GDEM_MAX_TILE_COUNTS * sizeof(GDM_TILE_INFO));
	for (int k = 0; k < (int)i_arrPt.size() - 1 ; k++)
	{
		GDM_TILE_INFO tileInfo1 , tileInfo2;
		tileInfo1 = gdem_GetTileFromPt(i_arrPt[k], i_Level);
		tileInfo2 = gdem_GetTileFromPt(i_arrPt[k + 1], i_Level);

		int x = tileInfo1.nX;
		int y = tileInfo1.nY;
		int delta_x = abs(tileInfo2.nX - tileInfo1.nX);
		int delta_y = abs(tileInfo2.nY - tileInfo1.nY);
		int s1 = sign(tileInfo2.nX - tileInfo1.nX);
		int s2 = sign(tileInfo2.nY - tileInfo1.nY);
		// interchange delta_x and delta_y, depending on the slope
		//   of the poly line
		bool Interchange;
		if (delta_y > delta_x) {
			int Temp = delta_x;
			delta_x = delta_y;
			delta_y = Temp;
			Interchange = true;
		}
		// initialize the error term to compensate for a nonzero intercept
		int e = 2 * delta_y - delta_x;

		if ( delta_x == 0 && delta_y == 0 && tileCounts == 0 ) 
		{
			tileCounts++;
			pTileInfo[tileCounts - 1].nX = x; 
			pTileInfo[tileCounts - 1].nY = y;

		}

		// begin the main loop
		for (int i = 0; i < delta_x; i++) {
			tileCounts++;

			if (GDEM_MAX_TILE_COUNTS <= tileCounts)
				break;

			pTileInfo[tileCounts - 1].nX = x; 
			pTileInfo[tileCounts - 1].nY = y;
			while (e > 0) {
				if (Interchange) {
					x += s1;
				} else {
					y += s2;
				}
				e -= 2 * delta_x;
			}
			if (Interchange) {
				y += s2;
			} else {
				x += s1;
			}
			e += 2 * delta_y;
		}
	}

	o_TileCount = tileCounts;
	return pTileInfo;
}

//Polygon과 사귀는 타일들의 정보얻기.
GDM_TILE_INFO *	gdem_GetCrossTilesFromPolygon(CGDM_LocationArr &i_arrPt,  int i_Level, int &o_TileCount)
{
	int tileCounts = 0;
	GDM_TILE_INFO* pTileInfo = NULL;

	pTileInfo = (GDM_TILE_INFO *)gdmMemMalloc(GDEM_MAX_TILE_COUNTS * sizeof(GDM_TILE_INFO));
	for (int k = 0; k < (int)i_arrPt.size() ; k++)
	{
		GDM_TILE_INFO tileInfo1 , tileInfo2;
		tileInfo1 = gdem_GetTileFromPt(i_arrPt[k], i_Level);
		tileInfo2 = gdem_GetTileFromPt(i_arrPt[k + 1], i_Level);

		int x = tileInfo1.nX;
		int y = tileInfo1.nY;
		int delta_x = abs(tileInfo2.nX - tileInfo1.nX);
		int delta_y = abs(tileInfo2.nY - tileInfo1.nY);
		int s1 = sign(tileInfo2.nX - tileInfo1.nX);
		int s2 = sign(tileInfo2.nY - tileInfo1.nY);
		// interchange delta_x and delta_y, depending on the slope
		//   of the polygon
		bool Interchange;
		if (delta_y > delta_x) {
			int Temp = delta_x;
			delta_x = delta_y;
			delta_y = Temp;
			Interchange = true;
		}
		// initialize the error term to compensate for a nonzero intercept
		int e = 2 * delta_y - delta_x;
		// begin the main loop
		for (int i = 0; i < delta_x; i++) {
			tileCounts++;

			if (GDEM_MAX_TILE_COUNTS <= tileCounts)
				break;

			pTileInfo[tileCounts - 1].nX = x; 
			pTileInfo[tileCounts - 1].nY = y;
			while (e > 0) {
				if (Interchange) {
					x += s1;
				} else {
					y += s2;
				}
				e -= 2 * delta_x;
			}
			if (Interchange) {
				y += s2;
			} else {
				x += s1;
			}
			e += 2 * delta_y;
		}
	}

	o_TileCount = tileCounts;
	return pTileInfo;

}

#define LENGTHGRID 10 // length-grid size of vector

double gdem_GetLengthWithPolygon(CGDM_LocationArr &i_arrPt, GDM_DEM_TILE* pTile)
{
	if (pTile == NULL)
		return 0;

	double dTotalLength = 0;
	int nSize = i_arrPt.size();

	for (int i = 0; i < nSize; i++)
	{
		CGDM_Location3D vi; // interval vector for grid
		CGDM_Location3D vc; // rest vector between two polygon point
		CGDM_Location3D vr; // real vector of grid point
		CGDM_Location3D vp1; // vector of cross point
		CGDM_Location3D vp2; // vector of cross point
		CGDM_Location3D vp3; // vector of cross point
		CGDM_Location3D vp4; // vector of cross point
		CGDM_Location3D vt; // temporary vector
		CGDM_Location3D v;  // real vector with real height
		CGDM_Location3D v1; // real vector with real height
		CGDM_Location3D ve; // end vector
		CGDM_Location3D v10;
		CGDM_Location3D v11;

		int nCount = 0;
		short nInTile = 0;

		int i1 = i - 1;

		if (i == 0)
			i1 = nSize - 1;

		// z initialize
		i_arrPt[i].m_tZ = 0;
		i_arrPt[i1].m_tZ = 0;

		// check number of polygon point in DEM tile
		if (i_arrPt[i].m_tX >= pTile->minLongitude && i_arrPt[i].m_tX <= pTile->maxLongitude 
			&& i_arrPt[i].m_tY >= pTile->minLatitude && i_arrPt[i].m_tX <= pTile->maxLatitude)
			nInTile++;

		if (i_arrPt[i1].m_tX >= pTile->minLongitude && i_arrPt[i1].m_tX <= pTile->maxLongitude 
			&& i_arrPt[i1].m_tY >= pTile->minLatitude && i_arrPt[i1].m_tX <= pTile->maxLatitude)
			nInTile = nInTile + 2;

		if (nInTile == 0)
		{
			// get vector cross point between two vectors
			if (gdem_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->minLatitude, pTile->minLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp1))
			{
				v10 = vp1;
				nCount++;
			}

			if (gdem_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp2))
			{
				if (nCount == 0)
					v10 = vp2;
				else if (nCount == 1)
					v11 = vp2;

				nCount++;
			}

			if (gdem_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp3))
			{
				if (nCount == 0)
					v10 = vp2;
				else if (nCount == 1)
					v11 = vp2;

				nCount++;
			}

			if (gdem_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->minLatitude, pTile->minLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp4))
			{
				if (nCount == 1)
					v11 = vp2;
				nCount++;
			}

			if (nCount != 2)
				continue;
		}
		else if (nInTile == 1 || nInTile == 2)
		{
			// get vector cross point between two vectors
			if (gdem_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->minLatitude, pTile->minLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp1))
				vt = vp1;
			else if (gdem_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp2))
				vt = vp2;
			else if (gdem_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp3))
				vt = vp3;
			else if (gdem_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->minLatitude, pTile->minLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp4))
				vt = vp4;
			else
				vt = i_arrPt[i1];
		}
		else
			vt = i_arrPt[i1];

		if (nInTile == 2)
		{
			v10 = vt;
			v11 = i_arrPt[i1];
		}
		else if (nInTile == 1 || nInTile == 3)
		{
			v10 = i_arrPt[i];
			v11 = vt;
		}

		vc = v10 - v11;
		vr = v11;
		ve = v10;

		// calculate grid vector
		vi = vc;
		vi = vi.setLength(LENGTHGRID);

		CGDM_Vector3DF v3D;
		CGDM_Vector3DF v3D1;
		double dHeight;

		while (vc > vi)
		{
			v1 = vr;
			gdem_GetHeightFromDemTile(v1, pTile);
			// get 3D coordinate
			gdem_Get3DPointFromLocation(v1, v3D1, dHeight);

			vr = vr + vi;
			v = vr;
			gdem_GetHeightFromDemTile(v, pTile);
			// get 3D coordinate
			gdem_Get3DPointFromLocation(v, v3D, dHeight);

			dTotalLength += (v3D - v3D1).getLength();
			vc = vc - vi;
		}

		gdem_GetHeightFromDemTile(ve, pTile);
		// get 3D coordinate
		gdem_Get3DPointFromLocation(ve, v3D, dHeight);

		gdem_GetHeightFromDemTile(vr, pTile);
		// get 3D coordinate
		gdem_Get3DPointFromLocation(vr, v3D1, dHeight);

		dTotalLength += (v3D - v3D1).getLength();
	}

	return dTotalLength;
}

bool gdem_GetCrossLocationBetweenTwoLine(double x1, double y1, double x2, double y2, CGDM_Location3D i_Pt3, CGDM_Location3D i_Pt4, CGDM_Location3D& vp)
{
	// get cross point between two line
	bool bResult = false;

	double x3 = i_Pt3.m_tX;
	double y3 = i_Pt3.m_tY;
	double x4 = i_Pt4.m_tX;
	double y4 = i_Pt4.m_tY;

	if (((x1 == x3 && y1 == y3) || (x2 == x3 && y2 == y3) ) || ((x1 == x4 && y1 == y4) || (x2 == x4 && y2 == y4)))  return false;

	double denominator = (x1 - x2)*(y3 - y4) - (x3 - x4)*(y1 - y2);

	if (denominator != 0)
	{
		vp.m_tX = ((x1 * y2 - x2 * y1)*(x3 - x4) - (x3 * y4 - x4 * y3)*(x1 - x2)) / denominator;
		vp.m_tY = ((x1 * y2 - x2 * y1)*(y3 - y4) - (x3 * y4 - x4 * y3)*(y1 - y2)) / denominator;
		
		if ((x1 > x2) && (y1 > y2))
		{
			if ((vp.m_tX < x1 && vp.m_tX > x2)  && (vp.m_tY < y1 && vp.m_tY > y2) )
				bResult = true;
		}
		else if ((x1 > x2) && (y1 < y2))
		{
			if ((vp.m_tX < x1 && vp.m_tX > x2)  && (vp.m_tY > y1 && vp.m_tY < y2) )
				bResult = true;
		}
		else if ((x1 < x2) && (y1 > y2))
		{
			if ((vp.m_tX > x1 && vp.m_tX < x2)  && (vp.m_tY < y1 && vp.m_tY > y2) )
				bResult = true;
		}
		else
		{
			if ((vp.m_tX > x1 && vp.m_tX < x2)  && (vp.m_tY > y1 && vp.m_tY < y2) )
				bResult = true;
		}
	}

	return bResult;
}

double gdem_GetAreaWithPolygon(CGDM_LocationArr &i_arrPt, GDM_DEM_TILE* pTile)
{
	if (pTile == NULL)
		return 0;

	// get area of polygon in DEM tile
	double dTotalArea = 0;
	double dx = (pTile->maxLongitude - pTile->minLongitude) / GDEM_MAX_DEM_DIVISION;
	double dy = (pTile->maxLatitude - pTile->minLatitude) / GDEM_MAX_DEM_DIVISION;

	CGDM_LocationPolygonArr arrayPolygon;

	for (int i = 0; i < GDEM_MAX_DEM_PT_COUNT; i++)
	{
		for (int j = 0; j < GDEM_MAX_DEM_PT_COUNT; j++)
		{
			double dMinX = pTile->minLongitude + dx * i;
			double dMaxX = pTile->minLongitude + dx * (i + 1);
			double dMinY = pTile->minLatitude + dy * j;
			double dMaxY = pTile->minLatitude + dy * (j + 1);

			int nCount = gdem_GetPolygonArrWithGrid(i_arrPt, dMinX, dMaxX, dMinY, dMaxY, arrayPolygon);

			if (nCount > 0)
			{
				for (int k = 0; k < nCount; k++)
					gdem_GetAreaWithPolygonInGrid(arrayPolygon[k], dTotalArea, pTile);
			}
		}
	}

	return dTotalArea;
}

void gdem_GetAreaWithPolygonInGrid(CGDM_LocationArr &i_arrPt, double& dTotalArea, GDM_DEM_TILE* pTile)
{
	int nSize = i_arrPt.size();
	int nk1 = -1;
	int nk2 = -1;
	int nSeparateStart = 0;
	gdm_array<int> indexarray;
	int k = 0;

	if (nSize == 3)
		return gdem_GetAreaWithBaseTriangle(i_arrPt, dTotalArea, pTile);

	// get angle of points
	for (k = 0; k < nSize; k++)
	{
		int k1 = k - 1;

		if (k == 0)
			k1 = nSize - 1;

		int k2 = k + 1;

		if (k == nSize - 1)
			k2 = 0;

		// z initialize
		i_arrPt[k].m_tZ = 0;
		i_arrPt[k1].m_tZ = 0;
		i_arrPt[k2].m_tZ = 0;

		CGDM_Location3D vc;

		vc = (i_arrPt[k] - i_arrPt[k1]).crossProduct(i_arrPt[k2] - i_arrPt[k]);

		// check array index of concave angle 
		if (vc.m_tZ < 0)
		{
			if (nk1 == -1)
			{
				nk1 = k;
				k++;
			}
			else
			{
				indexarray.push_back(k);
			}
		}
	}

	// search concave point is in minimum length from nk1
	int nsmin = nSize;
	int nArraySize = indexarray.size();

	for (k = 0; k < nArraySize; k++)
	{
		int ns1 = indexarray[k] - nk1;
		int ns2 = nk1 + indexarray.size() - indexarray[k];
		int ns = ns1;

		if (ns1 > ns2)
			ns = ns2;
		
		if (nsmin > ns)
		{
			nsmin = ns;
			nk2 = indexarray[k];
		}
	}

	// if nk2 is next pint of nk1, nk2 is invalid concave point
	if ((nk2 + 1) == nk1 || (nk1 == 0 && nk2 == nSize - 1))
		nk2 = -1;

	if (nk1 != -1 && nk2 != -1)
	{
		// separate until number of concave point is one in polygon
		CGDM_LocationArr arrPt1;
		CGDM_LocationArr arrPt2;
		int j;
		int n;

		for (int i = 0; i < nSize; i++)
		{
			j = nk1 + i;
			n = j;
			
			if (j >= nSize)
				n = j - nSize;

			if (j == nk1 || j == nk2)
			{
				arrPt1.push_back(i_arrPt[n]);
				arrPt2.push_back(i_arrPt[n]);
			}
			else if (j > nk1 && j < nk2)
				arrPt1.push_back(i_arrPt[n]);
			else if (j > nk2)
				arrPt2.push_back(i_arrPt[n]);

			gdem_GetAreaWithPolygonInGrid(arrPt1, dTotalArea, pTile);
			gdem_GetAreaWithPolygonInGrid(arrPt2, dTotalArea, pTile);
		}
	}
	else if (nk1 != -1 && nk2 == -1)
	{
		// start separation from concave point to triangles
		nSeparateStart = nk1;

		for (int n = 0; n < nSize - 2; n++)
		{
			CGDM_LocationArr arrPt3;

			for (int m = 0; m < 3; m++)
			{
				CGDM_Location3D v;
				int nIndex = n + m + nk1;

				if (nIndex >= nSize)
					nIndex = nIndex - nSize;

				v.m_tX = i_arrPt[nIndex].m_tX;
				v.m_tY = i_arrPt[nIndex].m_tY;

				arrPt3.push_back(v);
			}

			// calculate of triangle
			//gdem_GetAreaWithTriangle(arrPt3, dTotalArea, pTile);
			gdem_GetAreaWithBaseTriangle(arrPt3, dTotalArea, pTile);
		}
	}
	else if (nk1 == -1 && nk2 == -1)
	{
		// start separation from 0 to triangles
		for (int n = 0; n < nSize - 2; n++)
		{
			CGDM_LocationArr arrPt4;

			for (int m = 0; m < 3; m++)
			{
				CGDM_Location3D v;
				int nIndex = n + m;

				v.m_tX = i_arrPt[nIndex].m_tX;
				v.m_tY = i_arrPt[nIndex].m_tY;

				arrPt4.push_back(v);
			}

			// calculate of triangle
			//gdem_GetAreaWithTriangle(arrPt4, dTotalArea, pTile);
			gdem_GetAreaWithBaseTriangle(arrPt4, dTotalArea, pTile);
		}
	}

	return;
}

#define AREAGRID 10 // area-grid size of vector
void gdem_GetAreaWithTriangle(CGDM_LocationArr &i_arrPt, double &dTotalArea, GDM_DEM_TILE* pTile)
{
	// search maximum line of side lines
	int nSeparate_Index = 0;

	double d1 = (i_arrPt[1] - i_arrPt[0]).getLength();
	double d2 = (i_arrPt[2] - i_arrPt[1]).getLength();
	double d3 = (i_arrPt[0] - i_arrPt[2]).getLength();

	if (d1 <= d2)
	{
		if (d2 <= d3 && d3 >= AREAGRID)
			nSeparate_Index = 3;
		else if (d2 >= AREAGRID)
			nSeparate_Index = 2;
	}
	else
	{
		if (d1 <= d3 && d3 >= AREAGRID)
			nSeparate_Index = 3;
		else if (d1 >= AREAGRID)
			nSeparate_Index = 1;
	}

	// if maximum line is not limit, separate triangle to two triangle
	CGDM_LocationArr newarrPt1(i_arrPt);
	CGDM_LocationArr newarrPt2(i_arrPt);

	switch (nSeparate_Index)
	{
	case 0:
		return gdem_GetAreaWithBaseTriangle(i_arrPt, dTotalArea, pTile);
	case 1: // d1
		{
			newarrPt1.erase(1);
			newarrPt1.push_back((i_arrPt[0] + i_arrPt[1]) / 2);
			newarrPt2.erase(2);
			newarrPt2.push_back((i_arrPt[0] + i_arrPt[1]) / 2);
		}
		break;
	case 2: // d2
		{
			newarrPt1.erase(1);
			newarrPt1.push_back((i_arrPt[1] + i_arrPt[2]) / 2);
			newarrPt2.erase(2);
			newarrPt2.push_back((i_arrPt[1] + i_arrPt[2]) / 2);
		}
		break;
	case 3: // d3
		{
			newarrPt1.erase(0);
			newarrPt1.push_back((i_arrPt[0] + i_arrPt[2]) / 2);
			newarrPt2.erase(2);
			newarrPt2.push_back((i_arrPt[0] + i_arrPt[2]) / 2);
		}
		break;
	}

	// calculate separated triangles
	gdem_GetAreaWithTriangle(newarrPt1, dTotalArea, pTile);
	gdem_GetAreaWithTriangle(newarrPt2, dTotalArea, pTile);
}

void gdem_GetAreaWithBaseTriangle(CGDM_LocationArr &i_arrPt, double &dTotalArea, GDM_DEM_TILE* pTile)
{
	// get height
	// get 3D coordinate
	CGDM_VectorArr arr3D;

	for (int i = 0; i < 3; i++)
	{
		CGDM_Vector3DF v3D;
		double dHeight;

		gdem_GetHeightFromDemTile(i_arrPt[i], pTile);
		gdem_Get3DPointFromLocation(i_arrPt[i], v3D, dHeight);

		arr3D.push_back(v3D);
	}

	// get area
	CGDM_Vector3DF vt = (arr3D[1] - arr3D[0]).crossProduct(arr3D[2] - arr3D[1]);

	// add area
	dTotalArea = dTotalArea + vt.getLength() / 2;
}

void gdem_CalcNormalVector(IN CGDM_Vector3DF a_Vec1, IN CGDM_Vector3DF a_Vec2, OUT CGDM_Vector3DF& normalVec)
{
	CGDM_Vector3DF vecProduct;
	CGDM_Vector3DF zero(0.f, 0.f, 0.f);
	double vecDist;
	
	vecProduct = a_Vec1.crossProduct(a_Vec2);
	vecDist = vecProduct.getLength();

	if (vecDist == 0)
		normalVec = zero;
	else
		normalVec = vecProduct / vecDist;			
}

int gdem_CalcAngle(CGDM_VectorArr a_NormalVecs, CGDM_VectorArr a_PosVecs, double& a_angle)
{
	int result = 0;
	double scalar = 0;	
	double angle = 0;
	double dist1 = 0;
	double dist2 = 0;
	CGDM_Vector3DF vecTemp;

	int nSize = a_NormalVecs.size();
	if (nSize < 0)
		return 0;

	for (int i = 0; i < nSize; i++)
	{
		// Calculate Angle...
		a_PosVecs[i].normalize();
		vecTemp = a_PosVecs[i];
		scalar = a_NormalVecs[i].dotProduct(vecTemp);
		dist1 = a_NormalVecs[i].getLength();
		dist2 = vecTemp.getLength();

		if ((dist1 != 0) && (dist2 != 0))
		{
			angle += radian2degree(PAI - acosf(scalar / (dist1 * dist2)));
		}
	}

	a_angle = angle / GDEM_MAX_VERTEX_COUNTS;
	return result;
}

// 면적계산 
double			gdem_GetAreaInMesh(GDEM_MESH &mesh)
{
	double dArea = 0;
	CGDM_VectorArr arr3D;
	CGDM_Vector3DF v3D;

	gdem_Get3DPointFromLocation(mesh.pt1, v3D, mesh.pt1.m_tZ);
	arr3D.push_back(v3D);
	gdem_Get3DPointFromLocation(mesh.pt2, v3D, mesh.pt2.m_tZ);
	arr3D.push_back(v3D);
	gdem_Get3DPointFromLocation(mesh.pt3, v3D, mesh.pt3.m_tZ);
	arr3D.push_back(v3D);

	// calculate area
	CGDM_Vector3DF vt = (arr3D[1] - arr3D[0]).crossProduct(arr3D[2] - arr3D[1]);
	dArea = vt.getLength() / 2;

	return dArea;
}

// 체적계산 
// param: height -> 해발고
double          gdem_GetVolumeInMesh(GDEM_MESH &mesh, double height)
{
	double dVolume = 0;

	// approximation of height
	double dCalHeight = (mesh.pt1.m_tZ + mesh.pt2.m_tZ + mesh.pt3.m_tZ) / 3;
	
	if (height > dCalHeight)
	{
		// calculate shadow area
		GDEM_MESH shadowmesh = mesh;
		shadowmesh.pt1.m_tZ = 0;
		shadowmesh.pt2.m_tZ = 0;
		shadowmesh.pt3.m_tZ = 0;

		double dShadowArea = gdem_GetAreaInMesh(shadowmesh);

		// calculate volume
		dVolume = dShadowArea * (height - dCalHeight);
	}

	return dVolume;
}

// 길이계산.RYM
double			gdem_GetLengthWithPolygon(CGDM_LocationArr &arrPt, bool bClose)
{
	double dLength = 0;
	double dHeight;
	CGDM_Vector3DF v3D1;
	CGDM_Vector3DF v3D2;

	int nCount = arrPt.size();

	if (bClose)
	{
		for (int i = 0; i < nCount; i++)
		{
			int i1 = (i + 1) % nCount;

			gdem_Get3DPointFromLocation(arrPt[i], v3D1, dHeight);
			gdem_Get3DPointFromLocation(arrPt[i1], v3D2, dHeight);

			// calculate length
			dLength += (v3D2 - v3D1).getLength();
		}
	}
	else
	{
		for (int i = 0; i < nCount - 1; i++)
		{
			int i1 = i + 1;

			gdem_Get3DPointFromLocation(arrPt[i], v3D1, dHeight);
			gdem_Get3DPointFromLocation(arrPt[i1], v3D2, dHeight);

			// calculate length
			dLength += (v3D2 - v3D1).getLength();
		}
	}

	return dLength;
}

// 최대경사점 계산
void			gdem_GetMaxRatePos(IN CGDM_LocationArr &arrPt, OUT GDM_POSITION& pos)
{
	
	double angle = 0;
	int index = 0;
	CGDM_VectorArr vecNormalArr; // Normal Vectors of Cross face	
	CGDM_Vector3DF vecTemp; // temporary vector	
	CGDM_VectorArr posArr;
	CGDM_VectorArr posVertexArr;	

	int nSize = arrPt.size();

	for (int i = 0; i < nSize; i++)
	{
		CGDM_Vector3DF tempVec;
		double height = 0;
		gdem_Get3DPointFromLocation(arrPt[i], tempVec, height);
		posVertexArr.push_back(tempVec);
	}	

	// Calculate normal vector
	gdem_CalcNormalVector(posVertexArr[3] - posVertexArr[0], posVertexArr[2] - posVertexArr[0], vecTemp);
	vecNormalArr.push_back(vecTemp);	
	posArr.push_back(posVertexArr[0]);
	gdem_CalcNormalVector(posVertexArr[0] - posVertexArr[1], posVertexArr[2] - posVertexArr[1], vecTemp);
	vecNormalArr.push_back(vecTemp);
	posArr.push_back(posVertexArr[1]);
	gdem_CalcNormalVector(posVertexArr[0] - posVertexArr[1], posVertexArr[3] - posVertexArr[1], vecTemp);
	vecNormalArr.push_back(vecTemp);
	posArr.push_back(posVertexArr[1]);
	gdem_CalcNormalVector(posVertexArr[1] - posVertexArr[2], posVertexArr[3] - posVertexArr[2], vecTemp);
	vecNormalArr.push_back(vecTemp);
	posArr.push_back(posVertexArr[2]);

	// Calculate Angle between normal vector with face vector
	index = gdem_CalcAngle(vecNormalArr, posArr, angle);
	
	pos.longitude	= arrPt[index].m_tX;
	pos.latitude	= arrPt[index].m_tY;
	pos.height		= arrPt[index].m_tZ;
	pos.rate		= angle;				
}

// 최대/최소높이 계산
void gdem_GetMinMaxHeightPos(IN CGDM_LocationArr &arrPt, OUT GDM_POSITION& minPos, OUT GDM_POSITION& maxPos)
{
	int nCount = arrPt.size();
	double dminHeight = 100000;
	double dmaxHeight = -100000;

	for (int i = 0; i < nCount; i++)
	{
		if (arrPt[i].m_tZ < dminHeight)
		{
			dminHeight = arrPt[i].m_tZ;

			minPos.longitude = arrPt[i].m_tX;
			minPos.latitude = arrPt[i].m_tY;
			minPos.height = arrPt[i].m_tZ;
		}

		if (arrPt[i].m_tZ > dmaxHeight)
		{
			dmaxHeight = arrPt[i].m_tZ;

			maxPos.longitude = arrPt[i].m_tX;
			maxPos.latitude = arrPt[i].m_tY;
			maxPos.height = arrPt[i].m_tZ;
		}
	}
}

extern bool polygonTriangulation(const CGDM_LocationArr &contour,Triangle3DArray &result);

// 3각분할모쥴
void gdem_DivisionToTriangle(IN CGDM_LocationArr &arrPt, OUT CGDM_MeshArr &meshArr)
{		
	int nPts = (int)arrPt.size();
	GDEM_MESH mesh;	
	if (nPts < 3) return;		
	polygonTriangulate(arrPt,meshArr);
}

bool gdem_PolygonIsTwisted(IN CGDM_LocationArr &arrPt, OUT CGDM_LocationPolygonArr & PolygonArray)
{
	int nPts = (int)arrPt.size();
	if (nPts < 3) return false;
	bool bTwist1 = false, bTwist2 = false;
	CGDM_Location3D pt1, pt2, pt3, pt4;
	CGDM_Location3D crosspt;
	CGDM_LocationArr arrPolygon1, arrPolygon2;

	crosspt.m_tX = 0;
	crosspt.m_tY = 0;
	int nStart1 =0, nEnd1 = 0, nStart2 = 0, nEnd2 = 0;
	for (int i = 0; i < nPts; i++)
	{
		int i1 = (i + 1) % nPts;
		for (int j = (i + 1); j < nPts ; j++)
		{
			int j1 = (j + 1) % nPts;
			pt1.m_tX = arrPt[j].m_tX;
			pt1.m_tY = arrPt[j].m_tY,
			pt2.m_tX = arrPt[j1].m_tX;
			pt2.m_tY = arrPt[j1].m_tY;
			pt3.m_tX = arrPt[i].m_tX;
			pt3.m_tY = arrPt[i].m_tY;
			pt4.m_tX = arrPt[i1].m_tX;
			pt4.m_tY = arrPt[i1].m_tY;
			bTwist1 = gdem_GetCrossLocationBetweenTwoLine(arrPt[i].m_tX, arrPt[i].m_tY, arrPt[i1].m_tX, arrPt[i1].m_tY,  pt1, pt2, crosspt);
			bTwist2 = gdem_GetCrossLocationBetweenTwoLine(pt1.m_tX, pt1.m_tY, pt2.m_tX, pt2.m_tY,  pt3, pt4, crosspt);
			if (bTwist1 && bTwist2) 
			{
				nStart1 = i;
				nEnd1 = i1;
				nStart2 = j;
				nEnd2 = j1;
				gdem_PolygonDivided(arrPt, nStart1, nEnd1,  nStart2, nEnd2, crosspt,  arrPolygon1,  arrPolygon2);
				if (arrPolygon1.size() >= 3) PolygonArray.push_back(arrPolygon1);
				//if (true == gdem_PolygonIsTwisted(arrPolygon1, PolygonArray)) 
				//	PolygonArray.erase(PolygonArray.size() - 2);
				if (arrPolygon2.size() >= 3) PolygonArray.push_back(arrPolygon2);
				//if (true == gdem_PolygonIsTwisted(arrPolygon2, PolygonArray)) 
				//	PolygonArray.erase(PolygonArray.size() - 2);
				return true;
			}
		}
	}
	PolygonArray.push_back(arrPt);
	return false;
}
bool gdem_PolygonDivided(IN CGDM_LocationArr &arrPt, IN int &a_nStart1, IN int &a_nEnd1,  IN int &a_nStart2, IN int &a_nEnd2, IN CGDM_Location3D &a_crosspt, OUT CGDM_LocationArr & arrPt1, OUT CGDM_LocationArr & arrPt2)
{
	arrPt1.clear();
	arrPt2.clear();
	int nStart = 0, nEnd = 0;
	int nCount = arrPt.size();

	if ((a_nEnd2 - a_nStart2) == 1) 
	{
		nStart = a_nStart1;
		nEnd = a_nEnd1;
		a_nStart1 = a_nStart2;
		a_nEnd1 = a_nEnd2;
		a_nStart2 = nStart;
		a_nEnd2 = nEnd;
	}

	if (a_nStart1 == 0 || a_nStart2 == 0 || a_nEnd1 == 0 || a_nEnd2 == 0)
	{
			for (int  i = 0; i <= a_nStart1; i++)
			{
				arrPt1.push_back(arrPt[i]);
			}
			arrPt1.push_back(a_crosspt);
			arrPt2.push_back(a_crosspt);

			for (int j = a_nEnd1; j < nCount  ; j++)
			{
				arrPt2.push_back(arrPt[j]);
			}
	}
	else
	{
		if (a_nStart1 > a_nStart2)
		{
			gdm_swap(a_nStart1, a_nStart2);
			gdm_swap(a_nEnd1, a_nEnd2);
		}
		for (int  i = 0; i <= a_nStart1; i++)
		{
			arrPt1.push_back(arrPt[i]);
		}
		arrPt1.push_back(a_crosspt);

		for (int j = a_nEnd2; j < nCount ;j++)
		{
			arrPt1.push_back(arrPt[j]);
		}
		arrPt2.push_back(a_crosspt);
		for (int k = a_nEnd1; k <= a_nEnd2  ; k++)
		{
			arrPt2.push_back(arrPt[k]);
		}
	}

	return true;
}
void 	gdem_Polygon2Polygons(IN CGDM_LocationArr &arrPt, OUT CGDM_LocationPolygonArr & PolygonArray)
{
	polygon2Polygons(arrPt,PolygonArray);
}


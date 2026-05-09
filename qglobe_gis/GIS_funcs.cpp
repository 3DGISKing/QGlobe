#include "GIS_funcs.h"
#include "QGlobe_Triangulate.h"

#define  QGLOBE_MAX_TILE_COUNTS  5000
#define	 QGLOBE_MAX_VERTEX_COUNTS 4
#define  SMALL_NUM  0.0000000000000001 // anything that avoids division overflow

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).m_tX * (v).m_tX + (u).m_tY * (v).m_tY + (u).m_tZ * (v).m_tZ)
#define cross(u,v,n)  n.m_tX = (u).m_tY * (v).m_tZ - (u).m_tZ * (v).m_tY;\
	n.m_tY = (u).m_tZ * (v).m_tX - (u).m_tX * (v).m_tZ;\
	n.m_tZ = (u).m_tX * (v).m_tY - (u).m_tY * (v).m_tX;
#define  qglobe_swap(u, v) {double t = v; v = u; u = t;}

int sign(float value) {
	if (value > 0)
		return 1;
	else if (value == 0) 
		return 0;
	else
		return -1;
}

//����, �浵�ڸ�ǥ�κ��� �����߽��ڸ�ǥ(x, y, z) ��� �Լ�.
void qglobe_Get3DPointFromLocation(CQGlobe_Location3D &i_pt, CQGlobe_Vector3DF &o_pt, double &height)
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

void qglobe_GetLocationFrom3DPoint(CQGlobe_Vector3DF &i_pt, CQGlobe_Location3D &o_pt)
{
}

//  Dem Tile������ ���� ���� �׸����� 4�������� ���̸� ��� �Լ� 
double qglobe_GetHeightFromDemTile(CQGlobe_Location3D &i_pt, QGlobe_DEM_TILE* pTile)
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

	double dBaseLongitude = pTile->tileWidth * pTile->m_sInfo.nX - qglobe_PI;
	double dBaseLatitude  = pTile->tileWidth * pTile->m_sInfo.nY - qglobe_PI;

	int iX = ((i_pt.m_tX - dBaseLongitude) / pTile->gridWidth);
	int iY = ((i_pt.m_tY - dBaseLatitude ) / pTile->gridWidth);

	if(iX >= QGLOBE_MAX_DEM_PT_COUNT || iY >= QGLOBE_MAX_DEM_PT_COUNT)
		return 0;

	h[0] = pTile->m_pData[iY * QGLOBE_MAX_DEM_PT_COUNT + iX];
	h[1] = pTile->m_pData[iY * QGLOBE_MAX_DEM_PT_COUNT + iX + 1];
	h[2] = pTile->m_pData[(iY + 1) * QGLOBE_MAX_DEM_PT_COUNT + iX + 1];
	h[3] = pTile->m_pData[(iY + 1) * QGLOBE_MAX_DEM_PT_COUNT + iX];

	d[0] = i_pt.getDistance(dBaseLongitude + iX * pTile->gridWidth, dBaseLatitude + iY * pTile->gridWidth, 0);
	d[1] = i_pt.getDistance(dBaseLongitude + (iX + 1) * pTile->gridWidth, dBaseLatitude + iY * pTile->gridWidth, 0);
	d[2] = i_pt.getDistance(dBaseLongitude + (iX + 1) * pTile->gridWidth, dBaseLatitude + (iY + 1) * pTile->gridWidth, 0);
	d[3] = i_pt.getDistance(dBaseLongitude + iX * pTile->gridWidth, dBaseLatitude + (iY + 1) * pTile->gridWidth, 0);

	high = (h[0] * d[1] * d[2] * d[3] + d[0] * h[1] * d[2] * d[3] + d[0] * d[1] * h[2] * d[3] + d[0] * d[1] * d[2] * h[3]) /
		(d[1] * d[2] * d[3] + d[0] * d[2] * d[3] + d[0] * d[1] * d[3] + d[0] * d[1] * d[2]);

	i_pt.m_tZ = high;

	return high;
}

//Polygon������ ����������.
double	qglobe_Get2DAreaWithPolygon(CQGlobe_VectorArr &i_arrPt)
{	
	double dArea = 0;
	int nPtCounts = i_arrPt.size();

	for (int i = 0; i< nPtCounts; i++)
	{
		dArea += (i_arrPt[i].m_tX * i_arrPt[i + 1].m_tY - i_arrPt[i + 1].m_tX * i_arrPt[i].m_tY) / 2;
	}
	return dArea;
}

int	qglobe_Sign2DAreaWithPolygon(CQGlobe_VectorArr &i_arrPt, int &a_nCounts)
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

//������������ Polygon�� �������.
double		qglobe_GetAreaWithoutTerrain(CQGlobe_LocationArr &i_arrPt, int i_Level)
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

		area += sign(angle) * qglobe_GetAreaWithoutTerrain(meanLatitude, diffLatitude, diffLongitude);
	}


	area = area / 2;
        return fabs(area) ;
	
}

//delta_latitude, delta_longitude�� ���� ������������ Polygon�� �������.
double		qglobe_GetAreaWithoutTerrain(double latitude, double delta_latitude, double delta_longitude)
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


//�����Ѹ��ڸ�ǥ���� ���� ���̴� Ÿ���� INFO ���.
QGlobe_TILE_INFO qglobe_GetTileFromPt(CQGlobe_Location3D i_Pt, int i_Level)
{
	QGlobe_TILE_INFO pTileInfo;
	CQGlobe_Vector3DF o_Pt;
	double dHeight = 0.0;

	double tileWidth = qglobe_TwoPI / (1 << i_Level);

	pTileInfo.nX = (int)((i_Pt.m_tX + qglobe_PI) / tileWidth);
	pTileInfo.nY = (int)((i_Pt.m_tY + qglobe_PI) / tileWidth);
	pTileInfo.nLevel = i_Level;
	qglobe_Get3DPointFromLocation(i_Pt, o_Pt,dHeight);

	return pTileInfo;
}

//Polygon���ο� ���� ���̴°��� ����.
bool qglobe_PtIsInPolygon(CQGlobe_Location3D &i_pt, CQGlobe_LocationArr &i_arrPt)
{
	unsigned int index, count, crossCount;
	CQGlobe_Location3D *pPt1, *pPt2;
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


// ���а� �ﰢ���� ��ʹ°��� �����ϴ� ����
/************************************************************************
qglobe_LineIsInIntersectWithTriangle(): intersect a ray with a 3D triangle
Input:  i_startPt, i_endPt, i_Pt1, i_Pt2, i_Pt3
Output: *destPt = intersection point (when it exists)
Return: -1 = triangle is degenerate (a segment or point)
0 = disjoint (no intersect)
1 = intersect in unique point I1
2 = are in the same plane
************************************************************************/
int qglobe_LineIsInIntersectWithTriangle(CQGlobe_Location3D &i_startPt, CQGlobe_Location3D &i_endPt, CQGlobe_Vector3DF *i_Pt1 , CQGlobe_Vector3DF *i_Pt2 , CQGlobe_Vector3DF *i_Pt3, CQGlobe_Vector3DF &i_destPt)
{

	CQGlobe_Location3D    u, v, n;             // triangle vectors
	CQGlobe_Vector3DF    w0, w;          // ray vectors
	float     r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = (CQGlobe_Location3D)(i_Pt2 - i_Pt1);
	v = (CQGlobe_Location3D)(i_Pt3 - i_Pt1);	
	cross(u,v,n);
	if (n == CQGlobe_Vector3DF(0))            // triangle is degenerate
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

	i_destPt = (CQGlobe_Vector3DF)(i_startPt + r * i_endPt);           // intersect point of ray and plane

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


//�� ���� ���� ������ ��ʹ� Ÿ�ϵ��� ���� ���.
QGlobe_TILE_INFO *	qglobe_GetTilesFromLine(CQGlobe_Location3D startPos, CQGlobe_Location3D endPos , int i_Level, int *o_TileCount)
{
	int tileCounts = 0;
	QGlobe_TILE_INFO* pTileInfo = NULL;
	QGlobe_TILE_INFO tileInfo1 , tileInfo2;
	pTileInfo = (QGlobe_TILE_INFO *)qGlobeMemMalloc(QGLOBE_MAX_TILE_COUNTS * sizeof(QGlobe_TILE_INFO));
	tileInfo1 = qglobe_GetTileFromPt(startPos, i_Level);
	tileInfo2 = qglobe_GetTileFromPt(endPos, i_Level);

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
		if (tileCounts >= QGLOBE_MAX_TILE_COUNTS)
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

//Polyline�� ��ʹ� Tile���� ���� ���.
QGlobe_TILE_INFO *	qglobe_GetTilesFromPolyLine(CQGlobe_LocationArr &i_arrPt,  int i_Level, int &o_TileCount)
{
	int tileCounts = 0;
	QGlobe_TILE_INFO* pTileInfo = NULL;

	pTileInfo = (QGlobe_TILE_INFO *)qGlobeMemMalloc(QGLOBE_MAX_TILE_COUNTS * sizeof(QGlobe_TILE_INFO));
	for (int k = 0; k < (int)i_arrPt.size() - 1 ; k++)
	{
		QGlobe_TILE_INFO tileInfo1 , tileInfo2;
		tileInfo1 = qglobe_GetTileFromPt(i_arrPt[k], i_Level);
		tileInfo2 = qglobe_GetTileFromPt(i_arrPt[k + 1], i_Level);

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

			if (QGLOBE_MAX_TILE_COUNTS <= tileCounts)
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

//Polygon�� ��ʹ� Ÿ�ϵ��� �������.
QGlobe_TILE_INFO *	qglobe_GetCrossTilesFromPolygon(CQGlobe_LocationArr &i_arrPt,  int i_Level, int &o_TileCount)
{
	int tileCounts = 0;
	QGlobe_TILE_INFO* pTileInfo = NULL;

	pTileInfo = (QGlobe_TILE_INFO *)qGlobeMemMalloc(QGLOBE_MAX_TILE_COUNTS * sizeof(QGlobe_TILE_INFO));
	for (int k = 0; k < (int)i_arrPt.size() ; k++)
	{
		QGlobe_TILE_INFO tileInfo1 , tileInfo2;
		tileInfo1 = qglobe_GetTileFromPt(i_arrPt[k], i_Level);
		tileInfo2 = qglobe_GetTileFromPt(i_arrPt[k + 1], i_Level);

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

			if (QGLOBE_MAX_TILE_COUNTS <= tileCounts)
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

double qglobe_GetLengthWithPolygon(CQGlobe_LocationArr &i_arrPt, QGlobe_DEM_TILE* pTile)
{
	if (pTile == NULL)
		return 0;

	double dTotalLength = 0;
	int nSize = i_arrPt.size();

	for (int i = 0; i < nSize; i++)
	{
		CQGlobe_Location3D vi; // interval vector for grid
		CQGlobe_Location3D vc; // rest vector between two polygon point
		CQGlobe_Location3D vr; // real vector of grid point
		CQGlobe_Location3D vp1; // vector of cross point
		CQGlobe_Location3D vp2; // vector of cross point
		CQGlobe_Location3D vp3; // vector of cross point
		CQGlobe_Location3D vp4; // vector of cross point
		CQGlobe_Location3D vt; // temporary vector
		CQGlobe_Location3D v;  // real vector with real height
		CQGlobe_Location3D v1; // real vector with real height
		CQGlobe_Location3D ve; // end vector
		CQGlobe_Location3D v10;
		CQGlobe_Location3D v11;

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
			if (qglobe_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->minLatitude, pTile->minLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp1))
			{
				v10 = vp1;
				nCount++;
			}

			if (qglobe_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp2))
			{
				if (nCount == 0)
					v10 = vp2;
				else if (nCount == 1)
					v11 = vp2;

				nCount++;
			}

			if (qglobe_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp3))
			{
				if (nCount == 0)
					v10 = vp2;
				else if (nCount == 1)
					v11 = vp2;

				nCount++;
			}

			if (qglobe_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->minLatitude, pTile->minLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp4))
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
			if (qglobe_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->minLatitude, pTile->minLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp1))
				vt = vp1;
			else if (qglobe_GetCrossLocationBetweenTwoLine(pTile->minLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->maxLatitude, i_arrPt[i], i_arrPt[i1], vp2))
				vt = vp2;
			else if (qglobe_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->maxLatitude, pTile->maxLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp3))
				vt = vp3;
			else if (qglobe_GetCrossLocationBetweenTwoLine(pTile->maxLongitude, pTile->minLatitude, pTile->minLongitude, pTile->minLatitude, i_arrPt[i], i_arrPt[i1], vp4))
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

		CQGlobe_Vector3DF v3D;
		CQGlobe_Vector3DF v3D1;
		double dHeight;

		while (vc > vi)
		{
			v1 = vr;
			qglobe_GetHeightFromDemTile(v1, pTile);
			// get 3D coordinate
			qglobe_Get3DPointFromLocation(v1, v3D1, dHeight);

			vr = vr + vi;
			v = vr;
			qglobe_GetHeightFromDemTile(v, pTile);
			// get 3D coordinate
			qglobe_Get3DPointFromLocation(v, v3D, dHeight);

			dTotalLength += (v3D - v3D1).getLength();
			vc = vc - vi;
		}

		qglobe_GetHeightFromDemTile(ve, pTile);
		// get 3D coordinate
		qglobe_Get3DPointFromLocation(ve, v3D, dHeight);

		qglobe_GetHeightFromDemTile(vr, pTile);
		// get 3D coordinate
		qglobe_Get3DPointFromLocation(vr, v3D1, dHeight);

		dTotalLength += (v3D - v3D1).getLength();
	}

	return dTotalLength;
}

bool qglobe_GetCrossLocationBetweenTwoLine(double x1, double y1, double x2, double y2, CQGlobe_Location3D i_Pt3, CQGlobe_Location3D i_Pt4, CQGlobe_Location3D& vp)
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

double qglobe_GetAreaWithPolygon(CQGlobe_LocationArr &i_arrPt, QGlobe_DEM_TILE* pTile)
{
	if (pTile == NULL)
		return 0;

	// get area of polygon in DEM tile
	double dTotalArea = 0;
	double dx = (pTile->maxLongitude - pTile->minLongitude) / QGLOBE_MAX_DEM_DIVISION;
	double dy = (pTile->maxLatitude - pTile->minLatitude) / QGLOBE_MAX_DEM_DIVISION;

	CQGlobe_LocationPolygonArr arrayPolygon;

	for (int i = 0; i < QGLOBE_MAX_DEM_PT_COUNT; i++)
	{
		for (int j = 0; j < QGLOBE_MAX_DEM_PT_COUNT; j++)
		{
			double dMinX = pTile->minLongitude + dx * i;
			double dMaxX = pTile->minLongitude + dx * (i + 1);
			double dMinY = pTile->minLatitude + dy * j;
			double dMaxY = pTile->minLatitude + dy * (j + 1);

			int nCount = qglobe_GetPolygonArrWithGrid(i_arrPt, dMinX, dMaxX, dMinY, dMaxY, arrayPolygon);

			if (nCount > 0)
			{
				for (int k = 0; k < nCount; k++)
					qglobe_GetAreaWithPolygonInGrid(arrayPolygon[k], dTotalArea, pTile);
			}
		}
	}

	return dTotalArea;
}

void qglobe_GetAreaWithPolygonInGrid(CQGlobe_LocationArr &i_arrPt, double& dTotalArea, QGlobe_DEM_TILE* pTile)
{
	int nSize = i_arrPt.size();
	int nk1 = -1;
	int nk2 = -1;
	int nSeparateStart = 0;
	qglobe_array<int> indexarray;
	int k = 0;

	if (nSize == 3)
		return qglobe_GetAreaWithBaseTriangle(i_arrPt, dTotalArea, pTile);

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

		CQGlobe_Location3D vc;

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
		CQGlobe_LocationArr arrPt1;
		CQGlobe_LocationArr arrPt2;
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

			qglobe_GetAreaWithPolygonInGrid(arrPt1, dTotalArea, pTile);
			qglobe_GetAreaWithPolygonInGrid(arrPt2, dTotalArea, pTile);
		}
	}
	else if (nk1 != -1 && nk2 == -1)
	{
		// start separation from concave point to triangles
		nSeparateStart = nk1;

		for (int n = 0; n < nSize - 2; n++)
		{
			CQGlobe_LocationArr arrPt3;

			for (int m = 0; m < 3; m++)
			{
				CQGlobe_Location3D v;
				int nIndex = n + m + nk1;

				if (nIndex >= nSize)
					nIndex = nIndex - nSize;

				v.m_tX = i_arrPt[nIndex].m_tX;
				v.m_tY = i_arrPt[nIndex].m_tY;

				arrPt3.push_back(v);
			}

			// calculate of triangle
			//qglobe_GetAreaWithTriangle(arrPt3, dTotalArea, pTile);
			qglobe_GetAreaWithBaseTriangle(arrPt3, dTotalArea, pTile);
		}
	}
	else if (nk1 == -1 && nk2 == -1)
	{
		// start separation from 0 to triangles
		for (int n = 0; n < nSize - 2; n++)
		{
			CQGlobe_LocationArr arrPt4;

			for (int m = 0; m < 3; m++)
			{
				CQGlobe_Location3D v;
				int nIndex = n + m;

				v.m_tX = i_arrPt[nIndex].m_tX;
				v.m_tY = i_arrPt[nIndex].m_tY;

				arrPt4.push_back(v);
			}

			// calculate of triangle
			//qglobe_GetAreaWithTriangle(arrPt4, dTotalArea, pTile);
			qglobe_GetAreaWithBaseTriangle(arrPt4, dTotalArea, pTile);
		}
	}

	return;
}

#define AREAGRID 10 // area-grid size of vector
void qglobe_GetAreaWithTriangle(CQGlobe_LocationArr &i_arrPt, double &dTotalArea, QGlobe_DEM_TILE* pTile)
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
	CQGlobe_LocationArr newarrPt1(i_arrPt);
	CQGlobe_LocationArr newarrPt2(i_arrPt);

	switch (nSeparate_Index)
	{
	case 0:
		return qglobe_GetAreaWithBaseTriangle(i_arrPt, dTotalArea, pTile);
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
	qglobe_GetAreaWithTriangle(newarrPt1, dTotalArea, pTile);
	qglobe_GetAreaWithTriangle(newarrPt2, dTotalArea, pTile);
}

void qglobe_GetAreaWithBaseTriangle(CQGlobe_LocationArr &i_arrPt, double &dTotalArea, QGlobe_DEM_TILE* pTile)
{
	// get height
	// get 3D coordinate
	CQGlobe_VectorArr arr3D;

	for (int i = 0; i < 3; i++)
	{
		CQGlobe_Vector3DF v3D;
		double dHeight;

		qglobe_GetHeightFromDemTile(i_arrPt[i], pTile);
		qglobe_Get3DPointFromLocation(i_arrPt[i], v3D, dHeight);

		arr3D.push_back(v3D);
	}

	// get area
	CQGlobe_Vector3DF vt = (arr3D[1] - arr3D[0]).crossProduct(arr3D[2] - arr3D[1]);

	// add area
	dTotalArea = dTotalArea + vt.getLength() / 2;
}

void qglobe_CalcNormalVector(IN CQGlobe_Vector3DF a_Vec1, IN CQGlobe_Vector3DF a_Vec2, OUT CQGlobe_Vector3DF& normalVec)
{
	CQGlobe_Vector3DF vecProduct;
	CQGlobe_Vector3DF zero(0.f, 0.f, 0.f);
	double vecDist;
	
	vecProduct = a_Vec1.crossProduct(a_Vec2);
	vecDist = vecProduct.getLength();

	if (vecDist == 0)
		normalVec = zero;
	else
		normalVec = vecProduct / vecDist;			
}

int qglobe_CalcAngle(CQGlobe_VectorArr a_NormalVecs, CQGlobe_VectorArr a_PosVecs, double& a_angle)
{
	int result = 0;
	double scalar = 0;	
	double angle = 0;
	double dist1 = 0;
	double dist2 = 0;
	CQGlobe_Vector3DF vecTemp;

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

	a_angle = angle / QGLOBE_MAX_VERTEX_COUNTS;
	return result;
}

// ������� 
double			qglobe_GetAreaInMesh(QGLOBE_MESH &mesh)
{
	double dArea = 0;
	CQGlobe_VectorArr arr3D;
	CQGlobe_Vector3DF v3D;

	qglobe_Get3DPointFromLocation(mesh.pt1, v3D, mesh.pt1.m_tZ);
	arr3D.push_back(v3D);
	qglobe_Get3DPointFromLocation(mesh.pt2, v3D, mesh.pt2.m_tZ);
	arr3D.push_back(v3D);
	qglobe_Get3DPointFromLocation(mesh.pt3, v3D, mesh.pt3.m_tZ);
	arr3D.push_back(v3D);

	// calculate area
	CQGlobe_Vector3DF vt = (arr3D[1] - arr3D[0]).crossProduct(arr3D[2] - arr3D[1]);
	dArea = vt.getLength() / 2;

	return dArea;
}

// ü����� 
// param: height -> �ع߰�
double          qglobe_GetVolumeInMesh(QGLOBE_MESH &mesh, double height)
{
	double dVolume = 0;

	// approximation of height
	double dCalHeight = (mesh.pt1.m_tZ + mesh.pt2.m_tZ + mesh.pt3.m_tZ) / 3;
	
	if (height > dCalHeight)
	{
		// calculate shadow area
		QGLOBE_MESH shadowmesh = mesh;
		shadowmesh.pt1.m_tZ = 0;
		shadowmesh.pt2.m_tZ = 0;
		shadowmesh.pt3.m_tZ = 0;

		double dShadowArea = qglobe_GetAreaInMesh(shadowmesh);

		// calculate volume
		dVolume = dShadowArea * (height - dCalHeight);
	}

	return dVolume;
}

// ���̰��.RYM
double			qglobe_GetLengthWithPolygon(CQGlobe_LocationArr &arrPt, bool bClose)
{
	double dLength = 0;
	double dHeight;
	CQGlobe_Vector3DF v3D1;
	CQGlobe_Vector3DF v3D2;

	int nCount = arrPt.size();

	if (bClose)
	{
		for (int i = 0; i < nCount; i++)
		{
			int i1 = (i + 1) % nCount;

			qglobe_Get3DPointFromLocation(arrPt[i], v3D1, dHeight);
			qglobe_Get3DPointFromLocation(arrPt[i1], v3D2, dHeight);

			// calculate length
			dLength += (v3D2 - v3D1).getLength();
		}
	}
	else
	{
		for (int i = 0; i < nCount - 1; i++)
		{
			int i1 = i + 1;

			qglobe_Get3DPointFromLocation(arrPt[i], v3D1, dHeight);
			qglobe_Get3DPointFromLocation(arrPt[i1], v3D2, dHeight);

			// calculate length
			dLength += (v3D2 - v3D1).getLength();
		}
	}

	return dLength;
}

// �ִ����� ���
void			qglobe_GetMaxRatePos(IN CQGlobe_LocationArr &arrPt, OUT QGlobe_POSITION& pos)
{
	
	double angle = 0;
	int index = 0;
	CQGlobe_VectorArr vecNormalArr; // Normal Vectors of Cross face	
	CQGlobe_Vector3DF vecTemp; // temporary vector	
	CQGlobe_VectorArr posArr;
	CQGlobe_VectorArr posVertexArr;	

	int nSize = arrPt.size();

	for (int i = 0; i < nSize; i++)
	{
		CQGlobe_Vector3DF tempVec;
		double height = 0;
		qglobe_Get3DPointFromLocation(arrPt[i], tempVec, height);
		posVertexArr.push_back(tempVec);
	}	

	// Calculate normal vector
	qglobe_CalcNormalVector(posVertexArr[3] - posVertexArr[0], posVertexArr[2] - posVertexArr[0], vecTemp);
	vecNormalArr.push_back(vecTemp);	
	posArr.push_back(posVertexArr[0]);
	qglobe_CalcNormalVector(posVertexArr[0] - posVertexArr[1], posVertexArr[2] - posVertexArr[1], vecTemp);
	vecNormalArr.push_back(vecTemp);
	posArr.push_back(posVertexArr[1]);
	qglobe_CalcNormalVector(posVertexArr[0] - posVertexArr[1], posVertexArr[3] - posVertexArr[1], vecTemp);
	vecNormalArr.push_back(vecTemp);
	posArr.push_back(posVertexArr[1]);
	qglobe_CalcNormalVector(posVertexArr[1] - posVertexArr[2], posVertexArr[3] - posVertexArr[2], vecTemp);
	vecNormalArr.push_back(vecTemp);
	posArr.push_back(posVertexArr[2]);

	// Calculate Angle between normal vector with face vector
	index = qglobe_CalcAngle(vecNormalArr, posArr, angle);
	
	pos.longitude	= arrPt[index].m_tX;
	pos.latitude	= arrPt[index].m_tY;
	pos.height		= arrPt[index].m_tZ;
	pos.rate		= angle;				
}

// �ִ�/�ּҳ��� ���
void qglobe_GetMinMaxHeightPos(IN CQGlobe_LocationArr &arrPt, OUT QGlobe_POSITION& minPos, OUT QGlobe_POSITION& maxPos)
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

extern bool polygonTriangulation(const CQGlobe_LocationArr &contour,Triangle3DArray &result);

// 3�����Ҹ���
void qglobe_DivisionToTriangle(IN CQGlobe_LocationArr &arrPt, OUT CQGlobe_MeshArr &meshArr)
{		
	int nPts = (int)arrPt.size();
	QGLOBE_MESH mesh;	
	if (nPts < 3) return;		
	polygonTriangulate(arrPt,meshArr);
}

bool qglobe_PolygonIsTwisted(IN CQGlobe_LocationArr &arrPt, OUT CQGlobe_LocationPolygonArr & PolygonArray)
{
	int nPts = (int)arrPt.size();
	if (nPts < 3) return false;
	bool bTwist1 = false, bTwist2 = false;
	CQGlobe_Location3D pt1, pt2, pt3, pt4;
	CQGlobe_Location3D crosspt;
	CQGlobe_LocationArr arrPolygon1, arrPolygon2;

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
			bTwist1 = qglobe_GetCrossLocationBetweenTwoLine(arrPt[i].m_tX, arrPt[i].m_tY, arrPt[i1].m_tX, arrPt[i1].m_tY,  pt1, pt2, crosspt);
			bTwist2 = qglobe_GetCrossLocationBetweenTwoLine(pt1.m_tX, pt1.m_tY, pt2.m_tX, pt2.m_tY,  pt3, pt4, crosspt);
			if (bTwist1 && bTwist2) 
			{
				nStart1 = i;
				nEnd1 = i1;
				nStart2 = j;
				nEnd2 = j1;
				qglobe_PolygonDivided(arrPt, nStart1, nEnd1,  nStart2, nEnd2, crosspt,  arrPolygon1,  arrPolygon2);
				if (arrPolygon1.size() >= 3) PolygonArray.push_back(arrPolygon1);
				//if (true == qglobe_PolygonIsTwisted(arrPolygon1, PolygonArray)) 
				//	PolygonArray.erase(PolygonArray.size() - 2);
				if (arrPolygon2.size() >= 3) PolygonArray.push_back(arrPolygon2);
				//if (true == qglobe_PolygonIsTwisted(arrPolygon2, PolygonArray)) 
				//	PolygonArray.erase(PolygonArray.size() - 2);
				return true;
			}
		}
	}
	PolygonArray.push_back(arrPt);
	return false;
}
bool qglobe_PolygonDivided(IN CQGlobe_LocationArr &arrPt, IN int &a_nStart1, IN int &a_nEnd1,  IN int &a_nStart2, IN int &a_nEnd2, IN CQGlobe_Location3D &a_crosspt, OUT CQGlobe_LocationArr & arrPt1, OUT CQGlobe_LocationArr & arrPt2)
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
			qglobe_swap(a_nStart1, a_nStart2);
			qglobe_swap(a_nEnd1, a_nEnd2);
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
void 	qglobe_Polygon2Polygons(IN CQGlobe_LocationArr &arrPt, OUT CQGlobe_LocationPolygonArr & PolygonArray)
{
	polygon2Polygons(arrPt,PolygonArray);
}


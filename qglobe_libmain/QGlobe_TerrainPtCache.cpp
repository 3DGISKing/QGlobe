#include <string.h>
#include "QGlobe_TerrainPtCache.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_Mem.h"
#include "QGlobe_Math.h"
#include "QGlobe_Vector3D.h"
#include "qglobeclientbase.h"
#include "QGlobe_Coord3D.h"
#include "QGlobe_Interface.h"

#define QGlobe_TERRAIN_CACHE_ITEM_COUNT	150
#define QGlobe_TERRAIN_MIN_HEIGHT_RATE		0.5
#define QGlobe_TERRAIN_MAX_HEIGHT_RATE		3

CQGlobe_TerrainPtCache::CQGlobe_TerrainPtCache(void)
{
	SetMaxEntryCnt(QGlobe_TERRAIN_CACHE_ITEM_COUNT);
	m_dHeightRate = 1.0;

	memset(&m_sDefaultTile, 0, sizeof(QGlobe_DEM_TILE));
	m_sDefaultTile.m_sInfo.eSubType = E_QGlobe_SUBDATA_TYPE_DEM;
	m_sDefaultTile.m_sInfo.eMainType= E_QGlobe_MAINDATA_TYPE_DEM;
	CalcutatePts(&m_sDefaultTile, m_pDefaultPts, 1.0);
	m_blHeight=false;
}

CQGlobe_TerrainPtCache::~CQGlobe_TerrainPtCache(void)
{
	Clear();
}

void CQGlobe_TerrainPtCache::SetHeightRate(double rate)
{
	if(fabs(m_dHeightRate - rate) < 0.001)
		return;

	if(rate < QGlobe_TERRAIN_MIN_HEIGHT_RATE || rate > QGlobe_TERRAIN_MAX_HEIGHT_RATE)
		return;

	m_dHeightRate = rate;
	Clear();
}

void CQGlobe_TerrainPtCache::SetEnableHeight(bool enable)
{
	if(m_blHeight == enable)
		return;

	Clear();
	m_blHeight = enable;
}

QGlobe_POINT3D* CQGlobe_TerrainPtCache::GetPts(QGlobe_DEM_TILE *pTile)
{
	QGlobe_POINT3D *pVertexBuf;
	int key1, key2;

	if(pTile == NULL)
		return 0;

	if(!pTile->m_isValidBuffer)
		return m_pDefaultPts;


	key1 = MAKE_CACHE_KEY1(pTile->m_sInfo.nLevel, pTile->m_sInfo.nX, pTile->m_sInfo.nY);
	key2 = MAKE_CACHE_KEY2(pTile->m_sInfo.nLevel, pTile->m_sInfo.nX, pTile->m_sInfo.nY);

	pVertexBuf = (QGlobe_POINT3D*)GetEntry(key1, key2);

	if(pVertexBuf != NULL)
		return pVertexBuf;

	// malloc memory
	pVertexBuf = (QGlobe_POINT3D*)gdmMemMalloc(QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT * sizeof(QGlobe_POINT3D));

	// calculate pts
	CalcutatePts(pTile, pVertexBuf, m_dHeightRate, m_blHeight);

	// add data to cach
	AddEntry(key1, key2, pVertexBuf);

	return pVertexBuf;
}

void CQGlobe_TerrainPtCache::CalcutatePts(QGlobe_DEM_TILE *pTile, QGlobe_POINT3D *pVertexBuf, double dHeightRate, bool blHeight)
{
	int k, count;
	double dLongitude, dLatitude,dBaseLongitude, dBaseLatitude, dNextLongitude, dNextLatitude,dRadius, dXRadius;
	double cellWidth, tileWidth, sinal; // with radian
	int index;

	count = QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT;

	tileWidth = qglobe_TwoPI / (1 << pTile->m_sInfo.nLevel);
	cellWidth = tileWidth / QGLOBE_MAX_DEM_DIVISION;

	dBaseLongitude = tileWidth * pTile->m_sInfo.nX - qglobe_PI;
	dBaseLatitude  = tileWidth * pTile->m_sInfo.nY - qglobe_PI;

	dNextLongitude = tileWidth * (pTile->m_sInfo.nX+1) - qglobe_PI;
	dNextLatitude  = tileWidth * (pTile->m_sInfo.nY+1) - qglobe_PI;

	dLatitude  = dBaseLatitude;	
	index = 0;
	for (int j = 0; j < QGLOBE_MAX_DEM_PT_COUNT;j++){		
		dLongitude = dBaseLongitude;
		sinal = sin(dLatitude);
		for(k = 0; k < QGLOBE_MAX_DEM_PT_COUNT; k++)
		{
			if(blHeight)
				dRadius	   = EARTH_RADIUS + pTile->m_pData[index] * dHeightRate;
			else
				dRadius		= EARTH_RADIUS;

			if(dRadius < EARTH_RADIUS)
				dRadius = EARTH_RADIUS;

			dXRadius = dRadius * cos(dLatitude);
			pVertexBuf[index].m_Z = dRadius * sinal;

			pVertexBuf[index].m_X = dXRadius * cos(dLongitude);
			pVertexBuf[index].m_Y = dXRadius * sin(dLongitude);

			dLongitude += cellWidth;
			if (k == QGLOBE_MAX_DEM_PT_COUNT-1)
				dLongitude = dNextLongitude;

			index++;
		}
		dLatitude  += cellWidth;
		if (j == QGLOBE_MAX_DEM_PT_COUNT-1)
			dLatitude = dNextLatitude;			
	}
}

void CQGlobe_TerrainPtCache::FreeData(void *pData)
{
	gdmMemFree(pData);
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
#define QGlobe_MIN_NORMAL_LEVEL	3

CQGlobe_TerrainDirCache::CQGlobe_TerrainDirCache(void)
{
	SetMaxEntryCnt(QGlobe_TERRAIN_CACHE_ITEM_COUNT);
	m_pPtCache = NULL;
}

CQGlobe_TerrainDirCache::~CQGlobe_TerrainDirCache(void)
{
	Clear();
}

QGlobe_DIRECTION_INFO* CQGlobe_TerrainDirCache::GetInfos(QGlobe_DEM_TILE *pTile)
{
	QGlobe_DIRECTION_INFO *pVertexBuf;
	QGlobe_DIRECTION_INFO *pLeft, *pRight, *pBottom, *pTop;
	int key1, key2;

	if(pTile == NULL)
		return 0;

	key1 = MAKE_CACHE_KEY1(pTile->m_sInfo.nLevel, pTile->m_sInfo.nX, pTile->m_sInfo.nY);
	key2 = MAKE_CACHE_KEY2(pTile->m_sInfo.nLevel, pTile->m_sInfo.nX, pTile->m_sInfo.nY);

	pVertexBuf = (QGlobe_DIRECTION_INFO*)GetEntry(key1, key2);

	if(pVertexBuf != NULL)
		return pVertexBuf;

	// malloc memory
	pVertexBuf = (QGlobe_DIRECTION_INFO*)gdmMemMalloc(QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT * sizeof(QGlobe_DIRECTION_INFO));
	memset(pVertexBuf, 0, QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT * sizeof(QGlobe_DIRECTION_INFO));

	// calculate pts
	CalcutatePts(pTile, pVertexBuf);

	// check merge
	pLeft	= GetInfos(pTile->m_sInfo.nX - 1, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel);
	pRight	= GetInfos(pTile->m_sInfo.nX + 1, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel);
	pTop	= GetInfos(pTile->m_sInfo.nX, pTile->m_sInfo.nY + 1, pTile->m_sInfo.nLevel);
	pBottom	= GetInfos(pTile->m_sInfo.nX, pTile->m_sInfo.nY - 1, pTile->m_sInfo.nLevel);

	MergeLeft(pVertexBuf, pLeft);
	MergeLeft(pRight, pVertexBuf);
	MergeBottom(pVertexBuf, pBottom);
	MergeBottom(pTop, pBottom);

	// add data to cach
	AddEntry(key1, key2, pVertexBuf);

	return pVertexBuf;
}

QGlobe_DIRECTION_INFO* CQGlobe_TerrainDirCache::GetInfos(int nX, int nY, int nLevel)
{
	QGlobe_DIRECTION_INFO *pVertexBuf;
	int key1, key2;

	key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	key2 = MAKE_CACHE_KEY2(nLevel, nX, nY);

	pVertexBuf = (QGlobe_DIRECTION_INFO*)GetEntry(key1, key2);
	
	return pVertexBuf;
}

void CQGlobe_TerrainDirCache::CalcutatePts(QGlobe_DEM_TILE *pTile, QGlobe_DIRECTION_INFO *pInfoBuf)
{
	int k, index;
	QGlobe_POINT3D pVertexBuf[QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT];
	QGlobe_POINT3D pt, pt1, pt2;
	CQGlobe_Vector3DF vt, vt1, vt2, vtDir;
	short *pHeight = pTile->m_pData;
	short h1, h2;

	QGlobe_Coord3D standCoord;
	QGlobe_LOCATION loc;

	if(pTile->m_sInfo.nLevel < QGlobe_MIN_NORMAL_LEVEL)
		return;

	double tileWidth = qglobe_TwoPI / (1 << pTile->m_sInfo.nLevel);

	loc.m_dLongitude = tileWidth * pTile->m_sInfo.nX - qglobe_PI + tileWidth / 2;
	loc.m_dLatitude  = tileWidth * pTile->m_sInfo.nY - qglobe_PI + tileWidth / 2;
	loc.m_dDist = 0;
	qglobe_GetStandardCoord(&loc, &standCoord);

	m_pPtCache->CalcutatePts(pTile, pVertexBuf, 1.0);

	for (int j = 0; j < QGLOBE_MAX_DEM_PT_COUNT;j++){
		for(k = 0; k < QGLOBE_MAX_DEM_PT_COUNT; k++)
		{	
			index = j * QGLOBE_MAX_DEM_PT_COUNT + k;

			loc.m_dLongitude = pTile->minLongitude + pTile->gridWidth * k;
			loc.m_dLatitude  = pTile->minLatitude + pTile->gridWidth * j;
			loc.m_dDist = 0;
			qglobe_GetStandardCoord(&loc, &standCoord);

			// self
			pt = pVertexBuf[index];

			// left
			if(k > 0)
			{
				if(j > 0) // left-bottom
				{
					pt1 = pVertexBuf[index - 1];
					pt2 = pVertexBuf[index - QGLOBE_MAX_DEM_PT_COUNT];
					h1 = pHeight[index - 1];
					h2 = pHeight[index - QGLOBE_MAX_DEM_PT_COUNT];

				}
				else	// top-left
				{
					pt1 = pVertexBuf[index + QGLOBE_MAX_DEM_PT_COUNT];
					pt2 = pVertexBuf[index - 1];
					h1 = pHeight[index + QGLOBE_MAX_DEM_PT_COUNT];
					h2 = pHeight[index - 1];
				}
			}
			else
			{
				if(j > 0) // bottom - right
				{
					pt1 = pVertexBuf[index - QGLOBE_MAX_DEM_PT_COUNT];
					pt2 = pVertexBuf[index + 1];
					h1 = pHeight[index - QGLOBE_MAX_DEM_PT_COUNT];
					h2 = pHeight[index + 1];
				}
				else // right - top
				{
					pt1 = pVertexBuf[index + 1];
					pt2 = pVertexBuf[index + QGLOBE_MAX_DEM_PT_COUNT];
					h1 = pHeight[index + 1];
					h2 = pHeight[index + QGLOBE_MAX_DEM_PT_COUNT];
				}
			}

			if(h1 != h2)
			{
				// calculate normal vector
				vt1.m_tX = pt1.m_X - pt.m_X;
				vt1.m_tY = pt1.m_Y - pt.m_Y;
				vt1.m_tZ = pt1.m_Z - pt.m_Z;

				vt2.m_tX = pt2.m_X - pt.m_X;
				vt2.m_tY = pt2.m_Y - pt.m_Y;
				vt2.m_tZ = pt2.m_Z - pt.m_Z;

				vt = vt1.crossProduct(vt2);
				vt.normalize();

				pInfoBuf[index].norX = (float)vt.m_tX;
				pInfoBuf[index].norY = (float)vt.m_tY;
				pInfoBuf[index].norZ = (float)vt.m_tZ;

				// Calculate Angle...
				double scalar = standCoord.m_z.dotProduct(vt);
				double angle  = PAI/ 2 - acosf(scalar);
				pInfoBuf[index].angle =(short)(angle * 180 / PAI);
				pInfoBuf[index].angle = 90 - pInfoBuf[index].angle;

				// calculate direction (east, west, north, south)
				vtDir = vt;
				vt  = standCoord.m_z.crossProduct(vtDir);
				vt.normalize();
				scalar = standCoord.m_y.dotProduct(vt);
				angle  = acosf(scalar);

				if(standCoord.m_x.dotProduct(vt) > 0)
					angle = -angle;
				if(angle < 0)
					angle = angle + 2 * PAI;
				pInfoBuf[index].direction =(short)(angle * 180 / PAI);
			}
			else
				pInfoBuf[index].direction = 361;

		}
	}
}

void CQGlobe_TerrainDirCache::MergeLeft(QGlobe_DIRECTION_INFO* pInfo, QGlobe_DIRECTION_INFO* pLeft)
{
	if(pInfo == NULL || pLeft == NULL)
		return;
	int k, offset;
	offset = QGLOBE_MAX_DEM_PT_COUNT - 1;

	for (k = 0; k < QGLOBE_MAX_DEM_PT_COUNT;k++)
	{
		pInfo[k * QGLOBE_MAX_DEM_PT_COUNT] = pLeft[k * QGLOBE_MAX_DEM_PT_COUNT + offset];
	}
}

void CQGlobe_TerrainDirCache::MergeBottom(QGlobe_DIRECTION_INFO* pInfo, QGlobe_DIRECTION_INFO* pBottom)
{
	if(pInfo == NULL || pBottom == NULL)
		return;
	int k, offset;

	offset = (QGLOBE_MAX_DEM_PT_COUNT - 1) * QGLOBE_MAX_DEM_PT_COUNT;

	for (k = 0; k < QGLOBE_MAX_DEM_PT_COUNT;k++)
	{
		pInfo[k] = pBottom[k + offset];
	}

}

void CQGlobe_TerrainDirCache::FreeData(void *pData)
{
	gdmMemFree(pData);
}
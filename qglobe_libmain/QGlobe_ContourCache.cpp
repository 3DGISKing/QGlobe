#include "QGlobe_ContourCache.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_Mem.h"
#include "QGlobe_Tile.h"
#include "QGlobe_Math.h"

extern int GDALFeedLine(short* pHeightData, int nXSize, int nYSize, int nInterval, GDALContour *pContour);
extern void GDALGenerateElevationLabelInfo (GDALContour* pContour, int nLevel);

#define MAKE_CONTOUR_KEY1(level, x)	((level << MAX_TILE_LEVEL) | x)
#define MAKE_CONTOUR_KEY2(y, interval)	((interval << 20) | y)

CQGlobe_ContourCache::CQGlobe_ContourCache(void)
{
}

CQGlobe_ContourCache::~CQGlobe_ContourCache(void)
{
	Clear();
}

GDALContour* CQGlobe_ContourCache::GetContourData(int nX, int nY, int nLevel, short* pHeightData, int nInterval)
{
	GDALContour* pData;

	// check cache data
	pData = (GDALContour*)GetEntry(MAKE_CONTOUR_KEY1(nLevel, nX), MAKE_CONTOUR_KEY2(nY, nInterval));

	if(pData != NULL)
		return pData;

	pData = new GDALContour();

	// calculate coordinate
	double tileWidth = qglobe_TwoPI / (1 << nLevel);
	pData->gridWidth = tileWidth / QGLOBE_MAX_DEM_DIVISION;
	pData->minLatitude = tileWidth * nY - qglobe_PI;
	pData->minLongitude = tileWidth * nX - qglobe_PI;
	pData->maxLatitude = pData->minLatitude + tileWidth;
	pData->maxLongitude = pData->minLongitude + tileWidth;
	pData->nCount = 0;

	// call calculate module
	GDALFeedLine(pHeightData, QGLOBE_MAX_DEM_PT_COUNT, QGLOBE_MAX_DEM_PT_COUNT, nInterval, pData);

	// Generate information of elevation labels
	GDALGenerateElevationLabelInfo (pData, nLevel);

	// add data to cach
	AddEntry(MAKE_CONTOUR_KEY1(nLevel, nX), MAKE_CONTOUR_KEY2(nY, nInterval), pData);

	return pData;
}

void CQGlobe_ContourCache::FreeData(void *pData)
{
	GDALContour* pContour = (GDALContour*)pData;

	delete pContour;
}

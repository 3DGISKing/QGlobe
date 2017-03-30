#include "GDM_ContourCache.h"
#include "GDM_CacheMgr.h"
#include "GDM_Mem.h"
#include "GDM_Tile.h"
#include "GDM_Math.h"

extern int GDALFeedLine(short* pHeightData, int nXSize, int nYSize, int nInterval, GDALContour *pContour);
extern void GDALGenerateElevationLabelInfo (GDALContour* pContour, int nLevel);

#define MAKE_CONTOUR_KEY1(level, x)	((level << MAX_TILE_LEVEL) | x)
#define MAKE_CONTOUR_KEY2(y, interval)	((interval << 20) | y)

CGDM_ContourCache::CGDM_ContourCache(void)
{
}

CGDM_ContourCache::~CGDM_ContourCache(void)
{
	Clear();
}

GDALContour* CGDM_ContourCache::GetContourData(int nX, int nY, int nLevel, short* pHeightData, int nInterval)
{
	GDALContour* pData;

	// check cache data
	pData = (GDALContour*)GetEntry(MAKE_CONTOUR_KEY1(nLevel, nX), MAKE_CONTOUR_KEY2(nY, nInterval));

	if(pData != NULL)
		return pData;

	pData = new GDALContour();

	// calculate coordinate
	double tileWidth = gdm_TwoPI / (1 << nLevel);
	pData->gridWidth = tileWidth / GDEM_MAX_DEM_DIVISION;
	pData->minLatitude = tileWidth * nY - gdm_PI;
	pData->minLongitude = tileWidth * nX - gdm_PI;
	pData->maxLatitude = pData->minLatitude + tileWidth;
	pData->maxLongitude = pData->minLongitude + tileWidth;
	pData->nCount = 0;

	// call calculate module
	GDALFeedLine(pHeightData, GDEM_MAX_DEM_PT_COUNT, GDEM_MAX_DEM_PT_COUNT, nInterval, pData);

	// Generate information of elevation labels
	GDALGenerateElevationLabelInfo (pData, nLevel);

	// add data to cach
	AddEntry(MAKE_CONTOUR_KEY1(nLevel, nX), MAKE_CONTOUR_KEY2(nY, nInterval), pData);

	return pData;
}

void CGDM_ContourCache::FreeData(void *pData)
{
	GDALContour* pContour = (GDALContour*)pData;

	delete pContour;
}

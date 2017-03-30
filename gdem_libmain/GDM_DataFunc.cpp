#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_Define.h"
#include "GDM_PyramidMgr.h"
#include "GDM_CacheMgr.h"
#include "GDM_RequestMgr.h"
#include "GDM_DataMgr.h"
#include "GDM_TerrainPtCache.h"
#include "GDM_RenderTile.h"
#include "GDM_SimpleBuildingCache.h"
#include "Layer.h"

#define GDM_MAX_SHAPE_TILES	15
#define GDM_MAX_NAME_TILES	15
#define GDM_MAX_3DMESH_TILES 15

#define GDM_MAX_DISPLAY_SHAPE_LEVEL		15

GDM_LOGIC_TILE_ARR g_gdm_tile_list;

CGDM_Vector3DF g_OffsetBodyPt;

void gdm_ClearPrepareRenderData()
{
	g_gdm_tile_list.nCount = 0;
}

GDM_TILE*	gdm_GetCoverTileAndUpdate(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE eType, int nOffset)
{
	GDM_TILE *pTile = NULL;
	E_GDM_MAINDATA_TYPE mainType = gdm_GetMainType(eType);

	int offsetLevel =nOffset;
	int minLevel = g_pGDMDataMgr->GetMinLevel(eType);

	if((nLevel - offsetLevel) < minLevel)
	{
		if(mainType == E_GDM_MAINDATA_TYPE_NAME)
			return NULL;
		offsetLevel = nLevel - minLevel;
	}

	if(offsetLevel > 0)
	{
		nLevel -= offsetLevel;
		nX >>= offsetLevel;
		nY >>= offsetLevel;
	}

	while (pTile == NULL && nLevel >= minLevel)
	{
		pTile = (GDM_TILE*)g_pGDMDataMgr->m_pCacheMgr->GetEntry(eType, nLevel, nX, nY);

		if (pTile == NULL)
		{
			nLevel--;
			nX >>= 1;
			nY >>= 1;
		}
	}
	
	return pTile;
}

const GDM_LOGIC_TILE_ARR*		gdm_GetRenderTiles()
{
	return &g_gdm_tile_list;
}

void gdm_MakePrepareRenderData()
{
	GDM_TILE_INFO_ARR sTileInfoArr;
	GDM_LOGIC_TILE *pTile;

	int nX, nY, nLevel, maxCount;
	int nOffsetLevel, nImgOffsetLevel, nBaseOffsetLevel;
	double tileWidth;
	
#ifdef GDM_RENDER_APPLY_OFFSET_BODY
	g_pGDMDataMgr->m_pCamera->GetBodyOffset(g_OffsetBodyPt);
#endif

	memset(&g_gdm_tile_list, 0, sizeof(g_gdm_tile_list));

	nBaseOffsetLevel = g_pGDMDataMgr->m_pRequestMgr->GetOffsetLevel();

	g_pGDMDataMgr->m_pPyramidMgr->GetPyramidTiles(&sTileInfoArr);

	g_gdm_tile_list.nCount = sTileInfoArr.count;
	g_gdm_tile_list.nLevel = sTileInfoArr.maxLevel;

	for(int i = 0; i < sTileInfoArr.count; i++)
	{
		pTile = &g_gdm_tile_list.pData[i];
		nX = pTile->nX = sTileInfoArr.data[i].nX;
		nY = pTile->nY = sTileInfoArr.data[i].nY;
		nLevel = pTile->nLevel = sTileInfoArr.data[i].nLevel;
		pTile->area	= sTileInfoArr.data[i].area2D;
		tileWidth = sTileInfoArr.data[i].tileWidth;

		pTile->minLongitude =  sTileInfoArr.data[i].minLongitude;
		pTile->minLatitude	= sTileInfoArr.data[i].minLatitude;
		pTile->maxLongitude = pTile->minLongitude + tileWidth;
		pTile->maxLatitude	= pTile->minLatitude + tileWidth;
    	pTile->offsetLevel = sTileInfoArr.data[i].offsetLevel;

		// check offset level
		nOffsetLevel =nBaseOffsetLevel+pTile->offsetLevel;
		nOffsetLevel=qBound(0,nOffsetLevel,nLevel-2);

		nImgOffsetLevel = sTileInfoArr.data[i].offsetLevel;	
		nImgOffsetLevel=qBound(0,nImgOffsetLevel,nLevel-2);

		// dem tile
		if(g_pGDMDataMgr->m_sOption.layerTextureMode != LTM_TEXTURE || g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
			pTile->pDemTile = (GDM_DEM_TILE*)gdm_GetCoverTileAndUpdate(nX, nY, nLevel, E_GDM_SUBDATA_TYPE_DEM, nOffsetLevel);
		else if(nLevel < 7)
			pTile->pDemTile = (GDM_DEM_TILE*)gdm_GetCoverTileAndUpdate(0, 0, 0, E_GDM_SUBDATA_TYPE_DEM, 0); // 0 level tile
		else
			pTile->pDemTile = NULL;

		if(pTile->pDemTile == NULL)
			pTile->pDemTile = g_pGDMDataMgr->m_pTerrainPtCache->GetDefaultDemTile();

		// texture tile
		if(g_pGDMDataMgr->m_sOption.layerTextureMode == LTM_TEXTURE)
			pTile->pImgTile = (GDM_TEXTURE_TILE*)gdm_GetCoverTileAndUpdate(nX, nY, nLevel, g_pGDMDataMgr->m_pActiveImageLayer->GetSubType(), nImgOffsetLevel);
		else
			pTile->pImgTile = NULL;

		// raster tile
		if(g_pGDMDataMgr->m_sOption.layerTextureMode == LTM_RASTER)
			pTile->prasterTile = gdm_GetCoverTileAndUpdate(nX, nY, nLevel, g_pGDMDataMgr->m_pActiveRasterLayer->GetSubType(), nImgOffsetLevel);
		else
			pTile->prasterTile = NULL;
			
		QList<E_GDM_SUBDATA_TYPE>		arrTypes;
		
		GDM_TILE		*pNameTile;
		GDM_SHAPE_TILE	*pShapeTile;
		GDM_TILE	    *p3dMeshTile;

		pTile->nShapeTileCnt = 0;
		pTile->nNameTileCnt = 0;

		// name tiles
		arrTypes=g_pGDMDataMgr->GetTypes(E_GDM_MAINDATA_TYPE_NAME);
		maxCount = 30 + (int)(g_pGDMDataMgr->m_pCamera->m_fScalarOfDir * GDM_MAX_NAME_TILES);

		// name tiles
		arrTypes=g_pGDMDataMgr->GetTypes(E_GDM_MAINDATA_TYPE_NAME);

		Q_ASSERT(arrTypes.size()==GDM_MAX_NAME_CHILD_TYPE_COUNT);

		maxCount = 30+(int)(g_pGDMDataMgr->m_pCamera->m_fScalarOfDir * sTileInfoArr.count);

		/******************************************
		2013 3 6 ugi
		i °¡ maxCount¸ó³Þ ¿Í·² ÃÍ³ëÂÙ À°ËÎË¾ ÊÐ¼è ·ÃÂÙ³Þ.   °ûµÍ·àµá ¶ìÊ­²ÛÊ¾²÷´ç 
		ËË°ÃË¼ Â£µá°ûµ½Ì© ºÂ²þËæ ÊéÂáË¾ ·ç¾»ºã ËØ³Þ.
		*******************************************/
		if(i < maxCount)
		{
			for(int k = 0; k < arrTypes.size(); k++)
			{
				E_GDM_SUBDATA_TYPE type = arrTypes[k];

				if(!g_pGDMDataMgr->IsVisible(type))
					continue;

				if(pTile->nNameTileCnt >= GDM_MAX_NAME_CHILD_TYPE_COUNT)
					continue;

				pNameTile = (GDM_TILE*)gdm_GetCoverTileAndUpdate(nX, nY, nLevel, type, nOffsetLevel);
				if(pNameTile)
				{
					pTile->sArrNameTile[pTile->nNameTileCnt] = pNameTile;
					pTile->nNameTileCnt++;
				}
			}
		}

		// shape tiles
		arrTypes=g_pGDMDataMgr->GetTypes(E_GDM_MAINDATA_TYPE_SHAPE);
		maxCount = 30 + (int)(g_pGDMDataMgr->m_pCamera->m_fScalarOfDir * GDM_MAX_SHAPE_TILES);

		/******************************************
		2013 3 6 ugi
		i °¡ maxCount¸ó³Þ ¿Í·² ÃÍ³ëÂÙ À°ËÎË¾ ÊÐ¼è ·ÃÂÙ³Þ.   °ûµÍ·àµá ¶ìÊ­²ÛÊ¾²÷´ç 
		ËË°ÃË¼ Â£µá°ûµ½Ì© ºÂ²þËæ ÊéÂáË¾ ·ç¾»ºã ËØ³Þ.
		*******************************************/
		if(i < maxCount)
		{
			for(int k = 0; k < arrTypes.size(); k++)
			{
				E_GDM_SUBDATA_TYPE type = arrTypes[k];

				if(!g_pGDMDataMgr->IsVisible(type))
					continue;

				if(pTile->nShapeTileCnt >= GDM_MAX_CHILD_TYPE_COUNT)
					continue;

				pShapeTile = (GDM_SHAPE_TILE*)gdm_GetCoverTileAndUpdate(nX, nY, nLevel, type, nOffsetLevel);
				if(pShapeTile)
				{
					pTile->sArrShapeTile[pTile->nShapeTileCnt] = pShapeTile;
					pTile->nShapeTileCnt++;
				}
			}					
		}
		
		// 3d mesh tiles
		arrTypes=g_pGDMDataMgr->GetTypes(E_GDM_MAINDATA_TYPE_3D);
		maxCount = 30 + (int)(g_pGDMDataMgr->m_pCamera->m_fScalarOfDir * GDM_MAX_3DMESH_TILES);

		/******************************************
		2013 3 6 ugi
		 i °¡ maxCount¸ó³Þ ¿Í·² ÃÍ³ëÂÙ À°ËÎË¾ ÊÐ¼è ·ÃÂÙ³Þ.   °ûµÍ·àµá ¶ìÊ­²ÛÊ¾²÷´ç 
		 ËË°ÃË¼ Â£µá°ûµ½Ì© ºÂ²þËæ ÊéÂáË¾ ·ç¾»ºã ËØ³Þ.
		*******************************************/

		if(i < maxCount)
		{
			for(int k = 0; k < arrTypes.size(); k++)
			{
				E_GDM_SUBDATA_TYPE type = arrTypes[k];

				if(!g_pGDMDataMgr->IsVisible(type))
					continue;

				if(type==E_GDM_SUBDATA_TYPE_3D_BUILDING)
				{
					if(pTile->n3dMeshTileCnt >= GDM_MAX_CHILD_TYPE_COUNT)
						continue;

					p3dMeshTile = gdm_GetCoverTileAndUpdate(nX, nY, nLevel, type, nOffsetLevel);
					if(p3dMeshTile)
					{
						pTile->sArr3DMeshTile[pTile->n3dMeshTileCnt] = p3dMeshTile;
						pTile->n3dMeshTileCnt++;
					}
				}
				else if(type==E_GDM_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING)
				{
					if(pTile->nPoygonTileCnt >= GDM_MAX_CHILD_TYPE_COUNT)
						continue;

					p3dMeshTile = gdm_GetCoverTileAndUpdate(nX, nY, nLevel, type, nOffsetLevel);
					if(p3dMeshTile)
					{
						pTile->sArrPolygonTile[pTile->nPoygonTileCnt] = p3dMeshTile;
						pTile->nPoygonTileCnt++;
					}
				}
			}
		}

		// contour dem tile
		if((g_pGDMDataMgr->m_sOption.blContour) && nLevel > 2)
		{
			pTile->pContourDemTile = (GDM_DEM_TILE*)gdm_GetCoverTileAndUpdate(nX, nY, nLevel, E_GDM_SUBDATA_TYPE_DEM, nOffsetLevel);
		}
		else
			pTile->pContourDemTile = NULL;

		// calculate edge point
		gdm_get3DWorldPoint(pTile->minLongitude, pTile->minLatitude, pTile->edge[0], 0, pTile->pDemTile, true);
		gdm_get3DWorldPoint(pTile->maxLongitude, pTile->minLatitude, pTile->edge[1], 0, pTile->pDemTile, true);
		gdm_get3DWorldPoint(pTile->minLongitude, pTile->maxLatitude, pTile->edge[3], 0, pTile->pDemTile, true);
		gdm_get3DWorldPoint(pTile->maxLongitude, pTile->maxLatitude, pTile->edge[2], 0, pTile->pDemTile, true);

		pTile->bndCenter.set( sTileInfoArr.data[i].cx, sTileInfoArr.data[i].cy, sTileInfoArr.data[i].cz );
		pTile->bndRadius = sTileInfoArr.data[i].radius;
	}
}

#define IS_IDENTICAL(a, b)	(fabs(a - b) < 0.000000000001)
void   gdm_get3DWorldPoint(double dLongitude, double dLatitude, GDM_POINT3D &pos, double offsetRadius, GDM_DEM_TILE* pTile, bool bHeight)
{
	double dRadius, dXRadius, d;

	dRadius	   = EARTH_RADIUS + offsetRadius;
	if(bHeight && g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
	{
		if(pTile == NULL)
			dRadius += gdm_getHeight(dLongitude, dLatitude);
		else
			dRadius += gdm_getHeightInTile(dLongitude, dLatitude, pTile);
			
	}

	d = dRadius-EARTH_RADIUS;
	if (IS_IDENTICAL(d, 0))
		d = 0;
	dXRadius = dRadius * cos(dLatitude);
	pos.m_Z = dRadius * sin(dLatitude);
	pos.m_X = dXRadius * cos(dLongitude);
	pos.m_Y = dXRadius * sin(dLongitude);
}

void   gdm_get3DWorldPoint(double dLongitude, double dLatitude, CGDM_Point3DF &pos, double offsetRadius, GDM_DEM_TILE* pTile, bool bHeight)
{
	double dRadius, dXRadius, d;

	dRadius	   = EARTH_RADIUS + offsetRadius;
	if(bHeight && g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
	{
		if(pTile == NULL)
			dRadius += gdm_getHeight(dLongitude, dLatitude);
		else
			dRadius += gdm_getHeightInTile(dLongitude, dLatitude, pTile);

	}

	d = dRadius-EARTH_RADIUS;
	if (IS_IDENTICAL(d, 0))
		d = 0;
	dXRadius = dRadius * cos(dLatitude);
	pos.m_tZ = dRadius * sin(dLatitude);
	pos.m_tX = dXRadius * cos(dLongitude);
	pos.m_tY = dXRadius * sin(dLongitude);
}

void   gdm_get3DBodyPoint(double dLongitude, double dLatitude, GDM_POINT3D &pos, double offsetRadius, GDM_DEM_TILE* pTile, bool bHeight)
{
	gdm_get3DWorldPoint(dLongitude, dLatitude, pos, offsetRadius, pTile, bHeight);

#ifdef GDM_RENDER_APPLY_OFFSET_BODY
	pos.m_Z = pos.m_Z - g_OffsetBodyPt.m_tZ;
	pos.m_X = pos.m_X - g_OffsetBodyPt.m_tX;
	pos.m_Y = pos.m_Y - g_OffsetBodyPt.m_tY;
#endif

}


short  gdm_getHeightInTile(double dLongitude, double dLatitude, GDM_DEM_TILE* pTile)
{
	short height;

	if(pTile == NULL)
		return 0;

	if(dLongitude < pTile->minLongitude || dLongitude > pTile->maxLongitude ||
		dLatitude < pTile->minLatitude || dLatitude > pTile->maxLatitude)
		return 0;

	int iX = ((dLongitude - pTile->minLongitude) / pTile->gridWidth + 0.5);
	int iY = ((dLatitude - pTile->minLatitude) / pTile->gridWidth + 0.5);

	height = pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX];

	if(g_pGDMDataMgr)
		height = g_pGDMDataMgr->m_sOption.dem_detail_rate * height;

	return height;
}

GDM_DIRECTION_INFO	gdm_getRateInfoInTile(double dLongitude, double dLatitude, GDM_DEM_TILE* pTile)
{
	GDM_DIRECTION_INFO info;
	GDM_DIRECTION_INFO *pInfos;

	memset(&info, 0, sizeof(info));

	if(dLongitude < pTile->minLongitude || dLongitude > pTile->maxLongitude ||
		dLatitude < pTile->minLatitude || dLatitude > pTile->maxLatitude)
		return info;

	pInfos = g_pGDMDataMgr->m_pTerrainDirCache->GetInfos(pTile);

	int iX = ((dLongitude - pTile->minLongitude) / pTile->gridWidth + 0.5);
	int iY = ((dLatitude - pTile->minLatitude) / pTile->gridWidth + 0.5);

	info = pInfos[iY * GDEM_MAX_DEM_PT_COUNT + iX];
	return info;
}

short  gdm_getMaxHeightInTile(double dLongitude, double dLatitude, GDM_DEM_TILE* pTile)
{
	short height[4];
	short high = 0;


	if(pTile == NULL)
		return 0;

	if(dLongitude < pTile->minLongitude || dLongitude > pTile->maxLongitude ||
		dLatitude < pTile->minLatitude || dLatitude > pTile->maxLatitude)
		return 0;

	memset(height, 0, sizeof(height));

	int iX = ((dLongitude - pTile->minLongitude) / pTile->gridWidth);
	int iY = ((dLatitude - pTile->minLatitude) / pTile->gridWidth);

	height[0] = pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX];
	if(iX < GDEM_MAX_DEM_PT_COUNT)
		height[1] = pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX + 1];
	if(iX < GDEM_MAX_DEM_PT_COUNT && iY < GDEM_MAX_DEM_PT_COUNT)
		height[2] = pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX + 1];
	if(iY < GDEM_MAX_DEM_PT_COUNT)
		height[3] = pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX];

	high = height[0];
	for(int i = 1; i < 4; i++)
	{
		if(high < height[i])
			high = height[i];
	}

	if(g_pGDMDataMgr)
		high = g_pGDMDataMgr->m_sOption.dem_detail_rate * high;

	return high;
}

inline void	gdm_getSpherePoint( double dLongitude, double dLatitude, double dRadius, CGDM_Point3DF* point )
{
	double dXRadius;
	dXRadius = dRadius * cos(dLatitude);
	point->m_tZ = dRadius  * sin(dLatitude);
	point->m_tX = dXRadius * cos(dLongitude);
	point->m_tY = dXRadius * sin(dLongitude);
}

//2013 2 27
//Ë§¶®²÷ Êß°º¶®¸ó±ãËæ Ì©ÂÙ ²ÚËË¶¦ °éÂ×²÷ ¿¸´Å°¡ Ì°¼­Â×¼è Ê±³Þ²÷ °ÃË¾ ¸Ë°ÌÂ×Êî³Þ.
//¼ÑÊÌ¼ê Ì¡°Ò´ª°¡ À°ËÎÌ© ¸ê´¸¶®Ëæ °¡Ä´Ëª ÆÁ ¸Æ°û°¡ ±ýÊò °¡²þºÂËË ËØ³Þ.
//¼ÑÊÌ¼ê Ì¡°Ò´ª°¡ À°ËÎÌ© ¸ê´¸¶®Ëæ ËØË¾ÆÁ ¶¦ °ÖµØ Â×ÊÞ ºã¼³Â×»¤Êï

double  gdm_getExactHeightInTile(double dLongitude, double dLatitude, GDM_DEM_TILE* pTile)
{		
	Q_ASSERT(pTile!=NULL);
	Q_ASSERT(dLongitude>=pTile->minLongitude);
	Q_ASSERT(dLongitude<=pTile->maxLongitude);
	Q_ASSERT(dLatitude>=pTile->minLatitude);
	Q_ASSERT(dLatitude<=pTile->maxLatitude);

#if 1
	short	h[4];		// height
	double	high = 0;
	double	d[4];		// distance from loc

	CGDM_Location3D i_pt;
	i_pt.m_tX = dLongitude;
	i_pt.m_tY = dLatitude;
	i_pt.m_tZ = 0;

	double dBaseLongitude = pTile->minLongitude;
	double dBaseLatitude  = pTile->minLatitude;

	int iX = ((dLongitude - dBaseLongitude) / pTile->gridWidth);
	int iY = ((dLatitude - dBaseLatitude ) / pTile->gridWidth);

	Q_ASSERT(iX < GDEM_MAX_DEM_PT_COUNT);
	Q_ASSERT(iY < GDEM_MAX_DEM_PT_COUNT);

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

	Q_ASSERT(g_pGDMDataMgr!=NULL);
	
	high = g_pGDMDataMgr->m_sOption.dem_detail_rate * high;

	return high;

#else

	if(pTile == NULL)
		return 0;

	double dBaseLongitude = pTile->tileWidth * pTile->m_sInfo.nX - gdm_PI;
	double dBaseLatitude  = pTile->tileWidth * pTile->m_sInfo.nY - gdm_PI;

	int iX = ((dLongitude - dBaseLongitude) / pTile->gridWidth);
	int iY = ((dLatitude - dBaseLatitude ) / pTile->gridWidth);

	if(iX >= GDEM_MAX_DEM_PT_COUNT || iY >= GDEM_MAX_DEM_PT_COUNT)
		return 0;
	//
	// Determine triangle that contains our position
	//
	bool bTri012 = false;
	// Make sure that tile is a regular square.
	if ( (dLongitude - pTile->minLongitude) > (dLatitude - pTile->minLatitude ) )
		bTri012 = true;
	else
		bTri012 = false;

	GDM_LOCATION		triV[3];
	CGDM_Triangle3DF	tri;
	CGDM_Point3DF		triP[3];

	triV[0].m_dLongitude	= pTile->minLongitude;
	triV[0].m_dLatitude		= pTile->minLatitude;
	triV[0].m_dDist			= pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX];
	if ( bTri012 )
	{
		triV[1].m_dLongitude	= pTile->maxLongitude;
		triV[1].m_dLatitude		= pTile->minLatitude;
		triV[1].m_dDist			= pTile->m_pData[iY * GDEM_MAX_DEM_PT_COUNT + iX + 1];

		triV[2].m_dLongitude	= pTile->maxLongitude;
		triV[2].m_dLatitude		= pTile->maxLatitude;
		triV[2].m_dDist			= pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX + 1];
	}
	else
	{
		triV[1].m_dLongitude	= pTile->maxLongitude;
		triV[1].m_dLatitude		= pTile->maxLatitude;
		triV[1].m_dDist			= pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX + 1];

		triV[2].m_dLongitude	= pTile->minLongitude;
		triV[2].m_dLatitude		= pTile->maxLatitude;
		triV[2].m_dDist			= pTile->m_pData[(iY + 1) * GDEM_MAX_DEM_PT_COUNT + iX];
	}

	//
	//	Make the triangle object
	//
	for ( int i = 0; i < 3; i++ )
		gdm_getSpherePoint( triV[i].m_dLongitude, triV[i].m_dLatitude, EARTH_RADIUS + triV[i].m_dDist , &triP[i] );
	tri.set( triP[0], triP[1], triP[2] );

	//
	//	Make the position object and its normal vector
	//
	CGDM_Point3DF pos, normVect, realPos;
	gdm_getSpherePoint( dLongitude, dLatitude, EARTH_RADIUS, &pos );
	normVect = pos; normVect.normalize();
	//
	//	Compute the distance
	//
	tri.getIntersectionOfPlaneWithLine( pos, normVect, realPos );
	return pos.getDistanceFrom( realPos );
#endif
}

void   gdm_get3DWorldHighPoint(double dLongitude, double dLatitude, GDM_POINT3D &pos)
{
	int height = 0;
	GDM_DEM_TILE *pTile = gdm_getDemTile(dLongitude, dLatitude);

	if(pTile && g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
	{
		height = gdm_getMaxHeightInTile(dLongitude, dLatitude, pTile);
	}
	gdm_get3DWorldPoint(dLongitude, dLatitude, pos, height);
}

short gdm_getMaxHeight(double dLongitude, double dLatitude)
{
	GDM_DEM_TILE *pTile = gdm_getDemTile(dLongitude, dLatitude);

	if(pTile == NULL)
		return 0;

	return gdm_getMaxHeightInTile(dLongitude, dLatitude, pTile);
}

double gdm_getExactHeight(double dLongitude, double dLatitude)
{
	GDM_DEM_TILE *pTile = gdm_getDemTile(dLongitude, dLatitude);
	if(pTile == NULL)
		return 0;
	return gdm_getExactHeightInTile(dLongitude, dLatitude, pTile);
}

short gdm_getHeight(double dLongitude, double dLatitude)
{
	GDM_DEM_TILE *pTile = gdm_getDemTile(dLongitude, dLatitude);

	if(pTile == NULL)
		return 0;

	return gdm_getHeightInTile(dLongitude, dLatitude, pTile);
}

GDM_DIRECTION_INFO	gdm_getRateInfo(double dLongitude, double dLatitude)
{
	GDM_DIRECTION_INFO info;
	GDM_DEM_TILE *pTile = NULL;

	memset(&info, 0, sizeof(info));

	if(g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_IMG))
		return info;

	pTile = gdm_getDemTile(dLongitude, dLatitude);
	if(pTile == NULL )
		return info;

	return gdm_getRateInfoInTile(dLongitude, dLatitude, pTile);
}

GDM_DEM_TILE* gdm_getDemTile(double dLongitude, double dLatitude)
{
	const GDM_LOGIC_TILE *pTile = NULL;
	const GDM_LOGIC_TILE_ARR* p_tile_list = gdm_GetRenderTiles();

	int i;

	// find tile in render
	for(i = 0; i < p_tile_list->nCount; i++)
	{
		pTile = &p_tile_list->pData[i];
		if(dLongitude < pTile->pDemTile->minLongitude || dLongitude > pTile->pDemTile->maxLongitude
			|| dLatitude < pTile->pDemTile->minLatitude || dLatitude > pTile->pDemTile->maxLatitude)
		{
			pTile = NULL;
			continue;
		}
		break;
	}

	if(pTile != NULL)
	{
		return pTile->pDemTile;
	}
	else
		return NULL;
}

//////////////////////////////////////////////////////////////////////////
//
//					Testing ray intersection with DEM
//
//////////////////////////////////////////////////////////////////////////

#define GDM_PATCH_SIZE_IN_TILE		2
#define GDM_TILE_PATCH_DIVISION		GDEM_MAX_DEM_DIVISION / GDM_PATCH_SIZE_IN_TILE

#define GDM_XY_TO_INDEX(x, y, stride)	(y) * (stride) + (x) 
#define GDM_PATCH_XY_TO_INDEX(px, py)	GDM_XY_TO_INDEX((px) * (GDM_PATCH_SIZE_IN_TILE), (py) * (GDM_PATCH_SIZE_IN_TILE), GDEM_MAX_DEM_PT_COUNT)  

inline
void getPatchQuadVertIdx(IN int px, IN int py, OUT int pVertIdx[4])
{
	pVertIdx[0] = GDM_PATCH_XY_TO_INDEX( px, py );
	pVertIdx[1] = GDM_PATCH_XY_TO_INDEX( px + 1, py );
	pVertIdx[2] = GDM_PATCH_XY_TO_INDEX( px + 1, py + 1 );
	pVertIdx[3] = GDM_PATCH_XY_TO_INDEX( px, py + 1 );

}

//    Return: -1 = triangle is degenerate (a segment or point)
//             0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
inline
int gdm_intersectRayTriangle(IN CGDM_Point3DF& rayOrg , IN CGDM_Vector3DF& rayDir , 
							 IN const GDM_POINT3D& triV0, IN const GDM_POINT3D& triV1, IN const GDM_POINT3D& triV2,
							 OUT CGDM_Point3DF* pOutPt)
{
	CGDM_Vector3DF    u, v, n;             // triangle vectors
	CGDM_Vector3DF    dir, w0, w;          // ray vectors
	double			  r, a, b;             // params to calc ray-plane intersect
// 	CGDM_Vector3DF v0, v1, v2;
// 
// 	v0.set( triV0.m_X, triV0.m_Y, triV0.m_Z );
// 	v1.set( triV1.m_X, triV1.m_Y, triV1.m_Z );
// 	v2.set( triV2.m_X, triV2.m_Y, triV2.m_Z );
// 
// 	r = v0.getDistanceFromSQ(v1);
// 	if ( r < 1 ) return -1;
// 	r = v1.getDistanceFromSQ(v2);
// 	if ( r < 1 ) return -1;
// 	r = v2.getDistanceFromSQ(v0);
// 	if ( r < 1 ) return -1;

	// get triangle edge vectors and plane normal
	u.set( triV1.m_X - triV0.m_X, triV1.m_Y - triV0.m_Y, triV1.m_Z - triV0.m_Z );
	v.set( triV2.m_X - triV0.m_X, triV2.m_Y - triV0.m_Y, triV2.m_Z - triV0.m_Z );

	n = u.crossProduct(v);					// cross product

	if ( n.equals( CGDM_Vector3DF(0)) )     // triangle is degenerate
		return -1;							// do not deal with this case

	dir = rayDir;							// ray direction vector
	w0.set( rayOrg.m_tX - triV0.m_X, rayOrg.m_tY - triV0.m_Y, rayOrg.m_tZ - triV0.m_Z );
	a = - n.dotProduct(w0);
	b = n.dotProduct(dir);

	if (gdm_equals(b, double(0) )) {     // ray is parallel to triangle plane
		if (a == 0)						// ray lies in triangle plane
			return 2;
		else return 0;					// ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                   // ray goes away from triangle
		return 0;                  // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	*pOutPt = rayOrg + (r * dir);  // intersect point of ray and plane

	// is I inside T?
	double    uu, uv, vv, wu, wv, D;
	uu = u.dotProduct( u );
	uv = u.dotProduct( v );
	vv = v.dotProduct( v );
	w.set( pOutPt->m_tX - triV0.m_X, pOutPt->m_tY - triV0.m_Y, pOutPt->m_tZ - triV0.m_Z );
	wu = w.dotProduct( u );
	wv = w.dotProduct( v );

	D = uv * uv - uu * vv;

	if ( gdm_equals(D, (double) 0) )
		return 0;
	// get and test parametric coords
	double s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                      // I is in T

}

inline void
intersectRayTriangle(IN CGDM_Point3DF& rayOrg , IN CGDM_Vector3DF& rayDir , 
					 IN const GDM_POINT3D& triV0, IN const GDM_POINT3D& triV1, IN const GDM_POINT3D& triV2,
					 OUT CGDM_Point3DF* pOutPt, IN OUT double* minDist, OUT GDM_ERROR_NO* err)
{
	int ret = 0;
	double dist;
	CGDM_Point3DF pt;
	ret = gdm_intersectRayTriangle( rayOrg, rayDir, triV0, triV1, triV2, &pt);
	if ( ret == 1 )
	{
		dist = rayOrg.getDistanceFromSQ( pt );
		if ( *minDist > dist )
		{
			*minDist = dist;
			*pOutPt = pt;
			*err = GDM_SUCCESS;
		}
	}
}


GDM_ERROR_NO gdm_intersectRayDEM(CGDM_Point3DF& rayOrg , CGDM_Vector3DF& rayDir , CGDM_Point3DF* pOutPt)
{
#ifdef DEBUG
	int nTriNum = 0;
	int nTick = gdm_GetTickTimer();
	int nTile = 0;
#endif
	const GDM_LOGIC_TILE_ARR* pTileList = gdm_GetRenderTiles();
	const GDM_LOGIC_TILE *pTile = NULL;
	GDM_DEM_TILE *pDEMTile = NULL;
	GDM_POINT3D* vertBuf = NULL;
	GDM_ERROR_NO err = GDM_FAIL;
	double minDist = 1.7976931348623158e+100;
	int nX, nY, nDelta;
	int base, nDivision;

	int nTileDivision = 0;
	int px, py, i;
	int patchVertIdx[4];

	double d;
	CGDM_Vector3DF	tmp;


	// Find the same dem tile pointers
	for ( i = 0; i < pTileList->nCount; i++ )
	{
		pTile = &pTileList->pData[i];

		tmp = pTile->bndCenter - rayOrg;
		tmp.crossProduct( rayDir );
		d = tmp.getLengthSQ();
		if ( d >= pTile->bndRadius * pTile->bndRadius )
			continue;
#ifdef DEBUG
		nTile ++;
#endif

		pDEMTile = pTile->pDemTile;

		// Get real dem pointer to  this tile
		nDelta = pTile->nLevel - pDEMTile->m_sInfo.nLevel;
		nDivision = GDEM_MAX_DEM_DIVISION / ( 1<<nDelta );		
		nX = pTile->nX - ( pDEMTile->m_sInfo.nX << nDelta );
		nY = pTile->nY - ( pDEMTile->m_sInfo.nY << nDelta );
		base = GDEM_MAX_DEM_PT_COUNT * nDivision * nY + nDivision * nX;
		vertBuf	= g_pGDMDataMgr->m_pTerrainPtCache->GetPts (pDEMTile ) + base;	

		nDivision /= GDM_PATCH_SIZE_IN_TILE ;

		if (nDivision)
		{
			// Divide the tile and make patches and Calculate the intersection
			for ( py = 0; py < nDivision; py++)
				for ( px = 0; px < nDivision; px++)
				{
					// get patch vertices
					getPatchQuadVertIdx( px, py, patchVertIdx );

					// test one triangle of the patch quad
					intersectRayTriangle( rayOrg, rayDir, 
						vertBuf[patchVertIdx[3]],
						vertBuf[patchVertIdx[0]],
						vertBuf[patchVertIdx[1]],
						pOutPt, &minDist, &err );

					// test one triangle of the patch quad
					intersectRayTriangle( rayOrg, rayDir, 
						vertBuf[patchVertIdx[3]],
						vertBuf[patchVertIdx[1]],
						vertBuf[patchVertIdx[2]],
						pOutPt, &minDist, &err );

#ifdef DEBUG
					nTriNum += 2;
#endif				
				}

		}
		else
		{
			// test one triangle of the patch quad
			intersectRayTriangle( rayOrg, rayDir, 
				pTile->edge[3],
				pTile->edge[0],
				pTile->edge[1],
				pOutPt, &minDist, &err );

			// test one triangle of the patch quad
			intersectRayTriangle( rayOrg, rayDir, 
				pTile->edge[3],
				pTile->edge[1],
				pTile->edge[2],
				pOutPt, &minDist, &err );
#ifdef DEBUG
			nTriNum += 2;
#endif
		}

	}
#ifdef DEBUG
	nTick = gdm_GetTickTimer() - nTick;
	qDebug( "HitPoint() : Tri : %d, Tick : %d , Tile :[%d/%d] \n", nTriNum, nTick, nTile, pTileList->nCount );
#endif

	return err;
}



#define EPSILON 1.0e-8
#define ZERO EPSILON
#define SMALL_NUM  0.0000000000000001 // anything that avoids division overflow
#define FLT_MAX         3.402823466e+38F        /* max value */

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).m_tX * (v).m_tX + (u).m_tY * (v).m_tY + (u).m_tZ * (v).m_tZ)
#define cross(u,v,n)  n.m_tX = (u).m_tY * (v).m_tZ - (u).m_tZ * (v).m_tY;\
	n.m_tY = (u).m_tZ * (v).m_tX - (u).m_tX * (v).m_tZ;\
	n.m_tZ = (u).m_tX * (v).m_tY - (u).m_tY * (v).m_tX;




/************************************************************************
intersect_RayTriangle(): intersect a ray with a 3D triangle
Input:  a ray R, and a triangle T
Output: *destPt = intersection point (when it exists)
Return: -1 = triangle is degenerate (a segment or point)
0 = disjoint (no intersect)
1 = intersect in unique point I1
2 = are in the same plane
************************************************************************/
bool isInTriangle(CGDM_Point3DF p, Triangle3DF T)
{
	CGDM_Point3DF    u, v, w;             // triangle vectors

	// get triangle edge vectors and plane normal
	u = T.pointB - T.pointA;
	v = T.pointC - T.pointA;	

	double    uu, uv, vv, wu, wv, D;
	uu = u.dotProduct(u);
	uv = u.dotProduct(v);
	vv = v.dotProduct(v);
	w = p - T.pointA;
	wu = w.dotProduct(u);
	wv = w.dotProduct(v);
	D = 1/(uv * uv - uu * vv);

	// get and test parametric coords
	double s, t;
	s = (uv * wv - vv * wu) * D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) * D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;
	return true;
}

int
intersect_RayTriangle(CGDM_Point3DF rayOrg, CGDM_Point3DF rayDir, Triangle3DF T, CGDM_Point3DF *destPt, double *distance, bool flag)
{
	CGDM_Point3DF    u, v, n;             // triangle vectors
	CGDM_Point3DF    w0, w;          // ray vectors
	double     r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T.pointB - T.pointA;
	v = T.pointC - T.pointA;	
	n = u.crossProduct(v);
	if (n == CGDM_Point3DF(0))            // triangle is degenerate
		return -1;                 // do not deal with this case

	a = -n.dotProduct(rayOrg - T.pointA);
	b = n.dotProduct(rayDir);
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

	*destPt = rayOrg + r * rayDir;           // intersect point of ray and plane
	if (!flag)
	{
		*distance = r;
		return 1;  
	}
	// is I inside T?
	double    uu, uv, vv, wu, wv, D;
	uu = u.dotProduct(u);
	uv = u.dotProduct(v);
	vv = v.dotProduct(v);
	w = *destPt - T.pointA;
	wu = w.dotProduct(u);
	wv = w.dotProduct(v);
	D = 1/(uv * uv - uu * vv);

	// get and test parametric coords
	double s, t;
	s = (uv * wv - vv * wu) * D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) * D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	*distance = r;
	return 1;                      // I is in T
}


/************************************************************************
QuadTree::intersectsBoxWithLine(): intersection possibility test between aabb and a ray
Input:  a ray R, current bbox index 
Output: index priority array idx, success/false
************************************************************************/

bool QuadTree::intersectsBoxWithLine(int index, CGDM_Point3DF rayOrg, CGDM_Point3DF rayDir, int *idx)
{	
	CGDM_Point3DF mid;	
	CGDM_Point3DF closest_pos;
	double diag, radius;
	double d[4];
	int n = 0;
	idx[0] = idx[1] = idx[2] = idx[3] = -1;
	index = (index<<2)+1;

	for (int i = index; i<index+4;i++){	
		if (!m_nIndexArray[i].end)
			continue;
		mid = m_nBboxArray[i].getCenter();
		d[n] = rayDir.dotProduct(mid-rayOrg);		
		diag = (mid - m_nBboxArray[i].MaxEdge).getLengthSQ();
		closest_pos = rayOrg+rayDir*d[n];	
		radius = (mid-closest_pos).getLengthSQ();
		if (radius <= diag)
			idx[n++] = i;			
	}

	int id;
	for (int i = 0;i < n-1;i++){
		for (int j = i+1;j<n;j++){
			if (d[i] > d[j]){
				diag = d[i];
				d[i] = d[j];
				d[j] = diag;

				id = idx[i];
				idx[i] = idx[j];
				idx[j] = id;
			}
		}
	}	
	return (idx[0] >= 0);
}

int QuadTree::getIntersectionAll(CGDM_Point3DF rayOrg, CGDM_Point3DF rayDir, double* lamda)
{			
	Triangle3DF tri;			
	double  dis, lamda2;//, d;
	lamda2 = (*lamda)*(*lamda);

	CGDM_Point3DF outPt;
	for (int i = m_nIndexArray[0].start; i+2 < m_nIndexArray[0].end;i+=3)
	{			
		tri.set(m_pVertexBuf[m_pIndexBuf[i]], m_pVertexBuf[m_pIndexBuf[i+1]], m_pVertexBuf[m_pIndexBuf[i+2]]);
		if (intersect_RayTriangle(rayOrg,rayDir, tri,&outPt, &dis)==1)																		
		{
			*lamda = dis;	
			m_pColTri = tri;
			return 1;					
		}
	}
	return 0;
}


/******************************************************************************
QuadTree::getIntersection(): recursive intersection point test with a given ray
Input:  a ray R, current index array index 
Output: intersection point *lamda(if exist), success/fail
******************************************************************************/

int QuadTree::getIntersection(CGDM_Point3DF rayOrg, CGDM_Point3DF rayDir, double* lamda, int index)
{	
	int chi = index<<2;	
	int flag = 0;	

	if (!(m_nIndexArray[chi+1].end+m_nIndexArray[chi+2].end+m_nIndexArray[chi+3].end+m_nIndexArray[chi+4].end)){
		Triangle3DF tri;			
		double  dis, lamda2;//, d;
		lamda2 = (*lamda)*(*lamda);

		CGDM_Point3DF outPt;
		for (int i = m_nIndexArray[index].start; i+2 < m_nIndexArray[index].end;i+=3)
		{			
			/*	outPt = rayOrg+rayDir*dot(rayDir,(m_pVertexBuf[m_pIndexBuf[i]]-rayOrg));	
			d = (m_pVertexBuf[m_pIndexBuf[i]] - m_pVertexBuf[m_pIndexBuf[i+1]]).getLengthSQ();
			if ((outPt-m_pVertexBuf[m_pIndexBuf[i]]).getLengthSQ() <= d)
			*/
			{
				tri.set(m_pVertexBuf[m_pIndexBuf[i]], m_pVertexBuf[m_pIndexBuf[i+1]], m_pVertexBuf[m_pIndexBuf[i+2]]);
				if (intersect_RayTriangle(rayOrg,rayDir, tri,&outPt, &dis)==1 && dis < (*lamda))																		
				{
					*lamda = dis;	
					m_pColTri = tri;
					return 1;			
				}
			}
		}
		return flag;
	}
	int id[4];
	if (intersectsBoxWithLine(index, rayOrg, rayDir, id))
	{
		for (int i = 0;i < 4 && id[i] >=0; i++){		
			if (getIntersection(rayOrg,  rayDir,lamda, id[i]))	{
				return 1;
			}
		}	
	}	
	return flag;
}
int QuadTree::getHeight(double dLongitude, double dLatitude, double &a_Height)
{
	double cellwidth = tileWidth/(ngridSize-1);	
	int previndex=0,index,tmp;
	Triangle3DF tri;
	double row, col;
	row = (dLatitude-minLatitude)/cellwidth;
	col = (dLongitude-minLongitude)/cellwidth;
		
	int a,b,c,n,i;
	CGDM_Point3DF pa,pb,pc,pt,ps;
	a = (int)(row)*ngridSize;
	b = (int)col;
	ps = (m_pVertexBuf[a+b] + m_pVertexBuf[a+ngridSize+b+1])/2;
	index = 0;
	while (1)
	{
		previndex = index;
		index = (index << 2)+1;
		n = 0;

		for (i = index;i < index+4;i++)
		{
			if (!m_nIndexArray[i].end)
			{
				n = 2;
				break;
			}
			if (m_nBboxArray[i].isPointInside(ps))
			{
				tmp = i;
				n++;
			}
		}
		if (n >= 2)
			break;
		if (!n)
			return 0;
		index = tmp;
	}

	CGDM_Point3DF rayOrg, rayDir,despt;
	pt = CGDM_Point3DF(col,row,0);

	double dRadius = EARTH_RADIUS+20000;
	double dXRadius = dRadius * cos(dLatitude);
	rayOrg.m_tZ = dRadius * sin(dLatitude);
	rayOrg.m_tX = dXRadius * cos(dLongitude);
	rayOrg.m_tY = dXRadius * sin(dLongitude);
	rayDir = -rayOrg;
	rayDir.normalize();

	for (i = m_nIndexArray[previndex].start;i < m_nIndexArray[previndex].end;i+=3)
	{
		a = m_pIndexBuf[i];
		b = m_pIndexBuf[i+1];
		c = m_pIndexBuf[i+2];
		pa.set((a%ngridSize), (a/ngridSize), 0);
		pb.set((b%ngridSize), (b/ngridSize), 0);
		pc.set((c%ngridSize), (c/ngridSize), 0);
		tri.set(pa,pb,pc);
		if (isInTriangle(pt, tri)){
			tri.set(m_pVertexBuf[a], m_pVertexBuf[b], m_pVertexBuf[c]);			
			if (intersect_RayTriangle(rayOrg,rayDir,tri,&despt,&a_Height,0) == 1){
				a_Height = (dRadius-a_Height)-EARTH_RADIUS;
				return 1;
			}
		}
	}
	a_Height = ps.getLength()-EARTH_RADIUS;
	return 1;	
}
/************************************************************************
gdm_intersectRayDEM2(): intersection point test with terrain
Input:  a ray R 
Output: intersection point *cp(if exist), success/fail
************************************************************************/

GDM_ERROR_NO gdm_intersectRayDEM2(CGDM_Point3DF& rayOrg, CGDM_Point3DF& rayDir, CGDM_Point3DF *cp)
{	
	QuadTree *qtree;
	int quicktree = -1;		
	double distance = FLT_MAX;
	bool flag = false;	
	rayDir.normalize();

	CGDM_Point3DF closest_pos = rayOrg+rayDir*(rayDir.dotProduct(-rayOrg));	
	double radius = closest_pos.getLength();

	if (radius > EARTH_RADIUS+10000){
		return GDM_FAIL;	
	}

	double t = (radius < EARTH_RADIUS)? sqrt(EARTH_RADIUS*EARTH_RADIUS - radius*radius): 0;
	CGDM_Point3DF nearpos = closest_pos+(-rayDir)*t;

	if (rayOrg.getLength() > EARTH_RADIUS+10000 && radius < EARTH_RADIUS - 1000)
	{
		*cp = nearpos;
		return GDM_SUCCESS;
	}

	CGDM_Point3DF rayorg(rayOrg.m_tX,rayOrg.m_tY,rayOrg.m_tZ);
	CGDM_Point3DF raydir(rayDir.m_tX,rayDir.m_tY,rayDir.m_tZ);

	if(!gdm_TileListSelector)
		return GDM_FAIL;

	TerrainQuadTreeList *tileQuadList = gdm_TileListSelector->GetBufForRead();
	if (tileQuadList){
		qtree = tileQuadList->quickSortTiles(rayorg,raydir,quicktree);
		if (quicktree >=0)
		{
			if (tileQuadList->quadTrees[quicktree].getIntersection(rayorg, raydir,&distance,0))
			{
				*cp = rayOrg+rayDir*distance;						
				return GDM_SUCCESS;
			}
		}

		for (;qtree;qtree = qtree->pNext){		
			if (qtree->getIntersection(rayorg, raydir,&distance,0)){				
				*cp = rayOrg+rayDir*distance;		
				return GDM_SUCCESS;
			}
		}			
	}
	return GDM_FAIL;
}


double gdm_IntersectHeight(double dLongitude, double dLatitude)
{	
	QuadTree *qtree;
	double a_Height;
	TerrainQuadTreeList *tileQuadList = gdm_TileListSelector->GetBufForRead();
	a_Height = 0;
	if (tileQuadList){
		qtree = tileQuadList->getbyPosition(dLongitude,dLatitude);
		if (qtree){		
			if (qtree->getHeight(dLongitude,dLatitude,a_Height))
				return a_Height;
		}
	}	
	return gdm_getExactHeight(dLongitude, dLatitude);
}
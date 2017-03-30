#include "GIS_TextureIDCache.h"
#include "GDM_CacheMgr.h"
#include "GIS_TextureMgr.h"
#include "GIS_Doc.h"
#include "GIS_Polygon.h"
#include "GIS_GenerateTexture.h"
#include "GDM_GISDataCache.h"
#include "../gdem_gis/GIS_Dataset.h"
#include "../gds_common/GDSCommon.h"

#define MAX_TEXTURE_ID_COUNT	180

CGIS_TextureIDCache*	g_pGisTextureIDCache = NULL;
CGIS_TextureIDCache * gdm_GetGISTextureIDCache(){return g_pGisTextureIDCache;}

CGIS_TextureIDCache::CGIS_TextureIDCache(void)
{
	SetMaxEntryCnt(MAX_TEXTURE_ID_COUNT);
	g_pGisTextureIDCache = this;
}

CGIS_TextureIDCache::~CGIS_TextureIDCache(void)
{
	Clear();
}

long CGIS_TextureIDCache::GetTextureID(int level,int x,int y)
{
	long textureID;
	int key1, key2;

	key1 = MAKE_CACHE_KEY1(level,x,y);
	key2 = MAKE_CACHE_KEY2(level,x,y);

	textureID = (long)GetEntry(key1, key2);

	if(textureID != 0)
		return textureID;

	CGIS_Doc* pGISDoc = gdm_GetGISDoc();
	unsigned int index, nCnt = pGISDoc->GetCount();

	double tileWidth = gdm_TwoPI / (1 << level);

	double u = tileWidth * x - gdm_PI;
	double v = tileWidth * y - gdm_PI;

	QRectF bound(u, v, tileWidth, tileWidth);
	GIS_POLYGON_RENDER_INFO renderInfo;

	GIS_GetTextureGenerator()->RenderBegin(bound);

	for(index = 0; index < nCnt; index++)
	{
		CGIS_Node *pNode = pGISDoc->GetNodeFromIndex(index);
		if(!pNode->IsVisible())
			continue;
		if(pNode->Get_GeoType() != E_GEONODE_TYPE_POLYGON)
			continue;

		//by RSH 2013.8.1
		memset(renderInfo.polyTextureName, 0, PMD_STR_MAX_SIZE);
		memcpy(renderInfo.polyTextureName, ((CGIS_Polygon*)pNode)->m_sTextureName, PMD_STR_MAX_SIZE);
		//end edition

		CGIS_Polygon* pPolygon = (CGIS_Polygon*)pNode;
		if(pPolygon->m_nDrawType != E_GISDRAWPOLY_FILL && pPolygon->m_nDrawType != E_GISDRAWPOLY_ALL)
			continue;
		if(pPolygon->m_nHeightType != E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
			continue;

		renderInfo.fillColor = pPolygon->Get_Brush()->color;
		renderInfo.strokeColor = pPolygon->Get_Pen()->color;
		if(pPolygon->m_nDrawType != E_GISDRAWPOLY_ALL)
			renderInfo.strokeThickness = 0;
		else if(level < 5)
			renderInfo.strokeThickness = pPolygon->Get_Pen()->width / 5 * (level+1);
		else
			renderInfo.strokeThickness = pPolygon->Get_Pen()->width;
		if(!GIS_GetTextureGenerator()->Render(pPolygon->GetPathData(), renderInfo))
		{
			//ASSERT
		}
	}

	QList<E_GDM_SUBDATA_TYPE>		arrTypes;

	arrTypes=g_pGDMDataMgr->GetTypes(E_GDM_MAINDATA_TYPE_NAME);

	GISLayer* layer;

	for(int i=0;i< arrTypes.size();i++)
	{
		if(!g_pGDMDataMgr->IsVisible(arrTypes[i]))
			continue;
		if(g_pGDMDataMgr->IsInvalidLevel(arrTypes[i],level))
			continue;

		layer=g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,arrTypes[i]);

		if(layer==NULL) continue;

		if(layer->GetType()!=GISLayer::GISPoygon)
			continue;

		for(int k=0;k<layer->FeatureCount();k++)
		{
			CGIS_Area* pPolygon=(CGIS_Area*)layer->GetFeature(k);

			if(pPolygon->m_nDrawType != E_GISDRAWPOLY_FILL && pPolygon->m_nDrawType != E_GISDRAWPOLY_ALL)
				continue;
			if(pPolygon->m_nHeightType != E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
				continue;

			renderInfo.fillColor = pPolygon->Get_Brush()->color;
			renderInfo.strokeColor = pPolygon->Get_Pen()->color;
			if(pPolygon->m_nDrawType != E_GISDRAWPOLY_ALL)
				renderInfo.strokeThickness = 0;
			else if(level < 5)
				renderInfo.strokeThickness = pPolygon->Get_Pen()->width / 5 * (level+1);
			else
				renderInfo.strokeThickness = pPolygon->Get_Pen()->width;
			if(!GIS_GetTextureGenerator()->Render(pPolygon->GetPathData(), renderInfo))
			{
				//ASSERT
			}
		}
	}

	QImage *pImage = GIS_GetTextureGenerator()->RenderEnd();
	if(!pImage)
#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
		return gdm_GetGISTextureMgr()->getTransparentTextureID();
#else
		return 0;
#endif

	// generate texture id
	textureID = gdm_GetGISTextureMgr()->AddTexture(x, y, level, (void*)pImage);

	// add data to cach
	AddEntry(key1, key2, (void*)textureID);

	return textureID;
}

long CGIS_TextureIDCache::GetTextureID(GDM_TILE_INFO *pInfo)
{
	long textureID;
	int key1, key2;

	if(pInfo == NULL)
		return 0;

	key1 = MAKE_CACHE_KEY1(pInfo->nLevel, pInfo->nX, pInfo->nY);
	key2 = MAKE_CACHE_KEY2(pInfo->nLevel, pInfo->nX, pInfo->nY);

	textureID = (long)GetEntry(key1, key2);

	if(textureID != 0)
	{
		return textureID;
	}

	CGIS_Doc* pGISDoc = gdm_GetGISDoc();
	unsigned int index, nCnt = pGISDoc->GetCount();

	double tileWidth = gdm_TwoPI / (1 << pInfo->nLevel);
	double u = tileWidth * pInfo->nX - gdm_PI;
	double v = tileWidth * pInfo->nY - gdm_PI;

	QRectF bound(u, v, tileWidth, tileWidth);
	GIS_POLYGON_RENDER_INFO renderInfo;

	GIS_GetTextureGenerator()->RenderBegin(bound);

	for(index = 0; index < nCnt; index++)
	{
		CGIS_Node *pNode = pGISDoc->GetNodeFromIndex(index);
		if(!pNode->IsVisible())
			continue;
		if(pNode->Get_GeoType() != E_GEONODE_TYPE_POLYGON)
			continue;

		//by RSH 2013.8.1
		memset(renderInfo.polyTextureName, 0, PMD_STR_MAX_SIZE);
		memcpy(renderInfo.polyTextureName, ((CGIS_Polygon*)pNode)->m_sTextureName, PMD_STR_MAX_SIZE);
		//end edition

		CGIS_Polygon* pPolygon = (CGIS_Polygon*)pNode;
		if(pPolygon->m_nDrawType != E_GISDRAWPOLY_FILL && pPolygon->m_nDrawType != E_GISDRAWPOLY_ALL)
			continue;
		if(pPolygon->m_nHeightType != E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
			continue;

		renderInfo.fillColor = pPolygon->Get_Brush()->color;
		renderInfo.strokeColor = pPolygon->Get_Pen()->color;
		if(pPolygon->m_nDrawType != E_GISDRAWPOLY_ALL)
			renderInfo.strokeThickness = 0;
		else if(pInfo->nLevel < 5)
			renderInfo.strokeThickness = pPolygon->Get_Pen()->width / 5 * (pInfo->nLevel+1);
		else
			renderInfo.strokeThickness = pPolygon->Get_Pen()->width;
		if(!GIS_GetTextureGenerator()->Render(pPolygon->GetPathData(), renderInfo))
		{
			//ASSERT
		}
	}

	QImage *pImage = GIS_GetTextureGenerator()->RenderEnd();
	if(!pImage)
#ifdef _DEFAULT_TRANSPARENT_TEXTURE_SUPPORT_
		return gdm_GetGISTextureMgr()->getTransparentTextureID();
#else
		return 0;
#endif

	// generate texture id
	textureID = gdm_GetGISTextureMgr()->AddTexture(pInfo->nX, pInfo->nY, pInfo->nLevel, (void*)pImage);

	// add data to cach
	AddEntry(key1, key2, (void*)textureID);

	return textureID;
}

void CGIS_TextureIDCache::FreeData(void *pData)
{
	unsigned long textureID = (unsigned long)pData;
	gdm_GetGISTextureMgr()->DelTexture(textureID);
}

void CGIS_TextureIDCache::AddTileEntry(GDM_TILE_INFO Info, void *buf)
{
	int key1, key2;

	key1 = MAKE_CACHE_KEY1(Info.nLevel, Info.nX, Info.nY);
	key2 = MAKE_CACHE_KEY2(Info.nLevel, Info.nX, Info.nY);

	void* tmp = GetEntry(key1, key2);
if(tmp!=NULL)
	return;
	
	AddEntry(key1, key2, buf);
}

void CGIS_TextureIDCache::AddTileEntry(int level,int x,int y, void *buf)
{
	int key1, key2;

	key1 = MAKE_CACHE_KEY1(level, x,y);
	key2 = MAKE_CACHE_KEY2(level, x,y);

	void* tmp = GetEntry(key1, key2);
	if(tmp!=NULL)
		return;

	AddEntry(key1, key2, buf);
}
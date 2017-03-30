#include "GDM_Tile.h"
#include "GDM_Define.h"
#include "GDM_Math.h"
#include "GDM_DataMgr.h"
#include "GDM_TerrainPtCache.h"
#include "IGDM_TextureIDCache.h"
#include "GDM_ContourCache.h"
#include "GDM_3DObjectCache.h"
#include "GDM_SimpleBuildingCache.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSGeoNameDBManager.h"
#include "GDM_GISDataCache.h"
#include "../gdem_scene/RasterTextureIDCache.h"
#include <math.h>

void gdm_PreProcessDemTile(GDM_DEM_TILE *pTile);
void gdm_PreProcessImgTile(GDM_TEXTURE_TILE *pTile);
void gdm_PreProcess3dMeshTile(GDM_TILE *pTile);
void gdm_PreProcessNameTile(GDM_TILE *pTile);
void gdm_PreProcessRasterTile(GDM_TILE *pTile);

void gdm_PreProcessTile(GDM_TILE *pTile)
{
	switch(pTile->m_sInfo.eSubType)
	{
	case E_GDM_SUBDATA_TYPE_DEM:
		gdm_PreProcessDemTile((GDM_DEM_TILE*)pTile);
		break;
	case E_GDM_SUBDATA_TYPE_IMG:
	case E_GDM_SUBDATA_TYPE_IMG_NASA:
		gdm_PreProcessImgTile((GDM_TEXTURE_TILE*)pTile);
		break;
	case E_GDM_SUBDATA_TYPE_3D_BUILDING:
	case E_GDM_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING:
		gdm_PreProcess3dMeshTile(pTile);
		break;
	}
	if(pTile->m_sInfo.eMainType==E_GDM_MAINDATA_TYPE_NAME)
		gdm_PreProcessNameTile(pTile);
	if(pTile->m_sInfo.eMainType==E_GDM_MAINDATA_TYPE_RASTER)
		gdm_PreProcessRasterTile((GDM_TILE*)pTile);
}

#define GDM_CONTOUR_MIN_LEVEL_100	9

void gdm_PreProcessDemTile(GDM_DEM_TILE *pTile)
{
	// pre calculate in loading process

	pTile->tileWidth	= gdm_TwoPI / (1 << pTile->m_sInfo.nLevel);
	pTile->gridWidth	= pTile->tileWidth / GDEM_MAX_DEM_DIVISION;
	pTile->minLongitude = pTile->tileWidth * pTile->m_sInfo.nX - gdm_PI;
	pTile->minLatitude	= pTile->tileWidth * pTile->m_sInfo.nY - gdm_PI;
	pTile->maxLongitude = pTile->minLongitude + pTile->tileWidth;
	pTile->maxLatitude	= pTile->minLatitude + pTile->tileWidth;

	g_pGDMDataMgr->m_pTerrainPtCache->GetPts(pTile);

	if(gdm_GetDataMgr()->m_sOption.blContour && pTile->m_sInfo.nLevel >= GDM_CONTOUR_MIN_LEVEL_100)
	{
		int nInterval = gdm_GetDataMgr()->m_sOption.contourOption.contourInterval;

		gdm_GetDataMgr()->m_pContour->GetContourData(pTile->m_sInfo.nX, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel, 
			pTile->m_pData, nInterval);
	}
}  

void gdm_PreProcess3dMeshTile(GDM_TILE *pTile)
{
	if(gdm_GetDataMgr()->IsVisible(pTile->m_sInfo.eSubType))
	{
		g_3dCacheMutex.lock();

		if(pTile->m_sInfo.eSubType==E_GDM_SUBDATA_TYPE_3D_BUILDING)
			gdm_GetDataMgr()->m_p3DObjectCache->GetMeshs(pTile->m_sInfo.nX, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel, 
				pTile->m_pBuffer);
		if(pTile->m_sInfo.eSubType==E_GDM_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING)
			g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray((GDM_TILE*)pTile);

		g_3dCacheMutex.unlock();
	}
}

void gdm_PreProcessImgTile(GDM_TEXTURE_TILE *pTile)
{
	g_TextureIDCacheMutex.lock();

	if(g_pGDMDataMgr->m_pTextureIDCache)
		g_pGDMDataMgr->m_pTextureIDCache->GetTextureID(pTile);

	g_TextureIDCacheMutex.unlock();
}

#include "../gdem_gis/GIS_Dataset.h "
void gdm_PreProcessNameTile(GDM_TILE *pTile)
{
	ST_GEODATA_NAMELIST* namelist=(ST_GEODATA_NAMELIST*)pTile->m_pBuffer;

	unsigned char* pBuff = (unsigned char*)namelist->data_list;

	PolygonLayer* polylayer =new PolygonLayer();
	PathLayer* pathlayer=new PathLayer();

	PathLayer* linklayer=new PathLayer();

	ST_DBTABLEINFO* tabledef;

	CGIS_Polygon* feature;

	CGIS_Polygon* link;

	for(unsigned int i= 0; i < namelist->count; i++)
	{
		ST_GEODATA_NAME* pRecord = (ST_GEODATA_NAME*)pBuff;

		pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;

		tabledef=g_DBManager->GetTableDef(pRecord->type);
		Q_ASSERT(tabledef!=NULL);

		if(tabledef->shape_type==GISPath)
		{
			//creat CGIS_Path using pRecord.id
			feature=g_DBManager->CreateFeature(pRecord->type,pRecord->id,link);
			feature->SetKey(pRecord->id);
			pathlayer->AddFeature(feature);
		}
		else if(tabledef->shape_type==GISPolygon)
		{
			//creat CGIS_Path using pRecord.id
			feature=g_DBManager->CreateFeature(pRecord->type,pRecord->id,link);
			feature->SetKey(pRecord->id);
			polylayer->AddFeature(feature);

			if(link)
				linklayer->AddFeature(link);
		}
	}

	if(polylayer->FeatureCount()==0)
		delete polylayer;
	else
	{
		GISLayer* l= g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,pTile->m_sInfo.eSubType);
		
		if(l==NULL)
			g_pGDMDataMgr->m_pGISDataCache->AddLayer(0,0,0,pTile->m_sInfo.eSubType,polylayer);
		else
		{
			for(int j=0;j<polylayer->FeatureCount();j++)
				if(!l->ExistFeature(polylayer->GetFeature(j)->GetKey()))
					l->AddFeature(polylayer->GetFeature(j)->Clone());

			delete polylayer;
		}
	}

	if(linklayer->FeatureCount()==0)
		delete linklayer;
	else
	{
		GISLayer* l= g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,E_GDM_SUBDATA_TYPE_NAME_ACCIDENT);

		if(l==NULL)
			g_pGDMDataMgr->m_pGISDataCache->AddLayer(0,0,0,E_GDM_SUBDATA_TYPE_NAME_ACCIDENT,linklayer);
		else
		{
			for(int j=0;j<linklayer->FeatureCount();j++)
				if(!l->ExistFeature(linklayer->GetFeature(j)->GetKey()))
					l->AddFeature(linklayer->GetFeature(j)->Clone());

			delete linklayer;
		}
	}
	if(pathlayer->FeatureCount()==0)
		delete pathlayer;
	else
	{
		GISLayer* l= g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,pTile->m_sInfo.eSubType);

		if(l==NULL)
			g_pGDMDataMgr->m_pGISDataCache->AddLayer(0,0,0,pTile->m_sInfo.eSubType,pathlayer);
		else
		{
			for(int j=0;j<pathlayer->FeatureCount();j++)
				if(!l->ExistFeature(pathlayer->GetFeature(j)->GetKey()))
					l->AddFeature(pathlayer->GetFeature(j)->Clone());

			delete pathlayer;
		}
	}
		
}

void gdm_PreProcessRasterTile(GDM_TILE *pTile)
{
	gdm_GetRasterTextureIDCache()->GetTextureID(pTile);
}

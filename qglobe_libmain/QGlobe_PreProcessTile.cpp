#include "QGlobe_Tile.h"
#include "QGlobe_Define.h"
#include "QGlobe_Math.h"
#include "QGlobe_DataMgr.h"
#include "QGlobe_TerrainPtCache.h"
#include "IQGlobe_TextureIDCache.h"
#include "QGlobe_ContourCache.h"
#include "QGlobe_3DObjectCache.h"
#include "QGlobe_SimpleBuildingCache.h"
#include "../qglobeds_database/QGlobeDSDatabaseInterface.h"
#include "../qglobeds_database/QGlobeDSGeoNameDBManager.h"
#include "QGlobe_GISDataCache.h"
#include "../qglobe_scene/RasterTextureIDCache.h"
#include <math.h>

void qglobe_PreProcessDemTile(QGlobe_DEM_TILE *pTile);
void qglobe_PreProcessImgTile(QGlobe_TEXTURE_TILE *pTile);
void qglobe_PreProcess3dMeshTile(QGlobe_TILE *pTile);
void qglobe_PreProcessNameTile(QGlobe_TILE *pTile);
void qglobe_PreProcessRasterTile(QGlobe_TILE *pTile);

void qglobe_PreProcessTile(QGlobe_TILE *pTile)
{
	switch(pTile->m_sInfo.eSubType)
	{
	case E_QGlobe_SUBDATA_TYPE_DEM:
		qglobe_PreProcessDemTile((QGlobe_DEM_TILE*)pTile);
		break;
	case E_QGlobe_SUBDATA_TYPE_IMG:
	case E_QGlobe_SUBDATA_TYPE_IMG_NASA:
		qglobe_PreProcessImgTile((QGlobe_TEXTURE_TILE*)pTile);
		break;
	case E_QGlobe_SUBDATA_TYPE_3D_BUILDING:
	case E_QGlobe_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING:
		qglobe_PreProcess3dMeshTile(pTile);
		break;
	}
	if(pTile->m_sInfo.eMainType==E_QGlobe_MAINDATA_TYPE_NAME)
		qglobe_PreProcessNameTile(pTile);
	if(pTile->m_sInfo.eMainType==E_QGlobe_MAINDATA_TYPE_RASTER)
		qglobe_PreProcessRasterTile((QGlobe_TILE*)pTile);
}

#define QGlobe_CONTOUR_MIN_LEVEL_100	9

void qglobe_PreProcessDemTile(QGlobe_DEM_TILE *pTile)
{
	// pre calculate in loading process

	pTile->tileWidth	= qglobe_TwoPI / (1 << pTile->m_sInfo.nLevel);
	pTile->gridWidth	= pTile->tileWidth / QGLOBE_MAX_DEM_DIVISION;
	pTile->minLongitude = pTile->tileWidth * pTile->m_sInfo.nX - qglobe_PI;
	pTile->minLatitude	= pTile->tileWidth * pTile->m_sInfo.nY - qglobe_PI;
	pTile->maxLongitude = pTile->minLongitude + pTile->tileWidth;
	pTile->maxLatitude	= pTile->minLatitude + pTile->tileWidth;

	g_pGDMDataMgr->m_pTerrainPtCache->GetPts(pTile);

	if(qglobe_GetDataMgr()->m_sOption.blContour && pTile->m_sInfo.nLevel >= QGlobe_CONTOUR_MIN_LEVEL_100)
	{
		int nInterval = qglobe_GetDataMgr()->m_sOption.contourOption.contourInterval;

		qglobe_GetDataMgr()->m_pContour->GetContourData(pTile->m_sInfo.nX, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel, 
			pTile->m_pData, nInterval);
	}
}  

void qglobe_PreProcess3dMeshTile(QGlobe_TILE *pTile)
{
	if(qglobe_GetDataMgr()->IsVisible(pTile->m_sInfo.eSubType))
	{
		g_3dCacheMutex.lock();

		if(pTile->m_sInfo.eSubType==E_QGlobe_SUBDATA_TYPE_3D_BUILDING)
			qglobe_GetDataMgr()->m_p3DObjectCache->GetMeshs(pTile->m_sInfo.nX, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel, 
				pTile->m_pBuffer);
		if(pTile->m_sInfo.eSubType==E_QGlobe_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING)
			g_pGDMDataMgr->m_pSimpleBuildingCache->GetBuildingArray((QGlobe_TILE*)pTile);

		g_3dCacheMutex.unlock();
	}
}

void qglobe_PreProcessImgTile(QGlobe_TEXTURE_TILE *pTile)
{
	g_TextureIDCacheMutex.lock();

	if(g_pGDMDataMgr->m_pTextureIDCache)
		g_pGDMDataMgr->m_pTextureIDCache->GetTextureID(pTile);

	g_TextureIDCacheMutex.unlock();
}

#include "../qglobe_gis/GIS_Dataset.h "
void qglobe_PreProcessNameTile(QGlobe_TILE *pTile)
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
		GISLayer* l= g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,E_QGlobe_SUBDATA_TYPE_NAME_ACCIDENT);

		if(l==NULL)
			g_pGDMDataMgr->m_pGISDataCache->AddLayer(0,0,0,E_QGlobe_SUBDATA_TYPE_NAME_ACCIDENT,linklayer);
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

void qglobe_PreProcessRasterTile(QGlobe_TILE *pTile)
{
	qglobe_GetRasterTextureIDCache()->GetTextureID(pTile);
}

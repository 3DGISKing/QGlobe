#include "GDM_DataMgr.h"
#include "GDM_ThreadMgr.h"
#include "GDM_Define.h"
#include "GDM_Camera.h"
#include "GDM_PyramidMgr.h"
#include "GDM_CacheMgr.h"
#include "GDM_RequestMgr.h"
#include "GDM_CollisionHandler.h"
#include "GDM_LoadDataMgr.h"
#include "GDM_Interface.h"
#include "GDM_ThreadMgr.h"
#include "GDM_FileCacheMgr.h"
#include "GDM_QuadNode.h"
#include "GDM_ContourCache.h"
#include "GDM_FPSTester.h"
#include "GDM_TerrainPtCache.h"
#include "GDM_TextMgr.h"
#include "datachannel.h"
#include "GDM_3DObjectCache.h"
#include "GDM_SimpleBuildingCache.h"
#include "GDM_GISDataCache.h"
#include "Layer.h"
#include "IGDM_TextureIDCache.h"
#include "GDM_GISDataCache.h"
#include "GIS_Dataset.h"
#include "../gdem_scene/RasterTextureIDCache.h"
#include "ColorRamp.h"
#include "../gdem_client/gdemclient.h"
#include "rendersrv.h"
#include "../gdem_scene/GDM_SceneManager.h"
#include "../gdem_scene/GDM_TextureMgr.h"

CGDM_DataMgr* g_pGDMDataMgr = NULL;

#define GDEM_MAX_CACHE_DEM_ENTRY_CNT		200

QString			g_WorkPath;

void gdm_SetWorkPath(IN QString &path)
{
	g_WorkPath = path;
}

QString	gdm_GetWorkPath()
{
	return g_WorkPath;
}

const E_GDM_MAINDATA_TYPE gdm_GetMainType(E_GDM_SUBDATA_TYPE subType)
{
	Layer* layer=g_pGDMDataMgr->m_LayerHash[subType];

	Q_ASSERT(layer!=NULL);

	return layer->GetMainType();
}

CGDM_DataMgr::CGDM_DataMgr()
{
	m_pCamera			= NULL;
	m_pPyramidMgr		= NULL;
	m_pCacheMgr			= NULL;
	m_pRequestMgr		= NULL;
	m_pCollisionHandler = NULL;
	m_pLoadDataMgr		= NULL;	
	m_pFileCacheMgr		= NULL;
	m_pNodeMgr			= NULL;
	m_pContour			= NULL;

	g_pGDMDataMgr		= this;
	m_pCamera			= new CGDM_Camera();
	m_pFileCacheMgr		= new CGDM_FileCacheMgr();
	m_pPyramidMgr		= new CGDM_PyramidMgr();
	m_pCacheMgr			= new CGDM_CacheMgr();
	m_pRequestMgr		= new CGDM_RequestMgr();
	m_pCollisionHandler = new CGDM_CollisionHandler();
	m_pLoadDataMgr		= new CGDM_LoadDataMgr();		
	m_pNodeMgr			= new CGDM_Node_Mgr();
	m_pContour			= new CGDM_ContourCache();
	m_pTerrainPtCache	= new CGDM_TerrainPtCache();
	m_pTerrainDirCache  = new CGDM_TerrainDirCache();
	m_p3DObjectCache    = new  CGDM_3DObjectCache();
	m_pTerrainDirCache->SetPtCache(m_pTerrainPtCache);
	m_pSimpleBuildingCache=new CGDM_SimpleBuildingCache();

	m_pGISDataCache     = new CGDM_GISDataCache();

	m_pTextMgr			= new CGDM_TextMgr();
	m_pDemCacheForCalcu	= new CGDM_CacheDataMgr();

	// create communication channel and cache for calulation
	m_pLocalChannel = new DataChannel();
	m_pLocalChannel->SetMaxWaitCount(0);
	m_pLocalDemCache = new CGDM_CacheDataMgr(GDEM_MAX_CACHE_DEM_ENTRY_CNT);

	// set default option
	memset(&m_sOption, 0 , sizeof(m_sOption));

	// default grid setting
	m_sOption.gridViewInfo.gridView = 1; // use grid view
	m_sOption.gridViewInfo.gridMode= 0; 

	// set default contour option
	m_sOption.contourOption.contourInterval = 500;
	m_sOption.contourOption.ravineColor = 0xB4AA96;
	m_sOption.contourOption.mainCurveColor = 0xBEBEBE;
	m_sOption.contourOption.ravineThick = 2;
	m_sOption.contourOption.mainCurveThick = 1;

	m_blStopDataLoad = false;

	ReadGeoShapeColorWidth();

	m_pActiveTerrainLayer=NULL;
	m_pActiveImageLayer=NULL;
	m_pActiveRasterLayer=NULL;

	m_ColorRampList.push_back(new AlgorithmicColorRamp(Qt::white,Qt::black,"White to Black"));
	m_ColorRampList.push_back(new AlgorithmicColorRamp(Qt::black,Qt::white,"Black to White"));
	m_ColorRampList.push_back(new AlgorithmicColorRamp(Qt::yellow,Qt::darkRed,"Yellow to DarkRed"));
	m_ColorRampList.push_back(new AlgorithmicColorRamp(Qt::green,Qt::blue,"Green to Blue"));
	m_ColorRampList.push_back(new AlgorithmicColorRamp(Qt::yellow,Qt::green,"Yellow to Green"));
}


QList<E_GDM_SUBDATA_TYPE> CGDM_DataMgr::GetTypes(E_GDM_MAINDATA_TYPE mainType)
{
	QList<E_GDM_SUBDATA_TYPE> arrType;
	QList<Layer*> layerlist=m_LayerHash.values();
	Layer* layer;

	foreach(layer,layerlist)
	{
		if(layer->GetMainType()==mainType)
			arrType.push_back(layer->GetSubType());
	}
	return arrType;
}


void CGDM_DataMgr::ReadGeoShapeColorWidth()
{
	QString m_shapeAttrFName = gdm_GetWorkPath() + "/ShapeAttribute.xml";

	QFile file(m_shapeAttrFName);
	QXmlSimpleReader reader;

	ColorXmlParser handler;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);
	reader.parse(&file);
	handler.SetAttributeList(m_sOption.m_geoShapeAttr);
}

CGDM_DataMgr::~CGDM_DataMgr()
{
	gdm_KillAllThread();

	//delete all layer

	QList<Layer*> layerlist=m_LayerHash.values();

	Layer* layer;

	foreach(layer,layerlist)
	{
		if(layer)
			delete layer;
	}

	layerlist.clear();

	if(m_pCamera)
		delete m_pCamera;
	if(m_pFileCacheMgr)
		delete m_pFileCacheMgr;
	if(m_pPyramidMgr)
		delete m_pPyramidMgr;
	if(m_pCacheMgr)
		delete m_pCacheMgr;
	if(m_pRequestMgr)
		delete m_pRequestMgr;
	if(m_pCollisionHandler)
		delete m_pCollisionHandler;
	if(m_pLoadDataMgr)
		delete m_pLoadDataMgr;
	if(m_pNodeMgr)
		delete m_pNodeMgr;
	if(m_pContour)
		delete m_pContour;
	if(m_pTerrainPtCache)
		delete m_pTerrainPtCache;
	if(m_pTerrainDirCache)
		delete m_pTerrainDirCache;
	if(m_pTextMgr)
		delete m_pTextMgr;
	if(m_pDemCacheForCalcu)
		delete m_pDemCacheForCalcu;
	if(m_pLocalChannel)
		delete m_pLocalChannel;
	if(m_pLocalDemCache)
		delete m_pLocalDemCache;
	if(m_p3DObjectCache)
		delete m_p3DObjectCache;
	if(m_pSimpleBuildingCache)
		delete m_pSimpleBuildingCache;
	if(m_pGISDataCache)
		delete m_pGISDataCache;

	for(int i=0;i<m_ColorRampList.size();i++)
		delete m_ColorRampList[i];

	m_ColorRampList.clear();
}

void CGDM_DataMgr::SetModifyFlag(bool modify)
{
	m_blModify = modify;
}

void CGDM_DataMgr::OnBeforeRender()
{
	// prepare rendering data
	if(IsModified())
		gdm_MakePrepareRenderData();

	gdm_GetDataMgr()->m_pTextMgr->Init();
}

GDM_DEM_TILE* CGDM_DataMgr::GetDemTileForCalcu(int m, int n, int level)
{
	GDM_DEM_TILE *pTile = NULL;
	short buffer[GDEM_MAX_DEM_PT_COUNT * GDEM_MAX_DEM_PT_COUNT + 50];
	GDM_TILE_INFO info;

	memset(&info, 0, sizeof(info));
	info.eSubType	= E_GDM_SUBDATA_TYPE_DEM;
	info.eMainType	= gdm_GetMainType(info.eSubType);
	info.nLevel	= level;
	info.nX		= m;
	info.nY		= n;
	info.nSize	= GDEM_MAX_DEM_PT_COUNT * GDEM_MAX_DEM_PT_COUNT + 50;

	if(m_pLocalChannel == NULL)
		return NULL;

	int key1 = MAKE_CACHE_KEY1(info.nLevel, info.nX, info.nY);
	int key2 = MAKE_CACHE_KEY2(info.nLevel, info.nX, info.nY);
	
	// check cache
	if(m_pLocalDemCache)
	{
		pTile = (GDM_DEM_TILE*) m_pLocalDemCache->GetEntry(key1, key2);

		if(pTile != NULL)
			return pTile;
	}

	// load data
	pTile = (GDM_DEM_TILE*)m_pLoadDataMgr->LoadOneTile(&info, buffer, m_pLocalChannel);
	if(pTile == NULL)
		return pTile;

	pTile->tileWidth	= gdm_TwoPI / (1 << pTile->m_sInfo.nLevel);
	pTile->gridWidth	= pTile->tileWidth / GDEM_MAX_DEM_DIVISION;
	pTile->minLongitude = pTile->tileWidth * pTile->m_sInfo.nX - gdm_PI;
	pTile->minLatitude	= pTile->tileWidth * pTile->m_sInfo.nY - gdm_PI;
	pTile->maxLongitude = pTile->minLongitude + pTile->tileWidth;
	pTile->maxLatitude	= pTile->minLatitude + pTile->tileWidth;

	// update chache
	m_pLocalDemCache->AddEntry(key1, key2, pTile);

	return pTile;
}

bool CGDM_DataMgr::IsNecessary(E_GDM_SUBDATA_TYPE type)
{
	switch(type)
	{
	case E_GDM_SUBDATA_TYPE_DEM:
		return (g_pGDMDataMgr->m_sOption.blTerrain) || 
			(g_pGDMDataMgr->m_sOption.layerTextureMode != LTM_TEXTURE) ||
			(g_pGDMDataMgr->m_sOption.blContour);
	default:
		return IsVisible(type);
	}
}

bool CGDM_DataMgr::IsVisible(E_GDM_SUBDATA_TYPE type)
{
	Layer* layer=m_LayerHash[type];

	//Q_ASSERT(layer!=NULL);

	if(layer)
		return layer->GetVisible();
	return false;
}

unsigned int CGDM_DataMgr::GetMinLevel(E_GDM_SUBDATA_TYPE type)
{
	Layer* layer=m_LayerHash[type];

	//Q_ASSERT(layer!=NULL);

	if(layer)
		return layer->GetMinLevel();
	return 0;
}

void CGDM_DataMgr::OnChangeServer()
{
	m_pNodeMgr->ClearAll();
}

CGDM_DataMgr* gdm_GetDataMgr() 
{
	return g_pGDMDataMgr;
}

void CGDM_DataMgr::AddLayer(Layer* layer)
{
	Q_ASSERT(m_LayerHash.value(layer->GetSubType())==NULL);
	m_LayerHash.insert(layer->GetSubType(),layer);
}

void CGDM_DataMgr::SetLayer(Layer* layer)
{
	if(layer->IsTerrainLayer())
	{
		if(layer->GetVisible())
		{
			m_pActiveTerrainLayer=(TerrainLayer*)layer;
			m_sOption.blTerrain=true;
			m_pTerrainPtCache->SetEnableHeight(true);
		}
		else
		{
			if(m_pActiveTerrainLayer==layer)
			{
				m_sOption.blTerrain=false;
				m_pTerrainPtCache->SetEnableHeight(false);
			}
		}
	}

	if(layer->IsImageLayer())
	{
		if(layer->GetVisible())
		{
			if(m_pActiveImageLayer!=layer)
			{
				m_pActiveImageLayer=(ImageLayer*)layer;
				m_pCacheMgr->ImageCacheClear();

				g_TextureIDCacheMutex.lock();
				m_pTextureIDCache->Clear();
				g_TextureIDCacheMutex.unlock();

				g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pTextureMgr->DelTextureList();

				SetModifyFlag();

				m_pRequestMgr->UpdateRequest();
			}	
		}
	}

	if(layer->IsRasterLayer())
	{
		if(layer->GetVisible())
		{
			if(m_pActiveRasterLayer!=layer)
			{
				m_pActiveRasterLayer=(RasterLayer*)layer;
				m_pCacheMgr->RasterCacheClear();
				gdm_GetRasterTextureIDCache()->Clear();
			}	
		}
	}
}

CGIS_Node* gdm_GetGISNode(E_GDM_SUBDATA_TYPE type,int id)
{
	GISLayer* layer=g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,type);

	if(!layer)
		return NULL;

	for(int i=0;i<layer->FeatureCount();i++)
	{
		CGIS_Node* node=layer->GetFeature(i);
		if(node->GetKey()==id)
			return node;
	}

	return NULL;
}

bool CGDM_DataMgr::IsNecessaryDirInfo()
{
	if (m_sOption.layerTextureMode==LTM_RATEPLAN)
		return true;
	if (m_sOption.layerTextureMode==LTM_RATEANGLE)
		return true;	
	return false;
}

unsigned int CGDM_DataMgr::GetMaxTileSize(E_GDM_MAINDATA_TYPE type)
{
	switch(type)
	{
		case E_GDM_MAINDATA_TYPE_IMG:
			return RasterLayer::MaxTileSize;
		case E_GDM_MAINDATA_TYPE_DEM:
			return TerrainLayer::MaxTileSize;
		case E_GDM_MAINDATA_TYPE_NAME:
			return	PlaceNameLayer::MaxTileSize;
		case E_GDM_MAINDATA_TYPE_SHAPE:
			return PolylineLayer::MaxTileSize;
		case E_GDM_MAINDATA_TYPE_3D:
			return ObjectLayer::MaxTileSize;
		case E_GDM_MAINDATA_TYPE_RASTER:
			return RasterLayer::MaxTileSize;
		default:
			qFatal("Error in GetMaxTileSize: Unknown Main Data Type ");
	}
	return 0;
}

bool CGDM_DataMgr::IsInvalidLevel(IN E_GDM_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel)
{
	if (a_nLevel < MIN_LEVEL || a_nLevel > MAX_LEVEL)
		return false;

	Layer* layer=m_LayerHash[a_eDataType];

	Q_ASSERT(layer!=NULL);

	return (a_nLevel < layer->GetMinLevel()) || (a_nLevel > layer->GetMaxLevel());
}

void CGDM_DataMgr::Clear3DCache()
{
	g_CacheMgrMutex.lock();
	gdm_GetDataMgr()->m_pCacheMgr->GetCacheMgr(E_GDM_MAINDATA_TYPE_3D)->Clear();
	g_CacheMgrMutex.unlock();

	g_3dCacheMutex.lock();
	gdm_GetDataMgr()->m_p3DObjectCache->Clear();
	g_3dCacheMutex.unlock();

	g_pGDMDataMgr->SetModifyFlag();
}

void CGDM_DataMgr::Delete3DData(int nX,int nY,int nLevel)
{
	g_CacheMgrMutex.lock();
	CGDM_CacheDataMgr* mgr=gdm_GetDataMgr()->m_pCacheMgr->GetCacheMgr(E_GDM_MAINDATA_TYPE_3D);

	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(E_GDM_SUBDATA_TYPE_3D_BUILDING, nX, nY);

	mgr->DeleteEntry(mgr->GetCacheEntry(key1,key2));
	g_CacheMgrMutex.unlock();

	g_3dCacheMutex.lock();
	gdm_GetDataMgr()->m_p3DObjectCache->DeleteMeshEntry(nX,nY,nLevel);
	g_3dCacheMutex.unlock();

	g_pGDMDataMgr->SetModifyFlag();
}

double CGDM_DataMgr::CameraHeight()
{
	return m_pCamera->m_location.m_dDist;
}

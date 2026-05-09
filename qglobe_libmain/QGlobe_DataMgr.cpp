#include "QGlobe_DataMgr.h"
#include "QGlobe_ThreadMgr.h"
#include "QGlobe_Define.h"
#include "QGlobe_Camera.h"
#include "QGlobe_PyramidMgr.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_RequestMgr.h"
#include "QGlobe_CollisionHandler.h"
#include "QGlobe_LoadDataMgr.h"
#include "QGlobe_Interface.h"
#include "QGlobe_ThreadMgr.h"
#include "QGlobe_FileCacheMgr.h"
#include "QGlobe_QuadNode.h"
#include "QGlobe_ContourCache.h"
#include "QGlobe_FPSTester.h"
#include "QGlobe_TerrainPtCache.h"
#include "QGlobe_TextMgr.h"
#include "datachannel.h"
#include "QGlobe_3DObjectCache.h"
#include "QGlobe_SimpleBuildingCache.h"
#include "QGlobe_GISDataCache.h"
#include "Layer.h"
#include "IQGlobe_TextureIDCache.h"
#include "QGlobe_GISDataCache.h"
#include "GIS_Dataset.h"
#include "../qglobe_scene/RasterTextureIDCache.h"
#include "ColorRamp.h"
#include "../qglobe_client/qglobeclient.h"
#include "rendersrv.h"
#include "../qglobe_scene/QGlobe_SceneManager.h"
#include "../qglobe_scene/QGlobe_TextureMgr.h"
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

CQGlobe_DataMgr* g_pQGlobeDataMgr = NULL;

#define QGLOBE_MAX_CACHE_DEM_ENTRY_CNT		200

QString			g_WorkPath;

void qglobe_SetWorkPath(IN QString &path)
{
	g_WorkPath = path;
}

QString	qglobe_GetWorkPath()
{
	return g_WorkPath;
}

const E_QGlobe_MAINDATA_TYPE qglobe_GetMainType(E_QGlobe_SUBDATA_TYPE subType)
{
	Layer* layer=g_pQGlobeDataMgr->m_LayerHash[subType];

	Q_ASSERT(layer!=NULL);

	return layer->GetMainType();
}

CQGlobe_DataMgr::CQGlobe_DataMgr()
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

	g_pQGlobeDataMgr		= this;
	m_pCamera			= new CQGlobe_Camera();
	m_pFileCacheMgr		= new CQGlobe_FileCacheMgr();
	m_pPyramidMgr		= new CQGlobe_PyramidMgr();
	m_pCacheMgr			= new CQGlobe_CacheMgr();
	m_pRequestMgr		= new CQGlobe_RequestMgr();
	m_pCollisionHandler = new CQGlobe_CollisionHandler();
	m_pLoadDataMgr		= new CQGlobe_LoadDataMgr();		
	m_pNodeMgr			= new CQGlobe_Node_Mgr();
	m_pContour			= new CQGlobe_ContourCache();
	m_pTerrainPtCache	= new CQGlobe_TerrainPtCache();
	m_pTerrainDirCache  = new CQGlobe_TerrainDirCache();
	m_p3DObjectCache    = new  CQGlobe_3DObjectCache();
	m_pTerrainDirCache->SetPtCache(m_pTerrainPtCache);
	m_pSimpleBuildingCache=new CQGlobe_SimpleBuildingCache();

	m_pGISDataCache     = new CQGlobe_GISDataCache();

	m_pTextMgr			= new CQGlobe_TextMgr();
	m_pDemCacheForCalcu	= new CQGlobe_CacheDataMgr();

	// create communication channel and cache for calulation
	m_pLocalChannel = new DataChannel();
	m_pLocalChannel->SetMaxWaitCount(0);
	m_pLocalDemCache = new CQGlobe_CacheDataMgr(QGLOBE_MAX_CACHE_DEM_ENTRY_CNT);

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


QList<E_QGlobe_SUBDATA_TYPE> CQGlobe_DataMgr::GetTypes(E_QGlobe_MAINDATA_TYPE mainType)
{
	QList<E_QGlobe_SUBDATA_TYPE> arrType;
	QList<Layer*> layerlist=m_LayerHash.values();
	Layer* layer;

	foreach(layer,layerlist)
	{
		if(layer->GetMainType()==mainType)
			arrType.push_back(layer->GetSubType());
	}
	return arrType;
}


void CQGlobe_DataMgr::ReadGeoShapeColorWidth()
{
	QString m_shapeAttrFName = qglobe_GetWorkPath() + "/ShapeAttribute.xml";

	QFile file(m_shapeAttrFName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996) // QtXml SAX API is deprecated in Qt 5.15
#endif
	QXmlSimpleReader reader;
	QXmlInputSource source(&file);

	ColorXmlParser handler;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);
	reader.parse(&source);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	handler.SetAttributeList(m_sOption.m_geoShapeAttr);
}

CQGlobe_DataMgr::~CQGlobe_DataMgr()
{
	qglobe_KillAllThread();

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

void CQGlobe_DataMgr::SetModifyFlag(bool modify)
{
	m_blModify = modify;
}

void CQGlobe_DataMgr::OnBeforeRender()
{
	// prepare rendering data
	if(IsModified())
		qglobe_MakePrepareRenderData();

	qglobe_GetDataMgr()->m_pTextMgr->Init();
}

QGlobe_DEM_TILE* CQGlobe_DataMgr::GetDemTileForCalcu(int m, int n, int level)
{
	QGlobe_DEM_TILE *pTile = NULL;
	short buffer[QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT + 50];
	QGlobe_TILE_INFO info;

	memset(&info, 0, sizeof(info));
	info.eSubType	= E_QGlobe_SUBDATA_TYPE_DEM;
	info.eMainType	= qglobe_GetMainType(info.eSubType);
	info.nLevel	= level;
	info.nX		= m;
	info.nY		= n;
	info.nSize	= QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT + 50;

	if(m_pLocalChannel == NULL)
		return NULL;

	int key1 = MAKE_CACHE_KEY1(info.nLevel, info.nX, info.nY);
	int key2 = MAKE_CACHE_KEY2(info.nLevel, info.nX, info.nY);
	
	// check cache
	if(m_pLocalDemCache)
	{
		pTile = (QGlobe_DEM_TILE*) m_pLocalDemCache->GetEntry(key1, key2);

		if(pTile != NULL)
			return pTile;
	}

	// load data
	pTile = (QGlobe_DEM_TILE*)m_pLoadDataMgr->LoadOneTile(&info, buffer, m_pLocalChannel);
	if(pTile == NULL)
		return pTile;

	pTile->tileWidth	= qglobe_TwoPI / (1 << pTile->m_sInfo.nLevel);
	pTile->gridWidth	= pTile->tileWidth / QGLOBE_MAX_DEM_DIVISION;
	pTile->minLongitude = pTile->tileWidth * pTile->m_sInfo.nX - qglobe_PI;
	pTile->minLatitude	= pTile->tileWidth * pTile->m_sInfo.nY - qglobe_PI;
	pTile->maxLongitude = pTile->minLongitude + pTile->tileWidth;
	pTile->maxLatitude	= pTile->minLatitude + pTile->tileWidth;

	// update chache
	m_pLocalDemCache->AddEntry(key1, key2, pTile);

	return pTile;
}

bool CQGlobe_DataMgr::IsNecessary(E_QGlobe_SUBDATA_TYPE type)
{
	switch(type)
	{
	case E_QGlobe_SUBDATA_TYPE_DEM:
		return (g_pQGlobeDataMgr->m_sOption.blTerrain) || 
			(g_pQGlobeDataMgr->m_sOption.layerTextureMode != LTM_TEXTURE) ||
			(g_pQGlobeDataMgr->m_sOption.blContour);
	default:
		return IsVisible(type);
	}
}

bool CQGlobe_DataMgr::IsVisible(E_QGlobe_SUBDATA_TYPE type)
{
	Layer* layer=m_LayerHash[type];

	//Q_ASSERT(layer!=NULL);

	if(layer)
		return layer->GetVisible();
	return false;
}

unsigned int CQGlobe_DataMgr::GetMinLevel(E_QGlobe_SUBDATA_TYPE type)
{
	Layer* layer=m_LayerHash[type];

	//Q_ASSERT(layer!=NULL);

	if(layer)
		return layer->GetMinLevel();
	return 0;
}

void CQGlobe_DataMgr::OnChangeServer()
{
	m_pNodeMgr->ClearAll();
}

CQGlobe_DataMgr* qglobe_GetDataMgr() 
{
	return g_pQGlobeDataMgr;
}

void CQGlobe_DataMgr::AddLayer(Layer* layer)
{
	Q_ASSERT(m_LayerHash.value(layer->GetSubType())==NULL);
	m_LayerHash.insert(layer->GetSubType(),layer);
}

void CQGlobe_DataMgr::SetLayer(Layer* layer)
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
				qglobe_GetRasterTextureIDCache()->Clear();
			}	
		}
	}
}

CGIS_Node* qglobe_GetGISNode(E_QGlobe_SUBDATA_TYPE type,int id)
{
	GISLayer* layer=g_pQGlobeDataMgr->m_pGISDataCache->GetLayer(0,0,0,type);

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

bool CQGlobe_DataMgr::IsNecessaryDirInfo()
{
	if (m_sOption.layerTextureMode==LTM_RATEPLAN)
		return true;
	if (m_sOption.layerTextureMode==LTM_RATEANGLE)
		return true;	
	return false;
}

unsigned int CQGlobe_DataMgr::GetMaxTileSize(E_QGlobe_MAINDATA_TYPE type)
{
	switch(type)
	{
		case E_QGlobe_MAINDATA_TYPE_IMG:
			return RasterLayer::MaxTileSize;
		case E_QGlobe_MAINDATA_TYPE_DEM:
			return TerrainLayer::MaxTileSize;
		case E_QGlobe_MAINDATA_TYPE_NAME:
			return	PlaceNameLayer::MaxTileSize;
		case E_QGlobe_MAINDATA_TYPE_SHAPE:
			return PolylineLayer::MaxTileSize;
		case E_QGlobe_MAINDATA_TYPE_3D:
			return ObjectLayer::MaxTileSize;
		case E_QGlobe_MAINDATA_TYPE_RASTER:
			return RasterLayer::MaxTileSize;
		default:
			qFatal("Error in GetMaxTileSize: Unknown Main Data Type ");
	}
	return 0;
}

bool CQGlobe_DataMgr::IsInvalidLevel(IN E_QGlobe_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel)
{
	if (a_nLevel < MIN_LEVEL || a_nLevel > MAX_LEVEL)
		return false;

	Layer* layer=m_LayerHash[a_eDataType];

	Q_ASSERT(layer!=NULL);

	return (a_nLevel < layer->GetMinLevel()) || (a_nLevel > layer->GetMaxLevel());
}

void CQGlobe_DataMgr::Clear3DCache()
{
	g_CacheMgrMutex.lock();
	qglobe_GetDataMgr()->m_pCacheMgr->GetCacheMgr(E_QGlobe_MAINDATA_TYPE_3D)->Clear();
	g_CacheMgrMutex.unlock();

	g_3dCacheMutex.lock();
	qglobe_GetDataMgr()->m_p3DObjectCache->Clear();
	g_3dCacheMutex.unlock();

	g_pQGlobeDataMgr->SetModifyFlag();
}

void CQGlobe_DataMgr::Delete3DData(int nX,int nY,int nLevel)
{
	g_CacheMgrMutex.lock();
	CQGlobe_CacheDataMgr* mgr=qglobe_GetDataMgr()->m_pCacheMgr->GetCacheMgr(E_QGlobe_MAINDATA_TYPE_3D);

	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(E_QGlobe_SUBDATA_TYPE_3D_BUILDING, nX, nY);

	mgr->DeleteEntry(mgr->GetCacheEntry(key1,key2));
	g_CacheMgrMutex.unlock();

	g_3dCacheMutex.lock();
	qglobe_GetDataMgr()->m_p3DObjectCache->DeleteMeshEntry(nX,nY,nLevel);
	g_3dCacheMutex.unlock();

	g_pQGlobeDataMgr->SetModifyFlag();
}

double CQGlobe_DataMgr::CameraHeight()
{
	return m_pCamera->m_location.m_dDist;
}

#pragma once

#include "qglobe_libmain_global.h"
#include "QGlobe_Tile.h"
#include "QGlobe_RenderTile.h"
#include "QGlobe_Interface.h"

#define USE_FAST_HITPOINT 1 

class CQGlobe_Camera;
class CQGlobe_PyramidMgr;
class CQGlobe_CacheMgr;
class CQGlobe_RequestMgr;
class CQGlobe_LoadDataMgr;
class CQGlobe_CollisionHandler;
class CQGlobe_FileCacheMgr;
class CQGlobe_Node_Mgr;
class CQGlobe_ContourCache;
class CQGlobe_Ruler;
class CQGlobe_TerrainPtCache;
class CQGlobe_TerrainDirCache;
class CQGlobe_TextMgr;
class CQGlobe_DataMgr;
class CQGlobe_CacheDataMgr;
class DataChannel;
class IQGlobe_TextureIDCache;
class CQGlobe_3DObjectCache;
class CQGlobe_SimpleBuildingCache;
class CQGlobe_GISDataCache;
class Layer;
class TerrainLayer;
class ImageLayer;
class RasterLayer;
class CGIS_Node;
class IColorRamp;

QGLOBE_LIBMAIN_EXPORT CQGlobe_DataMgr* qglobe_GetDataMgr();
QGLOBE_LIBMAIN_EXPORT QGlobe_DEM_TILE* qglobe_getDemTile(double dLongitude, double dLatitude);
QGLOBE_LIBMAIN_EXPORT short  qglobe_getHeightInTile(double dLongitude, double dLatitude, QGlobe_DEM_TILE* pTile);
QGLOBE_LIBMAIN_EXPORT void   qglobe_get3DWorldPoint(double dLongitude, double dLatitude, QGlobe_POINT3D &pos, double offsetRadius = 0, QGlobe_DEM_TILE* pTile = NULL, bool bHeight = false);
QGLOBE_LIBMAIN_EXPORT void   qglobe_get3DWorldPoint(double dLongitude, double dLatitude, CQGlobe_Point3DF &pos, double offsetRadius = 0, QGlobe_DEM_TILE* pTile = NULL, bool bHeight = false);
QGLOBE_LIBMAIN_EXPORT void   qglobe_get3DBodyPoint(double dLongitude, double dLatitude, QGlobe_POINT3D &pos, double offsetRadius = 0, QGlobe_DEM_TILE* pTile = NULL, bool bHeight = false);
QGLOBE_LIBMAIN_EXPORT void   qglobe_get3DWorldHighPoint(double dLongitude, double dLatitude, QGlobe_POINT3D &pos);
QGLOBE_LIBMAIN_EXPORT const  QGlobe_LOGIC_TILE_ARR*	qglobe_GetRenderTiles();
QGLOBE_LIBMAIN_EXPORT const  E_QGlobe_MAINDATA_TYPE	qglobe_GetMainType(E_QGlobe_SUBDATA_TYPE subType);


CGIS_Node *qglobe_GetGISNode(E_QGlobe_SUBDATA_TYPE type,int id);

// make prepare render data
void qglobe_MakePrepareRenderData();

// Preprocessing to clear cache data
void qglobe_ClearPrepareRenderData();

class QGLOBE_LIBMAIN_EXPORT CQGlobe_DataMgr
{
public:
	CQGlobe_DataMgr();
	~CQGlobe_DataMgr();
public:
	inline void		SetModifyFlag(bool modify = true );
	inline bool		IsModified(){return m_blModify;}
	void		    OnBeforeRender();
	inline void		StopDataLoad(bool enable) { m_blStopDataLoad = enable;}
	inline bool		IsStopDataLoad() {return m_blStopDataLoad;}

	bool		    IsVisible(E_QGlobe_SUBDATA_TYPE type);
	bool		    IsNecessary(E_QGlobe_SUBDATA_TYPE type);
	unsigned int    GetMinLevel(E_QGlobe_SUBDATA_TYPE type) ;
	bool            IsInvalidLevel(IN E_QGlobe_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel);
	QList<E_QGlobe_SUBDATA_TYPE>	    GetTypes(E_QGlobe_MAINDATA_TYPE mainType);
	inline void		SetTextureIDCache(IQGlobe_TextureIDCache *pCache) {m_pTextureIDCache = pCache;}
	void		    OnChangeServer();
	void            AddLayer(Layer* layer);
	void            SetLayer(Layer* layer);
	bool            IsNecessaryDirInfo();
	unsigned int    GetMaxTileSize(E_QGlobe_MAINDATA_TYPE);
	
	QGlobe_DEM_TILE *  GetDemTileForCalcu(int m, int n, int level);
	void            Clear3DCache();
	double          CameraHeight();
	void            Delete3DData(int x,int y,int level);
public:
	CQGlobe_CollisionHandler	*m_pCollisionHandler;
	CQGlobe_Camera				*m_pCamera;
	CQGlobe_PyramidMgr			*m_pPyramidMgr;
	CQGlobe_CacheMgr			*m_pCacheMgr;
	CQGlobe_RequestMgr			*m_pRequestMgr;
	CQGlobe_LoadDataMgr		*m_pLoadDataMgr;
	CQGlobe_FileCacheMgr		*m_pFileCacheMgr;
	CQGlobe_Node_Mgr			*m_pNodeMgr;
	CQGlobe_ContourCache		*m_pContour;
	CQGlobe_Ruler				*m_pRuler;
	CQGlobe_TerrainPtCache		*m_pTerrainPtCache;
	CQGlobe_TerrainDirCache	*m_pTerrainDirCache;
	CQGlobe_TextMgr			*m_pTextMgr;
	CQGlobe_CacheDataMgr		*m_pDemCacheForCalcu;
	QGlobe_OPTION				m_sOption;
	CQGlobe_3DObjectCache      *m_p3DObjectCache;
	CQGlobe_SimpleBuildingCache*m_pSimpleBuildingCache;
	IQGlobe_TextureIDCache		*m_pTextureIDCache;
	CQGlobe_GISDataCache*       m_pGISDataCache;

	QHash<E_QGlobe_SUBDATA_TYPE,Layer*> m_LayerHash;
	TerrainLayer*                    m_pActiveTerrainLayer; 
	ImageLayer*                      m_pActiveImageLayer; 
	RasterLayer*                     m_pActiveRasterLayer;
	QList<IColorRamp*>               m_ColorRampList;

private:
	void ReadGeoShapeColorWidth();

	bool					m_blModify;

	// for calculation data
	DataChannel*			m_pLocalChannel;
	CQGlobe_CacheDataMgr*		m_pLocalDemCache;
	bool					m_blStopDataLoad;
};

extern CQGlobe_DataMgr* g_pGDMDataMgr;
extern CQGlobe_Vector3DF g_OffsetBodyPt;


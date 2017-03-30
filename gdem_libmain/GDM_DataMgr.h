#pragma once

#include "gdem_libmain_global.h"
#include "GDM_Tile.h"
#include "GDM_RenderTile.h"
#include "GDM_Interface.h"

#define USE_FAST_HITPOINT 1 

class CGDM_Camera;
class CGDM_PyramidMgr;
class CGDM_CacheMgr;
class CGDM_RequestMgr;
class CGDM_LoadDataMgr;
class CGDM_CollisionHandler;
class CGDM_FileCacheMgr;
class CGDM_Node_Mgr;
class CGDM_ContourCache;
class CGDM_Ruler;
class CGDM_TerrainPtCache;
class CGDM_TerrainDirCache;
class CGDM_TextMgr;
class CGDM_DataMgr;
class CGDM_CacheDataMgr;
class DataChannel;
class IGDM_TextureIDCache;
class CGDM_3DObjectCache;
class CGDM_SimpleBuildingCache;
class CGDM_GISDataCache;
class Layer;
class TerrainLayer;
class ImageLayer;
class RasterLayer;
class CGIS_Node;
class IColorRamp;

GDEM_LIBMAIN_EXPORT CGDM_DataMgr* gdm_GetDataMgr();
GDEM_LIBMAIN_EXPORT GDM_DEM_TILE* gdm_getDemTile(double dLongitude, double dLatitude);
GDEM_LIBMAIN_EXPORT short  gdm_getHeightInTile(double dLongitude, double dLatitude, GDM_DEM_TILE* pTile);
GDEM_LIBMAIN_EXPORT void   gdm_get3DWorldPoint(double dLongitude, double dLatitude, GDM_POINT3D &pos, double offsetRadius = 0, GDM_DEM_TILE* pTile = NULL, bool bHeight = false);
GDEM_LIBMAIN_EXPORT void   gdm_get3DWorldPoint(double dLongitude, double dLatitude, CGDM_Point3DF &pos, double offsetRadius = 0, GDM_DEM_TILE* pTile = NULL, bool bHeight = false);
GDEM_LIBMAIN_EXPORT void   gdm_get3DBodyPoint(double dLongitude, double dLatitude, GDM_POINT3D &pos, double offsetRadius = 0, GDM_DEM_TILE* pTile = NULL, bool bHeight = false);
GDEM_LIBMAIN_EXPORT void   gdm_get3DWorldHighPoint(double dLongitude, double dLatitude, GDM_POINT3D &pos);
GDEM_LIBMAIN_EXPORT const  GDM_LOGIC_TILE_ARR*	gdm_GetRenderTiles();
GDEM_LIBMAIN_EXPORT const  E_GDM_MAINDATA_TYPE	gdm_GetMainType(E_GDM_SUBDATA_TYPE subType);


CGIS_Node *gdm_GetGISNode(E_GDM_SUBDATA_TYPE type,int id);

// make prepare render data
void gdm_MakePrepareRenderData();

// Preprocessing to clear cache data
void gdm_ClearPrepareRenderData();

class GDEM_LIBMAIN_EXPORT CGDM_DataMgr
{
public:
	CGDM_DataMgr();
	~CGDM_DataMgr();
public:
	inline void		SetModifyFlag(bool modify = true );
	inline bool		IsModified(){return m_blModify;}
	void		    OnBeforeRender();
	inline void		StopDataLoad(bool enable) { m_blStopDataLoad = enable;}
	inline bool		IsStopDataLoad() {return m_blStopDataLoad;}

	bool		    IsVisible(E_GDM_SUBDATA_TYPE type);
	bool		    IsNecessary(E_GDM_SUBDATA_TYPE type);
	unsigned int    GetMinLevel(E_GDM_SUBDATA_TYPE type) ;
	bool            IsInvalidLevel(IN E_GDM_SUBDATA_TYPE a_eDataType, IN unsigned int a_nLevel);
	QList<E_GDM_SUBDATA_TYPE>	    GetTypes(E_GDM_MAINDATA_TYPE mainType);
	inline void		SetTextureIDCache(IGDM_TextureIDCache *pCache) {m_pTextureIDCache = pCache;}
	void		    OnChangeServer();
	void            AddLayer(Layer* layer);
	void            SetLayer(Layer* layer);
	bool            IsNecessaryDirInfo();
	unsigned int    GetMaxTileSize(E_GDM_MAINDATA_TYPE);
	
	GDM_DEM_TILE *  GetDemTileForCalcu(int m, int n, int level);
	void            Clear3DCache();
	double          CameraHeight();
	void            Delete3DData(int x,int y,int level);
public:
	CGDM_CollisionHandler	*m_pCollisionHandler;
	CGDM_Camera				*m_pCamera;
	CGDM_PyramidMgr			*m_pPyramidMgr;
	CGDM_CacheMgr			*m_pCacheMgr;
	CGDM_RequestMgr			*m_pRequestMgr;
	CGDM_LoadDataMgr		*m_pLoadDataMgr;
	CGDM_FileCacheMgr		*m_pFileCacheMgr;
	CGDM_Node_Mgr			*m_pNodeMgr;
	CGDM_ContourCache		*m_pContour;
	CGDM_Ruler				*m_pRuler;
	CGDM_TerrainPtCache		*m_pTerrainPtCache;
	CGDM_TerrainDirCache	*m_pTerrainDirCache;
	CGDM_TextMgr			*m_pTextMgr;
	CGDM_CacheDataMgr		*m_pDemCacheForCalcu;
	GDM_OPTION				m_sOption;
	CGDM_3DObjectCache      *m_p3DObjectCache;
	CGDM_SimpleBuildingCache*m_pSimpleBuildingCache;
	IGDM_TextureIDCache		*m_pTextureIDCache;
	CGDM_GISDataCache*       m_pGISDataCache;

	QHash<E_GDM_SUBDATA_TYPE,Layer*> m_LayerHash;
	TerrainLayer*                    m_pActiveTerrainLayer; 
	ImageLayer*                      m_pActiveImageLayer; 
	RasterLayer*                     m_pActiveRasterLayer;
	QList<IColorRamp*>               m_ColorRampList;

private:
	void ReadGeoShapeColorWidth();

	bool					m_blModify;

	// for calculation data
	DataChannel*			m_pLocalChannel;
	CGDM_CacheDataMgr*		m_pLocalDemCache;
	bool					m_blStopDataLoad;
};

extern CGDM_DataMgr* g_pGDMDataMgr;
extern CGDM_Vector3DF g_OffsetBodyPt;


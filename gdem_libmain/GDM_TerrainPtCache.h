#pragma once

#include "GDM_CacheDataMgr.h"
#include "GDM_Tile.h"
#include "gdem_libmain_global.h"

class GDEM_LIBMAIN_EXPORT CGDM_TerrainPtCache :public CGDM_CacheDataMgr
{

public:
	CGDM_TerrainPtCache(void);
	~CGDM_TerrainPtCache(void);

public:
	GDM_POINT3D*	GetPts(GDM_DEM_TILE *pTile);
	void			SetHeightRate(double rate);
	GDM_DEM_TILE*	GetDefaultDemTile() {return &m_sDefaultTile;}
	void			CalcutatePts(GDM_DEM_TILE *pTile, GDM_POINT3D *pVertexBuf, double dHeightRate, bool blHeight = true);
	void			SetEnableHeight(bool enable);

private:
	double			m_dHeightRate; // 0.5 - 3
	GDM_POINT3D		m_pDefaultPts[GDEM_MAX_DEM_PT_COUNT * GDEM_MAX_DEM_PT_COUNT];
	GDM_DEM_TILE	m_sDefaultTile;
	bool			m_blHeight;

protected:
	virtual void	FreeData(void *pData);
};

class GDEM_LIBMAIN_EXPORT CGDM_TerrainDirCache :public CGDM_CacheDataMgr
{

public:
	CGDM_TerrainDirCache(void);
	~CGDM_TerrainDirCache(void);

public:
	GDM_DIRECTION_INFO*	GetInfos(GDM_DEM_TILE *pTile);
	void			SetPtCache(CGDM_TerrainPtCache *pCache) { m_pPtCache = pCache;}

protected:
	virtual void	FreeData(void *pData);

private:
	void			CalcutatePts(GDM_DEM_TILE *pTile, GDM_DIRECTION_INFO *pInfoBuf);
	GDM_DIRECTION_INFO* GetInfos(int nX, int nY, int nLevel);

	void			MergeLeft(GDM_DIRECTION_INFO* pInfo, GDM_DIRECTION_INFO* pLeft);
	void			MergeBottom(GDM_DIRECTION_INFO* pInfo, GDM_DIRECTION_INFO* pBottom);

private:
	CGDM_TerrainPtCache*	m_pPtCache;

};

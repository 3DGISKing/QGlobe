#pragma once

#include "QGlobe_CacheDataMgr.h"
#include "QGlobe_Tile.h"
#include "qglobe_libmain_global.h"

class QGLOBE_LIBMAIN_EXPORT CQGlobe_TerrainPtCache :public CQGlobe_CacheDataMgr
{

public:
	CQGlobe_TerrainPtCache(void);
	~CQGlobe_TerrainPtCache(void);

public:
	QGlobe_POINT3D*	GetPts(QGlobe_DEM_TILE *pTile);
	void			SetHeightRate(double rate);
	QGlobe_DEM_TILE*	GetDefaultDemTile() {return &m_sDefaultTile;}
	void			CalcutatePts(QGlobe_DEM_TILE *pTile, QGlobe_POINT3D *pVertexBuf, double dHeightRate, bool blHeight = true);
	void			SetEnableHeight(bool enable);

private:
	double			m_dHeightRate; // 0.5 - 3
	QGlobe_POINT3D		m_pDefaultPts[QGLOBE_MAX_DEM_PT_COUNT * QGLOBE_MAX_DEM_PT_COUNT];
	QGlobe_DEM_TILE	m_sDefaultTile;
	bool			m_blHeight;

protected:
	virtual void	FreeData(void *pData);
};

class QGLOBE_LIBMAIN_EXPORT CQGlobe_TerrainDirCache :public CQGlobe_CacheDataMgr
{

public:
	CQGlobe_TerrainDirCache(void);
	~CQGlobe_TerrainDirCache(void);

public:
	QGlobe_DIRECTION_INFO*	GetInfos(QGlobe_DEM_TILE *pTile);
	void			SetPtCache(CQGlobe_TerrainPtCache *pCache) { m_pPtCache = pCache;}

protected:
	virtual void	FreeData(void *pData);

private:
	void			CalcutatePts(QGlobe_DEM_TILE *pTile, QGlobe_DIRECTION_INFO *pInfoBuf);
	QGlobe_DIRECTION_INFO* GetInfos(int nX, int nY, int nLevel);

	void			MergeLeft(QGlobe_DIRECTION_INFO* pInfo, QGlobe_DIRECTION_INFO* pLeft);
	void			MergeBottom(QGlobe_DIRECTION_INFO* pInfo, QGlobe_DIRECTION_INFO* pBottom);

private:
	CQGlobe_TerrainPtCache*	m_pPtCache;

};

#pragma once

#include "QGlobe_CacheDataMgr.h"
#include "Building.h"
#include "QGlobe_Tile.h"

class CQGlobe_SimpleBuildingCache :public CQGlobe_CacheDataMgr
{
public:
	CQGlobe_SimpleBuildingCache(void);
public:
	~CQGlobe_SimpleBuildingCache(void);
	BuildingArray* GetBuildingArray(int x,int y,int level);
	BuildingArray* GetBuildingArray(QGlobe_TILE* ptile);
protected:
	virtual void	FreeData(void *pData);
};

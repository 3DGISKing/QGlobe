#pragma once

#include "GDM_CacheDataMgr.h"
#include "Building.h"
#include "GDM_Tile.h"

class CGDM_SimpleBuildingCache :public CGDM_CacheDataMgr
{
public:
	CGDM_SimpleBuildingCache(void);
public:
	~CGDM_SimpleBuildingCache(void);
	BuildingArray* GetBuildingArray(int x,int y,int level);
	BuildingArray* GetBuildingArray(GDM_TILE* ptile);
protected:
	virtual void	FreeData(void *pData);
};

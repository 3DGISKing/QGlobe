#pragma once

#include "GDM_CacheDataMgr.h"
#include "GDM_Tile.h"
#include "gdem_libmain_global.h"

class GDEM_LIBMAIN_EXPORT RasterTextureIDCache :public CGDM_CacheDataMgr
{
public:
	RasterTextureIDCache(void);
	~RasterTextureIDCache(void);
public:
	long		GetTextureID(GDM_TILE *pInfo);
protected:
	virtual void	FreeData(void *pData);
};

RasterTextureIDCache * gdm_GetRasterTextureIDCache();

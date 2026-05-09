#pragma once

#include "QGlobe_CacheDataMgr.h"
#include "QGlobe_Tile.h"
#include "qglobe_libmain_global.h"

class QGLOBE_LIBMAIN_EXPORT RasterTextureIDCache :public CQGlobe_CacheDataMgr
{
public:
	RasterTextureIDCache(void);
	~RasterTextureIDCache(void);
public:
	long		GetTextureID(QGlobe_TILE *pInfo);
protected:
	virtual void	FreeData(void *pData);
};

RasterTextureIDCache * qglobe_GetRasterTextureIDCache();

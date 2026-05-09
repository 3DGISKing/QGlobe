#pragma once

#include "QGlobe_CacheDataMgr.h"
#include "QGlobe_Tile.h"
#include "qglobe_libmain_global.h"

class QGLOBE_LIBMAIN_EXPORT CGIS_TextureIDCache :public CQGlobe_CacheDataMgr
{
public:
	CGIS_TextureIDCache(void);
	~CGIS_TextureIDCache(void);
public:
	long		GetTextureID(QGlobe_TILE_INFO *pInfo);
	void        AddTileEntry(QGlobe_TILE_INFO Info,void *);
	void        AddTileEntry(int level,int x,int y, void *buf);
	long        GetTextureID(int level,int x,int y);
protected:
	virtual void	FreeData(void *pData);
};

CGIS_TextureIDCache * qglobe_GetGISTextureIDCache();

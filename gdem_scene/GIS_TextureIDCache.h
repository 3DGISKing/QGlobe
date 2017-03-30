#pragma once

#include "GDM_CacheDataMgr.h"
#include "GDM_Tile.h"
#include "gdem_libmain_global.h"

class GDEM_LIBMAIN_EXPORT CGIS_TextureIDCache :public CGDM_CacheDataMgr
{
public:
	CGIS_TextureIDCache(void);
	~CGIS_TextureIDCache(void);
public:
	long		GetTextureID(GDM_TILE_INFO *pInfo);
	void        AddTileEntry(GDM_TILE_INFO Info,void *);
	void        AddTileEntry(int level,int x,int y, void *buf);
	long        GetTextureID(int level,int x,int y);
protected:
	virtual void	FreeData(void *pData);
};

CGIS_TextureIDCache * gdm_GetGISTextureIDCache();

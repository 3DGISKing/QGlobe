#pragma once

#include "GDM_CacheDataMgr.h"
#include "GDM_Tile.h"

#include <QtCore/QMutex>

class GDEM_LIBMAIN_EXPORT IGDM_TextureIDCache :public CGDM_CacheDataMgr
{
public:
	IGDM_TextureIDCache(void) {}
	~IGDM_TextureIDCache(void) {}
public:
	virtual long	GetTextureID(GDM_TEXTURE_TILE *pTile) = 0;
protected:
	virtual void	FreeData(void *pData) {};
};
extern QMutex g_TextureIDCacheMutex;

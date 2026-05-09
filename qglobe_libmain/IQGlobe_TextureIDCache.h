#pragma once

#include "QGlobe_CacheDataMgr.h"
#include "QGlobe_Tile.h"

#include <QtCore/QMutex>

class QGLOBE_LIBMAIN_EXPORT IQGlobe_TextureIDCache :public CQGlobe_CacheDataMgr
{
public:
	IQGlobe_TextureIDCache(void) {}
	~IQGlobe_TextureIDCache(void) {}
public:
	virtual long	GetTextureID(QGlobe_TEXTURE_TILE *pTile) = 0;
protected:
	virtual void	FreeData(void *pData) {};
};
extern QMutex g_TextureIDCacheMutex;

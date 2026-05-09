#pragma once

#include "IQGlobe_TextureIDCache.h"

class CQGlobe_TextureIDCache :public IQGlobe_TextureIDCache
{
public:
	CQGlobe_TextureIDCache(void);
	~CQGlobe_TextureIDCache(void);
public:
	virtual long	GetTextureID(QGlobe_TEXTURE_TILE *pTile);
protected:
	virtual void	FreeData(void *pData);
};

CQGlobe_TextureIDCache * qglobe_GetTextureIDCache();

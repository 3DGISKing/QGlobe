#pragma once

#include "IGDM_TextureIDCache.h"

class CGDM_TextureIDCache :public IGDM_TextureIDCache
{
public:
	CGDM_TextureIDCache(void);
	~CGDM_TextureIDCache(void);
public:
	virtual long	GetTextureID(GDM_TEXTURE_TILE *pTile);
protected:
	virtual void	FreeData(void *pData);
};

CGDM_TextureIDCache * gdm_GetTextureIDCache();

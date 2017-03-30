#include "GDM_TextureIDCache.h"
#include "GDM_CacheMgr.h"
#include "GDM_TextureMgr.h"

#define MAX_TEXTURE_ID_COUNT	180


CGDM_TextureIDCache*	g_pGdmTextureIDCache;
CGDM_TextureIDCache * gdm_GetTextureIDCache(){return g_pGdmTextureIDCache;}

QMutex g_TextureIDCacheMutex;
CGDM_TextureIDCache::CGDM_TextureIDCache(void)
{
	SetMaxEntryCnt(MAX_TEXTURE_ID_COUNT);
	g_pGdmTextureIDCache = this;
}

CGDM_TextureIDCache::~CGDM_TextureIDCache(void)
{
	Clear();
}

long CGDM_TextureIDCache::GetTextureID(GDM_TEXTURE_TILE *pTile)
{
	long textureID;
	int key1, key2;

	if(pTile == NULL)
		return 0;

	key1 = MAKE_CACHE_KEY1(pTile->m_sInfo.nLevel, pTile->m_sInfo.nX, pTile->m_sInfo.nY);
	key2 = MAKE_CACHE_KEY2(pTile->m_sInfo.nLevel, pTile->m_sInfo.nX, pTile->m_sInfo.nY);

	textureID = (long)GetEntry(key1, key2);

	if(textureID != 0)
		return textureID;

	// generate texture id
	textureID = gdm_GetTextureMgr()->AddTexture(
		pTile->m_sInfo.nX, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel, pTile->m_pBuffer, pTile->m_nBufSize);

	// add data to cach
	AddEntry(key1, key2, (void*)textureID);

	return textureID;
}

void CGDM_TextureIDCache::FreeData(void *pData)
{
	unsigned long textureID = (unsigned long)pData;
	gdm_GetTextureMgr()->DelTexture(textureID);
}

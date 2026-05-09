#include "QGlobe_TextureIDCache.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_TextureMgr.h"

#define MAX_TEXTURE_ID_COUNT	180


CQGlobe_TextureIDCache*	g_pGdmTextureIDCache;
CQGlobe_TextureIDCache * qglobe_GetTextureIDCache(){return g_pGdmTextureIDCache;}

QMutex g_TextureIDCacheMutex;
CQGlobe_TextureIDCache::CQGlobe_TextureIDCache(void)
{
	SetMaxEntryCnt(MAX_TEXTURE_ID_COUNT);
	g_pGdmTextureIDCache = this;
}

CQGlobe_TextureIDCache::~CQGlobe_TextureIDCache(void)
{
	Clear();
}

long CQGlobe_TextureIDCache::GetTextureID(QGlobe_TEXTURE_TILE *pTile)
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
	textureID = qglobe_GetTextureMgr()->AddTexture(
		pTile->m_sInfo.nX, pTile->m_sInfo.nY, pTile->m_sInfo.nLevel, pTile->m_pBuffer, pTile->m_nBufSize);

	// add data to cach
	AddEntry(key1, key2, (void*)textureID);

	return textureID;
}

void CQGlobe_TextureIDCache::FreeData(void *pData)
{
	unsigned long textureID = (unsigned long)pData;
	qglobe_GetTextureMgr()->DelTexture(textureID);
}

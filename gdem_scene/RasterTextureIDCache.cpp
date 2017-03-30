#include "RasterTextureIDCache.h"
#include <QtGui/QImage>

#include "GDM_CacheMgr.h"
#include "RasterTextureMgr.h"
#include "Layer.h"
#include "GDM_DataMgr.h"
#include "RasterRenderer.h"

#define MAX_TEXTURE_ID_COUNT	180

RasterTextureIDCache*	g_pRasterTextureIDCache = NULL;
RasterTextureIDCache * gdm_GetRasterTextureIDCache(){return g_pRasterTextureIDCache;}

RasterTextureIDCache::RasterTextureIDCache(void)
{
	SetMaxEntryCnt(MAX_TEXTURE_ID_COUNT);
	g_pRasterTextureIDCache = this;
}

RasterTextureIDCache::~RasterTextureIDCache(void)
{
	Clear();
}

long RasterTextureIDCache::GetTextureID(GDM_TILE *tile)
{
	long textureID;
	int key1, key2;

	if(tile == NULL)
		return 0;

	key1 = MAKE_CACHE_KEY1(tile->m_sInfo.nLevel, tile->m_sInfo.nX,tile->m_sInfo.nY);
	key2 = MAKE_CACHE_KEY2(tile->m_sInfo.nLevel, tile->m_sInfo.nX, tile->m_sInfo.nY);

	textureID = (long)GetEntry(key1, key2);

	if(textureID != 0)	return textureID;

	RasterLayer *layer=(RasterLayer*)g_pGDMDataMgr->m_LayerHash.value(tile->m_sInfo.eSubType);

	QImage im(256,256,QImage::Format_ARGB32);

	int headersize=72;

	float* data=(float*)(tile->m_pBuffer+headersize);
	float val;
	QRgb color;

	for(int y=0;y<256;y++)
		for(int x=0;x<256;x++)
		{
			val=data[y*256+x];
				
			color=layer->GetRenderer()->GetColorFromData(val);
			im.setPixel(x,y,color);
		}

	//debug
	/*QString filename;
	filename.sprintf("C:/%d/%d_%d_%d.bmp",tile->m_sInfo.nLevel,tile->m_sInfo.nX,tile->m_sInfo.nY,tile->m_sInfo.nLevel);
	im.save(filename);*/
	//end
		
	// generate texture id
	textureID = gdm_GetRasterTextureMgr()->AddTexture(tile->m_sInfo.nX, tile->m_sInfo.nY, tile->m_sInfo.nLevel, (void*)&im);

	// add data to cache
	AddEntry(key1, key2, (void*)textureID);

	return textureID;
}

void RasterTextureIDCache::FreeData(void *pData)
{
	unsigned long textureID = (unsigned long)pData;
	gdm_GetRasterTextureMgr()->DelTexture(textureID);
}

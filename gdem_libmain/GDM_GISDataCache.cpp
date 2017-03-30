#include "GDM_GISDataCache.h"
#include "../gdem_client/gdemclient.h"
#include "GIS_Dataset.h"

#define MAKE_CACHE_KEY1(level, x, y)	((level << MAX_TILE_LEVEL) | x)
#define MAKE_CACHE_KEY2(level, x, y)	((level << MAX_TILE_LEVEL) | y)

CGDM_GISDataCache::~CGDM_GISDataCache()
{
	Clear();
}

void CGDM_GISDataCache::AddLayer(int x,int y,int level,E_GDM_SUBDATA_TYPE type,GISLayer* layer)
{
	Q_ASSERT(layer!=NULL);

	int key1 = MAKE_CACHE_KEY1(level, x, y);
	int key2 = MAKE_CACHE_KEY2(type, x, y);

	AddEntry(key1,key2,layer);
}

GISLayer* CGDM_GISDataCache::GetLayer(int nX, int nY, int nLevel, E_GDM_SUBDATA_TYPE type)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);

	GISLayer* layer=(GISLayer*)GetEntry(key1,key2);
	return layer;
}

void CGDM_GISDataCache::FreeData(void *layer)
{
	GISLayer* gislayer=(GISLayer*)layer;
	delete gislayer;
}
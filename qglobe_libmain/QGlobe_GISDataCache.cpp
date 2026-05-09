#include "QGlobe_GISDataCache.h"
#include "../qglobe_client/qglobeclient.h"
#include "GIS_Dataset.h"

#define MAKE_CACHE_KEY1(level, x, y)	((level << MAX_TILE_LEVEL) | x)
#define MAKE_CACHE_KEY2(level, x, y)	((level << MAX_TILE_LEVEL) | y)

CQGlobe_GISDataCache::~CQGlobe_GISDataCache()
{
	Clear();
}

void CQGlobe_GISDataCache::AddLayer(int x,int y,int level,E_QGlobe_SUBDATA_TYPE type,GISLayer* layer)
{
	Q_ASSERT(layer!=NULL);

	int key1 = MAKE_CACHE_KEY1(level, x, y);
	int key2 = MAKE_CACHE_KEY2(type, x, y);

	AddEntry(key1,key2,layer);
}

GISLayer* CQGlobe_GISDataCache::GetLayer(int nX, int nY, int nLevel, E_QGlobe_SUBDATA_TYPE type)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);

	GISLayer* layer=(GISLayer*)GetEntry(key1,key2);
	return layer;
}

void CQGlobe_GISDataCache::FreeData(void *layer)
{
	GISLayer* gislayer=(GISLayer*)layer;
	delete gislayer;
}
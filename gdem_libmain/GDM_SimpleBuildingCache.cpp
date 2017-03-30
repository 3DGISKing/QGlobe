#include "GDM_SimpleBuildingCache.h"
#include "GDM_CacheMgr.h"
#include "GDM_Mem.h"
#define GDM_SIMPLEBUILG_CACHE_ITEM_COUNT	100

CGDM_SimpleBuildingCache::CGDM_SimpleBuildingCache(void)
{
	SetMaxEntryCnt(GDM_SIMPLEBUILG_CACHE_ITEM_COUNT);
}

CGDM_SimpleBuildingCache::~CGDM_SimpleBuildingCache(void)
{
	Clear();
}

//ÂÙ±¶ À°ËÎÊ¯Ëæ ËØ²÷ °¼·Ï¼ïÂÝË¾ ÊÐ²÷³Þ
BuildingArray* CGDM_SimpleBuildingCache::GetBuildingArray(int x,int y,int level)
{
	BuildingArray *pBuildingArray;
	int key1,key2;

	key1 = MAKE_CACHE_KEY1(level,x,y);
	key2 = MAKE_CACHE_KEY2(level,x,y);

	pBuildingArray = (BuildingArray*)GetEntry(key1, key2);

	return pBuildingArray;
}

BuildingArray* CGDM_SimpleBuildingCache::GetBuildingArray(GDM_TILE* ptile)
{
	if(ptile == NULL) return NULL; 

	int key1, key2;

	key1 = MAKE_CACHE_KEY1(ptile->m_sInfo.nLevel, ptile->m_sInfo.nX, ptile->m_sInfo.nY);
	key2 = MAKE_CACHE_KEY2(ptile->m_sInfo.nLevel, ptile->m_sInfo.nX, ptile->m_sInfo.nY);

	BuildingArray *pArray=(BuildingArray*)GetEntry(key1, key2);

	if(pArray) 
		return pArray;

	pArray= new BuildingArray();

	pArray->ConstructFromRawData(ptile->m_pBuffer);

	AddEntry(key1, key2, pArray);

	return pArray;
}

void CGDM_SimpleBuildingCache::FreeData(void* pData)
{
	BuildingArray *pArray=(BuildingArray*)pData;
	delete pArray;
}
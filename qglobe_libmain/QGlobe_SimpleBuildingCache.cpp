#include "QGlobe_SimpleBuildingCache.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_Mem.h"
#define QGlobe_SIMPLEBUILG_CACHE_ITEM_COUNT	100

CQGlobe_SimpleBuildingCache::CQGlobe_SimpleBuildingCache(void)
{
	SetMaxEntryCnt(QGlobe_SIMPLEBUILG_CACHE_ITEM_COUNT);
}

CQGlobe_SimpleBuildingCache::~CQGlobe_SimpleBuildingCache(void)
{
	Clear();
}

//�ٱ� ����ʯ�� �ز� ���ϼ���˾ �в���
BuildingArray* CQGlobe_SimpleBuildingCache::GetBuildingArray(int x,int y,int level)
{
	BuildingArray *pBuildingArray;
	int key1,key2;

	key1 = MAKE_CACHE_KEY1(level,x,y);
	key2 = MAKE_CACHE_KEY2(level,x,y);

	pBuildingArray = (BuildingArray*)GetEntry(key1, key2);

	return pBuildingArray;
}

BuildingArray* CQGlobe_SimpleBuildingCache::GetBuildingArray(QGlobe_TILE* ptile)
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

void CQGlobe_SimpleBuildingCache::FreeData(void* pData)
{
	BuildingArray *pArray=(BuildingArray*)pData;
	delete pArray;
}
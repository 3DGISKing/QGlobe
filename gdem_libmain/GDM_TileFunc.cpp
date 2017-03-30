#include "GDM_TileFunc.h"
#include "GDM_Define.h"

int gdm_IsSameTile(const GDM_TILE_INFO* pInfo1, const GDM_TILE_INFO* pInfo2)
{
	if(pInfo1->nLevel == pInfo2->nLevel &&
		pInfo1->nX == pInfo2->nX &&
		pInfo1->nY == pInfo2->nY)
		return  TRUE;
	return FALSE;
}
int gdm_IsContained(int nX, int nY, GDM_TILE_INFO_ARR *pInfoList)
{
	int i = 0;
	for(i = 0; i < pInfoList->count; i++)
	{
		if(pInfoList->data[i].nX == nX && pInfoList->data[i].nY == nY)
			return TRUE;
	}
	return FALSE;
}

int gdm_IsContained(int nX, int nY, int nLevel, GDM_TILE_INFO_ARR *pInfoList)
{
	int i = 0;
	for(i = 0; i < pInfoList->count; i++)
	{
		if(pInfoList->data[i].nX == nX && pInfoList->data[i].nY == nY && pInfoList->data[i].nLevel == nLevel)
			return TRUE;
	}
	return FALSE;
}

int gdm_IsTileValid(int nX, int nY, int nLevel)
{
	int count = 1 << nLevel;

	// check x axis
	if(nX < 0 || nX >=count)
		return FALSE;

	// check y axis
	if(nX < 0 || nX >=count)
		return FALSE;

	if(nLevel >= 2)
	{
		if(nY >= (count - count / 4) || nY < (count / 4))
			return FALSE;
	}

	return TRUE;
}

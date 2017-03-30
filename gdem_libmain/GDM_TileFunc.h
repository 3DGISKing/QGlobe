#pragma once

#include "GDM_Tile.h"

int gdm_IsSameTile(const GDM_TILE_INFO* pInfo1, const GDM_TILE_INFO* pInfo2);
int gdm_IsContained(int nX, int nY, GDM_TILE_INFO_ARR *pInfoList);
int gdm_IsContained(int nX, int nY, int nLevel, GDM_TILE_INFO_ARR *pInfoList);
int gdm_IsTileValid(int nX, int nY, int nLevel);

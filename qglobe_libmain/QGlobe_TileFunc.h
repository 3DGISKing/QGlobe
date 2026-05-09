#pragma once

#include "QGlobe_Tile.h"

int qglobe_IsSameTile(const QGlobe_TILE_INFO* pInfo1, const QGlobe_TILE_INFO* pInfo2);
int qglobe_IsContained(int nX, int nY, QGlobe_TILE_INFO_ARR *pInfoList);
int qglobe_IsContained(int nX, int nY, int nLevel, QGlobe_TILE_INFO_ARR *pInfoList);
int qglobe_IsTileValid(int nX, int nY, int nLevel);

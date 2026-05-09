#pragma once

#include "QGlobe_Tile.h"

typedef enum
{
	LOAD_RESP_OK,
	LOAD_RESP_NO_EXIST,
	LOAD_RESP_ERROR,
}E_LOAD_RESP;

class CQGlobe_LoadDataMgr
{
public:
	CQGlobe_LoadDataMgr(void);
	~CQGlobe_LoadDataMgr(void);
public:
	int			GetTileBuffer(const QGlobe_TILE_INFO* tileInfo, void* buffer, void *channel);
	QGlobe_TILE *	LoadOneTile(QGlobe_TILE_INFO *pTileInfo, void * buffer, void *channel);
};

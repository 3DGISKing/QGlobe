#pragma once

#include "GDM_Tile.h"

typedef enum
{
	LOAD_RESP_OK,
	LOAD_RESP_NO_EXIST,
	LOAD_RESP_ERROR,
}E_LOAD_RESP;

class CGDM_LoadDataMgr
{
public:
	CGDM_LoadDataMgr(void);
	~CGDM_LoadDataMgr(void);
public:
	int			GetTileBuffer(const GDM_TILE_INFO* tileInfo, void* buffer, void *channel);
	GDM_TILE *	LoadOneTile(GDM_TILE_INFO *pTileInfo, void * buffer, void *channel);
};

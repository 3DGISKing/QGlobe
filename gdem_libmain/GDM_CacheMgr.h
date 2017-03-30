#pragma once

#include "GDM_Tile.h"

#include "gdem_libmain_global.h"
#include <QtCore/QMutex>

#define MAKE_CACHE_KEY1(level, x, y)	((level << MAX_TILE_LEVEL) | x)
#define MAKE_CACHE_KEY2(level, x, y)	((level << MAX_TILE_LEVEL) | y)

typedef enum	/* Cache Callback function's flag */
{
	E_CACHE_CB_DEL_BEFORE,	/* before deleting tile */
	E_CACHE_CB_DEL_AFTER,	/* after tile is deleted */

} E_CACHE_CB_FLAG;

typedef struct  
{
	int		key1;
	int		key2;
	void*	pData;
} CACHE_CB_DATA;

typedef int (GDM_CacheCallBackFunc)(int type, CACHE_CB_DATA *data, E_CACHE_CB_FLAG flag);

class CGDM_CacheDataMgr;

class GDEM_LIBMAIN_EXPORT CGDM_CacheMgr
{
public:
	CGDM_CacheMgr(void);
	~CGDM_CacheMgr(void);

public:
	int			AddEntry(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY, void* buf);
	void*		GetEntry(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY);
	int			IsEntryExist(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY);
	void        DeleteEntry(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY);
	void        DeleteEntry(int type, int key1, int key2);

	void		Clear();

	int			SetCbFunc (GDM_CacheCallBackFunc *pFunc);

	int			ProcessCbMsg (int type, CACHE_CB_DATA* data, E_CACHE_CB_FLAG flag);

	void		SetMaxSize(int nSize);
	void        ImageCacheClear();
	void        RasterCacheClear();
	void        NameCacheClear();
	CGDM_CacheDataMgr* GetCacheMgr(E_GDM_MAINDATA_TYPE type);
protected:
	int			AddEntry(int type, int key1, int key2, void* buf);
	void*		GetEntry(int type, int key1, int key2);
	int			IsEntryExist(int type, int key1, int key2);

private:
	GDM_CacheCallBackFunc * m_pCbFunc;	/* Callback function */

	int			m_nSize;
	
private:
	CGDM_CacheDataMgr**	m_pItemMgr;
};

#define MAX_CACHE_GEODATA_TYPE		E_GDM_MAINDATA_TYPE_MAX


int gdm_CacheCallbackFunc(int type, CACHE_CB_DATA* data, E_CACHE_CB_FLAG flag);
extern QMutex g_CacheMgrMutex;
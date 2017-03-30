#pragma once

#include "avl.h"
#include "GDM_Tile.h"
#include <QMutex>

#include "gdem_libmain_global.h"

typedef struct  
{
	int key1;
	int key2;
	int offset;
	int datasize;
} CACHE_FILE_INDEX;

class GDEM_LIBMAIN_EXPORT CGDM_CacheIndexTree
{
public:
	CGDM_CacheIndexTree();
	~CGDM_CacheIndexTree();

public:
	int		AddIndex(CACHE_FILE_INDEX* entry);
	int		GetIndex(CACHE_FILE_INDEX* entry);

	void	RemoveAll();
private:
	struct avl_Tree*	m_pTree;
	int					m_nFlag;	
};

class GDEM_LIBMAIN_EXPORT CGDM_FileCacheMgr
{
public:
	CGDM_FileCacheMgr();
	~CGDM_FileCacheMgr();

public:	
	void	WriteTileBuffer(GDM_TILE_INFO* pInfo, void* buffer); // Write to file
	int		ReadTileBuffer(GDM_TILE_INFO* pInfo, void* buffer); // Load tile buffer
	void	Clear();// Clear file data

	void	SetCacheFileSize(int nSize) { m_nMaxSize = (nSize << 20);}

private:
	void	OpenCacheDataFile(const char *szPathname);	// Open cache file	

private:
	void	LoadStoredData();
	void	Initialize();
	int		IsValidCacheFile();

	int		ReadData(int offset, int size, void* buffer);
	int		WriteData(CACHE_FILE_INDEX* key, void* buffer, int size);

private:
	CGDM_CacheIndexTree**	m_pIndices;
	int						m_nState;
	int						m_nMaxSize;
	int						m_nCurSize;

	char					m_szDataFilePath[255];
	FILE*					m_fpCacheData;
};

#pragma once

#include "avl.h"
#include "QGlobe_Tile.h"
#include <QMutex>

#include "qglobe_libmain_global.h"

typedef struct  
{
	int key1;
	int key2;
	int offset;
	int datasize;
} CACHE_FILE_INDEX;

class QGLOBE_LIBMAIN_EXPORT CQGlobe_CacheIndexTree
{
public:
	CQGlobe_CacheIndexTree();
	~CQGlobe_CacheIndexTree();

public:
	int		AddIndex(CACHE_FILE_INDEX* entry);
	int		GetIndex(CACHE_FILE_INDEX* entry);

	void	RemoveAll();
private:
	struct avl_Tree*	m_pTree;
	int					m_nFlag;	
};

class QGLOBE_LIBMAIN_EXPORT CQGlobe_FileCacheMgr
{
public:
	CQGlobe_FileCacheMgr();
	~CQGlobe_FileCacheMgr();

public:	
	void	WriteTileBuffer(QGlobe_TILE_INFO* pInfo, void* buffer); // Write to file
	int		ReadTileBuffer(QGlobe_TILE_INFO* pInfo, void* buffer); // Load tile buffer
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
	CQGlobe_CacheIndexTree**	m_pIndices;
	int						m_nState;
	int						m_nMaxSize;
	int						m_nCurSize;

	char					m_szDataFilePath[255];
	FILE*					m_fpCacheData;
};

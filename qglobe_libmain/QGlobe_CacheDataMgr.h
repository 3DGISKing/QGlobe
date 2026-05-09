#pragma once

#include "QGlobe_List.h"
#include "qglobe_libmain_global.h"

#define CACHE_HASH_BIT	6
#define CACHE_MAX_HASH_SIZE	(1<<CACHE_HASH_BIT)

class CQGlobe_CacheMgr;

class QGLOBE_LIBMAIN_EXPORT CQGlobe_CacheDataMgr
{
public:
	CQGlobe_CacheDataMgr(int type, int nCnt);
	CQGlobe_CacheDataMgr(int nCnt);
	CQGlobe_CacheDataMgr();
	~CQGlobe_CacheDataMgr();

public:
	int			AddEntry(int key1, int key2, void* buf);
	void*		GetEntry(int key1, int key2);

	void		SetDataType(int type) {m_nType = type;}
	void		SetMaxEntryCnt(int nCnt) {m_nMaxCnt = nCnt;}
	void		SetParent(CQGlobe_CacheMgr* pParent) {m_pParent = pParent;}
	void		Clear(bool blDestroy = true);
	void		RemoveAll();
	void		DeleteEntry(void *pEntry);
	void*       GetCacheEntry(int key1,int key2);
private:
	void		LRUFixup(void *pEntry);
	void		CleanUp ();
	
	void		Init();

protected:
	virtual void	FreeData(void *pData);

private:

	CQGlobe_CacheMgr*	m_pParent;
	int				m_nType;
	int				m_nMaxCnt;
	int				m_nCurrentCnt;
	list_head		m_sLruList;
	list_head		m_sHash[CACHE_MAX_HASH_SIZE];
};

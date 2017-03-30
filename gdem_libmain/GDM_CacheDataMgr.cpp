#include <string.h>
#include "GDM_Mem.h"
#include "GDM_Define.h"
#include "GDM_CacheMgr.h"
#include "GDM_CacheDataMgr.h"



#define CACHE_ENTRY_STATIC		0x0001

#define CACHE_ENTRY_IN_MEM		0x0010
#define CACHE_ENTRY_IN_HARD		0x0020
#define CACHE_ENTRY_DEFAULT_COUNT	100

typedef struct GDM_CACHE_ENTRY
{
	int			flag;		/* entry's state */
	int			key1, key2; /* search key */
	void*		pData;		/* pointer to the data */
	list_head	hash_item;	
	list_head	lru_item;
} GDM_CACHE_ENTRY;

static inline unsigned long
_cache_tile_hash (int key1, int key2)
{
	return (key1 | key2) & 0x3f;
}


CGDM_CacheDataMgr::CGDM_CacheDataMgr(int type, int nCnt)
{
	Init();
	m_nType		= type;
	m_nMaxCnt	= nCnt;
}

CGDM_CacheDataMgr::CGDM_CacheDataMgr(int nCnt)
{
	Init();
	m_nMaxCnt	= nCnt;
}

CGDM_CacheDataMgr::CGDM_CacheDataMgr()
{
	Init();
}

void CGDM_CacheDataMgr::Init()
{
	int i;

	m_nCurrentCnt = 0;
	m_nType		= 0;
	m_nMaxCnt	= CACHE_ENTRY_DEFAULT_COUNT;
	m_pParent	= NULL;

	/* Initialize hash table */
	for (i = 0; i < CACHE_MAX_HASH_SIZE; i++)
		INIT_LIST_HEAD (&m_sHash[i]);

	/* Initialize lru list */
	INIT_LIST_HEAD(&m_sLruList);
}

CGDM_CacheDataMgr::~CGDM_CacheDataMgr()
{
	Clear();
}

int CGDM_CacheDataMgr::AddEntry(int key1, int key2, void *buf)
{
	GDM_CACHE_ENTRY	*pEntry;
	list_head		*hlist;

	pEntry = (GDM_CACHE_ENTRY *)gdmMemMalloc(sizeof(GDM_CACHE_ENTRY));
	if (pEntry == NULL)
		return FALSE;

	memset(pEntry, 0, sizeof(GDM_CACHE_ENTRY));
	pEntry->flag	= 0;
	pEntry->key1	= key1;
	pEntry->key2	= key2;
	pEntry->pData	= buf;

	if (key1 == 0 && key2 == 0)
		pEntry->flag |= CACHE_ENTRY_STATIC;
	else
		pEntry->flag &= ~CACHE_ENTRY_STATIC;

	if (m_nCurrentCnt >= m_nMaxCnt)
		CleanUp();

	/* Add to hash table */
	hlist = &m_sHash[_cache_tile_hash(key1, key2)];	
	list_add(&pEntry->hash_item, hlist);

	/* Add to LRU list */
	if (!(pEntry->flag & CACHE_ENTRY_STATIC))
		list_add_tail(&pEntry->lru_item, &m_sLruList); 

	m_nCurrentCnt++;

	return TRUE;
}

void* CGDM_CacheDataMgr::GetEntry(int key1, int key2)
{
	GDM_CACHE_ENTRY	*pEntry;
	list_head		*hlist;	/* hash list */
	list_head		*hitem;

	hlist = &m_sHash[_cache_tile_hash(key1, key2)];	

	list_for_each(hitem, hlist)
	{
		pEntry = list_entry(hitem, GDM_CACHE_ENTRY, hash_item);
		if (pEntry->key1 == key1 && pEntry->key2 == key2)
		{
			if (list_item_is_valid (&pEntry->lru_item))
				LRUFixup(pEntry);
			return pEntry->pData;
		}
	}
	return NULL;
}

// Function name   : CGDM_CacheDataMgr::DeleteEntry
// Description     : This function is called only from CleanUp().
// Return type     : void 
// Argument        : void *entry

void CGDM_CacheDataMgr::DeleteEntry(void *entry)
{
	GDM_CACHE_ENTRY	*pEntry = NULL;
	CACHE_CB_DATA	data;
	
	if (entry == NULL)
		return;


	pEntry = (GDM_CACHE_ENTRY*) entry;

	data.key1	= pEntry->key1;
	data.key2	= pEntry->key2;
	data.pData	= pEntry->pData;

	/* Send msg that tile is deleting */
	if(m_pParent)
	{
		if (!m_pParent->ProcessCbMsg(m_nType, &data, E_CACHE_CB_DEL_BEFORE))
			return;		/* Tile is still used */
	}

	/* delete from hast table */
	list_del(&pEntry->hash_item);

	/* delete from LRU list */
	list_del(&pEntry->lru_item);

	/* delete entry */
	if (pEntry->pData != NULL)
		FreeData(pEntry->pData);

	gdmMemFree(pEntry);

	data.pData = NULL;

	/* Send msg that tile is deleted */
	if(m_pParent)
	{
		m_pParent->ProcessCbMsg(m_nType, &data, E_CACHE_CB_DEL_AFTER);
	}

	m_nCurrentCnt--;
}

void CGDM_CacheDataMgr::FreeData(void *pData)
{
	gdmMemFree(pData);
}

void CGDM_CacheDataMgr::CleanUp()
{
	list_head			*cur_item  = NULL;
	list_head			*next_item = NULL;
	GDM_CACHE_ENTRY		*pEntry = NULL;

	next_item = m_sLruList.next;

	while (m_nCurrentCnt > (8 * m_nMaxCnt)/10
		&& !list_empty(&m_sLruList))
	{
		cur_item  = next_item;
		next_item = cur_item->next;
		if (cur_item != NULL)
		{
			pEntry = list_entry(cur_item, GDM_CACHE_ENTRY, lru_item);
			DeleteEntry(pEntry);
		}
	}
	return;
}

void CGDM_CacheDataMgr::Clear(bool blDestroy)
{
	GDM_CACHE_ENTRY*	pEntry;
	list_head			*cur_item  = NULL;
	list_head			*next_item = NULL;
	int i;

	m_nCurrentCnt = 0;

	for (i = 0; i < CACHE_MAX_HASH_SIZE; i++)
	{
		next_item = m_sHash[i].next;

		while (!list_empty(&m_sHash[i]) && (next_item != &m_sHash[i]))
		{
			cur_item  = next_item;
			next_item = cur_item->next;
			if (cur_item != NULL)
			{
				/* Get cache entry */
				pEntry = list_entry (cur_item, GDM_CACHE_ENTRY, hash_item);

				if (!blDestroy && (pEntry->flag & CACHE_ENTRY_STATIC)) // 0 level tile will be never deleted.
				{
					m_nCurrentCnt++;
				}
				else
				{
					/* Remove entry from hash and LRU list */
					list_del(&pEntry->hash_item);
					if (list_item_is_valid(&pEntry->lru_item))
						list_del(&pEntry->lru_item);

					/* Destroy entry */
					if (pEntry->pData)
						FreeData(pEntry->pData);
					gdmMemFree(pEntry);
				}
			}
		}
	}

}

void CGDM_CacheDataMgr::LRUFixup(void *entry)
{
	GDM_CACHE_ENTRY	*pEntry;

	if (entry == NULL)
		return;

	pEntry = (GDM_CACHE_ENTRY*) entry;

	/* move item to end in LRU list */
	list_move_tail (&pEntry->lru_item, &m_sLruList);
}

void CGDM_CacheDataMgr::RemoveAll()
{
	Clear(false);
}

void* CGDM_CacheDataMgr::GetCacheEntry(int key1,int key2)
{
	GDM_CACHE_ENTRY	*pEntry;
	list_head		*hlist;	/* hash list */
	list_head		*hitem;

	hlist = &m_sHash[_cache_tile_hash(key1, key2)];	

	list_for_each(hitem, hlist)
	{
		pEntry = list_entry(hitem, GDM_CACHE_ENTRY, hash_item);
		if (pEntry->key1 == key1 && pEntry->key2 == key2)
		{
			return pEntry;
		}
	}
	return NULL;
}
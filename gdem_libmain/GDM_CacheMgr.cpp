#include "GDM_Mem.h"
#include "GDM_Define.h"
#include "GDM_Tile.h"
#include "GDM_CacheMgr.h"
#include "GDM_CacheDataMgr.h"
#include "GDM_DataMgr.h"

static int g_cache_max_entry_count[MAX_CACHE_GEODATA_TYPE] = {
        100,	/* IMG */
        20,		/* DEM */
        60,		/* Name*/
        60,		/* shape*/
        20,		/* 3D */
		100,    /*raster*/
};

#define		MEM_CACHE_SIZE_DEFAULT		300
#define		MEM_CACHE_SIZE_MIN			20

QMutex g_CacheMgrMutex;

CGDM_CacheMgr::CGDM_CacheMgr()
{
	int i;

	m_pItemMgr = (CGDM_CacheDataMgr **)gdmMemMalloc(sizeof(CGDM_CacheDataMgr*) * MAX_CACHE_GEODATA_TYPE);

	for (i = 0; i < MAX_CACHE_GEODATA_TYPE; i++)
	{
		m_pItemMgr[i] = new CGDM_CacheDataMgr(i, g_cache_max_entry_count[i]);
		if (m_pItemMgr[i] != NULL)
			m_pItemMgr[i]->SetParent(this);
	}

	SetMaxSize(MEM_CACHE_SIZE_DEFAULT);

	/* Set Cb func */
	m_pCbFunc = gdm_CacheCallbackFunc;
}

CGDM_CacheDataMgr* CGDM_CacheMgr::GetCacheMgr(E_GDM_MAINDATA_TYPE type)
{
	return m_pItemMgr[(int)type];
}
CGDM_CacheMgr::~CGDM_CacheMgr()
{
	int i;

	for (i = 0; i < MAX_CACHE_GEODATA_TYPE; i++)
	{
		if (m_pItemMgr[i] != NULL)
			delete m_pItemMgr[i];
	}

	gdmMemFree(m_pItemMgr);
}

int CGDM_CacheMgr::AddEntry(int type, int key1, int key2, void *buf)
{
	int pos = type;
	int ret = FALSE;
	GDM_TILE *pTile = (GDM_TILE*) buf;

	if (pos >= MAX_CACHE_GEODATA_TYPE)
	{
		return ret; /* Invalid data type */
	}

	if (m_pItemMgr[pos] != NULL)
	{
		ret = m_pItemMgr[pos]->AddEntry(key1, key2, buf);
	}

	return ret;
}

void* CGDM_CacheMgr::GetEntry(int type, int key1, int key2)
{
	int pos = type;
	void* retPtr = NULL;

	if (pos >= MAX_CACHE_GEODATA_TYPE)
	{
		return NULL; /* Invalid data type */
	}

	if (m_pItemMgr[pos] != NULL)
		retPtr = m_pItemMgr[pos]->GetEntry(key1, key2);
	return retPtr;
}

int CGDM_CacheMgr::IsEntryExist(int type, int key1, int key2)
{
	void *pbuf = NULL;

	pbuf = GetEntry(type, key1, key2);

	if (pbuf == NULL)
		return FALSE;

	return TRUE;
}

int CGDM_CacheMgr::SetCbFunc(GDM_CacheCallBackFunc *pFunc)
{
	if (pFunc == NULL)
		return FALSE;
	
	m_pCbFunc = pFunc;
	return TRUE;
}

// Function name   : CGDM_CacheMgr::ProcessCbMsg
// Description     : 
// Return type     : int 
// Argument        : int type
// Argument        : int key1
// Argument        : int key2
// Argument        : E_CACHE_CB_FLAG flag

int CGDM_CacheMgr::ProcessCbMsg(int type, CACHE_CB_DATA* data, E_CACHE_CB_FLAG flag)
{
	if (m_pCbFunc != NULL)
		return m_pCbFunc(type, data, flag);

	return FALSE;
}

void CGDM_CacheMgr::Clear()
{
	int pos;

	gdm_ClearPrepareRenderData();

	for (pos = 0; pos < MAX_CACHE_GEODATA_TYPE; pos++)	
		if (m_pItemMgr[pos])
			m_pItemMgr[pos]->RemoveAll();

}

void CGDM_CacheMgr::SetMaxSize(int nSize)
{
	int pos, nCnt;

	if (nSize == m_nSize)
		return;

	if (nSize < MEM_CACHE_SIZE_MIN)
		nSize = MEM_CACHE_SIZE_MIN;

	for (pos = 0; pos < MAX_CACHE_GEODATA_TYPE; pos++)
	{
		nCnt = (g_cache_max_entry_count[pos] * nSize) / MEM_CACHE_SIZE_MIN;
		if (m_pItemMgr[pos])
			m_pItemMgr[pos]->SetMaxEntryCnt(nCnt);
	}

	m_nSize = nSize;
}

int gdm_CacheCallbackFunc(int type, CACHE_CB_DATA* data, E_CACHE_CB_FLAG flag)
{
        GDM_TEXTURE_TILE*		pTexture = NULL;

        switch (flag)
        {
        case E_CACHE_CB_DEL_BEFORE:
                {
#if 0
					if (type == E_GDM_SUBDATA_TYPE_IMG)
                        {
                                pTexture = (GDM_TEXTURE_TILE*)data->pData;
                                if (pTexture != NULL)
                                {
                                        gdm_GetTextureMgr()->DelTexture(pTexture->m_nID);
                                }
                        }
#endif
                }
                break;
        case E_CACHE_CB_DEL_AFTER:
                break;
        default:
                return FALSE;
        }

        return TRUE;
}

int	CGDM_CacheMgr::AddEntry(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY, void* buf)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = gdm_GetMainType(type);

	return AddEntry(mainType, key1, key2, buf);
}
void* CGDM_CacheMgr::GetEntry(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = gdm_GetMainType(type);

	return GetEntry(mainType, key1, key2);
}

void CGDM_CacheMgr::DeleteEntry(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = gdm_GetMainType(type);

	return DeleteEntry(mainType, key1, key2);
}

void CGDM_CacheMgr::DeleteEntry(int type, int key1, int key2)
{
	int pos = type;
	void* retPtr = NULL;

	if (pos >= MAX_CACHE_GEODATA_TYPE)
	{
		return ; /* Invalid data type */
	}

	if (m_pItemMgr[pos] != NULL)
	{
		void * data= m_pItemMgr[pos]->GetCacheEntry(key1,key2);
		 m_pItemMgr[pos]->DeleteEntry(data);
	}
}
	
int	CGDM_CacheMgr::IsEntryExist(E_GDM_SUBDATA_TYPE type, int nLevel, int nX, int nY)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = gdm_GetMainType(type);

	return IsEntryExist(mainType, key1, key2);
}

void CGDM_CacheMgr::ImageCacheClear()
{
	g_CacheMgrMutex.lock();
	m_pItemMgr[E_GDM_MAINDATA_TYPE_IMG]->RemoveAll();
	g_CacheMgrMutex.unlock();
}

void CGDM_CacheMgr::RasterCacheClear()
{
	m_pItemMgr[E_GDM_MAINDATA_TYPE_RASTER]->RemoveAll();
}

void CGDM_CacheMgr::NameCacheClear()
{
	m_pItemMgr[E_GDM_MAINDATA_TYPE_NAME]->RemoveAll();
}
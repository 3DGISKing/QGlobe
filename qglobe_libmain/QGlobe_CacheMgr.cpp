#include "QGlobe_Mem.h"
#include "QGlobe_Define.h"
#include "QGlobe_Tile.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_CacheDataMgr.h"
#include "QGlobe_DataMgr.h"

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

CQGlobe_CacheMgr::CQGlobe_CacheMgr()
{
	int i;

	m_pItemMgr = (CQGlobe_CacheDataMgr **)gdmMemMalloc(sizeof(CQGlobe_CacheDataMgr*) * MAX_CACHE_GEODATA_TYPE);

	for (i = 0; i < MAX_CACHE_GEODATA_TYPE; i++)
	{
		m_pItemMgr[i] = new CQGlobe_CacheDataMgr(i, g_cache_max_entry_count[i]);
		if (m_pItemMgr[i] != NULL)
			m_pItemMgr[i]->SetParent(this);
	}

	SetMaxSize(MEM_CACHE_SIZE_DEFAULT);

	/* Set Cb func */
	m_pCbFunc = qglobe_CacheCallbackFunc;
}

CQGlobe_CacheDataMgr* CQGlobe_CacheMgr::GetCacheMgr(E_QGlobe_MAINDATA_TYPE type)
{
	return m_pItemMgr[(int)type];
}
CQGlobe_CacheMgr::~CQGlobe_CacheMgr()
{
	int i;

	for (i = 0; i < MAX_CACHE_GEODATA_TYPE; i++)
	{
		if (m_pItemMgr[i] != NULL)
			delete m_pItemMgr[i];
	}

	gdmMemFree(m_pItemMgr);
}

int CQGlobe_CacheMgr::AddEntry(int type, int key1, int key2, void *buf)
{
	int pos = type;
	int ret = FALSE;
	QGlobe_TILE *pTile = (QGlobe_TILE*) buf;

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

void* CQGlobe_CacheMgr::GetEntry(int type, int key1, int key2)
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

int CQGlobe_CacheMgr::IsEntryExist(int type, int key1, int key2)
{
	void *pbuf = NULL;

	pbuf = GetEntry(type, key1, key2);

	if (pbuf == NULL)
		return FALSE;

	return TRUE;
}

int CQGlobe_CacheMgr::SetCbFunc(QGlobe_CacheCallBackFunc *pFunc)
{
	if (pFunc == NULL)
		return FALSE;
	
	m_pCbFunc = pFunc;
	return TRUE;
}

// Function name   : CQGlobe_CacheMgr::ProcessCbMsg
// Description     : 
// Return type     : int 
// Argument        : int type
// Argument        : int key1
// Argument        : int key2
// Argument        : E_CACHE_CB_FLAG flag

int CQGlobe_CacheMgr::ProcessCbMsg(int type, CACHE_CB_DATA* data, E_CACHE_CB_FLAG flag)
{
	if (m_pCbFunc != NULL)
		return m_pCbFunc(type, data, flag);

	return FALSE;
}

void CQGlobe_CacheMgr::Clear()
{
	int pos;

	qglobe_ClearPrepareRenderData();

	for (pos = 0; pos < MAX_CACHE_GEODATA_TYPE; pos++)	
		if (m_pItemMgr[pos])
			m_pItemMgr[pos]->RemoveAll();

}

void CQGlobe_CacheMgr::SetMaxSize(int nSize)
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

int qglobe_CacheCallbackFunc(int type, CACHE_CB_DATA* data, E_CACHE_CB_FLAG flag)
{
        QGlobe_TEXTURE_TILE*		pTexture = NULL;

        switch (flag)
        {
        case E_CACHE_CB_DEL_BEFORE:
                {
#if 0
					if (type == E_QGlobe_SUBDATA_TYPE_IMG)
                        {
                                pTexture = (QGlobe_TEXTURE_TILE*)data->pData;
                                if (pTexture != NULL)
                                {
                                        qglobe_GetTextureMgr()->DelTexture(pTexture->m_nID);
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

int	CQGlobe_CacheMgr::AddEntry(E_QGlobe_SUBDATA_TYPE type, int nLevel, int nX, int nY, void* buf)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = qglobe_GetMainType(type);

	return AddEntry(mainType, key1, key2, buf);
}
void* CQGlobe_CacheMgr::GetEntry(E_QGlobe_SUBDATA_TYPE type, int nLevel, int nX, int nY)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = qglobe_GetMainType(type);

	return GetEntry(mainType, key1, key2);
}

void CQGlobe_CacheMgr::DeleteEntry(E_QGlobe_SUBDATA_TYPE type, int nLevel, int nX, int nY)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = qglobe_GetMainType(type);

	return DeleteEntry(mainType, key1, key2);
}

void CQGlobe_CacheMgr::DeleteEntry(int type, int key1, int key2)
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
	
int	CQGlobe_CacheMgr::IsEntryExist(E_QGlobe_SUBDATA_TYPE type, int nLevel, int nX, int nY)
{
	int key1 = MAKE_CACHE_KEY1(nLevel, nX, nY);
	int key2 = MAKE_CACHE_KEY2(type, nX, nY);
	int mainType = qglobe_GetMainType(type);

	return IsEntryExist(mainType, key1, key2);
}

void CQGlobe_CacheMgr::ImageCacheClear()
{
	g_CacheMgrMutex.lock();
	m_pItemMgr[E_QGlobe_MAINDATA_TYPE_IMG]->RemoveAll();
	g_CacheMgrMutex.unlock();
}

void CQGlobe_CacheMgr::RasterCacheClear()
{
	m_pItemMgr[E_QGlobe_MAINDATA_TYPE_RASTER]->RemoveAll();
}

void CQGlobe_CacheMgr::NameCacheClear()
{
	m_pItemMgr[E_QGlobe_MAINDATA_TYPE_NAME]->RemoveAll();
}
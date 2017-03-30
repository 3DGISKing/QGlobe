#include <QDesktopServices>
#include <QDir>
#include <QString>
#include <stdio.h>
#include "common.h"
#include "../gdem_common/gdem_protocol.h"
#include "GDM_Mem.h"
#include "GDM_FileCacheMgr.h"
#include "gdem_version.h"


#define MAX_CACHE_FILE_SIZE		(300<<20)

#define CACHE_FILE_IDLE			1
#define CACHE_FILE_BUSY			2

#define CACHE_FILE_HEADER_SIZE	16
#define CACHE_FILE_HEADER_STRING_SIZE	(16-4)

#define MAX_MAIN_TYPE_OFFSET	(MAX_TILE_LEVEL + 5)


static char g_cacheDataHeader[CACHE_FILE_HEADER_STRING_SIZE] = {
	0x50, 0x49, 0x43, 0x45, 0x41, 0x52, 0x54, 0x48, 0x00, 0x00, 0x00, 0x00}; // string header
	// 4: version number

static int _index_compare (void *a, void *b)
{
	CACHE_FILE_INDEX	*la = (CACHE_FILE_INDEX*) a;
	CACHE_FILE_INDEX	*lb = (CACHE_FILE_INDEX*) b;

	if (la->key1 > lb->key1)
		return -1;
	else if (la->key1 < lb->key1)
		return 1;

	if (la->key2 > lb->key2)
		return -1;
	else if (la->key2 < lb->key2)
		return 1;

	return 0;
}

static int _clear_entry (void *data, void *aux)
{
	CACHE_FILE_INDEX* entry = (CACHE_FILE_INDEX*) data;

	if (entry)
		gdmMemFree(entry);

	return 0;
}

static void _generate_key (GDM_TILE_INFO* pInfo, int *key1, int* key2)
{
	*key1 = ((pInfo->eMainType << MAX_MAIN_TYPE_OFFSET) | (pInfo->nLevel << MAX_TILE_LEVEL) | pInfo->nX);
	*key2 = ((pInfo->eSubType << MAX_TILE_LEVEL) | pInfo->nY);
}

#if 0
static int _calc_check_sum (char* lpBuf, int nSize)
{
	int sum = 0, r = 55655, c1 = 52845, c2 = 22719;
	char cipher;

	for (int i = 0; i < nSize; i++)
	{
		cipher = (lpBuf[i] ^ (r >> 8));
		r = (cipher + r) * c1 + c2;
		sum += cipher;
	}

	return sum;
}
#endif

CGDM_CacheIndexTree::CGDM_CacheIndexTree()
{
	m_pTree = avl_Create(_index_compare, 0);
	m_nFlag = CACHE_FILE_IDLE;
}

CGDM_CacheIndexTree::~CGDM_CacheIndexTree()
{
	if (m_pTree)
		avl_Destroy (m_pTree, _clear_entry, NULL);
}

int CGDM_CacheIndexTree::AddIndex(CACHE_FILE_INDEX* entry)
{
	int ret;

	if (entry == NULL || m_pTree == NULL || m_nFlag == CACHE_FILE_BUSY)
		return 0;

	m_nFlag = CACHE_FILE_BUSY;
	if (avl_Insert(m_pTree, entry, NULL) == NULL)
		ret = 0;
	else
		ret = 1;
	m_nFlag = CACHE_FILE_IDLE;

	return ret;
}

int CGDM_CacheIndexTree::GetIndex(CACHE_FILE_INDEX* entry)
{
	CACHE_FILE_INDEX* pData = NULL;
	int ret;

	if (entry == NULL || m_pTree == NULL || m_nFlag == CACHE_FILE_BUSY)
		return 0;
	
	m_nFlag = CACHE_FILE_BUSY;

	if (NULL == (pData = (CACHE_FILE_INDEX*)avl_Find(m_pTree, entry, COND_EQ, NULL)))	
		ret = 0;
	else
	{
		memcpy(entry, pData, sizeof(CACHE_FILE_INDEX));
		ret = 1;
	}

	m_nFlag = CACHE_FILE_IDLE;
	return ret;
}

void CGDM_CacheIndexTree::RemoveAll()
{
	while (m_nFlag != CACHE_FILE_IDLE)
	{
		int i = 0;
	}

	m_nFlag = CACHE_FILE_BUSY;
	if (m_pTree)
		avl_Destroy (m_pTree, _clear_entry, NULL);

	m_pTree = avl_Create(_index_compare, 0);
	m_nFlag = CACHE_FILE_IDLE;
}

/****************************************************************
 *						CGDM_FileCacheMgr						*
 ****************************************************************/


CGDM_FileCacheMgr::CGDM_FileCacheMgr()
{
	QString tmpPath;

	/* Create index tree for each data type */
	m_pIndices = (CGDM_CacheIndexTree**)gdmMemMalloc(sizeof(CGDM_CacheIndexTree*) * E_GDM_MAINDATA_TYPE_MAX);
	if (m_pIndices != NULL)
	{
		for (int i = 0; i < E_GDM_MAINDATA_TYPE_MAX; i++)
			m_pIndices[i] = new CGDM_CacheIndexTree();
	}

	/* Initialize member variables */
	memset (m_szDataFilePath, 0x00, sizeof(m_szDataFilePath));
	m_fpCacheData	= NULL;
	m_nState		= CACHE_FILE_IDLE;
	m_nMaxSize		= MAX_CACHE_FILE_SIZE;
	m_nCurSize		= 0;

	/* Open cache data file */
	tmpPath = QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/dbCache.dat");	
	OpenCacheDataFile(tmpPath.toUtf8().constData());
}

CGDM_FileCacheMgr::~CGDM_FileCacheMgr()
{
	CGDM_CacheIndexTree *pIndex = NULL;

	if (m_pIndices)
	{
		for (int i = 0; i < E_GDM_MAINDATA_TYPE_MAX; i++)
		{
			pIndex = m_pIndices[i];
			m_pIndices[i] = NULL;
			if (pIndex)
				delete pIndex;
		}
	}

	gdmMemFree(m_pIndices);
	if (m_fpCacheData)
		fclose(m_fpCacheData);
}

 /*
 * Description : Open cache data file. This function must be called only once.		
 *  
 * IN         : char *szPathname - application data location path
 */

void CGDM_FileCacheMgr::OpenCacheDataFile(const char *szPathname)
{
	if (szPathname == NULL)
		return;

	strcpy(m_szDataFilePath, szPathname);

	m_fpCacheData = fopen(m_szDataFilePath, "rb");
	if (m_fpCacheData)
	{
		LoadStoredData();
	}
	else
	{
		Initialize();
	}
}

/*
 * Description : This function is called when cache file not exists or is invalid.
 *				Create cache file and Write cache file header.  
 */

void CGDM_FileCacheMgr::Initialize()
{
	if (m_fpCacheData)
	{
		fclose(m_fpCacheData);
		remove(m_szDataFilePath);
	}

	m_fpCacheData = fopen(m_szDataFilePath, "w+b");
	if (m_fpCacheData)
	{
		m_nCurSize = fwrite(g_cacheDataHeader, sizeof(char), CACHE_FILE_HEADER_STRING_SIZE, m_fpCacheData);

		// write version number
		int nVersion = GetMainVersion();
		fwrite(&nVersion, sizeof(int), 1, m_fpCacheData);
		m_nCurSize += sizeof(int);
	}
}

/*
 * Description : This function is called when cache file exists.
 *				Build index tree from cache file.  
 */

void CGDM_FileCacheMgr::LoadStoredData()
{
	CACHE_FILE_INDEX* entry;
	E_GDM_MAINDATA_TYPE  eMainType;
	int	pos;

	if (m_fpCacheData == NULL)
		return;

	/*Check whether cache file is valid or not.*/
	if (!IsValidCacheFile())
	{
		Initialize();
		return;
	}

	/* Read previous stored data */
	fseek(m_fpCacheData, CACHE_FILE_HEADER_SIZE, SEEK_SET);
	while (!feof(m_fpCacheData))
	{
		entry = (CACHE_FILE_INDEX*)gdmMemMalloc(sizeof(CACHE_FILE_INDEX));
		if (entry == NULL)
			break;

		if (fread (entry, sizeof(CACHE_FILE_INDEX), 1, m_fpCacheData) != 1)
		{
			gdmMemFree(entry);
			break;
		}

		pos = ftell(m_fpCacheData);

		eMainType = (E_GDM_MAINDATA_TYPE) (entry->key1 >> MAX_MAIN_TYPE_OFFSET);
		if (pos == entry->offset)
		{
			if (m_pIndices[eMainType] != NULL)
			{
				m_pIndices[eMainType]->AddIndex(entry);
			}
		}
		if (fseek (m_fpCacheData, entry->datasize, SEEK_CUR))
			break;
	}
	fclose(m_fpCacheData);

	/* Reopen file in append mode. */
	m_fpCacheData = fopen(m_szDataFilePath, "a+b");
	if (m_fpCacheData)
	{
		fseek(m_fpCacheData, 0, SEEK_END);
		m_nCurSize = ftell(m_fpCacheData);
	}
}

/*
 * Description : Check cache file.
 *  
 */

int CGDM_FileCacheMgr::IsValidCacheFile()
{
	char buf[CACHE_FILE_HEADER_STRING_SIZE];
	int	 nVersion;

	if (fread(buf, sizeof(char), CACHE_FILE_HEADER_STRING_SIZE, m_fpCacheData) != CACHE_FILE_HEADER_STRING_SIZE)
		return 0;

	if (memcmp(g_cacheDataHeader, buf, CACHE_FILE_HEADER_STRING_SIZE))
		return 0;

	// read version number
	fread(&nVersion, sizeof(int), 1, m_fpCacheData);
	E_VERSION_MISMATCH_TYPE tResult = CheckMainVersion(nVersion);
	if(tResult > E_VERSION_MISMATCH_EXTRA)
		return 0;

	return 1;
}

/*
 * Description : This function is called when user selects [Option]/[Delete cache file].
 *				Action - 1. Remove all indexing data.
 *						 2. Delete and recreate cache file 
 */

void CGDM_FileCacheMgr::Clear()
{
	CGDM_CacheIndexTree *pIndex = NULL;

	/* Wait till other thread is finished */
	while(m_nState != CACHE_FILE_IDLE)
	{
		int i = 0;
	}

	m_nState = CACHE_FILE_BUSY;

	/* Remove all indexing data */
	if (m_pIndices)
	{
		for (int i = 0; i < E_GDM_MAINDATA_TYPE_MAX; i++)
		{
			pIndex = m_pIndices[i];
			m_pIndices[i] = NULL;
			if (pIndex)
				pIndex->RemoveAll();
			m_pIndices[i] = pIndex;
		}
	}

	/* Delete and recreate cache file */
	Initialize();

	m_nState = CACHE_FILE_IDLE;
}

void CGDM_FileCacheMgr::WriteTileBuffer(GDM_TILE_INFO *pInfo, void *buffer)
{
	CACHE_FILE_INDEX	entry;
	CACHE_FILE_INDEX	*pentry = NULL;	
	ST_PACKET_HEADER*	pHdr = NULL;
	int					datasize;

	if (pInfo == NULL || buffer == NULL || m_fpCacheData == NULL)
		return;	/* fail */

	if (pInfo->eMainType >= E_GDM_MAINDATA_TYPE_MAX || pInfo->eMainType < 0)
		return; /* data invalid */

	_generate_key(pInfo, &entry.key1, &entry.key2);

	if ((m_pIndices[pInfo->eMainType] == NULL) || (m_pIndices[pInfo->eMainType]->GetIndex(&entry)))
		return;	/* already exist */

	if (m_nCurSize >= m_nMaxSize)
		return;	/* Exceeds file size limit */

	// Get data size
	pHdr = (ST_PACKET_HEADER*)buffer;
	datasize = pHdr->bytes;

	// create and set index entry
	pentry = (CACHE_FILE_INDEX*)gdmMemMalloc(sizeof(CACHE_FILE_INDEX));
	if (pentry == NULL)
		return;
	memcpy(pentry, &entry, sizeof(CACHE_FILE_INDEX));
	pentry->datasize = datasize;

	if (WriteData(pentry, buffer, datasize))
	{
		m_pIndices[pInfo->eMainType]->AddIndex(pentry);	// success
#ifdef DEBUG
		qDebug("Saving Data(type[%d] row[%d] col[%d] level[%d]): offset=%d size=%d \n",
			pInfo->eMainType, pInfo->nX, pInfo->nY, pInfo->nLevel, m_nCurSize, datasize);
#endif
	}
	else
		gdmMemFree(pentry); // fail
}

int CGDM_FileCacheMgr::ReadTileBuffer(GDM_TILE_INFO* pInfo, void* buffer)
{
	CACHE_FILE_INDEX	entry;

	if (pInfo == NULL || buffer == NULL || m_fpCacheData == NULL)
		return RESPONSE_GET_TILE_ERROR_MEMORY;	/* fail */

	if (pInfo->eMainType >= E_GDM_MAINDATA_TYPE_MAX || pInfo->eMainType < 0)
		return RESPONSE_GET_TILE_ERROR_MEMORY; /* Invalid data request */

	/* Check whether data exists or not */
	_generate_key(pInfo, &entry.key1, &entry.key2);

	if ((m_pIndices[pInfo->eMainType] == NULL) || (!m_pIndices[pInfo->eMainType]->GetIndex(&entry)))
	{
#ifdef DEBUG
		qDebug("!!!Failed to Read Data(type[%d] row[%d] col[%d] level[%d])\n",
	pInfo->eSubType, pInfo->nX, pInfo->nY, pInfo->nLevel);
#endif
		return RESPONSE_GET_TILE_ERROR_MEMORY; /* Index tree is not prepared */
	}
	
	/* Check buffer size */
	if (entry.datasize > pInfo->nSize)
		return RESPONSE_GET_TILE_ERROR_MEMORY; /* not sufficient buffer size */

	if (ReadData(entry.offset, entry.datasize, buffer))
	{
#ifdef DEBUG
		qDebug("Reading Data(type[%d] row[%d] col[%d] level[%d]): offset=%d size=%d \n",
			pInfo->eMainType, pInfo->nX, pInfo->nY, pInfo->nLevel, entry.offset, entry.datasize);
#endif
		return RESPONSE_GET_TILE_ERROR_NO;
	}
	else
	{
#ifdef DEBUG
		qDebug("!!!Failed to Read Data(type[%d] row[%d] col[%d] level[%d]): offset=%d size=%d \n",
			pInfo->eMainType, pInfo->nX, pInfo->nY, pInfo->nLevel, entry.offset, entry.datasize);
#endif
	}


	return RESPONSE_GET_TILE_ERROR_MEMORY;
}

/*
 * Description : Read data from cache data file
 *  
 * IN          : int offset
 * IN          : int size
 * OUT         : void* buffer
 */
int CGDM_FileCacheMgr::ReadData(int offset, int size, void* buffer)
{
	int nRet;

	if (m_nState == CACHE_FILE_BUSY)
		return 0;

	m_nState = CACHE_FILE_BUSY;

	if (fseek(m_fpCacheData, offset, SEEK_SET))
	{
		m_nState = CACHE_FILE_IDLE;
		return 0;
	}

	nRet = fread(buffer, sizeof(char), size, m_fpCacheData);

	fseek(m_fpCacheData, 0, SEEK_END);

	m_nState = CACHE_FILE_IDLE;

	if (nRet != size)
		return 0;
	return 1;
}

/*
 * Description : Write buffer to cache data file
 *  
 * IN         : void* buffer
 * IN         : int size
 */

int CGDM_FileCacheMgr::WriteData(CACHE_FILE_INDEX* key, void* buffer, int size)
{
	int nRet;
	int nIndexSize = sizeof(CACHE_FILE_INDEX);

	if (m_nState == CACHE_FILE_BUSY)
		return 0;

	m_nState = CACHE_FILE_BUSY;

	key->offset = m_nCurSize + nIndexSize;
	nRet = fwrite(key, sizeof(char), nIndexSize, m_fpCacheData);
	m_nCurSize += nRet;
	if (nRet != nIndexSize)
	{
		m_nState = CACHE_FILE_IDLE;
		return 0;
	}

	nRet = fwrite(buffer, sizeof(char), size, m_fpCacheData);
	m_nCurSize	+= nRet;

	m_nState = CACHE_FILE_IDLE;

	if (nRet != size)
		return 0;
	fflush(m_fpCacheData);
	return 1;
}
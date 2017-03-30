#include "GDSFileManager.h"
#include "../gds_common/GDSLog.h"

#include <QMutex>

QMutex			g_GDSFileManagerLock;

GDSFileManager::GDSFileManager (IN unsigned int	a_nMaxEntryCount,
								IN unsigned int	a_nTimerPeriod)
								: m_nMaxEntryCount(a_nMaxEntryCount)
								, m_nTimerPeriod(a_nTimerPeriod)
{
	m_pFileEntryList = NULL;
}

GDSFileManager::~GDSFileManager()
{
	Finalize();
}

bool GDSFileManager::Initialize()
{
	if (m_pFileEntryList == NULL)
	{
		m_pFileEntryList = new ST_FILEENTRY[m_nMaxEntryCount];
		if (m_pFileEntryList == NULL)
		{
			GDSLogOutput(E_LOG_ERROR, "new ST_FILEENTRY failed at GDSFileManager::Initialize.");
			return false;
		}

	}

	m_MappedHashTable.clear();

	for (int i=0; i<m_nMaxEntryCount; i++)
	{
		m_UnmappedHashTable.insert(i, i);
	}

	m_Timer.start(1, this);

	return true;
}

void GDSFileManager::Finalize()
{
	m_Timer.stop();

	RemoveAllFileMap();

	if (m_pFileEntryList != NULL)
	{
		delete[] m_pFileEntryList;
		m_pFileEntryList = NULL;
	}
}

void GDSFileManager::RemoveAllFileMap()
{
	g_GDSFileManagerLock.lock();

	if (m_MappedHashTable.count() > 0)
	{
		QHash<QString, int>::const_iterator tIterator = m_MappedHashTable.begin();
		while (tIterator != m_MappedHashTable.end())
		{
			RemoveFileMap(tIterator.value());
			tIterator ++;
		}
	}

	m_MappedHashTable.clear();
	m_UnmappedHashTable.clear();

	g_GDSFileManagerLock.unlock();
}

void GDSFileManager::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_Timer.timerId())
	{
		for (int i=0; i<m_nMaxEntryCount; i++)
		{
			if (m_pFileEntryList[i].is_using)
				continue;

			if (m_pFileEntryList[i].timer_count == 0)
				continue;

			m_pFileEntryList[i].timer_count --;
		}
	}
}

QString g_FileNullName("");
QString& GDSFileManager::GetFileName(IN GDSFILEHANDLE a_FileHandle)
{
	if (a_FileHandle == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "File handle is invalid at GDSFileManager::GetFileName.");
		return g_FileNullName;
	}

	return m_pFileEntryList[a_FileHandle].file_name;
}

bool GDSFileManager::OpenFileMap(IN QString& a_FileName, 
								 IN unsigned int a_HeaderSize,
								 OUT GDSFILEHANDLE* a_pFileHandle, 
								 OUT bool* bExist)
{
	*bExist = true;

	g_GDSFileManagerLock.lock();
	bool bFound = GetMappedFileHandle(a_FileName, a_pFileHandle);
	g_GDSFileManagerLock.unlock();

	if (bFound)
		return true;

	g_GDSFileManagerLock.lock();
	bool bResult = CreatFileMap(a_FileName, a_HeaderSize, a_pFileHandle, bExist);
	g_GDSFileManagerLock.unlock();

	return bResult;
}

unsigned int GDSFileManager::GetHeaderPointer(IN GDSFILEHANDLE a_FileHandle,
											  OUT unsigned char** a_ppHeaderBuff)
{
	if (a_FileHandle == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "File handle is invalid at GDSFileManager::GetHeaderPointer.");
		return 0;
	}

	*a_ppHeaderBuff = m_pFileEntryList[a_FileHandle].file_header;
	return m_pFileEntryList[a_FileHandle].header_size;
}

bool GDSFileManager::GetMappedFileHandle(IN QString& a_FileName, 
										 OUT GDSFILEHANDLE* a_pFileHandle)
{
	if (!m_MappedHashTable.contains(a_FileName))
		return false;

	GDSFILEHANDLE tHandle = m_MappedHashTable.value(a_FileName);
	m_pFileEntryList[tHandle].timer_count = m_nTimerPeriod;
	m_pFileEntryList[tHandle].is_using = true;

	*a_pFileHandle = tHandle;
	return true;
}

bool GDSFileManager::CreatFileMap(IN QString& a_FileName, 
								  IN unsigned int a_HeaderSize,
								  OUT GDSFILEHANDLE* a_pFileHandle, 
								  OUT bool* bExist)
{
	if (!QFile::exists(a_FileName))
	{
		*bExist = false;
		return false;
	}

	GDSFILEHANDLE tHandle = INVALID_GDSFILEHANDLE;
	if (m_UnmappedHashTable.size() <= 0)
	{
		if (!RemoveIdleFileMap(&tHandle))
		{
			GDSLogOutput(E_LOG_ERROR, "RemoveIdleFileMap failed at GDSFileManager::CreatFileMap.[file : %s]", a_FileName.toAscii().data());
			return false;
		}
	}
	else
	{
		tHandle = m_UnmappedHashTable.keys().first();
	}

	QFile* newFile = new QFile(a_FileName);
	if (newFile == NULL)
	{
		GDSLogOutput(E_LOG_ERROR, "new QFile failed at GDSFileManager::CreatFileMap.[file : %s]", a_FileName.toAscii().data());
		return false;
	}

	unsigned int tFileSize = newFile->size();

	if (0 == tFileSize)
	{
		delete newFile;
		GDSLogOutput(E_LOG_ERROR, "File size is zero at GDSFileManager::CreatFileMap.[file : %s]", a_FileName.toAscii().data());
		return false;
	}

	if ( !newFile->open(QIODevice::ReadOnly) )
	{
		delete newFile;
		GDSLogOutput(E_LOG_ERROR, "File open failed at GDSFileManager::CreatFileMap.[file : %s]", a_FileName.toAscii().data());
		return false;
	}

	unsigned char* lpData = NULL;
	lpData = newFile->map(0, a_HeaderSize);
	if (lpData == NULL)
	{
		newFile->close();
		delete newFile;
		GDSLogOutput(E_LOG_ERROR, "File mapping failed at GDSFileManager::CreatFileMap.[file : %s]", a_FileName.toAscii().data());
		return false;
	}

	m_pFileEntryList[tHandle].header_size = a_HeaderSize;
	memcpy(m_pFileEntryList[tHandle].file_header, lpData, a_HeaderSize);

	newFile->unmap(lpData);

	m_pFileEntryList[tHandle].handle = tHandle;
	m_pFileEntryList[tHandle].file_pointer = newFile;
	m_pFileEntryList[tHandle].file_size = tFileSize;
	m_pFileEntryList[tHandle].file_name = a_FileName;
	m_pFileEntryList[tHandle].timer_count = m_nTimerPeriod;
	m_pFileEntryList[tHandle].is_using = true;

	m_MappedHashTable.insert(a_FileName, tHandle);
	m_UnmappedHashTable.remove(tHandle);

	*a_pFileHandle = tHandle;
	return true;
}

void GDSFileManager::RemoveFileMap(IN GDSFILEHANDLE a_FileHandle)
{	
	if (a_FileHandle == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "File handle is invalid at GDSFileManager::RemoveFileMap.");
		return;
	}

	QFile* tFile = m_pFileEntryList[a_FileHandle].file_pointer;
	if (tFile != NULL)
	{
		tFile->close();
		delete tFile;
		m_pFileEntryList[a_FileHandle].file_pointer = NULL;
		m_pFileEntryList[a_FileHandle].file_size = 0;
		m_pFileEntryList[a_FileHandle].header_size = 0;
	}

	m_pFileEntryList[a_FileHandle].handle = INVALID_GDSFILEHANDLE;
	m_pFileEntryList[a_FileHandle].timer_count = m_nTimerPeriod;
}


bool GDSFileManager::RemoveIdleFileMap(OUT GDSFILEHANDLE* a_pFileHandle)
{
	if (m_MappedHashTable.count() <= 0)
	{
		GDSLogOutput(E_LOG_ERROR, "Mapped hashtable count is zero at GDSFileManager::RemoveIdleFileMap.");
		return false;
	}

	unsigned int tMinTime = m_nTimerPeriod + 1;
	GDSFILEHANDLE tHanlde = INVALID_GDSFILEHANDLE;
	GDSFILEHANDLE tIdleHanlde = INVALID_GDSFILEHANDLE;
	QHash<QString, int>::const_iterator tIdleIterator;

	QHash<QString, int>::const_iterator tIterator = m_MappedHashTable.begin();
	while (tIterator != m_MappedHashTable.end())
	{
		tHanlde = tIterator.value();
		if (!m_pFileEntryList[tHanlde].is_using)
		{
			if (m_pFileEntryList[tHanlde].timer_count == 0)
			{
				tIdleHanlde = tIterator.value();
				tIdleIterator = tIterator;
				break;
			}

			if (m_pFileEntryList[tHanlde].timer_count < tMinTime)
			{
				tIdleHanlde = tIterator.value();
				tIdleIterator = tIterator;
				tMinTime = m_pFileEntryList[tHanlde].timer_count;
			}
		}

		tIterator ++;
	}

	if (tIdleHanlde == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "Idle handle is not found at GDSFileManager::RemoveIdleFileMap.");
		return false;
	}

	RemoveFileMap(tIdleHanlde);

	m_MappedHashTable.remove(tIdleIterator.key());
	m_UnmappedHashTable.insert(tIdleHanlde, tIdleHanlde);

	*a_pFileHandle = tIdleHanlde;
	return true;
}

bool GDSFileManager::ReadFileMap(IN GDSFILEHANDLE a_FileHandle,
								 IN unsigned int a_Offset,
								 IN unsigned int a_Size,
								 OUT unsigned char* a_pBuff)
{
	if (a_FileHandle == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "File handle is invalid at GDSFileManager::ReadFileMap.");
		return false;
	}

	bool bResult = false;

	g_GDSFileManagerLock.lock();

	QFile* tFile = m_pFileEntryList[a_FileHandle].file_pointer;

	unsigned char* lpData = NULL;
	lpData = tFile->map(a_Offset, a_Size);
	if (lpData != NULL)
	{
		memcpy(a_pBuff, lpData, a_Size);

		tFile->unmap(lpData);

		bResult = true;
	}

	g_GDSFileManagerLock.unlock();

	return bResult;
}

bool GDSFileManager::CheckFileOffsetAndSize(IN GDSFILEHANDLE a_FileHandle,
											IN unsigned int a_Offset,
											IN unsigned int a_Size)
{
	if (a_FileHandle == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "File handle is invalid at GDSFileManager::CheckFileOffsetAndSize.");
		return false;
	}

	return ((a_Offset + a_Size) <= m_pFileEntryList[a_FileHandle].file_size);
}

void GDSFileManager::CloseFileMap(IN GDSFILEHANDLE a_FileHandle)
{
	if (a_FileHandle == INVALID_GDSFILEHANDLE)
	{
		GDSLogOutput(E_LOG_ERROR, "File handle is invalid at GDSFileManager::CloseFileMap.");
		return;
	}

	g_GDSFileManagerLock.lock();

	m_pFileEntryList[a_FileHandle].is_using = false;

	g_GDSFileManagerLock.unlock();
}

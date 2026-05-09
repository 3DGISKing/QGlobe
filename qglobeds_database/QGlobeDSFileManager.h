#ifndef __GDSFILEMANAGER_H__
#define __GDSFILEMANAGER_H__

#include "../qglobeds_common/QGlobeDSType.h"
#include "../qglobeds_common/QGlobeDSCommon.h"

#include <QHash>
#include <QFile>
#include <QBasicTimer>
#include <QTimerEvent>


typedef struct tagFILEENTRY
{
	tagFILEENTRY()
	{
		handle = INVALID_GDSFILEHANDLE;
		is_using = false;
	}

	QGlobeDSFILEHANDLE	handle;
	QString			file_name;			
	QFile*			file_pointer;
	unsigned int	file_size;
	unsigned int	header_size;
	unsigned char	file_header[4+4+8*(TILE_ROW_COUNT_IN_BLOCK*TILE_ROW_COUNT_IN_BLOCK+1)];
	unsigned int	timer_count;
	bool			is_using;
} ST_FILEENTRY;

class QGlobeDSFileManager : QObject
{
public:
	QGlobeDSFileManager (IN unsigned int	a_nMaxEntryCount,
					IN unsigned int	a_nTimerPeriod);

	~QGlobeDSFileManager();

	bool Initialize();
	void Finalize();

	QString& GetFileName(IN QGlobeDSFILEHANDLE a_FileHandle);

	bool OpenFileMap(IN QString& a_FileName, 
					IN unsigned int a_HeaderSize,
					OUT QGlobeDSFILEHANDLE* a_pFileHandle, 
					OUT bool* bExist);

	unsigned int GetHeaderPointer(IN QGlobeDSFILEHANDLE a_FileHandle,
								OUT unsigned char** a_ppHeaderBuff);

	bool CheckFileOffsetAndSize(IN QGlobeDSFILEHANDLE a_FileHandle,
								IN unsigned int a_Offset,
								IN unsigned int a_Size);

	bool ReadFileMap(IN QGlobeDSFILEHANDLE a_FileHandle,
					IN unsigned int a_Offset,
					IN unsigned int a_Size,
					OUT unsigned char* a_pBuff);

	void CloseFileMap(IN QGlobeDSFILEHANDLE a_FileHandle);

private:
	void RemoveAllFileMap();

	bool GetMappedFileHandle(IN QString& a_FileName, 
		OUT QGlobeDSFILEHANDLE* a_pFileHandle);

	bool CreatFileMap(IN QString& a_FileName,
					IN unsigned int a_HeaderSize,
					OUT QGlobeDSFILEHANDLE* a_pFileHandle, 
					OUT bool* bExist);

	void RemoveFileMap(IN QGlobeDSFILEHANDLE a_FileHandle);
	bool RemoveIdleFileMap(OUT QGlobeDSFILEHANDLE* a_pFileHandle);

	ST_FILEENTRY* m_pFileEntryList;
	QHash<QString, int> m_MappedHashTable;
	QHash<int, int> m_UnmappedHashTable;

	unsigned int	m_nMaxEntryCount; // unit : s
	unsigned int	m_nTimerPeriod; // unit : s

	QBasicTimer		m_Timer;
protected:
	virtual void timerEvent(QTimerEvent *event);
};

extern QGlobeDSFileManager* g_FileManager;

#endif // __GDSFILE_H__
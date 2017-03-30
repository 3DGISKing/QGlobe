#ifndef __GDSFILEMANAGER_H__
#define __GDSFILEMANAGER_H__

#include "../gds_common/GDSType.h"
#include "../gds_common/GDSCommon.h"

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

	GDSFILEHANDLE	handle;
	QString			file_name;			
	QFile*			file_pointer;
	unsigned int	file_size;
	unsigned int	header_size;
	unsigned char	file_header[4+4+8*(TILE_ROW_COUNT_IN_BLOCK*TILE_ROW_COUNT_IN_BLOCK+1)];
	unsigned int	timer_count;
	bool			is_using;
} ST_FILEENTRY;

class GDSFileManager : QObject
{
	Q_OBJECT
public:
	GDSFileManager (IN unsigned int	a_nMaxEntryCount,
					IN unsigned int	a_nTimerPeriod);

	~GDSFileManager();

	bool Initialize();
	void Finalize();

	QString& GetFileName(IN GDSFILEHANDLE a_FileHandle);

	bool OpenFileMap(IN QString& a_FileName, 
					IN unsigned int a_HeaderSize,
					OUT GDSFILEHANDLE* a_pFileHandle, 
					OUT bool* bExist);

	unsigned int GetHeaderPointer(IN GDSFILEHANDLE a_FileHandle,
								OUT unsigned char** a_ppHeaderBuff);

	bool CheckFileOffsetAndSize(IN GDSFILEHANDLE a_FileHandle,
								IN unsigned int a_Offset,
								IN unsigned int a_Size);

	bool ReadFileMap(IN GDSFILEHANDLE a_FileHandle,
					IN unsigned int a_Offset,
					IN unsigned int a_Size,
					OUT unsigned char* a_pBuff);

	void CloseFileMap(IN GDSFILEHANDLE a_FileHandle);

private:
	void RemoveAllFileMap();

	bool GetMappedFileHandle(IN QString& a_FileName, 
		OUT GDSFILEHANDLE* a_pFileHandle);

	bool CreatFileMap(IN QString& a_FileName,
					IN unsigned int a_HeaderSize,
					OUT GDSFILEHANDLE* a_pFileHandle, 
					OUT bool* bExist);

	void RemoveFileMap(IN GDSFILEHANDLE a_FileHandle);
	bool RemoveIdleFileMap(OUT GDSFILEHANDLE* a_pFileHandle);

	ST_FILEENTRY* m_pFileEntryList;
	QHash<QString, int> m_MappedHashTable;
	QHash<int, int> m_UnmappedHashTable;

	unsigned int	m_nMaxEntryCount; // unit : s
	unsigned int	m_nTimerPeriod; // unit : s

	QBasicTimer		m_Timer;
protected:
	virtual void timerEvent(QTimerEvent *event);
};

extern GDSFileManager* g_FileManager;

#endif // __GDSFILE_H__
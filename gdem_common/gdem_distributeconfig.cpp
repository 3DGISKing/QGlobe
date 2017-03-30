#include "gdem_distributeconfig.h"
#include "gdem_config.h"

#include <QtCore>
#include <QTextStream>
#include <QStringList>
#include <QFile>

#ifndef TILE_ROW_COUNT_IN_BLOCK
#define TILE_ROW_COUNT_IN_BLOCK		128
#endif // TILE_ROW_COUNT_IN_BLOCK

#define CONF_SET_ORG				"PIC"
#define CONF_SET_APP				"gdemserver"
#define CONF_SET_DISTRIBUTE			"distribute"
#define CONF_DIST_GOU
#define CONF_DIST_TYPE				"type"
#define CONF_DIST_ORGNUM			"distribute_num"
#define CONF_DIST_NUM				"target_num"
#define CONF_DIST_SERVERS			"servers"
#define CONF_DIST_HARDS				"hards"
#define CONF_DIST_SERVER_ADDR		"address"
#define CONF_DIST_SERVER_STARTID	"startid"
#define CONF_DIST_SERVER_ENDID		"endid"
#define CONF_DIST_SERVER_ADDR		"address"
#define CONF_DIST_SERVER_PORT		"port"
#define CONF_DIST_HARD_STARTID		"startid"
#define CONF_DIST_HARD_ENDID		"endid"
#define CONF_DIST_HARD_PATH			"dbpath"

void gs_wchar2ushort(wchar_t *src, ushort *dst, int len)
{
	int i = 0;
	while (src[i] > 0)
	{
		if (len > 0 && i >= len)
			break;
		dst[i] = (ushort)src[i];
		i ++;
	}
	dst[i] = 0;
}

GDEMDistributeConfig g_DistributeConfig;

GDEMDistributeConfig::GDEMDistributeConfig()
{
	Initialize();
}


GDEMDistributeConfig::~GDEMDistributeConfig()
{
	Initialize();
}

void GDEMDistributeConfig::Initialize()
{
	m_DistributeNum = 0;
	m_TargetNum = 0;
	m_DistributeType = E_DISTRIBUTE_TYPE_NONE;
	m_FolderMap.clear();

	//m_DistributeNum = 1024;
	//m_TargetNum = 2;
	//m_DistributeType = E_DISTRIBUTE_TYPE_DIRECTORY;

	//wchar_t temppath[MAX_DIST_PATH];
	//QString tPath;

	//m_FolderMap.clear();

	//ST_DISTRIBUTE_FOLDER tFolder;
	//tFolder.start_index = 0;
	//tFolder.end_index = 643;
	//tPath = "D:/database";
	//tPath.toWCharArray(temppath);
	//gs_wchar2ushort(temppath, tFolder.path, tPath.count());
	//m_FolderMap.append(tFolder);

	//tFolder.start_index = 644;
	//tFolder.end_index = 1023;
	//tPath = "E:/database";
	//tPath.toWCharArray(temppath);
	//gs_wchar2ushort(temppath, tFolder.path, tPath.count());
	//m_FolderMap.append(tFolder);

	m_ServerPCMap.clear();
	m_IndexingMap.clear();

	m_DistributeStartLevel = 0;
}

bool GDEMDistributeConfig::ReadFromRegistry()
{
	Initialize();

	QSettings settings(QSettings::SystemScope, CONF_SET_ORG, CONF_SET_APP);
	settings.beginGroup(CONF_SET_DISTRIBUTE);
	m_DistributeType = (E_DISTRIBUTE_TYPE)settings.value(CONF_DIST_TYPE, m_DistributeType).toUInt();
	SetDistributeNum(settings.value(CONF_DIST_ORGNUM, m_DistributeNum).toUInt());
	m_TargetNum = settings.value(CONF_DIST_NUM, m_TargetNum).toUInt();

	if (m_DistributeType == E_DISTRIBUTE_TYPE_PC)
	{
		ST_DISTRIBUTE_PC pc;
		int sizePCs = settings.beginReadArray(CONF_DIST_SERVERS);
		QString strIP = "";

		m_ServerPCMap.clear();
		for (int i = 0; i < sizePCs; ++i)
		{
			settings.setArrayIndex(i);
			pc.start_index	= settings.value(CONF_DIST_SERVER_STARTID).toUInt();
			pc.end_index	= settings.value(CONF_DIST_SERVER_ENDID).toUInt();
			strIP			= settings.value(CONF_DIST_SERVER_ADDR).toString();
			pc.port			= settings.value(CONF_DIST_SERVER_PORT).toUInt();
			strcpy(pc.ip_address, strIP.toAscii());
			m_ServerPCMap.append(pc);
		}
		settings.endArray();

		MakeIndexingMap();
	}
	else if (m_DistributeType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		ST_DISTRIBUTE_FOLDER dirs;
		int sizeHards = settings.beginReadArray(CONF_DIST_HARDS);
		QString strHard = "";
		wchar_t temppath[MAX_DIST_PATH];

		m_FolderMap.clear();
		for (int i = 0; i < sizeHards; ++i)
		{
			settings.setArrayIndex(i);
			dirs.start_index	= settings.value(CONF_DIST_HARD_STARTID).toUInt();
			dirs.end_index		= settings.value(CONF_DIST_HARD_ENDID).toUInt();
			strHard = settings.value(CONF_DIST_HARD_PATH).toString();
			strHard.toWCharArray(temppath);
			gs_wchar2ushort(temppath, dirs.path, strHard.count());
			m_FolderMap.append(dirs);
		}
		settings.endArray();

		MakeIndexingMap();
	}

	settings.endGroup();

	return true;
}

bool GDEMDistributeConfig::WriteToRegistry()
{
	QSettings settings(QSettings::SystemScope, CONF_SET_ORG, CONF_SET_APP);
	settings.beginGroup(CONF_SET_DISTRIBUTE);
	settings.remove("");
	settings.setValue(CONF_DIST_TYPE, m_DistributeType);
	settings.setValue(CONF_DIST_ORGNUM, m_DistributeNum);
	settings.setValue(CONF_DIST_NUM, m_TargetNum);

	if (m_DistributeType == E_DISTRIBUTE_TYPE_PC)
	{
		settings.beginWriteArray(CONF_DIST_SERVERS);
		for (int i = 0; i < m_ServerPCMap.count(); ++i)
		{
			settings.setArrayIndex(i);
			settings.setValue(CONF_DIST_SERVER_STARTID, m_ServerPCMap[i].start_index);
			settings.setValue(CONF_DIST_SERVER_ENDID, m_ServerPCMap[i].end_index);
			settings.setValue(CONF_DIST_SERVER_ADDR, m_ServerPCMap[i].ip_address);
			settings.setValue(CONF_DIST_SERVER_PORT, m_ServerPCMap[i].port);
		}
		settings.endArray();
	}
	else if (m_DistributeType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		settings.beginWriteArray(CONF_DIST_HARDS);
		QString strPath = "";
		for (int i = 0; i < m_FolderMap.count(); ++i)
		{
			settings.setArrayIndex(i);
			settings.setValue(CONF_DIST_HARD_STARTID, m_FolderMap[i].start_index);
			settings.setValue(CONF_DIST_HARD_ENDID, m_FolderMap[i].end_index);
			strPath = QString::fromUtf16(m_FolderMap[i].path);
			settings.setValue(CONF_DIST_HARD_PATH, strPath);
		}
		settings.endArray();
	}

	settings.endGroup();

	return true;
}

bool GDEMDistributeConfig::ReadFromBuffer(char* a_pBuffer, unsigned int a_nBytes)
{
	Initialize();

	unsigned int tBytes = 0;
	if (a_nBytes <= tBytes)
		return false;

	char* pTempBuffer = a_pBuffer;

	m_DistributeType = *(E_DISTRIBUTE_TYPE*)pTempBuffer;
	pTempBuffer += sizeof(unsigned int);
	tBytes += sizeof(unsigned int);
	if (a_nBytes <= tBytes)
	{
		Initialize();
		return false;
	}

	SetDistributeNum(*(unsigned int*)pTempBuffer);
	pTempBuffer += sizeof(unsigned int);
	tBytes += sizeof(unsigned int);
	if (a_nBytes <= tBytes)
	{
		Initialize();
		return false;
	}

	m_TargetNum = *(unsigned int*)pTempBuffer;
	pTempBuffer += sizeof(unsigned int);
	tBytes += sizeof(unsigned int);
	if (a_nBytes <= tBytes)
	{
		Initialize();
		return true; // RYM. no communication bug.
	}

	if (m_DistributeType == E_DISTRIBUTE_TYPE_PC)
	{
		if (a_nBytes < (tBytes + (sizeof(ST_DISTRIBUTE_PC) * m_TargetNum)))
		{
			Initialize();
			return false;
		}

		ST_DISTRIBUTE_PC *pPCInfoList = (ST_DISTRIBUTE_PC *)pTempBuffer;
		m_ServerPCMap.clear();
		for (unsigned int i=0; i<m_TargetNum; i++)
		{
			AddPCInfo(pPCInfoList[i]);
		}
		pTempBuffer += sizeof(ST_DISTRIBUTE_PC) * m_TargetNum;
		tBytes += sizeof(ST_DISTRIBUTE_PC) * m_TargetNum;

		MakeIndexingMap();
	}
	else if (m_DistributeType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		if (a_nBytes < (tBytes + (sizeof(ST_DISTRIBUTE_FOLDER) * m_TargetNum)))
		{
			Initialize();
			return false;
		}

		ST_DISTRIBUTE_FOLDER *pFolderInfoList = (ST_DISTRIBUTE_FOLDER *)pTempBuffer;
		m_FolderMap.clear();
		for (unsigned int i=0; i<m_TargetNum; i++)
		{
			AddFolderInfo(pFolderInfoList[i]);
		}
		pTempBuffer += sizeof(ST_DISTRIBUTE_FOLDER) * m_TargetNum;
		tBytes += sizeof(ST_DISTRIBUTE_FOLDER) * m_TargetNum;

		MakeIndexingMap();
	}

	return true;
}

bool GDEMDistributeConfig::WriteToBuffer(char* a_pBuffer, unsigned int& a_nBytes)
{
	unsigned int tBytes = 0;
	char* pTempBuffer = a_pBuffer;

	*(int*)pTempBuffer = m_DistributeType;
	pTempBuffer += sizeof(int);
	tBytes += sizeof(int);

	*(unsigned int*)pTempBuffer = m_DistributeNum;
	pTempBuffer += sizeof(unsigned int);
	tBytes += sizeof(unsigned int);

	*(unsigned int*)pTempBuffer = m_TargetNum;
	pTempBuffer += sizeof(unsigned int);
	tBytes += sizeof(unsigned int);


	if (m_DistributeType == E_DISTRIBUTE_TYPE_PC)
	{
		ST_DISTRIBUTE_PC *pPCInfoList = (ST_DISTRIBUTE_PC *)pTempBuffer;
		for (unsigned int i=0; i<m_TargetNum; i++)
		{
			pPCInfoList[i] = m_ServerPCMap[i];
		}
		pTempBuffer += sizeof(ST_DISTRIBUTE_PC) * m_TargetNum;
		tBytes += sizeof(ST_DISTRIBUTE_PC) * m_TargetNum;
	}
	else if (m_DistributeType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		ST_DISTRIBUTE_FOLDER *pFolderInfoList = (ST_DISTRIBUTE_FOLDER *)pTempBuffer;
		for (unsigned int i=0; i<m_TargetNum; i++)
		{
			pFolderInfoList[i] = m_FolderMap[i];
		}
		pTempBuffer += sizeof(ST_DISTRIBUTE_FOLDER) * m_TargetNum;
		tBytes += sizeof(ST_DISTRIBUTE_FOLDER) * m_TargetNum;
	}

	a_nBytes = tBytes;

	return true;
}

void GDEMDistributeConfig::MakeIndexingMap()
{
	m_IndexingMap.clear();
	if (m_DistributeType == E_DISTRIBUTE_TYPE_PC)
	{
		for (unsigned int i=0; i<m_TargetNum; i++)
		{
			ST_DISTRIBUTE_PC tPCInfo;
			GetPCInfo(i, tPCInfo);
			for (unsigned int k=tPCInfo.start_index; k<=tPCInfo.end_index; k++)
			{
				m_IndexingMap.insert(k, i);
			}
		}
	}
	else if (m_DistributeType == E_DISTRIBUTE_TYPE_DIRECTORY)
	{
		for (unsigned int i=0; i<m_TargetNum; i++)
		{
			ST_DISTRIBUTE_FOLDER tFolderInfo;
			GetFolderInfo(i, tFolderInfo);
			for (unsigned int k=tFolderInfo.start_index; k<=tFolderInfo.end_index; k++)
			{
				m_IndexingMap.insert(k, i);
			}
		}
	}
}

bool GDEMDistributeConfig::GetPCInfo(unsigned int a_nIndex, ST_DISTRIBUTE_PC& a_PCInfo)
{
	if (a_nIndex < 0 || a_nIndex >= m_TargetNum)
		return false;

	a_PCInfo = m_ServerPCMap[a_nIndex];

	return true;
}

bool GDEMDistributeConfig::GetFolderInfo(unsigned int a_nIndex, ST_DISTRIBUTE_FOLDER& a_FolderInfo)
{
	if (a_nIndex < 0 || a_nIndex >= m_TargetNum)
		return false;

	a_FolderInfo = m_FolderMap[a_nIndex];

	return true;
}

void GDEMDistributeConfig::AddPCInfo(ST_DISTRIBUTE_PC& a_PCInfo)
{
	m_ServerPCMap.append(a_PCInfo);
}

void GDEMDistributeConfig::AddFolderInfo(ST_DISTRIBUTE_FOLDER& a_FolderInfo)
{
	m_FolderMap.append(a_FolderInfo);
}

void GDEMDistributeConfig::UpdateFolderInfo(unsigned int a_nIndex, ST_DISTRIBUTE_FOLDER& a_FolderInfo)
{
	m_FolderMap[a_nIndex] = a_FolderInfo;
}

bool GDEMDistributeConfig::ConvertIPStringToLong(QString& a_strIP, long& a_lIP)
{
	QStringList fields = a_strIP.split('.');
	if (fields.size() != 4 /* Key=Value */)
		return false;

	a_lIP = 0;
	a_lIP += fields.takeFirst().toInt() * 0x1000000;
	a_lIP += fields.takeFirst().toInt() * 0x10000;
	a_lIP += fields.takeFirst().toInt() * 0x100;
	a_lIP += fields.takeFirst().toInt();

	return true;
}

void GDEMDistributeConfig::CalcDistributeStartLevel()
{
	unsigned int tNum = m_DistributeNum;
	unsigned int tPrefix = 0;

	while(true)
	{
		tNum /= 4;
		if (tNum == 0)
			break;

		tPrefix ++;
	}

	m_DistributeStartLevel = 7+tPrefix;
}

unsigned int GDEMDistributeConfig::GetTargetIndex(E_GDM_MAINDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nX, unsigned int a_nY)
{
	if (a_eDataType != E_GDM_MAINDATA_TYPE_DEM && a_eDataType != E_GDM_MAINDATA_TYPE_IMG)
		return 0;

	int nIndex = 0;
	if (a_nLevel >= m_DistributeStartLevel)
	{
		unsigned int block_x = a_nX / TILE_ROW_COUNT_IN_BLOCK;
		unsigned int block_y = a_nY / TILE_ROW_COUNT_IN_BLOCK;

		unsigned int ndivcount = 1 << (m_DistributeStartLevel-7);

		unsigned int step = (1 << (a_nLevel-7)) / ndivcount;


		int mStepinex = block_x / step;
		int nStepinex = block_y / step;

		nIndex = mStepinex + (nStepinex * ndivcount);
	}

	return m_IndexingMap[nIndex];
}

bool GDEMDistributeConfig::GetPCInfo(E_GDM_MAINDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nX, unsigned int a_nY, ST_DISTRIBUTE_PC& a_PCInfo)
{
	return GetPCInfo(GetTargetIndex(a_eDataType, a_nLevel, a_nX, a_nY), a_PCInfo);
}

bool GDEMDistributeConfig::GetFolderInfo(E_GDM_MAINDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nX, unsigned int a_nY, ST_DISTRIBUTE_FOLDER& a_Path)
{
	return GetFolderInfo(GetTargetIndex(a_eDataType, a_nLevel, a_nX, a_nY), a_Path);
}

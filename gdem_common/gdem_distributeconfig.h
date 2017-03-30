#ifndef __GDEM_DISTRIBUTECONFIG_H__
#define __GDEM_DISTRIBUTECONFIG_H__

#include "gdem_protocol.h"

#include <QMap>
#include <QString>

#define MAX_DIST_SRV_CNT	10
#define MAX_DIST_PATH		260

// geometric name type
typedef enum {
	E_DISTRIBUTE_TYPE_NONE,
	E_DISTRIBUTE_TYPE_PC,
	E_DISTRIBUTE_TYPE_DIRECTORY,
} E_DISTRIBUTE_TYPE;


typedef struct tagDISTRIBUTE_PC
{
	unsigned int	start_index;
	unsigned int	end_index;
	unsigned int	port;
	char			ip_address[DS_ADDR_LEN];
} ST_DISTRIBUTE_PC;

typedef struct tagDISTRIBUTE_FOLDER
{
	unsigned int	start_index;
	unsigned int	end_index;
	ushort			path[MAX_DIST_PATH];
} ST_DISTRIBUTE_FOLDER;

class GDEMDistributeConfig
{
public:
	GDEMDistributeConfig();
	~GDEMDistributeConfig();

public:
	void Initialize();

	bool ReadFromRegistry();
	bool WriteToRegistry();

	bool ReadFromBuffer(char* a_pBuffer, unsigned int a_nBytes);
	bool WriteToBuffer(char* a_pBuffer, unsigned int& a_nBytes);

	void MakeIndexingMap();

	bool GetPCInfo(unsigned int a_nIndex, ST_DISTRIBUTE_PC& a_PCInfo);
	bool GetFolderInfo(unsigned int a_nIndex, ST_DISTRIBUTE_FOLDER& a_FolderInfo);

	void AddPCInfo(ST_DISTRIBUTE_PC& a_PCInfo);
	void AddFolderInfo(ST_DISTRIBUTE_FOLDER& a_FolderInfo);
	void UpdateFolderInfo(unsigned int a_nIndex, ST_DISTRIBUTE_FOLDER& a_FolderInfo);

	bool ConvertIPStringToLong(QString& a_strIP, long& a_lIP);


	E_DISTRIBUTE_TYPE GetDistributableType() { return m_DistributeType; }
	void SetDistributableType(E_DISTRIBUTE_TYPE a_Type) { m_DistributeType = a_Type; }
	unsigned int GetDistributeNum() { return m_DistributeNum; }
	void SetDistributeNum(unsigned int a_DistNum) 
	{ 
		m_DistributeNum = a_DistNum; 
		CalcDistributeStartLevel();
	}
	unsigned int GetDistributeTargetNum()  { return m_TargetNum; }
	void SetDistributeTargetNum(unsigned int a_TargetNum) { m_TargetNum = a_TargetNum; }

	bool IsDistributable() { return m_TargetNum > 1; }

	unsigned int GetTargetIndex(E_GDM_MAINDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nX, unsigned int a_nY);
	bool GetPCInfo(E_GDM_MAINDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nX, unsigned int a_nY, ST_DISTRIBUTE_PC& a_PCInfo);
	bool GetFolderInfo(E_GDM_MAINDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nX, unsigned int a_nY, ST_DISTRIBUTE_FOLDER& a_Path);

private:
	void CalcDistributeStartLevel();

private:

	E_DISTRIBUTE_TYPE	m_DistributeType;
	unsigned int		m_DistributeNum;
	unsigned int		m_TargetNum;

	QList<ST_DISTRIBUTE_PC>				m_ServerPCMap;
	QList<ST_DISTRIBUTE_FOLDER>			m_FolderMap;
	QMap<unsigned int, unsigned int>	m_IndexingMap;

	unsigned int		m_DistributeStartLevel;
};

extern GDEMDistributeConfig g_DistributeConfig;

#endif // __GDEM_DISTRIBUTECONFIG_H__

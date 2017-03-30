#ifndef __GDSCONFIGMGR_H__
#define __GDSCONFIGMGR_H__

#include "GDSDefine.h"

#include "../gdem_common/gdem_config.h"

#include <QString>

class GDSConfig
{
public:
	GDSConfig();
	~GDSConfig();

public:
	bool InitializeConfig(const QString& a_strCurPath = QString());

	void ReadServerConfig();
	void WriteServerConfig();

	/* get and set value functions */
#ifndef STANDARD_ALONE
	void SetPortNo(unsigned short a_nPortNo)
	{
		m_nPortNo = a_nPortNo;
	}
	unsigned short GetPortNo(void)
	{
		return m_nPortNo;
	}

	void SetMaxConnectNum(int a_nMaxNum)
	{
		m_nMaxSocketNum = a_nMaxNum * ONE_CLIENT_SOCKET_NUM;
	}
	int GetMaxConnectNum(void)
	{
		return m_nMaxSocketNum / ONE_CLIENT_SOCKET_NUM;
	}
	int GetMaxSocketNum(void)
	{
		return m_nMaxSocketNum;
	}
#endif

#ifdef USE_MYSQL
	void SetDBServerIP(QString& a_SrvAddr)
	{
		m_dbSrvAddr = a_SrvAddr;
	}
	QString& GetDBServerIP(void)
	{
		return m_dbSrvAddr;
	}

	void SetDBUserID(QString& a_UserName)
	{
		m_dbUserName = a_UserName;
	}
	QString& GetDBUserID(void)
	{
		return m_dbUserName;
	}

	void SetDBPassword(QString& a_DBPassword)
	{
		m_dbPassword = a_DBPassword;
	}
	QString& GetDBPassword(void)
	{
		return m_dbPassword;
	}

	void SetDBName(QString& a_DBName)
	{
		m_dbName = a_DBName;
	}
	QString& GetDBName(void)
	{
		return m_dbName;
	}
#endif

	void SetFileDBPath(QString& a_FileDBPath)
	{
		m_dbPath = a_FileDBPath;
	}
	QString& GetFileDBPath(void)
	{
		return m_dbPath;
	}

	void SetLogOutputType(E_LOG_OUTPUT_TYPE a_LogOutputType)
	{
		m_LogOutputType = a_LogOutputType;
	}
	E_LOG_OUTPUT_TYPE GetLogOutputType(void)
	{
		return m_LogOutputType;
	}

	void SetLogOutputLevel(E_LOG_OUTPUT_LEVEL a_LogOutputLevel)
	{
		m_LogOutputLevel = a_LogOutputLevel;
	}
	E_LOG_OUTPUT_LEVEL GetLogOutputLevel(void)
	{
		return m_LogOutputLevel;
	}

	QString& GetCurrentPath(void)
	{
		return m_CurrentPath;
	}

private:
	QString			m_CurrentPath;

	// server info	
#ifndef STANDARD_ALONE
	unsigned short	m_nPortNo;
	unsigned short	m_nMaxSocketNum;
#endif

	// database info
#ifdef USE_MYSQL
	QString			m_dbSrvAddr;
	QString			m_dbUserName;
	QString			m_dbPassword;
	QString			m_dbName;
#endif

	QString					m_dbPath;
	E_LOG_OUTPUT_TYPE		m_LogOutputType;
	E_LOG_OUTPUT_LEVEL		m_LogOutputLevel;
};

extern GDSConfig g_ServerConfig;

#endif // __GDSCONFIGMGR_H__

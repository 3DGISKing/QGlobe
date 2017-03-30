#include "GDSConfigMgr.h"
#include "GDSDefine.h"
#include "GDSType.h"
#include "GDSCommon.h"
#include <QSettings>
#include <qglobal.h>

#define CONF_SET_ORG    "IEEI"
#define CONF_SET_APP    "GeoBrowser"

#define CONF_SET_SRVPORT			"srv_port"
#define CONF_SET_SRVMAX				"srv_maxcount"
#define CONF_SET_DBSRVADDR			"db_srvaddr"
#define CONF_SET_DBUSER				"db_user"
#define CONF_SET_DBPASSWD			"db_passwd"
#define CONF_SET_DBNAME				"db_name"
#define CONF_SET_DBPATH				"db_path"
#define CONF_SET_LOGTYPE			"log_type"
#define CONF_SET_LOGLEVEL			"log_level"

#ifdef Q_OS_WIN
#define DEFAULT_DBPATH_WIN                  "C:/Program Files/GeoBrowser/database"          
#endif

GDSConfig g_ServerConfig;

GDSConfig::GDSConfig()
{
#ifndef STANDARD_ALONE
	m_nPortNo = 33333;
	m_nMaxSocketNum = 30 * ONE_CLIENT_SOCKET_NUM;
#endif

#ifdef USE_MYSQL
	m_dbSrvAddr		= "localhost";
	m_dbUserName	= "root";
	m_dbPassword	= "autoset";
	m_dbName		= "dbworld";
#endif

#ifdef Q_WS_WIN
	m_dbPath		= DEFAULT_DBPATH_WIN;
#else
	m_dbPath		= "/database";
#endif
	m_LogOutputType = E_LOG_OUTPUT_QDEBUG;
	m_LogOutputLevel = E_LOG_LEVEL_0;
}

GDSConfig::~GDSConfig()
{
}

bool GDSConfig::InitializeConfig(const QString& a_strCurPath)
{
	m_CurrentPath = a_strCurPath;
	ReadServerConfig();
	WriteServerConfig();
	return true;
}

void GDSConfig::ReadServerConfig()
{
	// server config
#ifdef STANDARD_ALONE
	QSettings settings(QSettings::UserScope, CONF_SET_ORG, CONF_SET_APP);
#else
	QSettings settings(QSettings::SystemScope, CONF_SET_ORG, CONF_SET_APP);
#endif

#ifndef STANDARD_ALONE
	m_nPortNo = settings.value(CONF_SET_SRVPORT, m_nPortNo).toUInt();
	m_nMaxSocketNum = settings.value(CONF_SET_SRVMAX, m_nMaxSocketNum).toUInt();
#endif

	// database config
#ifdef USE_MYSQL
	m_dbSrvAddr = settings.value(CONF_SET_DBSRVADDR, m_dbSrvAddr).toString();
	m_dbUserName = settings.value(CONF_SET_DBUSER, m_dbUserName).toString();
	m_dbPassword = settings.value(CONF_SET_DBPASSWD, m_dbPassword).toString();
	m_dbName = settings.value(CONF_SET_DBNAME, m_dbName).toString();
#endif

	m_dbPath = settings.value(CONF_SET_DBPATH, m_dbPath).toString();
	m_LogOutputType = (E_LOG_OUTPUT_TYPE)settings.value(CONF_SET_LOGTYPE, m_LogOutputType).toUInt();
	m_LogOutputLevel = (E_LOG_OUTPUT_LEVEL)settings.value(CONF_SET_LOGLEVEL, m_LogOutputLevel).toUInt();
}

void GDSConfig::WriteServerConfig()
{
	// server config
#ifdef STANDARD_ALONE
	QSettings settings(QSettings::UserScope, CONF_SET_ORG, CONF_SET_APP);
#else
	QSettings settings(QSettings::SystemScope, CONF_SET_ORG, CONF_SET_APP);
#endif

#ifndef STANDARD_ALONE
	settings.setValue(CONF_SET_SRVPORT, m_nPortNo);
	settings.setValue(CONF_SET_SRVMAX, m_nMaxSocketNum);
#endif

	// database config
#ifdef USE_MYSQL
	settings.setValue(CONF_SET_DBSRVADDR, m_dbSrvAddr);
	settings.setValue(CONF_SET_DBUSER, m_dbUserName);
	settings.setValue(CONF_SET_DBPASSWD, m_dbPassword);
	settings.setValue(CONF_SET_DBNAME, m_dbName);
#endif

	settings.setValue(CONF_SET_DBPATH, m_dbPath);
	settings.setValue(CONF_SET_LOGTYPE, m_LogOutputType);
	settings.setValue(CONF_SET_LOGLEVEL, m_LogOutputLevel);
}

/* Get Value Functions */

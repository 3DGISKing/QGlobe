#include "GDSLog.h"
#include "GDSConfigMgr.h"
#include <QDate>
#include <QTime>
#include <QString>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QDebug>
#include <stdio.h>
#include <stdarg.h>

static E_LOG_OUTPUT_TYPE g_eLogOutputType = E_LOG_OUTPUT_NONE;
static E_LOG_OUTPUT_LEVEL g_eLogOutputLevel = E_LOG_LEVEL_0;

static QFile* g_pLogFile = NULL;

//====================================================================================
// Function name	: OpenLogFile
// Creator			: u myongsong
// Description		: open log file
// Return type		: bool 
//====================================================================================

void OpenLogFile()
{
	if (g_pLogFile != NULL)
		return;

	QString strLogPath(g_ServerConfig.GetCurrentPath());

	int nLen = strLogPath.length();
	if (nLen > 0
		&& strLogPath[nLen-1] != '/')
	{
		strLogPath.append('/');
	}	
	
	strLogPath.append("log");

	QDir tDir;
	tDir.mkdir(strLogPath);

	QDate tDate = QDate::currentDate();
	QTime tTime = QTime::currentTime();

	QString strLogFileName;
	strLogFileName.sprintf("%s/server%04d-%02d-%02d-%02d-%02d-%02d.log",
		strLogPath.toAscii().data(),
		tDate.year(), 
		tDate.month(), 
		tDate.day(),
		tTime.hour(), 
		tTime.minute(), 
		tTime.second());


	g_pLogFile = new QFile(strLogFileName);
	if (g_pLogFile == NULL)
	{
		qDebug() << "Error new QFile(a_LogFileName)";
		g_eLogOutputType = E_LOG_OUTPUT_NONE;
		return;
	}

	if (!g_pLogFile->open(QIODevice::Text | QIODevice::WriteOnly))
	{
		qDebug() << g_pLogFile->errorString();
		g_eLogOutputType = E_LOG_OUTPUT_NONE;
		return;
	}
}

//====================================================================================
// Function name	: CloseLogFile
// Creator			: u myongsong
// Description		: close log file
// Return type		: bool
//====================================================================================
void CloseLogFile()
{
	if (g_pLogFile != NULL)
	{
		if (g_pLogFile->isOpen()) 
			g_pLogFile->close();

		delete g_pLogFile;
		g_pLogFile = NULL;
	}
}

void InitializeLog(E_LOG_OUTPUT_TYPE a_LogType, E_LOG_OUTPUT_LEVEL a_LogOutputLevel)
{
	g_eLogOutputType = a_LogType;
	g_eLogOutputLevel = a_LogOutputLevel;

	if (g_eLogOutputType == E_LOG_OUTPUT_FILE)
	{
		OpenLogFile();
	}
}

void FinalizeLog()
{
	if (g_eLogOutputType == E_LOG_OUTPUT_FILE)
	{
		CloseLogFile();
	}

	g_eLogOutputType = E_LOG_OUTPUT_NONE;
	g_eLogOutputLevel = E_LOG_LEVEL_0;
}

//====================================================================================
// Function name	: WriteLineToOutput
// Creator			: u myongsong
// Description		: print log to output window
// Return type		: void 
//====================================================================================
void WriteLineToQDebug(IN const char *a_format, IN va_list arglist)
{
	char strLogBuf1[512];
	char strLogBuf2[512];

	vsprintf_s(strLogBuf1, a_format, arglist);
	sprintf_s(strLogBuf2, "%s\n", strLogBuf1);

	qDebug() << strLogBuf2;
}

//====================================================================================
// Function name	: WriteLineToFile
// Creator			: u myongsong
// Description		: write log to log file
// Return type		: bool 
//====================================================================================
bool WriteLineToFile(IN const char *a_format, IN va_list arglist)
{
	char strLogBuf1[512];
	char strLogBuf2[512];

	bool retval = false;


	vsprintf_s(strLogBuf2, a_format, arglist);

	QDate tDate = QDate::currentDate();
	QTime tTime = QTime::currentTime();

	sprintf_s(strLogBuf1, "%04d/%02d/%02d %02d:%02d:%02d.%03d\t%s\n",
				tDate.year(), 
				tDate.month(), 
				tDate.day(),
				tTime.hour(), 
				tTime.minute(), 
				tTime.second(), 
				tTime.msec(),
				strLogBuf2);

	// check log file open state.
	if (g_pLogFile->isOpen()
		&& g_pLogFile->seek(g_pLogFile->size()))
	{
		// write log line to log file
		if (g_pLogFile->write(strLogBuf1))
			retval = true;	// return true

		g_pLogFile->seek(g_pLogFile->size());
	}


	return retval;
}

void GDSLogOutput(E_LOG_LEVEL a_LogLevel, IN const char *a_format, ...)
{
	if (g_eLogOutputType == E_LOG_OUTPUT_NONE)
		return;

	if (g_eLogOutputLevel == E_LOG_LEVEL_0 && a_LogLevel == E_LOG_TRACK)
		return;

	va_list arglist;

	va_start(arglist, a_format);

	if (g_eLogOutputType == E_LOG_OUTPUT_FILE)
		WriteLineToFile(a_format, arglist);
	else if (g_eLogOutputType == E_LOG_OUTPUT_QDEBUG)
		WriteLineToQDebug(a_format, arglist);

	va_end(arglist);
}
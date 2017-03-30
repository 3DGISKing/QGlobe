#include "gdsservice_log.h"
#include <QTextStream>
#include <QDir>

#undef WORKING_PATH
#define WORKING_PATH	"E:/GDEM_Solution/src/server/bin"
#undef LOG_FILENAME
#define LOG_FILENAME	"gdem_service.log"

GDSServiceLog::GDSServiceLog(void)
{
	m_workingPath  = WORKING_PATH;
	m_logFileName = LOG_FILENAME;
}

GDSServiceLog::~GDSServiceLog(void)
{
}

void GDSServiceLog::write(const QString& sMsg)
{
	QString oldDir = QDir::currentPath();
	QDir::setCurrent(m_workingPath);
	QFile logFile(m_logFileName);

	if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
		return;

	QTextStream out((QIODevice *)&logFile);
	out << sMsg << "\n";
	QDir::setCurrent(oldDir);
}

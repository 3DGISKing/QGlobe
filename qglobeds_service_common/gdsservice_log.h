#pragma once

#include <QString>
#include <QFile>

class QGlobeDSServiceLog
{
public:
	QGlobeDSServiceLog(void);
	~QGlobeDSServiceLog(void);

	void write(const QString& sMsg);

private:
	QString		m_workingPath;
	QString		m_logFileName;
};

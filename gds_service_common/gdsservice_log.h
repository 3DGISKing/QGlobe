#pragma once

#include <QString>
#include <QFile>

class GDSServiceLog
{
public:
	GDSServiceLog(void);
	~GDSServiceLog(void);

	void write(const QString& sMsg);

private:
	QString		m_workingPath;
	QString		m_logFileName;
};

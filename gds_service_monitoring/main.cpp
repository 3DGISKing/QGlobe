#include "gds_service_monitoring.h"

#include <QtCore/QSettings>
#include <QtCore/QStringList>

int main(int argc, char *argv[])
{
#if !defined(Q_WS_WIN)
	// GDSService stores service settings in SystemScope, which normally require root privileges.
	// To allow testing this example as non-root, we change the directory of the SystemScope settings file.
	QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
	qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
	GdsMonitorService service(argc, argv);
	QString str = service.serviceName();
	return service.exec();
}


#include <QtCore/QCoreApplication>
#include <QProcess>
#include <QBasicTimer>
#include <QTimerEvent>

#include "../qglobeds_service_common/gdsservice_common.h"

#undef MONITOR_SERVICE_NAME
#define MONITOR_SERVICE_NAME	"GDS Service Monitoring"
#undef SERVICE_NAME
#define SERVICE_NAME    "QGLOBEService"

#undef MONITER_INTERVAL_TIME
#define MONITER_INTERVAL_TIME		30000

class GdsMonitorService : public QGlobeDSService<QCoreApplication>
{
public:
	GdsMonitorService(int argc, char **argv);
	~GdsMonitorService();

protected:
	void start();

	void stop();

	QBasicTimer		m_Timer;
	void timerEvent(QTimerEvent *event);

};

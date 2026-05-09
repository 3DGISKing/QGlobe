#include "qglobeds_service_monitoring.h"


void serviceStart()
{
	QGlobeDSServiceController gdsServiceController(SERVICE_NAME);
	if (gdsServiceController.isInstalled())
	{
		if (!gdsServiceController.isRunning())
			gdsServiceController.start();
	}
}

void serviceStop()
{
#if 0
	QGlobeDSServiceController gdsServiceController(SERVICE_NAME);
	if (gdsServiceController.isInstalled())
	{
		if (gdsServiceController.isRunning())
			gdsServiceController.stop();
	}
#else	/* kill service process */
#ifdef Q_OS_WIN
	QString sysCmd = "TASKKILL /F /IM qglobeds_service.exe /T";
#else /* LINUX, MacOS */
	QString sysCmd = "killall qglobeds_service";
#endif
	QProcess::startDetached(sysCmd);
#endif
}

GdsMonitorService::GdsMonitorService(int argc, char **argv)
	: QGlobeDSService<QCoreApplication>(argc, argv, MONITOR_SERVICE_NAME)
{
	setServiceDescription("A dummy GDS Monitor service implemented with Qt");
	setServiceFlags(QGlobeDSServiceBase::Default);
	setStartupType(QGlobeDSServiceController::AutoStartup);

}

GdsMonitorService::~GdsMonitorService()
{
	m_Timer.stop();
}

void GdsMonitorService::start()
{
	serviceStart();
	m_Timer.start(30000, this);
}

void GdsMonitorService::stop()
{
	serviceStop();
	m_Timer.stop();

	QCoreApplication *app = application();
	app->quit();
}

void GdsMonitorService::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_Timer.timerId())
	{
		serviceStart();
	}
}

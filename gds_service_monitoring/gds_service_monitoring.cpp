#include "gds_service_monitoring.h"


void serviceStart()
{
	GDSServiceController gdsServiceController(SERVICE_NAME);
	if (gdsServiceController.isInstalled())
	{
		if (!gdsServiceController.isRunning())
			gdsServiceController.start();
	}
}

void serviceStop()
{
#if 0
	GDSServiceController gdsServiceController(SERVICE_NAME);
	if (gdsServiceController.isInstalled())
	{
		if (gdsServiceController.isRunning())
			gdsServiceController.stop();
	}
#else	/* kill service process */
#ifdef Q_WS_WIN
	QString sysCmd = "TASKKILL /F /IM gds_service.exe /T";
#else /* LINUX, MacOS */
	QString sysCmd = "killall gds_service";
#endif
	QProcess::startDetached(sysCmd);
#endif
}

GdsMonitorService::GdsMonitorService(int argc, char **argv)
	: GDSService<QCoreApplication>(argc, argv, MONITOR_SERVICE_NAME)
{
	setServiceDescription("A dummy GDS Monitor service implemented with Qt");
	setServiceFlags(GDSServiceBase::Default);
	setStartupType(GDSServiceController::AutoStartup);

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

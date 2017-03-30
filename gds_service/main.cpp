#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include "../gds_service_common/gdsservice_common.h"
#include "../gds_core/GDSCoreInterface.h"
#include "../gdem_common/gdem_environment.h"

#include <qglobal.h>

#undef SERVICE_NAME
#define SERVICE_NAME	"GDEMService"

//#define SERVICE_CUR_PATH

#ifndef SERVICE_CUR_PATH
#ifdef Q_WS_WIN
QString g_CurrentPath("C:/GDEM Server");
#else
QString g_CurrentPath("/GDEM Server");
#endif
#endif

#ifdef DEBUG
#include "../gds_service_common/gdsservice_log.h"
#endif

class GdemService : public GDSService<QCoreApplication>
{
public:
    GdemService(int argc, char **argv)
        : GDSService<QCoreApplication>(argc, argv, SERVICE_NAME)
    {
		m_bGDSCoreRunning = false;

        setServiceDescription("A dummy GDEM service implemented with Qt");
        setServiceFlags(GDSServiceBase::Default);
		setStartupType(GDSServiceController::AutoStartup);
	}

protected:
    void start()
    {
		QString tCurPath;
#ifdef SERVICE_CUR_PATH
		GetServerSeviceDir(tCurPath);
		m_bGDSCoreRunning = InitializeGDSCore(tCurPath);
#else
		m_bGDSCoreRunning = InitializeGDSCore(g_CurrentPath);
#endif

#ifdef DEBUG
#ifdef SERVICE_CUR_PATH
		QString msg;
		msg.sprintf("current path : %s", tCurPath.toAscii().data());
		GDSServiceLog svcLog;
		svcLog.write(msg);
#endif
#endif


		if (!m_bGDSCoreRunning)
		{
			stop();
		}
    }

    void stop()
    {
#if 0		/* kill service process */
		if (m_bGDSCoreRunning)
		{
			ReleaseGDSCore();
		}

		m_bGDSCoreRunning=false;
#endif

		application()->quit();
   }

private:
	bool m_bGDSCoreRunning;

};

int main(int argc, char **argv)
{
#if !defined(Q_WS_WIN)
    // GDSService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    GdemService service(argc, argv);

    QString str = service.serviceName();
    return service.exec();
}

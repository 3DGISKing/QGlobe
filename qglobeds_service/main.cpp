#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include "../qglobeds_service_common/gdsservice_common.h"
#include "../qglobeds_core/QGlobeDSCoreInterface.h"
//#include "../qglobe_common/qglobe_environment.h"

#include <qglobal.h>

#undef SERVICE_NAME
#define SERVICE_NAME	"QGLOBEService"

//#define SERVICE_CUR_PATH

#ifndef SERVICE_CUR_PATH
#ifdef Q_OS_WIN
QString g_CurrentPath("C:/QGLOBE Server");
#else
QString g_CurrentPath("/QGLOBE Server");
#endif
#endif

#ifdef DEBUG
#include "../qglobeds_service_common/gdsservice_log.h"
#endif

class QGlobeService : public QGlobeDSService<QCoreApplication>
{
public:
    QGlobeService(int argc, char **argv)
        : QGlobeDSService<QCoreApplication>(argc, argv, SERVICE_NAME)
    {
		m_bGDSCoreRunning = false;

        setServiceDescription("A dummy QGLOBE service implemented with Qt");
        setServiceFlags(QGlobeDSServiceBase::Default);
		setStartupType(QGlobeDSServiceController::AutoStartup);
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
		msg.sprintf("current path : %s", tCurPath.toUtf8().data());
		QGlobeDSServiceLog svcLog;
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
#if !defined(Q_OS_WIN)
    // QGlobeDSService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    QGlobeService service(argc, argv);

    QString str = service.serviceName();
    return service.exec();
}

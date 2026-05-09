#ifndef QTSERVICE_H
#define QTSERVICE_H

#include <QtCore/QCoreApplication>
#include <QObject>

#if defined(Q_OS_WIN)
#  if !defined(QT_QTSERVICE_EXPORT) && !defined(QT_QTSERVICE_IMPORT)
#    define QT_QTSERVICE_EXPORT
#  elif defined(QT_QTSERVICE_IMPORT)
#    if defined(QT_QTSERVICE_EXPORT)
#      undef QT_QTSERVICE_EXPORT
#    endif
#    define QT_QTSERVICE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTSERVICE_EXPORT)
#    undef QT_QTSERVICE_EXPORT
#    define QT_QTSERVICE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTSERVICE_EXPORT
#endif

class QStringList;
class QGlobeDSServiceControllerPrivate;

class QT_QTSERVICE_EXPORT QGlobeDSServiceController
{
    Q_DECLARE_PRIVATE(QGlobeDSServiceController)
public:
    enum StartupType
    {
	    AutoStartup = 0, ManualStartup
    };

    QGlobeDSServiceController(const QString &name);
    virtual ~QGlobeDSServiceController();

    bool isInstalled() const;
    bool isRunning() const;

    QString serviceName() const;
    QString serviceDescription() const;
    StartupType startupType() const;
    QString serviceFilePath() const;

    static bool install(const QString &serviceFilePath, const QString &account = QString(),
                const QString &password = QString());
    bool uninstall();

    bool start(const QStringList &arguments);
    bool start();
    bool stop();
    bool pause();
    bool resume();
    bool sendCommand(int code);

private:
    QGlobeDSServiceControllerPrivate *d_ptr;
};

class QGlobeDSServiceBasePrivate;

class QT_QTSERVICE_EXPORT QGlobeDSServiceBase : public QObject
{
	Q_OBJECT
    Q_DECLARE_PRIVATE(QGlobeDSServiceBase)
public:

    enum MessageType
    {
	Success = 0, Error, Warning, Information
    };

    enum ServiceFlag
    {
        Default = 0x00,
        CanBeSuspended = 0x01,
        CannotBeStopped = 0x02
    };

    Q_DECLARE_FLAGS(ServiceFlags, ServiceFlag)

    QGlobeDSServiceBase(int argc, char **argv, const QString &name);
    virtual ~QGlobeDSServiceBase();

    QString serviceName() const;

    QString serviceDescription() const;
    void setServiceDescription(const QString &description);

    QGlobeDSServiceController::StartupType startupType() const;
    void setStartupType(QGlobeDSServiceController::StartupType startupType);

    ServiceFlags serviceFlags() const;
    void setServiceFlags(ServiceFlags flags);

    int exec();

    void logMessage(const QString &message, MessageType type = Success,
                int id = 0, uint category = 0, const QByteArray &data = QByteArray());

    static QGlobeDSServiceBase *instance();

protected:

    virtual void start() = 0;
    virtual void stop();
    virtual void pause();
    virtual void resume();
    virtual void processCommand(int code);

    virtual void createApplication(int &argc, char **argv) = 0;

    virtual int executeApplication() = 0;

private:

    friend class QtServiceSysPrivate;
    QGlobeDSServiceBasePrivate *d_ptr;
};

template <typename Application>
class QGlobeDSService : public QGlobeDSServiceBase
{
public:
    QGlobeDSService(int argc, char **argv, const QString &name)
        : QGlobeDSServiceBase(argc, argv, name), app(0)
    {  }
    ~QGlobeDSService()
    {
    }

protected:
    Application *application() const
    { return app; }

    virtual void createApplication(int &argc, char **argv)
    {
        app = new Application(argc, argv);
        QCoreApplication *a = app;
        Q_UNUSED(a);
    }

    virtual int executeApplication()
    { return Application::exec(); }

private:
    Application *app;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QGlobeDSServiceBase::ServiceFlags)

#endif // QTSERVICE_H

#ifndef QTSERVICE_H
#define QTSERVICE_H

#include <QtCore/QCoreApplication>
#include <QObject>

#if defined(Q_WS_WIN)
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
class GDSServiceControllerPrivate;

class QT_QTSERVICE_EXPORT GDSServiceController
{
    Q_DECLARE_PRIVATE(GDSServiceController)
public:
    enum StartupType
    {
	    AutoStartup = 0, ManualStartup
    };

    GDSServiceController(const QString &name);
    virtual ~GDSServiceController();

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
    GDSServiceControllerPrivate *d_ptr;
};

class GDSServiceBasePrivate;

class QT_QTSERVICE_EXPORT GDSServiceBase : public QObject
{
	Q_OBJECT
    Q_DECLARE_PRIVATE(GDSServiceBase)
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

    GDSServiceBase(int argc, char **argv, const QString &name);
    virtual ~GDSServiceBase();

    QString serviceName() const;

    QString serviceDescription() const;
    void setServiceDescription(const QString &description);

    GDSServiceController::StartupType startupType() const;
    void setStartupType(GDSServiceController::StartupType startupType);

    ServiceFlags serviceFlags() const;
    void setServiceFlags(ServiceFlags flags);

    int exec();

    void logMessage(const QString &message, MessageType type = Success,
                int id = 0, uint category = 0, const QByteArray &data = QByteArray());

    static GDSServiceBase *instance();

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
    GDSServiceBasePrivate *d_ptr;
};

template <typename Application>
class GDSService : public GDSServiceBase
{
public:
    GDSService(int argc, char **argv, const QString &name)
        : GDSServiceBase(argc, argv, name), app(0)
    {  }
    ~GDSService()
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

Q_DECLARE_OPERATORS_FOR_FLAGS(GDSServiceBase::ServiceFlags)

#endif // QTSERVICE_H

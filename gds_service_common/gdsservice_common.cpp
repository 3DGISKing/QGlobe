#include "gdsservice_common.h"
#include "gdsservice_common_p.h"
#include <QtCore/QCoreApplication>
#include <stdio.h>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include <QtCore/QProcess>

#if defined(QTSERVICE_DEBUG)
#include <QDebug>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtCore/QMutex>
#if defined(Q_OS_WIN32)
#include <qt_windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#endif

static QFile* f = 0;

static void qtServiceCloseDebugLog()
{
    if (!f)
        return;
    QString ps(QTime::currentTime().toString("HH:mm:ss.zzz ") + QLatin1String("--- DEBUG LOG CLOSED ---\n\n"));
    f->write(ps.toAscii());
    f->flush();
    f->close();
    delete f;
    f = 0;
}

void qtServiceLogDebug(QtMsgType type, const char* msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QString s(QTime::currentTime().toString("HH:mm:ss.zzz "));
    s += QString("[%1] ").arg(
#if defined(Q_OS_WIN32)
                               GetCurrentProcessId());
#else
                               getpid());
#endif

    if (!f) {
#if defined(Q_OS_WIN32)
        f = new QFile("c:/service-debuglog.txt");
#else
        f = new QFile("/tmp/service-debuglog.txt");
#endif
        if (!f->open(QIODevice::WriteOnly | QIODevice::Append)) {
            delete f;
            f = 0;
            return;
        }
        QString ps(QLatin1String("\n") + s + QLatin1String("--- DEBUG LOG OPENED ---\n"));
        f->write(ps.toAscii());
    }

    switch (type) {
    case QtWarningMsg:
        s += QLatin1String("WARNING: ");
        break;
    case QtCriticalMsg:
        s += QLatin1String("CRITICAL: ");
        break;
    case QtFatalMsg:
        s+= QLatin1String("FATAL: ");
        break;
    case QtDebugMsg:
        s += QLatin1String("DEBUG: ");
        break;
    default:
        // Nothing
        break;
    }

    s += msg;
    s += QLatin1String("\n");

    f->write(s.toAscii());
    f->flush();

    if (type == QtFatalMsg) {
        qtServiceCloseDebugLog();
        exit(1);
    }
}

#endif


/*!
    Creates a controller object for the service with the given
    \a name.
*/
GDSServiceController::GDSServiceController(const QString &name)
 : d_ptr(new GDSServiceControllerPrivate())
{
    Q_D(GDSServiceController);
    d->q_ptr = this;
    d->serviceName = name;
}
/*!
    Destroys the service controller. This neither stops nor uninstalls
    the controlled service.

    To stop a service the stop() function must be called
    explicitly. To uninstall a service, you can use the uninstall()
    function.

    \sa stop(), GDSServiceController::uninstall()
*/
GDSServiceController::~GDSServiceController()
{
    delete d_ptr;
}
/*!
    \fn bool GDSServiceController::isInstalled() const

    Returns true if the service is installed; otherwise returns false.

    On Windows it uses the system's service control manager.

    On Unix it checks configuration written to QSettings::SystemScope
    using "QtSoftware" as organization name.

    \sa install()
*/

/*!
    \fn bool GDSServiceController::isRunning() const

    Returns true if the service is running; otherwise returns false. A
    service must be installed before it can be run using a controller.

    \sa start(), isInstalled()
*/

/*!
    Returns the name of the controlled service.

    \sa GDSServiceController(), serviceDescription()
*/
QString GDSServiceController::serviceName() const
{
    Q_D(const GDSServiceController);
    return d->serviceName;
}
/*!
    \fn QString GDSServiceController::serviceDescription() const

    Returns the description of the controlled service.

    \sa install(), serviceName()
*/

/*!
    \fn GDSServiceController::StartupType GDSServiceController::startupType() const

    Returns the startup type of the controlled service.

    \sa install(), serviceName()
*/

/*!
    \fn QString GDSServiceController::serviceFilePath() const

    Returns the file path to the controlled service.

    \sa install(), serviceName()
*/

/*!
    Installs the service with the given \a serviceFilePath
    and returns true if the service is installed
    successfully; otherwise returns false.

    On Windows service is installed in the system's service control manager with the given
    \a account and \a password.

    On Unix service configuration is written to QSettings::SystemScope
    using "QtSoftware" as organization name. \a account and \a password
    arguments are ignored.

    \warning Due to the different implementations of how services (daemons)
    are installed on various UNIX-like systems, this method doesn't
    integrate the service into the system's startup scripts.

    \sa uninstall(), start()
*/
bool GDSServiceController::install(const QString &serviceFilePath, const QString &account,
                const QString &password)
{
    QStringList arguments;
    arguments << QLatin1String("-i");
    arguments << account;
    arguments << password;
    return (QProcess::execute(serviceFilePath, arguments) == 0);
}


/*!
    \fn bool GDSServiceController::uninstall()

    Uninstalls the service and returns true if successful; otherwise returns false.

    On Windows service is uninstalled using the system's service control manager.

    On Unix service configuration is cleared using QSettings::SystemScope
    with "QtSoftware" as organization name.


    \sa install()
*/

/*!
    \fn bool GDSServiceController::start(const QStringList &arguments)

    Starts the installed service passing the given \a arguments to the
    service. A service must be installed before a controller can run it.

    Returns true if the service could be started; otherwise returns
    false.

    \sa install(), stop()
*/

/*!
    \overload

    Starts the installed service without passing any arguments to the service.
*/
bool GDSServiceController::start()
{
    return start(QStringList());
}

/*!
    \fn bool GDSServiceController::stop()

    Requests the running service to stop. The service will call the
    GDSServiceBase::stop() implementation unless the service's state
    is GDSServiceBase::CannotBeStopped.  This function does nothing if
    the service is not running.

    Returns true if a running service was successfully stopped;
    otherwise false.

    \sa start(), GDSServiceBase::stop(), GDSServiceBase::ServiceFlags
*/

/*!
    \fn bool GDSServiceController::pause()

    Requests the running service to pause. If the service's state is
    GDSServiceBase::CanBeSuspended, the service will call the
    GDSServiceBase::pause() implementation. The function does nothing
    if the service is not running.

    Returns true if a running service was successfully paused;
    otherwise returns false.

    \sa resume(), GDSServiceBase::pause(), GDSServiceBase::ServiceFlags
*/

/*!
    \fn bool GDSServiceController::resume()

    Requests the running service to continue. If the service's state
    is GDSServiceBase::CanBeSuspended, the service will call the
    GDSServiceBase::resume() implementation. This function does nothing
    if the service is not running.

    Returns true if a running service was successfully resumed;
    otherwise returns false.

    \sa pause(), GDSServiceBase::resume(), GDSServiceBase::ServiceFlags
*/

/*!
    \fn bool GDSServiceController::sendCommand(int code)

    Sends the user command \a code to the service. The service will
    call the GDSServiceBase::processCommand() implementation.  This
    function does nothing if the service is not running.

    Returns true if the request was sent to a running service;
    otherwise returns false.

    \sa GDSServiceBase::processCommand()
*/

class QtServiceStarter : public QObject
{
    Q_OBJECT
public:
    QtServiceStarter(GDSServiceBasePrivate *service)
        : QObject(), d_ptr(service) {}
public slots:
    void slotStart()
    {
        d_ptr->startService();
    }
private:
    GDSServiceBasePrivate *d_ptr;
};
#include "gdsservice_common.moc"

GDSServiceBase *GDSServiceBasePrivate::instance = 0;

GDSServiceBasePrivate::GDSServiceBasePrivate(const QString &name)
    : startupType(GDSServiceController::ManualStartup), serviceFlags(0), controller(name)
{

}

GDSServiceBasePrivate::~GDSServiceBasePrivate()
{

}

void GDSServiceBasePrivate::startService()
{
    q_ptr->start();
}

int GDSServiceBasePrivate::run(bool asService, const QStringList &argList)
{
    int argc = argList.size();
    QVector<char *> argv(argc);
    QList<QByteArray> argvData;
    for (int i = 0; i < argc; ++i)
        argvData.append(argList.at(i).toLocal8Bit());
    for (int i = 0; i < argc; ++i)
        argv[i] = argvData[i].data();

    if (asService && !sysInit())
        return -1;

    q_ptr->createApplication(argc, argv.data());
    QCoreApplication *app = QCoreApplication::instance();
    if (!app)
        return -1;

    if (asService)
        sysSetPath();

    QtServiceStarter starter(this);
    QTimer::singleShot(0, &starter, SLOT(slotStart()));
    int res = q_ptr->executeApplication();
    delete app;

    if (asService)
        sysCleanup();
    return res;
}


GDSServiceBase::GDSServiceBase(int argc, char **argv, const QString &name)
{
#if defined(QTSERVICE_DEBUG)
    qInstallMsgHandler(qtServiceLogDebug);
    qAddPostRoutine(qtServiceCloseDebugLog);
#endif

    Q_ASSERT(!GDSServiceBasePrivate::instance);
    GDSServiceBasePrivate::instance = this;

    QString nm(name);
    if (nm.length() > 255) {
	qWarning("GDSService: 'name' is longer than 255 characters.");
	nm.truncate(255);
    }
    if (nm.contains('\\')) {
	qWarning("GDSService: 'name' contains backslashes '\\'.");
	nm.replace((QChar)'\\', (QChar)'\0');
    }

    d_ptr = new GDSServiceBasePrivate(nm);
    d_ptr->q_ptr = this;

    d_ptr->serviceFlags = 0;
    d_ptr->sysd = 0;
    for (int i = 0; i < argc; ++i)
        d_ptr->args.append(QString::fromLocal8Bit(argv[i]));
}

/*!
    Destroys the service object. This neither stops nor uninstalls the
    service.

    To stop a service the stop() function must be called
    explicitly. To uninstall a service, you can use the
    GDSServiceController::uninstall() function.

    \sa stop(), GDSServiceController::uninstall()
*/
GDSServiceBase::~GDSServiceBase()
{
    delete d_ptr;
    GDSServiceBasePrivate::instance = 0;
}

/*!
    Returns the name of the service.

    \sa GDSServiceBase(), serviceDescription()
*/
QString GDSServiceBase::serviceName() const
{
    return d_ptr->controller.serviceName();
}

/*!
    Returns the description of the service.

    \sa setServiceDescription(), serviceName()
*/
QString GDSServiceBase::serviceDescription() const
{
    return d_ptr->serviceDescription;
}

/*!
    Sets the description of the service to the given \a description.

    \sa serviceDescription()
*/
void GDSServiceBase::setServiceDescription(const QString &description)
{
    d_ptr->serviceDescription = description;
}

/*!
    Returns the service's startup type.

    \sa GDSServiceController::StartupType, setStartupType()
*/
GDSServiceController::StartupType GDSServiceBase::startupType() const
{
    return d_ptr->startupType;
}

/*!
    Sets the service's startup type to the given \a type.

    \sa GDSServiceController::StartupType, startupType()
*/
void GDSServiceBase::setStartupType(GDSServiceController::StartupType type)
{
    d_ptr->startupType = type;
}

/*!
    Returns the service's state which is decribed using the
    ServiceFlag enum.

    \sa ServiceFlags, setServiceFlags()
*/
GDSServiceBase::ServiceFlags GDSServiceBase::serviceFlags() const
{
    return d_ptr->serviceFlags;
}

/*!
    \fn void GDSServiceBase::setServiceFlags(ServiceFlags flags)

    Sets the service's state to the state described by the given \a
    flags.

    \sa ServiceFlags, serviceFlags()
*/

/*!
    Executes the service.

    When the exec() function is called, it will parse the \l
    {serviceSpecificArguments} {service specific arguments} passed in
    \c argv, perform the required actions, and exit.

    If none of the arguments is recognized as service specific, exec()
    will first call the createApplication() function, then executeApplication() and
    finally the start() function. In the end, exec()
    returns while the service continues in its own process waiting for
    commands from the service controller.

    \sa GDSServiceController
*/
int GDSServiceBase::exec()
{
    if (d_ptr->args.size() > 1) {
        QString a =  d_ptr->args.at(1);
        if (a == QLatin1String("-i") || a == QLatin1String("-install")) {
            if (!d_ptr->controller.isInstalled()) {
                QString account;
                QString password;
                if (d_ptr->args.size() > 2)
                    account = d_ptr->args.at(2);
                if (d_ptr->args.size() > 3)
                    password = d_ptr->args.at(3);
                if (!d_ptr->install(account, password)) {
                    fprintf(stderr, "The service %s could not be installed\n", serviceName().toLatin1().constData());
                    return -1;
                } else {
                    printf("The service %s has been installed under: %s\n",
                        serviceName().toLatin1().constData(), d_ptr->filePath().toLatin1().constData());
                }
            } else {
                fprintf(stderr, "The service %s is already installed\n", serviceName().toLatin1().constData());
            }
            return 0;
        } else if (a == QLatin1String("-u") || a == QLatin1String("-uninstall")) {
            if (d_ptr->controller.isInstalled()) {
                if (!d_ptr->controller.uninstall()) {
                    fprintf(stderr, "The service %s could not be uninstalled\n", serviceName().toLatin1().constData());
                    return -1;
                } else {
                    printf("The service %s has been uninstalled.\n",
                        serviceName().toLatin1().constData());
                }
            } else {
                fprintf(stderr, "The service %s is not installed\n", serviceName().toLatin1().constData());
            }
            return 0;
        } else if (a == QLatin1String("-v") || a == QLatin1String("-version")) {
            printf("The service\n"
                "\t%s\n\t%s\n\n", serviceName().toLatin1().constData(), d_ptr->args.at(0).toLatin1().constData());
            printf("is %s", (d_ptr->controller.isInstalled() ? "installed" : "not installed"));
            printf(" and %s\n\n", (d_ptr->controller.isRunning() ? "running" : "not running"));
            return 0;
        } else if (a == QLatin1String("-e") || a == QLatin1String("-exec")) {
            d_ptr->args.removeAt(1);
            int ec = d_ptr->run(false, d_ptr->args);
            if (ec == -1)
                qErrnoWarning("The service could not be executed.");
            return ec;
        } else if (a == QLatin1String("-t") || a == QLatin1String("-terminate")) {
            if (!d_ptr->controller.stop())
                qErrnoWarning("The service could not be stopped.");
            return 0;
        } else if (a == QLatin1String("-p") || a == QLatin1String("-pause")) {
            d_ptr->controller.pause();
            return 0;
        } else if (a == QLatin1String("-r") || a == QLatin1String("-resume")) {
            d_ptr->controller.resume();
            return 0;
        } else if (a == QLatin1String("-c") || a == QLatin1String("-command")) {
            int code = 0;
            if (d_ptr->args.size() > 2)
                code = d_ptr->args.at(2).toInt();
            d_ptr->controller.sendCommand(code);
            return 0;
        } else  if (a == QLatin1String("-h") || a == QLatin1String("-help")) {
            printf("\n%s -[i|u|e|s|v|h]\n"
                   "\t-i(nstall) [account] [password]\t: Install the service, optionally using given account and password\n"
                   "\t-u(ninstall)\t: Uninstall the service.\n"
                   "\t-e(xec)\t\t: Run as a regular application. Useful for debugging.\n"
                   "\t-t(erminate)\t: Stop the service.\n"
                   "\t-c(ommand) num\t: Send command code num to the service.\n"
                   "\t-v(ersion)\t: Print version and status information.\n"
                   "\t-h(elp)   \t: Show this help\n"
                   "\tNo arguments\t: Start the service.\n",
                   d_ptr->args.at(0).toLatin1().constData());
            return 0;
        }
    }
#if defined(Q_OS_UNIX)
    if (::getenv("QTSERVICE_RUN")) {
        // Means we're the detached, real service process.
        int ec = d_ptr->run(true, d_ptr->args);
        if (ec == -1)
            qErrnoWarning("The service failed to run.");
        return ec;
    }
#endif
    if (!d_ptr->start()) {
        fprintf(stderr, "The service %s could not start\n", serviceName().toLatin1().constData());
        return -4;
    }
    return 0;
}

/*!
    \fn void GDSServiceBase::logMessage(const QString &message, MessageType type,
            int id, uint category, const QByteArray &data)

    Reports a message of the given \a type with the given \a message
    to the local system event log.  The message identifier \a id and
    the message \a category are user defined values. The \a data
    parameter can contain arbitrary binary data.

    Message strings for \a id and \a category must be provided by a
    message file, which must be registered in the system registry.
    Refer to the MSDN for more information about how to do this on
    Windows.

    \sa MessageType
*/

/*!
    Returns a pointer to the current application's GDSServiceBase
    instance.
*/
GDSServiceBase *GDSServiceBase::instance()
{
    return GDSServiceBasePrivate::instance;
}

/*!
    \fn void GDSServiceBase::start()

    This function must be implemented in GDSServiceBase subclasses in
    order to perform the service's work. Usually you create some main
    object on the heap which is the heart of your service.

    The function is only called when no service specific arguments
    were passed to the service constructor, and is called by exec()
    after it has called the executeApplication() function.

    Note that you \e don't need to create an application object or
    call its exec() function explicitly.

    \sa exec(), stop(), GDSServiceController::start()
*/

/*!
    Reimplement this function to perform additional cleanups before
    shutting down (for example deleting a main object if it was
    created in the start() function).

    This function is called in reply to controller requests. The
    default implementation does nothing.

    \sa start(), GDSServiceController::stop()
*/
void GDSServiceBase::stop()
{
}

/*!
    Reimplement this function to pause the service's execution (for
    example to stop a polling timer, or to ignore socket notifiers).

    This function is called in reply to controller requests.  The
    default implementation does nothing.

    \sa resume(), GDSServiceController::pause()
*/
void GDSServiceBase::pause()
{
}

/*!
    Reimplement this function to continue the service after a call to
    pause().

    This function is called in reply to controller requests. The
    default implementation does nothing.

    \sa pause(), GDSServiceController::resume()
*/
void GDSServiceBase::resume()
{
}

/*!
    Reimplement this function to process the user command \a code.


    This function is called in reply to controller requests.  The
    default implementation does nothing.

    \sa GDSServiceController::sendCommand()
*/
void GDSServiceBase::processCommand(int /*code*/)
{
}

/*!
    \fn void GDSServiceBase::createApplication(int &argc, char **argv)

    Creates the application object using the \a argc and \a argv
    parameters.

    This function is only called when no \l
    {serviceSpecificArguments}{service specific arguments} were
    passed to the service constructor, and is called by exec() before
    it calls the executeApplication() and start() functions.

    The createApplication() function is implemented in GDSService, but
    you might want to reimplement it, for example, if the chosen
    application type's constructor needs additional arguments.

    \sa exec(), GDSService
*/

/*!
    \fn int GDSServiceBase::executeApplication()

    Executes the application previously created with the
    createApplication() function.

    This function is only called when no \l
    {serviceSpecificArguments}{service specific arguments} were
    passed to the service constructor, and is called by exec() after
    it has called the createApplication() function and before start() function.

    This function is implemented in GDSService.

    \sa exec(), createApplication()
*/

/*!
    \class GDSService

    \brief The GDSService is a convenient template class that allows
    you to create a service for a particular application type.

    A Windows service or Unix daemon (a "service"), is a program that
    runs "in the background" independently of whether a user is logged
    in or not. A service is often set up to start when the machine
    boots up, and will typically run continuously as long as the
    machine is on.

    Services are usually non-interactive console applications. User
    interaction, if required, is usually implemented in a separate,
    normal GUI application that communicates with the service through
    an IPC channel. For simple communication,
    GDSServiceController::sendCommand() and GDSService::processCommand()
    may be used, possibly in combination with a shared settings file. For
    more complex, interactive communication, a custom IPC channel
    should be used, e.g. based on Qt's networking classes. (In certain
    circumstances, a service may provide a GUI itself, ref. the
    "interactive" example documentation).

    \bold{Note:} On Unix systems, this class relies on facilities
    provided by the QtNetwork module, provided as part of the
    \l{Qt Open Source Edition} and certain \l{Qt Commercial Editions}.

    The GDSService class functionality is inherited from GDSServiceBase,
    but in addition the GDSService class binds an instance of
    GDSServiceBase with an application type.

    Typically, you will create a service by subclassing the GDSService
    template class. For example:

    \code
    class MyService : public GDSService<QApplication>
    {
    public:
        MyService(int argc, char **argv);
        ~MyService();

    protected:
        void start();
        void stop();
        void pause();
        void resume();
        void processCommand(int code);
    };
    \endcode

    The application type can be QCoreApplication for services without
    GUI, QApplication for services with GUI or you can use your own
    custom application type.

    You must reimplement the GDSServiceBase::start() function to
    perform the service's work. Usually you create some main object on
    the heap which is the heart of your service.

    In addition, you might want to reimplement the
    GDSServiceBase::pause(), GDSServiceBase::processCommand(),
    GDSServiceBase::resume() and GDSServiceBase::stop() to intervene the
    service's process on controller requests. You can control any
    given service using an instance of the GDSServiceController class
    which also allows you to control services from separate
    applications. The mentioned functions are all virtual and won't do
    anything unless they are reimplemented.

    Your custom service is typically instantiated in the application's
    main function. Then the main function will call your service's
    exec() function, and return the result of that call. For example:

    \code
        int main(int argc, char **argv)
        {
            MyService service(argc, argv);
            return service.exec();
        }
    \endcode

    When the exec() function is called, it will parse the \l
    {serviceSpecificArguments} {service specific arguments} passed in
    \c argv, perform the required actions, and exit.

    If none of the arguments is recognized as service specific, exec()
    will first call the createApplication() function, then executeApplication() and
    finally the start() function. In the end, exec()
    returns while the service continues in its own process waiting for
    commands from the service controller.

    \sa GDSServiceBase, GDSServiceController
*/

/*!
    \fn GDSService::GDSService(int argc, char **argv, const QString &name)

    Constructs a GDSService object called \a name. The \a argc and \a
    argv parameters are parsed after the exec() function has been
    called. Then they are passed to the application's constructor.

    There can only be one GDSService object in a process.

    \sa GDSServiceBase()
*/

/*!
    \fn GDSService::~GDSService()

    Destroys the service object.
*/

/*!
    \fn Application *GDSService::application() const

    Returns a pointer to the application object.
*/

/*!
    \fn void GDSService::createApplication(int &argc, char **argv)

    Creates application object of type Application passing \a argc and
    \a argv to its constructor.

    \reimp

*/

/*!
    \fn int GDSService::executeApplication()

    \reimp
*/




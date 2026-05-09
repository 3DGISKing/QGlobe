#ifndef QTSERVICE_P_H
#define QTSERVICE_P_H

#include <QtCore/QStringList>
#include "gdsservice_common.h"

class QGlobeDSServiceControllerPrivate
{
    Q_DECLARE_PUBLIC(QGlobeDSServiceController)
public:
    QString serviceName;
    QGlobeDSServiceController *q_ptr;
};

class QGlobeDSServiceBasePrivate
{
    Q_DECLARE_PUBLIC(QGlobeDSServiceBase)
public:

    QGlobeDSServiceBasePrivate(const QString &name);
    ~QGlobeDSServiceBasePrivate();

    QGlobeDSServiceBase *q_ptr;

    QString serviceDescription;
    QGlobeDSServiceController::StartupType startupType;
    QGlobeDSServiceBase::ServiceFlags serviceFlags;
    QStringList args;

    static class QGlobeDSServiceBase *instance;

    QGlobeDSServiceController controller;

    void startService();
    int run(bool asService, const QStringList &argList);
    bool install(const QString &account, const QString &password);

    bool start();

    QString filePath() const;
    bool sysInit();
    void sysSetPath();
    void sysCleanup();
    class QtServiceSysPrivate *sysd;
};

#endif

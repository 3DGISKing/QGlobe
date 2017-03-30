#ifndef QTSERVICE_P_H
#define QTSERVICE_P_H

#include <QtCore/QStringList>
#include "gdsservice_common.h"

class GDSServiceControllerPrivate
{
    Q_DECLARE_PUBLIC(GDSServiceController)
public:
    QString serviceName;
    GDSServiceController *q_ptr;
};

class GDSServiceBasePrivate
{
    Q_DECLARE_PUBLIC(GDSServiceBase)
public:

    GDSServiceBasePrivate(const QString &name);
    ~GDSServiceBasePrivate();

    GDSServiceBase *q_ptr;

    QString serviceDescription;
    GDSServiceController::StartupType startupType;
    GDSServiceBase::ServiceFlags serviceFlags;
    QStringList args;

    static class GDSServiceBase *instance;

    GDSServiceController controller;

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

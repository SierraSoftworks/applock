#ifndef DAEMON_H
#define DAEMON_H

#include <QtCore>
#include <QtDBus>
#include "launchmonitor.h"
#include "phonecontrol.h"
#include "notifications.h"
#include "settings.h"
#include "dbusmonitor.h"

class Daemon : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("AppLock Daemon", "com.sierrasoftworks.AppLock")

public:
    explicit Daemon();
    virtual ~Daemon();

private slots:
    void OnAppLaunched(QString app);

public slots:
    void PrintMonitorList();
    void PrintStatus();
    QList<QString> GetMonitoredApplicationNames();

signals:
    void LockedAppLaunched(QString app);
    void LockedAppClosed(QString app);
    void PhoneLocked();
};


#endif // DAEMON_H

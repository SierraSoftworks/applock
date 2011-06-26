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
    void OnSessionBusMethod(QString interface, QString method);
    void OnSystemBusMethod(QString interface, QString method);

public slots:
    void PrintMonitorList();
    void PrintStatus();
    QString GetStatus();
    bool AddSearch(QString name, QString bus, QString search);
    bool AddApp(QString name, QString path);
    bool RemoveSearch(QString name);
    bool RemoveApp(QString name);

signals:
    void LockedAppLaunched(QString app);
    void PhoneLocked();
};


#endif // DAEMON_H

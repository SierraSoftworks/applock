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

private:
    PhoneControl *phoneControl;
    Settings *settings;

    DBusMonitor *sessionMonitor;
    LaunchMonitor *launchMonitor;

public:
    explicit Daemon();
    virtual ~Daemon();

private slots:
    void OnAppLaunched(QString app);
    void OnSessionBusMethod(QString interface, QString method);
    void OnSystemBusMethod(QString interface, QString method);

public slots:
    QString GetStatus();
    bool AddSearch(QString name, QString bus, QString search);
    bool AddApp(QString name, QString path);
    bool AddCMD(QString name, QString cmdline);
    bool RemoveSearch(QString name);
    bool RemoveApp(QString name);
    bool RemoveCMD(QString name);
    bool IsLockedApp(QString name);
    bool IsLockedDBus(QString name);
    bool IsLockedCMD(QString name);
    void Refresh();
    void Exit();

signals:
    void LockedAppLaunched(QString app);
    void PhoneLocked();
};


#endif // DAEMON_H

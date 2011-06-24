#ifndef LAUNCHMONITOR_H
#define LAUNCHMONITOR_H

#include <QtCore>
#include <QtDBus>

#include "applicationdescription.h"
#include "dbusmonitor.h"

class LaunchMonitor : public QThread
{
    Q_OBJECT

public:
    LaunchMonitor();
    virtual ~LaunchMonitor();
    void AddMonitoredApp(ApplicationDescription *app);
    void RemoveMonitoredApp(ApplicationDescription *app);
    void SetMonitoredApps(QList<ApplicationDescription*> apps);
    bool IsMonitoredApp(QString app);
    bool IsMonitoredDBus(QString dbus);
    QString GetDBusAppName(QString interface);
    QString GetAppPath(QString dbusName);
    ApplicationDescription* GetMonitoredApp(QString appPath);

private:
    void ProcessRunningApps();
    void ProcessDBus();

private slots:
    void OnMethodCall(QString interface, QString method);
    void OnSignal(QString interface, QString signal);
    void OnDBusLinked(QString name, QString oldName, QString newName);

public slots:
    void PrintMonitoredApps();
    void PrintMonitoredDBus();
    void PrintMonitors();

signals:
    void ApplicationLaunched(QString app);
    void ApplicationClosed(QString app);
    void DBusLaunched(QString dbus);
};

#endif // LAUNCHMONITOR_H

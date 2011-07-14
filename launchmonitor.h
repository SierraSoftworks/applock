#ifndef LAUNCHMONITOR_H
#define LAUNCHMONITOR_H

#include <QtCore>
#include <QtDBus>

#include "settings.h"

class LaunchMonitor : public QThread
{
    Q_OBJECT

public:
    LaunchMonitor();
    virtual ~LaunchMonitor();
    void SetMonitoredApps(QStringList apps, QStringList cmdlines);
    bool IsMonitoredApp(QString app);
    bool IsMonitoredCMDLine(QString cmdline);
    void AddMonitoredApp(QString app);
    void RemoveMonitoredApp(QString app);
    void AddMonitoredCMDLine(QString cmdline);
    void RemoveMonitoredCMDLine(QString cmdline);

private:
    void ProcessRunningApps();
    QString GetAppPath(quint32 appID);
    quint32 GetAppID(QString dbusID);
    QString GetAppCMDLine(quint32 appID);

private slots:
    void OnDBusLinked(QString name, QString oldName, QString newName);

signals:
    void ApplicationLaunched(QString app);
};

#endif // LAUNCHMONITOR_H

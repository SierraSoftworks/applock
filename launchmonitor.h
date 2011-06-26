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
    void SetMonitoredApps(QStringList apps);
    bool IsMonitoredApp(QString app);
    void AddMonitoredApp(QString app);

private:
    void ProcessRunningApps();

private slots:
    void OnDBusLinked(QString name, QString oldName, QString newName);

signals:
    void ApplicationLaunched(QString app);
};

#endif // LAUNCHMONITOR_H

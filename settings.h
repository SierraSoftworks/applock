#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QtSql>
#include "applicationdescription.h"

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings();
    virtual ~Settings();    
    void AddApp(ApplicationDescription app);
    void RemoveApp(ApplicationDescription app);
    bool IsMonitoredApp(QString path);
    bool IsMonitoredDBus(QString dbus);
    bool IsDBusMonitor(QString name);
    QString GetAppNamePath(QString path);
    QString GetAppNameDBus(QString dbus);
    ApplicationDescription* GetAppFromPath(QString path);
    ApplicationDescription* GetAppFromDBus(QString dbus);
    ApplicationDescription* GetAppFromName(QString name);
    QList<QString> GetAppNames();
    QVariant GetSetting(QString setting, QVariant defaultValue = QVariant(QString()));
    void SetSetting(QString setting, QVariant value);
    void ClearApps();

};

#endif // SETTINGS_H

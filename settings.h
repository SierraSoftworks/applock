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
    void SetLockedApps(QList<ApplicationDescription*> apps);
    QList<ApplicationDescription*> GetLockedApps();
    void AddApp(ApplicationDescription *app);
    QVariant GetSetting(QString setting, QVariant defaultValue = QVariant(QString()));
    void SetSetting(QString setting, QVariant value);

};

#endif // SETTINGS_H

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

    void AddSearch(QString name, int bus, QString watch);
    QStringList GetSearches(int bus);
    QStringList GetSearchNames(int bus);
    void RemoveSearch(QString name);

    void AddApp(QString name, QString path);
    QStringList GetApps();
    QStringList GetAppNames();
    void RemoveApp(QString name);

    void ClearApps();
    void ClearSearches();
};

#endif // SETTINGS_H

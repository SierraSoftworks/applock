#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QtSql>
#include "applicationdescription.h"

class Settings : public QObject
{
    Q_OBJECT

private:
    bool Connect();
    void CreateDB();

public:
    Settings();
    virtual ~Settings();    

    void AddSearch(QString name, int bus, QString watch);
    QStringList GetSearches(int bus);
    QStringList GetSearchNames(int bus);
    void RemoveSearch(QString name);
    QString GetSearch(QString name, int bus);
    bool IsSearch(QString name, int bus);

    QStringList GetCommandLines();
    QString GetCommandLine(QString name);
    void AddCommandLine(QString name, QString cmdline);
    void RemoveCommandLine(QString name);
    QStringList GetCommandLineNames();
    bool IsCommandLine(QString name);

    void AddApp(QString name, QString path);
    QStringList GetApps();
    QStringList GetAppNames();
    void RemoveApp(QString name);
    QString GetApp(QString name);
    bool IsApp(QString name);

    void ClearApps();
    void ClearSearches();
    void ClearCMDLines();
};

#endif // SETTINGS_H

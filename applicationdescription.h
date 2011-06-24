#ifndef APPLICATIONDESCRIPTION_H
#define APPLICATIONDESCRIPTION_H

#include <QtCore>

class ApplicationDescription
{
public:
    ApplicationDescription();
    ApplicationDescription(QString desktopFile);
    ApplicationDescription(QString name, QString path, QString icon, QString dbus, QString desktopFile);
    virtual ~ApplicationDescription();
    bool Load(QString desktopFile);
    QString getAppName();
    QString getAppIcon();
    QString getAppPath();
    QString getAppDBus();
    QString getAppDesktopFile();
    bool isNull();
    bool IsDBusService();

};

#endif // APPLICATIONDESCRIPTION_H

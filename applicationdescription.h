#ifndef APPLICATIONDESCRIPTION_H
#define APPLICATIONDESCRIPTION_H

#include <QtCore>

class ApplicationDescription : public QObject
{
    Q_OBJECT

public:
    ApplicationDescription();
    ApplicationDescription(QString desktopFile);
    virtual ~ApplicationDescription();
    bool Load(QString desktopFile);
    QString getAppName();
    QString getAppIcon();
    QString getAppPath();
    QString getAppDBus();
    QString getAppDesktopFile();
    bool IsDBusService();

};

#endif // APPLICATIONDESCRIPTION_H

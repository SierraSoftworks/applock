#ifndef APPLICATIONDESCRIPTION_H
#define APPLICATIONDESCRIPTION_H

#include <QtCore>
#include "filelocator.h"
#include "locale.h"
#include "locale.h"
#include "glib/gi18n.h"
#include "substitutionfileparser.h"

class ApplicationDescription
{
public:
    ApplicationDescription();
    ApplicationDescription(const ApplicationDescription &app);
    ApplicationDescription(QString desktopFile);
    ApplicationDescription(QString name, QString path, QString icon, QString dbus, QString desktopFile);
    virtual ~ApplicationDescription();
    bool Load(QString desktopFile);
    QString getAppName();
    QString getAppIcon();
    QString getAppPath();
    QString getAppDBus();
    QString getAppDesktopFile();
    QString getAppDescription();
    QString getAppCMDLine();
    bool isNull();
    bool IsDBusService();
    bool isScript();
    bool isComplex();

private:
    QString appName;
    QString appIcon;
    QString appPath;
    QString appDBus;
    QString appDesktopFile;
    QString appDescription;
    QString commandLine;

    bool isnull;

    void LoadServiceFile(QString file);

};

#endif // APPLICATIONDESCRIPTION_H

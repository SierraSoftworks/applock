#ifndef UIAPPDESC_H
#define UIAPPDESC_H

#include "applicationdescription.h"
#include <QtDBus>
#include <QtCore>
#include "substitutionfileparser.h"
#include "settings.h"

class UIAppDesc : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getAppName NOTIFY NameChanged)
    Q_PROPERTY(QString icon READ getAppIcon NOTIFY IconChanged)
    Q_PROPERTY(QString path READ getAppPath NOTIFY PathChanged)
    Q_PROPERTY(QString dbus READ getAppDBus NOTIFY DBusChanged)
    Q_PROPERTY(QString desktopFile READ getAppDesktopFile NOTIFY DesktopFileChanged)
    Q_PROPERTY(QString description READ getAppDescription NOTIFY DescriptionChanged)
    Q_PROPERTY(QString cmdline READ getAppCMDLine)
    Q_PROPERTY(bool service READ IsDBusService)
    Q_PROPERTY(bool complex READ IsComplexApp)
    Q_PROPERTY(bool locked READ IsLocked NOTIFY lockedChanged)
    Q_PROPERTY(bool checked READ IsChecked WRITE SetChecked NOTIFY checkChanged)
    Q_PROPERTY(bool script READ IsScript)

private:
    ApplicationDescription* appInstance;

#ifndef DBUSSETTINGS
    Settings* settings;
#endif
    bool islocked;
    bool hasUpdated;
    bool isChecked;
    void updateLock();
    bool isLocked();

public:
#ifndef DBUSSETTINGS
    UIAppDesc(ApplicationDescription* app, Settings* settings);
#else
    UIAppDesc(ApplicationDescription* app);
#endif
    bool IsLocked();
    bool IsComplexApp();
    bool IsScript();
    QString getAppName();
    QString getAppIcon();
    QString getAppPath();
    QString getAppDBus();
    QString getAppDesktopFile();
    QString getAppDescription();
    QString getAppCMDLine();
    bool IsDBusService();
    void OnLockChanged();
    bool IsChecked();
    void SetChecked(bool checked);

signals:
    void NameChanged();
    void IconChanged();
    void PathChanged();
    void DBusChanged();
    void DesktopFileChanged();
    void lockedChanged();
    void checkChanged();
    void DescriptionChanged();

};

#endif // UIAPPDESC_H

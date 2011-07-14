#ifndef UIDAEMONHANDLER_H
#define UIDAEMONHANDLER_H

#include <QtCore>
#include <QtDBus>
#include "desktopfileloader.h"
#include "applicationdescription.h"
#include "qmldaemoninterface.h"
#include "substitutionfileparser.h"
#include "uiappdesc.h"
#include "phonecontrol.h"

#ifndef DBUSSETTINGS
#include "settings.h"
#endif

class UIDaemonHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> applist READ getAppList NOTIFY appListChanged)

public:
    UIDaemonHandler(QObject* qmlContext);
    ~UIDaemonHandler();
    void UpdateAppList();

private slots:
    void onfileloaded(ApplicationDescription* app);
    void onfileloadingcomplete();
    void unlockFinished(QDBusPendingCallWatcher* watcher);
    void lockFinished(QDBusPendingCallWatcher* watcher);

public slots:
    void AddApp(QString name, QString path);
    void AddSearch(QString name, QString search);
    void AddSearch(QString name, QString interface, QString method);
    void AddDBus(QString name);
    void AddComplex(QString name);
    void AddCMDLine(QString name);
    bool IsMonitored(QString name);
    void RemoveApp(QString name);
    void RemoveSearch(QString name);
    void RemoveDBus(QString name);
    void RemoveComplex(QString name);
    void RemoveCMDLine(QString name);
    bool IsService(QString name);
    bool IsComplex(QString name);
    bool IsScript(QString name);
    QString DaemonStatus();
    QList<QObject*> getAppList();
    QString getAppDBus(QString name);
    QString getAppPath(QString name);
    QString getAppIcon(QString name);
    QString getAppDescription(QString name);
    QString getAppCMDLine(QString name);
    void ShowTaskSwitcher();
    void Exit();
    void Restart();
    void LockPhone();
    void ExitDaemon();
    void UnlockAll();

signals:
    void appListChanged();

};

#endif // UIDAEMONHANDLER_H

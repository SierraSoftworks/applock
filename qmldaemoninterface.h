#ifndef QMLDAEMONINTERFACE_H
#define QMLDAEMONINTERFACE_H

#include <QtDeclarative>

class QMLDaemonInterface : public QDeclarativeItem
{
    Q_OBJECT

public:
    QMLDaemonInterface();
    ~QMLDaemonInterface();
    static void AppLoaded(QString name, int totalApps);
    static void AllAppsLoaded(int totalApps);
    static void AppUnlocked();
    static void AppLocked();
    static void Error(QString msg);

private:
    void OnAppLoaded(QString name, int totalApps);
    void OnAllAppsLoaded(int totalApps);
    void OnAppUnlocked();
    void OnAppLocked();
    void OnError(QString message);
    \
signals:
    void applicationLoaded(const QString& name, const int& totalApps);
    void applicationLoadingComplete(const int& totalApps);
    void applicationUnlocked();
    void applicationLocked();
    void error(QString message);
};

#endif // QMLDAEMONINTERFACE_H

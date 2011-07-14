#include "uidaemonhandler.h"

QDBusInterface *dbus;
QMap<QString,QObject*> appList;
DesktopFileLoader* loader;
QObject* qml;

#ifndef DBUSSETTINGS
Settings *settings;
#endif


int totalApps = 0;

UIDaemonHandler::UIDaemonHandler(QObject* qmlContext)
{
    qml = qmlContext;

    dbus = new QDBusInterface("com.sierrasoftworks.AppLock","/com/sierrasoftworks/AppLock","com.sierrasoftworks.AppLock.Daemon");

    loader = new DesktopFileLoader();

    connect(loader, SIGNAL(FileLoaded(ApplicationDescription*)), this, SLOT(onfileloaded(ApplicationDescription*)));
    connect(loader, SIGNAL(FileLoadingComplete()), this, SLOT(onfileloadingcomplete()));

#ifndef DBUSSETTINGS
    settings = new Settings();
#endif

    PhoneControl::LockPhone();
}

UIDaemonHandler::~UIDaemonHandler()
{

}

void UIDaemonHandler::ShowTaskSwitcher()
{
    QDBusMessage message = QDBusMessage::createSignal("/","com.nokia.hildon_desktop","exit_app_view");
    QDBusConnection::sessionBus().send(message);
}

void UIDaemonHandler::Exit()
{
    exit(0);
}

QList<QObject*> UIDaemonHandler::getAppList()
{
    return appList.values();
}

void UIDaemonHandler::UpdateAppList()
{
    loader->LoadFiles();
}

void UIDaemonHandler::AddSearch(QString name, QString search)
{
    if(SubstitutionFileParser::IsSubstituted(((UIAppDesc*)appList[name])->getAppDesktopFile()))
    {
	AddComplex(name);
    }
    else
    {
	//QDBusPendingReply<bool> result = dbus->asyncCall("AddSearch",name,"session",search);
	//QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result);
	//connect(watcher,SIGNAL(finished(QDBusPendingCallWatcher*)),this, SLOT(lockFinished(QDBusPendingCallWatcher*)));
    #ifndef DBUSSETTINGS
	    settings->AddSearch(name, 1, search);
    #endif
	    dbus->call("AddSearch",name,"session",search);
	    ((UIAppDesc*)appList[name])->OnLockChanged();
    }
}

void UIDaemonHandler::AddSearch(QString name, QString interface, QString method)
{
    if(SubstitutionFileParser::IsSubstituted(((UIAppDesc*)appList[name])->getAppDesktopFile()))
    {
	AddComplex(name);
    }
    else
    {
	QString search = "type='method_call',interface='" + interface + "',member='" + method + "'";
#ifndef DBUSSETTINGS
	settings->AddSearch(name,1, search);
#endif
	dbus->call("AddSearch",name,"session",search);
	((UIAppDesc*)appList[name])->OnLockChanged();
    //QDBusPendingCall result = dbus->asyncCall("AddSearch",name,"session","type='method_call',interface='" + interface + "',member='" + method + "'");

    //QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result);
    //connect(watcher,SIGNAL(finished(QDBusPendingCallWatcher*)),this, SLOT(lockFinished(QDBusPendingCallWatcher*)));
    }
}

void UIDaemonHandler::AddApp(QString name, QString exe)
{
#ifndef DBUSSETTINGS
    settings->AddApp(name, exe);
#endif
    dbus->call("AddApp", name, exe);
    ((UIAppDesc*)appList[name])->OnLockChanged();
    //QDBusPendingCall result = dbus->asyncCall("AddApp", name, exe);
    //QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result);
    //connect(watcher,SIGNAL(finished(QDBusPendingCallWatcher*)),this, SLOT(lockFinished(QDBusPendingCallWatcher*)));
}

bool UIDaemonHandler::IsMonitored(QString name)
{
    return ((UIAppDesc*)appList[name])->IsLocked();
}

bool UIDaemonHandler::IsService(QString name)
{
    if(((UIAppDesc*)appList[name])->IsDBusService())
	return true;
    return false;
}

bool UIDaemonHandler::IsComplex(QString name)
{
    return SubstitutionFileParser::IsSubstituted(((UIAppDesc*)appList[name])->getAppDesktopFile());
}

void UIDaemonHandler::RemoveApp(QString name)
{
#ifndef DBUSSETTINGS
    settings->RemoveApp(name);
#endif
    dbus->call("RemoveApp",name);
    ((UIAppDesc*)appList[name])->OnLockChanged();
    //QDBusPendingCall result = dbus->asyncCall("RemoveApp",name);
    //QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result);
    //connect(watcher,SIGNAL(finished(QDBusPendingCallWatcher*)),this, SLOT(unlockFinished(QDBusPendingCallWatcher*)));
}

void UIDaemonHandler::RemoveSearch(QString name)
{
    if(SubstitutionFileParser::IsSubstituted(((UIAppDesc*)appList[name])->getAppDesktopFile()))
    {
	RemoveComplex(name);
    }
    else
    {
#ifndef DBUSSETTINGS
	settings->RemoveSearch(name);
#endif
	dbus->call("RemoveSearch",name);
	((UIAppDesc*)appList[name])->OnLockChanged();
	emit appListChanged();
	//QDBusPendingCall result = dbus->asyncCall("RemoveSearch",name);
	//QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result);
	//connect(watcher,SIGNAL(finished(QDBusPendingCallWatcher*)),this, SLOT(unlockFinished(QDBusPendingCallWatcher*)));
    }
}

void UIDaemonHandler::unlockFinished(QDBusPendingCallWatcher *watcher)
{
    if(watcher->isError())
	QMLDaemonInterface::Error(watcher->error().message());
    else
	QMLDaemonInterface::AppUnlocked();
}

void UIDaemonHandler::lockFinished(QDBusPendingCallWatcher *watcher)
{
    if(watcher->isError())
	QMLDaemonInterface::Error(watcher->error().message());
    else
	QMLDaemonInterface::AppLocked();
}

QString UIDaemonHandler::DaemonStatus()
{
    QDBusReply<QString> reply = dbus->call("GetStatus");
    if(!reply.isValid() || reply.error().type() != QDBusError::NoError)
	return "Not Running";

    return reply.value();
}

void UIDaemonHandler::ExitDaemon()
{
    dbus->call("Exit");
}

QString UIDaemonHandler::getAppPath(QString name)
{
    return ((UIAppDesc*)appList[name])->getAppPath();
}

QString UIDaemonHandler::getAppDBus(QString name)
{
    return ((UIAppDesc*)appList[name])->getAppDBus();
}

QString UIDaemonHandler::getAppIcon(QString name)
{
    return ((UIAppDesc*)appList[name])->getAppIcon();
}

QString UIDaemonHandler::getAppDescription(QString name)
{
    return ((UIAppDesc*)appList[name])->getAppDescription();
}

QString UIDaemonHandler::getAppCMDLine(QString name)
{
    return ((UIAppDesc*)appList[name])->getAppCMDLine();
}

void UIDaemonHandler::onfileloaded(ApplicationDescription *app)
{
    if(app->isNull())
	return;
#ifdef DBUSSETTINGS
    appList.insert(app->getAppName(),new UIAppDesc(app));
#else
    appList.insert(app->getAppName(),new UIAppDesc(app, settings));
#endif

    totalApps++;

    QMLDaemonInterface::AppLoaded(app->getAppName(),totalApps);
}

void UIDaemonHandler::onfileloadingcomplete()
{
    QMLDaemonInterface::AllAppsLoaded(totalApps);
}

void UIDaemonHandler::Restart()
{
    dbus->call(QDBus::Block, "Refresh");
}

void UIDaemonHandler::LockPhone()
{
    PhoneControl::LockPhone();
}

bool UIDaemonHandler::IsScript(QString name)
{
    return ((UIAppDesc*)appList[name])->IsScript();
}

void UIDaemonHandler::AddCMDLine(QString name)
{
    dbus->call("AddCMD", name, ((UIAppDesc*)appList[name])->getAppCMDLine());
    ((UIAppDesc*)appList[name])->OnLockChanged();
}

void UIDaemonHandler::RemoveCMDLine(QString name)
{
    dbus->call("RemoveCMD",name);
    ((UIAppDesc*)appList[name])->OnLockChanged();
}

void UIDaemonHandler::AddDBus(QString name)
{
    QString dbusFilter = "type='method_call',interface='" + ((UIAppDesc*)appList[name])->getAppDBus() + "',member='top_application'";

#ifndef DBUSSETTINGS
    settings->AddSearch(name,1,dbusFilter);
#endif

    dbus->call(QDBus::NoBlock,"AddSearch",name,"session",dbusFilter);


    ((UIAppDesc*)appList[name])->OnLockChanged();
}

void UIDaemonHandler::RemoveDBus(QString name)
{
#ifndef DBUSSETTINGS
    settings->RemoveSearch(name);
#endif

    dbus->call(QDBus::NoBlock,"RemoveSearch",name);


    ((UIAppDesc*)appList[name])->OnLockChanged();
}

void UIDaemonHandler::AddComplex(QString name)
{
    if(SubstitutionFileParser::IsSubstituted(((UIAppDesc*)appList[name])->getAppDesktopFile()))
    {
	qDebug() << "Locking Complex App";
	QMap<QString,QString> dbusList = SubstitutionFileParser::GetApplicationDBusSearches(((UIAppDesc*)appList[name])->getAppDesktopFile());
	QMap<QString,QString> pathList = SubstitutionFileParser::GetApplicationPaths(((UIAppDesc*)appList[name])->getAppDesktopFile());
	QMap<QString,QString> cmdList = SubstitutionFileParser::GetApplicationCommandLines(((UIAppDesc*)appList[name])->getAppDesktopFile());

	QDBusReply<bool> result;

	for(int i = 0; i < dbusList.count(); i++)
	{
#ifndef DBUSSETTINGS
	    settings->AddSearch(dbusList.keys().at(i),1,dbusList.values().at(i));
#endif
	    dbus->call("AddSearch",dbusList.keys()[i],"session",dbusList.values().at(i));
	}

	for(int i = 0; i < pathList.count(); i++)
	{
#ifndef DBUSSETTINGS
	    settings->AddApp(pathList.keys().at(i),pathList.values().at(i));
#endif

	    dbus->call("AddPath", pathList.keys().at(i),pathList.values().at(i));
	}

	for(int i = 0; i < cmdList.count(); i++)
	{
#ifndef DBUSSETTINGS
	    settings->AddCommandLine(cmdList.keys().at(i),cmdList.values().at(i));
#endif
	    dbus->call("AddCMD",cmdList.keys().at(i),cmdList.values().at(i));
	}

	((UIAppDesc*)appList[name])->OnLockChanged();
	QMLDaemonInterface::AppLocked();
    }
}

void UIDaemonHandler::RemoveComplex(QString name)
{
    if(SubstitutionFileParser::IsSubstituted(((UIAppDesc*)appList[name])->getAppDesktopFile()))
    {
	qDebug() << "Unlocking Complex App:" << name;
	QMap<QString,QString> dbusList = SubstitutionFileParser::GetApplicationDBusSearches(((UIAppDesc*)appList[name])->getAppDesktopFile());
	QMap<QString,QString> pathList = SubstitutionFileParser::GetApplicationPaths(((UIAppDesc*)appList[name])->getAppDesktopFile());
	QMap<QString,QString> cmdList = SubstitutionFileParser::GetApplicationCommandLines(((UIAppDesc*)appList[name])->getAppDesktopFile());

	for(int i = 0; i < dbusList.count(); i++)
	{
#ifndef DBUSSETTINGS
	    settings->RemoveSearch(dbusList.keys().at(i));
#endif
	    dbus->call("RemoveSearch",dbusList.keys().at(i));
	}

	for(int i = 0; i < pathList.count(); i++)
	{
#ifndef DBUSSETTINGS
	    settings->RemoveApp(pathList.keys().at(i));
#endif
	    dbus->call("RemoveApp",pathList.keys().at(i));
	}

	for(int i = 0; i < cmdList.count(); i++)
	{
#ifndef DBUSSETTINGS
	    settings->RemoveCommandLine(cmdList.keys().at(i));
#endif
	    dbus->call("RemoveCMD",cmdList.keys().at(i));
	}

	((UIAppDesc*)appList[name])->OnLockChanged();
	QMLDaemonInterface::AppUnlocked();

	emit appListChanged();
    }

}

void UIDaemonHandler::UnlockAll()
{
    for(int i = 0; i < appList.count(); i++)
    {
	if(!((UIAppDesc*)appList.values().at(i))->IsLocked())
	    continue;

	if(((UIAppDesc*)appList.values().at(i))->IsComplexApp())
	{


	}
	else if(((UIAppDesc*)appList.values().at(i))->IsDBusService())
	{
	    RemoveSearch(((UIAppDesc*)appList.values().at(i))->getAppName());
	}
	else if(((UIAppDesc*)appList.values().at(i))->IsScript())
	{
	    RemoveCMDLine(((UIAppDesc*)appList.values().at(i))->getAppName());
	}
	else
	{
	    RemoveApp(((UIAppDesc*)appList.values().at(i))->getAppName());
	}
    }
}

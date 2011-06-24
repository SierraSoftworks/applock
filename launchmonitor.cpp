#include "launchmonitor.h"

bool firstRun = true;
QStringList runningApps;

Settings * thisSettings;

DBusMonitor *monitor;
QDBusInterface *freedesktopInterface;


LaunchMonitor::LaunchMonitor()
{
    QStringList dbusFilters;
    dbusFilters << "type='method_call',member='top_application'";

    monitor = new DBusMonitor(dbusFilters);
    connect(monitor,SIGNAL(MethodCall(QString,QString)), this, SLOT(OnMethodCall(QString,QString)));
    connect(monitor,SIGNAL(Signal(QString,QString)), this, SLOT(OnSignal(QString,QString)));
    monitor->Start();

    freedesktopInterface = new QDBusInterface(
		"org.freedesktop.DBus",
		"/org/freedesktop/DBus",
		"org.freedesktop.DBus",
		QDBusConnection::sessionBus());

    connect(freedesktopInterface->connection().interface(),
	    SIGNAL(serviceOwnerChanged(QString,QString,QString)),
	    this, SLOT(OnDBusLinked(QString,QString,QString)));
}

LaunchMonitor::~LaunchMonitor()
{

}

QString LaunchMonitor::GetAppPath(QString dbusName)
{
    QDBusReply<quint32> reply = freedesktopInterface->call("GetConnectionUnixProcessID",dbusName);
    if(!reply.isValid())
	return QString();

    QFileInfo exe("/proc/" + QString::number(reply.value()) + "/exe");
    if(exe.isSymLink())
	return exe.symLinkTarget();
    return QString();
}

ApplicationDescription* LaunchMonitor::GetMonitoredApp(QString appPath)
{
    return thisSettings->GetAppFromPath(appPath);
}

void LaunchMonitor::SetSettings(Settings *settings)
{
    thisSettings = settings;
}

bool LaunchMonitor::IsMonitoredApp(QString app)
{
    ApplicationDescription *appDes = thisSettings->GetAppFromPath(app);
    if(appDes->isNull())
	return false;
    return !appDes->IsDBusService();
}

bool LaunchMonitor::IsMonitoredDBus(QString interface)
{
    ApplicationDescription *appDes = thisSettings->GetAppFromDBus(interface);
    if(appDes->isNull())
	return false;
    return appDes->IsDBusService();
}

QString LaunchMonitor::GetDBusAppName(QString interface)
{
    return thisSettings->GetAppNameDBus(interface);
}

void LaunchMonitor::ProcessRunningApps()
{
    QStringList newRunningApps;
    QDir processes = QDir("/proc");
    QDir proc;
    QStringList processList = processes.entryList();
    foreach(QString process, processList)
    {
	proc = QDir(processes.absolutePath() + "/" + process);
	if(!proc.isReadable())
	    continue;

	QString appEXE = QFileInfo(proc.absoluteFilePath("exe")).symLinkTarget();
	if(appEXE.isEmpty() || appEXE.isNull())
	    continue;

	if(IsMonitoredApp(appEXE))
	    newRunningApps << appEXE;
    }

    /*
    if(!firstRun)
	for(int i = 0; i < monitoredApps.count(); i++)
	{
	    ApplicationDescription *monitored = monitoredApps[i];
	    if(!runningApps.contains(monitored->getAppPath(),Qt::CaseInsensitive) && newRunningApps.contains(monitored->getAppPath(), Qt::CaseInsensitive))
		emit ApplicationLaunched(monitored->getAppName());
	    else if(!newRunningApps.contains(monitored->getAppPath()) && runningApps.contains(monitored->getAppPath(), Qt::CaseInsensitive))
		emit ApplicationClosed(monitored->getAppName());
	}

	//*/
    runningApps = newRunningApps;
    firstRun = false;
}

void LaunchMonitor::OnMethodCall(QString interface, QString method)
{
    if(IsMonitoredDBus(interface) && method == "top_application")
    {
	emit ApplicationLaunched(GetDBusAppName(interface));
    }
}

void LaunchMonitor::OnSignal(QString interface, QString signal)
{
    if(signal == "tklock_mode_ind")
    {
	//Check phone's lock state
    }
}

void LaunchMonitor::OnDBusLinked(QString name, QString oldName, QString newName)
{
    //qDebug() << "DBus Linked Name=" << name << " OldName=" << oldName << " NewName=" << newName;
    if((oldName.isEmpty() || oldName.isNull()) && !(newName.isEmpty() || newName.isNull()))
    {
	//qDebug() << "DBLL - New App Launched";
	//New app launched
	QString appPath = GetAppPath(newName);
	//qDebug() << "New App Path:" << appPath;
	if(IsMonitoredApp(appPath))
	{
	    emit ApplicationLaunched(thisSettings->GetAppNamePath(appPath));
	    //qDebug() << "DBus Detected an app launch:" << monitoredApp->getAppName();
	}

	//ProcessRunningApps();
    }
    else if(!(oldName.isEmpty() || oldName.isNull()) && (newName.isEmpty() || newName.isNull()))
    {
	//App closed
	//ProcessRunningApps();
    }
}

void LaunchMonitor::PrintMonitoredApps()
{
    QList<QString> lockedApps = thisSettings->GetAppNames();
    for(int i = 0; i < lockedApps.count(); i++)
    {
	ApplicationDescription *app = thisSettings->GetAppFromName(lockedApps[i]);
	if(!app->IsDBusService())
	    qDebug() << app->getAppName() << "-" << app->getAppPath();
	delete app;
    }
}

void LaunchMonitor::PrintMonitoredDBus()
{
    QList<QString> lockedApps = thisSettings->GetAppNames();
    for(int i = 0; i < lockedApps.count(); i++)
    {
	ApplicationDescription *app = thisSettings->GetAppFromName(lockedApps[i]);
	if(app->IsDBusService())
	    qDebug() << app->getAppName() << "-" << app->getAppDBus();
	delete app;
    }
}

void LaunchMonitor::PrintMonitors()
{
    QList<QString> lockedApps = thisSettings->GetAppNames();
    for(int i = 0; i < lockedApps.count(); i++)
    {
	ApplicationDescription *app = thisSettings->GetAppFromName(lockedApps[i]);
	if(app->IsDBusService())
	    qDebug() << app->getAppName() << "-" << app->getAppDBus();
	else
	    qDebug() << app->getAppName() << "-" << app->getAppPath();
	delete app;
    }
}

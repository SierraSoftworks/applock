#include "launchmonitor.h"

bool firstRun = true;
QStringList runningApps;
QList<ApplicationDescription*> monitoredApps;

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
    for(int i = 0; i < monitoredApps.count(); i++)
	if(monitoredApps[i]->getAppPath() == appPath)
	    return monitoredApps[i];
    return 0;
}

void LaunchMonitor::AddMonitoredApp(ApplicationDescription *app)
{
    if(!IsMonitoredApp(app->getAppPath()))
	monitoredApps << app;
}

void LaunchMonitor::RemoveMonitoredApp(ApplicationDescription *app)
{
    monitoredApps.removeAll(app);
}

void LaunchMonitor::SetMonitoredApps(QList<ApplicationDescription*> apps)
{
    monitoredApps = apps;
}

bool LaunchMonitor::IsMonitoredApp(QString app)
{
    for(int i = 0; i < monitoredApps.count(); i++)
	if(!monitoredApps[i]->IsDBusService() && monitoredApps[i]->getAppPath() == app)
	    return true;
    return false;
}

bool LaunchMonitor::IsMonitoredDBus(QString interface)
{
    for(int i = 0; i < monitoredApps.count(); i++)
	if(monitoredApps[i]->IsDBusService() && monitoredApps[i]->getAppDBus() == interface)
	    return true;
    return false;
}

QString LaunchMonitor::GetDBusAppName(QString interface)
{
    for(int i = 0; i < monitoredApps.count(); i++)
	if(monitoredApps[i]->IsDBusService() && monitoredApps[i]->getAppDBus() == interface)
	    return monitoredApps[i]->getAppName();
    return interface;
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

    if(!firstRun)
	for(int i = 0; i < monitoredApps.count(); i++)
	{
	    ApplicationDescription *monitored = monitoredApps[i];
	    if(!runningApps.contains(monitored->getAppPath(),Qt::CaseInsensitive) && newRunningApps.contains(monitored->getAppPath(), Qt::CaseInsensitive))
		emit ApplicationLaunched(monitored->getAppName());
	    else if(!newRunningApps.contains(monitored->getAppPath()) && runningApps.contains(monitored->getAppPath(), Qt::CaseInsensitive))
		emit ApplicationClosed(monitored->getAppName());
	}

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
	qDebug() << "New App Path:" << appPath;
	if(IsMonitoredApp(appPath))
	{
	    ApplicationDescription *monitoredApp = GetMonitoredApp(appPath);
	    //emit ApplicationLaunched(monitoredApp->getAppName());
	    qDebug() << "DBus Detected an app launch:" << monitoredApp->getAppName();
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
    for(int i = 0; i < monitoredApps.count();i++)
    {
	if(monitoredApps[i]->IsDBusService())
	   continue;

	qDebug() << "  " << monitoredApps[i]->getAppName() << ":" << monitoredApps[i]->getAppPath();
    }
}

void LaunchMonitor::PrintMonitoredDBus()
{
    for(int i = 0; i < monitoredApps.count();i++)
    {
	if(!monitoredApps[i]->IsDBusService())
	   continue;

	qDebug() << "  " << monitoredApps[i]->getAppName() << ":" << monitoredApps[i]->getAppDBus();
    }
}

void LaunchMonitor::PrintMonitors()
{
    for(int i = 0; i < monitoredApps.count();i++)
    {
	if(monitoredApps[i]->IsDBusService())
	    qDebug() << "  " << monitoredApps[i]->getAppName() << ":" << monitoredApps[i]->getAppDBus();
	else
	    qDebug() << "  " << monitoredApps[i]->getAppName() << ":" << monitoredApps[i]->getAppPath();
    }
}

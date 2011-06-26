#include "launchmonitor.h"

bool firstRun = true;
QStringList monitoredApps;
QDBusInterface *freedesktopInterface;


LaunchMonitor::LaunchMonitor()
{

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

QString GetAppPath(QString appID)
{
    QDBusReply<quint32> reply = freedesktopInterface->call("GetConnectionUnixProcessID", appID);
    if(!reply.isValid())
	return QString();

    QFileInfo file("/proc/" + QString::number(reply.value()) + "/exe");
    if(file.exists() && file.isSymLink())
	return file.symLinkTarget();

    return QString();
}

void LaunchMonitor::SetMonitoredApps(QStringList apps)
{
    monitoredApps = apps;
}

bool LaunchMonitor::IsMonitoredApp(QString app)
{
    return monitoredApps.contains(app,Qt::CaseSensitive);
}

void LaunchMonitor::OnDBusLinked(QString name, QString oldName, QString newName)
{
    //qDebug() << "NameOwnerChanged(" << name << "," << oldName << "," << newName << ")";
    if((oldName.isEmpty() || oldName.isNull()) && !(newName.isEmpty() || newName.isNull()))
    {
	QString appPath = GetAppPath(name);
	if(!appPath.isNull() && !appPath.isEmpty() && IsMonitoredApp(appPath))
	{
	    emit ApplicationLaunched(appPath);
	}
    }
}

void LaunchMonitor::AddMonitoredApp(QString app)
{
    monitoredApps << app;
}

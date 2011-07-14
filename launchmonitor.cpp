#include "launchmonitor.h"

bool firstRun = true;
QStringList monitoredApps;
QStringList monitoredCMDLines;
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

quint32 LaunchMonitor::GetAppID(QString dbusID)
{
    QDBusReply<quint32> reply = freedesktopInterface->call("GetConnectionUnixProcessID", dbusID);
    if(!reply.isValid())
	return 0;

    return reply.value();
}

QString	LaunchMonitor::GetAppPath(quint32 appID) {

    QFileInfo file("/proc/" + QString::number(appID) + "/exe");
    if(file.exists() && file.isSymLink())
	return file.symLinkTarget();

    return QString();
}

QString LaunchMonitor::GetAppCMDLine(quint32 appID)
{
    QFileInfo file("/proc/" + QString::number(appID) + "/cmdline");
    if(!file.exists())
	return QString();

    QFile fr(file.absoluteFilePath());
    fr.open(QFile::ReadOnly);
    QString cmdLine;
    if(fr.isReadable())
	cmdLine = fr.readAll();
    fr.close();
    return cmdLine;
}


void LaunchMonitor::SetMonitoredApps(QStringList apps, QStringList cmdlines)
{
    monitoredApps = apps;
    monitoredCMDLines = cmdlines;
}

bool LaunchMonitor::IsMonitoredApp(QString app)
{
    return monitoredApps.contains(app,Qt::CaseSensitive);
}

bool LaunchMonitor::IsMonitoredCMDLine(QString cmdline)
{
    for(int i = 0; i < monitoredCMDLines.count(); i++)
	if(monitoredCMDLines.at(i).contains(cmdline,Qt::CaseSensitive))
	    return true;
    return false;
}

void LaunchMonitor::OnDBusLinked(QString name, QString oldName, QString newName)
{
    //qDebug() << "NameOwnerChanged(" << name << "," << oldName << "," << newName << ")";
    if((oldName.isEmpty() || oldName.isNull()) && !(newName.isEmpty() || newName.isNull()))
    {
	quint32 id = GetAppID(name);

	if(id == 0)
	    return;

	QString appPath = GetAppPath(id);
	if(!appPath.isNull() && !appPath.isEmpty() && IsMonitoredApp(appPath))
	{
	    emit ApplicationLaunched(appPath);
	    return;
	}

	QString appCMDLine = GetAppCMDLine(id);
	if(!appCMDLine.isNull() && !appCMDLine.isEmpty() && IsMonitoredCMDLine(appCMDLine))
	{
	    emit ApplicationLaunched(appPath);
	    return;
	}
    }
}

void LaunchMonitor::AddMonitoredApp(QString app)
{
    monitoredApps << app;
}

void LaunchMonitor::RemoveMonitoredApp(QString app)
{
    monitoredApps.removeAll(app);
}

void LaunchMonitor::AddMonitoredCMDLine(QString cmdline)
{
    monitoredCMDLines << cmdline;
}

void LaunchMonitor::RemoveMonitoredCMDLine(QString cmdline)
{
    monitoredCMDLines.removeAll(cmdline);
}

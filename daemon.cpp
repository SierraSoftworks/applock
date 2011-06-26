#include "daemon.h"

PhoneControl *phoneControl;
Settings *settings;

DBusMonitor *systemMonitor;
DBusMonitor *sessionMonitor;
LaunchMonitor *launchMonitor;

Daemon::Daemon()
{
    qDebug() << "AppLock Daemon - Loading Settings";
    settings = new Settings();

    phoneControl = new PhoneControl();

    qDebug() << "Loading Application Monitor";
    launchMonitor = new LaunchMonitor();
    connect(launchMonitor,SIGNAL(ApplicationLaunched(QString)), this, SLOT(OnAppLaunched(QString)));
    launchMonitor->SetMonitoredApps(settings->GetApps());

    //qDebug() << "Loading System Bus Monitor";

    //systemMonitor = new DBusMonitor(2, settings->GetSearches(2)); //System Bus
    //connect(systemMonitor,SIGNAL(MethodCall(QString,QString)), this, SLOT(OnSystemBusMethod(QString,QString)));
    //systemMonitor->Start();

    qDebug() << "Loading Session Bus Monitor";

    QStringList sessionFilters = settings->GetSearches(1);
    qDebug() << "Session Filters:";
    foreach(QString filter, sessionFilters)
	qDebug() << "  " << filter;

    sessionMonitor = new DBusMonitor(1, sessionFilters); //Session Bus
    connect(sessionMonitor,SIGNAL(MethodCall(QString,QString)), this, SLOT(OnSessionBusMethod(QString,QString)));
    sessionMonitor->Start();
}

Daemon::~Daemon()
{

}

void Daemon::OnAppLaunched(QString app)
{
    qDebug() << "Locked application launched:" << app;
    emit LockedAppLaunched(app);

    if(!PhoneControl::IsLocked())
    {
	qDebug() << "Locking phone";
	//We should lock the phone here and show a notification
	if(PhoneControl::LockPhone())
	{
	    //qDebug() << "Phone locked";
	    Notifications::ShowBanner("<b>AppLock</b> has locked your phone");
	    emit PhoneLocked();
	    return;
	}
    }
    else
    {
	qDebug() << "Phone already locked";
    }
}

void Daemon::PrintMonitorList()
{

}

void Daemon::PrintStatus()
{
    qDebug() << "Status: Running";
    qDebug() << "Phone Status:" << (PhoneControl::IsLocked() ? "Locked" : "Unlocked");
}

QString Daemon::GetStatus()
{
    return QString("Running - Phone ") + (PhoneControl::IsLocked() ? "Locked" : "Unlocked");
}

bool Daemon::AddApp(QString name, QString path)
{
    QFileInfo exe(path);

    QString absPath;

    if(!exe.isFile())
	return false;

    if(exe.isSymLink())
	absPath = exe.symLinkTarget();
    else
	absPath = exe.absoluteFilePath();

    qDebug() << "Adding App:" << name;
    qDebug() << "Path:" << absPath;

    settings->AddApp(name,absPath);
    launchMonitor->AddMonitoredApp(absPath);
    return true;
}

bool Daemon::AddSearch(QString name, QString bus, QString search)
{
    qDebug() << "Adding Filter:" << name << "(" << bus << ")";
    qDebug() << search;
    int busID = 1;

    if(!bus.contains("session", Qt::CaseInsensitive))
	//busID = 2;
	return false;

    settings->AddSearch(name, busID, search);
    if(busID == 1)
	sessionMonitor->AddFilter(search);
    else if(busID == 2)
	systemMonitor->AddFilter(search);

    qDebug() << "Filter Added";
    return true;
}

bool Daemon::RemoveApp(QString name)
{
    settings->RemoveApp(name);
    return true;
}

bool Daemon::RemoveSearch(QString name)
{
    settings->RemoveSearch(name);
    return true;
}

void Daemon::OnSystemBusMethod(QString interface, QString method)
{
    if(interface == "org.freedesktop.DBus")
	return;
    OnAppLaunched(interface);
}

void Daemon::OnSessionBusMethod(QString interface, QString method)
{
    if(interface == "org.freedesktop.DBus")
	return;
    OnAppLaunched(interface);
}

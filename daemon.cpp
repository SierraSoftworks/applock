#include "daemon.h"

Daemon::Daemon()
{
    qDebug() << "AppLock Daemon - Loading Settings";
    settings = new Settings();

    phoneControl = new PhoneControl();

    qDebug() << "Loading Application Monitor";
    launchMonitor = new LaunchMonitor();
    connect(launchMonitor,SIGNAL(ApplicationLaunched(QString)), this, SLOT(OnAppLaunched(QString)));
    launchMonitor->SetMonitoredApps(settings->GetApps(), settings->GetCommandLines());

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

#ifdef DBUSSETTINGS
    settings->AddApp(name,absPath);
#endif
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

    qDebug() << "Adding search to settings";
#ifdef DBUSSETTINGS
    settings->AddSearch(name, busID, search);
#endif

    //qDebug() << "Adding filter to monitor";
    //if(busID == 1)
	//sessionMonitor->AddFilter(search);
    //else if(busID == 2)
	//systemMonitor->AddFilter(search);

    //qDebug() << "Filter Added";
    return true;
}

bool Daemon::AddCMD(QString name, QString cmdline)
{
    qDebug() << "Adding Command Line Filter:" << name;
    qDebug() << "Filter:" << cmdline;

#ifdef DBUSSETTINGS
    settings->AddCommandLine(name,cmdline);
#endif
    launchMonitor->AddMonitoredCMDLine(cmdline);
    return true;
}

bool Daemon::RemoveCMD(QString name)
{
    QString cmdline = settings->GetCommandLine(name);
    if(cmdline.isNull() || cmdline.isEmpty())
	return false;

    qDebug() << "Removing Command Line Filter:" << name;
    launchMonitor->RemoveMonitoredCMDLine(cmdline);
#ifdef DBUSSETTINGS
    settings->RemoveCommandLine(name);
#endif
    return true;
}

bool Daemon::RemoveApp(QString name)
{
    launchMonitor->RemoveMonitoredApp(settings->GetApp(name));
#ifdef DBUSSETTINGS
    settings->RemoveApp(name);
#endif
    return true;
}

bool Daemon::RemoveSearch(QString name)
{
   // QString filter = settings->GetSearch(name,1);
    //if(!filter.isEmpty() && !filter.isNull())
	//sessionMonitor->RemoveFilter(filter);
#ifdef DBUSSETTINGS
    settings->RemoveSearch(name);
#endif
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

void Daemon::Refresh()
{
    sessionMonitor->Stop();

    QStringList sessionFilters = settings->GetSearches(1);
    sessionMonitor->Refresh(sessionFilters);

    sessionMonitor->Start();
}

void Daemon::Exit()
{
    PhoneControl::LockPhone();
    exit(0);
}

bool Daemon::IsLockedApp(QString name)
{
    if(settings->IsApp(name))
    {
	qDebug() << "App" << name << "is locked";
	return true;
    }
    return false;
}

bool Daemon::IsLockedDBus(QString name)
{
    if(settings->IsSearch(name,1))
    {
	qDebug() << "DBus" << name << "is locked";
	return true;
    }
    return false;
}

bool Daemon::IsLockedCMD(QString name)
{
    if(settings->IsCommandLine(name))
    {
	qDebug() << "CMD" << name << "is locked";
	return true;
    }
    return false;
}

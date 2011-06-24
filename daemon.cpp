#include "daemon.h"

LaunchMonitor *launchMonitor;
PhoneControl *phoneControl;
Settings *settings;

Daemon::Daemon()
{
    qDebug() << "AppLock Daemon - Loading Settings";
    settings = new Settings();

    phoneControl = new PhoneControl();

    qDebug() << "Loading Application Monitor";
    launchMonitor = new LaunchMonitor();
    connect(launchMonitor,SIGNAL(ApplicationLaunched(QString)), this, SLOT(OnAppLaunched(QString)));
    connect(launchMonitor,SIGNAL(ApplicationClosed(QString)), this, SIGNAL(LockedAppClosed(QString)));
    launchMonitor->SetSettings(settings);

    PrintMonitorList();

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
    qDebug() << "Monitored Applications:";
    launchMonitor->PrintMonitors();
    qDebug() << "";
}

void Daemon::PrintStatus()
{
    qDebug() << "Status: Running";
    qDebug() << "Phone Status:" << (PhoneControl::IsLocked() ? "Locked" : "Unlocked");
}

QList<QString> Daemon::GetMonitoredApplicationNames()
{
    return settings->GetAppNames();
}

QString Daemon::GetStatus()
{
    return QString("Running - Phone ") + (PhoneControl::IsLocked() ? "Locked" : "Unlocked");
}

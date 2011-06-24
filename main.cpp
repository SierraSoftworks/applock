#include "mainwindow.h"

#include "settings.h"
#include "daemon.h"
#include "applicationdescription.h"

#include <QtGui/QApplication>
#include <QtDBus>

void PrintHelp()
{
    qDebug() << "applock [OPTIONS]";
    qDebug() << "";
    qDebug() << "Options:";
    qDebug() << "  --add	[DESKTOP FILE]";
    qDebug() <<	"	Adds the specified desktop file's application to the";
    qDebug() <<	"	list of locked applications";
    qDebug() << "  --show-apps";
    qDebug() << "	Shows the list of locked executables";

    qDebug() << "  --help -?";
    qDebug() << "	Shows this help page";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("AppLock");
    app.setOrganizationName("Sierra Softworks");
    app.setOrganizationDomain("sierrasoftworks.com");


    //Now we need to check the command line args
    for(int i = 0; i < argc; i++)
    {
	QString arg = argv[i];
	if(arg == "--add")
	{
	    if(i == argc - 1)
	    {
		qWarning("You must provide the path to the Desktop file");
		exit(1);
	    }

	    qDebug() << "Adding" << argv[i + 1] << "to the list of locked applications";
	    ApplicationDescription appDescriptor;
	    if(!appDescriptor.Load(argv[i + 1]))
	    {
		qDebug() << "Failed to open Desktop file";
		exit(1);
	    }
	    qDebug() << "Name:" << appDescriptor.getAppName();
	    qDebug() << "Executable:" << appDescriptor.getAppPath();
	    if(appDescriptor.IsDBusService())
		qDebug() << "DBus Service:" << appDescriptor.getAppDBus();

	    Settings settings;

	    settings.AddApp(&appDescriptor);

	    qDebug() << "Added to list of locked apps";
	}
	else if(arg == "--show-apps")
	{
	    //Print a list of the locked DBus apps
	    Settings settings;
	    QList<ApplicationDescription*> lockedApps = settings.GetLockedApps();
	    for(int i = 0; i < lockedApps.count(); i++)
	    {
		if(lockedApps[i]->IsDBusService())
		    qDebug() << lockedApps[i]->getAppName() << "-" << lockedApps[i]->getAppDBus();
		else
		    qDebug() << lockedApps[i]->getAppName() << "-" << lockedApps[i]->getAppPath();
	    }
	    exit(0);
	}
	else if (arg == "--help" || arg == "-?")
	{
	    PrintHelp();
	    exit(0);
	}
	else if(arg == "-ui")
	{
	    MainWindow mainWindow;
	    mainWindow.setOrientation(MainWindow::ScreenOrientationLockLandscape);
	    mainWindow.showExpanded();
	    mainWindow.LoadDesktopFiles();

	    return app.exec();
	}
	else if(arg == "-d")
	{
	    //Run in Daemon mode
	    if(!QDBusConnection::sessionBus().isConnected())
	    {
		qWarning("Could not connect to session bus");
		exit(1);
	    }
	    if(!QDBusConnection::sessionBus().registerService("com.sierrasoftworks.AppLock"))
	    {
		qDebug() << "Failed to register service:" << QDBusConnection::sessionBus().lastError().message();
		exit(2);
	    }

	    Daemon *daemon = new Daemon();

	    if(!QDBusConnection::sessionBus().registerObject(QString("/com/sierrasoftworks/AppLock"),
							     daemon,QDBusConnection::ExportAllContents))
	    {
		qDebug() << "Failed to register object:" << QDBusConnection::sessionBus().lastError().message();
		exit(3);
	    }
	    return app.exec();
	}
    }



}

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
    qDebug() << "  --remove	[DESKTOP FILE]";
    qDebug() << "	Removes the specified desktop file's application from";
    qDebug() << "	the list of locked applications";
    qDebug() << "  --show-apps";
    qDebug() << "	Shows the list of locked executables";
    qDebug() << "  --clear-apps";
    qDebug() << "	Clears the database's list of locked applications";
    qDebug() << "  --status";
    qDebug() <<	"	Shows the current status of the daemon";
    qDebug() << "";
    qDebug() << "[DESKTOP FILE]";
    qDebug() << "   Can either be the absolute path to a desktop file or the";
    qDebug() << "   name of the desktop file (without the .desktop extension)";
    qDebug() << "";
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
		return 1;
	    }

	    qDebug() << "Adding" << argv[i + 1] << "to the list of locked applications";

	    QFileInfo file(argv[i + 1]);
	    if(!file.isFile())
	    {
		file.setFile("/usr/share/applications/hildon/" + QString(argv[i + 1]) + ".desktop");
		if(!file.isFile())
		{
		    qDebug() << "You have not specified a valid file";
		    return 1;
		}
	    }

	    ApplicationDescription appDescriptor;
	    if(!appDescriptor.Load(argv[i + 1]))
	    {
		qDebug() << "Failed to open Desktop file";
		return 1;
	    }
	    qDebug() << "Name:" << appDescriptor.getAppName();
	    qDebug() << "Executable:" << appDescriptor.getAppPath();
	    if(appDescriptor.IsDBusService())
		qDebug() << "DBus Service:" << appDescriptor.getAppDBus();

	    Settings settings;

	    settings.AddApp(appDescriptor);

	    qDebug() << "Added to list of locked apps";
	    return 0;
	}
	else if(arg == "--remove")
	{
	    if(i == argc - 1)
	    {
		qWarning("You must provide the path to the Desktop file");
		return 1;
	    }

	    qDebug() << "Removing" << argv[i + 1] << "from the list of locked applications";

	    QFileInfo file(argv[i + 1]);
	    if(!file.isFile())
	    {
		file.setFile("/usr/share/applications/hildon/" + QString(argv[i + 1]) + ".desktop");
		if(!file.isFile())
		{
		    qDebug() << "You have not specified a valid file";
		    return 1;
		}
	    }

	    ApplicationDescription appDescriptor;
	    if(!appDescriptor.Load(argv[i + 1]))
	    {
		qDebug() << "Failed to open Desktop file";
		return 1;
	    }
	    qDebug() << "Name:" << appDescriptor.getAppName();
	    qDebug() << "Executable:" << appDescriptor.getAppPath();
	    if(appDescriptor.IsDBusService())
		qDebug() << "DBus Service:" << appDescriptor.getAppDBus();

	    Settings settings;

	    settings.RemoveApp(appDescriptor);

	    qDebug() << "Removed from list of locked apps";
	    return 0;
	}
	else if(arg == "--show-apps")
	{
	    //Print a list of the locked DBus apps
	    Settings settings;
	    QList<QString> lockedApps = settings.GetAppNames();
	    for(int i = 0; i < lockedApps.count(); i++)
	    {
		ApplicationDescription *app = settings.GetAppFromName(lockedApps[i]);
		if(app->IsDBusService())
		    qDebug() << app->getAppName() << "-" << app->getAppDBus();
		else
		    qDebug() << app->getAppName() << "-" << app->getAppPath();
		delete app;
	    }
	    return 0;
	}
	else if (arg == "--help" || arg == "-?")
	{
	    PrintHelp();
	    return 0;
	}
	else if(arg == "-ui")
	{
	    qDebug() << "This functionality is not yet available";
	    exit(1);

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
	else if(arg == "--clear-apps")
	{
	    Settings settings;

	    settings.ClearApps();

	    return 0;
	}
	else if(arg == "--status")
	{
	    QDBusInterface dbus(
			"com.sierrasoftworks.AppLock",
			"/com/sierrasoftworks/AppLock",
			"com.sierrasoftworks.AppLock.Daemon");


	    if(!dbus.isValid())
	    {
		qDebug() << "Not Running";
		return 1;
	    }

	    QDBusReply<QString> reply = dbus.call("GetStatus");

	    if(!reply.isValid())
	    {
		qDebug() << "Not Responding";
		return 1;
	    }

	    qDebug() << reply.value();
	    return 0;
	}
    }

    PrintHelp();



}

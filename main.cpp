#include "settings.h"
#include "daemon.h"
#include "applicationdescription.h"
#include "qmlviewer.h"
#include "uidaemonhandler.h"
#include "qmldaemoninterface.h"

#include <QtGui/QApplication>
#include <QtDBus>
#include <QtGui>
#include <QtDeclarative>

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

bool IsDaemonRunning()
{
    QDBusInterface dbus(
		"com.sierrasoftworks.AppLock",
		"/com/sierrasoftworks/AppLock",
		"com.sierrasoftworks.AppLock.Daemon");


    return dbus.isValid();
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

	    if(IsDaemonRunning())
	    {
		QDBusInterface dbus(
			    "com.sierrasoftworks.AppLock",
			    "/com/sierrasoftworks/AppLock",
			    "com.sierrasoftworks.AppLock.Daemon");


		if(!dbus.isValid())
		{
		    qDebug() << "Daemon Not Running";
		    return 1;
		}
		if(appDescriptor.IsDBusService() && !SubstitutionFileParser::IsSubstituted(appDescriptor.getAppDesktopFile()))
		    dbus.call("AddSearch", appDescriptor.getAppName(), "session", "type='method_call',interface='" + appDescriptor.getAppDBus() + "',member='top_application'");
		else if(!appDescriptor.IsDBusService() && !SubstitutionFileParser::IsSubstituted(appDescriptor.getAppDesktopFile()))
		    dbus.call("AddApp", appDescriptor.getAppName(), appDescriptor.getAppPath());
		else
		{
		    QMap<QString,QString> searches = SubstitutionFileParser::GetApplicationDBusSearches(appDescriptor.getAppDesktopFile());
		    QMap<QString,QString> paths = SubstitutionFileParser::GetApplicationPaths(appDescriptor.getAppDesktopFile());

		    foreach(QString search, searches.keys())
			dbus.call("AddSearch",search,"session",searches.value(search));

		    foreach(QString path, paths.keys())
			dbus.call("AddApp",path,paths.value(path));

		}

	    }
	    else
	    {
		Settings settings;

		if(appDescriptor.IsDBusService())
		    settings.AddSearch(appDescriptor.getAppName(), 1, "type='method_call',interface='" + appDescriptor.getAppDBus() + "',member='top_application'");
		else
		    settings.AddApp(appDescriptor.getAppName(), appDescriptor.getAppPath());

	    }
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

	    if(IsDaemonRunning())
	    {
		QDBusInterface dbus(
			    "com.sierrasoftworks.AppLock",
			    "/com/sierrasoftworks/AppLock",
			    "com.sierrasoftworks.AppLock.Daemon");


		if(!dbus.isValid())
		{
		    qDebug() << "Daemon Not Running";
		    return 1;
		}

		if(appDescriptor.IsDBusService())
		    dbus.call("RemoveSearch", appDescriptor.getAppName());
		else
		    dbus.call("RemoveApp", appDescriptor.getAppName());
	    }
	    else
	    {
		Settings settings;

		if(appDescriptor.IsDBusService())
		    settings.RemoveSearch(appDescriptor.getAppName());
		else
		    settings.RemoveApp(appDescriptor.getAppName());
	    }

	    qDebug() << "Removed from list of locked apps";
	    return 0;
	}
	else if(arg == "--show-apps")
	{
	    //Print a list of the locked DBus apps
	    Settings settings;
	    QStringList apps = settings.GetAppNames();
	    QStringList session = settings.GetSearchNames(1);
	    //QStringList system = settings.GetSearchNames(2);

	    qDebug() << "Applications:";
	    foreach(QString app, apps)
		qDebug() << " " << app;

	    //qDebug() << "";
	    //qDebug() << "System Bus Monitors:";
	    //foreach(QString name, system)
		//qDebug() << " " << name;

	    qDebug() << "";
	    qDebug() << "Session Bus Monitors:";
	    foreach(QString name, session)
		qDebug() << " " << name;

	    return 0;
	}
	else if (arg == "--help" || arg == "-?")
	{
	    PrintHelp();
	    return 0;
	}
	else if(arg == "-ui")
	{
	    QmlApplicationViewer viewer;
	    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

	    QObject *qmlContext;
	    UIDaemonHandler *daemonInterface = new UIDaemonHandler(qmlContext);

	    qmlRegisterType<QMLDaemonInterface>("Sierra",1,0,"AppLock");

	    viewer.rootContext()->setContextProperty("daemon", daemonInterface);

	    //viewer.setMainQmlFile(QLatin1String("qml/mainWindow.qml"));
	    viewer.setMainQmlFile(QLatin1String("qrc:/qml/mainWindow.qml"));

	    viewer.showExpanded();
	    viewer.showFullScreen();
	    viewer.setWindowTitle("AppLock");

	    daemonInterface->UpdateAppList();

	    return app.exec();

	}
	else if(arg == "-d")
	{
	    //Run in Daemon mode
	    if(!QDBusConnection::sessionBus().isConnected())
	    {
		qWarning("Could not connect to session bus when starting daemon.");
		qWarning("This should never happen and indicates a problem communicating with DBus");
		return 1;
	    }
	    if(!QDBusConnection::sessionBus().registerService("com.sierrasoftworks.AppLock"))
	    {
		qDebug() << "Failed to register service:" << QDBusConnection::sessionBus().lastError().message();
		qDebug() << "This probably means that the daemon is already running";
		return 1;
	    }

	    Daemon *daemon = new Daemon();

	    if(!QDBusConnection::sessionBus().registerObject(QString("/com/sierrasoftworks/AppLock"),
							     daemon,QDBusConnection::ExportAllContents))
	    {
		qDebug() << "Failed to register object:" << QDBusConnection::sessionBus().lastError().message();
		qDebug() << "This shouldn't happen and probably indicates a problem with your Qt install or with DBus";
		return 1;
	    }
	    return app.exec();
	}
	else if(arg == "--clear")
	{
	    Settings settings;

	    if(i == argc - 1)
	    {
		settings.ClearApps();
		settings.ClearCMDLines();
		settings.ClearSearches();
	    }
	    else
	    {
		if(QString(argv[i+1]).contains("apps",Qt::CaseInsensitive))
		    settings.ClearApps();
		else if(QString(argv[i+1]).contains("dbus",Qt::CaseInsensitive))
		    settings.ClearSearches();
		else if(QString(argv[i+1]).contains("cmdlines",Qt::CaseInsensitive))
		    settings.ClearCMDLines();
	    }

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


    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    QObject *qmlContext;
    UIDaemonHandler *daemonInterface = new UIDaemonHandler(qmlContext);

    qmlRegisterType<QMLDaemonInterface>("Sierra",1,1,"AppLock");

    viewer.rootContext()->setContextProperty("daemon", daemonInterface);

    //viewer.setMainQmlFile(QLatin1String("qml/mainWindow.qml"));
    viewer.setMainQmlFile(QLatin1String("qrc:/qml/mainWindow.qml"));

    viewer.showExpanded();
    viewer.showFullScreen();
    viewer.setWindowTitle("AppLock");

    daemonInterface->UpdateAppList();

    return app.exec();


}

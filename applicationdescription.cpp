#include "applicationdescription.h"

#ifndef DEBUG
//#define DEBUG
#endif

static QStringList iconFolders;
static QStringList binaryFolders;
static QStringList serviceFiles;



ApplicationDescription::ApplicationDescription()
{
    isnull = true;
}

ApplicationDescription::ApplicationDescription(const ApplicationDescription &app)
{
    appName = app.appName;
    appIcon = app.appIcon;
    appPath = app.appPath;
    appDBus = app.appDBus;
    appDesktopFile = app.appDesktopFile;
    isnull = app.isnull;
}

ApplicationDescription::ApplicationDescription(QString desktopFile)
{
    appDesktopFile = desktopFile;
    Load(desktopFile);
}

ApplicationDescription::ApplicationDescription(QString name, QString path, QString icon, QString dbus, QString desktopFile)
{
    appName = name;
    appPath = path;
    appIcon = icon;
    appDBus = dbus;
    appDesktopFile = desktopFile;
    isnull = false;
}

ApplicationDescription::~ApplicationDescription()
{

}

bool ApplicationDescription::isNull()
{
    return isnull;
}

bool ApplicationDescription::isComplex()
{
    return SubstitutionFileParser::IsSubstituted(appDesktopFile);
}

bool ApplicationDescription::isScript()
{
    if(IsDBusService())
	return false;


    if(!commandLine.isEmpty() && !commandLine.isNull())
	return true;

    if(appPath.endsWith(".sh") || appPath.endsWith(".py"))
	return true;

    return false;
}

QMap<QString,QString> LoadFileValues(QString file)
{
    QMap<QString,QString> result;
    if(!QFileInfo(file).exists())
	return result;

    //qDebug() << "Loading File";
    QFile *file2 = new QFile(file);

    file2->open(QFile::ReadOnly);
    if(!file2->isOpen() || !file2->isReadable())
	return result;

    //qDebug() << "Reading file";
    QString fileData = QString(file2->readAll());
    file2->close();
    //Now parse the desktop file

    QStringList fileParts =  fileData.split("\n");
    //qDebug() << fileParts.count() << "entries in file";
    foreach(QString line,fileParts)
    {
	QStringList data = line.trimmed().split("=");
	if(data.count() < 2)
	    continue; //Invalid entry

	if(result.contains(data[0].trimmed()))
	    continue; //Duplicate key
	result.insert(data[0].trimmed(),data[1].trimmed());
    }

    return result;
}

void ApplicationDescription::LoadServiceFile(QString serviceName)
{
    if(serviceFiles.count() == 0)
    {
	QDir serviceDir("/usr/share/dbus-1/services");
	QStringList files = serviceDir.entryList(QDir::Files,QDir::NoSort);

	foreach(QString file, files)
	{
	    QMap<QString,QString> values = LoadFileValues(serviceDir.absoluteFilePath(file));
	    serviceFiles << values["Name"];
	}
    }

    if(serviceFiles.contains(serviceName))
    {
	appDBus = serviceName;
	qDebug() << "DBus Monitor Loaded:" << appName;
	qDebug() << "  " << appDBus;
	return;
    }
    else
    {
	foreach(QString svc, serviceFiles)
	{
	    if(svc.contains(serviceName))
	    {
		appDBus = svc;
#ifdef DEBUG
		qDebug() << "DBus Monitor Loaded:" << appName;
		qDebug() << "  " << appDBus;
#endif
		return;
	    }
	}
    }
    isnull = true;
    appDBus = "";
}

bool ApplicationDescription::Load(QString desktopFile)
{

    if(iconFolders.count() == 0)
    {
	iconFolders << "/usr/share/icons/hicolor/scalable/apps"
		    << "/usr/share/icons/hicolor/scalable/hildon"
		    << "/usr/share/icons/hicolor/64x64/apps"
		    << "/usr/share/icons/hicolor/64x64/hildon"
		    << "/usr/share/icons/hicolor/48x48/apps"
		    << "/usr/share/icons/hicolor/48x48/hildon";
    }

    if(binaryFolders.count() == 0)
    {
	binaryFolders << "/bin"
		      << "/usr/bin"
		      << "/opt/usr/bin"
		      << "/usr/sbin"
		      << "/opt/usr/sbin";
    }


    appName = "";
    appPath = "";
    appDBus = "";
    appIcon = "";
    isnull = true;

    appDesktopFile = QFileInfo(desktopFile).fileName();

    //qDebug() << "Loading Desktop File:" << desktopFile;
    QMap<QString,QString> fileEntries = LoadFileValues(desktopFile);

    if(fileEntries.count() == 0)
    {
	//qDebug() << "Desktop file didn't have any entries or was invalid";
	isnull = true;
	return false;
    }

    try
    {
	if(!fileEntries.contains("Type") || fileEntries["Type"] != "Application")
	{
	    isnull = true;
	    return false;
	}

	QFileInfo exec(fileEntries["Exec"].split(" ")[0]);

	QString execPath;
	if(exec.isFile())
	    execPath = (exec.isSymLink() ? exec.symLinkTarget() : exec.absoluteFilePath());
	else	
	    execPath = FileLocator::GetFilePath(binaryFolders,fileEntries["Exec"].split(" ")[0]);

	QStringList pathParts = fileEntries["Exec"].split(" ");
	if(pathParts.count() > 1)
	{
	    commandLine = fileEntries["Exec"].remove(0,pathParts.at(0).length());
	}
	else if(isScript())
	{
	    commandLine = appPath;
	}


	bool localised = false;
	if(fileEntries.contains("X-Text-Domain"))
	{
	    setlocale(LC_ALL, "en_GB");
	    bindtextdomain(fileEntries["X-Text-Domain"].toAscii().data(), "/usr/share/locale");
	    bind_textdomain_codeset(fileEntries["X-Text-Domain"].toAscii().data(), "UTF-8");
	    textdomain(fileEntries["X-Text-Domain"].toAscii().data());

	    localised = true;
	}

	if(localised)
	    appName =  _(fileEntries["Name"].toAscii().data());
	else
	    appName = SubstitutionFileParser::GetApplicationName(appDesktopFile, fileEntries["Name"]);



    appPath = execPath;
    appIcon = FileLocator::GetFilePath(iconFolders,fileEntries["Icon"] + ".png");

    if(fileEntries.contains("Comment"))
    {
	if(localised)
	    appDescription = SubstitutionFileParser::GetApplicationDescription(appDesktopFile, _(fileEntries["Comment"].toAscii().data()));
	else
	    appDescription = SubstitutionFileParser::GetApplicationDescription(appDesktopFile,fileEntries["Comment"]);
    }
    else
	appDescription = SubstitutionFileParser::GetApplicationDescription(appDesktopFile,QString());


    if(fileEntries.keys().contains("X-Osso-Service"))
    {
	LoadServiceFile(fileEntries["X-Osso-Service"]);
	isnull = false;
	return true;
    }
#ifdef DEBUG
    qDebug() << "Application Monitor Loaded:";
    qDebug() << " Name:" << appName;
    qDebug() << " Path:" << appPath;
#endif

    }
    catch(...)
    {
	//qDebug() << "Failed to load desktop file";
	isnull = true;
	return false;
    }

    isnull = false;
    return true;
}


QString ApplicationDescription::getAppIcon()
{
    return appIcon;
}

QString ApplicationDescription::getAppName()
{
    return appName;
}

QString ApplicationDescription::getAppPath()
{
    return appPath;
}

QString ApplicationDescription::getAppDBus()
{
    return appDBus;
}

QString ApplicationDescription::getAppCMDLine()
{
    return commandLine;
}

QString ApplicationDescription::getAppDescription()
{
    return appDescription;
}


QString ApplicationDescription::getAppDesktopFile()
{
    return appDesktopFile;
}

bool ApplicationDescription::IsDBusService()
{
    return !appDBus.isEmpty() && !appDBus.isNull();
}

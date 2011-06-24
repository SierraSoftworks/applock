#include "applicationdescription.h"

QString appName;
QString appIcon;
QString appPath;
QString appDBus;
QString appDesktopFile;
QString appDBusFile;

bool isnull;

ApplicationDescription::ApplicationDescription()
{
    isnull = true;
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
	result.insert(data[0].trimmed(),data[1].trimmed());
    }

    return result;
}

void LoadServiceFile(QString serviceName, QString executable)
{
    QDir serviceDir("/usr/share/dbus-1/services");
    QStringList nameMatchList;
    nameMatchList
	    << serviceName + ".service"
	    << "*" + serviceName + ".service"
	    << serviceName + "*.service"
	    << "*" + serviceName + "*.service";
    QStringList serviceFiles = serviceDir.entryList(nameMatchList,QDir::Files,QDir::NoSort);
    if(serviceFiles.count() == 0)
    {
	qDebug() << "Failed to load service file for" << serviceName;
	return;
    }
    for(int i = 0; i < serviceFiles.count(); i++)
    {
	//qDebug() << "Loading Service File:" << serviceFiles[i];
	QMap<QString,QString> fileContents = LoadFileValues(serviceDir.absoluteFilePath(serviceFiles[i]));
	if(fileContents.count() == 0)
	{
	    //qDebug() << "File had no records";
	    continue;
	}
	QFileInfo exec(fileContents["Exec"]);
	QString execPath = (exec.isSymLink() ? exec.symLinkTarget() : exec.absolutePath());
	if(execPath == executable)
	{
	    appDBusFile = serviceDir.absoluteFilePath(serviceFiles[i]);
	    if(!fileContents.keys().contains("Name"))
	    {
		//qDebug() << "Service file doesn't specify a name";
		return;
	    }
	    appDBus = fileContents["Name"];
	    qDebug() << "DBUS Service Monitor Loaded:";
	    qDebug() << " Name:" << appName;
	    qDebug() << " DBus:" << appDBus;
	    return;
	}
    }

    appDBus = "";
}

bool ApplicationDescription::Load(QString desktopFile)
{

    appDesktopFile = desktopFile;

    //qDebug() << "Loading Desktop File:" << desktopFile;
    QMap<QString,QString> fileEntries = LoadFileValues(desktopFile);

    if(fileEntries.count() == 0)
    {
	//qDebug() << "Desktop file didn't have any entries or was invalid";
	return false;
    }

    try
    {
	QFileInfo exec(fileEntries["Exec"].split(" ")[0]);

	QString execPath;
	if(exec.isFile())
	    execPath = (exec.isSymLink() ? exec.symLinkTarget() : exec.absoluteFilePath());
	else
	{
	    exec.setFile("/usr/bin/" + fileEntries["Exec"].split(" ")[0]);
	    qDebug() << "Checking" << exec.filePath();
	    //Check if /usr/bin/<exec> is valid
	    if(exec.isFile())
	    {
		execPath = (exec.isSymLink() ? exec.symLinkTarget() : exec.absoluteFilePath());
	    }
	    else
	    {
		exec.setFile("/bin/" + fileEntries["Exec"].split(" ")[0]);
		if(exec.isFile())
		{
		    execPath = (exec.isSymLink() ? exec.symLinkTarget() : exec.absoluteFilePath());
		}
	    }
	}

    appName = fileEntries["Name"];
    appPath = execPath;
    appIcon = QString("/usr/share/icons/hicolor/64x64/apps/") + fileEntries["Icon"] + QString(".png");

    if(fileEntries.keys().contains("X-Osso-Service"))
    {	
	appDBusFile = fileEntries["X-Osso-Service"];
	LoadServiceFile(appDBusFile, appPath);
	return true;
    }
    qDebug() << "Application Monitor Loaded:";
    qDebug() << " Name:" << appName;
    qDebug() << " Path:" << appPath;

    }
    catch(...)
    {
	//qDebug() << "Failed to load desktop file";
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



QString ApplicationDescription::getAppDesktopFile()
{
    return appDesktopFile;
}

bool ApplicationDescription::IsDBusService()
{
    return !appDBus.isEmpty() && !appDBus.isNull();
}

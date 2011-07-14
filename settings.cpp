#include "settings.h"

QSqlDatabase dbConnection;

QFileInfo databaseFile = QFileInfo("/home/user/.applock/settings.db");


bool Settings::Connect()
{
    QDir *databaseDir = new QDir(databaseFile.absoluteDir());

    if(!databaseDir->exists())
	databaseDir->mkdir(databaseDir->absolutePath());


    dbConnection = QSqlDatabase::addDatabase("QSQLITE");
    dbConnection.setDatabaseName(databaseFile.absoluteFilePath());
    dbConnection.open();


    return dbConnection.isOpen() && !dbConnection.isOpenError();
}


void Settings::CreateDB()
{
    dbConnection.exec("CREATE TABLE IF NOT EXISTS searches(name TEXT primary key, bus INT(1), search TEXT)");
    dbConnection.exec("CREATE TABLE IF NOT EXISTS apps(name TEXT primary key, path TEXT)");
    dbConnection.exec("CREATE TABLE IF NOT EXISTS cmdlines(name TEXT primary key, cmdline TEXT");
}




Settings::Settings()
{
    Connect();
    CreateDB();
}

Settings::~Settings()
{
    dbConnection.close();
}



void Settings::ClearApps()
{
    dbConnection.exec("DELETE FROM `apps`");
}

void Settings::ClearSearches()
{
    dbConnection.exec("DELETE FROM `searches`");
}

void Settings::ClearCMDLines()
{
    dbConnection.exec("DELTE FROM `cmdlines`");
}

void Settings::AddApp(QString name, QString path)
{
    dbConnection.exec(
		QString("INSERT INTO `apps` VALUES('%1','%2')")
		.replace("%1", name.replace("'",""))
		.replace("%2", path.replace("'",""))
		);
}

void Settings::AddSearch(QString name, int bus, QString watch)
{
    dbConnection.exec(
		QString("INSERT INTO `searches` VALUES('%1','%2','%3')")
		.replace("%1", name.replace("'",""))
		.replace("%2", QString::number(bus))
		.replace("%3",watch.replace("'","&APOS;"))
		);
}

QStringList Settings::GetApps()
{
    QSqlQuery result = dbConnection.exec("SELECT path from `apps`");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString();

    return apps;

}

QString Settings::GetApp(QString name)
{
    QSqlQuery result = dbConnection.exec("SELECT path from `apps` where `name`='" + name.replace("'","") + "'");

    while(result.next())
	return result.value(0).toString();

    return "";

}

QStringList Settings::GetSearches(int bus)
{
    QSqlQuery result = dbConnection.exec("SELECT search from `searches` WHERE `bus`='" + QString::number(bus) + "'");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString().replace("&APOS;","'");

    return apps;
}

QString Settings::GetSearch(QString name, int bus)
{
    QSqlQuery result = dbConnection.exec("SELECT search from `searches` WHERE `bus`='" + QString::number(bus) + "' AND `name`='" + name.replace("'","") + "'");

    while(result.next())
	return result.value(0).toString().replace("&APOS;","'");

    return "";
}

void Settings::RemoveApp(QString name)
{
    qDebug() << "SETTINGS - Removing App:" << name;
    dbConnection.exec("DELETE FROM `apps` WHERE `name`='" + name.replace("'","") + "'");
}


void Settings::RemoveSearch(QString name)
{
    qDebug() << "SETTINGS - Removing Search:" << name;
    dbConnection.exec("DELETE FROM `searches` WHERE `name`='" + name.replace("'","") + "'");
}


QStringList Settings::GetAppNames()
{
    QSqlQuery result = dbConnection.exec("SELECT name from `apps`");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString();

    return apps;

}

QStringList Settings::GetSearchNames(int bus)
{
    QSqlQuery result = dbConnection.exec("SELECT name from `searches` WHERE `bus`='" + QString::number(bus) + "'");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString();

    return apps;
}

QStringList Settings::GetCommandLineNames()
{
    QSqlQuery result = dbConnection.exec("SELECT name from `cmdlines`");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString();

    return apps;
}

QStringList Settings::GetCommandLines()
{
    QSqlQuery result = dbConnection.exec("SELECT cmdline from `cmdlines`");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString();

    return apps;
}

QString Settings::GetCommandLine(QString name)
{
    QSqlQuery result = dbConnection.exec("SELECT cmdline from `cmdlines` where `name`='" + name.replace("'","") + "'");

    while(result.next())
	return result.value(0).toString();

    return "";
}

void Settings::AddCommandLine(QString name, QString regex)
{
    dbConnection.exec(
		QString("INSERT INTO `cmdlines` VALUES('%1','%2')")
		.replace("%1", name.replace("'",""))
		.replace("%2", regex.replace("'",""))
		);
}

void Settings::RemoveCommandLine(QString name)
{
    qDebug() << "SETTINGS - Removing Command Line:" << name;
    dbConnection.exec("DELETE FROM `cmdlines` WHERE `name`='" + name.replace("'","") + "'");
}

bool Settings::IsSearch(QString name, int bus)
{
    QSqlQuery res = dbConnection.exec("SELECT COUNT(*) FROM `searches` WHERE `bus`='" + QString::number(bus) + "' AND `name`='" + name.replace("'","") + "'");
    if(!res.next())
	return false;
    return (res.value(0).toInt() > 0);
}

bool Settings::IsCommandLine(QString name)
{
    QSqlQuery res = dbConnection.exec("SELECT COUNT(*) FROM `cmdlines` WHERE `name`='" + name.replace("'","") + "'");
    if(!res.next())
	return false;
    return (res.value(0).toInt() > 0);
}

bool Settings::IsApp(QString name)
{
    QSqlQuery res = dbConnection.exec("SELECT COUNT(*) FROM `apps` WHERE `name`='" + name.replace("'","") + "'");
    if(!res.next())
	return false;
    return (res.value(0).toInt() > 0);
}

#include "settings.h"

QSqlDatabase dbConnection;

QFileInfo databaseFile = QFileInfo("/opt/applock/settings.db");


bool Connect()
{
    QDir *databaseDir = new QDir(databaseFile.absoluteDir());

    if(!databaseDir->exists())
	databaseDir->mkdir(databaseDir->absolutePath());

    dbConnection = QSqlDatabase::addDatabase("QSQLITE");
    dbConnection.setDatabaseName(databaseFile.absoluteFilePath());
    dbConnection.open();

    return dbConnection.isOpen() && !dbConnection.isOpenError();
}


void CreateDB()
{
    dbConnection.exec("CREATE TABLE IF NOT EXISTS searches(name TEXT primary key, bus INT(1), search TEXT)");
    dbConnection.exec("CREATE TABLE IF NOT EXISTS apps(name TEXT primary key, path TEXT)");
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

QStringList Settings::GetSearches(int bus)
{
    QSqlQuery result = dbConnection.exec("SELECT search from `searches` WHERE `bus`='" + QString::number(bus) + "'");

    QStringList apps;
    while(result.next())
	apps << result.value(0).toString().replace("&APOS;","'");

    return apps;
}

void Settings::RemoveApp(QString name)
{
    dbConnection.exec("DELETE FROM `apps` WHERE `name`='" + name.replace("'","") + "'");
}


void Settings::RemoveSearch(QString name)
{
    dbConnection.exec("DELETE FROM `searches` WHERE `search`='" + name.replace("'","") + "'");
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

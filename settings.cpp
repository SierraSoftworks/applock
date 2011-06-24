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
    dbConnection.exec("CREATE TABLE IF NOT EXISTS apps(name TEXT primary key, exec TEXT, icon TEXT, dbus TEXT, desktopFile TEXT)");
    dbConnection.exec("CREATE TABLE IF NOT EXISTS settings(name TEXT primary key, value TEXT)");
}

void ClearApps()
{
    dbConnection.exec("DELETE FROM `apps`");
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
    dbConnection.exec("DELETE FROM `apps");
}

void Settings::AddApp(ApplicationDescription app)
{
    dbConnection.exec(
		QString("INSERT INTO `apps` VALUES('%1','%2','%3','%4','%5')")
		.replace("%1", app.getAppName().replace("'",""))
		.replace("%2", app.getAppPath().replace("'",""))
		.replace("%3", app.getAppIcon().replace("'",""))
		.replace("%4", app.getAppDBus().replace("'",""))
		.replace("%5", app.getAppDesktopFile().replace("'",""))
		);
}

void Settings::RemoveApp(ApplicationDescription app)
{
    dbConnection.exec(QString("DELETE FROM `apps` WHERE name='%1'")
		      .replace("%1", app.getAppName().replace("'","")));
}

bool Settings::IsMonitoredApp(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT COUNT(*) FROM `apps` WHERE exec='" + path.replace("'","") + "'");
    while(result.next())
	return result.value(0).toInt() > 0;
    return false;
}

bool Settings::IsMonitoredDBus(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT COUNT(*) FROM `apps` WHERE dbus='" + path.replace("'","") + "'");
    while(result.next())
	return result.value(0).toInt() > 0;
    return false;
}

QString Settings::GetAppNamePath(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT name FROM `apps` WHERE exec='" + path.replace("'","") + "'");
    while(result.next())
	return result.value(0).toString();
    return QString();
}

QString Settings::GetAppNameDBus(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT name FROM `apps` WHERE dbus='" + path.replace("'","") + "'");
    while(result.next())
	return result.value(0).toString();
    return QString();
}


ApplicationDescription* Settings::GetAppFromName(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT name, exec, icon, dbus, desktopFile FROM `apps` WHERE name='" + path.replace("'","") + "'");
    while(result.next())
    {
	return new ApplicationDescription(
		    result.value(0).toString(),
		    result.value(1).toString(),
		    result.value(2).toString(),
		    result.value(3).toString(),
		    result.value(4).toString()
		    );
    }
    return new ApplicationDescription();
}

ApplicationDescription* Settings::GetAppFromPath(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT name, exec, icon, dbus, desktopFile FROM `apps` WHERE exec='" + path.replace("'","") + "'");
    while(result.next())
    {
	return new ApplicationDescription(
		    result.value(0).toString(),
		    result.value(1).toString(),
		    result.value(2).toString(),
		    result.value(3).toString(),
		    result.value(4).toString()
		    );
    }
    return new ApplicationDescription();
}

ApplicationDescription* Settings::GetAppFromDBus(QString path)
{
    QSqlQuery result = dbConnection.exec("SELECT name, exec, icon, dbus, desktopFile FROM `apps` WHERE dbus='" + path.replace("'","") + "'");
    while(result.next())
    {
	return new ApplicationDescription(
		    result.value(0).toString(),
		    result.value(1).toString(),
		    result.value(2).toString(),
		    result.value(3).toString(),
		    result.value(4).toString()
		    );
    }
    return new ApplicationDescription();
}

QList<QString> Settings::GetAppNames()
{
    QList<QString> appNames;
    QSqlQuery result = dbConnection.exec("SELECT name FROM `apps`");
    while(result.next())
	appNames << result.value(0).toString();
    return appNames;
}

bool Settings::IsDBusMonitor(QString name)
{
    QSqlQuery result = dbConnection.exec("SELECT dbus FROM `apps` WHERE name='" + name.replace("'","") + "'");
    while(result.next())
	return !result.value(0).toString().isEmpty();
    return false;
}

bool SettingExists(QString setting)
{
    QSqlQuery reply = dbConnection.exec("SELECT name from `settings`");
    while(reply.next())
    {
	if(reply.value(0) == setting)
	    return true;
    }
    return false;
}

void Settings::SetSetting(QString setting, QVariant value)
{
    if(SettingExists(setting))
	dbConnection.exec(QString("UPDATE `settings` value='") + value.toString().replace("'","") + QString("' WHERE name='") + setting + QString("'"));
    else
	dbConnection.exec(QString("INSERT INTO `settings` VALUES('") + setting + QString("','") + value.toString().replace("'","") + QString("')"));
}

QVariant Settings::GetSetting(QString setting, QVariant defaultValue)
{
	QSqlQuery query = dbConnection.exec(QString("SELECT name, value FROM `settings` WHERE name='") + setting + QString("'"));
	while(query.next())
		return query.value(1);
	return defaultValue;
}

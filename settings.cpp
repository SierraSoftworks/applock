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
    dbConnection.exec("CREATE TABLE IF NOT EXISTS apps(desktop TEXT primary key)");
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



void Settings::AddApp(ApplicationDescription *app)
{
    dbConnection.exec(QString("INSERT INTO `apps` values ('") + app->getAppDesktopFile().replace("'","") + QString("')"));
}

void Settings::SetLockedApps(QList<ApplicationDescription*> apps)
{
    ClearApps();
    for(int i = 0; i < apps.count(); i++)
	AddApp(apps[i]);
}

QList<ApplicationDescription*> Settings::GetLockedApps()
{
    QList<ApplicationDescription*> appsList;

    QSqlQuery reply = dbConnection.exec("SELECT desktop FROM `apps`");
    while(reply.next())
    {
	ApplicationDescription app;
	app.Load(reply.value(0).toString());
	appsList.append(&app);
    }

    qDebug() << appsList.count() << "application monitors loaded";
    return appsList;
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

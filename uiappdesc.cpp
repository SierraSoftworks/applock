#include "uiappdesc.h"
static QDBusInterface* dbus = 0;

#ifndef DBUSSETTINGS
UIAppDesc::UIAppDesc(ApplicationDescription *app, Settings* _settings)
#else
UIAppDesc::UIAppDesc(ApplicationDescription *app)
#endif
{
    if(!dbus)
	dbus = new QDBusInterface("com.sierrasoftworks.AppLock",
					     "/com/sierrasoftworks/AppLock",
					     "com.sierrasoftworks.AppLock.Daemon");

    islocked = false;
    hasUpdated = false;

#ifndef DBUSSETTINGS
    settings = _settings;
#endif

    appInstance = app;

    if(appInstance->isNull())
	return;

    updateLock();

}

void UIAppDesc::updateLock()
{
    if(!hasUpdated)
    {
	islocked = isLocked();


	hasUpdated = true;
	emit lockedChanged();
    }
}

bool UIAppDesc::isLocked()
{
    if(SubstitutionFileParser::IsSubstituted(appInstance->getAppDesktopFile()))
    {
	qDebug() << "Checking Lock Status for Complex App";
	QMap<QString,QString> dbusList =
		SubstitutionFileParser::GetApplicationDBusSearches(appInstance->getAppDesktopFile());
	QMap<QString,QString> pathList =
		SubstitutionFileParser::GetApplicationPaths(appInstance->getAppDesktopFile());
	QMap<QString,QString> cmdList =
		SubstitutionFileParser::GetApplicationCommandLines(appInstance->getAppDesktopFile());

	for(int i = 0; i < dbusList.count(); i++)
	{
#ifdef DBUSSETTINGS
	    QDBusReply<bool> reply = dbus->call("IsLockedDBus",dbusList.keys()[i]);
	    if(!reply.isValid())
		continue;
	    if(reply.error().type() != QDBusError::NoError)
		continue;

	    if(reply.value())
		return true;
#else
	    if(settings->IsSearch(dbusList.keys().at(i),1))
		return true;
#endif
	}

	for(int i = 0; i < pathList.count(); i++)
	{
#ifdef DBUSSETTINGS
	    QDBusReply<bool> reply = dbus->call("IsLockedApp",pathList.keys()[i]);
	    if(!reply.isValid())
		continue;
	    if(reply.error().type() != QDBusError::NoError)
		continue;

	    if(reply.value())
		return true;
#else
	    if(settings->IsApp(pathList.keys().at(i)))
		return true;

#endif
	}

	for(int i = 0; i < cmdList.count(); i++)
	{
#ifdef DBUSSETTINGS

	    QDBusReply<bool> reply = dbus->call("IsLockedCMD",cmdList.keys()[i]);
	    if(!reply.isValid())
		continue;
	    if(reply.error().type() != QDBusError::NoError)
		continue;

	    if(reply.value())
		return true;
#else
	    if(settings->IsCommandLine(cmdList.keys().at(i)))
		return true;
#endif
	}

	return false;
    }
    else if(appInstance->IsDBusService())
    {
#ifdef DBUSSETTINGS
	qDebug() << "Checking Lock Status for DBus App";
	QDBusReply<bool> reply = dbus->call("IsLockedDBus",appInstance->getAppName());

	if(!reply.isValid())
	    return false;
	if(reply.error().type() != QDBusError::NoError)
	    return false;

	return reply.value();
#else
	return settings->IsSearch(appInstance->getAppName(),1);
#endif

    }
    else if(appInstance->isScript())
    {
#ifdef DBUSSETTINGS

	qDebug() << "Checking Lock Status for Command Line App";
	QDBusReply<bool> reply = dbus->call("IsLockedCMD",appInstance->getAppName());

	if(!reply.isValid())
	    return false;
	if(reply.error().type() != QDBusError::NoError)
	    return false;

	return reply.value();
#else

	return settings->IsCommandLine(appInstance->getAppName());
#endif
    }
    else
    {
#ifdef DBUSSETTINGS
	qDebug() << "Checking Lock Status for standard app";
	QDBusReply<bool> reply = dbus->call("IsLockedApp",appInstance->getAppName());

	if(!reply.isValid())
	    return false;
	if(reply.error().type() != QDBusError::NoError)
	    return false;

	return reply.value();

#else
	return settings->IsApp(appInstance->getAppName());
#endif
    }
}

bool UIAppDesc::IsLocked()
{
    updateLock();

    return islocked;
}

bool UIAppDesc::IsComplexApp()
{
    return SubstitutionFileParser::IsSubstituted(appInstance->getAppDesktopFile());
}

void UIAppDesc::OnLockChanged()
{
    hasUpdated = false;

    updateLock();

    emit lockedChanged();
}

QString UIAppDesc::getAppName()
{
    return SubstitutionFileParser::GetApplicationName(appInstance->getAppDesktopFile(),appInstance->getAppName());
    //return appInstance->getAppName();
}
QString UIAppDesc::getAppIcon()
{
    return appInstance->getAppIcon();
}
QString UIAppDesc::getAppDBus()
{
    return appInstance->getAppDBus();
}
QString UIAppDesc::getAppDesktopFile()
{
    return appInstance->getAppDesktopFile();
}
QString UIAppDesc::getAppPath()
{
    return appInstance->getAppPath();
}
bool UIAppDesc::IsDBusService()
{
    return appInstance->IsDBusService();
}
bool UIAppDesc::IsChecked()
{
    return isChecked;
}
void UIAppDesc::SetChecked(bool checked)
{
    if(isChecked != checked)
    {
	isChecked = checked;
	emit checkChanged();
    }
}

bool UIAppDesc::IsScript()
{
    return appInstance->isScript();
}

QString UIAppDesc::getAppDescription()
{
    return appInstance->getAppDescription();
}

QString UIAppDesc::getAppCMDLine()
{
    return appInstance->getAppCMDLine();
}

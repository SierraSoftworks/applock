#include "qmldaemoninterface.h"

static QMLDaemonInterface* instance;

QMLDaemonInterface::QMLDaemonInterface()
{
    instance = this;
}

QMLDaemonInterface::~QMLDaemonInterface()
{
    instance = 0;
}

void QMLDaemonInterface::Error(QString msg)
{
    if(instance)
	instance->OnError(msg);
}

void QMLDaemonInterface::AppLoaded(QString name, int totalApps)
{
    if(instance)
	instance->OnAppLoaded(name,totalApps);
}

void QMLDaemonInterface::AllAppsLoaded(int totalApps)
{
    if(instance)
	instance->OnAllAppsLoaded(totalApps);
}

void QMLDaemonInterface::AppLocked()
{
    if(instance)
	instance->OnAppLocked();
}

void QMLDaemonInterface::AppUnlocked()
{
    if(instance)
	instance->OnAppUnlocked();
}


void QMLDaemonInterface::OnError(QString message)
{
    emit error(message);
}

void QMLDaemonInterface::OnAppLoaded(QString name, int totalApps)
{
    emit applicationLoaded(name, totalApps);
}

void QMLDaemonInterface::OnAllAppsLoaded(int totalApps)
{
    emit applicationLoadingComplete(totalApps);
}

void QMLDaemonInterface::OnAppLocked()
{
    emit applicationLocked();
}

void QMLDaemonInterface::OnAppUnlocked()
{
    emit applicationUnlocked();
}


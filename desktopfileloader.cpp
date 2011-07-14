#include "desktopfileloader.h"

QStringList* blacklist1;

DesktopFileLoader::DesktopFileLoader(QStringList* ignore)
{
    blacklist1 = ignore;
}

DesktopFileLoader::~DesktopFileLoader()
{

}

void DesktopFileLoader::LoadFiles()
{
    start(QThread::LowPriority);
}

void DesktopFileLoader::run()
{
    QDir appsFolder("/usr/share/applications/hildon/");
    QStringList files = appsFolder.entryList(QStringList() << "*.desktop",QDir::Files);
    foreach(QString file, files)
	if(!blacklist1 || blacklist1->count() == 0)
	{
	    ApplicationDescription *app = new ApplicationDescription(appsFolder.absoluteFilePath(file));
	    if(!app->isNull())
		emit FileLoaded(app);
	    continue;
	}
	else if(!blacklist1->contains(file))
	{
	    ApplicationDescription *app = new ApplicationDescription( appsFolder.absoluteFilePath(file));
	    if(!app->isNull())
		emit FileLoaded(app);
	}

    emit FileLoadingComplete();
}

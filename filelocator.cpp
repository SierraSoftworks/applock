#include "filelocator.h"

QString FileLocator::GetFilePath(QStringList directories, QString fileName)
{
    foreach(QString dir, directories)
    {
	QDir dirInfo(dir);
	QStringList files = dirInfo.entryList(QStringList() << fileName << fileName + ".*", QDir::Files);
	if(files.count() > 0)
	    return dirInfo.absoluteFilePath(files[0]);
    }
    return QString();
}

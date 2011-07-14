#include "substitutionfileparser.h"

static QMap<QString,QString> replacementMap;


QMap<QString,QString> LoadFileValuesEx(QString file)
{
    QMap<QString,QString> result;
    result.clear();
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
    QString section = "";
    foreach(QString line,fileParts)
    {
	int firstIndexEquals = line.trimmed().indexOf("=");
	QStringList data;
	if(firstIndexEquals == -1)
	{
	    section = line.trimmed();
	    continue;
	}

	data << line.trimmed().mid(0,firstIndexEquals) << line.trimmed().mid(firstIndexEquals + 1, line.trimmed().length() - firstIndexEquals);


	if(result.contains((section != "" ? section + "/" + data[0].trimmed() : data[0].trimmed())))
	    continue; //Duplicate key

	result.insert((section != "" ? section + "/" + data[0].trimmed() : data[0].trimmed()),data[1].trimmed());
    }

    return result;
}


static void loadSubs()
{
    QDir subsDir("/opt/applock/AppOverrides");
    foreach(QString file, subsDir.entryList(QStringList() << "*.sub",QDir::Files))
    {
	QMap<QString,QString> map = LoadFileValuesEx(subsDir.absoluteFilePath(file));
	replacementMap.insert(map["DesktopFile"],subsDir.absoluteFilePath(file));
    }
}

bool SubstitutionFileParser::IsSubstituted(QString desktopFile)
{
    if(replacementMap.count() == 0)
	loadSubs();

    if(replacementMap.contains(desktopFile))
	return true;

    return false;
}

QString SubstitutionFileParser::GetApplicationName(QString desktopFile, QString defaultValue)
{
    if(IsSubstituted(desktopFile))
    {
	QMap<QString,QString> data = LoadFileValuesEx(replacementMap[desktopFile]);

	if(data.contains("Name"))
	{
	    //qDebug() << "Returning " << data["Name"] << "for" << desktopFile;
	    return data["Name"];
	}
	//qDebug() << "Returning" << defaultValue << "for" << desktopFile;
	return defaultValue;
    }
    else
	return defaultValue;
}


QString SubstitutionFileParser::GetApplicationDescription(QString desktopFile, QString defaultValue)
{
    if(IsSubstituted(desktopFile))
    {
	QMap<QString,QString> data = LoadFileValuesEx(replacementMap[desktopFile]);

	if(data.contains("Description"))
	{
	    //qDebug() << "Returning " << data["Description"] << "for" << desktopFile;
	    return data["Description"];
	}
	//qDebug() << "Returning" << defaultValue << "for" << desktopFile;
	return defaultValue;
    }
    else
	return defaultValue;
}


QMap<QString,QString> SubstitutionFileParser::GetApplicationDBusSearches(QString desktopFile)
{
    QMap<QString,QString> result;


    QMap<QString,QString> map = LoadFileValuesEx(replacementMap[desktopFile]);
    for(int i = 0; i < map.count(); i++)
    {	
	if(((QString)map.keys().at(i)).startsWith("[DBus]/"))
	    result.insert(((QString)map.keys().at(i)).remove(0,7),map.values().at(i));
    }

    return result;
}

QMap<QString,QString> SubstitutionFileParser::GetApplicationPaths(QString desktopFile)
{
    QMap<QString,QString> result;

    QMap<QString,QString> map = LoadFileValuesEx(replacementMap[desktopFile]);
    for(int i = 0; i < map.count(); i++)
    {
	if(((QString)map.keys().at(i)).startsWith("[Bin]/"))
	    result.insert(((QString)map.keys().at(i)).remove(0,6),map.values().at(i));
    }

    return result;
}


QMap<QString,QString> SubstitutionFileParser::GetApplicationCommandLines(QString desktopFile)
{
    QMap<QString,QString> result;

    QMap<QString,QString> map = LoadFileValuesEx(replacementMap[desktopFile]);
    for(int i = 0; i < map.count(); i++)
    {
	if(((QString)map.keys().at(i)).startsWith("[CMD]/"))
	    result.insert(((QString)map.keys().at(i)).remove(0,6),map.values().at(i));
    }

    return result;
}

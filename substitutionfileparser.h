#ifndef SUBSTITUTIONFILEPARSER_H
#define SUBSTITUTIONFILEPARSER_H

#include <QtCore>
#include "applicationdescription.h"

class SubstitutionFileParser
{
public:
    static bool IsSubstituted(QString desktopFile);
    static QMap<QString,QString> GetApplicationDBusSearches(QString desktopFile);
    static QMap<QString,QString> GetApplicationPaths(QString desktopFile);
    static QMap<QString,QString> GetApplicationCommandLines(QString desktopFile);
    static QString GetApplicationName(QString desktopFile, QString defaultValue);
    static QString GetApplicationDescription(QString desktopFile, QString defaultValue);
};

#endif // SUBSTITUTIONFILEPARSER_H

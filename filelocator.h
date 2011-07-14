#ifndef FILELOCATOR_H
#define FILELOCATOR_H

#include <QtCore>

class FileLocator
{
public:
    static QString GetFilePath(QStringList directories, QString fileName);
};

#endif // FILELOCATOR_H

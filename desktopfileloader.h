#ifndef DESKTOPFILELOADER_H
#define DESKTOPFILELOADER_H

#include <QtCore>
#include "applicationdescription.h"

class DesktopFileLoader : public QThread
{
    Q_OBJECT

public:
    DesktopFileLoader(QStringList * blacklist = new QStringList());
    ~DesktopFileLoader();

protected:
    void run();

public slots:
    void LoadFiles();

signals:
    void FileLoaded(ApplicationDescription *app);
    void FileLoadingComplete();
};

#endif // DESKTOPFILELOADER_H

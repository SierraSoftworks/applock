#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QtCore>
#include <QtDBus>
#include <QtGui>

class Notifications
{
public slots:
    static void ShowBanner(QString message);
    static void ShowDialog(QString message, quint32 timeout);
    static void SetBusyIndicator(QWidget *parent, bool visible);
};

#endif // NOTIFICATIONS_H

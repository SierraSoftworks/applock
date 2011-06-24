#include "notifications.h"

static QDBusInterface bannerInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications",QString(),QDBusConnection::systemBus());

void Notifications::ShowBanner(QString message)
{
    bannerInterface.call("SystemInfoprint", message);
}

void Notifications::ShowDialog(QString message, quint32 timeout)
{
    bannerInterface.call("SystemNoteDialog",message, timeout, "OK");
}

void Notifications::SetBusyIndicator(QWidget *parent, bool visible)
{
    parent->setAttribute( static_cast<Qt::WidgetAttribute>(131), visible);
}

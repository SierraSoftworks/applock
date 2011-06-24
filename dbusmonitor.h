#ifndef DBUSMONITOR_H
#define DBUSMONITOR_H

#include <QtCore>
#include "dbus/dbus.h"

class DBusMonitor : public QThread
{
    Q_OBJECT

public:
    DBusMonitor();
    DBusMonitor(QStringList filters);
    ~DBusMonitor();
    void OnMethodCall(QString interface, QString method);
    void OnSignal(QString interface, QString signal);

public slots:
    void Start();
    void Stop();

private:
    static void HandleMessage(DBusMessage *message, dbus_bool_t literal);
    static DBusHandlerResult monitor_filter_func (DBusConnection     *connection,
			 DBusMessage        *message,
			 void               *user_data);

protected:
    void run();

signals:
    void MethodCall(QString interface, QString method);
    void Signal(QString interface, QString signal);

};

#endif // DBUSMONITOR_H

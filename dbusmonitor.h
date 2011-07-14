#ifndef DBUSMONITOR_H
#define DBUSMONITOR_H

#include <QtCore>
#include "dbus/dbus.h"

class DBusMonitor : public QThread
{
    Q_OBJECT

public:
    DBusMonitor(int bus);
    DBusMonitor(QString bus);
    DBusMonitor(int bus, QStringList filters);
    DBusMonitor(QString bus, QStringList filters);
    ~DBusMonitor();
    void OnMethodCall(QString interface, QString method);
    void OnSignal(QString interface, QString signal);
    bool AddFilter(QString filter);
    bool RemoveFilter(QString filter);

public slots:
    void Start();
    void Stop();
    void Refresh(QStringList filters);

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

#include "dbusmonitor.h"


DBusConnection *connection;
DBusError error;
DBusBusType type = DBUS_BUS_SESSION;
DBusHandleMessageFunction filter_func;



static DBusMonitor *instance;


DBusMonitor::DBusMonitor(QString bus)
{
    instance = this;

    if(bus.contains("session", Qt::CaseInsensitive))
	type = DBUS_BUS_SESSION;
    else if(bus.contains("system", Qt::CaseInsensitive))
	type = DBUS_BUS_SYSTEM;

    filter_func = monitor_filter_func;

    dbus_error_init (&error);

    connection = dbus_bus_get (type, &error);

    if (connection == NULL)
    {
	qDebug() << "Failed to open connection";
	dbus_error_free (&error);

    }

    qDebug() << "Using Default Filter: type=method_call, member=top_application";
    dbus_bus_add_match (connection, "type='method_call',member='top_application'", &error);

    if (dbus_error_is_set (&error))
    {
	qDebug() << "Failed to setup match filter";
	dbus_error_free (&error);
    }

    if (!dbus_connection_add_filter (connection, filter_func, NULL, NULL))
    {
	qDebug() << "Failed to add message filter";
    }

      qDebug() << "DBus Monitor Connected";
}

DBusMonitor::DBusMonitor(int bus)
{
    instance = this;

    if(bus == 1)
	type = DBUS_BUS_SESSION;
    else if(bus == 2)
	type = DBUS_BUS_SYSTEM;

    filter_func = monitor_filter_func;

    dbus_error_init (&error);

    connection = dbus_bus_get (type, &error);

    if (connection == NULL)
    {
	qDebug() << "Failed to open connection";
	dbus_error_free (&error);

    }

    qDebug() << "Using Default Filter: type=method_call, member=top_application";
    dbus_bus_add_match (connection, "type='method_call',member='top_application'", &error);

    if (dbus_error_is_set (&error))
    {
	qDebug() << "Failed to setup match filter";
	dbus_error_free (&error);
    }

    if (!dbus_connection_add_filter (connection, filter_func, NULL, NULL))
    {
	qDebug() << "Failed to add message filter";
    }

      qDebug() << "DBus Monitor Connected";
}

DBusMonitor::DBusMonitor(int bus, QStringList filters)
{
    instance = this;

    if(bus == 2)
	type = DBUS_BUS_SYSTEM;


    filter_func = monitor_filter_func;

    dbus_error_init (&error);

    connection = dbus_bus_get (type, &error);

    if (connection == NULL)
    {
	qDebug() << "Failed to open connection";
	dbus_error_free (&error);

    }

    foreach(QString filter, filters)
    {
	AddFilter(filter);
    }

    if (!dbus_connection_add_filter (connection, filter_func, NULL, NULL))
    {
	qDebug() << "Failed to add message filter";
    }

      qDebug() << "DBus Monitor Connected";
}

DBusMonitor::DBusMonitor(QString bus, QStringList filters)
{
    instance = this;

    if(bus.contains("system", Qt::CaseInsensitive))
	type = DBUS_BUS_SYSTEM;


    filter_func = monitor_filter_func;

    dbus_error_init (&error);

    connection = dbus_bus_get (type, &error);

    if (connection == NULL)
    {
	qDebug() << "Failed to open connection";
	dbus_error_free (&error);

    }

    foreach(QString filter, filters)
    {
	AddFilter(filter);
    }

    if (!dbus_connection_add_filter (connection, filter_func, NULL, NULL))
    {
	qDebug() << "Failed to add message filter";
    }

      qDebug() << "DBus Monitor Connected";
}

DBusMonitor::~DBusMonitor()
{
    instance = NULL;
}


void DBusMonitor::OnMethodCall(QString interface, QString method)
{
    emit MethodCall(interface, method);
}

void DBusMonitor::OnSignal(QString interface, QString signal)
{
    emit Signal(interface, signal);
}


static const char*
type_to_name (int message_type)
{
  switch (message_type)
    {
    case DBUS_MESSAGE_TYPE_SIGNAL:
      return "signal";
    case DBUS_MESSAGE_TYPE_METHOD_CALL:
      return "method call";
    case DBUS_MESSAGE_TYPE_METHOD_RETURN:
      return "method return";
    case DBUS_MESSAGE_TYPE_ERROR:
      return "error";
    default:
      return "(unknown message type)";
    }
}

void DBusMonitor::HandleMessage(DBusMessage *message, dbus_bool_t literal)
{
  QString sender;
  QString destination;
  QString message_type;
  QString interface;
  QString member;


  message_type = type_to_name(dbus_message_get_type (message));
  sender = dbus_message_get_sender (message);
  destination = dbus_message_get_destination (message);




  if (!literal)
    {
    int msgType = dbus_message_get_type (message);

    qDebug() << "Message Recieved on DBus Type=" << message_type << " Sender=" << sender << " Destination=" << destination;
      switch (msgType)
	{
	case DBUS_MESSAGE_TYPE_METHOD_CALL:
	    //QString serial = QString(dbus_message_get_serial (message));
	    //QString path = QString(dbus_message_get_path (message));
	    interface = (dbus_message_get_interface (message));
	    member = (dbus_message_get_member (message));
	    qDebug() << "Method Called - Interface:"<<interface << " && Member:" << member;

	    instance->OnMethodCall(interface,member);
	    break;

	case DBUS_MESSAGE_TYPE_SIGNAL:
	    //QString serial = QString(dbus_message_get_serial (message));
	    //QString path = QString(dbus_message_get_path (message));
	    interface = (dbus_message_get_interface (message));
	    member = (dbus_message_get_member (message));
	    qDebug() << "Signal - Interface:"<<interface << " && Member:" << member;
	    //emit MethodCall(interface, member);
	    instance->OnSignal(interface,member);
	  break;
	default:
	  break;
	}
    }

}

DBusHandlerResult DBusMonitor::monitor_filter_func (DBusConnection     *connection,
		     DBusMessage        *message,
		     void               *user_data)
{

  HandleMessage(message, FALSE);

  if (dbus_message_is_signal (message,
			      DBUS_INTERFACE_LOCAL,
			      "Disconnected"))

  /* Conceptually we want this to be
   * DBUS_HANDLER_RESULT_NOT_YET_HANDLED, but this raises
   * some problems.  See bug 1719.
   */
  return DBUS_HANDLER_RESULT_HANDLED;

}

#define TRAP_NULL_STRING(str) ((str) ? (str) : "<none>")


bool shouldExit1 = false;
void DBusMonitor::run()
{    
    qDebug() << "DBus Monitor Running";
    while(!shouldExit1 && dbus_connection_read_write_dispatch(connection, 2));
    qDebug() << "DBus Monitor Stopping";
}


void DBusMonitor::Start()
{
    shouldExit1 = false;
    start(QThread::LowPriority);
}

void DBusMonitor::Stop()
{
    shouldExit1 = true;
}

bool DBusMonitor::AddFilter(QString filter)
{
    qDebug() << "  Adding Filter:" << filter;

    char* data;
    data = filter.toAscii().data();
    qDebug() << "    DATA:" << data;

    dbus_error_init (&error);

    dbus_bus_add_match (connection, filter.toAscii().data(), &error);

    if (dbus_error_is_set (&error))
    {
	qDebug() << "  Failed to setup match filter";
	dbus_error_free (&error);
	return false;
    }
    return true;
}

bool DBusMonitor::RemoveFilter(QString filter)
{
    qDebug() << "  Removing Filter:" << filter;

    char* data;
    data = filter.toAscii().data();
    qDebug() << "    DATA:" << data;


    dbus_error_init (&error);

    dbus_bus_remove_match (connection, filter.toAscii().data(), &error);

    if (dbus_error_is_set (&error))
    {
	qDebug() << "  Failed to setup match filter";
	dbus_error_free (&error);
	return false;
    }
    return true;
}

void DBusMonitor::Refresh(QStringList filters)
{
    dbus_connection_remove_filter(connection, filter_func, NULL);


    filter_func = monitor_filter_func;

    dbus_error_init (&error);

    connection = dbus_bus_get (type, &error);

    if (connection == NULL)
    {
	qDebug() << "Failed to open connection";
	dbus_error_free (&error);

    }

    foreach(QString filter, filters)
    {
	AddFilter(filter);
    }

    if (!dbus_connection_add_filter (connection, filter_func, NULL, NULL))
    {
	qDebug() << "Failed to add message filter";
    }
}

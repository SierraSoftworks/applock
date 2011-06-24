#include "phonecontrol.h"

static QDBusInterface LockInterface("com.nokia.system_ui",
				    "/com/nokia/system_ui/request",
				    "com.nokia.system_ui.request",
				    QDBusConnection::systemBus());

QDBusInterface *LockSignalInterface;

PhoneControl::PhoneControl()
{
    LockSignalInterface = new QDBusInterface("com.nokia.mce.signal",
					     "/com/nokia/mce/signal",
					     "com.nokia.mce.signal",
					     QDBusConnection::systemBus());

    if(!LockSignalInterface->isValid())
	qWarning("Failed to connect to MCE interface");


    //if(!LockSignalInterface->connect(LockSignalInterface, SIGNAL(tklock_mode_ind(QString)), this, SLOT(OnPhoneLocked(QString))))
	//qWarning("Failed to connect to lock signal");

    if(!LockSignalInterface->connection().connect(
		LockSignalInterface->service(),
		LockSignalInterface->path(),
		LockSignalInterface->interface(),
		"tklock_mode_ind",
		this,SLOT(OnPhoneLocked(QString))))
	qWarning("Failed to connect to lock signal");

    if(!LockSignalInterface->connection().connect(
		LockSignalInterface->service(),
		LockSignalInterface->path(),
		LockSignalInterface->interface(),
		"display_status_ind",
		this,SLOT(OnScreenOff(QString))))
	qWarning("Failed to connect to screen off signal");

    if(!LockSignalInterface->connection().connect(
		LockSignalInterface->service(),
		LockSignalInterface->path(),
		LockSignalInterface->interface(),
		"system_inactivity_ind",
		this,SLOT(OnActivityChange(bool))))
	qWarning("Failed to connect to inactivity signal");

}

PhoneControl::~PhoneControl()
{

}

void PhoneControl::OnPhoneLocked(QString mode)
{
    qDebug() << "Lock Signal:" << mode;
    if(mode == "locked")
	emit PhoneLocked();
    else if(mode == "unlocked")
	emit PhoneUnlocked();
}

void PhoneControl::OnScreenOff(QString mode)
{
    qDebug() << "Screen Mode Signal:" << mode;
    if(mode == "off")
	emit ScreenOff();
    else if(mode == "dimmed")
	emit ScreenDimmed();
    else if(mode == "on")
	emit ScreenOn();
}

void PhoneControl::OnActivityChange(bool active)
{
    qDebug() << "Phone Activity State:" << active;

    if(active)
	emit PhoneActive();
    else
	emit PhoneInactive();
}


bool PhoneControl::LockPhone()
{
    QDBusMessage reply = LockInterface.call("devlock_open",
					     "com.nokia.mce",
					     "/com/nokia/mce/request",
					     "com.nokia.mce.request",
					     "devlock_callback", (quint32)3);



    if(reply.type() == QDBusMessage::ErrorMessage)
    {
	qDebug() << "Failed to lock phone:" << reply.errorName() << reply.errorMessage();
	return false;
    }

    if(reply.arguments()[0].toBool() == true)
    {
	qDebug() << "Phone locked";
	return true;
    }
    else
    {
	qDebug() << "Phone failed to lock";
	return false;
    }
}

bool PhoneControl::IsLocked()
{
    QDBusReply<QString> reply = LockInterface.call("get_devicelock_mode");
    if(!reply.isValid())
	return false;
    return reply.value() == "locked";
}

bool PhoneControl::IsKeypadLocked()
{
    QDBusReply<QString> reply = LockInterface.call("get_tklock_mode");
    if(!reply.isValid())
	return false;
    return !reply.value().contains("unlocked");
}

#ifndef PHONECONTROL_H
#define PHONECONTROL_H

#include <QtDBus>

class PhoneControl : public QObject
{
    Q_OBJECT

public:
    PhoneControl();
    ~PhoneControl();
	static bool LockPhone();
	static bool IsLocked();
	static bool IsKeypadLocked();

private slots:
	void OnPhoneLocked(QString mode);
	void OnScreenOff(QString mode);
	void OnActivityChange(bool active);

signals:
	void PhoneLocked();
	void PhoneUnlocked();
	void ScreenOn();
	void ScreenDimmed();
	void ScreenOff();
	void PhoneInactive();
	void PhoneActive();


};

#endif // PHONECONTROL_H

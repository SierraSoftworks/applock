# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
dir2.source = AppOverrides
dir2.target = ./
DEPLOYMENTFOLDERS = dir2

QT	+= sql
QT	+= dbus

CONFIG += link_pkgconfig
PKGCONFIG += dbus-1 glib-2.0
#LIBS += -ldbus-1 -L../

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp \
    daemon.cpp \
    launchmonitor.cpp \
    settings.cpp \
    notifications.cpp \
    phonecontrol.cpp \
    applicationdescription.cpp \
    dbusmonitor.cpp \
    desktopfileloader.cpp \
    uidaemonhandler.cpp \
    qmldaemoninterface.cpp \
    filelocator.cpp \
    substitutionfileparser.cpp \
    uiappdesc.cpp
HEADERS += \
    daemon.h \
    launchmonitor.h \
    settings.h \
    notifications.h \
    phonecontrol.h \
    applicationdescription.h \
    dbusmonitor.h \
    desktopfileloader.h \
    uidaemonhandler.h \
    qmldaemoninterface.h \
    filelocator.h \
    substitutionfileparser.h \
    uiappdesc.h
FORMS +=



include(QMLViewer.pri)
#include(GetText.pri)
include(AppOverrides.pri)


# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    qtc_packaging/debian_fremantle/postinst \
    applock \
    com.sierrasoftworks.AppLock.service \
    qtc_packaging/debian_fremantle/preinst \
    qml/mainWindow.qml \
    qml/Lock.png \
    init.d/applock \
    qml/Menu.qml \
    qml/NotificationBar.qml \
    qml/Header.qml \
    qml/ScrollBar.qml \
    qml/ItemDetails.qml \
    qml/List.qml \
    qml/Button.qml \
    qml/RestartDaemon.qml \
    qtc_packaging/debian_fremantle/prerm

RESOURCES += \
    QML.qrc

# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

QT	+= sql
QT	+= dbus

CONFIG += link_pkgconfig
PKGCONFIG += dbus-1
#LIBS += -ldbus-1 -L../

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp \
    daemon.cpp \
    launchmonitor.cpp \
    settings.cpp \
    notifications.cpp \
    phonecontrol.cpp \
    applicationdescription.cpp \
    dbusmonitor.cpp
HEADERS += mainwindow.h \
    daemon.h \
    launchmonitor.h \
    settings.h \
    notifications.h \
    phonecontrol.h \
    applicationdescription.h \
    dbusmonitor.h
FORMS += mainwindow.ui

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
    com.sierrasoftworks.AppLock.service

INCLUDEPATH += $$PWD

Qml.files = qml/mainWindow.qml
Qml.path = /opt/AppLock/qml
export(Qml.files)
export(Qml.path)

INSTALLS += Qml

OTHER_FILES = qml/mainWindow.qml

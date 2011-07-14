QT += declarative

SOURCES += $$PWD/qmlviewer.cpp
HEADERS += $$PWD/qmlviewer.h
INCLUDEPATH += $$PWD

# Include JS debugger library if QMLJSDEBUGGER_PATH is set
!isEmpty(QMLJSDEBUGGER_PATH) {
    include($$QMLJSDEBUGGER_PATH/qmljsdebugger-lib.pri)
} else {
    DEFINES -= QMLJSDEBUGGER
}

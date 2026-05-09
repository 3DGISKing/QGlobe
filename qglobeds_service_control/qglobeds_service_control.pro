# -------------------------------------------------
# Project created by QtCreator 2009-08-29T16:48:53
# -------------------------------------------------
TARGET = qglobeds_service_control
DESTDIR = ../bin
TEMPLATE = app
FORMS += qglobeds_ctrldialog.ui

QT += network

LIBS += -L"../lib" \
        -lqglobeds_service_common

INCLUDEPATH += ./../qglobeds_service_common \

macx: {
CONFIG += x86
}

precompile_header:!isEmpty(PRECOMPILED_HEADER):DEFINES += USING_PCH
build_pass:CONFIG(debug, debug|release): {
    INCLUDEPATH += ./GeneratedFiles \
        ./GeneratedFiles/debug \
        .
    DEPENDPATH += .
    MOC_DIR += ./GeneratedFiles/debug
    OBJECTS_DIR += debug
    DEFINES += DEBUG
}
else: {
    INCLUDEPATH += ./GeneratedFiles \
        ./GeneratedFiles/release \
        .
    DEPENDPATH += .
    MOC_DIR += ./GeneratedFiles/release
    OBJECTS_DIR += release
    DEFINES += RELEASE
}

SOURCES += main.cpp \
    qglobeds_ctrldialog.cpp \
    qglobeds_servicetrayicon.cpp

HEADERS += qglobeds_ctrldialog.h \
    qglobeds_servicetrayicon.h

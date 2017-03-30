# -------------------------------------------------
# Project created by QtCreator 2009-08-29T16:48:53
# -------------------------------------------------
TARGET = gds_service_control
DESTDIR = ../bin
TEMPLATE = app
FORMS += gds_ctrldialog.ui

QT += network

LIBS += -L"../lib" \
        -lgds_service_common

INCLUDEPATH += ./../gds_service_common \

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
    gds_ctrldialog.cpp \
    gds_servicetrayicon.cpp

HEADERS += gds_ctrldialog.h \
    gds_servicetrayicon.h

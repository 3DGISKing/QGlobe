TARGET   = gds_service
DESTDIR = ../bin
TEMPLATE = app
CONFIG   += console qt
macx: {
CONFIG += x86
}
QT = core network sql
DEFINES += QT_SQL_LIB QT_NETWORK_LIB
LIBS += -L"../lib" \
        -lgds_service_common \
        -lgds_core \
        -lgds_network \
        -lgds_database \
        -lgds_common \
        -lgdem_common \

INCLUDEPATH += ./../gds_service_common \

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

SOURCES  = main.cpp

TARGET   = qglobeds_service
DESTDIR = ../bin
TEMPLATE = app
CONFIG   += console qt
QT = core network sql
include(../qt5_compat.pri)
DEFINES += QT_SQL_LIB QT_NETWORK_LIB
LIBS += -L"../lib" \
        -lqglobeds_service_common \
        -lqglobeds_core \
        -lqglobeds_network \
        -lqglobeds_database \
        -lqglobeds_common \
        -lqglobe_common \

INCLUDEPATH += ./../qglobeds_service_common \

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

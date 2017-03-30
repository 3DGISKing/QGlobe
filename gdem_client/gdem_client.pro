# -------------------------------------------------
# Project created by QtCreator 2009-07-28T23:28:46
# -------------------------------------------------
TEMPLATE = app
QT += network \
    opengl \

DESTDIR = ../bin

DEFINES += GDEM_CLIENT

CONFIG += precompile_header \
    build_pass
macx: {
CONFIG += x86
}

TRANSLATIONS += translation\gdem_client.ts

UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

PRECOMPILED_HEADER = stable.h
precompile_header:!isEmpty(PRECOMPILED_HEADER):DEFINES += USING_PCH
build_pass:CONFIG(debug, debug|release): { 
    DEFINES += DEBUG
    OBJECTS_DIR = ./debug
}
else: { 
    DEFINES += RELEASE
    DEFINES += QT_NO_DEBUG_OUTPUT
    OBJECTS_DIR = ./release
}

include(gdemlib.lib)
include(gdem_client.pri)

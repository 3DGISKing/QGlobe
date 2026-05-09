# -------------------------------------------------
# Project created by QtCreator 2009-07-28T23:28:46
# -------------------------------------------------
TEMPLATE = app
QT += network \
    opengl \
    webenginewidgets \
    multimedia \
    multimediawidgets \

DESTDIR = ../bin

DEFINES += QGLOBE_CLIENT

CONFIG += precompile_header \
    build_pass

TRANSLATIONS += translation\qglobe_client.ts

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

exists(gdemlib.lib):include(gdemlib.lib)
include(../qt5_compat.pri)
include(qglobe_client.pri)

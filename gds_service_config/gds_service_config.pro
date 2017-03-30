# -------------------------------------------------
# Project created by QtCreator 2009-11-11T23:09:59
# -------------------------------------------------
TEMPLATE = app
DESTDIR = ../bin
QT += network


macx: {
CONFIG += x86
}

TRANSLATIONS += translation\gds_service_config.ts
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
PRECOMPILED_HEADER = stable.h
precompile_header:!isEmpty(PRECOMPILED_HEADER):DEFINES += USING_PCH
build_pass:CONFIG(debug, debug|release): { 
	INCLUDEPATH += ./GeneratedFiles \
		./GeneratedFiles/debug \
		.
	DEPENDPATH += .
	MOC_DIR += ./GeneratedFiles/debug
    DEFINES += DEBUG
    OBJECTS_DIR = ./debug
}
else: { 
	INCLUDEPATH += ./GeneratedFiles \
		./GeneratedFiles/release \
		.
	DEPENDPATH += .
	MOC_DIR += ./GeneratedFiles/release
    DEFINES += RELEASE
    OBJECTS_DIR = ./release
}

#Include file(s)
include(gds_service_config.pri)
include(gdslib.lib)

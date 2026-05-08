# Shared Qt 5.15 / VS2022 qmake settings.

CONFIG += c++17
win32-msvc*: QMAKE_CXXFLAGS += /Zc:__cplusplus

# Qt4 compatibility macros still used by legacy sources.
DEFINES += Q_WS_WIN=Q_OS_WIN
DEFINES += Q_WS_MAC=Q_OS_MAC
DEFINES += Q_WS_X11=Q_OS_UNIX

# Allow selected deprecated Qt APIs during migration.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040800

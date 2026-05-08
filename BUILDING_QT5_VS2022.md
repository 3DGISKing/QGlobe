# Building QGlobe with Qt 5.15 and Visual Studio 2022

This document describes a reproducible qmake-based build for QGlobe on Windows using Qt 5.15 and MSVC 2022.

## 1) Prerequisites

- Visual Studio 2022 with:
  - Desktop development with C++
  - MSVC v143 toolset
  - Windows 10/11 SDK
- Qt 5.15.x built for MSVC 2022 (`msvc2022_64` or `msvc2022`)
  - Required modules: `QtCore`, `QtGui`, `QtWidgets`, `QtNetwork`, `QtOpenGL`, `QtXml`, `QtXmlPatterns`, `QtSql`, `QtWebEngine`, `QtWebEngineWidgets`, `QtMultimedia`, `QtMultimediaWidgets`
- Optional: OpenSSL runtime matching your Qt package for HTTPS features.

## 2) Environment setup

Open **x64 Native Tools Command Prompt for VS 2022** (or x86 if you are intentionally building 32-bit), then set Qt paths:

```bat
set QTDIR=C:\Qt\5.15.2\msvc2022_64
set PATH=%QTDIR%\bin;%PATH%
qmake -v
```

Expected `qmake -v` output should reference Qt 5.15.x and the chosen MSVC 2022 kit.

You can also use the repo helper script:

```bat
copy env.vs2022.example.bat env.vs2022.bat
env.vs2022.bat
```

Edit `env.vs2022.bat` and set your local `QTDIR`.

## 3) Generate VS solution with qmake

From repo root:

```bat
cd /d D:\MyGithub\QGlobe
qmake gdem.pro -spec win32-msvc "CONFIG+=qt5_vs2022"
qmake -tp vc gdem.pro -spec win32-msvc "CONFIG+=qt5_vs2022"
```

This generates a Visual Studio solution and vcxproj files from current `.pro/.pri` files.

## 4) Build

```bat
msbuild gdem.sln /m /p:Configuration=Release /p:Platform=x64
```

For debug:

```bat
msbuild gdem.sln /m /p:Configuration=Debug /p:Platform=x64
```

## 5) Runtime notes

- QtWebEngine requires deployable runtime files and helper process (`QtWebEngineProcess`).
- Use `windeployqt` on the built client executable for local runtime packaging:

```bat
windeployqt --release --webengine --multimedia D:\MyGithub\QGlobe\bin\gdem_client.exe
```

Adjust path/configuration as needed.

## 6) Troubleshooting

- If web engine symbols are missing, verify `webenginewidgets` is in `QT +=`.
- If multimedia symbols are missing, verify `multimedia multimediawidgets` is in `QT +=`.
- If OpenGL classes are unresolved, verify `opengl` is present and the selected Qt kit includes QtOpenGL.
- If generated projects still reference Qt4 names, remove old generated `.sln/.vcxproj` and regenerate via qmake.

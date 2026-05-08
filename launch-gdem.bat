@echo off
setlocal

REM Local machine settings (ignored by git).
set "QTDIR=C:\qt-5.15.18-dynamic-msvc2022-x86_64"
set "PATH=%QTDIR%\bin;%PATH%"

if "%~1"=="" (
    start "" "gdem_vs2022.sln"
	start "" "gdem_server_vs2022.sln"
	start "" "gdem_client_vs2022.sln"
) else (
    start "" "%~1"
)

endlocal

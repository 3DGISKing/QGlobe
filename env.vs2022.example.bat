@echo off
setlocal

REM Copy this file to env.vs2022.bat and set your local Qt path.
set "QTDIR=C:\Qt\5.15.18\msvc2022_64"
set "PATH=%QTDIR%\bin;%PATH%"

if "%~1"=="" (
    start "" "gdem_vs2022.sln"
) else (
    start "" "%~1"
)

endlocal

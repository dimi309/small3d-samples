@echo off
setlocal enabledelayedexpansion
set args_ok=false
set opengl_ok=false
set opengl_cmake_def=

if /I "%~1" == "debug" (
set args_ok=true
set BUILDTYPE=Debug
)
if /I "%~1" == "release" (
set args_ok=true
set BUILDTYPE=Release
)
if /I "%~2" == "" set opengl_ok=true
if /I "%~2" == "opengl" (
set opengl_ok=true
set opengl_cmake_def=-DSMALL3D_OPENGL=ON
)
if not "%opengl_ok%" == "true" set args_ok=false

if "%args_ok%" == "false" (
echo Please indicate build type: debug or release, followed by opengl if you would like to also prepare OpenGL-related libraries.
endlocal & exit /b 1
)

REM echo Entering small3d directory (must have been copied here)...
REM cd small3d
REM if %errorlevel% neq 0 endlocal & exit /b %errorlevel%
REM git clean -fdx
REM if %errorlevel% neq 0 endlocal & exit /b %errorlevel%
REM cd deps
REM call prepare-vs %1 %2
REM if %errorlevel% neq 0 endlocal & exit /b %errorlevel%
REM cd ..
REM call build-vs %1 %2
REM if %errorlevel% neq 0 endlocal & exit /b %errorlevel%
REM cd ..\..

for %%A in (avoidthebug, ball, chasethegoat, frogremixed, gloom) do (
cd %%A
if exist deps rmdir /Q /S deps
mkdir deps
cd deps
for %%B in (include, lib, shaders) do (
xcopy ..\..\small3d\build\%%B %%B\ /s /e /y
if "!errorlevel!" neq "0" endlocal & exit /b !errorlevel!
)
cd ..
if exist build rmdir /Q /S build
mkdir build
cd build
cmake .. -G"Visual Studio 16 2019" -A x64 %opengl_cmake_def%
if "!errorlevel!" neq "0" endlocal & exit /b !errorlevel!
cmake --build . --config %BUILDTYPE%
if "!errorlevel!" neq "0" endlocal & exit /b !errorlevel!
cd ..\..
)

@echo all games built successfully

@endlocal

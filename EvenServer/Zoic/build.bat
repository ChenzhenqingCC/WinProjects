@echo off
setlocal enabledelayedexpansion
color 1F

set BUILDACTION=
set BUILDFLAG=

if "%1"=="build" set BUILDACTION=build
if "%1"=="rebuild" set BUILDACTION=rebuild
if "%2"=="debug" set BUILDFLAG=Debug
if "%2"=="release" set BUILDFLAG=Release
if not "%BUILDFLAG%"=="" goto sub_build

:main
echo.
echo 1.build "Debug|Win32"
echo 2.rebuild "Debug|Win32
echo 3.build "Release|Win32"
echo 4.rebuild "Release|Win32"
choice /c 1234 /m "«Î—°‘Ò:"
echo.

if %ERRORLEVEL%==1 (
	set BUILDACTION=build
	set BUILDFLAG=Debug
)

if %ERRORLEVEL%==2 (
	set BUILDACTION=rebuild
	set BUILDFLAG=Debug
)

if %ERRORLEVEL%==3 (
	set BUILDACTION=build
	set BUILDFLAG=Release
)

if %ERRORLEVEL%==4 (
	set BUILDACTION=rebuild
	set BUILDFLAG=Release
)

if not "%BUILDFLAG%"=="" goto sub_build

echo  ‰»Î¥ÌŒÛ¥ÌŒÛ!
goto main

:sub_build
call "%VS90COMNTOOLS%\vsvars32.bat"
echo.
echo Start %BUILDACTION% "%BUILDFLAG%|Win32" ...
devenv Zoic.vcproj /%BUILDACTION% "%BUILDFLAG%|Win32"
goto sub_end

:sub_end
pause

@echo OFF
setlocal enabledelayedexpansion
cd %cd%
set XECS_PATH=%cd%

rem --------------------------------------------------------------------------------------------------------
rem Set the color of the terminal to blue with yellow text
rem --------------------------------------------------------------------------------------------------------
COLOR 8E
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore Cyan  UPDATING PROJECT ...
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.

:DOWNLOAD_DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

REM echo.
REM rmdir "../_dependencies/glfw" /S /Q
REM git clone https://github.com/glfw/glfw.git "../_dependencies/glfw"
REM if %ERRORLEVEL% GEQ 1 goto :PAUSE

rmdir "../_dependencies/glut" /S /Q
git clone https://github.com/markkilgard/glut.git "../_dependencies/glut"
if %ERRORLEVEL% GEQ 1 goto :PAUSE


:FIND_VSTUDIO
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White XECS - FINDING VISUAL STUDIO / MSBuild
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
cd /d %XECS_PATH%
for /f "usebackq tokens=*" %%i in (`.\..\dependencies\xcore\bin\vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    SET MSBUILD=%%i
    GOTO :BREAK_OUT
)
:BREAK_OUT

for /f "usebackq tokens=1* delims=: " %%i in (`.\..\dependencies\xcore\bin\vswhere -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop`) do (
    if /i "%%i"=="installationPath" set VSPATH=%%j
)

IF EXIST "%MSBUILD%" ( 
    echo OK 
    GOTO :COMPILATION
    )
echo Failed to find MSBuild!!! 
GOTO :ERROR

:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White XECS - COMPILING DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

cd ../dependencies/xcore/builds
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd /d %XECS_PATH%

powershell write-host -fore Cyan glut: Updating...
"%VSPATH%\Common7\IDE\devenv.exe" "%CD%\..\dependencies\glut\glut_2012.vcxproj" /upgrade
if %ERRORLEVEL% GEQ 1 goto :ERROR

echo.
powershell write-host -fore Cyan glut Release: Compiling...
"%MSBUILD%" "%CD%\..\dependencies\glut\glut_2012.vcxproj" /p:configuration=Release /p:Platform="x64" /verbosity:minimal 
if %ERRORLEVEL% GEQ 1 goto :ERROR

echo.
powershell write-host -fore Cyan glut Debug: Compiling...
"%MSBUILD%" "%CD%\..\dependencies\glut\glut_2012.vcxproj" /p:configuration=Debug /p:Platform="x64" /verbosity:minimal 
if %ERRORLEVEL% GEQ 1 goto :ERROR

:DONE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DONE!!
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
goto :PAUSE

:ERROR
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------
powershell write-host -fore Red ERROR!!
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------

:PAUSE
rem if no one give us any parameters then we will pause it at the end, else we are assuming that another batch file called us
if %1.==. pause

@echo off
rem Automatically increase build number
rem parsing the line 
rem
rem #define BUILD_COUNT 100
rem
rem from file build_count.h
rem and regenerating it.
FOR /F "tokens=*" %%g IN ('findstr define %1') DO ( SET line=%%g )
rem echo %line%
FOR /F "tokens=3 usebackq" %%a IN (`echo %line%`) DO ( SET build_no=%%a )

set /A build_no=%build_no%+1
echo %build_no%

rem set outf="outf.h"
set outf=%1
(
echo // Counter of the current build, updated by increase_build_number.bat script
echo #define BUILD_COUNT %build_no%
echo int g_auroraBuildCounter = BUILD_COUNT;
) > %outf%

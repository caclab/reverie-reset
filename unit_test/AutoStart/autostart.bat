@echo off

set _interval=10

set _processExe=AutoStart.exe

set _processCmd=.\bin\AutoStart.exe

:LOOP

tasklist | find /C "%_processExe%" > temp.txt
set /p num= < temp.txt
del /F temp.txt

if "%num%" == "0" (start %_processCmd% | echo start %_processExe% at %time%)

if "%num%" NEQ "0" echo already launched

ping -n %_interval% 127.0.0.1>nul

goto LOOP

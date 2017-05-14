@echo off

set _interval=5

set _processName=AutoStart

set _processCmd=.\bin\AutoStart.exe

set _username=Administrator

:LOOP
set /a isAlive=false

tasklist /FI "username eq %_username%" | find /C "%_processName%" > temp.txt
set /p num= < temp.txt
del /F temp.txt

if "%num%" == "0" (
start %_processCmd% | echo start %_processName% at %time%
)

if "%num%" NEQ "0" echo started

ping -n %_interval% 127.1>nul

goto LOOP

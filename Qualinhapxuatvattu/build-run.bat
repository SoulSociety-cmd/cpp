
@echo off
cd /d "%~dp0"

g++ main.cpp -o main.exe

if errorlevel 1 (
    echo.
    echo BIEN DICH THAT BAI!
    pause
    exit /b 1
)

start "" "%~dp0main.exe"

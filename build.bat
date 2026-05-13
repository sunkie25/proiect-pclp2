@echo off
taskkill /f /im main.exe /t 2>nul
gcc -Wall -Wextra -g main.c -o main.exe -lfreeglut -lglu32 -lopengl32
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)
echo Compilation successful!
main.exe
@echo off
setlocal

taskkill /f /im main.exe /t 2>nul

if /I "%1"=="release" goto release

:test
gcc -Wall -Wextra -Wdeclaration-after-statement -g -DTEST_MODE main.c -o main.exe -lfreeglut -lglu32 -lopengl32
if %errorlevel% neq 0 (
    echo Test build failed!
    pause
    exit /b 1
)

echo Test build successful!
main.exe
exit /b 0

:release
gcc -Wall -Wextra -Wdeclaration-after-statement -O2 -mwindows main.c -o main.exe -lfreeglut -lglu32 -lopengl32
if %errorlevel% neq 0 (
    echo Release build failed!
    pause
    exit /b 1
)

echo Release build successful!
start "" main.exe
exit /b 0

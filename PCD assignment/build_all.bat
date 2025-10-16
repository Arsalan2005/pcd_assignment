@echo off
REM Unified Build Script for Compiler Project

echo.
echo ╔══════════════════════════════════════════════════════════════╗
echo ║              Compiler Build System                           ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.
echo Select build option:
echo   1. GUI Compiler (Recommended)
echo   2. Console Compiler
echo   3. Build Both
echo   4. Clean All
echo.
set /p choice="Enter choice (1-4): "

if "%choice%"=="1" goto build_gui
if "%choice%"=="2" goto build_console
if "%choice%"=="3" goto build_both
if "%choice%"=="4" goto clean
goto end

:build_gui
echo.
echo Building GUI Compiler...
gcc -Wall -std=c11 -O2 -mwindows -o compiler_gui.exe gui_compiler.c lexer.c ll_parser.c lr_parser.c utils.c codegen.c -lcomctl32 -lgdi32
if %ERRORLEVEL% EQU 0 (
    echo ✓ GUI Build successful!
    echo Starting GUI...
    start compiler_gui.exe
) else (
    echo ✗ Build failed!
)
goto end

:build_console
echo.
echo Building Console Compiler...
gcc -Wall -std=c11 -O2 -o compiler.exe main.c lexer.c ll_parser.c lr_parser.c utils.c
if %ERRORLEVEL% EQU 0 (
    echo ✓ Console Build successful!
    echo Run with: compiler.exe
) else (
    echo ✗ Build failed!
)
goto end

:build_both
echo.
echo Building Console Compiler...
gcc -Wall -std=c11 -O2 -o compiler.exe main.c lexer.c ll_parser.c lr_parser.c utils.c
if %ERRORLEVEL% EQU 0 (
    echo ✓ Console Build successful!
) else (
    echo ✗ Console Build failed!
    goto end
)

echo.
echo Building GUI Compiler...
gcc -Wall -std=c11 -O2 -mwindows -o compiler_gui.exe gui_compiler.c lexer.c ll_parser.c lr_parser.c utils.c codegen.c -lcomctl32 -lgdi32
if %ERRORLEVEL% EQU 0 (
    echo ✓ GUI Build successful!
    echo.
    echo Both versions built successfully!
) else (
    echo ✗ GUI Build failed!
)
goto end

:clean
echo.
echo Cleaning build files...
if exist compiler.exe del compiler.exe
if exist compiler_gui.exe del compiler_gui.exe
if exist *.o del *.o
echo ✓ Clean complete!
goto end

:end
echo.
pause

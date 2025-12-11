@echo off
REM Build script pentru Windows (fara CMake)

echo ====================================
echo Building Effective C++ Demo Project
echo ====================================
echo.

REM Creare director build daca nu exista
if not exist "build" mkdir build

REM Compilare cu g++ (MinGW) sau MSVC cl
echo Checking for g++...
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found g++, compiling with C++17 and threading support...
    g++ -std=c++17 -I./include -pthread -o build/EffectiveCppDemo.exe src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ====================================
        echo Build successful!
        echo Executable: build\EffectiveCppDemo.exe
        echo ====================================
        echo.
        echo Running the program...
        echo.
        build\EffectiveCppDemo.exe
    ) else (
        echo Build failed!
    )
) else (
    echo g++ not found. Please install MinGW or use CMake with MSVC.
    echo.
    echo Alternative: Use CMake with Visual Studio:
    echo   mkdir build
    echo   cd build
    echo   cmake ..
    echo   cmake --build .
)

pause



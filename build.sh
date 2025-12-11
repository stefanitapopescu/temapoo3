#!/bin/bash
# Build script pentru Linux/MacOS cu CMake

EXEC_NAME="EffectiveCppDemo"

echo "===================================="
echo "Building Effective C++ Demo Project"
echo "===================================="
echo ""

echo "--- 1. Pregatire folder build ---"
mkdir -p build

echo "--- 2. Configurare CMake ---"
cmake -S . -B build

echo "--- 3. Compilare ---"
cmake --build build

if [ $? -eq 0 ]; then
    echo ""
    echo "===================================="
    echo "Build successful!"
    echo "Executable: build/$EXEC_NAME"
    echo "===================================="
    echo ""
    echo "--- 4. RULARE ---"
    ./build/$EXEC_NAME
else
    echo "!!! EROARE LA COMPILARE !!!"
    exit 1
fi



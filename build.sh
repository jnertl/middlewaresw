#!/bin/bash
set -e

BUILD_DIR="build"

if [ "$1" == "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    echo "Clean complete."
    exit 0
fi

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"
echo "Running CMake..."
cmake ..
echo "Building project..."
make -j$(nproc)
echo "Build complete."

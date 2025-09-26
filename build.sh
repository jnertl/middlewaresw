#!/bin/bash

BUILD_DIR="build_application"

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
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi
echo "Building project..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 2
fi
echo "Build complete."

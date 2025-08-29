#!/bin/bash
set -e

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Building project first..."
    ./build.sh
fi

build/tests/runUnitTests --gtest_output=xml:gtestresults.xml

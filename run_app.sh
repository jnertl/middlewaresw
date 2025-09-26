#!/bin/bash

if [ ! -f build_application/middlewaresw ]; then
    echo "Executable build/middlewaresw not found. Building project..."
    ./build.sh
fi

if [ $# -lt 1 ]; then
    echo "Usage: $0 <UpdateIntervalMs>"
    exit 1
fi

build_application/middlewaresw "$1"

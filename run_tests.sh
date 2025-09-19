#!/bin/bash
set -e

./build.sh

./build_application/tests/runUnitTests --gtest_output=xml:gtestresults.xml > /dev/null 2>&1

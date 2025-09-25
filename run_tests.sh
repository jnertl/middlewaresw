#!/bin/bash
set -e

./build.sh

./build_application/tests/runUnitTests --gtest_output=xml:gtestresults.xml > /dev/null 2>&1
if [ -f gtestresults.xml ]; then
    total_tests=$(grep -o 'tests="[0-9]*"' gtestresults.xml | head -1 | grep -o '[0-9]*')
    failures=$(grep -o 'failures="[0-9]*"' gtestresults.xml | head -1 | grep -o '[0-9]*')
    disabled=$(grep -o 'disabled="[0-9]*"' gtestresults.xml | head -1 | grep -o '[0-9]*')
    errors=$(grep -o 'errors="[0-9]*"' gtestresults.xml | head -1 | grep -o '[0-9]*')

    echo "Google Test Report:"
    echo "-------------------"
    echo "Total tests:   $total_tests"
    echo "Failures:      $failures"
    echo "Disabled:      $disabled"
    echo "Errors:        $errors"

    if [ "$failures" -gt 0 ] || [ "$errors" -gt 0 ]; then
        echo "Some tests failed or had errors."
        exit 1
    else
        echo "All tests passed."
    fi
else
    echo "gtestresults.xml not found."
    exit 1
fi
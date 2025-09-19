#!/bin/bash
# run_coverage.sh: Build with coverage flags, run tests, and generate a coverage report for C++ code

set -e

COVERAGE_BUILD_DIR=build_coverage
COVERAGE_INFO=coverage.info
COVERAGE_HTML_DIR=coverage_html

if [ "$1" == "clean" ]; then
    echo "Cleaning coverage build directory..."
    rm -rf ${COVERAGE_BUILD_DIR}
    echo "Clean complete."
    exit 0
fi

# Clean previous coverage data
rm -rf $COVERAGE_BUILD_DIR $COVERAGE_HTML_DIR
mkdir -p ${COVERAGE_BUILD_DIR}
echo ${COVERAGE_BUILD_DIR}
cd ${COVERAGE_BUILD_DIR}

# Build coverage-enabled tests
echo "Building coverage..."
cmake ../coverage
make -j$(nproc)

# Run tests
./test_coverage > /dev/null 2>&1

# Clean and explude test artifacts from coverage
echo "Clean test artifacts folder: middlewaresw/tests..."
find . -type d -path "*/middlewaresw/tests" -exec rm -rf "{}" +

# Capture coverage data
echo "Capturing coverage..."
#  --ignore-errors mismatch,mismatch
lcov --capture --directory . --output-file $COVERAGE_INFO -rc geninfo_unexecuted_blocks=1

# Remove coverage for system and test files
echo "Filtering coverage..."
lcov --remove ${COVERAGE_INFO} '/usr/*' 'engine_data.pb.*' --output-file $COVERAGE_INFO

# Generate HTML report
genhtml ${COVERAGE_INFO} --output-directory ../$COVERAGE_HTML_DIR

echo "Coverage HTML report generated in $COVERAGE_HTML_DIR/index.html"

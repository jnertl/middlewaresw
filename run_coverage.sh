#!/bin/bash
# run_coverage.sh: Build with coverage flags, run tests, and generate a coverage report for C++ code

COVERAGE_BUILD_DIR=build_coverage
COVERAGE_INFO=coverage.info
COVERAGE_SUMMARY=coverage_summary.txt
COVERAGE_HTML_DIR=coverage_html
MIN_LINE_COVERAGE_TRESHOLD=10.0
MIN_FUNTION_COVERAGE_TRESHOLD=10.0

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
lcov --remove ${COVERAGE_INFO} '/usr/*' 'spdlog/*' 'engine_data.pb.*' --output-file $COVERAGE_INFO

# Generate HTML report
genhtml ${COVERAGE_INFO} --output-directory ../$COVERAGE_HTML_DIR

echo "Coverage HTML report generated in $COVERAGE_HTML_DIR/index.html"

echo "**********************************************************************"
echo "Coverage report summary:"
echo "**********************************************************************"
lcov --summary $COVERAGE_INFO > $COVERAGE_SUMMARY
cat $COVERAGE_SUMMARY
echo
lines_covered=$(grep -Po 'lines\.*:\s*\K[0-9]+\.[0-9]+' $COVERAGE_SUMMARY)
line_coverage_ok=1
if (( $(echo "$lines_covered < $MIN_LINE_COVERAGE_TRESHOLD" | bc -l) )); then
    echo "FAILED: Lines coverage ($lines_covered%) is below threshold ($MIN_LINE_COVERAGE_TRESHOLD%)"
    line_coverage_ok=0
else
    echo "PASSED: Lines coverage ($lines_covered%) meets the threshold ($MIN_LINE_COVERAGE_TRESHOLD%)"
fi
echo
functions_covered=$(grep -Po 'functions\.*:\s*\K[0-9]+\.[0-9]+' $COVERAGE_SUMMARY)
function_coverage_ok=1
if (( $(echo "$functions_covered < $MIN_FUNTION_COVERAGE_TRESHOLD" | bc -l) )); then
    echo "FAILED: Functions coverage ($functions_covered%) is below threshold ($MIN_FUNTION_COVERAGE_TRESHOLD%)"
    function_coverage_ok=0
else
    echo "PASSED: Functions coverage ($functions_covered%) meets the threshold ($MIN_FUNTION_COVERAGE_TRESHOLD%)"
fi

final_status=0
if [[ $line_coverage_ok -eq 0 || $function_coverage_ok -eq 0 ]]; then
    final_status=1
fi
echo "**********************************************************************"

exit $final_status

#!/bin/bash

# This script first builds the project and then runs the test executable.

# Exit immediately if a command exits with a non-zero status.
set -e

# Find the directory where the script is located.
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Run the build script to ensure everything is compiled.
echo "--- Ensuring project is built ---"
$SCRIPT_DIR/build.sh

# Define the path to the test executable.
TEST_EXECUTABLE="$SCRIPT_DIR/build/testExecutables/run_lexer_tests"

# Check if the test executable exists.
if [ ! -f "$TEST_EXECUTABLE" ]; then
    echo "ERROR: Test executable not found at $TEST_EXECUTABLE"
    echo "Please ensure the project has been built successfully."
    exit 1
fi

# Run the tests.
echo "--- Running tests ---"
$TEST_EXECUTABLE

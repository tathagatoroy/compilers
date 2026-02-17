#!/bin/bash

# This script automates the CMake build process.

# Exit immediately if a command exits with a non-zero status.
set -e

# Define the build directory.
BUILD_DIR="build"

# Create the build directory if it doesn't exist.
echo "--- Creating build directory ---"
mkdir -p $BUILD_DIR

# Navigate into the build directory.
cd $BUILD_DIR

# Run CMake to configure the project.
# The ".." points CMake to the source directory (where the root CMakeLists.txt is).
echo "--- Configuring project with CMake ---"
cmake ..

# Build the project using the number of available CPU cores to speed up compilation.
# The "." tells cmake to build the project in the current directory.
echo "--- Building project ---"
cmake --build . -j$(nproc)

echo "--- Build complete ---"
echo "Executables are in: $BUILD_DIR/executables"
echo
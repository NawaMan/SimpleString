#!/bin/bash
set -e

BUILD_TYPE=$1
BUILD_TESTS=$2

# Clean any existing build
rm -rf build

# Create fresh build directory
mkdir -p build
cd build

# Configure with absolute paths
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DBUILD_TESTING=$BUILD_TESTS \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      /build

# Build
make -j$(nproc)

# Run tests if enabled
if [ "${BUILD_TESTS}" = "ON" ]; then
    ctest --output-on-failure
fi

# Copy output files
mkdir -p /build/output
cp -r * /build/output/
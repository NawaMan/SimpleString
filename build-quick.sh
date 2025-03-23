#!/bin/bash

# build-quick.sh: Quick development build script for SString library
#
# Purpose:
#   - Fast build for development and testing
#   - Supports Debug/Release builds
#   - Configurable test building
#
# Usage: 
#   ./build-quick.sh [options]
#
# Options:
#   -h, --help          Show this help message
#   -t, --with-tests   Build with tests (default)
#   -n, --no-tests     Build without tests
#   -c, --clean        Clean build directory before building
#   --coverage        Enable code coverage (requires tests)
#
# Examples:
#   ./build-quick.sh              # Debug build with tests
#   ./build-quick.sh -n          # Debug build without tests
#   ./build-quick.sh --coverage   # Debug build with tests and coverage
#   ./build-quick.sh -c          # Clean debug build

set -e  # Exit on error

# Default configuration
BUILD_TESTS="ON"
ENABLE_COVERAGE="OFF"
CLEAN_BUILD=0

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default options
BUILD_TESTS="ON"
CLEAN_BUILD=0

# Function to print section header
print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}\n"
}

# Function to print status
print_status() {
    echo -e "${BLUE}$1${NC}"
}

# Function to show help
show_help() {
    cat << EOF
Usage: $(basename "$0") [options]

Options:
  -h, --help          Show this help message
  -t, --with-tests   Build with tests (default)
  -n, --no-tests     Build without tests
  -c, --clean        Clean build directory before building

Examples:
  ./build-quick.sh              # Debug build with tests
  ./build-quick.sh -n          # Debug build without tests
  ./build-quick.sh -c          # Clean debug build with tests
EOF
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -t|--with-tests)
            BUILD_TESTS="ON"
            shift
            ;;
        -n|--no-tests)
            BUILD_TESTS="OFF"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=1
            shift
            ;;
        --coverage)
            ENABLE_COVERAGE="ON"
            BUILD_TESTS="ON"  # Coverage requires tests
            shift
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            show_help
            ;;
    esac
done

print_section "Quick Build Configuration"
echo -e "Build Tests: ${BLUE}${BUILD_TESTS}${NC}"
echo -e "Clean Build: ${BLUE}${CLEAN_BUILD}${NC}"

# Clean if requested
if [ $CLEAN_BUILD -eq 1 ]; then
    print_section "Cleaning Build Directory"
    rm -rf build
fi

# Create build directory
mkdir -p build
cd build

# Configure
print_section "Configuring Build"
print_status "Running CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DBUILD_TESTING=${BUILD_TESTS} \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DENABLE_COVERAGE=ON \
      ..

# Build
print_section "Building Project"
print_status "Running make..."
make -j$(nproc)

# Run tests if enabled
if [ "${BUILD_TESTS}" = "ON" ]; then
    print_section "Running Tests"
    print_status "Running CTest..."
    ctest --output-on-failure

    # Generate coverage report if enabled
    if [ -x "$(command -v lcov)" ]; then
        print_status "Generating coverage report..."
        lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
        lcov --remove coverage.info '/usr/*' --output-file coverage.info --rc lcov_branch_coverage=1
    fi
fi

# Copy compile commands to root for tooling
cp compile_commands.json ..

print_section "Build Summary"
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "Build Type: ${BLUE}Debug${NC}"
echo -e "Tests: ${BLUE}${BUILD_TESTS}${NC}"
echo -e "Coverage: ${BLUE}${ENABLE_COVERAGE}${NC}"
echo -e "\nOutput files:"
echo -e "  - Library: ${BLUE}$(pwd)/../dist/libsstring_lib.a${NC}"
if [ "${BUILD_TESTS}" = "ON" ]; then
    echo -e "  - Test executable: ${BLUE}$(pwd)/sstring_tests${NC}"
fi
echo -e "  - Compile Commands: ${BLUE}$(pwd)/../compile_commands.json${NC}"

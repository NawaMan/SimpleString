#!/bin/bash

# build-quick.sh: Quick development build script for Simple String library
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
#   -h, --help         Show this help message
#   -t, --with-tests   Build with tests (default)
#   -n, --no-tests     Build without tests
#   -c, --clean        Clean build directory before building
#   --coverage         Enable code coverage (requires tests)
#
# Examples:
#   ./build-quick.sh             # Debug build with tests
#   ./build-quick.sh -n          # Debug build without tests
#   ./build-quick.sh --coverage  # Debug build with tests and coverage
#   ./build-quick.sh -c          # Clean debug build

set -e  # Exit on error

# Default configuration
BUILD_TESTS="ON"
ENABLE_COVERAGE="OFF"
CLEAN_BUILD=0
VERBOSE_TESTS=0

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
  -h, --help         Show this help message
  -t, --with-tests   Build with tests (default)
  -n, --no-tests     Build without tests
  -c, --clean        Clean build directory before building
  -v, --verbose      Show verbose test output with individual test details

Examples:
  ./build-quick.sh             # Debug build with tests
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
        -v|--verbose)
            VERBOSE_TESTS=1
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
echo -e "Verbose Tests: ${BLUE}$([ $VERBOSE_TESTS -eq 1 ] && echo "ON" || echo "OFF")${NC}"

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
print_status  "Running CMake..."

# Configure coverage flags
if [ "${ENABLE_COVERAGE}" = "ON" ]; then
    export CXXFLAGS="${CXXFLAGS} --coverage -fprofile-arcs -ftest-coverage -fprofile-update=atomic"
    export LDFLAGS="${LDFLAGS}   --coverage"
fi

cmake -DCMAKE_BUILD_TYPE=Debug             \
      -DBUILD_TESTING=${BUILD_TESTS}       \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   \
      -DENABLE_COVERAGE=${ENABLE_COVERAGE} \
      ..

# Build
print_section "Building Project"
print_status  "Running make..."
make -j$(nproc)

# Run tests if enabled
if [ "${BUILD_TESTS}" = "ON" ]; then
    print_section "Running Tests"
    if [ $VERBOSE_TESTS -eq 1 ]; then
        print_status "Running CTest with verbose output..."
        ctest --output-on-failure -V
    else
        print_status "Running CTest..."
        ctest --output-on-failure
    fi

    # Generate coverage report if enabled
    if [ -x "$(command -v lcov)" ] && [ "${ENABLE_COVERAGE}" = "ON" ]; then
        print_status "Generating coverage report..."
        # Reset counters
        lcov --directory . --zerocounters

        # Run tests again to generate coverage
        ./sstring_tests

        # Capture coverage data
        lcov --directory      .                              \
             --capture                                       \
             --output-file    coverage.info.raw              \
             --rc             branch_coverage=1              \
             --ignore-errors  mismatch,negative,empty,unused \
             --rc             geninfo_unexecuted_blocks=1    \
             --base-directory "$(pwd)/.."

        # Remove system files and test files
        lcov --remove coverage.info.raw        \
             '/usr/include/*'                  \
             '*/tests/*'                       \
             --output-file   coverage.info     \
             --rc            branch_coverage=1 \
             --ignore-errors unused,mismatch

        # Generate HTML report
        if [ -x "$(command -v genhtml)" ]; then
            print_status "Generating HTML coverage report..."
            genhtml coverage.info                            \
                    --output-directory ../coverage_report    \
                    --branch-coverage                        \
                    --legend                                 \
                    --title  "Simple String Coverage Report" \
                    --prefix $(pwd)/..                       \
                    --rc     branch_coverage=1
            echo -e "Coverage report generated at: ${BLUE}$(pwd)/../coverage_report/index.html${NC}"
        fi
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

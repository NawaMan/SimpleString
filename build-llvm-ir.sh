#!/bin/bash

# build-llvm-ir.sh: LLVM IR generation script for Simple String library
#
# Purpose:
#   - Generate LLVM IR files for all source files in the library
#   - Support both debug and release configurations
#   - Provide options for different optimization levels
#
# Usage: 
#   ./build-llvm-ir.sh [options]
#
# Options:
#   -h, --help         Show this help message
#   -c, --clean        Clean build directory before building
#   -o, --opt-level N  Set optimization level (0-3, default: 0)
#   -d, --debug        Build with debug information (default)
#   -r, --release      Build with release optimization
#
# Examples:
#   ./build-llvm-ir.sh             # Debug build with no optimization
#   ./build-llvm-ir.sh -r -o 2     # Release build with -O2 optimization
#   ./build-llvm-ir.sh -c          # Clean and rebuild

set -e  # Exit on error

# Default configuration
OPT_LEVEL=0
BUILD_TYPE="Debug"
CLEAN_BUILD=0

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print section header
print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}\n"
}

# Function to print status
print_status() {
    echo -e "${BLUE}$1${NC}"
}

# Function to print error
print_error() {
    echo -e "${RED}$1${NC}"
}

# Function to show help
show_help() {
    cat << EOF
Usage: $(basename "$0") [options]

Options:
  -h, --help         Show this help message
  -c, --clean        Clean build directory before building
  -o, --opt-level N  Set optimization level (0-3, default: 0)
  -d, --debug        Build with debug information (default)
  -r, --release      Build with release optimization

Examples:
  ./build-llvm-ir.sh             # Debug build with no optimization
  ./build-llvm-ir.sh -r -o 2     # Release build with -O2 optimization
  ./build-llvm-ir.sh -c          # Clean and rebuild
EOF
    exit 0
}

# Check if Clang is installed
if ! command -v clang++ &> /dev/null; then
    print_error "Error: clang++ is required but not installed"
    exit 1
fi

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -c|--clean)
            CLEAN_BUILD=1
            shift
            ;;
        -o|--opt-level)
            if [[ $2 =~ ^[0-3]$ ]]; then
                OPT_LEVEL=$2
                shift 2
            else
                print_error "Invalid optimization level: $2. Must be 0-3."
                exit 1
            fi
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_help
            ;;
    esac
done

print_section "LLVM IR Build Configuration"
echo -e "Build Type: ${BLUE}${BUILD_TYPE}${NC}"
echo -e "Optimization Level: ${BLUE}-O${OPT_LEVEL}${NC}"
echo -e "Clean Build: ${BLUE}${CLEAN_BUILD}${NC}"

# Set directories
BUILD_DIR="build-llvm-ir"
IR_OUTPUT_DIR="dist/llvm-ir/${BUILD_TYPE,,}-O${OPT_LEVEL}"
SOURCE_DIR="src"
INCLUDE_DIR="include"

# Clean if requested
if [ $CLEAN_BUILD -eq 1 ]; then
    print_section "Cleaning Build Directory"
    rm -rf "${BUILD_DIR}"
    rm -rf "${IR_OUTPUT_DIR}"
fi

# Create build and output directories
mkdir -p "${BUILD_DIR}"
mkdir -p "${IR_OUTPUT_DIR}"

# First, run a normal CMake configuration to generate compile_commands.json
print_section "Configuring Build"
print_status "Running CMake to generate compile configuration..."

# Create build directory and run CMake
cd "${BUILD_DIR}"
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DCMAKE_CXX_COMPILER=clang++ \
      ..

# Go back to the project root
cd ..

# Get a list of all source files
print_section "Finding Source Files"
SOURCE_FILES=$(find ${SOURCE_DIR} -name "*.cpp")

# Generate LLVM IR for each source file
print_section "Generating LLVM IR"
for src_file in ${SOURCE_FILES}; do
    # Get the base filename without extension
    base_name=$(basename "${src_file}" .cpp)
    output_file="${IR_OUTPUT_DIR}/${base_name}.ll"
    
    print_status "Generating IR for ${src_file} -> ${output_file}"
    
    # Handle special case for unicode_category.cpp
    if [[ "${src_file}" == *"unicode_category.cpp" ]]; then
        print_status "  Skipping problematic file (constexpr issues)"
        continue
    fi
    
    # Generate LLVM IR
    clang++ -S -emit-llvm \
        -std=c++20 \
        -O${OPT_LEVEL} \
        -I${INCLUDE_DIR} \
        -I/usr/include \
        -I/usr/local/include \
        -Wno-inconsistent-missing-override \
        -o "${output_file}" \
        "${src_file}"
    
    # Check if generation was successful
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ Success${NC}"
    else
        echo -e "  ${RED}✗ Failed${NC}"
    fi
done

print_section "Build Summary"
IR_FILE_COUNT=$(find "${IR_OUTPUT_DIR}" -name "*.ll" | wc -l)
echo -e "${GREEN}LLVM IR generation completed successfully!${NC}"
echo -e "Generated ${BLUE}${IR_FILE_COUNT}${NC} LLVM IR files in: ${BLUE}${IR_OUTPUT_DIR}${NC}"

# List generated IR files
print_section "Generated LLVM IR Files"
find "${IR_OUTPUT_DIR}" -name "*.ll" | sort

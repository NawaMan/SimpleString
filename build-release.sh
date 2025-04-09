#!/bin/bash

# build-release.sh: Release build script for Simple String library
# Builds release packages for multiple platforms using Docker

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print functions
print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}\n"
}

print_status() {
    echo -e "${BLUE}${1}${NC}"
}

print_error() {
    echo -e "${RED}${1}${NC}"
}

print_success() {
    echo -e "${GREEN}${1}${NC}"
}

# Show help
show_help() {
    echo "Usage: $0 [options]"
    echo
    echo "Options:"
    echo "  -h, --help              Show this help message"
    echo "  -p, --platform PLATFORM Build for specific platform (linux, windows, macos)"
    echo "                          Default: all platforms"
    echo "  -t, --type TYPE         Package type (tar.gz, deb, rpm, zip, msi, pkg)"
    echo "                          Default: all types for selected platform"
    echo "  -v, --version VERSION   Set package version (required)"
    echo "      --exclude-llvm-ir   Exclude LLVM IR files during build (requires Clang)"
    echo
}

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    print_error "Error: Docker is required but not installed"
    exit 1
fi

# Parse arguments
PLATFORMS="linux windows macos"
PACKAGE_TYPES="all"
VERSION=""
GENERATE_LLVM_IR=1

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -p|--platform)
            PLATFORMS="$2"
            shift 2
            ;;
        -t|--type)
            PACKAGE_TYPES="$2"
            shift 2
            ;;
        -v|--version)
            VERSION="$2"
            shift 2
            ;;
        --exclude-llvm-ir)
            GENERATE_LLVM_IR=0
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# Check version
if [ -z "$VERSION" ]; then
    if [ -f "version.txt" ]; then
        VERSION=$(cat version.txt)
        print_status "Using version from version.txt: $VERSION"
    else
        print_error "Version is required. Use -v/--version or create version.txt"
        show_help
        exit 1
    fi
fi

# Create Docker build context
print_section "Creating Docker build context"

# Copy the docker-build.sh template from config directory
print_status "Copying docker-build.sh template..."
cp           config/templates/docker-build.sh docker-build.sh
chmod +x     docker-build.sh

# Copy the CMake templates from config directory
print_status "Copying CMake templates..."
mkdir -p cmake
cp config/templates/cmake/mingw-w64-x86_64.cmake          cmake/mingw-w64-x86_64.cmake
cp config/templates/cmake/windows-x86_64-msvc-cross.cmake cmake/windows-x86_64-msvc-cross.cmake

# Create dist directory
mkdir -p dist

# Build for each platform
for platform in $PLATFORMS; do
    print_section "Building for $platform"
    
    # Build the platform-specific image
    print_status "Building Docker image for $platform..."
    docker build --target    $platform          \
                 --build-arg VERSION=$VERSION   \
                 -f          config/Dockerfile  \
                 -t          sstring-$platform-builder .
    
    # Run the build
    print_status "Running build for $platform..."
    docker run --rm \
        -v "$(pwd)/dist:/build/dist" \
        sstring-$platform-builder ./docker-build.sh $platform $VERSION $GENERATE_LLVM_IR
done

print_section "Build Summary"
print_success "Release packages have been created in the dist/ directory:"
ls -l dist/

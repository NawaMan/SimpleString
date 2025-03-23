#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print section header
print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}\n"
}

# Function to build for a specific platform
build_platform() {
    local platform=$1
    local toolchain=$2
    local build_dir="build-${platform}"
    
    print_section "Building for ${platform}"
    
    # Clean and create build directory
    rm -rf "${build_dir}"
    mkdir -p "${build_dir}"
    
    # Configure and build
    cmake -B "${build_dir}" \
          -GNinja \
          -DCMAKE_TOOLCHAIN_FILE="cmake/toolchains/${toolchain}" \
          -DCMAKE_BUILD_TYPE=Release \
          -DBUILD_TESTING=OFF || {
        echo -e "${RED}Failed to configure for ${platform}${NC}"
        return 1
    }
    
    cmake --build "${build_dir}" --config Release || {
        echo -e "${RED}Failed to build for ${platform}${NC}"
        return 1
    }
    
    # Copy library to dist with platform-specific name
    if [ -f "${build_dir}/../dist/libsstring_lib.a" ]; then
        cp "${build_dir}/../dist/libsstring_lib.a" "dist/libsstring_lib-${platform}.a"
        echo -e "${GREEN}Successfully built for ${platform}${NC}"
    else
        echo -e "${RED}Library file not found for ${platform}${NC}"
        return 1
    fi
}

print_section "Starting multi-platform build"

# Clean dist directory
rm -rf dist/*
mkdir -p dist

# Build for each platform
build_platform "linux-x86_64-gcc" "linux-x86_64-gcc.cmake"
build_platform "linux-x86_64-clang" "linux-x86_64-clang.cmake"
build_platform "linux-aarch64-gcc" "linux-aarch64-gcc.cmake"
build_platform "windows-x86_64-mingw" "windows-x86_64-mingw.cmake"

print_section "Build Summary"
echo -e "${GREEN}All builds completed!${NC}"
echo -e "\nBuilt libraries:"
ls -l dist/libsstring_lib-*.a

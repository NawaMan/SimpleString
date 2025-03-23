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

# Function to check dependencies
check_dependencies() {
    local missing_deps=0

    # Check for CMake
    if ! command -v cmake >/dev/null 2>&1; then
        echo -e "${RED}Error: CMake is required but not installed${NC}"
        missing_deps=1
    fi

    # Check for Boost
    if ! dpkg -l | grep -q libboost-all-dev; then
        echo -e "${RED}Error: Boost development libraries are required but not installed${NC}"
        echo -e "${YELLOW}Install with: sudo apt-get install libboost-all-dev${NC}"
        missing_deps=1
    fi

    # Check for cross-compilation tools
    if ! command -v aarch64-linux-gnu-g++ >/dev/null 2>&1; then
        echo -e "${YELLOW}Warning: aarch64 cross-compiler not found, skipping aarch64 build${NC}"
        echo -e "${YELLOW}Install with: sudo apt-get install g++-aarch64-linux-gnu${NC}"
    fi

    if ! command -v x86_64-w64-mingw32-g++ >/dev/null 2>&1; then
        echo -e "${YELLOW}Warning: MinGW cross-compiler not found, skipping Windows build${NC}"
        echo -e "${YELLOW}Install with: sudo apt-get install g++-mingw-w64${NC}"
    fi

    if [ $missing_deps -eq 1 ]; then
        echo -e "\n${RED}Please install missing dependencies and try again${NC}"
        exit 1
    fi
}

# Create dist directory if it doesn't exist
mkdir -p dist

# Function to build for a specific platform/toolchain
build_for_platform() {
    local platform=$1
    local toolchain=$2
    local build_dir="build-${platform}"
    
    print_section "Building for ${platform}"
    
    # Create build directory
    mkdir -p "${build_dir}"
    
    # Configure
    cmake -B "${build_dir}" \
          -DCMAKE_TOOLCHAIN_FILE="cmake/toolchains/${toolchain}" \
          -DCMAKE_BUILD_TYPE=Release \
          -DBUILD_TESTING=OFF \
          . || {
        echo -e "${RED}Failed to configure for ${platform}${NC}"
        return 1
    }
    
    # Build
    cmake --build "${build_dir}" --config Release || {
        echo -e "${RED}Failed to build for ${platform}${NC}"
        return 1
    }
    
    # Rename library with platform-specific name
    if [ -f "dist/libsstring_lib.a" ]; then
        mv "dist/libsstring_lib.a" "dist/libsstring_lib-${platform}.a" || {
            echo -e "${RED}Failed to rename library for ${platform}${NC}"
            return 1
        }
        echo -e "${GREEN}Successfully built for ${platform}${NC}"
    else
        echo -e "${RED}Library file not found for ${platform}${NC}"
        return 1
    fi
}

# Main execution
print_section "Checking dependencies"
check_dependencies

# Linux builds
build_for_platform "linux-x86_64-gcc" "linux-x86_64-gcc.cmake"
build_for_platform "linux-x86_64-clang" "linux-x86_64-clang.cmake"

# Only build for aarch64 if cross-compiler is available
if command -v aarch64-linux-gnu-g++ >/dev/null 2>&1; then
    build_for_platform "linux-aarch64-gcc" "linux-aarch64-gcc.cmake"
fi

# macOS builds (when on macOS)
if [[ "$OSTYPE" == "darwin"* ]]; then
    build_for_platform "macos-x86_64" "macos-x86_64.cmake"
    build_for_platform "macos-aarch64" "macos-aarch64.cmake"
fi

# Windows builds (requires MinGW)
if command -v x86_64-w64-mingw32-g++ >/dev/null 2>&1; then
    build_for_platform "windows-x86_64-mingw" "windows-x86_64-mingw.cmake"
fi

print_section "Build Summary"
echo -e "${GREEN}All builds completed!${NC}"
echo -e "\nBuilt libraries:"
ls -l dist/libsstring_lib-*.a

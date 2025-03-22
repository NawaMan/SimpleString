#!/bin/bash

# Create dist directory if it doesn't exist
mkdir -p dist

# Function to build for a specific platform/toolchain
build_for_platform() {
    local platform=$1
    local toolchain=$2
    local build_dir="build-${platform}"
    
    echo "Building for ${platform}..."
    
    # Create build directory
    mkdir -p "${build_dir}"
    
    # Configure
    cmake -B "${build_dir}" \
          -DCMAKE_TOOLCHAIN_FILE="cmake/toolchains/${toolchain}" \
          -DCMAKE_BUILD_TYPE=Release \
          -DBUILD_TESTING=OFF \
          .
    
    # Build
    cmake --build "${build_dir}" --config Release
    
    # Rename library with platform-specific name
    mv "dist/libsstring_lib.a" "dist/libsstring_lib-${platform}.a"
}

# Linux builds
build_for_platform "linux-x86_64-gcc" "linux-x86_64-gcc.cmake"
build_for_platform "linux-x86_64-clang" "linux-x86_64-clang.cmake"
build_for_platform "linux-aarch64-gcc" "linux-aarch64-gcc.cmake"

# macOS builds (when on macOS)
if [[ "$OSTYPE" == "darwin"* ]]; then
    build_for_platform "macos-x86_64" "macos-x86_64.cmake"
    build_for_platform "macos-aarch64" "macos-aarch64.cmake"
fi

# Windows builds (requires MinGW and MSVC)
if command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    build_for_platform "windows-x86_64-mingw" "windows-x86_64-mingw.cmake"
fi

if command -v cl &> /dev/null; then
    build_for_platform "windows-x86_64-msvc" "windows-x86_64-msvc.cmake"
fi

echo "All builds completed!"

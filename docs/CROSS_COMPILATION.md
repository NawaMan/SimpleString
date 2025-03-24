# Cross-Compilation Guide

This project supports building for multiple platforms and architectures. Here's how to set up and use cross-compilation.

## Prerequisites

### Linux Host
To build for different platforms on Linux, install these packages:
```bash
# For Linux ARM64
sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# For Windows (MinGW)
sudo apt-get install mingw-w64
```

### macOS Host
To build for different architectures on macOS:
1. Install Xcode and Xcode Command Line Tools
2. Both x86_64 and arm64 toolchains are included by default

### Windows Host
To build for different platforms on Windows:
1. Install Visual Studio with C++ support for MSVC
2. Install MinGW-w64 for GCC support
3. Install Windows Subsystem for Linux (WSL) for Linux builds

## Building

### Using build-all.sh
The easiest way to build for all supported platforms is to use the build-all.sh script:
```bash
./build-all.sh
```

This will create builds for all available toolchains and place the libraries in the dist/ directory.

### Manual Cross-Compilation
To build for a specific platform manually:
```bash
# Create build directory
mkdir build-<platform>

# Configure with toolchain
cmake -B build-<platform> \
      -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/<toolchain-file>.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      .

# Build
cmake --build build-<platform> --config Release
```

Replace `<platform>` and `<toolchain-file>` with one of:
- linux-x86_64-gcc
- linux-x86_64-clang
- linux-aarch64-gcc
- macos-x86_64
- macos-aarch64
- windows-x86_64-msvc
- windows-x86_64-mingw

## Output Files
Built libraries will be placed in the dist/ directory with platform-specific names:
- libsstring_lib-linux-x86_64-gcc.a
- libsstring_lib-linux-x86_64-clang.a
- libsstring_lib-linux-aarch64-gcc.a
- libsstring_lib-macos-x86_64.a
- libsstring_lib-macos-aarch64.a
- libsstring_lib-windows-x86_64-msvc.lib
- libsstring_lib-windows-x86_64-mingw.a

## Notes
- Not all builds will be available on all hosts
- Windows MSVC builds will only work on Windows hosts
- macOS builds will only work on macOS hosts
- Cross-compilation for different architectures requires the appropriate toolchains

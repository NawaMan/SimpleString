#!/bin/bash
set -e

PLATFORM=$1
VERSION=$2

# Ensure version is provided
if [ -z "$VERSION" ]; then
    echo "Error: Version must be provided"
    exit 1
fi

case $PLATFORM in
    linux)
        # Build for x86_64 with GCC
        mkdir -p build-linux-x86_64-gcc && cd build-linux-x86_64-gcc
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Linux-x86_64-gcc" \
              -DCPACK_SYSTEM_NAME="Linux-x86_64-gcc" \
              ..
        make -j$(nproc)
        cpack -G TGZ
        cpack -G DEB
        cpack -G RPM
        cd ..

        # Build for x86_64 with Clang
        mkdir -p build-linux-x86_64-clang && cd build-linux-x86_64-clang
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DCMAKE_TOOLCHAIN_FILE=../cmake/x86_64-linux-clang.cmake \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Linux-x86_64-clang" \
              -DCPACK_SYSTEM_NAME="Linux-x86_64-clang" \
              ..
        make -j$(nproc)
        cpack -G TGZ
        cpack -G DEB
        cpack -G RPM
        cd ..

        # Build for AArch64 with GCC
        if command -v aarch64-linux-gnu-gcc &> /dev/null; then
            mkdir -p build-linux-aarch64-gcc && cd build-linux-aarch64-gcc
            cmake -DCMAKE_BUILD_TYPE=Release \
                  -DBUILD_TESTING=OFF \
                  -DCMAKE_INSTALL_PREFIX=/usr \
                  -DCMAKE_TOOLCHAIN_FILE=../cmake/aarch64-linux-gcc.cmake \
                  -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Linux-aarch64-gcc" \
                  -DCPACK_SYSTEM_NAME="Linux-aarch64-gcc" \
                  ..
            make -j$(nproc)
            cpack -G TGZ
            cpack -G DEB
            cpack -G RPM
            cd ..
        else
            echo "Warning: AArch64 cross-compiler not found, skipping AArch64 build"
        fi
        ;;
        
    windows)
        # Build for MinGW-w64
        mkdir -p build-windows-mingw && cd build-windows-mingw
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64-x86_64.cmake \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Windows-x86_64-mingw" \
              -DCPACK_SYSTEM_NAME="Windows-x86_64-mingw" \
              ..
        make -j$(nproc)
        cpack -G ZIP
        cd ..
        ;;
        
    macos)
        # Build macOS packages
        mkdir -p build-macos && cd build-macos
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-macOS-universal" \
              -DCPACK_SYSTEM_NAME="macOS-universal" \
              ..
        make -j$(nproc)
        cpack -G TGZ
        cd ..
        ;;
esac

# Copy packages to dist directory
mkdir -p /build/dist
find . -type f \( -name "*.tar.gz" -o -name "*.deb" -o -name "*.rpm" -o -name "*.zip" -o -name "*.msi" \) -exec cp {} /build/dist/ \;

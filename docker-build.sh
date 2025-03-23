#!/bin/bash
set -e

PLATFORM=$1

case $PLATFORM in
    linux)
        # Build Linux packages
        mkdir -p build-linux && cd build-linux
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              ..
        make -j$(nproc)
        # Generate each package type
        cpack -G TGZ
        cpack -G DEB
        cpack -G RPM
        ;;
        
    windows)
        # Build Windows packages
        mkdir -p build-windows && cd build-windows
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64-x86_64.cmake \
              ..
        make -j$(nproc)
        # Generate each package type
        cpack -G ZIP
        ;;
        
    macos)
        # Build macOS packages (cross-compilation placeholder)
        mkdir -p build-macos && cd build-macos
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
              ..
        make -j$(nproc)
        cpack -G TGZ
        ;;
esac

# Copy packages to dist directory
mkdir -p /build/dist
cp *.tar.gz *.deb *.rpm *.zip *.msi 2>/dev/null /build/dist/ || true

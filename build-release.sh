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
    echo "  -t, --type TYPE        Package type (tar.gz, deb, rpm, zip, msi, pkg)"
    echo "                          Default: all types for selected platform"
    echo "  -v, --version VERSION  Set package version (required)"
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

# Create common build script
cat > docker-build.sh << 'EOFSCRIPT'
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
              -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux-x86_64-clang.cmake \
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
                  -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux-aarch64-gcc.cmake \
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
              -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/windows-x86_64-mingw.cmake \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Windows-x86_64-mingw" \
              -DCPACK_SYSTEM_NAME="Windows-x86_64-mingw" \
              ..
        make -j$(nproc)
        # Build DLL and import library
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_SHARED_LIBS=ON \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DBOOST_INCLUDEDIR=/usr/x86_64-w64-mingw32/include \
              -DBOOST_LIBRARYDIR=/usr/x86_64-w64-mingw32/lib \
              -DBoost_USE_STATIC_LIBS=ON \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Windows-x86_64-mingw" \
              -DCPACK_SYSTEM_NAME="Windows-x86_64-mingw" \
              ..
        make -j$(nproc)
        x86_64-w64-mingw32-strip bin/libsstring_lib.dll

        # Create MSI package
        cat > sstring.wxs << 'WXSEOF'
<?xml version='1.0' encoding='windows-1252'?>
<Wix xmlns='http://schemas.microsoft.com/wix/2006/wi'>
    <Product Name='SString Library'
             Id='*'
             UpgradeCode='12345678-1234-1234-1234-123456789012'
             Language='1033'
             Codepage='1252'
             Version='${VERSION}'
             Manufacturer='SString'>
        <Package Id='*'
                 Keywords='Installer'
                 Description='SString Library Installer'
                 Manufacturer='SString'
                 InstallerVersion='100'
                 Languages='1033'
                 Compressed='yes'
                 SummaryCodepage='1252'/>

        <Media Id='1' Cabinet='Sample.cab' EmbedCab='yes'/>

        <Directory Id='TARGETDIR' Name='SourceDir'>
            <Directory Id='ProgramFilesFolder' Name='PFiles'>
                <Directory Id='SString' Name='SString'>
                    <Directory Id='INSTALLDIR' Name='.'>
                        <Component Id='MainLibrary' Guid='12345678-1234-1234-1234-123456789013'>
                            <File Id='LibraryDLL'
                                  Name='sstring.dll'
                                  Source='bin/libsstring_lib.dll'
                                  KeyPath='yes'/>
                        </Component>
                    </Directory>
                </Directory>
            </Directory>
        </Directory>

        <Feature Id='Complete' Level='1'>
            <ComponentRef Id='MainLibrary'/>
        </Feature>
    </Product>
</Wix>
WXSEOF

        wixl -v sstring.wxs -o "SString-${VERSION}-Windows-x86_64-mingw.msi"
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
        # Create PKG package
        mkdir -p pkg_root/usr/local/{lib,include}
        cp /build/dist/libsstring_lib.a pkg_root/usr/local/lib/
        cp /build/include/string.hpp pkg_root/usr/local/include/
        cp /build/include/char.hpp pkg_root/usr/local/include/
        cp /build/include/compare_result.hpp pkg_root/usr/local/include/

        fpm -s dir -t tar \
            -n sstring \
            -v "${VERSION}" \
            --description "SString Library - A C++ string library with UTF-16 support" \
            --url "https://github.com/NawaMan/SString" \
            --vendor "SString" \
            --license "MIT" \
            --maintainer "NawaMan" \
            --architecture universal \
            --prefix / \
            -C pkg_root \
            usr/local/lib/libsstring_lib.a \
            usr/local/include/string.hpp \
            usr/local/include/char.hpp \
            usr/local/include/compare_result.hpp
        gzip -f sstring*.tar
        mv sstring*.tar.gz "/build/dist/SString-${VERSION}-macOS-universal.tar.gz"
        cd ..
        ;;
esac

# Copy packages to dist directory
mkdir -p /build/dist

# Find and copy only release packages
find . -type f \( \
    -name "SString-*.tar.gz" -o \
    -name "SString-*.deb" -o \
    -name "SString-*.rpm" -o \
    -name "SString-*.pkg" -o \
    -name "SString-*.msi" \
\) -exec cp {} /build/dist/ \;

# Clean up any temporary files from package creation
find /build/dist -type f ! -name "SString-*" -delete
EOFSCRIPT

chmod +x docker-build.sh

# Create Windows toolchain file
mkdir -p cmake
cat > cmake/mingw-w64-x86_64.cmake << 'EOFCMAKE'
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Set target environment path
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set installation paths
set(CMAKE_INSTALL_PREFIX "C:/Program Files/SString" CACHE PATH "Installation directory")

# Configure Boost for cross-compilation
set(Boost_INCLUDE_DIR /usr/x86_64-w64-mingw32/include)
set(Boost_LIBRARY_DIR /usr/x86_64-w64-mingw32/lib)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_NO_SYSTEM_PATHS ON)

# Set Boost component paths
set(Boost_LOCALE_LIBRARY_DEBUG ${Boost_LIBRARY_DIR}/libboost_locale-mt-x64.a)
set(Boost_LOCALE_LIBRARY_RELEASE ${Boost_LIBRARY_DIR}/libboost_locale-mt-x64.a)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Set target environment path
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set installation paths
set(CMAKE_INSTALL_PREFIX "C:/Program Files/SString" CACHE PATH "Installation directory")
EOFCMAKE

# Create dist directory
mkdir -p dist

# Build for each platform
for platform in $PLATFORMS; do
    print_section "Building for $platform"
    
    # Build the platform-specific image
    print_status "Building Docker image for $platform..."
    docker build --target $platform \
                --build-arg VERSION=$VERSION \
                -f config/Dockerfile \
                -t sstring-$platform-builder .
    
    # Run the build
    print_status "Running build for $platform..."
    docker run --rm \
        -v "$(pwd)/dist:/build/dist" \
        sstring-$platform-builder ./docker-build.sh $platform $VERSION
done

print_section "Build Summary"
print_success "Release packages have been created in the dist/ directory:"
ls -l dist/

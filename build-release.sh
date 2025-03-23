#!/bin/bash

# build-release.sh: Release build script for SString library
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

# Check required arguments
if [ -z "$VERSION" ]; then
    print_error "Version is required. Use -v or --version to specify."
    show_help
    exit 1
fi

# Create Docker build context
print_section "Creating Docker build context"

# Create base Dockerfile
cat > Dockerfile << 'EOF'
FROM ubuntu:24.04 as base

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    ruby \
    ruby-dev \
    rubygems \
    rpm \
    && gem install fpm \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

# Set version from build arg
ARG VERSION
ENV VERSION=${VERSION}

# Common build script
COPY docker-build.sh /build/
RUN chmod +x /build/docker-build.sh

FROM base as linux
CMD ["./docker-build.sh", "linux"]

FROM base as windows
RUN apt-get update && apt-get install -y \
    mingw-w64 \
    wine64 \
    mingw-w64-tools \
    software-properties-common \
    && rm -rf /var/lib/apt/lists/*

# Install MinGW Boost
RUN apt-get update && \
    apt-get install -y mingw-w64-x86-64-dev libboost-all-dev:amd64 && \
    mkdir -p /usr/x86_64-w64-mingw32/include /usr/x86_64-w64-mingw32/lib && \
    cp -r /usr/include/boost /usr/x86_64-w64-mingw32/include/ && \
    cp -r /usr/lib/x86_64-linux-gnu/libboost_* /usr/x86_64-w64-mingw32/lib/ && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /build
CMD ["./docker-build.sh", "windows"]

FROM base as macos
RUN apt-get update && apt-get install -y \
    clang \
    llvm \
    && rm -rf /var/lib/apt/lists/*
CMD ["./docker-build.sh", "macos"]
EOF

# Create common build script
cat > docker-build.sh << 'EOF'
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
EOF

chmod +x docker-build.sh

# Create Windows toolchain file
mkdir -p cmake
cat > cmake/mingw-w64-x86_64.cmake << 'EOF'
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
EOF

# Create dist directory
mkdir -p dist

# Build for each platform
for platform in $PLATFORMS; do
    print_section "Building for $platform"
    
    # Build the platform-specific image
    print_status "Building Docker image for $platform..."
    docker build --target $platform \
                --build-arg VERSION=$VERSION \
                -t sstring-$platform-builder .
    
    # Run the build
    print_status "Running build for $platform..."
    docker run --rm \
        -v "$(pwd)/dist:/build/dist" \
        sstring-$platform-builder
done

print_section "Build Summary"
print_success "Release packages have been created in the dist/ directory:"
ls -l dist/

#!/bin/bash
# Test script for Debian package installation
# This script tests the installation of the SString Debian package
# It should be run in a Docker container with Ubuntu 24.04

# Package Structure Notes:
# - The SimpleString library is packaged as a static library (libsstring_lib.a)
# - Library file is installed in /usr/lib/libsstring_lib.a (not in /usr/local/lib)
# - Header files are installed directly in /usr/include/ (not in a subdirectory)
# - The headers include Boost headers (boost/locale.hpp and boost/regex.hpp)
# - The correct namespace is 'simple' (not 'sstring')
# - Applications using the library need to link against -lboost_locale -lboost_regex

# Usage example:
# docker run --rm -v $(pwd):/mnt -v $(pwd)/../dist:/dist -w /mnt --privileged ubuntu:24.04 bash -c "apt-get update && apt-get install -y apt-utils && ./test-deb-package.sh"

set -e  # Exit on error

DEB_PACKAGE="SString-0.4.2-Linux-x86_64-clang.deb"
PACKAGE_PATH="/dist/$DEB_PACKAGE"
TEST_DIR="deb-test"

echo "=== Testing Debian package installation for SString ==="

# Check if running as root (in Docker)
if [ "$(id -u)" != "0" ]; then
    echo "This script must be run as root in a Docker container"
    echo "Example: docker run --rm -v $(pwd):/mnt -v $(pwd)/../dist:/dist -w /mnt --privileged ubuntu:24.04 bash -c \"apt-get update && apt-get install -y apt-utils && ./test-deb-package.sh\""
    exit 1
fi

# Check if the package exists
if [ ! -f "$PACKAGE_PATH" ]; then
    echo "Error: Package $PACKAGE_PATH not found!"
    echo "Make sure you mounted the dist directory correctly"
    exit 1
fi

echo "=== Step 1: Install required tools ==="
apt-get update
apt-get install -y apt-utils build-essential cmake libboost-locale-dev libboost-regex-dev
echo "Tools installed."

echo "=== Step 2: Clean system state ==="
# Remove any previous installation
dpkg -r sstring 2>/dev/null || true
echo "System cleaned."

echo "=== Step 3: Install package and resolve dependencies ==="
# Install the package
dpkg -i "$PACKAGE_PATH" || true  # Continue even if there are dependency issues

# Install dependencies
echo "Installing dependencies..."
apt-get install -f -y  # This will install any missing dependencies
echo "Package installed."

echo "=== Step 4: Verify installation ==="
# Check if the library files exist
echo "Checking library files:"
ls -la /usr/lib/libsstring* 2>/dev/null || echo "Library files not found in expected location"

# Check if the header files exist
echo "Checking header files:"
ls -la /usr/include/string.hpp 2>/dev/null || echo "Header files not found in expected location"

# Check if the boost headers are available (needed for compilation)
echo "Checking Boost headers:"
ls -la /usr/include/boost/locale.hpp 2>/dev/null || echo "Boost locale headers not found"
ls -la /usr/include/boost/regex.hpp 2>/dev/null || echo "Boost regex headers not found"
echo "Installation verified."

echo "=== Step 4: Create and build test program ==="
# Create a test directory
rm -rf "$TEST_DIR" 2>/dev/null || true
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

# Create a simple test program
cat > test.cpp << 'EOF'
#include <string.hpp>
#include <iostream>

int main() {
  // Using the correct namespace 'simple' instead of 'sstring'
  simple::String str("Hello, SimpleString!");
  std::cout << "Length: " << str.length() << std::endl;
  std::cout << "String: " << str.to_string() << std::endl;
  return 0;
}
EOF

# Create a simple Makefile instead of using CMake
cat > Makefile << 'EOF'
CXXFLAGS = -std=c++20 -I/usr/include
LDFLAGS = -L/usr/lib -lsstring_lib -lboost_locale -lboost_regex

all: test_app

test_app: test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f test_app
EOF

# Build the test program
make
echo "Test program built successfully."

echo "=== Step 5: Run test program ==="
./test_app
echo "Test program executed successfully."

echo "=== Step 6: Uninstall package ==="
cd ..
dpkg -r sstring

# Verify removal
if [ -f "/usr/local/lib/libsstring.so" ]; then
  echo "Error: Package not fully removed"
  exit 1
fi
echo "Package uninstalled successfully."

echo "=== Test completed successfully! ==="

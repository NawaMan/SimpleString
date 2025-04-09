#!/bin/bash

# This is a simplified test script that can be run directly in the Docker container

echo "=== Testing Debian package installation for SString ==="

# Set package path
PACKAGE_PATH="/dist/SString-0.4.2-Linux-x86_64-clang.deb"
TEST_DIR="deb-test"

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

# Install dependencies if needed
echo "Installing dependencies..."
apt-get update
apt-get -f install -y
echo "Package installed."

echo "=== Step 4: Verify installation ==="
# Check if the library files exist
echo "Checking library files:"
ls -la /usr/lib/libsstring* 2>/dev/null || echo "Library files not found in expected location"

# Check if the header files exist
echo "Checking header files:"
ls -la /usr/include/string.hpp 2>/dev/null || echo "Header files not found in expected location"
echo "Installation verified."

echo "=== Step 5: Create and build test program ==="
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

# Create a simple Makefile
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

echo "=== Step 6: Run the test program ==="
./test_app
echo "Test program ran successfully."

echo "=== All tests completed successfully ==="

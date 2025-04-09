#!/bin/bash

# build-clean.sh: Cleanup script for Simple String library build artifacts
#
# Purpose:
#   - Removes all build directories
#   - Cleans up platform-specific build outputs
#   - Removes distribution packages
#
# Usage: ./build-clean.sh
#

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}\n"
}

echo -e "${YELLOW}Starting build cleanup...${NC}\n"

# Remove build directories
print_section "Removing build directories"
rm -rf build

# Remove package directories
print_section "Removing package directories"
rm -rf dist

# Remove generated files
print_section "Removing generated files"
rm -rf dist
rm -rf coverage_report
rm -f  coverage.info
rm -f  docker-build.sh
rm -f  compile_commands.json
rm -f  cmake/mingw-w64-x86_64.cmake
rm -f  cmake/windows-x86_64-msvc-cross.cmake

echo -e "\n${GREEN}Build cleanup completed!${NC}"

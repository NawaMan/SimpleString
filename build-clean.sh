#!/bin/bash

# build-clean.sh: Cleanup script for SString library build artifacts
#
# Purpose:
#   - Removes all build directories
#   - Cleans up platform-specific build outputs
#   - Removes distribution packages
#
# Usage: ./build-clean.sh
#
# Cleans:
#   - build/          (local build)
#   - build-linux-*   (Linux platform builds)
#   - build-windows-* (Windows platform builds)
#   - dist/           (distribution packages)

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
rm -rf build-linux-*
rm -rf build-windows-*

# Remove package directories
print_section "Removing package directories"
rm -rf dist

echo -e "\n${GREEN}Build cleanup completed!${NC}"

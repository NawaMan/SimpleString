#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print section header
print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}\n"
}

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    echo -e "${RED}Error: Docker is required but not installed${NC}"
    exit 1
fi

print_section "Building Docker image"
docker build -t sstring-builder .

print_section "Running multi-platform build"
docker run --rm \
    -v "$(pwd)/dist:/app/dist" \
    sstring-builder

print_section "Build Results"
echo -e "\nBuilt libraries:"
ls -l dist/libsstring_lib-*.a

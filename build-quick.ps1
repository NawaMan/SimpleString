# build-quick.ps1: Quick development build script for Simple String library using Docker
# Purpose:
#   - Fast build for development and testing
#   - Uses Docker for consistent build environment
#   - Supports Debug/Release builds
#   - Configurable test building

param(
    [switch]$Help,
    [switch]$WithTests = $true,
    [switch]$Clean,
    [ValidateSet('Debug', 'Release')]
    [string]$BuildType = 'Debug'
)

# Colors for output
$Colors = @{
    Red    = 'Red'
    Green  = 'Green'
    Yellow = 'Yellow'
    Blue   = 'Cyan'
}

# Print functions
function Write-Section {
    param([string]$Message)
    Write-Host "`n=== $Message ===" -ForegroundColor $Colors.Yellow
}

function Write-Status {
    param([string]$Message)
    Write-Host $Message -ForegroundColor $Colors.Blue
}

function Write-Success {
    param([string]$Message)
    Write-Host $Message -ForegroundColor $Colors.Green
}

function Write-Error {
    param([string]$Message)
    Write-Host $Message -ForegroundColor $Colors.Red
}

function Show-Help {
    Write-Host @"
Usage: .\build-quick.ps1 [options]

Options:
  -Help          Show this help message
  -WithTests     Build with tests (default: true)
  -Clean         Clean build directory before building
  -BuildType     Build type: Debug or Release (default: Debug)

Examples:
  .\build-quick.ps1                  # Debug build with tests
  .\build-quick.ps1 -WithTests:`$false  # Debug build without tests
  .\build-quick.ps1 -Clean          # Clean debug build with tests
  .\build-quick.ps1 -BuildType Release  # Release build with tests
"@
    exit 0
}

# Show help if requested
if ($Help) {
    Show-Help
}

# Check if Docker is installed
if (-not (Get-Command "docker" -ErrorAction SilentlyContinue)) {
    Write-Error "Error: Docker is required but not installed"
    exit 1
}

# Create build context
Write-Section "Creating Build Context"

# Create Dockerfile for quick build
$dockerfileContent = @"
FROM ubuntu:24.04

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libboost-all-dev \
    lcov \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

# Build script
COPY docker-quick-build.sh /build/
RUN chmod +x /build/docker-quick-build.sh
"@

# Create docker-quick-build.sh
$quickBuildScript = @"
#!/bin/bash
set -e

BUILD_TYPE=`$1
BUILD_TESTS=`$2

# Clean any existing build
rm -rf build

# Create fresh build directory
mkdir -p build
cd build

# Configure with absolute paths
cmake -DCMAKE_BUILD_TYPE=`$BUILD_TYPE \
      -DBUILD_TESTING=`$BUILD_TESTS \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      /build

# Build
make -j`$(nproc)

# Run tests if enabled
if [ "`${BUILD_TESTS}" = "ON" ]; then
    ctest --output-on-failure
fi

# Copy output files
mkdir -p /build/output
cp -r * /build/output/
"@

# Write files
Write-Status "Creating build files..."
Set-Content -Path "Dockerfile.quick" -Value $dockerfileContent
# Ensure the file has Unix line endings
$quickBuildScript = $quickBuildScript -replace "`r`n", "`n"
[System.IO.File]::WriteAllText("$PWD/docker-quick-build.sh", $quickBuildScript)

# Clean if requested
if ($Clean) {
    Write-Section "Cleaning Build Directory"
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
    }
}

# Create output directory
New-Item -ItemType Directory -Path "build" -Force | Out-Null

# Build Docker image
Write-Section "Building Docker Image"
Write-Status "Building development image..."
docker build -t sstring-dev -f Dockerfile.quick .

if ($LASTEXITCODE -ne 0) {
    Write-Error "Docker build failed"
    exit 1
}

# Run build in container
Write-Section "Building Project"
Write-Status "Running build in container..."
$buildTests = if ($WithTests) { "ON" } else { "OFF" }

docker run --rm `
    -v "${PWD}/build:/build/output" `
    sstring-dev `
    /build/docker-quick-build.sh $BuildType $buildTests

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

# Build summary
Write-Section "Build Summary"
Write-Success "Build completed successfully!"
Write-Host "Build Type: " -NoNewline
Write-Host $BuildType -ForegroundColor $Colors.Blue
Write-Host "Tests: " -NoNewline
Write-Host $buildTests -ForegroundColor $Colors.Blue

Write-Host "`nOutput files in ./build:"
Get-ChildItem "build" -Recurse -File |
    Where-Object { $_.Name -match '\.(a|dll|exe|json)$' } |
    ForEach-Object {
        Write-Host "  - " -NoNewline
        Write-Host $_.FullName.Replace("$PWD\", "") -ForegroundColor $Colors.Blue
    }

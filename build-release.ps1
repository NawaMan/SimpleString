# build-release.ps1: Release build script for Simple String library
# Builds release packages for multiple platforms using Docker

param(
    [switch]$Help,
    [ValidateSet('all', 'linux', 'windows', 'macos')]
    [string]$Platform = 'all',
    [ValidateSet('all', 'tar.gz', 'deb', 'rpm', 'zip', 'msi', 'pkg')]
    [string]$PackageType = 'all',
    [string]$Version,
    [switch]$ExcludeLlvmIr
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

function Write-Error {
    param([string]$Message)
    Write-Host $Message -ForegroundColor $Colors.Red
}

function Write-Success {
    param([string]$Message)
    Write-Host $Message -ForegroundColor $Colors.Green
}

function Show-Help {
    Write-Host "Usage: .\build-release.ps1 [options]`n"
    Write-Host "Options:"
    Write-Host "  -Help                 Show this help message"
    Write-Host "  -Platform PLATFORM    Build for specific platform (linux, windows, macos)"
    Write-Host "                        Default: all platforms"
    Write-Host "  -PackageType TYPE     Package type (tar.gz, deb, rpm, zip, msi, pkg)"
    Write-Host "                        Default: all types for selected platform"
    Write-Host "  -Version VERSION      Set package version (required)"
    Write-Host "  -ExcludeLlvmIr        Exclude LLVM IR files during build (requires Clang)`n"
}

# Show help if requested
if ($Help) {
    Show-Help
    exit 0
}

# Check if Docker is installed
if (-not (Get-Command "docker" -ErrorAction SilentlyContinue)) {
    Write-Error "Error: Docker is required but not installed"
    exit 1
}

# Set platforms based on input
$Platforms = if ($Platform -eq "all") {
    @("linux", "windows", "macos")
} else {
    @($Platform)
}

# Check version
if (-not $Version) {
    if (Test-Path "version.txt") {
        $Version = Get-Content "version.txt"
        Write-Status "Using version from version.txt: $Version"
    } else {
        Write-Error "Version is required. Use -Version or create version.txt"
        Show-Help
        exit 1
    }
}

# Create Docker build context
Write-Section "Creating Docker build context"

# Copy and prepare the docker-build.sh template
Write-Status "Copying and preparing docker-build.sh template..."
Copy-Item "config/templates/docker-build.sh" "docker-build.sh" -Force
# Ensure the file has Unix line endings (LF instead of CRLF)
$content = Get-Content "docker-build.sh" -Raw
$content = $content -replace "`r`n", "`n"
[System.IO.File]::WriteAllText("$PWD/docker-build.sh", $content)

# Copy the CMake templates from config directory
Write-Status "Copying CMake templates..."
New-Item -ItemType Directory -Path "cmake" -Force | Out-Null
Copy-Item "config/templates/cmake/mingw-w64-x86_64.cmake"          "cmake/mingw-w64-x86_64.cmake" -Force
Copy-Item "config/templates/cmake/windows-x86_64-msvc-cross.cmake" "cmake/windows-x86_64-msvc-cross.cmake" -Force

# Create dist directory
New-Item -ItemType Directory -Path "dist" -Force | Out-Null

# Build for each platform
foreach ($platform in $Platforms) {
    Write-Section "Building for $platform"
    
    # Build the platform-specific image
    Write-Status "Building Docker image for $platform..."
    docker build --target $platform `
                --build-arg "VERSION=$Version" `
                -f "config/Dockerfile" `
                -t "sstring-$platform-builder" .
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Docker build failed for $platform"
        exit 1
    }
    
    # Run the build
    Write-Status "Running build for $platform..."
    # Set LLVM IR generation flag (enabled by default unless excluded)
    $generateLlvmIr = if ($ExcludeLlvmIr) { "0" } else { "1" }
    
    # Use /bin/bash explicitly to run the script in the container
    docker run --rm `
        -v "${PWD}/dist:/build/dist" `
        "sstring-$platform-builder" /bin/bash /build/docker-build.sh $platform $Version $generateLlvmIr
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Docker run failed for $platform"
        exit 1
    }
}

Write-Section "Build Summary"
Write-Success "Release packages have been created in the dist/ directory:"
Get-ChildItem "dist" | Format-Table Name, Length, LastWriteTime

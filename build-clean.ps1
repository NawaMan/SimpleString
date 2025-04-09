# build-clean.ps1: Clean script for Simple String library
# Removes all build artifacts and generated files

# Colors for output
$Colors = @{
    Yellow = 'Yellow'
}

function Write-Section {
    param([string]$Message)
    Write-Host "`n=== $Message ===" -ForegroundColor $Colors.Yellow
}

# Remove build directories
Write-Section "Removing build directories"
Remove-Item -Path "build" -Recurse -Force -ErrorAction SilentlyContinue

# Remove package directories
Write-Section "Removing package directories"
Remove-Item -Path "dist" -Recurse -Force -ErrorAction SilentlyContinue

# Remove generated files
Write-Section "Removing generated files"
Remove-Item -Path "coverage_report" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "coverage.info" -Force -ErrorAction SilentlyContinue
Remove-Item -Path "docker-build.sh" -Force -ErrorAction SilentlyContinue
Remove-Item -Path "compile_commands.json" -Force -ErrorAction SilentlyContinue
Remove-Item -Path "cmake/mingw-w64-x86_64.cmake" -Force -ErrorAction SilentlyContinue
Remove-Item -Path "cmake/windows-x86_64-msvc-cross.cmake" -Force -ErrorAction SilentlyContinue

Write-Host "`nClean completed successfully!" -ForegroundColor Green

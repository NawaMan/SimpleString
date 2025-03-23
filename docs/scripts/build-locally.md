# build-locally.sh

## Purpose
`build-locally.sh` is a comprehensive build script designed for local development and testing of the SString library. It provides a complete development workflow including building, testing, coverage analysis, static analysis, and package generation.

## When to Use
Use this script when you want to:
1. Build and test the library locally
2. Run coverage analysis to check test coverage
3. Perform static analysis on the code
4. Generate distribution packages
5. Validate changes before committing

## Features
- Cleans build directory for fresh builds
- Builds the library and tests
- Runs all unit tests
- Generates coverage reports
- Performs static analysis using clang-tidy
- Creates distribution packages (TGZ, DEB, RPM, ZIP)

## Prerequisites
- CMake 3.10 or higher
- C++ compiler (GCC or Clang)
- Google Test framework
- lcov (for coverage reports)
- clang-tidy (for static analysis)
- CPack tools (for package generation)

## Usage
```bash
./build-locally.sh
```

## Output
The script produces:
1. Built library in `dist/libsstring_lib.a`
2. Test executable in `build/sstring_tests`
3. Coverage report in `coverage_report/index.html`
4. Distribution packages in `dist/`:
   - `SString-<version>-Linux.tar.gz`
   - `SString-<version>-Linux.deb`
   - `SString-<version>-Linux.rpm`
   - `SString-<version>-Linux.zip`

## Build Process
1. **Clean Build**
   - Removes previous build artifacts
   - Ensures clean state for new build

2. **Build Project**
   - Configures CMake
   - Builds library and tests
   - Links against dependencies

3. **Run Tests**
   - Executes all unit tests
   - Reports test results

4. **Coverage Analysis**
   - Generates coverage data
   - Creates HTML coverage report
   - Shows lines, functions, and branch coverage

5. **Static Analysis**
   - Runs clang-tidy
   - Checks for potential issues
   - Reports warnings and errors

6. **Package Generation**
   - Creates distribution packages
   - Supports multiple formats
   - Ready for distribution

## Error Handling
- Fails fast on any build error
- Reports clear error messages
- Shows compilation warnings
- Indicates test failures

## Example Output
```
=== Building project ===
[Build output...]

=== Running tests ===
100% tests passed, 0 tests failed

=== Generating coverage report ===
Overall coverage rate:
  lines......: 100.0%
  functions..: 100.0%
  branches...: 55.6%

=== Generated files in dist directory ===
SString-1.0.0-Linux.deb
SString-1.0.0-Linux.rpm
SString-1.0.0-Linux.tar.gz
SString-1.0.0-Linux.zip
```

## See Also
- [build-all-platforms.sh](build-all-platforms.md) - For multi-platform builds
- [CMakeLists.txt](../cmake/CMakeLists.md) - Build system configuration

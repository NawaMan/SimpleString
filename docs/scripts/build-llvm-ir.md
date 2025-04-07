# LLVM IR Build Script

## Overview
The `build-llvm-ir.sh` script generates LLVM Intermediate Representation (IR) files for all source files in the SimpleString library. This is useful for advanced static analysis, optimization studies, and cross-platform compatibility research.

## Requirements
- Clang compiler (clang++) must be installed
- CMake 3.10 or later

## Usage

```bash
./build-llvm-ir.sh [options]
```

### Options
- `-h, --help`: Show help message
- `-c, --clean`: Clean build directory before building
- `-o, --opt-level N`: Set optimization level (0-3, default: 0)
- `-d, --debug`: Build with debug information (default)
- `-r, --release`: Build with release optimization

### Examples

```bash
# Debug build with no optimization
./build-llvm-ir.sh

# Release build with -O2 optimization
./build-llvm-ir.sh -r -o 2

# Clean and rebuild
./build-llvm-ir.sh -c
```

## Output
The script generates `.ll` files (LLVM IR text format) for each source file in the library. The output files are placed in the `dist/llvm-ir/` directory, organized by build type and optimization level.

### Output Directory Structure
```
dist/llvm-ir/
  ├── debug-O0/      # Debug build with no optimization
  ├── debug-O1/      # Debug build with -O1 optimization
  ├── release-O2/    # Release build with -O2 optimization
  └── release-O3/    # Release build with -O3 optimization
```

## Use Cases

1. **Static Analysis**: Examine the LLVM IR to identify potential optimizations or bugs
2. **Optimization Studies**: Compare different optimization levels and their effects
3. **Cross-Platform Analysis**: Study platform-independent representation of the code
4. **Educational Purposes**: Learn about compiler internals and code generation

## Notes
- LLVM IR files can be large, especially at lower optimization levels
- This feature is primarily intended for library developers and contributors
- The IR generation process is separate from the main build process and does not affect normal library builds

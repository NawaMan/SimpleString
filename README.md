# Simple String Library

A modern C++ string library with robust Unicode support and efficient memory management. Simple String provides a safe and performant way to handle text in C++, with full UTF-8 and UTF-16 support and comprehensive Unicode character handling.

## Features
- Full Unicode support (UTF-8/UTF-16)
- Efficient memory management with copy-on-write
- Comprehensive character access and manipulation
- Robust handling of invalid UTF-8 sequences
- Advanced Unicode whitespace handling in string trimming operations
- Unicode character categorization and code point utilities
- Cross-platform compatibility
- Extensive test coverage

## Requirements

### Build System
- CMake 3.10 or later
- C++20 compatible compiler (e.g., GCC 10+, Clang 11+, MSVC 19.29+)

### Required Dependencies
- Boost 1.71 or later
  - boost-locale component required

### Optional Dependencies
- Google Test 1.11 or later (for testing)

### Ubuntu 24.04 Installation
```bash
# Install build tools
sudo apt-get update
sudo apt-get install -y cmake g++

# Install required dependencies
sudo apt-get install -y libboost-dev libboost-locale-dev

# Install optional dependencies (for testing)
sudo apt-get install -y libgtest-dev
```

## Installation

### From Source
```bash
# Clone the repository
git clone https://github.com/nawaman/SString.git
cd SString

# Build and test locally
./build-locally.sh
```

### Using Package Manager
Download the appropriate package for your system from the releases:
- DEB package (Debian/Ubuntu)
- RPM package (Red Hat/Fedora)
- ZIP archive (Windows)
- TGZ archive (Unix-like systems)

## Usage
```cpp
#include "string.hpp"

using namespace simple;

// Create strings
String str1{"Hello, 世界!"};
String str2{"🌟 Stars"};  // Emoji support

// Access characters
auto ch = str1.char_at(7);  // Gets '世'

// Unicode whitespace handling
String text = "  Hello, world!  ";
String stripped = text.strip();  // Removes all Unicode whitespace

// Safe handling of invalid UTF-8
String invalid{"\xFF\xFE"};  // Invalid UTF-8 bytes
// Automatically replaced with U+FFFD (replacement character)
```

## Building

### Local Development
```bash
# Build, test, and generate packages
./build-locally.sh

# Clean build artifacts
./build-clean.sh
```

### Multi-Platform Build
```bash
# Build for all supported platforms
./build-all-platforms.sh
```

## Testing
The library comes with comprehensive test suites:
```bash
# Run all tests
./build-locally.sh

# View coverage report
open coverage_report/index.html
```

## Documentation
- [API Documentation](docs/api/README.md)
- [Build Scripts](docs/scripts/build-locally.md)
- [Task Documentation](docs/tasks/)

## Contributing
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Developer on Linux

### Prerequisites
- Linux
- CMake
- Git
- sed
- Boost

```bash
sudo apt-get install -y build-essential cmake git sed libgtest-dev libboost-all-dev
```

### Setup
1. Clone the repository
2. Build the Docker image
3. Run the Docker container

### Ensure proper line ends before commit

Run : find . \( -path "./.git" -o -name "*.ps1" \) -prune -o -type f -exec sed -i 's/\r/\n/g' {} +

## Developing on Windows

### Prerequisites
- Windows 10 or later
- WSL (Windows Subsystem for Linux)
- Docker
- CMake
- Git
- sed
- Boost

### Setup
1. Install WSL
2. Install Docker
3. Clone the repository
4. Build the Docker image
5. Run the Docker container

### Ensure proper line ends before commit

Run : find . -path "./.git" -prune -o -type f ! -name "*.ps1" -exec sed -i 's/\r$//' {} +

## Developer on MacOS

### Prerequisites
- Windows 10 or later
- WSL (Windows Subsystem for Linux)
- Docker
- CMake
- Git
- sed
- Boost
- Clang

### Ensure proper line ends before commit

Run : find . -path "./.git" -prune -o -type f ! -name "*.ps1" -exec sed -i 's/\r$//' {} +

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author
[NawaMan](https://github.com/nawaman) - nawa@nawaman.net

The real reason to write this library is for Java developers like myself to use Java-style string in C++.
# Unicode Boost Example

This project demonstrates how to use Boost's Unicode handling capabilities in C++. It shows various Unicode operations including:
- Case conversion
- Text normalization
- Character properties
- Word boundary analysis
- UTF-8 string handling

## Requirements
- C++20 (using Clang 18.1.3)
- CMake 3.10 or later
- Boost library with locale component

## Building the Project
```bash
mkdir build
cd build
cmake ..
make
```

## Running
After building, run the executable:
```bash
./unicode_example
```

The program will demonstrate various Unicode operations with multilingual text including English, Chinese, Russian, and emoji characters.

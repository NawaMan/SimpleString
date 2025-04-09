# Changelog

## [1.0.0] - 2025-04-08

Code improvements and documentation updates.
- Improved Unicode whitespace handling in string trimming methods (trim(), strip(), stripLeading(), stripTrailing(), isStripped())
- Refactored String class by moving method implementations from header to cpp file for better compilation efficiency
- Enhanced Unicode support with improved UnicodeCategory implementation
- Added comprehensive documentation for Unicode-related classes
- Improved code organization and reduced header dependencies

## [0.4.0] - 2025-03-27

Rename many things.
- The name of the library is changed from SString to Simple String. (project artifacts like lib name still sstring).
- SString class is now String.
- simple_string namespace is now simple.

## [0.3.0] - 2025-03-24

Foundational functionalities and build infrastructure is completed and tidy.

## [0.2.0] - 2025-03-24

Add the check to ensure that changelog entry exists for the release -- Failed.


## [0.1.0] - 2025-03-24

Initial release of the SString library, providing a Java String-like implementation for C++.

### Features

- **UTF-16 Based String Implementation**
  - Implements Java-like string behavior with UTF-16 code unit indexing
  - Efficient UTF-8 to UTF-16 conversion with caching mechanism
  - Proper handling of surrogate pairs for characters outside BMP

- **String Operations**
  - `length()`: Get string length in UTF-16 code units
  - `is_empty()`: Check if string is empty
  - `char_at(index)`: Get character at specific index
  - `char_value(index)`: Get raw UTF-16 code unit at index
  - Array-style character access with `operator[]`

- **String Comparison**
  - Byte-by-byte comparison for exact Java-like behavior
  - Support for equality testing (`equals`)
  - Lexicographical comparison (`compare_to`)
  - Standard comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)

- **Unicode Support**
  - Full UTF-8 input support
  - Proper handling of invalid UTF-8 sequences
  - Replacement of invalid sequences with U+FFFD
  - Detection and handling of overlong encodings
  - Support for the full Unicode range (U+0000 to U+10FFFF)

- **Memory Efficiency**
  - Immutable string implementation
  - String data sharing between copies
  - Lazy UTF-16 conversion with caching
  - Thread-safe cache implementation

- **Error Handling**
  - `StringIndexOutOfBoundsException` for invalid index access
  - Proper bounds checking for all string operations

### Technical Details

- Built with modern C++ features
- Uses Boost.Locale for UTF-8 handling
- Header-only library with minimal dependencies
- Thread-safe implementation
- Comprehensive error handling

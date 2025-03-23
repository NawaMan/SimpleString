# SString API Documentation

## Overview
SString is a Java-style string class for C++ that provides robust Unicode support and efficient memory management through copy-on-write optimization.

## Core Classes

### `SString`
Main string class that provides Java String-like functionality.

#### Constructors
```cpp
explicit SString(const std::string& str);
SString(const char* str, std::size_t length);
```

#### Core Methods
- `std::size_t length() const`: Returns string length in UTF-16 code units
- `bool is_empty() const`: Checks if string is empty
- `Char char_at(std::size_t index) const`: Gets character at index
- `char16_t char_value(std::size_t index) const`: Gets raw UTF-16 code unit at index

#### Comparison Methods
- `bool equals(const SString& other) const`: Exact string equality
- `CompareResult compare_to(const SString& other) const`: Lexicographic comparison
- Operators: `==`, `!=`, `<`, `<=`, `>`, `>=`

#### String Operations
- `const std::string& to_string() const`: Get underlying UTF-8 string
- `operator[](std::size_t index)`: Array-style character access

### `Char`
Represents a UTF-16 code unit, similar to Java's Character.

#### Constants
```cpp
static const char16_t MIN_VALUE = 0;
static const char16_t MAX_VALUE = 0xFFFF;
static const char16_t REPLACEMENT_CHAR = 0xFFFD;
```

#### Methods
- `char16_t value() const`: Get raw UTF-16 code unit value
- `bool is_surrogate() const`: Check if code unit is a surrogate
- `bool is_high_surrogate() const`: Check if code unit is high surrogate
- `bool is_low_surrogate() const`: Check if code unit is low surrogate

### `CompareResult`
Wrapper for string comparison results.

#### Constants
```cpp
static const CompareResult LESS;
static const CompareResult EQUAL;
static const CompareResult GREATER;
```

#### Methods
- `bool is_less() const`: Check if comparison result is "less than"
- `bool is_equal() const`: Check if comparison result is "equal"
- `bool is_greater() const`: Check if comparison result is "greater than"
- `bool is_less_or_equal() const`: Check if result is "less than or equal"
- `bool is_greater_or_equal() const`: Check if result is "greater than or equal"

## Exception Classes

### `StringIndexOutOfBoundsException`
Thrown when attempting to access a character at an invalid index.

```cpp
class StringIndexOutOfBoundsException : public std::out_of_range;
```

## Unicode Support
- Full UTF-8 and UTF-16 support
- Proper handling of surrogate pairs
- Automatic replacement of invalid UTF-8 sequences with U+FFFD
- Byte-order-mark (BOM) handling
- NFC normalization support via Boost.Locale

## Memory Management
- Copy-on-write optimization using `std::shared_ptr`
- Lazy UTF-16 conversion with caching
- Thread-safe implementation of string sharing

## Example Usage
```cpp
#include "sstring.hpp"
using namespace simple_string;

// Create strings
SString str1{"Hello, 世界!"};  // UTF-8 literal
SString str2{"🌟 Stars"};      // Emoji support

// Access characters
Char ch = str1.char_at(7);     // Gets '世'
char16_t raw = ch.value();     // Get raw UTF-16 code unit

// Compare strings
if (str1.equals(str2)) {
    // Strings are identical
}

// Lexicographic comparison
CompareResult result = str1.compare_to(str2);
if (result.is_less()) {
    // str1 comes before str2
}
```

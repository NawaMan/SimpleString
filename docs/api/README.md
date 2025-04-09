# Simple String API Documentation

## Overview
String is a Java-style string class for C++ that provides robust Unicode support and efficient memory management through copy-on-write optimization. The library offers comprehensive Unicode handling with both UTF-8 and UTF-16 support.

## Core Classes

### `String`
Main string class that provides Java String-like functionality.

#### Constructors
```cpp
explicit String(const std::string& str);
String(const char* str, std::size_t length);
```

#### Core Methods
- `std::size_t length() const`: Returns string length in UTF-16 code units
- `bool is_empty() const`: Checks if string is empty
- `Char char_at(std::size_t index) const`: Gets character at index
- `char16_t char_value(std::size_t index) const`: Gets raw UTF-16 code unit at index

#### Comparison Methods
- `bool equals(const String& other) const`: Exact string equality
- `CompareResult compare_to(const String& other) const`: Lexicographic comparison
- Operators: `==`, `!=`, `<`, `<=`, `>`, `>=`

#### String Operations
- `const std::string& to_string() const`: Get underlying UTF-8 string
- `operator[](std::size_t index)`: Array-style character access
- `String substring(std::size_t beginIndex) const`: Extract substring from beginIndex to end
- `String substring(std::size_t beginIndex, std::size_t endIndex) const`: Extract substring from beginIndex to endIndex-1

#### String Trimming Methods
- `String trim() const`: Removes ASCII whitespace from both ends of the string
- `String strip() const`: Removes Unicode whitespace from both ends of the string
- `String stripLeading() const`: Removes Unicode whitespace from the beginning of the string
- `String stripTrailing() const`: Removes Unicode whitespace from the end of the string
- `bool isStripped() const`: Returns true if the string has no leading or trailing Unicode whitespace

These methods properly handle all Unicode whitespace characters, including zero-width spaces and other special whitespace characters.

#### Static valueOf Methods
- `static String valueOf(bool b)`: Returns a String representation of the boolean argument
- `static String valueOf(char c)`: Returns a String representation of the char argument
- `static String valueOf(int i)`: Returns a String representation of the int argument
- `static String valueOf(long l)`: Returns a String representation of the long argument
- `static String valueOf(float f)`: Returns a String representation of the float argument
- `static String valueOf(double d)`: Returns a String representation of the double argument
- `template<typename T> static String valueOf(const T& obj)`: Generic method to convert any type to a String

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
- NFC normalization support
- Comprehensive Unicode character categorization via the UnicodeCategory class
- Efficient code point handling with UnicodeUtil
- Regular expression support with Unicode properties

### `RegEx`
Provides regular expression pattern matching and manipulation with Unicode support.

#### Constructors
```cpp
explicit RegEx(const String& pattern);
RegEx(const String& pattern, const Flags& flags);
RegEx(const String& pattern, const Flag& flag); // Implicit conversion from Flag to Flags
```

#### Flag and Flags
```cpp
// Individual flags
static const Flag CASE_INSENSITIVE;  // Case-insensitive matching
static const Flag MULTILINE;         // Multiline mode (^ and $ match at line breaks)
static const Flag DOTALL;            // Dot matches all characters including newlines
static const Flag EXTENDED;          // Extended syntax with whitespace ignored
static const Flag ECMAScript;        // ECMAScript (JavaScript) syntax

// Combining flags
Flags flags = Flags::of(Flag::CASE_INSENSITIVE, Flag::MULTILINE);
Flags flags = Flag::CASE_INSENSITIVE | Flag::MULTILINE;

// Checking flags
bool hasFlag = flags.contain(Flag::CASE_INSENSITIVE);
```

#### Methods
- `bool matches(const String& input) const`: Tests if the pattern matches the entire input
- `bool find(const String& input) const`: Tests if the pattern appears anywhere in the input
- `String replaceAll(const String& input, const String& replacement) const`: Replaces all matches
- `String replaceFirst(const String& input, const String& replacement) const`: Replaces first match
- `std::vector<String> split(const String& input) const`: Splits input around matches
- `std::vector<String> split(const String& input, int limit) const`: Splits with limit control

## Memory Management
- Copy-on-write optimization using `std::shared_ptr`
- Lazy UTF-16 conversion with caching
- Thread-safe implementation of string sharing

## Example Usage
```cpp
#include "string.hpp"
using namespace simple;

// Create strings
String str1{"Hello, 世界!"};  // UTF-8 literal
String str2{"🌟 Stars"};      // Emoji support

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

// String trimming
String text = "  Hello, world!  ";
String trimmed = text.trim();      // Removes ASCII whitespace: "Hello, world!"
String stripped = text.strip();    // Removes Unicode whitespace: "Hello, world!"

// Remove only leading or trailing whitespace
String noLeading = text.stripLeading();    // "Hello, world!  "
String noTrailing = text.stripTrailing();  // "  Hello, world!"

// Check if a string has no leading/trailing whitespace
if (trimmed.isStripped()) {
    // String has no leading or trailing whitespace
}
```

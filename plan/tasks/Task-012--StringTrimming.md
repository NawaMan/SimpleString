# Task-012: String Trimming Methods

## Motivation

Trimming whitespace from strings is a common operation in string manipulation. Java's String class provides methods for removing leading and/or trailing whitespace, which are useful for cleaning up user input, parsing data, and many other text processing tasks. Adding these methods to our SimpleString library will enhance its functionality and make it more compatible with Java's String API.

## Description

Implement the following trimming methods for the String class:

1. `trim()`: Removes all leading and trailing whitespace from the string (Java's traditional method)
2. `strip()`: Modern equivalent of trim() that handles Unicode whitespace better (Java 11+)
3. `stripLeading()`: Removes all leading whitespace from the string (Java 11+)
4. `stripTrailing()`: Removes all trailing whitespace from the string (Java 11+)

All methods should:
- Return a new String with the whitespace removed
- Preserve the original String (immutability)
- Handle Unicode whitespace characters correctly
- Share the underlying string data when possible (using the existing string sharing mechanism)
- Follow Java's behavior for whitespace identification

## Acceptance Criteria

- [x] Implement `trim()` method that removes both leading and trailing whitespace (ASCII whitespace only, for Java compatibility)
- [x] Implement `strip()` method that removes both leading and trailing Unicode whitespace
- [x] Implement `stripLeading()` method that removes only leading Unicode whitespace
- [x] Implement `stripTrailing()` method that removes only trailing Unicode whitespace
- [x] Add comprehensive unit tests for all methods:
  - [x] Basic whitespace trimming (spaces, tabs, newlines)
  - [x] Unicode whitespace characters (verify exact same behavior as Java)
  - [x] Empty strings
  - [x] Strings with only whitespace
  - [x] Strings with no whitespace
  - [x] Strings with whitespace only at the beginning
  - [x] Strings with whitespace only at the end
  - [x] Strings with whitespace in the middle (should be preserved)
  - [x] Strings with embedded null characters
  - [x] Method chaining (e.g., stripLeading().stripTrailing())
  - [x] Verify string sharing is maintained when appropriate
- [x] Ensure all tests pass
- [x] Update documentation to include the new methods

## Implementation Details

1. Method Signatures:
   ```cpp
   // In string.hpp
   /**
    * Returns a string with all leading and trailing ASCII whitespace removed.
    * This method is equivalent to Java's String.trim() method.
    * 
    * @return A new string with leading and trailing ASCII whitespace removed
    */
   String trim() const;

   /**
    * Returns a string with all leading and trailing Unicode whitespace removed.
    * This method is equivalent to Java 11's String.strip() method.
    * 
    * @return A new string with leading and trailing Unicode whitespace removed
    */
   String strip() const;

   /**
    * Returns a string with all leading Unicode whitespace removed.
    * This method is equivalent to Java 11's String.stripLeading() method.
    * 
    * @return A new string with leading Unicode whitespace removed
    */
   String stripLeading() const;

   /**
    * Returns a string with all trailing Unicode whitespace removed.
    * This method is equivalent to Java 11's String.stripTrailing() method.
    * 
    * @return A new string with trailing Unicode whitespace removed
    */
   String stripTrailing() const;

   /**
    * Returns true if this string has no leading or trailing whitespace.
    * Useful for optimization and testing.
    *
    * @return true if the string has no leading or trailing whitespace
    */
   bool isStripped() const;
   ```

2. Whitespace Definition:
   - For `trim()`: Use ASCII whitespace only (space, tab, newline, carriage return, form feed, vertical tab)
     - This matches Java's traditional trim() behavior which only considers codepoints <= '\u0020' (space)
   - For `strip()`, `stripLeading()`, and `stripTrailing()`:
     - Use Unicode's definition of whitespace via Character.isWhitespace()
     - This includes all characters with Unicode general categories: Zs (space separator), Zl (line separator), Zp (paragraph separator)
     - Also includes control characters like tab, newline, carriage return, form feed
     - Use Boost.Locale's is_space() function which should match Java's behavior
   - Document exact whitespace definition in code comments for clarity

3. Implementation Approach:
   - For `trim()`: Use simple ASCII comparison (codepoint <= 0x20)
   - For Unicode-aware methods: Use Boost.Locale for Unicode-aware whitespace detection
   - Leverage existing String class mechanisms for substring operations
   - Ensure proper handling of UTF-8 encoding
   - Maintain immutability by returning new String instances
   - Utilize string sharing for efficiency by returning substring views when possible
   - Implement trimming directly on UTF-8 data to avoid unnecessary conversions
   - Ensure the UTF-16 cache is properly maintained/invalidated when creating new trimmed strings

4. Edge Cases to Handle:
   - Empty strings (should return empty string)
   - Strings with only whitespace (should return empty string)
   - Strings with no whitespace (should return the original string, with the same data reference)
   - Strings with embedded null characters (should handle correctly, not treating null as a terminator)
   - Unicode edge cases (combining characters, surrogate pairs)
   - Strings with mixed ASCII and Unicode whitespace (should behave differently for trim() vs strip())
   - Method chaining (e.g., stripLeading().stripTrailing() should work correctly and efficiently)

5. Performance Considerations:
   - Avoid unnecessary string copies
   - Use efficient algorithms for finding whitespace boundaries
   - Leverage string sharing for memory efficiency
   - Ensure trimmed strings properly share the underlying data with the original string
   - For strings that are already trimmed, return the original string instance (same reference)
   - Consider caching UTF-16 representation when beneficial
   - Optimize the isStripped() method for quick checking
   - When chaining methods, avoid creating intermediate String objects when possible

## Expected Benefits

1. Enhanced API:
   - More complete String API that matches Java's functionality
   - Support for both traditional (trim) and modern (strip) Java String methods
   - Convenient methods for common string operations

2. Robust Text Processing:
   - Proper handling of Unicode whitespace
   - Consistent behavior with Java's String class
   - Clear distinction between ASCII-only trim() and Unicode-aware strip() methods

3. Maintainability:
   - Clear, well-documented implementation
   - Comprehensive test coverage
   - Consistent with existing codebase style and patterns
   - Explicit documentation of whitespace definitions

4. Performance:
   - Efficient implementation that avoids unnecessary copies
   - Leverages existing string sharing mechanism
   - Optimized for method chaining
   - Fast path for already-trimmed strings

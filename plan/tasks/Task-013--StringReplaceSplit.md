# Task-013: String Replacement and Splitting Methods

## Motivation

Replacement and splitting operations are fundamental string manipulation capabilities in modern programming. Java's String class provides comprehensive methods for replacing characters or substrings and splitting strings by delimiters. Adding these methods to our SimpleString library will significantly enhance its functionality and make it more compatible with Java's String API, while providing essential tools for text processing.

## Description

Implement the following string manipulation methods for the String class:

1. **Replacement Methods**:
   - replace(char oldChar, char newChar): Replaces all occurrences of a specified character with another character
   - replace(const String& target, const String& replacement): Replaces all occurrences of a target substring with a replacement string
   - replaceAll(const String& regex, const String& replacement): Replaces all substrings matching the given regular expression with the replacement string
   - replaceFirst(const String& regex, const String& replacement): Replaces the first substring matching the given regular expression with the replacement string

2. **Splitting Methods**:
   - split(const String& delimiter): Splits the string around matches of the given delimiter string
   - split(const String& regex, int limit): Splits the string around matches of the given regular expression, with a limit on the number of splits

3. **Regular Expression Support**:
   - Integrate Boost.Regex for pattern matching and replacement functionality
   - Ensure proper handling of Unicode characters in regular expressions

All methods should:
- Return appropriate results based on Java's behavior
- Preserve the original String (immutability)
- Handle Unicode characters correctly
- Share the underlying string data when possible (using the existing string sharing mechanism)

## Acceptance Criteria

- [ ] Implement character replacement method:
  - [ ] replace(char oldChar, char newChar)
- [ ] Implement substring replacement method:
  - [ ] replace(const String& target, const String& replacement)
- [ ] Implement regex-based replacement methods:
  - [ ] replaceAll(const String& regex, const String& replacement)
  - [ ] replaceFirst(const String& regex, const String& replacement)
- [ ] Implement string splitting methods:
  - [ ] split(const String& delimiter)
  - [ ] split(const String& regex, int limit)
- [ ] Add comprehensive unit tests for all methods:
  - [ ] Basic character and substring replacement
  - [ ] Regular expression replacement with various patterns
  - [ ] Edge cases (empty strings, no matches, etc.)
  - [ ] Unicode character handling
  - [ ] Splitting with different delimiters
  - [ ] Splitting with regex patterns
  - [ ] Limit parameter behavior in split operations
  - [ ] Verify string sharing is maintained when appropriate
- [ ] Ensure all tests pass
- [ ] Update documentation to include the new methods

## Implementation Details

1. Method Signatures:
   

cpp
   // In string.hpp
   /**
    * Returns a new string resulting from replacing all occurrences of oldChar in this string with newChar.
    * @param oldChar the old character
    * @param newChar the new character
    * @return a new string with all occurrences of oldChar replaced by newChar
    */
   String replace(Char oldChar, Char newChar) const;

   /**
    * Returns a new string resulting from replacing all occurrences of target in this string with replacement.
    * @param target the sequence of characters to be replaced
    * @param replacement the replacement sequence of characters
    * @return a new string with all occurrences of target replaced by replacement
    */
   String replace(const String& target, const String& replacement) const;

   /**
    * Replaces each substring of this string that matches the given regular expression with the given replacement.
    * @param regex the regular expression to which this string is to be matched
    * @param replacement the string to be substituted for each match
    * @return a new string with all matches of the regular expression replaced by the replacement string
    */
   String replaceAll(const String& regex, const String& replacement) const;

   /**
    * Replaces the first substring of this string that matches the given regular expression with the given replacement.
    * @param regex the regular expression to which this string is to be matched
    * @param replacement the string to be substituted for the first match
    * @return a new string with the first match of the regular expression replaced by the replacement string
    */
   String replaceFirst(const String& regex, const String& replacement) const;

   /**
    * Splits this string around matches of the given delimiter.
    * @param delimiter the delimiting string
    * @return an array of strings computed by splitting this string around matches of the given delimiter
    */
   std::vector<String> split(const String& delimiter) const;

   /**
    * Splits this string around matches of the given regular expression.
    * @param regex the delimiting regular expression
    * @param limit the result threshold (if positive, limits the number of splits; if zero, no limit; if negative, trailing empty strings are not included)
    * @return an array of strings computed by splitting this string around matches of the given regular expression
    */
   std::vector<String> split(const String& regex, int limit) const;



2. Dependencies:
   - Boost.Regex for regular expression support
   - Existing String implementation for character handling and string sharing

3. Implementation Strategy:
   - Implement character replacement first (simplest case)
   - Then implement substring replacement without regex
   - Add Boost.Regex integration for regex-based methods
   - Implement splitting methods, reusing regex functionality where appropriate
   - Ensure proper handling of edge cases and Unicode characters
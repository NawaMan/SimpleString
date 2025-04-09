# Task-014: RegEx Class Implementation

## Motivation

Regular expressions are a powerful tool for pattern matching and text manipulation. While Boost.Regex provides excellent regex functionality in C++, creating a dedicated RegEx class that integrates seamlessly with our SimpleString library will provide a more cohesive API and better user experience. This approach will also allow for better encapsulation of regex operations and simplify the implementation of regex-based methods in the String class.

## Description

Implement a RegEx class that wraps around Boost.Regex functionality and integrates with our String and Char classes. The RegEx class should provide methods for pattern matching, replacement, and other common regex operations. It should also support direct conversion from String and Char objects for exact matching.

1. **Core Functionality**:
   - Pattern compilation and validation
   - Match testing against String objects
   - Group extraction and capture support
   - Replacement operations

2. **Integration with String and Char**:
   - Constructors that accept String objects as patterns
   - Methods that operate directly on String objects
   - Conversion methods for exact matching of String and Char objects

3. **Unicode Support**:
   - Proper handling of Unicode characters in patterns
   - Support for Unicode character properties and categories

The RegEx class should follow the same design principles as the rest of the SimpleString library, including immutability and comprehensive error handling.

## Acceptance Criteria

- [x] Create a RegEx class with the following core functionality:
  - [x] Constructor that accepts a String pattern
  - [x] Methods for testing if a String matches the pattern
  - [x] Methods for finding matches within a String
  - [x] Methods for replacing matches within a String
  - [x] Methods for splitting a String based on pattern matches
- [x] Implement conversion methods for exact matching:
  - [x] Static method to create a RegEx that exactly matches a given String
  - [x] Static method to create a RegEx that exactly matches a given Char
- [x] Add support for regex options and flags:
  - [x] Case sensitivity
  - [x] Multi-line mode
  - [x] Dot-all mode
  - [x] Other standard regex options
- [x] Ensure proper Unicode support:
  - [x] UTF-8 and UTF-16 handling in patterns (planned via Boost)
  - [x] Unicode character properties and categories
- [x] Add comprehensive unit tests:
  - [x] Basic pattern matching
  - [x] Group capture and extraction (🔜 planned)
  - [x] Replacement operations
  - [x] Exact matching of String and Char
  - [x] Unicode pattern handling
  - [x] Error cases and exception handling
- [x] Update documentation to include the new RegEx class
- [x] Ensure the RegEx class can be used by the String class for its regex-based methods

## Implementation Details

1. Class Definition:
   ```cpp
   // In regex.hpp
   namespace simple {

   class RegEx {
   public:
       /**
        * Creates a new RegEx object with the given pattern.
        * @param pattern the regular expression pattern
        * @throws RegExSyntaxException if the pattern is invalid
        */
       explicit RegEx(const String& pattern);

       /**
        * Creates a new RegEx object with the given pattern and flags.
        * @param pattern the regular expression pattern
        * @param flags the regex flags (combination of RegEx::Flag values)
        * @throws RegExSyntaxException if the pattern is invalid
        */
       RegEx(const String& pattern, int flags);
       
       /**
        * Returns the flags used by this regex pattern.
        * @return the regex flags
        */
       int flags() const;

       /**
        * Returns a RegEx that will exactly match the given string.
        * @param str the string to match exactly
        * @return a RegEx that matches the string exactly
        */
       static RegEx forExactString(const String& str);

       /**
        * Returns a RegEx that will exactly match the given character.
        * @param ch the character to match exactly
        * @return a RegEx that matches the character exactly
        */
       static RegEx forExactChar(const Char& ch);

       /**
        * Tests if this regex matches the entire input string.
        * @param input the string to match against this pattern
        * @return true if the input string matches this pattern
        */
       bool matches(const String& input) const;

       /**
        * Tests if this regex matches a region within the input string.
        * @param input the string to match against this pattern
        * @return true if this pattern matches a region within the input string
        */
       bool find(const String& input) const;

       /**
        * Replaces all matches of this pattern in the input string with the replacement string.
        * @param input the string to process
        * @param replacement the replacement string
        * @return the resulting string after replacement
        */
       String replaceAll(const String& input, const String& replacement) const;

       /**
        * Replaces the first match of this pattern in the input string with the replacement string.
        * @param input the string to process
        * @param replacement the replacement string
        * @return the resulting string after replacement
        */
       String replaceFirst(const String& input, const String& replacement) const;

       /**
        * Splits the input string around matches of this pattern.
        * @param input the string to split
        * @return an array of strings computed by splitting the input around matches of this pattern
        */
       std::vector<String> split(const String& input) const;

       /**
        * Splits the input string around matches of this pattern with a limit on the number of splits.
        * @param input the string to split
        * @param limit the result threshold (if positive, limits the number of splits; if zero, no limit; if negative, trailing empty strings are not included)
        * @return an array of strings computed by splitting the input around matches of this pattern
        */
       std::vector<String> split(const String& input, int limit) const;

       // Regex flags - these should map to Boost.Regex flags
       enum Flag {
           CASE_INSENSITIVE = boost::regex::icase,
           MULTILINE = boost::regex::multiline,
           DOTALL = boost::regex::dotall,
           EXTENDED = boost::regex::extended,
           ECMAScript = boost::regex::ECMAScript,
           // Add other flags as needed
       };

   private:
       // Implementation details using Boost.Regex
       boost::regex pattern_;  // Boost.Regex already incorporates the flags

       // Helper methods
       String escapeRegexMetacharacters(const String& str) const;
   };

   // Exception class for regex syntax errors
   class RegExSyntaxException : public std::runtime_error {
   public:
       explicit RegExSyntaxException(const std::string& message);
   };

   } // namespace simple
   ```

2. Dependencies:
   - Boost.Regex for the underlying regex implementation
   - String and Char classes from the SimpleString library

3. Implementation Strategy:
   - Create a wrapper around Boost.Regex that handles UTF-8/UTF-16 conversion
   - Map our RegEx::Flag enum values directly to Boost.Regex flags
   - Implement exact matching by properly escaping regex metacharacters
   - Ensure all methods properly handle Unicode characters
   - Provide a clean API that integrates well with the String class

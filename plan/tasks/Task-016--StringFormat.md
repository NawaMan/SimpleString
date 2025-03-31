# Task-016: String Formatting (format)

## Motivation

Formatting strings with placeholders and variable substitution is a common requirement in many applications. Java's `String.format()` method provides a powerful way to create formatted strings using format specifiers. Implementing similar functionality in the SimpleString library will enhance its utility and make it more familiar to Java developers working in C++.

## Description

Implement string formatting capabilities similar to Java's `String.format()` method using Boost.Format as the underlying implementation. This will allow users to create formatted strings using format specifiers and provide arguments to be substituted into those specifiers. The implementation should support a wide range of format specifiers for different data types and formatting options while leveraging Boost's mature and well-tested formatting capabilities.

1. **Core Functionality**:
   - Static `String::format(const String& format, ...)` method that accepts a format string and variable arguments
   - Support for format specifiers similar to Java's `String.format()` and C's `printf()`
   - Type-safe formatting with appropriate error handling for mismatched types
   - Leverage Boost.Format for the underlying implementation

2. **Format Specifiers**:
   - General: `%s` for strings, `%d` for integers, `%f` for floating-point numbers, etc.
   - Numeric formatting: width, precision, alignment, zero-padding, etc.
   - Support for Boost.Format's extended syntax where appropriate
   - Date and time formatting (optional, could be a separate task)
   - Custom formatting for user-defined types (optional)

3. **Error Handling**:
   - Utilize Boost.Format's error handling mechanisms
   - Wrap Boost exceptions in SimpleString's exception hierarchy
   - Clear error messages for format string syntax errors
   - Exception handling for argument type mismatches
   - Handling of missing or extra arguments

## Acceptance Criteria

- [ ] Implement static `String::format(const String& format, ...)` method for basic formatting
- [ ] Implement variadic template version for type safety
- [ ] Support the following format specifiers:
  - [ ] `%s` for strings
  - [ ] `%%` for literal percent sign
  - [ ] `%d`, `%i` for integers
  - [ ] `%f`, `%e`, `%g` for floating-point numbers
  - [ ] `%c` for characters
  - [ ] `%b` for boolean values -- as "true" and "false"
- [ ] Support format modifiers (leveraging Boost.Format capabilities):
  - [ ] Width specification (e.g., `%10s`)
  - [ ] Precision for floating-point numbers (e.g., `%.2f`)
  - [ ] Left/right alignment (e.g., `%-10s`)
    - [ ] Ensure to check that 0 pad-number (like for `%010s`) should not be interpreted as octal
  - [ ] Zero-padding for numbers (e.g., `%04d`)
    - Note: zero-padding flag is ignored for strings (e.g., `%010s` will use space padding, not zeros)
  - [ ] Grouping separators (if supported by Boost.Format or implementable with reasonable effort)
- [ ] Implement proper error handling:
  - [ ] Create a `FormatException` class that wraps Boost.Format exceptions
  - [ ] Provide meaningful error messages with context information
  - [ ] Handle argument type mismatches through Boost.Format's error detection
  - [ ] Handle missing or extra arguments through Boost.Format's error detection
- [ ] Add comprehensive unit tests:
  - [ ] Basic format specifier tests
  - [ ] Format modifier tests
  - [ ] Error handling tests
  - [ ] Edge cases (empty strings, null values, etc.)
  - [ ] Performance tests for large format strings
- [ ] Update documentation to include the new formatting methods

## Implementation Details

1. **Boost.Format Integration**:
   ```cpp
   // In string.hpp
   #include <boost/format.hpp>  // Include Boost.Format
   
   namespace simple {
   
   class String {
   public:
       // Existing methods...
       
       /**
        * Formats a string using the specified format string and arguments.
        * This method uses Boost.Format internally but provides an interface
        * similar to Java's String.format().
        * 
        * @param format the format string
        * @param args the arguments to be formatted
        * @return a formatted string
        * @throws FormatException if the format string is invalid or if there is a type mismatch
        */
       template<typename... Args>
       static String format(const String& format, Args&&... args) {
           try {
               boost::format fmt(format.toStdString());
               return formatImpl(fmt, std::forward<Args>(args)...);
           } catch (const boost::io::format_error& e) {
               throw FormatException(e.what(), 0, format.toStdString());
           }
       }
       
   private:
       // Helper method for recursive argument processing using Boost.Format
       template<typename T, typename... Args>
       static String formatImpl(boost::format& fmt, T&& value, Args&&... args) {
           fmt % std::forward<T>(value);
           return formatImpl(fmt, std::forward<Args>(args)...);
       }
       
       // Base case for recursion
       static String formatImpl(boost::format& fmt) {
           return String(fmt.str());
       }
       
       // Other methods...
   };
   
   /**
    * Exception thrown when a formatting error occurs.
    * This wraps Boost.Format exceptions to provide a consistent interface.
    */
   class FormatException : public std::runtime_error {
   public:
       explicit FormatException(const std::string& message);
       
       /**
        * Creates a new FormatException with detailed context information.
        * 
        * @param message the error message
        * @param position the position in the format string where the error occurred
        * @param formatString the format string that caused the error
        */
       FormatException(const std::string& message, 
                      std::size_t position, 
                      const std::string& formatString);
       
       /**
        * Gets the position in the format string where the error occurred.
        * 
        * @return the position
        */
       std::size_t getPosition() const;
       
       /**
        * Gets the format string that caused the error.
        * 
        * @return the format string
        */
       std::string getFormatString() const;
       
   private:
       std::size_t position_;
       std::string formatString_;
       bool hasContext_;
       std::string fullMessage_;
   };
   
   } // namespace simple
   ```

2. **Format Specifier Handling**:
   - Leverage Boost.Format's built-in format specifier handling
   - Provide adapters for Java-style format specifiers to Boost.Format specifiers
   - Handle width, precision, alignment, and other format modifiers through Boost.Format
   - Document any differences between Java's formatting and Boost.Format's behavior

3. **Test Cases**:
   ```cpp
   // Basic formatting tests
   TEST(StringFormatTest, BasicFormatting) {
       // String formatting
       EXPECT_EQ("Hello, World!", String::format("%s, %s!", "Hello", "World"));
       
       // Integer formatting
       EXPECT_EQ("The answer is 42", String::format("The answer is %d", 42));
       
       // Float formatting
       EXPECT_EQ("Pi is approximately 3.14", String::format("Pi is approximately %.2f", 3.14159));
       
       // Character formatting
       EXPECT_EQ("The first letter is A", String::format("The first letter is %c", 'A'));
       
       // Boolean formatting
       EXPECT_EQ("This is true", String::format("This is %b", true));
       
       // Boost.Format specific features
       EXPECT_EQ("Hello, World!", String::format("%1%, %2%!", "Hello", "World"));
   }
   
   // Format modifier tests
   TEST(StringFormatTest, FormatModifiers) {
       // Width and alignment
       EXPECT_EQ("    Hello", String::format("%10s", "Hello"));
       EXPECT_EQ("Hello    ", String::format("%-10s", "Hello"));
       
       // Zero padding
       EXPECT_EQ("00042", String::format("%05d", 42));
       
       // Precision
       EXPECT_EQ("3.14", String::format("%.2f", 3.14159));
       
       // Grouping (if supported by Boost.Format)
       // Note: This might need adaptation if Boost.Format doesn't support this directly
       EXPECT_EQ("1,234,567", String::format("%'d", 1234567));
   }
   
   // Error handling tests
   TEST(StringFormatTest, ErrorHandling) {
       // Format string syntax error
       EXPECT_THROW(String::format("%"), FormatException);
       
       // Argument type mismatch
       EXPECT_THROW(String::format("%d", "not an integer"), FormatException);
       
       // Missing argument
       EXPECT_THROW(String::format("%s %s", "only one"), FormatException);
       
       // Test Boost.Format exception wrapping
       EXPECT_THROW({
           try {
               String::format("%s %s", "only one");
           } catch (const FormatException& e) {
               // Verify the exception contains the original format string
               EXPECT_EQ("%s %s", e.getFormatString());
               throw;
           }
       }, FormatException);
   }
   ```

## Dependencies

- C++ Standard Library
- Boost.Format (primary dependency for implementation)
- Boost.Locale (already a dependency of the project)

## Notes

- The implementation should maintain the immutability principle of the String class
- Leverage Boost.Format for the core implementation to ensure reliability and efficiency
- Document any differences between Java's String.format() and the Boost.Format-based implementation
- Features that are difficult to implement reliably or efficiently with Boost.Format may be omitted
- Performance considerations should be documented, especially for large format strings
- Future extensions could include date/time formatting and custom formatters for user-defined types
- Consider providing both Java-style (%s, %d) and Boost-style (%1%, %2%) format specifiers for flexibility

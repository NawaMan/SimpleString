# Task-015: String Encoding and Decoding (getBytes)

## Motivation

In many applications, it's necessary to convert strings between different character encodings or to obtain the raw byte representation of a string. This is particularly important for I/O operations, network communication, and interoperability with systems that use different encodings. Implementing `getBytes` and related encoding/decoding methods will enhance the SimpleString library's utility for these scenarios.

## Description

Implement methods to retrieve the byte representation of a string in different encodings and to create strings from byte arrays with specified encodings. The primary focus will be on supporting common encodings such as UTF-8, UTF-16, UTF-32, ISO-8859-1 (Latin-1), and ASCII.

1. **Core Functionality**:
   - Methods to convert a String to a byte array in a specified encoding (defaulting to UTF-8)
   - Methods to create a String from a byte array with a specified encoding (defaulting to UTF-8)
   - Support for detecting encoding errors and handling them appropriately

2. **Encoding Support**:
   - UTF-8 (already used internally, default encoding)
   - UTF-16 (both BE and LE variants, with BOM handling options)
   - UTF-32 (both BE and LE variants, with BOM handling options)
   - ISO-8859-1 (Latin-1)
   - ASCII
   - Potentially other encodings through Boost.Locale or ICU

3. **Error Handling**:
   - Options for handling encoding errors (throw, replace, ignore)
   - Clear documentation of behavior for invalid input
   - Detailed exception information including encoding type, error location, and context

## Acceptance Criteria

- [x] Implement `getBytes()` method that returns the UTF-8 encoded bytes (default encoding)
- [x] Implement `getBytes(Encoding encoding)` method that returns bytes in the specified encoding
- [x] Implement `getBytes(Encoding encoding, ErrorHandling errorHandling)` for customized error handling
- [x] Add static `String::fromBytes(byte[] bytes)` method that creates a String from UTF-8 encoded bytes
- [x] Add static `String::fromBytes(byte[] bytes, Encoding encoding)` method for specified encoding
- [x] Add static `String::fromBytes(byte[] bytes, Encoding encoding, ErrorHandling errorHandling)` method
- [x] Create an `Encoding` enum or class to represent different character encodings
- [x] Create an `ErrorHandling` enum to represent different error handling strategies
- [x] Create a `BOMPolicy` enum to control Byte Order Mark handling for UTF-16/UTF-32
- [x] Add convenience methods for std::string interoperability:
  - [x] `toStdString(Encoding encoding = Encoding::UTF_8)` method
  - [x] Static `fromStdString(const std::string& str, Encoding encoding = Encoding::UTF_8)` method
- [x] Add comprehensive unit tests:
  - [x] Converting strings to bytes in different encodings
  - [x] Creating strings from bytes in different encodings
  - [x] Handling of invalid byte sequences
  - [x] Round-trip conversion tests
  - [x] BOM handling tests for UTF-16/UTF-32
  - [x] Edge cases (empty strings, strings with null characters, etc.)
  - [x] Error handling tests with different strategies
- [x] Update documentation to include the new encoding/decoding methods

## Implementation Details

1. **Encoding Class/Enum**:
   ```cpp
   // In encoding.hpp
   namespace simple {

   enum class Encoding {
       UTF_8,
       UTF_16BE,
       UTF_16LE,
       UTF_32BE,
       UTF_32LE,
       ISO_8859_1,
       ASCII
   };

   enum class EncodingErrorHandling {
       THROW,     // Throw exception on invalid input
       REPLACE,   // Replace invalid sequences with replacement character
       IGNORE     // Skip invalid sequences
   };
   
   enum class BOMPolicy {
       AUTO,      // Auto-detect BOM when decoding, don't add when encoding
       INCLUDE,   // Add BOM when encoding, expect when decoding
       EXCLUDE    // Don't add BOM when encoding, ignore when decoding
   };

   } // namespace simple
   ```

2. **String Class Extensions**:
   ```cpp
   // In string.hpp
   namespace simple {

   class String {
   public:
       // Existing methods...

       /**
        * Returns the bytes representing this string in UTF-8 encoding.
        * This is the default encoding used by the library.
        * 
        * @return a vector of bytes representing this string in UTF-8
        */
       std::vector<uint8_t> getBytes() const;

       /**
        * Returns the bytes representing this string in the specified encoding.
        * 
        * @param encoding the character encoding to use
        * @return a vector of bytes representing this string in the specified encoding
        * @throws EncodingException if the string cannot be encoded in the specified encoding
        */
       std::vector<uint8_t> getBytes(Encoding encoding) const;

       /**
        * Returns the bytes representing this string in the specified encoding,
        * with the specified error handling strategy.
        * 
        * @param encoding the character encoding to use
        * @param errorHandling how to handle encoding errors
        * @return a vector of bytes representing this string in the specified encoding
        * @throws EncodingException if errorHandling is THROW and an encoding error occurs
        */
       std::vector<uint8_t> getBytes(Encoding encoding, EncodingErrorHandling errorHandling) const;
       
       /**
        * Returns the bytes representing this string in the specified encoding,
        * with the specified error handling strategy and BOM policy.
        * 
        * @param encoding the character encoding to use
        * @param errorHandling how to handle encoding errors
        * @param bomPolicy how to handle Byte Order Marks for UTF-16/UTF-32
        * @return a vector of bytes representing this string in the specified encoding
        * @throws EncodingException if errorHandling is THROW and an encoding error occurs
        */
       std::vector<uint8_t> getBytes(Encoding encoding, EncodingErrorHandling errorHandling, BOMPolicy bomPolicy) const;

       /**
        * Converts this string to a std::string using the specified encoding.
        * Defaults to UTF-8 if no encoding is specified.
        * 
        * @param encoding the character encoding to use
        * @return a std::string representation of this string
        * @throws EncodingException if the string cannot be encoded in the specified encoding
        */
       std::string toStdString(Encoding encoding = Encoding::UTF_8) const;

       /**
        * Creates a new String from the specified array of bytes using UTF-8 encoding.
        * UTF-8 is the default encoding used by the library.
        * 
        * @param bytes the bytes to decode
        * @return a new String created from the specified bytes
        * @throws EncodingException if the bytes are not valid UTF-8
        */
       static String fromBytes(const std::vector<uint8_t>& bytes);

       /**
        * Creates a new String from the specified array of bytes using the specified encoding.
        * 
        * @param bytes the bytes to decode
        * @param encoding the character encoding to use
        * @return a new String created from the specified bytes
        * @throws EncodingException if the bytes are not valid in the specified encoding
        */
       static String fromBytes(const std::vector<uint8_t>& bytes, Encoding encoding);

       /**
        * Creates a new String from the specified array of bytes using the specified encoding
        * and error handling strategy.
        * 
        * @param bytes the bytes to decode
        * @param encoding the character encoding to use
        * @param errorHandling how to handle decoding errors
        * @return a new String created from the specified bytes
        * @throws EncodingException if errorHandling is THROW and a decoding error occurs
        */
       static String fromBytes(const std::vector<uint8_t>& bytes, Encoding encoding, EncodingErrorHandling errorHandling);
       
       /**
        * Creates a new String from the specified array of bytes using the specified encoding,
        * error handling strategy, and BOM policy.
        * 
        * @param bytes the bytes to decode
        * @param encoding the character encoding to use
        * @param errorHandling how to handle decoding errors
        * @param bomPolicy how to handle Byte Order Marks for UTF-16/UTF-32
        * @return a new String created from the specified bytes
        * @throws EncodingException if errorHandling is THROW and a decoding error occurs
        */
       static String fromBytes(const std::vector<uint8_t>& bytes, Encoding encoding, 
                               EncodingErrorHandling errorHandling, BOMPolicy bomPolicy);

       /**
        * Creates a new String from a std::string using the specified encoding.
        * Defaults to UTF-8 if no encoding is specified.
        * 
        * @param str the std::string to convert
        * @param encoding the character encoding to use
        * @return a new String created from the specified std::string
        * @throws EncodingException if the string is not valid in the specified encoding
        */
       static String fromStdString(const std::string& str, Encoding encoding = Encoding::UTF_8);

       // Other methods...
   };

   /**
    * Exception thrown when an encoding or decoding error occurs.
    */
   class EncodingException : public std::runtime_error {
   public:
       explicit EncodingException(const std::string& message);
       
       /**
        * Creates a new EncodingException with detailed context information.
        * 
        * @param message the error message
        * @param encoding the encoding that was being used
        * @param byteOffset the offset in the byte array where the error occurred
        * @param errorHandling the error handling strategy that was being used
        */
       EncodingException(const std::string& message, Encoding encoding, 
                         size_t byteOffset, EncodingErrorHandling errorHandling);
       
       /**
        * Gets the encoding that was being used when the error occurred.
        * 
        * @return the encoding
        */
       Encoding getEncoding() const;
       
       /**
        * Gets the byte offset where the error occurred.
        * 
        * @return the byte offset
        */
       size_t getByteOffset() const;
       
       /**
        * Gets the error handling strategy that was being used.
        * 
        * @return the error handling strategy
        */
       EncodingErrorHandling getErrorHandling() const;
       
   private:
       Encoding encoding_;
       size_t byteOffset_;
       EncodingErrorHandling errorHandling_;
   };

   } // namespace simple
   ```

3. **Implementation Approach**:
   - Leverage Boost.Locale or ICU for encoding conversion
   - Implement efficient byte array handling
   - Ensure proper error detection and reporting with detailed context
   - Optimize for common encoding scenarios
   - Provide clear BOM handling policies for UTF-16/UTF-32

4. **Test Cases**:
   ```cpp
   // Test invalid UTF-8 handling
   TEST(StringEncodingTest, InvalidUtf8Handling) {
       // Overlong encoding for '/' character (invalid UTF-8)
       std::vector<uint8_t> invalidUtf8 = {0xC0, 0xAF};
       
       // Should throw with default error handling
       EXPECT_THROW(String::fromBytes(invalidUtf8), EncodingException);
       
       // Should replace with replacement character with REPLACE handling
       String replaced = String::fromBytes(invalidUtf8, Encoding::UTF_8, EncodingErrorHandling::REPLACE);
       EXPECT_EQ(replaced.length(), 1); // One replacement character
       
       // Should ignore with IGNORE handling
       String ignored = String::fromBytes(invalidUtf8, Encoding::UTF_8, EncodingErrorHandling::IGNORE);
       EXPECT_TRUE(ignored.is_empty());
   }
   
   // Test round-trip conversion
   TEST(StringEncodingTest, RoundTripConversion) {
       // ASCII round-trip
       std::vector<uint8_t> asciiBytes = {'h', 'e', 'l', 'l', 'o'};
       String asciiString = String::fromBytes(asciiBytes);
       EXPECT_EQ(asciiString.getBytes(), asciiBytes);
       
       // UTF-8 round-trip with Unicode
       String unicodeString("Hello 世界");
       std::vector<uint8_t> utf8Bytes = unicodeString.getBytes();
       String roundTrip = String::fromBytes(utf8Bytes);
       EXPECT_EQ(roundTrip, unicodeString);
   }
   
   // Test BOM handling
   TEST(StringEncodingTest, BOMHandling) {
       String text("Hello");
       
       // UTF-16BE with BOM
       std::vector<uint8_t> utf16beWithBOM = text.getBytes(Encoding::UTF_16BE, 
                                                          EncodingErrorHandling::THROW, 
                                                          BOMPolicy::INCLUDE);
       // First two bytes should be the BOM (0xFE, 0xFF for UTF-16BE)
       EXPECT_EQ(utf16beWithBOM[0], 0xFE);
       EXPECT_EQ(utf16beWithBOM[1], 0xFF);
       
       // Decode with BOM auto-detection
       String decoded = String::fromBytes(utf16beWithBOM, Encoding::UTF_16BE, 
                                         EncodingErrorHandling::THROW, 
                                         BOMPolicy::AUTO);
       EXPECT_EQ(decoded, text);
   }
   ```

## Dependencies

- Boost.Locale (already a dependency)
- Potentially ICU for more comprehensive encoding support

## Notes

- The implementation should maintain the immutability principle of the String class
- Performance considerations should be documented, especially for large strings
- The API should be designed to be intuitive and consistent with the rest of the library
- BOM handling should be well-documented and follow common conventions
- Error reporting should include sufficient context for debugging encoding issues

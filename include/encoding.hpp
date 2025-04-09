#ifndef SIMPLE_ENCODING_HPP
#define SIMPLE_ENCODING_HPP

#include <string>
#include <stdexcept>

namespace simple {

/**
 * Enumeration of supported character encodings.
 * 
 * This enumeration defines the character encodings that can be used for
 * encoding and decoding text data in the String class.
 */
enum class Encoding {
    UTF_8,      // UTF-8 encoding (default)
    UTF_16BE,   // UTF-16 Big Endian
    UTF_16LE,   // UTF-16 Little Endian
    UTF_32BE,   // UTF-32 Big Endian
    UTF_32LE,   // UTF-32 Little Endian
    ISO_8859_1, // ISO-8859-1 (Latin-1)
    ASCII       // ASCII (7-bit)
};

/**
 * Enumeration of error handling strategies for encoding/decoding operations.
 * 
 * This enumeration defines how encoding and decoding errors should be handled
 * when processing text data with unsupported or invalid character sequences.
 */
enum class EncodingErrorHandling {
    THROW,    // Throw exception on invalid input
    REPLACE,  // Replace invalid sequences with replacement character
    IGNORE    // Skip invalid sequences
};

/**
 * Enumeration of Byte Order Mark (BOM) handling policies.
 * 
 * This enumeration defines how Byte Order Marks (BOMs) should be handled
 * during encoding and decoding operations. BOMs are special byte sequences
 * at the beginning of a text stream that indicate the encoding and endianness.
 */
enum class BOMPolicy {
    AUTO,     // Auto-detect BOM when decoding, don't add when encoding
    INCLUDE,  // Add BOM when encoding, expect when decoding
    EXCLUDE   // Don't add BOM when encoding, ignore when decoding
};

/**
 * Exception thrown when an encoding or decoding error occurs.
 * 
 * This exception provides detailed information about encoding and decoding errors,
 * including the encoding being used, the byte offset where the error occurred,
 * and the error handling strategy that was in effect.
 */
class EncodingException : public std::runtime_error {
public:
    /**
     * Creates a new EncodingException with a simple message.
     * 
     * This constructor creates an exception with minimal context information.
     * 
     * @param message The error message describing the exception
     */
    explicit EncodingException(const std::string& message);
    
    /**
     * Creates a new EncodingException with detailed context information.
     * 
     * This constructor creates an exception with comprehensive context information
     * about where and how the encoding error occurred.
     * 
     * @param message The error message describing the exception
     * @param encoding The encoding that was being used when the error occurred
     * @param byteOffset The offset in the byte array where the error occurred
     * @param errorHandling The error handling strategy that was being used
     */
    EncodingException(const std::string& message, Encoding encoding, 
                      size_t byteOffset, EncodingErrorHandling errorHandling);
    
    /**
     * Gets the encoding that was being used when the error occurred.
     * 
     * @return The encoding that was active during the error
     */
    Encoding getEncoding() const;
    
    /**
     * Gets the byte offset where the error occurred.
     * 
     * @return The position in the byte array where the error was detected
     */
    size_t getByteOffset() const;
    
    /**
     * Gets the error handling strategy that was being used.
     * 
     * @return The error handling strategy that was active during the error
     */
    EncodingErrorHandling getErrorHandling() const;
    
    /**
     * Override of what() to provide more detailed error messages.
     * 
     * This method returns a detailed error message that includes context information
     * about the encoding error when available.
     * 
     * @return The detailed error message as a null-terminated string
     */
    const char* what() const noexcept override;
    
private:
    Encoding encoding_;
    size_t byteOffset_;
    EncodingErrorHandling errorHandling_;
    bool hasContext_;
    std::string fullMessage_;
};

/**
 * Returns a string representation of the encoding.
 * 
 * This function converts an Encoding enum value to its corresponding string name,
 * which is useful for logging and error messages.
 * 
 * @param encoding The encoding to convert to a string
 * @return A string representation of the encoding (e.g., "UTF-8", "UTF-16BE")
 */
std::string to_string(Encoding encoding);

/**
 * Returns a string representation of the error handling strategy.
 * 
 * This function converts an EncodingErrorHandling enum value to its corresponding
 * string name, which is useful for logging and error messages.
 * 
 * @param errorHandling The error handling strategy to convert to a string
 * @return A string representation of the error handling strategy (e.g., "THROW", "REPLACE")
 */
std::string to_string(EncodingErrorHandling errorHandling);

/**
 * Returns a string representation of the BOM policy.
 * 
 * This function converts a BOMPolicy enum value to its corresponding string name,
 * which is useful for logging and configuration settings.
 * 
 * @param bomPolicy The BOM policy to convert to a string
 * @return A string representation of the BOM policy (e.g., "AUTO", "INCLUDE")
 */
std::string to_string(BOMPolicy bomPolicy);

} // namespace simple

#endif // SIMPLE_ENCODING_HPP

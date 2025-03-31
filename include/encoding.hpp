#ifndef SIMPLE_ENCODING_HPP
#define SIMPLE_ENCODING_HPP

#include <string>
#include <stdexcept>

namespace simple {

/**
 * Enumeration of supported character encodings.
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
 */
enum class EncodingErrorHandling {
    THROW,    // Throw exception on invalid input
    REPLACE,  // Replace invalid sequences with replacement character
    IGNORE    // Skip invalid sequences
};

/**
 * Enumeration of Byte Order Mark (BOM) handling policies.
 */
enum class BOMPolicy {
    AUTO,     // Auto-detect BOM when decoding, don't add when encoding
    INCLUDE,  // Add BOM when encoding, expect when decoding
    EXCLUDE   // Don't add BOM when encoding, ignore when decoding
};

/**
 * Exception thrown when an encoding or decoding error occurs.
 */
class EncodingException : public std::runtime_error {
public:
    /**
     * Creates a new EncodingException with a message.
     * 
     * @param message the error message
     */
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
    
    /**
     * Override of what() to provide more detailed error messages.
     * 
     * @return the detailed error message
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
 * @param encoding the encoding
 * @return a string representation of the encoding
 */
std::string to_string(Encoding encoding);

/**
 * Returns a string representation of the error handling strategy.
 * 
 * @param errorHandling the error handling strategy
 * @return a string representation of the error handling strategy
 */
std::string to_string(EncodingErrorHandling errorHandling);

/**
 * Returns a string representation of the BOM policy.
 * 
 * @param bomPolicy the BOM policy
 * @return a string representation of the BOM policy
 */
std::string to_string(BOMPolicy bomPolicy);

} // namespace simple

#endif // SIMPLE_ENCODING_HPP

#pragma once

#include <string>

namespace simple {
    
/**
 * Char - A class representing a UTF-16 code unit, similar to Java's Character.
 * 
 * This class provides:
 * 1. Storage of a single UTF-16 code unit (16 bits)
 * 2. Methods for surrogate pair detection and handling
 * 3. Conversion between different character types
 * 4. Value comparison operators
 * 
 * UTF-16 encoding uses 16-bit code units to represent Unicode code points.
 * Code points in the Basic Multilingual Plane (BMP, U+0000 to U+FFFF) are represented directly.
 * Supplementary code points (U+10000 to U+10FFFF) are represented using surrogate pairs:
 * - A high surrogate (U+D800 to U+DBFF) followed by
 * - A low surrogate (U+DC00 to U+DFFF)
 */
class Char {
    
    char16_t value_;  // UTF-16 code unit

public:
    /**
     * Special values used by the Char class
     */
    static constexpr char32_t INVALID_CODEPOINT = 0xFFFFFFFF; ///< Value used to indicate an invalid code point
    static constexpr char16_t REPLACEMENT_CHAR  = 0xFFFD;    ///< Unicode replacement character for invalid sequences

    /**
     * Constructors for creating Char objects from different character types
     */
    constexpr Char() noexcept : value_(0) {} ///< Default constructor creates a null character
    
    /**
     * Creates a Char from an ASCII character
     * @param c The ASCII character to convert
     */
    constexpr explicit Char(char c) noexcept : value_(static_cast<char16_t>(c)) {}
    
    /**
     * Creates a Char from a UTF-16 code unit
     * @param c The UTF-16 code unit
     */
    constexpr explicit Char(char16_t c) noexcept : value_(c) {}
    
    /**
     * Creates a Char from a Unicode code point
     * @param c The Unicode code point
     * @note If the code point is outside the BMP, the replacement character is used
     */
    constexpr Char(char32_t c) noexcept 
        : value_(c <= 0xFFFF ? 
                 static_cast<char16_t>(c) : 
                 REPLACEMENT_CHAR) {}

    /**
     * Gets the UTF-16 code unit value
     * @return The UTF-16 code unit stored in this Char
     */
    constexpr char16_t value() const noexcept { return value_; }
    
    /**
     * Checks if this character is the null character (U+0000)
     * @return true if this is the null character, false otherwise
     */
    constexpr bool is_null() const noexcept {
        return value_ == 0;
    }
    
    /**
     * Checks if this character is a high surrogate (first part of a surrogate pair)
     * @return true if this is a high surrogate (U+D800 to U+DBFF), false otherwise
     */
    constexpr bool is_high_surrogate() const noexcept {
        return value_ >= 0xD800
            && value_ <= 0xDBFF;
    }
    
    /**
     * Checks if this character is a low surrogate (second part of a surrogate pair)
     * @return true if this is a low surrogate (U+DC00 to U+DFFF), false otherwise
     */
    constexpr bool is_low_surrogate() const noexcept {
        return value_ >= 0xDC00
            && value_ <= 0xDFFF;
    }
    
    /**
     * Checks if this character is either a high or low surrogate
     * @return true if this is either a high or low surrogate, false otherwise
     */
    constexpr bool is_surrogate() const noexcept {
        return is_high_surrogate() || is_low_surrogate();
    }

    /**
     * Converts a surrogate pair to a Unicode code point.
     * 
     * @param low_surrogate The low surrogate character to pair with this high surrogate
     * @return The Unicode code point represented by the surrogate pair, or INVALID_CODEPOINT if invalid
     */
    char32_t to_code_point(Char low_surrogate) const noexcept;

    /**
     * Converts this Char to a UTF-16 string.
     * 
     * @return A UTF-16 string containing this character
     */
    std::u16string to_string() const;

    /**
     * Comparison operators for comparing Char objects
     * These compare the underlying UTF-16 code unit values
     */
    constexpr bool operator==(Char other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(Char other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (Char other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator> (Char other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator<=(Char other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>=(Char other) const noexcept { return value_ >= other.value_; }
};

} // namespace simple

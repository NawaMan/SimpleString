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
 */
class Char {
    
    char16_t value_;  // UTF-16 code unit

public:
    // Special values
    static constexpr char32_t INVALID_CODEPOINT = 0xFFFFFFFF;
    static constexpr char16_t REPLACEMENT_CHAR  = 0xFFFD;

    // Constructors
    constexpr Char()                    noexcept : value_(0) {}
    constexpr explicit Char(char c)     noexcept : value_(static_cast<char16_t>(c)) {}
    constexpr explicit Char(char16_t c) noexcept : value_(c) {}
    constexpr Char(char32_t c) noexcept 
        : value_(c <= 0xFFFF ? 
                 static_cast<char16_t>(c) : 
                 REPLACEMENT_CHAR) {}

    // Value accessors
    constexpr char16_t value() const noexcept { return value_; }
    
    // Null check
    constexpr bool is_null() const noexcept {
        return value_ == 0;
    }
    
    // Surrogate pair checks
    constexpr bool is_high_surrogate() const noexcept {
        return value_ >= 0xD800
            && value_ <= 0xDBFF;
    }
    constexpr bool is_low_surrogate() const noexcept {
        return value_ >= 0xDC00
            && value_ <= 0xDFFF;
    }
    
    constexpr bool is_surrogate() const noexcept {
        return is_high_surrogate() || is_low_surrogate();
    }

    // Code point conversion
    constexpr char32_t to_code_point(Char low_surrogate) const noexcept {
        if (!is_high_surrogate() || !low_surrogate.is_low_surrogate()) {
            return INVALID_CODEPOINT;
        }
        return 0x10000 + ((value_ - 0xD800) << 10) + (low_surrogate.value() - 0xDC00);
    }

    // String conversion
    std::u16string to_string() const {
        return std::u16string(1, value_);
    }

    // Comparison operators
    constexpr bool operator==(Char other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(Char other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (Char other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator> (Char other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator<=(Char other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>=(Char other) const noexcept { return value_ >= other.value_; }
};

} // namespace simple

#ifndef SIMPLE_STRING_CHAR_HPP
#define SIMPLE_STRING_CHAR_HPP

#include <cstdint>
#include <string>

namespace simple_string {

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
public:
    // Constructors
    constexpr Char() noexcept : value_(0) {}
    constexpr explicit Char(char c) noexcept : value_(static_cast<char16_t>(c)) {}
    constexpr explicit Char(char16_t c) noexcept : value_(c) {}
    constexpr explicit Char(char32_t c) noexcept 
        : value_(c <= 0xFFFF ? static_cast<char16_t>(c) : highSurrogateOf(c)) {}

    // Value accessors
    constexpr char16_t value() const noexcept { return value_; }
    
    // Surrogate pair checks
    constexpr bool isHighSurrogate() const noexcept {
        return value_ >= 0xD800 && value_ <= 0xDBFF;
    }
    
    constexpr bool isLowSurrogate() const noexcept {
        return value_ >= 0xDC00 && value_ <= 0xDFFF;
    }
    
    constexpr bool isSurrogate() const noexcept {
        return value_ >= 0xD800 && value_ <= 0xDFFF;
    }

    // Code point conversion
    constexpr char32_t toCodePoint(Char lowSurrogate) const {
        if (!isHighSurrogate() || !lowSurrogate.isLowSurrogate()) {
            throw std::invalid_argument("Invalid surrogate pair");
        }
        return 0x10000 + ((value_ - 0xD800) << 10) + (lowSurrogate.value() - 0xDC00);
    }

    // Comparison operators
    constexpr bool operator==(const Char& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const Char& other) const noexcept { return value_ != other.value_; }
    constexpr bool operator<(const Char& other) const noexcept { return value_ < other.value_; }
    constexpr bool operator<=(const Char& other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>(const Char& other) const noexcept { return value_ > other.value_; }
    constexpr bool operator>=(const Char& other) const noexcept { return value_ >= other.value_; }

    // Static utility methods
    static constexpr char16_t highSurrogateOf(char32_t codePoint) noexcept {
        return static_cast<char16_t>(((codePoint - 0x10000) >> 10) + 0xD800);
    }
    
    static constexpr char16_t lowSurrogateOf(char32_t codePoint) noexcept {
        return static_cast<char16_t>(((codePoint - 0x10000) & 0x3FF) + 0xDC00);
    }
    
    static constexpr bool isSupplementaryCodePoint(char32_t codePoint) noexcept {
        return codePoint >= 0x10000 && codePoint <= 0x10FFFF;
    }

private:
    char16_t value_;  // UTF-16 code unit
};

} // namespace simple_string

#endif // SIMPLE_STRING_CHAR_HPP

#pragma once

#include <cstddef>

namespace simple {

/**
 * Size - A class representing a size value, similar to Java's concept of size.
 * 
 * This class provides a type-safe wrapper around std::size_t for representing sizes
 * and indices in the String class. It helps maintain consistency with Java's
 * behavior for size-related operations and provides a foundation for special
 * size values like npos (-1).
 * 
 * This class is designed to be lightweight and efficient, with pass-by-value
 * semantics similar to the Char class.
 */
class Size {
    
    std::size_t value_; // The underlying size value

public:
    // Special values
    static constexpr std::size_t npos = static_cast<std::size_t>(-1); // Represents "not found" or "until the end"
    
    /**
     * Constructs a Size object with the specified value.
     * 
     * @param size The size value to wrap
     */
    constexpr Size(std::size_t size) noexcept 
        : value_(size) {}
    
    /**
     * Default constructor - creates a Size with value 0.
     */
    constexpr Size() noexcept 
        : value_(0) {}

    /**
     * Returns the underlying size value.
     * 
     * @return The wrapped size value
     */
    constexpr std::size_t value() const noexcept { return value_; }
    
    /**
     * Checks if this Size represents the "not found" value.
     * 
     * @return true if this Size equals npos, false otherwise
     */
    constexpr bool is_npos() const noexcept { return value_ == npos; }
    
    // Comparison operators
    constexpr bool operator==(const Size& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const Size& other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (const Size& other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator> (const Size& other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator<=(const Size& other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>=(const Size& other) const noexcept { return value_ >= other.value_; }
    
    // Arithmetic operators
    constexpr Size operator+(const Size& other) const noexcept { return Size(value_ + other.value_); }
    constexpr Size operator-(const Size& other) const noexcept { return Size(value_ - other.value_); }
};

} // namespace simple
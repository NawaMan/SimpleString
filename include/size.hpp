#pragma once

#include <cstddef>

namespace simple {

/**
 * @brief A class representing a size value, similar to Java's concept of size.
 * 
 * This class provides a type-safe wrapper around std::size_t for representing sizes
 * and indices in the String class. It helps maintain consistency with Java's
 * behavior for size-related operations and provides a foundation for special
 * size values like npos (-1).
 * 
 * This class is designed to be lightweight and efficient, with pass-by-value
 * semantics similar to the Char class. All operations are constexpr and noexcept
 * for optimal performance.
 */
class Size {
    
    std::size_t value_; ///< The underlying size value

public:
    /**
     * @brief Special value representing "not found" or "until the end"
     * 
     * This value is equivalent to the maximum value of std::size_t (typically -1 when
     * cast to std::size_t), which is used to indicate that an element was not found
     * or that an operation should continue until the end of a sequence.
     */
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
    
    /**
     * @brief Constructs a Size object with the specified value.
     * 
     * @param size The size value to wrap
     */
    constexpr Size(std::size_t size) noexcept 
        : value_(size) {}
    
    /**
     * @brief Default constructor - creates a Size with value 0.
     */
    constexpr Size() noexcept 
        : value_(0) {}

    /**
     * @brief Returns the underlying size value.
     * 
     * @return The wrapped size value as std::size_t
     */
    constexpr std::size_t value() const noexcept { return value_; }
    
    /**
     * @brief Checks if this Size represents the "not found" value.
     * 
     * @return true if this Size equals npos, false otherwise
     */
    constexpr bool is_npos() const noexcept { return value_ == npos; }
    
    /**
     * @brief Comparison operators for Size objects
     * 
     * These operators allow Size objects to be compared with each other
     * using standard comparison operators.
     * 
     * @param other The Size object to compare with
     * @return Result of the comparison operation
     */
    constexpr bool operator==(const Size& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const Size& other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (const Size& other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator> (const Size& other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator<=(const Size& other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>=(const Size& other) const noexcept { return value_ >= other.value_; }
    
    /**
     * @brief Arithmetic operators for Size objects
     * 
     * These operators allow Size objects to be added and subtracted.
     * Note that these operations do not perform any bounds checking.
     * 
     * @param other The Size object to perform arithmetic with
     * @return A new Size object with the result of the operation
     */
    constexpr Size operator+(const Size& other) const noexcept { return Size(value_ + other.value_); }
    constexpr Size operator-(const Size& other) const noexcept { return Size(value_ - other.value_); }
};

} // namespace simple
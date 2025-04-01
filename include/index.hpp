#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @file index.hpp
 * @brief Defines the Index class for representing and manipulating string indices
 */

namespace simple {

/**
 * @class Index
 * @brief A wrapper class for string indices with special handling for invalid positions
 * 
 * The Index class provides a type-safe way to represent positions within strings,
 * with built-in support for invalid indices (such as "not found" results).
 * It offers comparison and arithmetic operations, and clear semantics for
 * handling special cases like SIZE_MAX representing "not found".
 */

class Index {
    
    std::size_t value_; ///< The underlying index value

    /**
     * @brief Constant representing an invalid index value
     * 
     * This value (SIZE_MAX) is used to represent "not found" or "until the end"
     * conditions when working with string indices.
     */
    static constexpr std::size_t INVALID = SIZE_MAX;

    /**
     * @brief Private default constructor that creates an invalid index
     * 
     * This constructor is private to encourage use of the static invalid constant
     * for better code readability.
     */
    constexpr Index() noexcept 
        : value_(SIZE_MAX) {}

public:

    /**
     * @brief Static constant representing an invalid index
     * 
     * Use this constant instead of constructing invalid indices directly.
     * Example: `if (str.index_of('x') == Index::invalid) { ... }`
     */
    static const Index invalid;

    /**
     * @brief Constructs an Index with the specified value
     * 
     * @param index The numeric index value to wrap
     */
    constexpr Index(std::size_t index) noexcept 
        : value_(index) {}
    
    /**
     * @brief Gets the underlying numeric index value
     * 
     * @return The raw index value as a size_t
     */
    constexpr std::size_t value() const noexcept { return value_; }
    
    /**
     * @brief Checks if this index represents a valid position
     * 
     * @return true if the index is valid (not SIZE_MAX), false otherwise
     */
    constexpr bool is_valid() const noexcept { return value_ != INVALID; }
    
    /**
     * @brief Checks if this index represents an invalid position
     * 
     * @return true if the index is invalid (equal to SIZE_MAX), false otherwise
     */
    constexpr bool is_invalid() const noexcept { return value_ == INVALID; }
    
    /**
     * @brief Comparison operators for Index objects
     * 
     * These operators allow direct comparison between Index objects.
     * Note that comparing with Index::invalid follows the rules of SIZE_MAX.
     */
    constexpr bool operator==(const Index& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const Index& other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (const Index& other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator> (const Index& other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator<=(const Index& other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>=(const Index& other) const noexcept { return value_ >= other.value_; }
    
    /**
     * @brief Arithmetic operators for Index objects
     * 
     * These operators allow arithmetic operations between Index objects.
     * Be cautious when using these with potentially invalid indices.
     */
    constexpr Index operator+(const Index& other) const noexcept { return Index(value_ + other.value_); }
    constexpr Index operator-(const Index& other) const noexcept { return Index(value_ - other.value_); }
};

/**
 * Initialize the static invalid member outside the class definition.
 * This represents the "not found" or "until the end" special index value.
 */
constexpr Index Index::invalid = Index();

} // namespace simple
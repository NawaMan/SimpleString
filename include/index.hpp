#pragma once

#include <cstddef>
#include <cstdint>

namespace simple {

class Index {
    
    std::size_t value_;

    static constexpr std::size_t INVALID = SIZE_MAX; // Represents "not found" or "until the end"

    // Represents "not found" or "until the end"
    constexpr Index() noexcept 
        : value_(SIZE_MAX) {}

public:

    // Special values
    static const Index invalid; // Represents "not found" or "until the end"

    constexpr Index(std::size_t index) noexcept 
        : value_(index) {}
    
    constexpr std::size_t value() const noexcept { return value_; }
    
    constexpr bool is_valid() const noexcept { return value_ != INVALID; }
    
    constexpr bool is_invalid() const noexcept { return value_ == INVALID; }
    
    // Comparison operators
    constexpr bool operator==(const Index& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const Index& other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (const Index& other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator> (const Index& other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator<=(const Index& other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>=(const Index& other) const noexcept { return value_ >= other.value_; }
    
    // Arithmetic operators
    constexpr Index operator+(const Index& other) const noexcept { return Index(value_ + other.value_); }
    constexpr Index operator-(const Index& other) const noexcept { return Index(value_ - other.value_); }
};

// Initialize the static member outside the class definition
constexpr Index Index::invalid = Index();

} // namespace simple
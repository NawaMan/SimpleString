#pragma once
#include <cstdint>
#include "unicode_category.hpp"

namespace simple_string {

/**
 * @brief A class representing a Unicode code point with category information
 * 
 * This class encapsulates a Unicode code point (char32_t) and provides methods
 * to query its Unicode category and properties. All methods are constexpr and
 * noexcept for compile-time usage.
 * 
 * Example usage:
 * @code
 * CodePoint cp(U'A');
 * if (cp.is_category(UnicodeCategory::UPPERCASE_LETTER)) {
 *     // Handle uppercase letter
 * }
 * auto cat = cp.category(); // Get the UnicodeCategory
 * @endcode
 */
class CodePoint final {
public:
    /**
     * @brief Constructs a CodePoint from a char32_t value
     * @param value The Unicode code point value
     */
    constexpr explicit CodePoint(char32_t value) noexcept : value_(value) {}
    
    /**
     * @brief Gets the raw code point value
     * @return The underlying char32_t value
     */
    constexpr char32_t value() const noexcept { return value_; }
    
    /**
     * @brief Gets the Unicode category of this code point
     * @return The UnicodeCategory for this code point
     */
    constexpr UnicodeCategory category() const noexcept {
        return UnicodeCategory::from_code_point(value_);
    }
    
    /**
     * @brief Checks if this code point belongs to a specific category
     * @param cat The category to check against
     * @return true if this code point belongs to the specified category
     */
    constexpr bool is_category(UnicodeCategory cat) const noexcept {
        return category() == cat;
    }
    
    // Convenience methods for common category checks
    constexpr bool is_letter()      const noexcept { return category().is_letter();      }
    constexpr bool is_mark()        const noexcept { return category().is_mark();        }
    constexpr bool is_number()      const noexcept { return category().is_number();      }
    constexpr bool is_punctuation() const noexcept { return category().is_punctuation(); }
    constexpr bool is_symbol()      const noexcept { return category().is_symbol();      }
    constexpr bool is_separator()   const noexcept { return category().is_separator();   }
    
    // Additional character properties
    constexpr bool is_whitespace()     const noexcept { return category().is_whitespace();     }
    constexpr bool is_alphanumeric()   const noexcept { return category().is_alphanumeric();   }
    constexpr bool is_printable()      const noexcept { return category().is_printable();      }
    constexpr bool is_case_ignorable() const noexcept { return category().is_case_ignorable(); }
    
    // Comparison operators
    constexpr bool operator==(const CodePoint& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const CodePoint& other) const noexcept { return value_ != other.value_; }
    constexpr bool operator< (const CodePoint& other) const noexcept { return value_ <  other.value_; }
    constexpr bool operator<=(const CodePoint& other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator> (const CodePoint& other) const noexcept { return value_ >  other.value_; }
    constexpr bool operator>=(const CodePoint& other) const noexcept { return value_ >= other.value_; }
    
private:
    char32_t value_;
};

} // namespace simple_string

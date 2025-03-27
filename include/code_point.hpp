#pragma once
#include "unicode_category.hpp"

namespace mosaic {

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
    constexpr mosaic::UnicodeCategory category() const noexcept {
        return mosaic::UnicodeCategory::from_code_point(value_);
    }
    
    /**
     * @brief Checks if this code point belongs to a specific category
     * @param cat The category to check against
     * @return true if this code point belongs to the specified category
     */
    constexpr bool is_category(mosaic::UnicodeCategory cat) const noexcept {
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
    
    /**
     * @brief Checks if this code point is part of a surrogate pair
     * @return True if the code point is a high or low surrogate, false otherwise
     */
    constexpr bool is_surrogate_pair() const noexcept {
        return value_ >= 0xD800 && value_ <= 0xDFFF;
    }

    /**
     * @brief Retrieves the high surrogate for supplementary code points
     * @return The high surrogate if the code point is supplementary, null character otherwise
     */
    constexpr char32_t high_surrogate() const noexcept {
        if (is_surrogate_pair() && value_ >= 0xD800 && value_ <= 0xDBFF) {
            return value_;  // High surrogate in the range [0xD800, 0xDBFF]
        }
        return 0;  // Not a valid high surrogate
    }

    /**
     * @brief Retrieves the low surrogate for supplementary code points
     * @return The low surrogate if the code point is supplementary, null character otherwise
     */
    constexpr char32_t low_surrogate() const noexcept {
        if (is_surrogate_pair() && value_ >= 0xDC00 && value_ <= 0xDFFF) {
            return value_;  // Low surrogate in the range [0xDC00, 0xDFFF]
        }
        return 0;  // Not a valid low surrogate
    }
    
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

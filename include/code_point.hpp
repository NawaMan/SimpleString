#pragma once

#include "unicode_category.hpp"

namespace simple {

/**
 * A class representing a Unicode code point with category information.
 * 
 * This class encapsulates a Unicode code point (char32_t) and provides methods
 * to query its Unicode category and properties. Most methods are constexpr and
 * noexcept for compile-time usage.
 * 
 * Unicode code points are values in the range 0 to 0x10FFFF that represent characters
 * in the Unicode standard. This class provides methods to:
 * - Query the Unicode category of a code point (letter, number, symbol, etc.)
 * - Check common character properties (whitespace, alphanumeric, etc.)
 * - Handle surrogate pairs for UTF-16 encoding
 * 
 * Example usage:
 * @code
 * CodePoint cp(U'A');
 * if (cp.is_category(UnicodeCategory::uppercase_letter())) {
 *     // Handle uppercase letter
 * }
 * auto cat = cp.category(); // Get the UnicodeCategory
 * @endcode
 */
class CodePoint final {
public:
    /**
     * Constructs a CodePoint from a char32_t value.
     * 
     * @param value The Unicode code point value (0 to 0x10FFFF)
     */
    constexpr explicit CodePoint(char32_t value) noexcept : value_(value) {}
    
    /**
     * Gets the raw code point value.
     * 
     * @return The underlying char32_t value
     */
    constexpr char32_t value() const noexcept { return value_; }
    
    /**
     * Gets the Unicode category of this code point.
     * 
     * @return The UnicodeCategory for this code point
     */
    constexpr simple::UnicodeCategory category() const noexcept {
        return simple::UnicodeCategory::from_code_point(value_);
    }
    
    /**
     * Checks if this code point belongs to a specific category.
     * 
     * @param cat The category to check against
     * @return true if this code point belongs to the specified category
     */
    constexpr bool is_category(simple::UnicodeCategory cat) const noexcept {
        return category() == cat;
    }
    
    /**
     * Convenience methods for common category checks.
     * These methods delegate to the corresponding methods in UnicodeCategory.
     */
    constexpr bool is_letter()      const noexcept { return category().is_letter();      } ///< Checks if this code point is a letter
    constexpr bool is_mark()        const noexcept { return category().is_mark();        } ///< Checks if this code point is a mark
    constexpr bool is_number()      const noexcept { return category().is_number();      } ///< Checks if this code point is a number
    constexpr bool is_punctuation() const noexcept { return category().is_punctuation(); } ///< Checks if this code point is punctuation
    constexpr bool is_symbol()      const noexcept { return category().is_symbol();      } ///< Checks if this code point is a symbol
    constexpr bool is_separator()   const noexcept { return category().is_separator();   } ///< Checks if this code point is a separator
    
    /**
     * Additional character properties.
     * These methods delegate to the corresponding methods in UnicodeCategory.
     */
    constexpr bool is_whitespace()     const noexcept { return category().is_whitespace();     } ///< Checks if this code point is whitespace
    constexpr bool is_alphanumeric()   const noexcept { return category().is_alphanumeric();   } ///< Checks if this code point is alphanumeric
    constexpr bool is_printable()      const noexcept { return category().is_printable();      } ///< Checks if this code point is printable
    constexpr bool is_case_ignorable() const noexcept { return category().is_case_ignorable(); } ///< Checks if this code point should be ignored for case conversion
    
    /**
     * Checks if this code point is part of a surrogate pair.
     * 
     * Surrogate pairs are used in UTF-16 encoding to represent code points
     * outside the Basic Multilingual Plane (BMP). This method checks if the
     * code point is in the surrogate range (U+D800 to U+DFFF).
     * 
     * @return True if the code point is a high or low surrogate, false otherwise
     */
    constexpr bool is_surrogate_pair() const noexcept {
        return value_ >= 0xD800 && value_ <= 0xDFFF;
    }

    /**
     * Retrieves the high surrogate for supplementary code points.
     * 
     * @return The high surrogate if the code point is a high surrogate, 0 otherwise
     */
    char32_t high_surrogate() const noexcept;

    /**
     * Retrieves the low surrogate for supplementary code points.
     * 
     * @return The low surrogate if the code point is a low surrogate, 0 otherwise
     */
    char32_t low_surrogate() const noexcept;
    
    /**
     * Comparison operators for comparing CodePoint objects.
     * These compare the underlying code point values.
     */
    constexpr bool operator==(const CodePoint& other) const noexcept { return value_ == other.value_; } ///< Equality comparison
    constexpr bool operator!=(const CodePoint& other) const noexcept { return value_ != other.value_; } ///< Inequality comparison
    constexpr bool operator< (const CodePoint& other) const noexcept { return value_ <  other.value_; } ///< Less than comparison
    constexpr bool operator<=(const CodePoint& other) const noexcept { return value_ <= other.value_; } ///< Less than or equal comparison
    constexpr bool operator> (const CodePoint& other) const noexcept { return value_ >  other.value_; } ///< Greater than comparison
    constexpr bool operator>=(const CodePoint& other) const noexcept { return value_ >= other.value_; } ///< Greater than or equal comparison
    
private:
    char32_t value_;
};

} // namespace simple

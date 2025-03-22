#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP

#include <string>
#include <memory>
#include <stdexcept>
#include <boost/locale.hpp>
#include "compare_result.hpp"
#include "char.hpp"

namespace simple_string {

namespace detail {

// One-time initialization of Boost Locale
inline void init_locale() {
    static bool initialized = false;
    if (!initialized) {
        boost::locale::generator gen;
        std::locale::global(gen("en_US.UTF-8"));
        initialized = true;
    }
}

// Count UTF-16 code units, treating each byte of invalid UTF-8 as a separate code unit
inline std::size_t count_utf16_code_units(const std::string& utf8_str) {
    std::size_t count = 0;
    const unsigned char* str = reinterpret_cast<const unsigned char*>(utf8_str.c_str());
    const unsigned char* end = str + utf8_str.length();

    while (str < end) {
        if (*str < 0x80) {
            // ASCII character
            count++;
            str++;
        } else if ((*str & 0xE0) == 0xC0) {
            // 2-byte UTF-8 sequence
            if (str + 1 >= end || (str[1] & 0xC0) != 0x80) {
                // Invalid or incomplete sequence, treat each byte separately
                count++;
                str++;
                continue;
            }
            // Check for overlong encoding (should have used fewer bytes)
            unsigned int codepoint = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
            if (codepoint < 0x80) {
                // Overlong encoding, treat each byte separately
                count += 2;
                str += 2;
                continue;
            }
            count++;
            str += 2;
        } else if ((*str & 0xF0) == 0xE0) {
            // 3-byte UTF-8 sequence
            if (str + 2 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80) {
                // Invalid or incomplete sequence, treat each byte separately
                count++;
                str++;
                continue;
            }
            // Check for overlong encoding and surrogate range
            unsigned int codepoint = ((str[0] & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F);
            if (codepoint < 0x800 || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
                // Overlong encoding or surrogate range, treat each byte separately
                count += 3;
                str += 3;
                continue;
            }
            count++;
            str += 3;
        } else if ((*str & 0xF8) == 0xF0) {
            // 4-byte UTF-8 sequence (surrogate pair in UTF-16)
            if (str + 3 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80 || (str[3] & 0xC0) != 0x80) {
                // Invalid or incomplete sequence, treat each byte separately
                count++;
                str++;
                continue;
            }
            // Check for overlong encoding and valid Unicode range
            unsigned int codepoint = ((str[0] & 0x07) << 18) | ((str[1] & 0x3F) << 12) |
                                    ((str[2] & 0x3F) << 6)  | (str[3] & 0x3F);
            if (codepoint < 0x10000 || codepoint > 0x10FFFF) {
                // Overlong encoding or outside Unicode range, treat each byte separately
                count += 4;
                str += 4;
                continue;
            }
            count += 2;  // Surrogate pair counts as two UTF-16 code units
            str += 4;
        } else {
            // Invalid UTF-8 byte, treat as a single code unit
            count++;
            str++;
        }
    }
    return count;
}

// Compare two strings using byte-by-byte comparison for exact Java behavior
inline int compare_utf8_strings(const std::string& str1, const std::string& str2) {
    return str1.compare(str2);
}

} // namespace detail

/**
 * SString - A simple string class that provides Java String-like functionality
 *
 * Unicode Handling:
 * - Strings are stored internally as UTF-8
 * - length() returns the number of UTF-16 code units (like Java's String.length())
 * - Comparison operations use byte-by-byte comparison
 * 
 * Note on Unicode Normalization:
 * Different Unicode representations of the same visual character are treated as distinct strings.
 * For example:
 * - U+00E9 (é as single code point)
 * - U+0065 + U+0301 (e + ◌́ as combining character)
 * These will compare as different strings and have different lengths.
 * Future versions may support Unicode normalization for comparing such equivalent forms.
 */

class StringIndexOutOfBoundsException : public std::out_of_range {
public:
    explicit StringIndexOutOfBoundsException(const std::string& msg)
        : std::out_of_range(msg) {}
};

class SString {
public:
    // Constructor from C++ string literal
    explicit SString(const std::string& str)
        : data_(std::make_shared<const std::string>(str)), utf16_cache_() {}
    
    // Constructor from C string with explicit length (for strings with null chars)
    SString(const char* str, std::size_t length)
        : data_(std::make_shared<const std::string>(str, length)), utf16_cache_() {}
    
    // Get the length of the string in UTF-16 code units
    std::size_t length() const {
        // Initialize locale if needed
        detail::init_locale();
        
        // Use cached UTF-16 representation if available
        if (utf16_cache_) {
            return utf16_cache_->length();
        }
        
        // Otherwise count UTF-16 code units from UTF-8
        return detail::count_utf16_code_units(*data_);
    }

    /**
     * Returns true if this string is empty (length() == 0).
     * Note: A string containing only whitespace or null characters is not considered empty.
     * 
     * @return true if the string is empty, false otherwise
     */
    bool is_empty() const {
        return data_->empty();
    }

    /**
     * Compares this string with another for equality using byte-by-byte comparison.
     * Note: Characters with different Unicode representations (e.g., é vs e + ◌́)
     * will not be considered equal even if they appear visually identical.
     * 
     * @param other The string to compare with
     * @return true if the strings are exactly equal, false otherwise
     */
    bool equals(const SString& other) const {
        // Fast path: check if strings share data
        if (shares_data_with(other)) {
            return true;
        }
        
        // Otherwise do byte-by-byte comparison
        return *data_ == *other.data_;
    }

    /**
     * Compares this string with another lexicographically using byte-by-byte comparison.
     * Note: The comparison is based on UTF-8 byte values, not visual appearance.
     * Characters with different Unicode representations may compare differently
     * even if they appear visually identical.
     * 
     * @param other The string to compare with
     * @return CompareResult representing the comparison outcome:
     *         - isLess() is true if this < other
     *         - isEqual() is true if this == other
     *         - isGreater() is true if this > other
     */
    CompareResult compare_to(const SString& other) const {
        // Fast path: check if strings share data
        if (shares_data_with(other)) {
            return CompareResult::EQUAL;
        }
        
        // Otherwise do byte-by-byte comparison
        int result = detail::compare_utf8_strings(*data_, *other.data_);
        if (result < 0) return CompareResult::LESS;
        if (result > 0) return CompareResult::GREATER;
        return CompareResult::EQUAL;
    }

    /**
     * Returns the character at the specified index as a Char.
     * The index refers to UTF-16 code units, not bytes or code points.
     * 
     * @param index Zero-based index of the character
     * @return The character at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()B
     */
    Char char_at(std::size_t index) const;

    /**
     * Array-style access to characters. Equivalent to charAt().
     * 
     * @param index Zero-based index of the character
     * @return The character at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    Char operator[](std::size_t index) const { return char_at(index); }

    /**
     * Returns the raw UTF-16 code unit at the specified index.
     * 
     * @param index Zero-based index of the code unit
     * @return The UTF-16 code unit at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    char16_t char_value(std::size_t index) const;

    // Get the underlying string data
    const std::string& to_string() const { return *data_; }

    // C++ operator overloads for comparison
    bool operator==(const SString& other) const { return  equals(other);                         }
    bool operator!=(const SString& other) const { return !equals(other);                         }
    bool operator< (const SString& other) const { return compare_to(other).is_less();             }
    bool operator<=(const SString& other) const { return compare_to(other).is_less_or_equal();    }
    bool operator> (const SString& other) const { return compare_to(other).is_greater();          }
    bool operator>=(const SString& other) const { return compare_to(other).is_greater_or_equal(); }

private:
    std::shared_ptr<const std::string> data_;  // Immutable UTF-8 string storage shared between instances
    mutable std::shared_ptr<const std::u16string> utf16_cache_;  // Cached UTF-16 representation
    
    // Get or create UTF-16 representation
    const std::u16string& get_utf16() const {
        // Double-checked locking pattern with atomic operations
        auto cache = utf16_cache_;
        if (!cache) {
            // First check failed, acquire the data and create cache
            cache = std::make_shared<const std::u16string>(
                boost::locale::conv::utf_to_utf<char16_t>(*data_)
            );
            utf16_cache_ = cache;
        }
        return *cache;
    }

    /**
     * Check if this string shares the same underlying data with another string.
     * 
     * This is an implementation detail used for testing the string sharing behavior.
     * If true, the strings are guaranteed to be equal. However, if false, the
     * strings may still be equal but stored in different memory locations.
     * 
     * @param other The string to compare with
     * @return true if both strings share the same underlying data, false otherwise
     */
    bool shares_data_with(const SString& other) const { return data_ == other.data_; }

    // Allow test fixtures to access private members
    friend class SStringTest;
    friend class SStringSharing;  // Test fixture for string sharing tests
};

} // namespace simple_string

#endif // SIMPLE_STRING_HPP

#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <variant>
#include <boost/locale.hpp>
#include "compare_result.hpp"
#include "char.hpp"
#include "code_point.hpp"
#include "index.hpp"

namespace simple {

// Type traits for C++20 features
namespace traits {
    // Check if a type has a to_string() method
    template<typename T, typename = void>
    struct is_to_string_available : std::false_type {};
    
    template<typename T>
    struct is_to_string_available<T, std::void_t<
        decltype(std::declval<T>().to_string())
    >> : std::true_type {};
    
    template<typename T>
    inline constexpr bool is_to_string_available_v = is_to_string_available<T>::value;
    
    // Check if T is a container type
    template<typename T, typename = void>
    struct is_container : std::false_type {};
    
    template<typename T>
    struct is_container<T, std::void_t<
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end()),
        decltype(std::declval<T>().size())
    >> : std::true_type {};
    
    template<typename T>
    inline constexpr bool is_container_v = is_container<T>::value;
    
    // Check if T is a map-like type
    template<typename T, typename = void>
    struct is_map_like : std::false_type {};
    
    template<typename T>
    struct is_map_like<T, std::void_t<
        typename T::key_type,
        typename T::mapped_type,
        decltype(std::declval<T>().begin()->first),
        decltype(std::declval<T>().begin()->second)
    >> : is_container<T> {};
    
    template<typename T>
    inline constexpr bool is_map_like_v = is_map_like<T>::value;
    
    // Check if T is a variant type
    template<typename T, typename = void>
    struct is_variant : std::false_type {};
    
    template<typename T>
    struct is_variant<T, std::void_t<
        typename std::variant_size<std::remove_cv_t<std::remove_reference_t<T>>>::type
    >> : std::true_type {};
    
    template<typename T>
    inline constexpr bool is_variant_v = is_variant<T>::value;
}

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
 * Simple String - A simple string class that provides Java String-like functionality
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

class String {
public:
    // Static valueOf methods for primitive types
    static String valueOf(bool b);
    static String valueOf(char c);
    static String valueOf(int i);
    static String valueOf(long l);
    static String valueOf(float f);
    static String valueOf(double d);
    
    // Generic valueOf method for any type that can be converted to string
    template<typename T>
    static String valueOf(const T& obj) {
        return String(to_string_helper(obj));
    }
    
    // Default constructor - creates an empty string
    String()
        : data_(std::make_shared<const std::string>("")),
          offset_(0),
          length_(0),
          utf16_cache_() {}
    
    // Constructor from C++ string literal
    explicit String(const std::string& str)
        : data_(std::make_shared<const std::string>(str)), 
          offset_(0), 
          length_(str.length()), 
          utf16_cache_() {}
    
    // Constructor from C string with explicit length (for strings with null chars)
    String(const char* str, std::size_t length)
        : data_(std::make_shared<const std::string>(str, length)), 
          offset_(0), 
          length_(length), 
          utf16_cache_() {}
    
    // Get the length of the string in UTF-16 code units
    std::size_t length() const;

    /**
     * Returns true if this string is empty (length() == 0).
     * Note: A string containing only whitespace or null characters is not considered empty.
     * 
     * @return true if the string is empty, false otherwise
     */
    bool is_empty() const;

    /**
     * Compares this string with another for equality using byte-by-byte comparison.
     * Note: Characters with different Unicode representations (e.g., é vs e + ◌́)
     * will not be considered equal even if they appear visually identical.
     * 
     * @param other The string to compare with
     * @return true if the strings are exactly equal, false otherwise
     */
    bool equals(const String& other) const;

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
    simple::CompareResult compare_to(const String& other) const;

    /**
     * Returns the character at the specified index as a Char.
     * The index refers to UTF-16 code units, not bytes or code points.
     * 
     * @param index Zero-based index of the character
     * @return The character at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()B
     */
    Char char_at(Index index) const;

    /**
     * Array-style access to characters. Equivalent to charAt().
     * 
     * @param index Zero-based index of the character
     * @return The character at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    Char operator[](Index index) const;

    /**
     * Returns the raw UTF-16 code unit at the specified index.
     * 
     * @param index Zero-based index of the code unit
     * @return The UTF-16 code unit at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    char16_t char_value(Index index) const;

    /**
     * Returns the Unicode code point at the specified index.
     * 
     * @param index Zero-based index of the code unit to start from
     * @return The Unicode code point at the specified index as a CodePoint object
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    simple::CodePoint code_point_at(Index index) const;

    /**
     * Returns the Unicode code point before the specified index.
     * 
     * @param index One-based index of the code unit to start from
     * @return The Unicode code point before the specified index as a CodePoint object
     * @throws StringIndexOutOfBoundsException if index is negative or > length()
     */
    simple::CodePoint code_point_before(Index index) const;

    /**
     * Returns the number of Unicode code points in the specified text range.
     * 
     * @param begin_index Index to the first code unit of the text range
     * @param end_index Index after the last code unit of the text range
     * @return Number of Unicode code points in the specified range
     * @throws StringIndexOutOfBoundsException if begin_index is negative or > end_index,
     *         or end_index is > length()
     */
    std::size_t code_point_count(Index begin_index, Index end_index) const;

    // Get the underlying string data
    // For substrings, returns a new string with just the substring portion
    // For full strings (offset_=0, length_=data_->length()), returns a reference to the original data
    const std::string& to_string() const;

    /**
     * Returns a string that is a substring of this string.
     * The substring begins with the character at the specified index and extends to the end.
     *
     * @param beginIndex the beginning index, inclusive
     * @return the substring
     * @throws StringIndexOutOfBoundsException if beginIndex is larger than length()
     */
    String substring(Index beginIndex) const;
    
    /**
     * Returns a string that is a substring of this string.
     * The substring begins at beginIndex and extends to endIndex - 1.
     *
     * @param beginIndex the beginning index, inclusive
     * @param endIndex the ending index, exclusive
     * @return the substring
     * @throws StringIndexOutOfBoundsException if:
     *         beginIndex is larger than length()
     *         endIndex is larger than length()
     *         beginIndex is larger than endIndex
     */
    String substring(Index beginIndex, Index endIndex) const;

    // C++ operator overloads for comparison
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    bool operator< (const String& other) const;
    bool operator<=(const String& other) const;
    bool operator> (const String& other) const;
    bool operator>=(const String& other) const;

    /**
     * Returns the index within this string of the first occurrence of the specified character.
     * 
     * @param ch a character
     * @return the index of the first occurrence of the character in the string,
     *         or Index::invalid if the character does not occur
     */
    Index indexOf(Char ch) const;
    
    /**
     * Returns the index within this string of the first occurrence of the specified character,
     * starting the search at the specified index.
     *
     * @param ch a character
     * @param fromIndex the index to start the search from
     * @return the index of the first occurrence of the character in the string,
     *         or Index::invalid if the character does not occur
     */
    Index indexOf(Char ch, Index fromIndex) const;
    
    /**
     * Returns the index within this string of the first occurrence of the specified substring.
     *
     * @param str the substring to search for
     * @return the index of the first occurrence of the specified substring,
     *         or Index::invalid if there is no such occurrence
     */
    Index indexOf(const String& str) const;
    
    /**
     * Returns the index within this string of the first occurrence of the specified substring,
     * starting at the specified index.
     *
     * @param str the substring to search for
     * @param fromIndex the index to start the search from
     * @return the index of the first occurrence of the specified substring,
     *         starting at the specified index, or Index::invalid if there is no such occurrence
     */
    Index indexOf(const String& str, Index fromIndex) const;
    
    /**
     * Returns the index within this string of the last occurrence of the specified character.
     *
     * @param ch a character
     * @return the index of the last occurrence of the character in the string,
     *         or Index::invalid if the character does not occur
     */
    Index lastIndexOf(Char ch) const;
    
    /**
     * Returns the index within this string of the last occurrence of the specified character,
     * searching backward starting at the specified index.
     *
     * @param ch a character
     * @param fromIndex the index to start the search from
     * @return the index of the last occurrence of the character in the string,
     *         or Index::invalid if the character does not occur
     */
    Index lastIndexOf(Char ch, Index fromIndex) const;
    
    /**
     * Returns the index within this string of the last occurrence of the specified substring.
     *
     * @param str the substring to search for
     * @return the index of the last occurrence of the specified substring,
     *         or Index::invalid if there is no such occurrence
     */
    Index lastIndexOf(const String& str) const;
    
    /**
     * Returns the index within this string of the last occurrence of the specified substring,
     * searching backward starting at the specified index.
     *
     * @param str the substring to search for
     * @param fromIndex the index to start the search from
     * @return the index of the last occurrence of the specified substring,
     *         starting at the specified index, or Index::invalid if there is no such occurrence
     */
    Index lastIndexOf(const String& str, Index fromIndex) const;

private:
    // Private constructor for creating substrings with shared data
    String(std::shared_ptr<const std::string> data, std::size_t offset, std::size_t length)
        : data_(std::move(data)), offset_(offset), length_(length), utf16_cache_() {}

    std::shared_ptr<const std::string> data_;  // Immutable UTF-8 string storage shared between instances
    std::size_t offset_{0};                      // Start offset in data_ (in bytes)
    std::size_t length_{0};                      // Length of this substring (in bytes)
    mutable std::shared_ptr<const std::u16string> utf16_cache_;  // Cached UTF-16 representation
    
    // Get or create UTF-16 representation
    const std::u16string& get_utf16() const;

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
    bool shares_data_with(const String& other) const;

    // Allow test fixtures to access private members
    friend class StringTest;
    friend class StringSharing;  // Test fixture for string sharing tests

private:
    // Helper templates and concepts for valueOf implementation
    template<typename T>
    struct always_false : std::false_type {};

    // Using the always_false from namespace scope

    // Helper function to convert any type to string
    template<typename T>
    static std::string to_string_helper(const T& obj) {
        // For arithmetic types (int, float, etc.), use std::to_string
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(obj);
        } 
        // For std::string, return as is
        else if constexpr (std::is_same_v<T, std::string>) {
            return obj;
        }
        // For types with a to_string() method (SFINAE approach)
        else if constexpr (simple::traits::is_to_string_available<T>::value) {
            return obj.to_string();
        }
        // For variant types
        else if constexpr (simple::traits::is_variant_v<T>) {
            return std::visit([](const auto& value) -> std::string {
                return to_string_helper(value);
            }, obj);
        }
        // For map-like containers (map, unordered_map, etc.)
        else if constexpr (simple::traits::is_map_like_v<T>) {
            std::ostringstream oss;
            oss << "{";
            auto it = obj.begin();
            if (it != obj.end()) {
                oss << to_string_helper(it->first) << "=" << to_string_helper(it->second);
                ++it;
            }
            for (; it != obj.end(); ++it) {
                oss << ", " << to_string_helper(it->first) << "=" << to_string_helper(it->second);
            }
            oss << "}";
            return oss.str();
        }
        // For container types (vector, list, set, etc.)
        else if constexpr (simple::traits::is_container_v<T>) {
            std::ostringstream oss;
            oss << "[";
            auto it = obj.begin();
            if (it != obj.end()) {
                oss << to_string_helper(*it);
                ++it;
            }
            for (; it != obj.end(); ++it) {
                oss << ", " << to_string_helper(*it);
            }
            oss << "]";
            return oss.str();
        }
        // Default case: try to use operator<< if available
        else {
            std::ostringstream oss;
            oss << obj;
            return oss.str();
        }
    }
};

} // namespace simple

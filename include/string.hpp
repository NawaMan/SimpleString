#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <variant>
#include <vector>
#include "compare_result.hpp"
#include "char.hpp"
#include "code_point.hpp"
#include "index.hpp"
#include "encoding.hpp"

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

// Forward declarations of implementation details
// These functions are implemented in string.cpp
void init_locale();

// Forward declaration of the StringImpl class
class StringImpl;


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
 * @brief A simple string class that provides Java String-like functionality
 *
 * This class implements a Java-like immutable string with Unicode support.
 * It provides methods for string manipulation, comparison, and conversion
 * that closely match Java's String class behavior.
 *
 * @section unicode Unicode Handling
 * - Strings are stored internally as UTF-8 for memory efficiency
 * - length() returns the number of UTF-16 code units (like Java's String.length())
 * - Comparison operations use byte-by-byte comparison
 * - Full support for Unicode surrogate pairs and code points
 * 
 * @section normalization Unicode Normalization
 * Different Unicode representations of the same visual character are treated as distinct strings.
 * For example:
 * - U+00E9 (é as single code point)
 * - U+0065 + U+0301 (e + ◌́ as combining character)
 * These will compare as different strings and have different lengths.
 * Future versions may support Unicode normalization for comparing such equivalent forms.
 *
 * @section memory Memory Efficiency
 * The String class uses copy-on-write semantics to efficiently share memory between
 * string instances. Substrings share the same underlying data without copying.
 */

/**
 * @brief Exception thrown when a string index is out of range
 *
 * This exception is thrown when an operation attempts to access a character
 * at an index that is negative or beyond the end of the string.
 */
class StringIndexOutOfBoundsException : public std::out_of_range {
public:
    /**
     * @brief Constructs a StringIndexOutOfBoundsException with the specified message
     * @param msg The error message
     */
    explicit StringIndexOutOfBoundsException(const std::string& msg)
        : std::out_of_range(msg) {}
};

class String {
public:
    /**
     * @name valueOf
     * @brief Static methods to convert primitive types to String
     *
     * These methods convert various primitive types to their string representation.
     * @{
     */
    
    /**
     * @brief Converts a boolean to a String
     * @param b The boolean value to convert
     * @return "true" or "false"
     */
    static String valueOf(bool b);
    
    /**
     * @brief Converts a character to a String
     * @param c The character to convert
     * @return A String containing the single character
     */
    static String valueOf(char c);
    
    /**
     * @brief Converts an integer to a String
     * @param i The integer to convert
     * @return The string representation of the integer
     */
    static String valueOf(int i);
    
    /**
     * @brief Converts a long to a String
     * @param l The long value to convert
     * @return The string representation of the long
     */
    static String valueOf(long l);
    
    /**
     * @brief Converts a float to a String
     * @param f The float value to convert
     * @return The string representation of the float ("NaN", "Infinity", or "-Infinity" for special values)
     */
    static String valueOf(float f);
    
    /**
     * @brief Converts a double to a String
     * @param d The double value to convert
     * @return The string representation of the double ("NaN", "Infinity", or "-Infinity" for special values)
     */
    static String valueOf(double d);
    
    /** @} */
    
    /**
     * @brief Generic valueOf method for any type that can be converted to string
     *
     * This template method can convert any type to a String if it:
     * - Is an arithmetic type (uses std::to_string)
     * - Is a std::string (uses direct conversion)
     * - Has a to_string() method
     * - Is a container or map-like type (creates a string representation)
     * - Supports operator<< with std::ostringstream
     *
     * @tparam T The type to convert
     * @param obj The object to convert to a String
     * @return A String representation of the object
     */
    template<typename T>
    static String valueOf(const T& obj) {
        return String(to_string_helper(obj));
    }
    
    // Default constructor - creates an empty string
    String();
    
    /**
     * @brief Constructor from C++ string literal
     * @param str The std::string to convert
     */
    explicit String(const std::string& str);
    
    /**
     * @brief Constructor from C string with explicit length
     * 
     * This constructor allows creating strings with embedded null characters.
     *
     * @param str The C string to convert
     * @param length The number of characters to use from the C string
     */
    String(const char* str, std::size_t length);
    
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

    /**
     * Returns a string resulting from replacing all occurrences of oldChar in this
     * string with newChar.
     *
     * @param oldChar the old character
     * @param newChar the new character
     * @return a string derived from this string by replacing every occurrence of oldChar with newChar
     */
    String replace(Char oldChar, Char newChar) const;
    
    /**
     * Returns a string resulting from replacing all occurrences of the target substring with
     * the replacement substring.
     *
     * @param target the sequence of char values to be replaced
     * @param replacement the replacement sequence of char values
     * @return a string derived from this string by replacing every occurrence of target with replacement
     */
    String replace(const String& target, const String& replacement) const;

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

    /**
     * Returns true if and only if this string contains the specified sequence of char values.
     * 
     * @param str the sequence to search for
     * @return true if this string contains the specified sequence, false otherwise
     */
    bool contains(const String& str) const;
    
    /**
     * Tests if this string starts with the specified prefix.
     * 
     * @param prefix the prefix
     * @return true if the character sequence represented by the argument is a prefix of the
     *         character sequence represented by this string; false otherwise
     */
    bool startsWith(const String& prefix) const;
    
    /**
     * Tests if the substring of this string beginning at the specified index starts with the specified prefix.
     * 
     * @param prefix the prefix
     * @param offset where to begin looking in this string
     * @return true if the character sequence represented by the argument is a prefix of the
     *         substring of this object starting at index offset; false otherwise
     * @throws StringIndexOutOfBoundsException if offset is negative or greater than the length of this string
     */
    bool startsWith(const String& prefix, Index offset) const;
    
    /**
     * Tests if this string ends with the specified suffix.
     * 
     * @param suffix the suffix
     * @return true if the character sequence represented by the argument is a suffix of the
     *         character sequence represented by this object; false otherwise
     */
    bool endsWith(const String& suffix) const;

    /**
     * Returns a string with all leading and trailing ASCII whitespace removed.
     * This method is equivalent to Java's String.trim() method.
     * 
     * @return A new string with leading and trailing ASCII whitespace removed
     */
    String trim() const;

    /**
     * Returns a string with all leading and trailing Unicode whitespace removed.
     * This method is equivalent to Java 11's String.strip() method.
     * 
     * @return A new string with leading and trailing Unicode whitespace removed
     */
    String strip() const;

    /**
     * Returns a string with all leading Unicode whitespace removed.
     * This method is equivalent to Java 11's String.stripLeading() method.
     * 
     * @return A new string with leading Unicode whitespace removed
     */
    String stripLeading() const;

    /**
     * Returns a string with all trailing Unicode whitespace removed.
     * This method is equivalent to Java 11's String.stripTrailing() method.
     * 
     * @return A new string with trailing Unicode whitespace removed
     */
    String stripTrailing() const;

    /**
     * Returns true if this string has no leading or trailing whitespace.
     * Useful for optimization and testing.
     *
     * @return true if the string has no leading or trailing whitespace
     */
    bool isStripped() const;

    /**
     * Returns a byte array representation of this string in the specified encoding.
     * 
     * @param encoding the encoding to use, defaults to UTF_8
     * @param errorHandling the error handling strategy to use, defaults to THROW
     * @return a byte array containing the string encoded in the specified encoding
     * @throws EncodingException if the string cannot be encoded in the specified encoding
     *         and the error handling strategy is THROW
     */
    std::vector<uint8_t> getBytes(Encoding encoding = Encoding::UTF_8, 
                                 EncodingErrorHandling errorHandling = EncodingErrorHandling::THROW) const;

    /**
     * Returns a byte array representation of this string in the specified encoding,
     * with control over Byte Order Mark (BOM) handling.
     * 
     * @param encoding the encoding to use
     * @param bomPolicy the BOM policy to use
     * @param errorHandling the error handling strategy to use, defaults to THROW
     * @return a byte array containing the string encoded in the specified encoding
     * @throws EncodingException if the string cannot be encoded in the specified encoding
     *         and the error handling strategy is THROW
     */
    std::vector<uint8_t> getBytes(Encoding encoding, BOMPolicy bomPolicy, 
                                 EncodingErrorHandling errorHandling = EncodingErrorHandling::THROW) const;

    /**
     * Returns a standard C++ string representation of this string.
     * This is a convenience method that uses UTF-8 encoding.
     * 
     * @return a standard C++ string containing this string encoded as UTF-8
     */
    std::string toStdString() const;

    /**
     * Creates a new String from a byte array using the specified encoding.
     * 
     * @param bytes the byte array to decode
     * @param encoding the encoding to use, defaults to UTF_8
     * @param errorHandling the error handling strategy to use, defaults to THROW
     * @return a new String created from the byte array
     * @throws EncodingException if the byte array cannot be decoded using the specified encoding
     *         and the error handling strategy is THROW
     */
    static String fromBytes(const std::vector<uint8_t>& bytes, 
                           Encoding encoding = Encoding::UTF_8,
                           EncodingErrorHandling errorHandling = EncodingErrorHandling::THROW);

    /**
     * Creates a new String from a byte array using the specified encoding,
     * with control over Byte Order Mark (BOM) handling.
     * 
     * @param bytes the byte array to decode
     * @param encoding the encoding to use
     * @param bomPolicy the BOM policy to use
     * @param errorHandling the error handling strategy to use, defaults to THROW
     * @return a new String created from the byte array
     * @throws EncodingException if the byte array cannot be decoded using the specified encoding
     *         and the error handling strategy is THROW
     */
    static String fromBytes(const std::vector<uint8_t>& bytes, 
                           Encoding encoding, 
                           BOMPolicy bomPolicy,
                           EncodingErrorHandling errorHandling = EncodingErrorHandling::THROW);

    /**
     * Creates a new String from a standard C++ string.
     * This is a convenience method that assumes UTF-8 encoding.
     * 
     * @param str the standard C++ string to convert
     * @return a new String created from the standard C++ string
     */
    static String fromStdString(const std::string& str);

private:
    /**
     * @brief Private constructor for creating substrings with shared data
     *
     * This constructor is used internally to create substrings that share
     * the same underlying data as the original string, avoiding unnecessary copying.
     *
     * @param data Shared pointer to the underlying string data
     * @param offset Start offset in the data (in bytes)
     * @param length Length of the substring (in bytes)
     */
    String(std::shared_ptr<const std::string> data, std::size_t offset, std::size_t length);

    // Implementation details are hidden using the PIMPL idiom
    std::shared_ptr<detail::StringImpl> pimpl_;
    
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
    /**
     * @brief Helper metafunction that is always false regardless of template parameter
     * 
     * Used for static_assert failures in template specializations
     */
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

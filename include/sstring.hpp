#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP

#include <string>
#include <memory>
#include <stdexcept>
#include <boost/locale.hpp>
#include "compare_result.hpp"
#include "char.hpp"

namespace simple_string {

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
    explicit SString(const std::string& str);
    
    // Constructor from C string with explicit length (for strings with null chars)
    SString(const char* str, std::size_t length);
    
    // Get the length of the string
    std::size_t length() const;

    /**
     * Returns true if this string is empty (length() == 0).
     * Note: A string containing only whitespace or null characters is not considered empty.
     * 
     * @return true if the string is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * Compares this string with another for equality using byte-by-byte comparison.
     * Note: Characters with different Unicode representations (e.g., é vs e + ◌́)
     * will not be considered equal even if they appear visually identical.
     * 
     * @param other The string to compare with
     * @return true if the strings are exactly equal, false otherwise
     */
    bool equals(const SString& other) const;

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
    CompareResult compareTo(const SString& other) const;

    /**
     * Returns the character at the specified index as a Char.
     * The index refers to UTF-16 code units, not bytes or code points.
     * 
     * @param index Zero-based index of the character
     * @return The character at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    Char charAt(std::size_t index) const;

    /**
     * Array-style access to characters. Equivalent to charAt().
     * 
     * @param index Zero-based index of the character
     * @return The character at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    Char operator[](std::size_t index) const { return charAt(index); }

    /**
     * Returns the raw UTF-16 code unit at the specified index.
     * 
     * @param index Zero-based index of the code unit
     * @return The UTF-16 code unit at the specified index
     * @throws StringIndexOutOfBoundsException if index is negative or >= length()
     */
    char16_t charValue(std::size_t index) const;

    // Get the underlying string data
    const std::string& toString() const { return *data_; }

    // C++ operator overloads for comparison
    bool operator==(const SString& other) const { return  equals(other);                      }
    bool operator!=(const SString& other) const { return !equals(other);                      }
    bool operator< (const SString& other) const { return compareTo(other).isLess();           }
    bool operator<=(const SString& other) const { return compareTo(other).isLessOrEqual();    }
    bool operator> (const SString& other) const { return compareTo(other).isGreater();        }
    bool operator>=(const SString& other) const { return compareTo(other).isGreaterOrEqual(); }

private:
    std::shared_ptr<const std::string> data_;  // Immutable string storage shared between instances

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
    bool sharesDataWith(const SString& other) const { return data_ == other.data_; }

    // Allow test fixtures to access private members
    friend class SStringTest;
    friend class SStringSharing;  // Test fixture for string sharing tests
};

} // namespace simple_string

#endif // SIMPLE_STRING_HPP

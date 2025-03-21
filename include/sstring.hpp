#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP

#include <string>
#include <boost/locale.hpp>

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

class SString {
public:
    // Constructor from C++ string literal
    explicit SString(const std::string& str);
    
    // Constructor from C string with explicit length (for strings with null chars)
    SString(const char* str, std::size_t length);
    
    // Get the length of the string
    std::size_t length() const;

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
     * @return negative if this < other, 0 if equal, positive if this > other
     */
    int compareTo(const SString& other) const;

    // Get the underlying string data
    const std::string& toString() const { return data_; }

    // C++ operator overloads for comparison
    bool operator==(const SString& other) const { return  equals(other);        }
    bool operator!=(const SString& other) const { return !equals(other);        }
    bool operator< (const SString& other) const { return compareTo(other) <  0; }
    bool operator<=(const SString& other) const { return compareTo(other) <= 0; }
    bool operator> (const SString& other) const { return compareTo(other) >  0; }
    bool operator>=(const SString& other) const { return compareTo(other) >= 0; }

private:
    const std::string data_;  // Truly immutable internal string storage - const prevents any modification after construction
};

} // namespace simple_string

#endif // SIMPLE_STRING_HPP

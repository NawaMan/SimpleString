#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP

#include <string>
#include <boost/locale.hpp>

namespace simple_string {

class SString {
public:
    // Constructor from C++ string literal
    explicit SString(const std::string& str);
    
    // Constructor from C string with explicit length (for strings with null chars)
    SString(const char* str, std::size_t length);
    
    // Get the length of the string
    std::size_t length() const;

    // Comparison methods (Java-style)
    bool equals   (const SString& other) const;
    int  compareTo(const SString& other) const;

    // Get the underlying string data
    const std::string& toString() const { return data_; }

    // C++ operator overloads for comparison
    bool operator==(const SString& other) const { return equals(other);         }
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

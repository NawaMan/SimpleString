#include "sstring.hpp"
#include <boost/locale/encoding.hpp>
#include <sstream>

namespace simple_string {

auto SString::char_at(std::size_t index) const -> Char {
    // Get UTF-16 representation (uses cache)
    const std::u16string& utf16 = get_utf16();
    
    // Check bounds (length in UTF-16 code units)
    if (index >= utf16.length()) {
        std::ostringstream oss;
        oss << "String index " << index << " out of bounds for length " << utf16.length();
        throw StringIndexOutOfBoundsException(oss.str());
    }
    
    // Get UTF-16 code unit at index
    return Char(utf16[index]);
}

auto SString::char_value(std::size_t index) const -> char16_t {
    return char_at(index).value();
}

} // namespace simple_string

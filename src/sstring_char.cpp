#include "sstring.hpp"
#include <boost/locale/encoding.hpp>
#include <sstream>

namespace simple_string {

Char SString::charAt(std::size_t index) const {
    // Get UTF-16 representation (uses cache)
    const std::u16string& utf16 = getUTF16();
    
    // Check bounds (length in UTF-16 code units)
    if (index >= utf16.length()) {
        std::ostringstream oss;
        oss << "String index " << index << " out of bounds for length " << utf16.length();
        throw StringIndexOutOfBoundsException(oss.str());
    }
    
    // Get UTF-16 code unit at index
    return Char(utf16[index]);
}

char16_t SString::charValue(std::size_t index) const {
    return charAt(index).value();
}

} // namespace simple_string

#include "sstring.hpp"
#include <boost/locale/encoding.hpp>
#include <sstream>

namespace simple_string {

Char SString::charAt(std::size_t index) const {
    // Convert UTF-8 to UTF-16
    std::u16string utf16 = boost::locale::conv::utf_to_utf<char16_t>(*data_);
    
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

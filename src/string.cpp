#include "../include/string.hpp"
#include <boost/locale/encoding.hpp>

namespace simple {

auto String::char_at(std::size_t index) const -> Char {
    const auto& utf16 = get_utf16();
    if (index >= utf16.length()) {
        throw StringIndexOutOfBoundsException("Index out of bounds");
    }
    return Char(utf16[index]);
}

auto String::char_value(std::size_t index) const -> char16_t {
    return char_at(index).value();
}

} // namespace simple

#include "../include/string.hpp"
#include <boost/locale/encoding.hpp>
#include <cmath>

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

// Implementation of valueOf methods for primitive types
String String::valueOf(bool b) {
    return b ? String("true") : String("false");
}

String String::valueOf(char c) {
    return String(std::string(1, c));
}

String String::valueOf(int i) {
    return String(std::to_string(i));
}

String String::valueOf(long l) {
    return String(std::to_string(l));
}

String String::valueOf(float f) {
    if (std::isnan(f)) {
        return String("NaN");
    } else if (std::isinf(f)) {
        return f > 0 ? String("Infinity") : String("-Infinity");
    }
    return String(std::to_string(f));
}

String String::valueOf(double d) {
    if (std::isnan(d)) {
        return String("NaN");
    } else if (std::isinf(d)) {
        return d > 0 ? String("Infinity") : String("-Infinity");
    }
    return String(std::to_string(d));
}

} // namespace simple

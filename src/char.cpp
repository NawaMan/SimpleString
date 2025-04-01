#include "../include/char.hpp"

namespace simple {

char32_t Char::to_code_point(Char low_surrogate) const noexcept {
    if (!is_high_surrogate() || !low_surrogate.is_low_surrogate()) {
        return INVALID_CODEPOINT;
    }
    return 0x10000 + ((value_ - 0xD800) << 10) + (low_surrogate.value() - 0xDC00);
}

std::u16string Char::to_string() const {
    return std::u16string(1, value_);
}

} // namespace simple

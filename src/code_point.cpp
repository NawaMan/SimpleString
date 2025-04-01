#include "../include/code_point.hpp"

namespace simple {

char32_t CodePoint::high_surrogate() const noexcept {
    if (is_surrogate_pair() && value_ >= 0xD800 && value_ <= 0xDBFF) {
        return value_;  // High surrogate in the range [0xD800, 0xDBFF]
    }
    return 0;  // Not a valid high surrogate
}

char32_t CodePoint::low_surrogate() const noexcept {
    if (is_surrogate_pair() && value_ >= 0xDC00 && value_ <= 0xDFFF) {
        return value_;  // Low surrogate in the range [0xDC00, 0xDFFF]
    }
    return 0;  // Not a valid low surrogate
}

} // namespace simple

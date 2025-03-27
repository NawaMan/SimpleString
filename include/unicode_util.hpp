#pragma once

namespace simple {

/**
 * @brief Utility functions for Unicode code point and surrogate pair operations
 */
class UnicodeUtil {
public:
    /**
     * @brief Calculates the high surrogate for a supplementary code point
     * @param code_point The Unicode code point to get high surrogate for
     * @return The high surrogate value as char16_t
     */
    static constexpr char16_t high_surrogate_of(char32_t code_point) noexcept {
        return static_cast<char16_t>(((code_point - 0x10000) >> 10) + 0xD800);
    }
    
    /**
     * @brief Calculates the low surrogate for a supplementary code point
     * @param code_point The Unicode code point to get low surrogate for
     * @return The low surrogate value as char16_t
     */
    static constexpr char16_t low_surrogate_of(char32_t code_point) noexcept {
        return static_cast<char16_t>(((code_point - 0x10000) & 0x3FF) + 0xDC00);
    }
    
    /**
     * @brief Checks if a code point is in the supplementary plane
     * @param code_point The Unicode code point to check
     * @return true if the code point is supplementary (>= 0x10000 and <= 0x10FFFF)
     */
    static constexpr bool is_supplementary_code_point(char32_t code_point) noexcept {
        return code_point >= 0x10000 && code_point <= 0x10FFFF;
    }
};

} // namespace simple

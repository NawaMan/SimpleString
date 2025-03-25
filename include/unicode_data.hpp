#pragma once
#include <array>
#include <cstdint>
#include "unicode_category_enum.hpp"

namespace simple_string {
namespace unicode_data {

struct CodePointRange {
    char32_t start;
    char32_t end;
    UnicodeCategoryEnum category;
};

// This is just a small subset of the Unicode data for common ranges
// A complete implementation would need the full Unicode Character Database
constexpr std::array<CodePointRange, 32> UNICODE_RANGES {{
    // Math symbols
    {0x002B, 0x002B, UnicodeCategoryEnum::MATH_SYMBOL},      // +
    {0x003C, 0x003E, UnicodeCategoryEnum::MATH_SYMBOL},      // <=>    
    
    // Currency symbols
    {0x0024, 0x0024, UnicodeCategoryEnum::CURRENCY_SYMBOL},  // $
    {0x00A2, 0x00A5, UnicodeCategoryEnum::CURRENCY_SYMBOL},  // ¢-¥
    
    // ASCII Letters
    {0x0041, 0x005A, UnicodeCategoryEnum::UPPERCASE_LETTER}, // A-Z
    {0x0061, 0x007A, UnicodeCategoryEnum::LOWERCASE_LETTER}, // a-z
    
    // ASCII Numbers
    {0x0030, 0x0039, UnicodeCategoryEnum::DECIMAL_NUMBER},   // 0-9
    
    // ASCII Punctuation
    {0x0021, 0x0023, UnicodeCategoryEnum::OTHER_PUNCTUATION},// !"#
    {0x0025, 0x0029, UnicodeCategoryEnum::OTHER_PUNCTUATION},// %&'()
    {0x002A, 0x002A, UnicodeCategoryEnum::OTHER_PUNCTUATION},// *
    {0x002C, 0x002F, UnicodeCategoryEnum::OTHER_PUNCTUATION},// ,-./
    {0x003A, 0x003B, UnicodeCategoryEnum::OTHER_PUNCTUATION},// :;
    {0x003F, 0x0040, UnicodeCategoryEnum::OTHER_PUNCTUATION},// ?@
    {0x005B, 0x0060, UnicodeCategoryEnum::OTHER_PUNCTUATION},// [\]^_`
    {0x007B, 0x007E, UnicodeCategoryEnum::OTHER_PUNCTUATION},// {|}~
    
    // ASCII Control
    {0x0000, 0x001F, UnicodeCategoryEnum::CONTROL},         // C0 controls
    {0x007F, 0x007F, UnicodeCategoryEnum::CONTROL},         // DEL
    
    // Whitespace
    {0x0020, 0x0020, UnicodeCategoryEnum::SPACE_SEPARATOR}, // SPACE
    {0x000A, 0x000D, UnicodeCategoryEnum::CONTROL},         // CR, LF, etc.
    {0x0009, 0x0009, UnicodeCategoryEnum::CONTROL},         // TAB
    
    // Latin-1 Supplement
    {0x00C0, 0x00D6, UnicodeCategoryEnum::UPPERCASE_LETTER}, // À-Ö
    {0x00D8, 0x00DE, UnicodeCategoryEnum::UPPERCASE_LETTER}, // Ø-Þ
    {0x00DF, 0x00F6, UnicodeCategoryEnum::LOWERCASE_LETTER}, // ß-ö
    {0x00F8, 0x00FF, UnicodeCategoryEnum::LOWERCASE_LETTER}, // ø-ÿ
    
    // Common punctuation
    {0x00A1, 0x00BF, UnicodeCategoryEnum::OTHER_PUNCTUATION}, // ¡-¿
    
    // Surrogates
    {0xD800, 0xDBFF, UnicodeCategoryEnum::SURROGATE},        // High surrogates
    {0xDC00, 0xDFFF, UnicodeCategoryEnum::SURROGATE},        // Low surrogates
    
    // Private Use Area
    {0xE000, 0xF8FF, UnicodeCategoryEnum::PRIVATE_USE},      // Private Use Area
    
    // Common CJK ranges (simplified)
    {0x4E00, 0x9FFF, UnicodeCategoryEnum::OTHER_LETTER},     // CJK Unified Ideographs
    
    // Unknown (default)
    {0x0000, 0x10FFFF, UnicodeCategoryEnum::UNKNOWN},         // Catch-all
    
    // Unassigned (example range)
    {0x10000, 0x10FFFF, UnicodeCategoryEnum::UNASSIGNED}    // Beyond BMP
}};

} // namespace unicode_data
} // namespace simple_string

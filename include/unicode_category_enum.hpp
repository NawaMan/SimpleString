#pragma once
#include <cstdint>

namespace simple {

/**
 * @brief Enumeration of Unicode General Categories
 * 
 * This enumeration represents the standard Unicode General Categories as defined in
 * the Unicode Character Database. Each value corresponds to a specific category that
 * describes the primary classification of a Unicode character.
 */
enum class UnicodeCategoryEnum : std::uint8_t {
    // Letters (L)
    UPPERCASE_LETTER     = 1,   // Lu: 'A', 'B', 'C'...
    LOWERCASE_LETTER     = 2,   // Ll: 'a', 'b', 'c'...
    TITLECASE_LETTER     = 3,   // Lt: 'ǅ', 'ǈ'...
    MODIFIER_LETTER      = 4,   // Lm: modifier letters
    OTHER_LETTER         = 5,   // Lo: ideographs, syllables
    
    // Marks (M)
    NON_SPACING_MARK     = 6,   // Mn: accent marks, tone marks
    SPACING_MARK         = 7,   // Mc: vowel signs
    ENCLOSING_MARK       = 8,   // Me: circle, square
    
    // Numbers (N)
    DECIMAL_NUMBER       = 9,   // Nd: '0'-'9'
    LETTER_NUMBER        = 10,  // Nl: roman numerals
    OTHER_NUMBER         = 11,  // No: fractions, superscript numbers
    CONNECTOR_PUNCTUATION= 12,  // Pc: '_'
    DASH_PUNCTUATION     = 13,  // Pd: '-'
    OPEN_PUNCTUATION     = 14,  // Ps: '(', '[', '{'
    CLOSE_PUNCTUATION    = 15,  // Pe: ')', ']', '}'
    INITIAL_PUNCTUATION  = 16,  // Pi: left quotes
    FINAL_PUNCTUATION    = 17,  // Pf: right quotes
    OTHER_PUNCTUATION    = 18,  // Po: '!', '.', ','
    
    // Symbols (S)
    MATH_SYMBOL          = 19,  // Sm: '+', '<', '>'
    CURRENCY_SYMBOL      = 20,  // Sc: '$', '€'
    MODIFIER_SYMBOL      = 21,  // Sk: circumflex, grave
    OTHER_SYMBOL         = 22,  // So: copyright, trademark
    
    // Separators (Z)
    SPACE_SEPARATOR      = 23,  // Zs: normal space
    LINE_SEPARATOR       = 24,  // Zl: line separator
    PARAGRAPH_SEPARATOR  = 25,  // Zp: paragraph separator
    
    // Control (C)
    CONTROL             = 26,  // Cc: control characters
    FORMAT              = 27,  // Cf: format controls
    SURROGATE          = 28,  // Cs: surrogates
    PRIVATE_USE        = 29,  // Co: private use
    UNASSIGNED         = 30,  // Cn: not assigned
    
    // Special cases
    UNKNOWN            = 0    // Not a valid Unicode category
};

} // namespace simple

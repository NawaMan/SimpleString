#include "../include/unicode_category.hpp"

namespace simple {

bool UnicodeCategory::is_letter() const noexcept {
    return is_uppercase_letter()
        || is_lowercase_letter()
        || is_titlecase_letter()
        || is_modifier_letter()
        || is_other_letter();
}

bool UnicodeCategory::is_mark() const noexcept {
    return is_non_spacing_mark()
        || is_spacing_mark()
        || is_enclosing_mark();
}

bool UnicodeCategory::is_number() const noexcept {
    return is_decimal_number()
        || is_letter_number()
        || is_other_number();
}

bool UnicodeCategory::is_punctuation() const noexcept {
    return is_connector_punctuation()
        || is_dash_punctuation()
        || is_open_punctuation()
        || is_close_punctuation()
        || is_initial_punctuation()
        || is_final_punctuation()
        || is_other_punctuation();
}

bool UnicodeCategory::is_symbol() const noexcept {
    return is_math_symbol()
        || is_currency_symbol()
        || is_modifier_symbol()
        || is_other_symbol();
}

bool UnicodeCategory::is_separator() const noexcept {
    return is_space_separator()
        || is_line_separator()
        || is_paragraph_separator();
}

bool UnicodeCategory::is_control_or_format() const noexcept {
    return is_control()
        || is_format()
        || is_surrogate()
        || is_private_use()
        || is_unassigned();
}

bool UnicodeCategory::is_whitespace() const noexcept {
    return is_space_separator()
        || is_line_separator()
        || is_paragraph_separator()
        || (is_control() 
         && (value_ == simple::UnicodeCategoryEnum::FORMAT 
          || value_ == simple::UnicodeCategoryEnum::CONTROL));
}

bool UnicodeCategory::is_printable() const noexcept {
    return !is_control()
        && !is_surrogate()
        && !is_unassigned()
        && !is_private_use();
}

bool UnicodeCategory::is_case_ignorable() const noexcept {
    return is_mark()
        || is_format()
        || is_connector_punctuation()
        || is_dash_punctuation();
}

std::string_view UnicodeCategory::name() const noexcept {
    switch (value_) {
        case simple::UnicodeCategoryEnum::UPPERCASE_LETTER:      return "Uppercase Letter";
        case simple::UnicodeCategoryEnum::LOWERCASE_LETTER:      return "Lowercase Letter";
        case simple::UnicodeCategoryEnum::TITLECASE_LETTER:      return "Titlecase Letter";
        case simple::UnicodeCategoryEnum::MODIFIER_LETTER:       return "Modifier Letter";
        case simple::UnicodeCategoryEnum::OTHER_LETTER:          return "Other Letter";
        case simple::UnicodeCategoryEnum::NON_SPACING_MARK:      return "Non-spacing Mark";
        case simple::UnicodeCategoryEnum::SPACING_MARK:          return "Spacing Mark";
        case simple::UnicodeCategoryEnum::ENCLOSING_MARK:        return "Enclosing Mark";
        case simple::UnicodeCategoryEnum::DECIMAL_NUMBER:        return "Decimal Number";
        case simple::UnicodeCategoryEnum::LETTER_NUMBER:         return "Letter Number";
        case simple::UnicodeCategoryEnum::OTHER_NUMBER:          return "Other Number";
        case simple::UnicodeCategoryEnum::CONNECTOR_PUNCTUATION: return "Connector Punctuation";
        case simple::UnicodeCategoryEnum::DASH_PUNCTUATION:      return "Dash Punctuation";
        case simple::UnicodeCategoryEnum::OPEN_PUNCTUATION:      return "Open Punctuation";
        case simple::UnicodeCategoryEnum::CLOSE_PUNCTUATION:     return "Close Punctuation";
        case simple::UnicodeCategoryEnum::INITIAL_PUNCTUATION:   return "Initial Punctuation";
        case simple::UnicodeCategoryEnum::FINAL_PUNCTUATION:     return "Final Punctuation";
        case simple::UnicodeCategoryEnum::OTHER_PUNCTUATION:     return "Other Punctuation";
        case simple::UnicodeCategoryEnum::MATH_SYMBOL:           return "Math Symbol";
        case simple::UnicodeCategoryEnum::CURRENCY_SYMBOL:       return "Currency Symbol";
        case simple::UnicodeCategoryEnum::MODIFIER_SYMBOL:       return "Modifier Symbol";
        case simple::UnicodeCategoryEnum::OTHER_SYMBOL:          return "Other Symbol";
        case simple::UnicodeCategoryEnum::SPACE_SEPARATOR:       return "Space Separator";
        case simple::UnicodeCategoryEnum::LINE_SEPARATOR:        return "Line Separator";
        case simple::UnicodeCategoryEnum::PARAGRAPH_SEPARATOR:    return "Paragraph Separator";
        case simple::UnicodeCategoryEnum::CONTROL:               return "Control";
        case simple::UnicodeCategoryEnum::FORMAT:                return "Format";
        case simple::UnicodeCategoryEnum::SURROGATE:             return "Surrogate";
        case simple::UnicodeCategoryEnum::PRIVATE_USE:           return "Private Use";
        case simple::UnicodeCategoryEnum::UNASSIGNED:            return "Unassigned";
        case simple::UnicodeCategoryEnum::UNKNOWN:               return "Unknown";
        default:                                         return "Invalid";
    }
}

UnicodeCategory UnicodeCategory::from_code_point(char32_t code_point) noexcept {
    for (const auto& range : unicode_data::UNICODE_RANGES) {
        if (code_point >= range.start && code_point <= range.end) {
            return UnicodeCategory(range.category);
        }
    }
    return unknown();
}

} // namespace simple

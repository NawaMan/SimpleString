#pragma once
#include <cstdint>
#include <string_view>
#include "unicode_category_enum.hpp"
#include "unicode_data.hpp"

namespace simple_string {

/**
 * @brief A class representing Unicode character categories with category testing methods
 *
 * This class provides a comprehensive interface for working with Unicode character
 * categories. It includes methods to test characters for specific categories (e.g.,
 * letters, numbers, symbols) as well as broader properties (e.g., whitespace,
 * alphanumeric). All methods are constexpr and noexcept for compile-time usage.
 *
 * Example usage:
 * @code
 * auto cat = UnicodeCategory::from_code_point(U'A');
 * if (cat.is_uppercase_letter()) {
 *     // Handle uppercase letter
 * }
 * if (cat.is_alphanumeric()) {
 *     // Handle letter or number
 * }
 * std::cout << cat.name(); // prints "Uppercase Letter"
 * @endcode
 */
class UnicodeCategory final {
public:
    /**
     * @brief Constructs a UnicodeCategory from a UnicodeCategoryEnum value
     * @param value The Unicode category enumeration value
     */
    constexpr explicit UnicodeCategory(UnicodeCategoryEnum value) noexcept : value_(value) {}

    // Category check methods
    constexpr bool is_uppercase_letter() const noexcept { return value_ == UnicodeCategoryEnum::UPPERCASE_LETTER; }
    constexpr bool is_lowercase_letter() const noexcept { return value_ == UnicodeCategoryEnum::LOWERCASE_LETTER; }
    constexpr bool is_titlecase_letter() const noexcept { return value_ == UnicodeCategoryEnum::TITLECASE_LETTER; }
    constexpr bool is_modifier_letter()  const noexcept { return value_ == UnicodeCategoryEnum::MODIFIER_LETTER; }
    constexpr bool is_other_letter()     const noexcept { return value_ == UnicodeCategoryEnum::OTHER_LETTER; }

    constexpr bool is_non_spacing_mark() const noexcept { return value_ == UnicodeCategoryEnum::NON_SPACING_MARK; }
    constexpr bool is_spacing_mark()     const noexcept { return value_ == UnicodeCategoryEnum::SPACING_MARK; }
    constexpr bool is_enclosing_mark()   const noexcept { return value_ == UnicodeCategoryEnum::ENCLOSING_MARK; }

    constexpr bool is_decimal_number() const noexcept { return value_ == UnicodeCategoryEnum::DECIMAL_NUMBER; }
    constexpr bool is_letter_number()  const noexcept { return value_ == UnicodeCategoryEnum::LETTER_NUMBER; }
    constexpr bool is_other_number()   const noexcept { return value_ == UnicodeCategoryEnum::OTHER_NUMBER; }

    constexpr bool is_connector_punctuation() const noexcept { return value_ == UnicodeCategoryEnum::CONNECTOR_PUNCTUATION; }
    constexpr bool is_dash_punctuation()      const noexcept { return value_ == UnicodeCategoryEnum::DASH_PUNCTUATION; }
    constexpr bool is_open_punctuation()      const noexcept { return value_ == UnicodeCategoryEnum::OPEN_PUNCTUATION; }
    constexpr bool is_close_punctuation()     const noexcept { return value_ == UnicodeCategoryEnum::CLOSE_PUNCTUATION; }
    constexpr bool is_initial_punctuation()   const noexcept { return value_ == UnicodeCategoryEnum::INITIAL_PUNCTUATION; }
    constexpr bool is_final_punctuation()     const noexcept { return value_ == UnicodeCategoryEnum::FINAL_PUNCTUATION; }
    constexpr bool is_other_punctuation()     const noexcept { return value_ == UnicodeCategoryEnum::OTHER_PUNCTUATION; }

    constexpr bool is_math_symbol()     const noexcept { return value_ == UnicodeCategoryEnum::MATH_SYMBOL; }
    constexpr bool is_currency_symbol() const noexcept { return value_ == UnicodeCategoryEnum::CURRENCY_SYMBOL; }
    constexpr bool is_modifier_symbol() const noexcept { return value_ == UnicodeCategoryEnum::MODIFIER_SYMBOL; }
    constexpr bool is_other_symbol()    const noexcept { return value_ == UnicodeCategoryEnum::OTHER_SYMBOL; }

    constexpr bool is_space_separator()     const noexcept { return value_ == UnicodeCategoryEnum::SPACE_SEPARATOR; }
    constexpr bool is_line_separator()      const noexcept { return value_ == UnicodeCategoryEnum::LINE_SEPARATOR; }
    constexpr bool is_paragraph_separator() const noexcept { return value_ == UnicodeCategoryEnum::PARAGRAPH_SEPARATOR; }

    constexpr bool is_control()     const noexcept { return value_ == UnicodeCategoryEnum::CONTROL; }
    constexpr bool is_format()      const noexcept { return value_ == UnicodeCategoryEnum::FORMAT; }
    constexpr bool is_surrogate()   const noexcept { return value_ == UnicodeCategoryEnum::SURROGATE; }
    constexpr bool is_private_use() const noexcept { return value_ == UnicodeCategoryEnum::PRIVATE_USE; }
    constexpr bool is_unassigned()  const noexcept { return value_ == UnicodeCategoryEnum::UNASSIGNED; }
    constexpr bool is_unknown()     const noexcept { return value_ == UnicodeCategoryEnum::UNKNOWN; }

    // Group check methods
    constexpr bool is_letter() const noexcept {
        return is_uppercase_letter()
            || is_lowercase_letter()
            || is_titlecase_letter()
            || is_modifier_letter()
            || is_other_letter();
    }

    constexpr bool is_mark() const noexcept {
        return is_non_spacing_mark()
            || is_spacing_mark()
            || is_enclosing_mark();
    }

    constexpr bool is_number() const noexcept {
        return is_decimal_number()
            || is_letter_number()
            || is_other_number();
    }

    constexpr bool is_punctuation() const noexcept {
        return is_connector_punctuation()
            || is_dash_punctuation()
            || is_open_punctuation()
            || is_close_punctuation()
            || is_initial_punctuation()
            || is_final_punctuation()
            || is_other_punctuation();
    }

    constexpr bool is_symbol() const noexcept {
        return is_math_symbol()
            || is_currency_symbol()
            || is_modifier_symbol()
            || is_other_symbol();
    }

    constexpr bool is_separator() const noexcept {
        return is_space_separator()
            || is_line_separator()
            || is_paragraph_separator();
    }

    constexpr bool is_control_or_format() const noexcept {
        return is_control()
            || is_format()
            || is_surrogate()
            || is_private_use()
            || is_unassigned();
    }

    // Additional character properties
    /**
     * @brief Checks if the category represents any kind of whitespace
     * @return true if the category is a whitespace category
     */
    constexpr bool is_whitespace() const noexcept {
        return is_space_separator()
            || is_line_separator()
            || is_paragraph_separator()
            || (is_control() && (value_ == UnicodeCategoryEnum::FORMAT || value_ == UnicodeCategoryEnum::CONTROL));
    }

    /**
     * @brief Checks if the category represents a letter or number
     * @return true if the category is a letter or number category
     */
    constexpr bool is_alphanumeric() const noexcept {
        return is_letter() || is_number();
    }

    /**
     * @brief Checks if the category represents a printable character
     * @return true if the category represents a visible character
     */
    constexpr bool is_printable() const noexcept {
        return !is_control()
            && !is_surrogate()
            && !is_unassigned()
            && !is_private_use();
    }

    /**
     * @brief Checks if the category should be ignored in case conversions
     * @return true if the category should be ignored in case conversions
     */
    constexpr bool is_case_ignorable() const noexcept {
        return is_mark()
            || is_format()
            || is_connector_punctuation()
            || is_dash_punctuation();
    }

    // Comparison operators
    constexpr bool operator==(const UnicodeCategory& other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(const UnicodeCategory& other) const noexcept { return value_ != other.value_; }

    // Get category name
    /**
     * @brief Gets a human-readable name for the Unicode category
     * @return A string view containing the category name
     */
    constexpr std::string_view name() const noexcept {
        switch (value_) {
            case UnicodeCategoryEnum::UPPERCASE_LETTER:      return "Uppercase Letter";
            case UnicodeCategoryEnum::LOWERCASE_LETTER:      return "Lowercase Letter";
            case UnicodeCategoryEnum::TITLECASE_LETTER:      return "Titlecase Letter";
            case UnicodeCategoryEnum::MODIFIER_LETTER:       return "Modifier Letter";
            case UnicodeCategoryEnum::OTHER_LETTER:          return "Other Letter";
            case UnicodeCategoryEnum::NON_SPACING_MARK:      return "Non-spacing Mark";
            case UnicodeCategoryEnum::SPACING_MARK:          return "Spacing Mark";
            case UnicodeCategoryEnum::ENCLOSING_MARK:        return "Enclosing Mark";
            case UnicodeCategoryEnum::DECIMAL_NUMBER:        return "Decimal Number";
            case UnicodeCategoryEnum::LETTER_NUMBER:         return "Letter Number";
            case UnicodeCategoryEnum::OTHER_NUMBER:          return "Other Number";
            case UnicodeCategoryEnum::CONNECTOR_PUNCTUATION: return "Connector Punctuation";
            case UnicodeCategoryEnum::DASH_PUNCTUATION:      return "Dash Punctuation";
            case UnicodeCategoryEnum::OPEN_PUNCTUATION:      return "Open Punctuation";
            case UnicodeCategoryEnum::CLOSE_PUNCTUATION:     return "Close Punctuation";
            case UnicodeCategoryEnum::INITIAL_PUNCTUATION:   return "Initial Punctuation";
            case UnicodeCategoryEnum::FINAL_PUNCTUATION:     return "Final Punctuation";
            case UnicodeCategoryEnum::OTHER_PUNCTUATION:     return "Other Punctuation";
            case UnicodeCategoryEnum::MATH_SYMBOL:           return "Math Symbol";
            case UnicodeCategoryEnum::CURRENCY_SYMBOL:       return "Currency Symbol";
            case UnicodeCategoryEnum::MODIFIER_SYMBOL:       return "Modifier Symbol";
            case UnicodeCategoryEnum::OTHER_SYMBOL:          return "Other Symbol";
            case UnicodeCategoryEnum::SPACE_SEPARATOR:       return "Space Separator";
            case UnicodeCategoryEnum::LINE_SEPARATOR:        return "Line Separator";
            case UnicodeCategoryEnum::PARAGRAPH_SEPARATOR:    return "Paragraph Separator";
            case UnicodeCategoryEnum::CONTROL:               return "Control";
            case UnicodeCategoryEnum::FORMAT:                return "Format";
            case UnicodeCategoryEnum::SURROGATE:             return "Surrogate";
            case UnicodeCategoryEnum::PRIVATE_USE:           return "Private Use";
            case UnicodeCategoryEnum::UNASSIGNED:            return "Unassigned";
            case UnicodeCategoryEnum::UNKNOWN:               return "Unknown";
            default:                                         return "Invalid";
        }
    }

    // Create a UnicodeCategory from a code point
    /**
     * @brief Creates a UnicodeCategory from a Unicode code point
     * @param code_point The Unicode code point to categorize
     * @return The UnicodeCategory for the given code point
     */
    static constexpr UnicodeCategory from_code_point(char32_t code_point) noexcept {
        for (const auto& range : unicode_data::UNICODE_RANGES) {
            if (code_point >= range.start && code_point <= range.end) {
                return UnicodeCategory(range.category);
            }
        }
        return unknown();
    }

    // Static instances for each category
    // Letters (L)
    static constexpr UnicodeCategory uppercase_letter() noexcept { return UnicodeCategory{UnicodeCategoryEnum::UPPERCASE_LETTER}; }
    static constexpr UnicodeCategory lowercase_letter() noexcept { return UnicodeCategory{UnicodeCategoryEnum::LOWERCASE_LETTER}; }
    static constexpr UnicodeCategory titlecase_letter() noexcept { return UnicodeCategory{UnicodeCategoryEnum::TITLECASE_LETTER}; }
    static constexpr UnicodeCategory modifier_letter()  noexcept { return UnicodeCategory{UnicodeCategoryEnum::MODIFIER_LETTER}; }
    static constexpr UnicodeCategory other_letter()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::OTHER_LETTER}; }

    // Marks (M)
    static constexpr UnicodeCategory non_spacing_mark() noexcept { return UnicodeCategory{UnicodeCategoryEnum::NON_SPACING_MARK}; }
    static constexpr UnicodeCategory spacing_mark()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::SPACING_MARK}; }
    static constexpr UnicodeCategory enclosing_mark()   noexcept { return UnicodeCategory{UnicodeCategoryEnum::ENCLOSING_MARK}; }

    // Numbers (N)
    static constexpr UnicodeCategory decimal_number()        noexcept { return UnicodeCategory{UnicodeCategoryEnum::DECIMAL_NUMBER}; }
    static constexpr UnicodeCategory letter_number()         noexcept { return UnicodeCategory{UnicodeCategoryEnum::LETTER_NUMBER}; }
    static constexpr UnicodeCategory other_number()          noexcept { return UnicodeCategory{UnicodeCategoryEnum::OTHER_NUMBER}; }
    static constexpr UnicodeCategory connector_punctuation() noexcept { return UnicodeCategory{UnicodeCategoryEnum::CONNECTOR_PUNCTUATION}; }
    static constexpr UnicodeCategory dash_punctuation()      noexcept { return UnicodeCategory{UnicodeCategoryEnum::DASH_PUNCTUATION}; }
    static constexpr UnicodeCategory open_punctuation()      noexcept { return UnicodeCategory{UnicodeCategoryEnum::OPEN_PUNCTUATION}; }
    static constexpr UnicodeCategory close_punctuation()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::CLOSE_PUNCTUATION}; }
    static constexpr UnicodeCategory initial_punctuation()   noexcept { return UnicodeCategory{UnicodeCategoryEnum::INITIAL_PUNCTUATION}; }
    static constexpr UnicodeCategory final_punctuation()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::FINAL_PUNCTUATION}; }
    static constexpr UnicodeCategory other_punctuation()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::OTHER_PUNCTUATION}; }

    // Symbols (S)
    static constexpr UnicodeCategory math_symbol()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::MATH_SYMBOL}; }
    static constexpr UnicodeCategory currency_symbol() noexcept { return UnicodeCategory{UnicodeCategoryEnum::CURRENCY_SYMBOL}; }
    static constexpr UnicodeCategory modifier_symbol() noexcept { return UnicodeCategory{UnicodeCategoryEnum::MODIFIER_SYMBOL}; }
    static constexpr UnicodeCategory other_symbol()    noexcept { return UnicodeCategory{UnicodeCategoryEnum::OTHER_SYMBOL}; }

    // Separators (Z)
    static constexpr UnicodeCategory space_separator()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::SPACE_SEPARATOR}; }
    static constexpr UnicodeCategory line_separator()      noexcept { return UnicodeCategory{UnicodeCategoryEnum::LINE_SEPARATOR}; }
    static constexpr UnicodeCategory paragraph_separator() noexcept { return UnicodeCategory{UnicodeCategoryEnum::PARAGRAPH_SEPARATOR}; }

    // Control (C)
    static constexpr UnicodeCategory control()     noexcept { return UnicodeCategory{UnicodeCategoryEnum::CONTROL}; }
    static constexpr UnicodeCategory format()      noexcept { return UnicodeCategory{UnicodeCategoryEnum::FORMAT}; }
    static constexpr UnicodeCategory surrogate()   noexcept { return UnicodeCategory{UnicodeCategoryEnum::SURROGATE}; }
    static constexpr UnicodeCategory private_use() noexcept { return UnicodeCategory{UnicodeCategoryEnum::PRIVATE_USE}; }
    static constexpr UnicodeCategory unassigned()  noexcept { return UnicodeCategory{UnicodeCategoryEnum::UNASSIGNED}; }

    // Special cases
    static constexpr UnicodeCategory unknown() noexcept { return UnicodeCategory{UnicodeCategoryEnum::UNKNOWN}; }

private:
    UnicodeCategoryEnum value_;

    constexpr UnicodeCategoryEnum value() const noexcept { return value_; }
};


} // namespace simple_string

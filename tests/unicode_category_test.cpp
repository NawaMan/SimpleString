#include <gtest/gtest.h>
#include "../include/unicode_category.hpp"

using namespace simple;

TEST(UnicodeCategoryTest, CategoryChecks) {
    // Test individual category checks
    EXPECT_TRUE(UnicodeCategory::uppercase_letter().is_uppercase_letter());
    EXPECT_FALSE(UnicodeCategory::lowercase_letter().is_uppercase_letter());
    
    EXPECT_TRUE(UnicodeCategory::lowercase_letter().is_lowercase_letter());
    EXPECT_FALSE(UnicodeCategory::uppercase_letter().is_lowercase_letter());
    
    EXPECT_TRUE(UnicodeCategory::decimal_number().is_decimal_number());
    EXPECT_FALSE(UnicodeCategory::letter_number().is_decimal_number());
}

TEST(UnicodeCategoryTest, GroupChecks) {
    // Test group category checks
    EXPECT_TRUE(UnicodeCategory::uppercase_letter().is_letter());
    EXPECT_TRUE(UnicodeCategory::lowercase_letter().is_letter());
    EXPECT_TRUE(UnicodeCategory::titlecase_letter().is_letter());
    EXPECT_FALSE(UnicodeCategory::decimal_number().is_letter());
    
    EXPECT_TRUE(UnicodeCategory::decimal_number().is_number());
    EXPECT_TRUE(UnicodeCategory::letter_number().is_number());
    EXPECT_FALSE(UnicodeCategory::uppercase_letter().is_number());
}

TEST(UnicodeCategoryTest, AdditionalProperties) {
    // Test whitespace
    EXPECT_TRUE(UnicodeCategory::space_separator().is_whitespace());
    EXPECT_TRUE(UnicodeCategory::line_separator().is_whitespace());
    EXPECT_FALSE(UnicodeCategory::uppercase_letter().is_whitespace());
    
    // Test alphanumeric
    EXPECT_TRUE(UnicodeCategory::uppercase_letter().is_alphanumeric());
    EXPECT_TRUE(UnicodeCategory::decimal_number().is_alphanumeric());
    EXPECT_FALSE(UnicodeCategory::math_symbol().is_alphanumeric());
    
    // Test printable
    EXPECT_TRUE(UnicodeCategory::uppercase_letter().is_printable());
    EXPECT_FALSE(UnicodeCategory::control().is_printable());
    EXPECT_FALSE(UnicodeCategory::surrogate().is_printable());
    
    // Test case ignorable
    EXPECT_TRUE(UnicodeCategory::non_spacing_mark().is_case_ignorable());
    EXPECT_TRUE(UnicodeCategory::format().is_case_ignorable());
    EXPECT_FALSE(UnicodeCategory::uppercase_letter().is_case_ignorable());
}

TEST(UnicodeCategoryTest, Comparison) {
    // Test equality operator
    EXPECT_EQ(UnicodeCategory::uppercase_letter(), UnicodeCategory::uppercase_letter());
    EXPECT_NE(UnicodeCategory::uppercase_letter(), UnicodeCategory::lowercase_letter());
}

TEST(UnicodeCategoryTest, CategoryNames) {
    // Test category names
    EXPECT_EQ(UnicodeCategory::uppercase_letter().name(), "Uppercase Letter");
    EXPECT_EQ(UnicodeCategory::lowercase_letter().name(), "Lowercase Letter");
    EXPECT_EQ(UnicodeCategory::decimal_number().name(), "Decimal Number");
}

TEST(UnicodeCategoryTest, CodePointCategories) {
    // Test ASCII characters
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'A').is_uppercase_letter());
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'a').is_lowercase_letter());
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'0').is_decimal_number());
    EXPECT_TRUE(UnicodeCategory::from_code_point(U' ').is_whitespace());
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'!').is_punctuation());
    
    // Test Latin-1 Supplement
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'À').is_uppercase_letter());
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'è').is_lowercase_letter());
    
    // Test control characters
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'\n').is_control());
    EXPECT_TRUE(UnicodeCategory::from_code_point(U'\t').is_control());
    
    // Test surrogates
    EXPECT_TRUE(UnicodeCategory::from_code_point(0xD800).is_surrogate());
    EXPECT_TRUE(UnicodeCategory::from_code_point(0xDFFF).is_surrogate());
    
    // Test private use
    EXPECT_TRUE(UnicodeCategory::from_code_point(0xE000).is_private_use());
    
    // Test unknown/unassigned (this might change with Unicode updates)
    EXPECT_TRUE(UnicodeCategory::from_code_point(0x110000).is_unknown());
}

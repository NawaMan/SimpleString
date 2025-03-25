#include <gtest/gtest.h>
#include "code_point.hpp"

using namespace simple_string;

TEST(CodePointTest, BasicProperties) {
    CodePoint a(U'A');
    EXPECT_EQ(a.value(), U'A');
    EXPECT_TRUE(a.is_category(UnicodeCategory::uppercase_letter()));
    EXPECT_EQ(a.category(), UnicodeCategory::uppercase_letter());
    
    CodePoint b(U'b');
    EXPECT_EQ(b.value(), U'b');
    EXPECT_TRUE(b.is_category(UnicodeCategory::lowercase_letter()));
    EXPECT_EQ(b.category(), UnicodeCategory::lowercase_letter());
}

TEST(CodePointTest, CategoryChecks) {
    // Test letters
    EXPECT_TRUE(CodePoint(U'A').is_letter());
    EXPECT_TRUE(CodePoint(U'a').is_letter());
    EXPECT_FALSE(CodePoint(U'1').is_letter());
    
    // Test numbers
    EXPECT_TRUE(CodePoint(U'1').is_number());
    EXPECT_TRUE(CodePoint(U'0').is_number());
    EXPECT_FALSE(CodePoint(U'A').is_number());
    
    // Test punctuation
    EXPECT_TRUE(CodePoint(U'!').is_punctuation());
    EXPECT_TRUE(CodePoint(U'.').is_punctuation());
    EXPECT_FALSE(CodePoint(U'A').is_punctuation());
    
    // Test symbols
    EXPECT_TRUE(CodePoint(U'+').is_symbol());
    EXPECT_TRUE(CodePoint(U'$').is_symbol());
    EXPECT_FALSE(CodePoint(U'A').is_symbol());
    
    // Test separators
    EXPECT_TRUE(CodePoint(U' ').is_separator());
    EXPECT_FALSE(CodePoint(U'A').is_separator());
}

TEST(CodePointTest, AdditionalProperties) {
    // Test whitespace
    EXPECT_TRUE(CodePoint(U' ').is_whitespace());
    EXPECT_TRUE(CodePoint(U'\n').is_whitespace());
    EXPECT_FALSE(CodePoint(U'A').is_whitespace());
    
    // Test alphanumeric
    EXPECT_TRUE(CodePoint(U'A').is_alphanumeric());
    EXPECT_TRUE(CodePoint(U'1').is_alphanumeric());
    EXPECT_FALSE(CodePoint(U'!').is_alphanumeric());
    
    // Test printable
    EXPECT_TRUE(CodePoint(U'A').is_printable());
    EXPECT_FALSE(CodePoint(U'\0').is_printable());
}

TEST(CodePointTest, Comparison) {
    CodePoint a1(U'A');
    CodePoint a2(U'A');
    CodePoint b(U'B');
    
    // Test equality
    EXPECT_EQ(a1, a2);
    EXPECT_NE(a1, b);
    
    // Test ordering
    EXPECT_LT(a1, b);
    EXPECT_LE(a1, a2);
    EXPECT_GT(b, a1);
    EXPECT_GE(a1, a2);
}

TEST(CodePointTest, NonASCII) {
    // Test Latin-1 Supplement
    CodePoint acute_a(U'á');
    EXPECT_TRUE(acute_a.is_letter());
    EXPECT_TRUE(acute_a.is_category(UnicodeCategory::lowercase_letter()));
    
    // Test control characters
    CodePoint null(U'\0');
    EXPECT_TRUE(null.is_category(UnicodeCategory::control()));
    EXPECT_FALSE(null.is_printable());
    
    // Test surrogates
    CodePoint high_surrogate(0xD800);
    EXPECT_TRUE(high_surrogate.is_category(UnicodeCategory::surrogate()));
    
    // Test private use
    CodePoint private_use(0xE000);
    EXPECT_TRUE(private_use.is_category(UnicodeCategory::private_use()));
}

#include <gtest/gtest.h>
#include "../include/char.hpp"
#include "../include/code_point.hpp"

// Task-005: Character and Code Point Support

using namespace mosaic;

TEST(CharTest, Construction) {
    // Default constructor
    Char c1;
    EXPECT_EQ(c1.value(), 0);
    EXPECT_TRUE(c1.is_null());
    
    // From char
    Char c2('A');
    EXPECT_EQ(c2.value(), 0x0041);
    
    // From char16_t
    Char c3(static_cast<char16_t>(0x00F1));  // ñ
    EXPECT_EQ(c3.value(), 0x00F1);
    
    // From char32_t (BMP)
    Char c4(static_cast<char32_t>(0x0101));  // ā
    EXPECT_EQ(c4.value(), 0x0101);
    
    // From char32_t (supplementary) - should use replacement char
    Char c5(static_cast<char32_t>(0x1F600));  // 😀
    EXPECT_EQ(c5.value(), Char::REPLACEMENT_CHAR);
    
    // From invalid code point (> 0x10FFFF)
    Char c6(static_cast<char32_t>(0x110000));
    EXPECT_EQ(c6.value(), Char::REPLACEMENT_CHAR);
}

TEST(CharTest, SurrogatePairs) {
    // High surrogate
    Char high(static_cast<char16_t>(0xD83D));
    EXPECT_TRUE(high.is_high_surrogate());
    EXPECT_FALSE(high.is_low_surrogate());
    EXPECT_TRUE(high.is_surrogate());
    
    // Low surrogate
    Char low(static_cast<char16_t>(0xDE00));
    EXPECT_FALSE(low.is_high_surrogate());
    EXPECT_TRUE(low.is_low_surrogate());
    EXPECT_TRUE(low.is_surrogate());
    
    // Non-surrogate
    Char regular('A');
    EXPECT_FALSE(regular.is_high_surrogate());
    EXPECT_FALSE(regular.is_low_surrogate());
    EXPECT_FALSE(regular.is_surrogate());
}

TEST(CharTest, CodePointConversion) {
    // BMP character (U+0041 LATIN CAPITAL LETTER A)
    char32_t bmpChar = 0x0041;
    EXPECT_FALSE(UnicodeUtil::is_supplementary_code_point(bmpChar));
    
    // Supplementary character (U+1F600 GRINNING FACE)
    char32_t supplementary = 0x1F600;
    EXPECT_TRUE(UnicodeUtil::is_supplementary_code_point(supplementary));
}

TEST(CharTest, NullCharacterConversion) {
    // Create a null Char
    Char nullChar;
    EXPECT_TRUE(nullChar.is_null());
    
    // Convert Char to CodePoint
    CodePoint nullPoint(nullChar.value());
    EXPECT_EQ(nullPoint.value(), 0);
    
    // Convert CodePoint back to Char
    Char convertedChar(static_cast<char16_t>(nullPoint.value()));
    EXPECT_TRUE(convertedChar.is_null());
    
    // Test explicit construction
    Char explicitNull(static_cast<char16_t>(0));
    EXPECT_TRUE(explicitNull.is_null());
    
    // Test char32_t constructor
    Char char32Null(static_cast<char32_t>(0));
    EXPECT_TRUE(char32Null.is_null());
    
    // Test non-null cases
    Char nonNull('A');
    EXPECT_FALSE(nonNull.is_null());
    
    // Test CodePoint with null high surrogate but non-null low surrogate
    CodePoint highNullPoint(0xDC00);  // Just a low surrogate
    EXPECT_EQ(highNullPoint.high_surrogate(), 0);
    EXPECT_NE(highNullPoint.low_surrogate(), 0);
    
    // Test CodePoint with null low surrogate but non-null high surrogate
    CodePoint lowNullPoint(0xD800);  // Just a high surrogate
    EXPECT_NE(lowNullPoint.high_surrogate(), 0);
    EXPECT_EQ(lowNullPoint.low_surrogate(), 0);
}

TEST(CharTest, StringConversion) {
    // ASCII character
    Char ascii('A');
    EXPECT_EQ(ascii.to_string(), std::u16string(1, u'A'));
    
    // BMP character
    Char bmp(static_cast<char16_t>(0x00F1));  // ñ
    EXPECT_EQ(bmp.to_string(), std::u16string(1, static_cast<char16_t>(0x00F1)));
    
    // Surrogate
    Char surrogate(static_cast<char16_t>(0xD83D));
    EXPECT_EQ(surrogate.to_string(), std::u16string(1, static_cast<char16_t>(0xD83D)));
    
    // Replacement character
    Char replacement(static_cast<char16_t>(Char::REPLACEMENT_CHAR));
    EXPECT_EQ(replacement.to_string(), std::u16string(1, static_cast<char16_t>(0xFFFD)));
}

TEST(CharTest, Comparison) {
    Char a('A');
    Char b('B');
    Char a2('A');
    
    // Equality
    EXPECT_TRUE(a == a2);
    EXPECT_FALSE(a == b);
    
    // Inequality
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != a2);
    
    // Less than
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a < a2);
    
    // Less than or equal
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= a2);
    EXPECT_FALSE(b <= a);
    
    // Greater than
    EXPECT_TRUE(b > a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a > a2);
    
    // Greater than or equal
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a >= a2);
    EXPECT_FALSE(a >= b);
}

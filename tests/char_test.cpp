#include <gtest/gtest.h>
#include "char.hpp"

// Task-005: Character and Code Point Support

using namespace simple_string;

TEST(CharTest, Construction) {
    // Default constructor
    Char c1;
    EXPECT_EQ(c1.value(), 0);
    
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
    EXPECT_EQ(c6.value(), static_cast<char16_t>(0x110000 & 0xFFFF));
}

TEST(CharTest, SurrogatePairs) {
    // High surrogate
    Char high(static_cast<char16_t>(0xD83D));
    EXPECT_TRUE(high.isHighSurrogate());
    EXPECT_FALSE(high.isLowSurrogate());
    EXPECT_TRUE(high.isSurrogate());
    
    // Low surrogate
    Char low(static_cast<char16_t>(0xDE00));
    EXPECT_FALSE(low.isHighSurrogate());
    EXPECT_TRUE(low.isLowSurrogate());
    EXPECT_TRUE(low.isSurrogate());
    
    // Non-surrogate
    Char regular('A');
    EXPECT_FALSE(regular.isHighSurrogate());
    EXPECT_FALSE(regular.isLowSurrogate());
    EXPECT_FALSE(regular.isSurrogate());
}

TEST(CharTest, CodePointConversion) {
    // BMP character (U+0041 LATIN CAPITAL LETTER A)
    char32_t bmpChar = 0x0041;
    EXPECT_FALSE(Char::isSupplementaryCodePoint(bmpChar));
    EXPECT_FALSE(Char::fromCodePoint(bmpChar).has_value());
    
    // Supplementary character (U+1F600 GRINNING FACE)
    char32_t supplementary = 0x1F600;
    EXPECT_TRUE(Char::isSupplementaryCodePoint(supplementary));
    
    // Convert supplementary to surrogate pair using new static method
    auto maybePair = Char::fromCodePoint(supplementary);
    EXPECT_TRUE(maybePair.has_value());
    
    const auto& [high, low] = *maybePair;
    
    // Verify surrogate pair
    EXPECT_TRUE(high.isHighSurrogate());
    EXPECT_TRUE(low.isLowSurrogate());
    
    // Convert back to code point
    char32_t roundTrip = high.toCodePoint(low);
    EXPECT_EQ(roundTrip, supplementary);
    
    // Invalid surrogate pair should return INVALID_CODEPOINT
    Char regular('A');
    EXPECT_EQ(regular.toCodePoint(low), Char::INVALID_CODEPOINT);
}

TEST(CharTest, StringConversion) {
    // ASCII character
    Char ascii('A');
    EXPECT_EQ(ascii.toString(), std::u16string(1, u'A'));
    
    // BMP character
    Char bmp(static_cast<char16_t>(0x00F1));  // ñ
    EXPECT_EQ(bmp.toString(), std::u16string(1, static_cast<char16_t>(0x00F1)));
    
    // Surrogate
    Char surrogate(static_cast<char16_t>(0xD83D));
    EXPECT_EQ(surrogate.toString(), std::u16string(1, static_cast<char16_t>(0xD83D)));
    
    // Replacement character
    Char replacement(static_cast<char16_t>(Char::REPLACEMENT_CHAR));
    EXPECT_EQ(replacement.toString(), std::u16string(1, static_cast<char16_t>(0xFFFD)));
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

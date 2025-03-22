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
    
    // From char32_t (supplementary)
    Char c5(static_cast<char32_t>(0x1F600));  // 😀
    EXPECT_TRUE(c5.isHighSurrogate());
    EXPECT_EQ(c5.value(), 0xD83D);
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
    
    // Supplementary character (U+1F600 GRINNING FACE)
    char32_t supplementary = 0x1F600;
    EXPECT_TRUE(Char::isSupplementaryCodePoint(supplementary));
    
    // Convert supplementary to surrogate pair
    Char high(static_cast<char16_t>(Char::highSurrogateOf(supplementary)));
    Char low(static_cast<char16_t>(Char::lowSurrogateOf(supplementary)));
    
    // Verify surrogate pair
    EXPECT_TRUE(high.isHighSurrogate());
    EXPECT_TRUE(low.isLowSurrogate());
    
    // Convert back to code point
    char32_t roundTrip = high.toCodePoint(low);
    EXPECT_EQ(roundTrip, supplementary);
}

TEST(CharTest, InvalidSurrogatePairs) {
    Char high(static_cast<char16_t>(0xD83D));
    Char low(static_cast<char16_t>(0xDE00));
    Char regular('A');
    
    // Invalid pairs should throw
    EXPECT_THROW(regular.toCodePoint(low), std::invalid_argument);  // Not a high surrogate
    EXPECT_THROW(high.toCodePoint(regular), std::invalid_argument); // Not a low surrogate
    EXPECT_THROW(low.toCodePoint(high), std::invalid_argument);     // Wrong order
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

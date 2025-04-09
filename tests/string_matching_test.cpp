#include <gtest/gtest.h>
#include "../include/string.hpp"

using namespace simple;

class StringMatchingTest : public ::testing::Test {
protected:
    // Test fixture setup
};

// Tests for contains() method
TEST_F(StringMatchingTest, ContainsBasic) {
    String s("Hello, world!");
    EXPECT_TRUE(s.contains(String("Hello")));
    EXPECT_TRUE(s.contains(String("world")));
    EXPECT_TRUE(s.contains(String(",")));
    EXPECT_TRUE(s.contains(String("!")));
    EXPECT_TRUE(s.contains(String("Hello, world!")));
    EXPECT_FALSE(s.contains(String("hello")));
    EXPECT_FALSE(s.contains(String("World")));
    EXPECT_FALSE(s.contains(String("xyz")));
}

TEST_F(StringMatchingTest, ContainsIsCaseSensitive) {
    String s("Hello");
    EXPECT_FALSE(s.contains(String("hello")));
    EXPECT_FALSE(s.contains(String("HELLO")));
    
    String mixed("aBcDeF");
    EXPECT_TRUE(mixed.contains(String("BcD")));
    EXPECT_FALSE(mixed.contains(String("bcd")));
    EXPECT_FALSE(mixed.contains(String("BCD")));
}

TEST_F(StringMatchingTest, ContainsEmptyStrings) {
    String s("Hello");
    String empty("");
    
    // Empty string is contained in any string (including empty string)
    EXPECT_TRUE(s.contains(empty));
    EXPECT_TRUE(empty.contains(empty));
    
    // Non-empty string is not contained in empty string
    EXPECT_FALSE(empty.contains(s));
}

TEST_F(StringMatchingTest, ContainsUnicode) {
    String s("こんにちは世界");
    EXPECT_TRUE(s.contains(String("こんにちは")));
    EXPECT_TRUE(s.contains(String("世界")));
    EXPECT_TRUE(s.contains(String("にち")));
    EXPECT_FALSE(s.contains(String("さようなら")));
    
    // Test with surrogate pairs
    String emoji("Hello 🌍 World");
    EXPECT_TRUE(emoji.contains(String("🌍")));
    EXPECT_TRUE(emoji.contains(String("Hello 🌍")));
    EXPECT_TRUE(emoji.contains(String("🌍 World")));
    EXPECT_FALSE(emoji.contains(String("🌎")));
    
    // Test with surrogate pairs in the middle
    String complexEmoji("ABC🌍😀XYZ");
    EXPECT_TRUE(complexEmoji.contains(String("🌍😀")));
    EXPECT_TRUE(complexEmoji.contains(String("C🌍😀")));
    EXPECT_FALSE(complexEmoji.contains(String("🌍Z"))); // Non-contiguous sequence
}

// Tests for startsWith() method
TEST_F(StringMatchingTest, StartsWithBasic) {
    String s("Hello, world!");
    EXPECT_TRUE(s.startsWith(String("Hello")));
    EXPECT_TRUE(s.startsWith(String("H")));
    EXPECT_TRUE(s.startsWith(String("Hello, world!")));
    EXPECT_FALSE(s.startsWith(String("hello")));
    EXPECT_FALSE(s.startsWith(String("world")));
}

TEST_F(StringMatchingTest, StartsWithOffset) {
    String s("Hello, world!");
    EXPECT_TRUE(s.startsWith(String("world"), Index(7)));
    EXPECT_TRUE(s.startsWith(String("w"), Index(7)));
    EXPECT_TRUE(s.startsWith(String(","), Index(5)));
    EXPECT_FALSE(s.startsWith(String("World"), Index(7)));
    EXPECT_FALSE(s.startsWith(String("Hello"), Index(1)));
    
    // Edge cases
    EXPECT_TRUE(s.startsWith(String(""), Index(s.length())));
    EXPECT_FALSE(s.startsWith(String("a"), Index(s.length())));
}

TEST_F(StringMatchingTest, StartsWithEmptyStrings) {
    String s("Hello");
    String empty("");
    
    // Empty string is a prefix of any string
    EXPECT_TRUE(s.startsWith(empty));
    EXPECT_TRUE(empty.startsWith(empty));
    
    // Non-empty string is not a prefix of empty string
    EXPECT_FALSE(empty.startsWith(s));
}

TEST_F(StringMatchingTest, StartsWithUnicode) {
    String s("こんにちは世界");
    EXPECT_TRUE(s.startsWith(String("こん")));
    EXPECT_TRUE(s.startsWith(String("こんにちは")));
    EXPECT_FALSE(s.startsWith(String("世界")));
    
    // Test with surrogate pairs
    String emoji("🌍 is Earth");
    EXPECT_TRUE(emoji.startsWith(String("🌍")));
    EXPECT_TRUE(emoji.startsWith(String("🌍 is")));
    EXPECT_FALSE(emoji.startsWith(String("Earth")));
}

TEST_F(StringMatchingTest, StartsWithSurrogatePairAtOffset) {
    String s("Hi 🌍 there");
    EXPECT_TRUE(s.startsWith(String("🌍"), Index(3))); // '🌍' starts at index 3 (in UTF-16 code units)
    EXPECT_TRUE(s.startsWith(String("🌍 there"), Index(3)));
    EXPECT_FALSE(s.startsWith(String("🌍"), Index(4))); // Offset in the middle of surrogate pair
    
    // Multiple surrogate pairs
    String multiEmoji("ABC🌍😀XYZ");
    EXPECT_TRUE(multiEmoji.startsWith(String("😀"), Index(5))); // Second emoji starts at index 5
    EXPECT_TRUE(multiEmoji.startsWith(String("😀XYZ"), Index(5)));
}

TEST_F(StringMatchingTest, StartsWithOffsetExceptions) {
    String s("Hello");
    // Offset is greater than the string length
    EXPECT_THROW(s.startsWith(String("a"), Index(10)), StringIndexOutOfBoundsException);
    
    // Offset exactly at length + 1
    std::size_t len = s.length();
    EXPECT_THROW(s.startsWith(String("a"), Index(len + 1)), StringIndexOutOfBoundsException);
    
    // Negative offset
    EXPECT_THROW(s.startsWith(String("H"), Index(-1)), StringIndexOutOfBoundsException);
}

// Tests for endsWith() method
TEST_F(StringMatchingTest, EndsWithBasic) {
    String s("Hello, world!");
    EXPECT_TRUE(s.endsWith(String("world!")));
    EXPECT_TRUE(s.endsWith(String("!")));
    EXPECT_TRUE(s.endsWith(String("Hello, world!")));
    EXPECT_FALSE(s.endsWith(String("World!")));
    EXPECT_FALSE(s.endsWith(String("Hello")));
}

TEST_F(StringMatchingTest, EndsWithEmptyStrings) {
    String s("Hello");
    String empty("");
    
    // Empty string is a suffix of any string
    EXPECT_TRUE(s.endsWith(empty));
    EXPECT_TRUE(empty.endsWith(empty));
    
    // Non-empty string is not a suffix of empty string
    EXPECT_FALSE(empty.endsWith(s));
}

TEST_F(StringMatchingTest, EndsWithUnicode) {
    String s("こんにちは世界");
    EXPECT_TRUE(s.endsWith(String("世界")));
    EXPECT_TRUE(s.endsWith(String("は世界")));
    EXPECT_FALSE(s.endsWith(String("こん")));
    
    // Test with surrogate pairs
    String emoji("Earth is 🌍");
    EXPECT_TRUE(emoji.endsWith(String("🌍")));
    EXPECT_TRUE(emoji.endsWith(String("is 🌍")));
    EXPECT_FALSE(emoji.endsWith(String("Earth")));
    
    // Multiple surrogate pairs at the end
    String multiEmoji("Hello🌍😀");
    EXPECT_TRUE(multiEmoji.endsWith(String("😀")));
    EXPECT_TRUE(multiEmoji.endsWith(String("🌍😀")));
    EXPECT_FALSE(multiEmoji.endsWith(String("🌍")));
}

// Performance-oriented tests
TEST_F(StringMatchingTest, ContainsLongString) {
    // Create a long string with the needle in the middle
    std::string prefix(10000, 'a');
    std::string suffix(10000, 'b');
    String longStr(prefix + "needle" + suffix);
    
    EXPECT_TRUE(longStr.contains(String("needle")));
    EXPECT_FALSE(longStr.contains(String("notfound")));
    
    // Test with needle at the beginning
    String longStrNeedleAtStart("needle" + std::string(20000, 'a'));
    EXPECT_TRUE(longStrNeedleAtStart.contains(String("needle")));
    
    // Test with needle at the end
    String longStrNeedleAtEnd(std::string(20000, 'a') + "needle");
    EXPECT_TRUE(longStrNeedleAtEnd.contains(String("needle")));
}

TEST_F(StringMatchingTest, StartsWithLongString) {
    std::string prefix(10000, 'a');
    String longStr(prefix + "suffix");
    
    EXPECT_TRUE(longStr.startsWith(String(prefix.substr(0, 100))));
    EXPECT_TRUE(longStr.startsWith(String(prefix)));
    EXPECT_FALSE(longStr.startsWith(String("b" + prefix.substr(1))));
}

TEST_F(StringMatchingTest, EndsWithLongString) {
    std::string suffix(10000, 'b');
    String longStr("prefix" + suffix);
    
    EXPECT_TRUE(longStr.endsWith(String(suffix.substr(suffix.length() - 100))));
    EXPECT_TRUE(longStr.endsWith(String(suffix)));
    EXPECT_FALSE(longStr.endsWith(String(suffix.substr(0, suffix.length() - 1) + "a")));
}

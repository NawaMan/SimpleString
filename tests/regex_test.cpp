#include <gtest/gtest.h>
#include "../include/regex.hpp"
#include "../include/string.hpp"

using namespace simple;

TEST(RegExTest, Construction) {
    // Test basic construction
    RegEx regex(String("[a-z]+"));
    EXPECT_EQ(regex.flags().value(), Flag::ECMAScript.value());
    
    // Test construction with flags
    RegEx regexWithFlags(String("[a-z]+"), Flag::CASE_INSENSITIVE);
    EXPECT_TRUE(regexWithFlags.flags().contain(Flag::CASE_INSENSITIVE));
    
    // Test invalid regex pattern
    EXPECT_THROW(RegEx(String("[")), RegExSyntaxException);
}

TEST(RegExTest, ExactMatching) {
    // Test forExactString
    String str("Hello.World");
    RegEx exactRegex = RegEx::forExactString(str);
    EXPECT_TRUE(exactRegex.matches(String("Hello.World")));
    EXPECT_FALSE(exactRegex.matches(String("Hello World")));
    
    // Test with regex metacharacters
    String metaStr("a.b*c+d?");
    RegEx metaRegex = RegEx::forExactString(metaStr);
    EXPECT_TRUE(metaRegex.matches(String("a.b*c+d?")));
    EXPECT_FALSE(metaRegex.matches(String("abbbbccccd")));
    
    // Test forExactChar
    Char ch('.');
    RegEx charRegex = RegEx::forExactChar(ch);
    EXPECT_TRUE(charRegex.matches(String(".")));
    EXPECT_FALSE(charRegex.matches(String("a")));
}

TEST(RegExTest, Matching) {
    // Test matches
    RegEx regex(String("^[a-z]+$"));
    EXPECT_TRUE(regex.matches(String("abc")));
    EXPECT_FALSE(regex.matches(String("123")));
    EXPECT_FALSE(regex.matches(String("abc123")));
    
    // Test case insensitive
    RegEx caseInsensitiveRegex(String("^[a-z]+$"), Flag::CASE_INSENSITIVE);
    EXPECT_TRUE(caseInsensitiveRegex.matches(String("ABC")));
    
    // Test find
    RegEx findRegex(String("[0-9]+"));
    EXPECT_TRUE(findRegex.find(String("abc123def")));
    EXPECT_FALSE(findRegex.find(String("abcdef")));
}

TEST(RegExTest, Replacement) {
    // Test replaceAll
    RegEx regex(String("[0-9]+"));
    String result = regex.replaceAll(String("abc123def456"), String("X"));
    EXPECT_EQ(result, String("abcXdefX"));
    
    // Test replaceFirst
    String firstResult = regex.replaceFirst(String("abc123def456"), String("X"));
    EXPECT_EQ(firstResult, String("abcXdef456"));
    
    // Test with empty replacement
    String emptyResult = regex.replaceAll(String("abc123def456"), String(""));
    EXPECT_EQ(emptyResult, String("abcdef"));
}

TEST(RegExTest, Splitting) {
    // Test split with no limit
    RegEx regex(String(","));
    std::vector<String> parts = regex.split(String("a,b,c,d"));
    ASSERT_EQ(parts.size(), 4);
    EXPECT_EQ(parts[0], String("a"));
    EXPECT_EQ(parts[1], String("b"));
    EXPECT_EQ(parts[2], String("c"));
    EXPECT_EQ(parts[3], String("d"));
    
    // Test split with positive limit
    std::vector<String> limitedParts = regex.split(String("a,b,c,d"), 2);
    ASSERT_EQ(limitedParts.size(), 2);
    EXPECT_EQ(limitedParts[0], String("a"));
    EXPECT_EQ(limitedParts[1], String("b,c,d"));
    
    // Test split with negative limit (include trailing empty strings, Java-compatible behavior)
    RegEx trailingRegex(String(","));
    std::vector<String> noTrailingParts = trailingRegex.split(String("a,b,c,"), -1);
    ASSERT_EQ(noTrailingParts.size(), 4);  // Should be 4 in Java-compatible behavior
    EXPECT_EQ(noTrailingParts[0], String("a"));
    EXPECT_EQ(noTrailingParts[1], String("b"));
    EXPECT_EQ(noTrailingParts[2], String("c"));
    EXPECT_EQ(noTrailingParts[3], String(""));  // Include trailing empty string
    
    // Test split with empty input
    std::vector<String> emptyParts = regex.split(String(""));
    ASSERT_EQ(emptyParts.size(), 1);
    EXPECT_TRUE(emptyParts[0].is_empty());
    
    // Test split with consecutive delimiters
    RegEx commaRegex(String(","));
    std::vector<String> consecutiveDelimParts = commaRegex.split(String("a,,b"));
    ASSERT_EQ(consecutiveDelimParts.size(), 3);
    EXPECT_EQ(consecutiveDelimParts[0], String("a"));
    EXPECT_EQ(consecutiveDelimParts[1], String(""));  // Empty string between consecutive delimiters
    EXPECT_EQ(consecutiveDelimParts[2], String("b"));
}

TEST(RegExTest, UnicodeSupport) {
    // Test Unicode matching
    RegEx unicodeRegex(String("\\p{L}+"));
    EXPECT_TRUE(unicodeRegex.matches(String("HelloПривет")));
    EXPECT_FALSE(unicodeRegex.matches(String("123")));
    
    // Test Unicode replacement
    // Using \p{S} for symbols instead of \p{So} which is more specific and may not be supported
    RegEx emojiRegex(String("\\p{S}"));
    String emojiResult = emojiRegex.replaceAll(String("Hello 😊 World 🌍"), String("X"));
    EXPECT_EQ(emojiResult, String("Hello X World X"));
    
    // Test Unicode splitting
    RegEx spaceSplitter(String("\\s+"));
    std::vector<String> unicodeParts = spaceSplitter.split(String("Hello Привет 世界"));
    ASSERT_EQ(unicodeParts.size(), 3);
    EXPECT_EQ(unicodeParts[0], String("Hello"));
    EXPECT_EQ(unicodeParts[1], String("Привет"));
    EXPECT_EQ(unicodeParts[2], String("世界"));
}

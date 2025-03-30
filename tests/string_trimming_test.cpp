#include <gtest/gtest.h>
#include "../include/string.hpp"

using namespace simple;

class StringTrimmingTest : public ::testing::Test {
protected:
    // Test fixture setup
};

// Tests for trim() method
TEST_F(StringTrimmingTest, TrimBasic) {
    // Basic ASCII whitespace trimming
    String s1("  Hello, world!  ");
    EXPECT_EQ(s1.trim(), String("Hello, world!"));
    
    // Only leading whitespace
    String s2("  Hello, world!");
    EXPECT_EQ(s2.trim(), String("Hello, world!"));
    
    // Only trailing whitespace
    String s3("Hello, world!  ");
    EXPECT_EQ(s3.trim(), String("Hello, world!"));
    
    // No whitespace
    String s4("Hello, world!");
    EXPECT_EQ(s4.trim(), String("Hello, world!"));
}

TEST_F(StringTrimmingTest, TrimEmptyAndWhitespace) {
    // Empty string
    String empty("");
    EXPECT_EQ(empty.trim(), empty);
    
    // String with only whitespace
    String whitespace(" \t\n\r");
    EXPECT_EQ(whitespace.trim(), String(""));
}

TEST_F(StringTrimmingTest, TrimWithNullCharacter) {
    // String with embedded null characters
    const char nullStr[] = "\0Hello\0";
    String s1(nullStr, 7);  // Include null characters in length
    EXPECT_EQ(s1.trim(), String(nullStr, 7));  // Should not trim null characters
    
    // String with whitespace and null characters
    const char nullWithSpace[] = " \t\0Hello\0 \n";
    String s2(nullWithSpace, 11);
    
    const char expectedStr[] = "\0Hello\0";
    EXPECT_EQ(s2.trim(), String(expectedStr, 7));
}

TEST_F(StringTrimmingTest, TrimAsciiWhitespaceOnly) {
    // trim() should only remove ASCII whitespace (≤ 0x20)
    String s1("  Hello, world!  ");
    EXPECT_EQ(s1.trim(), String("Hello, world!"));
    
    // Unicode whitespace should not be trimmed by trim()
    String s2("\u00A0Hello, world!\u2003");  // non-breaking space and em space
    EXPECT_EQ(s2.trim(), String("\u00A0Hello, world!\u2003"));
    
    // Mix of ASCII and Unicode whitespace
    String s3(" \t\u00A0Hello, world!\u2003\n");
    EXPECT_EQ(s3.trim(), String("\u00A0Hello, world!\u2003"));
}

// Tests for strip() method
TEST_F(StringTrimmingTest, StripBasic) {
    // Basic ASCII whitespace stripping
    String s1("  Hello, world!  ");
    EXPECT_EQ(s1.strip(), String("Hello, world!"));
    
    // Only leading whitespace
    String s2("  Hello, world!");
    EXPECT_EQ(s2.strip(), String("Hello, world!"));
    
    // Only trailing whitespace
    String s3("Hello, world!  ");
    EXPECT_EQ(s3.strip(), String("Hello, world!"));
    
    // No whitespace
    String s4("Hello, world!");
    EXPECT_EQ(s4.strip(), String("Hello, world!"));
}

TEST_F(StringTrimmingTest, StripUnicodeWhitespace) {
    // Unicode whitespace should be stripped by strip()
    String s1("\u00A0Hello, world!\u2003");  // non-breaking space and em space
    EXPECT_EQ(s1.strip(), String("Hello, world!"));
    
    // Mix of ASCII and Unicode whitespace
    String s2(" \t\u00A0Hello, world!\u2003\n");
    EXPECT_EQ(s2.strip(), String("Hello, world!"));
    
    // Various Unicode whitespace characters
    String s3("\u2000\u2001\u2002Hello, world!\u2004\u2005\u2006");
    EXPECT_EQ(s3.strip(), String("Hello, world!"));
    
    // Zero-width spaces
    String s4("\u200BHello, world!\u200C");
    EXPECT_EQ(s4.strip(), String("Hello, world!"));
}

TEST_F(StringTrimmingTest, StripEmptyAndWhitespace) {
    // Empty string
    String empty("");
    EXPECT_EQ(empty.strip(), empty);
    
    // String with only ASCII whitespace
    String asciiWhitespace(" \t\n\r");
    EXPECT_EQ(asciiWhitespace.strip(), String(""));
    
    // String with only Unicode whitespace
    String unicodeWhitespace("\u00A0\u2003\u2000\u200B");
    EXPECT_EQ(unicodeWhitespace.strip(), String(""));
    
    // String with mixed whitespace
    String mixedWhitespace(" \t\u00A0\u2003\n");
    EXPECT_EQ(mixedWhitespace.strip(), String(""));
}

// Tests for stripLeading() method
TEST_F(StringTrimmingTest, StripLeadingBasic) {
    // Basic ASCII whitespace stripping
    String s1("  Hello, world!  ");
    EXPECT_EQ(s1.stripLeading(), String("Hello, world!  "));
    
    // Only leading whitespace
    String s2("  Hello, world!");
    EXPECT_EQ(s2.stripLeading(), String("Hello, world!"));
    
    // Only trailing whitespace
    String s3("Hello, world!  ");
    EXPECT_EQ(s3.stripLeading(), String("Hello, world!  "));
    
    // No whitespace
    String s4("Hello, world!");
    EXPECT_EQ(s4.stripLeading(), String("Hello, world!"));
}

TEST_F(StringTrimmingTest, StripLeadingUnicodeWhitespace) {
    // Unicode whitespace should be stripped by stripLeading()
    String s1("\u00A0Hello, world!");  // non-breaking space
    EXPECT_EQ(s1.stripLeading(), String("Hello, world!"));
    
    // Mix of ASCII and Unicode whitespace
    String s2(" \t\u00A0Hello, world!");
    EXPECT_EQ(s2.stripLeading(), String("Hello, world!"));
    
    // Various Unicode whitespace characters
    String s3("\u2000\u2001\u2002Hello, world!");
    EXPECT_EQ(s3.stripLeading(), String("Hello, world!"));
    
    // Unicode whitespace at the end should be preserved
    String s4("\u2000Hello, world!\u2001");
    EXPECT_EQ(s4.stripLeading(), String("Hello, world!\u2001"));
}

TEST_F(StringTrimmingTest, StripLeadingEmptyAndWhitespace) {
    // Empty string
    String empty("");
    EXPECT_EQ(empty.stripLeading(), empty);
    
    // String with only whitespace
    String whitespace(" \t\n\r\u00A0\u2003");
    EXPECT_EQ(whitespace.stripLeading(), String(""));
}

// Tests for stripTrailing() method
TEST_F(StringTrimmingTest, StripTrailingBasic) {
    // Basic ASCII whitespace stripping
    String s1("  Hello, world!  ");
    EXPECT_EQ(s1.stripTrailing(), String("  Hello, world!"));
    
    // Only leading whitespace
    String s2("  Hello, world!");
    EXPECT_EQ(s2.stripTrailing(), String("  Hello, world!"));
    
    // Only trailing whitespace
    String s3("Hello, world!  ");
    EXPECT_EQ(s3.stripTrailing(), String("Hello, world!"));
    
    // No whitespace
    String s4("Hello, world!");
    EXPECT_EQ(s4.stripTrailing(), String("Hello, world!"));
}

TEST_F(StringTrimmingTest, StripTrailingUnicodeWhitespace) {
    // Unicode whitespace should be stripped by stripTrailing()
    String s1("Hello, world!\u00A0");  // non-breaking space
    EXPECT_EQ(s1.stripTrailing(), String("Hello, world!"));
    
    // Mix of ASCII and Unicode whitespace
    String s2("Hello, world! \t\u00A0");
    EXPECT_EQ(s2.stripTrailing(), String("Hello, world!"));
    
    // Various Unicode whitespace characters
    String s3("Hello, world!\u2000\u2001\u2002");
    EXPECT_EQ(s3.stripTrailing(), String("Hello, world!"));
    
    // Unicode whitespace at the beginning should be preserved
    String s4("\u2000Hello, world!\u2001");
    EXPECT_EQ(s4.stripTrailing(), String("\u2000Hello, world!"));
}

TEST_F(StringTrimmingTest, StripTrailingEmptyAndWhitespace) {
    // Empty string
    String empty("");
    EXPECT_EQ(empty.stripTrailing(), empty);
    
    // String with only whitespace
    String whitespace(" \t\n\r\u00A0\u2003");
    EXPECT_EQ(whitespace.stripTrailing(), String(""));
}

// Tests for isStripped() method
TEST_F(StringTrimmingTest, IsStrippedBasic) {
    // String with no whitespace
    String s1("Hello, world!");
    EXPECT_TRUE(s1.isStripped());
    
    // String with leading whitespace
    String s2("  Hello, world!");
    EXPECT_FALSE(s2.isStripped());
    
    // String with trailing whitespace
    String s3("Hello, world!  ");
    EXPECT_FALSE(s3.isStripped());
    
    // String with both leading and trailing whitespace
    String s4("  Hello, world!  ");
    EXPECT_FALSE(s4.isStripped());
    
    // String with internal whitespace
    String s5("Hello, world!");
    EXPECT_TRUE(s5.isStripped());
}

TEST_F(StringTrimmingTest, IsStrippedUnicodeWhitespace) {
    // String with Unicode whitespace at the beginning
    String s1("\u00A0Hello, world!");
    EXPECT_FALSE(s1.isStripped());
    
    // String with Unicode whitespace at the end
    String s2("Hello, world!\u2003");
    EXPECT_FALSE(s2.isStripped());
    
    // String with Unicode whitespace at both ends
    String s3("\u2000Hello, world!\u2001");
    EXPECT_FALSE(s3.isStripped());
    
    // String with Unicode whitespace in the middle
    String s4("Hello,\u2003world!");
    EXPECT_TRUE(s4.isStripped());
}

TEST_F(StringTrimmingTest, IsStrippedEmptyAndWhitespace) {
    // Empty string
    String empty("");
    EXPECT_TRUE(empty.isStripped());
    
    // String with only whitespace
    String whitespace(" \t\n\r\u00A0\u2003");
    EXPECT_FALSE(whitespace.isStripped());
}

// Tests for surrogate pairs in trimming methods
TEST_F(StringTrimmingTest, TrimmingWithSurrogatePairs) {
    // Emoji are multi-byte in UTF-8 and surrogate pairs in UTF-16, should not be trimmed
    String s1("🌟Hello, world!🌍");
    EXPECT_EQ(s1.trim(), String("🌟Hello, world!🌍"));
    EXPECT_EQ(s1.strip(), String("🌟Hello, world!🌍"));
    
    // String with whitespace and surrogate pairs
    String s2("  🌟Hello, world!🌍  ");
    EXPECT_EQ(s2.trim(), String("🌟Hello, world!🌍"));
    EXPECT_EQ(s2.strip(), String("🌟Hello, world!🌍"));
    
    // String with Unicode whitespace and surrogate pairs
    String s3("\u2003🌟Hello, world!🌍\u00A0");
    EXPECT_EQ(s3.trim(), String("\u2003🌟Hello, world!🌍\u00A0"));
    EXPECT_EQ(s3.strip(), String("🌟Hello, world!🌍"));
    
    // Test isStripped with surrogate pairs
    EXPECT_TRUE(s1.isStripped());
    EXPECT_FALSE(s2.isStripped());
    EXPECT_FALSE(s3.isStripped());
}

// Tests for identity optimization
TEST_F(StringTrimmingTest, IdentityOptimization) {
    // String with no whitespace should return the same content
    String s1("Hello, world!");
    String trimmed1 = s1.trim();
    String stripped1 = s1.strip();
    
    EXPECT_EQ(s1, trimmed1);
    EXPECT_EQ(s1, stripped1);
    
    // Also verify that the string is already considered stripped
    EXPECT_TRUE(s1.isStripped());
    
    // Empty string should return the same content
    String empty("");
    String trimmedEmpty = empty.trim();
    String strippedEmpty = empty.strip();
    
    EXPECT_EQ(empty, trimmedEmpty);
    EXPECT_EQ(empty, strippedEmpty);
    
    // String with whitespace should return different content
    String s2("  Hello, world!  ");
    String trimmed2 = s2.trim();
    String stripped2 = s2.strip();
    
    EXPECT_NE(s2, trimmed2);
    EXPECT_NE(s2, stripped2);
    
    // Test that strip() produces a string that is already stripped
    EXPECT_TRUE(String(" \t\u2000Hello\u2000\t ").strip().isStripped());
    
    // Test method chaining
    String s3(" \t\u00A0Hello, world!\u2003\n");
    EXPECT_EQ(s3.stripLeading().stripTrailing(), String("Hello, world!"));
    EXPECT_EQ(s3.stripLeading().stripTrailing(), s3.strip());
    EXPECT_NE(s2, stripped2);
    
    // Trimmed strings should be equal to each other if they produce the same result
    EXPECT_EQ(trimmed2, stripped2);
}

#include <gtest/gtest.h>
#include "../include/string.hpp"

using namespace simple;

class StringSubstringTest : public ::testing::Test {
protected:
    // Test strings initialized in constructor
    StringSubstringTest()
        : ascii("Hello, World!"),
          utf8("Hello, 世界!"),
          emoji("Hello 🌟🌍!"),
          empty("") {}

    // Test strings
    String ascii;  // ASCII string
    String utf8;   // UTF-8 string with non-ASCII characters
    String emoji;  // String with surrogate pairs (emojis)
    String empty;  // Empty string
};

TEST_F(StringSubstringTest, SubstringEntireString) {
    // Test substring of entire string
    String s1 = ascii.substring(0, ascii.length());
    EXPECT_TRUE(s1.equals(ascii));
    EXPECT_EQ(s1.length(), ascii.length());

    String s2 = utf8.substring(0, utf8.length());
    EXPECT_TRUE(s2.equals(utf8));
    EXPECT_EQ(s2.length(), utf8.length());

    String s3 = emoji.substring(0, emoji.length());
    EXPECT_TRUE(s3.equals(emoji));
    EXPECT_EQ(s3.length(), emoji.length());

    String s4 = empty.substring(0, empty.length());
    EXPECT_TRUE(s4.equals(empty));
    EXPECT_EQ(s4.length(), empty.length());
}

TEST_F(StringSubstringTest, SubstringFromBeginning) {
    // Test substring from beginning
    String s1 = ascii.substring(0, 5);  // "Hello"
    EXPECT_EQ(s1.to_string(), "Hello");
    EXPECT_EQ(s1.length(), 5);

    String s2 = utf8.substring(0, 7);  // "Hello, "
    EXPECT_EQ(s2.to_string(), "Hello, ");
    EXPECT_EQ(s2.length(), 7);

    String s3 = emoji.substring(0, 6);  // "Hello "
    EXPECT_EQ(s3.to_string(), "Hello ");
    EXPECT_EQ(s3.length(), 6);
}

TEST_F(StringSubstringTest, SubstringToEnd) {
    // Test substring to end
    String s1 = ascii.substring(7);  // "World!"
    EXPECT_EQ(s1.to_string(), "World!");
    EXPECT_EQ(s1.length(), 6);

    String s2 = utf8.substring(7);  // "世界!"
    EXPECT_EQ(s2.to_string(), "世界!");
    EXPECT_EQ(s2.length(), 3);

    String s3 = emoji.substring(6);  // "🌟🌍!"
    EXPECT_EQ(s3.to_string(), "🌟🌍!");
    EXPECT_EQ(s3.length(), 5);  // 2 for 🌟, 2 for 🌍, 1 for !
}

TEST_F(StringSubstringTest, SubstringMiddle) {
    // Test substring from middle
    String s1 = ascii.substring(7, 12);  // "World"
    EXPECT_EQ(s1.to_string(), "World");
    EXPECT_EQ(s1.length(), 5);

    String s2 = utf8.substring(7, 9);  // "世界" (2 characters)
    EXPECT_EQ(s2.to_string(), "世界");
    EXPECT_EQ(s2.length(), 2);

    String s3 = emoji.substring(6, 10);  // "🌟🌍" (2 surrogate pairs)
    EXPECT_EQ(s3.to_string(), "🌟🌍");
    EXPECT_EQ(s3.length(), 4);  // 2 for 🌟, 2 for 🌍
}

TEST_F(StringSubstringTest, EmptySubstring) {
    // Test empty substring
    String s1 = ascii.substring(5, 5);
    EXPECT_TRUE(s1.is_empty());
    EXPECT_EQ(s1.length(), 0);

    String s2 = utf8.substring(7, 7);
    EXPECT_TRUE(s2.is_empty());
    EXPECT_EQ(s2.length(), 0);

    String s3 = emoji.substring(6, 6);
    EXPECT_TRUE(s3.is_empty());
    EXPECT_EQ(s3.length(), 0);
}

TEST_F(StringSubstringTest, SubstringWithSurrogatePairs) {
    // Test substring with surrogate pairs at boundaries
    // emoji = "Hello 🌟🌍!"
    //                ^^ ^^
    // Indices:       6-7 8-9

    // Get just the first emoji
    String star = emoji.substring(6, 8);  // "🌟"
    EXPECT_EQ(star.length(), 2);
    EXPECT_EQ(star.to_string(), "🌟");

    // Get just the second emoji
    String earth = emoji.substring(8, 10);  // "🌍"
    EXPECT_EQ(earth.length(), 2);
    EXPECT_EQ(earth.to_string(), "🌍");
}

TEST_F(StringSubstringTest, SubstringExceptions) {
    // Test invalid indices
    EXPECT_THROW(ascii.substring(14), StringIndexOutOfBoundsException);  // beginIndex > length
    EXPECT_THROW(ascii.substring(5, 14), StringIndexOutOfBoundsException);  // endIndex > length
    EXPECT_THROW(ascii.substring(10, 5), StringIndexOutOfBoundsException);  // beginIndex > endIndex
}

TEST_F(StringSubstringTest, NestedSubstrings) {
    // Test taking substrings of substrings
    String s1 = ascii.substring(0, 5);  // "Hello"
    String s2 = s1.substring(1, 4);     // "ell"
    EXPECT_EQ(s2.to_string(), "ell");
    EXPECT_EQ(s2.length(), 3);

    String s3 = utf8.substring(0, 9);   // "Hello, 世界"
    String s4 = s3.substring(7, 9);     // "世界"
    EXPECT_EQ(s4.to_string(), "世界");
    EXPECT_EQ(s4.length(), 2);

    // Multiple levels of nesting
    String s5 = s4.substring(0, 1);     // "世"
    EXPECT_EQ(s5.to_string(), "世");
    EXPECT_EQ(s5.length(), 1);
}

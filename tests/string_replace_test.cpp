#include <gtest/gtest.h>
#include "../include/string.hpp"

using namespace simple;

// Tests for String replacement methods

TEST(StringReplaceTest, BasicCharacterReplace) {
    // This test will fail until the replace(Char, Char) method is implemented
    String s("banana");
    EXPECT_EQ(s.replace(Char('a'), Char('o')), String("bonono"));
}

TEST(StringReplaceTest, BasicSubstringReplace) {
    // This test will fail until the replace(String, String) method is implemented
    String s("foo bar foo");
    EXPECT_EQ(s.replace(String("foo"), String("baz")), String("baz bar baz"));
}

TEST(StringReplaceTest, EmptyReplacements) {
    // Test replacing with empty string
    String s("hello");
    EXPECT_EQ(s.replace(String("l"), String("")), String("heo"));
    
    // Test replacing empty string (should replace between each character)
    EXPECT_EQ(s.replace(String(""), String("-")), String("-h-e-l-l-o-"));
}

TEST(StringReplaceTest, UnicodeReplacements) {
    // Test replacing Unicode characters
    String s("Hello 世界!");
    EXPECT_EQ(s.replace(Char(u'世'), Char(u'地')), String("Hello 地界!"));
    
    // Test replacing Unicode strings
    EXPECT_EQ(s.replace(String("世界"), String("Earth")), String("Hello Earth!"));
}

TEST(StringReplaceTest, NoMatchReplacements) {
    // Test when there's no match (should return the original string)
    String s("hello");
    EXPECT_EQ(s.replace(Char('z'), Char('x')), String("hello"));
    EXPECT_EQ(s.replace(String("xyz"), String("abc")), String("hello"));
}

// Don't add this to the main test suite yet since the methods aren't implemented
#ifdef RUN_REPLACE_TESTS
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif

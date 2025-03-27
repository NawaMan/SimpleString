#include <gtest/gtest.h>
#include "../include/sstring.hpp"
#include "../include/char.hpp"

// Task-005: Character and Code Point Support

using namespace mosaic;

TEST(StringCharTest, CharacterAccess) {
    // ASCII string
    String ascii("Hello");
    EXPECT_EQ(ascii.char_at(0).value(), u'H');
    EXPECT_EQ(ascii[1].value(), u'e');
    EXPECT_EQ(ascii.char_value(2), u'l');
    
    // Out of bounds access
    EXPECT_THROW(ascii.char_at(5), StringIndexOutOfBoundsException);
    EXPECT_THROW(ascii[-1], StringIndexOutOfBoundsException);
}

TEST(StringCharTest, UTF8Characters) {
    // String with various UTF-8 sequences
    String utf8("Hello 世界");  // Mix of ASCII and CJK
    
    // ASCII part
    EXPECT_EQ(utf8.char_at(0).value(), u'H');
    EXPECT_EQ(utf8[5].value(), u' ');
    
    // CJK characters (世 = U+4E16, 界 = U+754C)
    EXPECT_EQ(utf8.char_at(6).value(), 0x4E16);  // 世
    EXPECT_EQ(utf8[7].value(), 0x754C);         // 界
}

TEST(StringCharTest, SurrogatePairs) {
    // String with surrogate pairs (🌍 = U+1F30D)
    String emoji("Hello 🌍");
    
    // ASCII part
    EXPECT_EQ(emoji.char_at(0).value(), u'H');
    EXPECT_EQ(emoji[5].value(), u' ');
    
    // Emoji (as surrogate pair)
    EXPECT_EQ(emoji.char_at(6).value(), 0xD83C);  // High surrogate
    EXPECT_EQ(emoji[7].value(), 0xDF0D);         // Low surrogate
    
    // Verify surrogate properties
    EXPECT_TRUE(emoji.char_at(6).is_high_surrogate());
    EXPECT_TRUE(emoji.char_at(7).is_low_surrogate());
    
    // Verify code point reconstruction
    EXPECT_EQ(emoji.char_at(6).to_code_point(emoji.char_at(7)), 0x1F30D);
}

TEST(StringCharTest, CombiningCharacters) {
    // String with combining characters (e + ◌́ = é)
    String combined("e\u0301");  // e + COMBINING ACUTE ACCENT
    
    // Should be two separate UTF-16 code units
    EXPECT_EQ(combined.char_at(0).value(), u'e');
    EXPECT_EQ(combined[1].value(), 0x0301);
    
    // Precomposed form (é as single character)
    String precomposed("\u00E9");
    EXPECT_EQ(precomposed.char_at(0).value(), 0x00E9);
    
    // Verify they have different lengths (as per memory 200223b4)
    EXPECT_EQ(combined.length(), 2);
    EXPECT_EQ(precomposed.length(), 1);
    
    // Verify they compare as different (as per memory 200223b4)
    EXPECT_NE(combined, precomposed);
}

TEST(StringCharTest, NullCharacters) {
    // String with embedded null characters
    String withNull("Hello\0World", 11);
    
    // Access around null character
    EXPECT_EQ(withNull.char_at(4).value(), u'o');
    EXPECT_EQ(withNull[5].value(), u'\0');
    EXPECT_EQ(withNull.char_at(6).value(), u'W');
    
    // Verify length includes null (as per memory 0ea2e31c)
    EXPECT_EQ(withNull.length(), 11);
}

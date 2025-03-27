#include <gtest/gtest.h>
#include "../include/sstring.hpp"

// Task-003: String Sharing Optimization

using namespace mosaic;

TEST(SStringTest, ConstructFromLiteral) {
    String s("Hello");
    EXPECT_EQ(s.length(), 5);
}

TEST(SStringTest, ConstructFromUTF8) {
    // In Java, String.length() returns the number of UTF-16 code units
    // "Hello, " = 7 chars
    // "世界" = 2 chars (each is a single code point requiring 1 UTF-16 code unit)
    // "!" = 1 char
    // Total = 10 chars
    String s("Hello, 世界!");
    EXPECT_EQ(s.length(), 10);
}

TEST(SStringTest, EmptyString) {
    String s("");
    EXPECT_EQ(s.length(), 0);
}

TEST(SStringTest, IsEmpty) {
    // Empty string returns true
    String empty("");
    EXPECT_TRUE(empty.is_empty());

    // Non-empty string returns false
    String nonEmpty("Hello");
    EXPECT_FALSE(nonEmpty.is_empty());

    // String with only whitespace returns false
    String whitespace("  \t\n\r");
    EXPECT_FALSE(whitespace.is_empty());

    // String with only null character returns false
    String nullChar("\0", 1);
    EXPECT_FALSE(nullChar.is_empty());
}

TEST(SStringTest, EmptyStringComparisons) {
    // Test empty string equality
    String empty1("");
    String empty2("");
    String nonEmpty("a");
    
    // Test equals() method
    EXPECT_TRUE (empty1.equals(empty2));
    EXPECT_FALSE(empty1.equals(nonEmpty));
    EXPECT_FALSE(nonEmpty.equals(empty1));
    
    // Test compare_to() method
    EXPECT_TRUE(empty1.compare_to(empty2).is_equal());
    EXPECT_TRUE(empty1.compare_to(nonEmpty).is_less());  // empty string sorts before non-empty
    EXPECT_TRUE(nonEmpty.compare_to(empty1).is_greater());  // non-empty string sorts after empty
    
    // Test operator overloads
    EXPECT_TRUE (empty1 == empty2);
    EXPECT_FALSE(empty1 != empty2);
    EXPECT_TRUE (empty1 != nonEmpty);
    EXPECT_TRUE (empty1 <  nonEmpty);
    EXPECT_TRUE (empty1 <= empty2);
    EXPECT_TRUE (empty1 <= nonEmpty);
    EXPECT_FALSE(empty1 >  nonEmpty);
    EXPECT_TRUE (empty1 >= empty2);
    EXPECT_FALSE(empty1 >= nonEmpty);
}

TEST(SStringTest, SurrogatePairs) {
    // "🌟" is outside the BMP and requires a surrogate pair in UTF-16
    // It's encoded as U+1F31F which becomes the surrogate pair U+D83C U+DF1F
    String s("🌟");
    EXPECT_EQ(s.length(), 2);  // Should be 2 UTF-16 code units

    // Multiple surrogate pairs
    String s2("Hello 🌟🌍");
    EXPECT_EQ(s2.length(), 10);  // 6 for "Hello " + 2*2 for the emojis
}

TEST(SStringTest, CombiningCharacters) {
    // "é" can be represented two ways:
    // 1. Single code point U+00E9 (Latin small letter e with acute)
    // 2. Two code points: U+0065 (Latin small letter e) + U+0301 (combining acute accent)
    
    // Single code point version (NFC - Normalized Form Canonical Composition)
    String s1("é");  // U+00E9
    EXPECT_EQ(s1.length(), 1);  // One UTF-16 code unit

    // Decomposed version with combining character (NFD - Normalized Form Canonical Decomposition)
    String s2("e\u0301");  // U+0065 + U+0301
    EXPECT_EQ(s2.length(), 2);  // Two UTF-16 code units

    // Currently they compare as different strings because we use byte-by-byte comparison
    // Note: In an ideal Unicode-aware implementation, these would be equal
    EXPECT_FALSE(s1.equals(s2));
    EXPECT_FALSE(s1.compare_to(s2).is_equal());

    // TODO: Consider implementing Unicode normalization to make these equal
    // Using Boost.Locale or ICU for proper Unicode normalization would make s1.equals(s2) return true
}

TEST(SStringTest, Equals) {
    // Test ASCII string equality
    String s1("Hello");
    String s2("Hello");
    String s3("hello");
    EXPECT_TRUE (s1.equals(s2));
    EXPECT_FALSE(s1.equals(s3));

    // Test UTF-8 string equality
    String s4("Hello, 世界!");
    String s5("Hello, 世界!");
    String s6("Hello, World!");
    EXPECT_TRUE (s4.equals(s5));
    EXPECT_FALSE(s4.equals(s6));

    // Test operator overloads
    EXPECT_TRUE (s1 == s2);
    EXPECT_FALSE(s1 == s3);
    EXPECT_TRUE (s1 != s3);
    EXPECT_FALSE(s1 != s2);
}

TEST(SStringTest, CompareTo) {
    // Test ASCII string comparison
    String s1("Hello");
    String s2("Hello");
    String s3("hello");
    String s4("Help");
    EXPECT_TRUE(s1.compare_to(s2).is_equal());
    EXPECT_TRUE(s1.compare_to(s3).is_less());  // 'H' < 'h'
    EXPECT_TRUE(s1.compare_to(s4).is_less());  // 'l' < 'p'

    // Test UTF-8 string comparison
    String s5("Hello, 世界!");  // Hello, 世界!
    String s6("Hello, 世界!");
    String s7("Hello, 世界");   // Hello, 世界
    EXPECT_TRUE(s5.compare_to(s6).is_equal());
    EXPECT_TRUE(s5.compare_to(s7).is_greater());  // '!' > ''

    // Test operator overloads
    EXPECT_TRUE(s1 <  s3);
    EXPECT_TRUE(s1 <= s2);
    EXPECT_TRUE(s3 >  s1);
    EXPECT_TRUE(s2 >= s1);
}

TEST(SStringTest, InvalidUtf8Handling) {
    // Test various types of invalid UTF-8 sequences
    
    // Invalid continuation byte
    const char invalid1[] = {static_cast<char>(0xFF), static_cast<char>(0xFE)};
    String s1(invalid1, 2);
    EXPECT_EQ(s1.length(), 2);  // Each invalid byte counts as 1 code unit
    
    // Incomplete 2-byte sequence
    const char incomplete2[] = {static_cast<char>(0xC0)};
    String s2(incomplete2, 1);
    EXPECT_EQ(s2.length(), 1);  // Incomplete sequence counts as 1 code unit
    
    // Incomplete 3-byte sequence
    const char incomplete3[] = {static_cast<char>(0xE0), static_cast<char>(0x80)};
    String s3(incomplete3, 2);
    EXPECT_EQ(s3.length(), 2);  // Each invalid/incomplete byte counts as 1
    
    // Incomplete 4-byte sequence
    const char incomplete4[] = {static_cast<char>(0xF0), static_cast<char>(0x80), static_cast<char>(0x80)};
    String s4(incomplete4, 3);
    EXPECT_EQ(s4.length(), 3);  // Each invalid/incomplete byte counts as 1
    
    // Overlong encoding of ASCII character (invalid)
    const char overlong[] = {static_cast<char>(0xC0), static_cast<char>(0xAF)};
    String s5(overlong, 2);
    EXPECT_EQ(s5.length(), 2);  // Each byte in invalid sequence counts as 1
    
    // Mixed valid and invalid sequences
    std::string mixed = "A" + std::string(incomplete2, 1) + "B" + std::string(invalid1, 2) + "C";
    String s6(mixed);
    EXPECT_EQ(s6.length(), 6);  // A(1) + invalid(1) + B(1) + invalid(2) + C(1)
    // Note: Each byte in invalid sequences counts as a separate code unit
    
    // Test comparison with invalid sequences
    String s7(invalid1, 2);
    String s8(incomplete2, 1);
    
    // Invalid sequences should still be comparable
    EXPECT_TRUE(s1.equals(s7));   // Same invalid sequence
    EXPECT_FALSE(s1.equals(s8));  // Different invalid sequences
    
    // Order should be consistent even with invalid sequences
    CompareResult cmp = s1.compare_to(s8);
    EXPECT_EQ(s8.compare_to(s1).value(), -cmp.value());  // Comparison should be symmetric
}

TEST(SStringTest, Immutability) {
    // Test immutability with std::string constructor
    std::string mutableStr = "Hello";
    String s1(mutableStr);
    mutableStr[0] = 'h';  // Modify original string
    EXPECT_EQ(s1.to_string(), "Hello");  // String should remain unchanged
    
    // Test immutability with char* constructor
    char mutableBuf[] = "World";
    String s2(mutableBuf, 5);
    mutableBuf[0] = 'w';  // Modify original buffer
    EXPECT_EQ(s2.to_string(), "World");  // String should remain unchanged
    
    // Test that toString() returns a const reference that can't modify internal data
    const std::string& ref = s1.to_string();
    static_assert(std::is_const<std::remove_reference_t<decltype(ref)>>::value,
                  "toString() must return a const reference");
    
    // Test immutability with UTF-8 strings
    std::string utf8Str = "Hello, 世界";
    String s3(utf8Str);
    utf8Str[7] = 'X';  // Try to modify UTF-8 sequence
    EXPECT_EQ(s3.to_string(), "Hello, 世界");  // String should remain unchanged
    
    // Test immutability with null characters
    char nullBuf[] = {'H', 'e', '\0', 'l', 'o'};
    String s4(nullBuf, 5);
    nullBuf[2] = 'l';  // Modify null character
    const char expected[] = {'H', 'e', '\0', 'l', 'o'};
    EXPECT_EQ(std::string(s4.to_string().data(), 5),
              std::string(expected, 5));
}

TEST(SStringTest, NullCharacterHandling) {
    // Test strings with embedded null characters
    const char str1[] = {'h', 'e', 'l', '\0', 'o', '!'};
    const char str2[] = {'h', 'e', 'l', '\0', 'o', '!'};
    const char str3[] = {'h', 'e', 'l', '\0', 'o'};      // One char shorter
    const char str4[] = {'h', 'e', 'l', '\1', 'o', '!'};  // Different middle char
    
    String s1(str1, 6);
    String s2(str2, 6);
    String s3(str3, 5);
    String s4(str4, 6);
    
    // Test length calculation
    EXPECT_EQ(s1.length(), 6);
    EXPECT_EQ(s3.length(), 5);
    
    // Test equality
    EXPECT_TRUE (s1.equals(s2));        // Same content and length
    EXPECT_FALSE(s1.equals(s3));        // Different length
    EXPECT_FALSE(s1.equals(s4));        // Same length, different content
    
    // Test comparison
    EXPECT_TRUE(s1.compare_to(s2).is_equal());     // Equal strings
    EXPECT_TRUE(s1.compare_to(s3).is_greater());   // Longer string > shorter string
    EXPECT_TRUE(s1.compare_to(s4).is_less());      // '\0' < '\1'
    
    // Test with regular strings
    String s5("hel");
    EXPECT_FALSE(s1.equals(s5));        // Different length
    EXPECT_TRUE(s1.compare_to(s5).is_greater());   // Longer string > shorter string
    
    // Test operator overloads
    EXPECT_TRUE (s1 == s2);
    EXPECT_FALSE(s1 != s2);
    EXPECT_TRUE (s1 != s3);
    EXPECT_TRUE (s1 <  s4);
    EXPECT_TRUE (s1 <= s2);
    EXPECT_FALSE(s1 >  s4);
    EXPECT_TRUE (s1 >= s2);
}

TEST(SStringTest, CodePointAt) {
    // Test BMP characters
    String bmp("Hello");
    EXPECT_EQ(bmp.code_point_at(0).value(), U'H');
    EXPECT_EQ(bmp.code_point_at(4).value(), U'o');
    EXPECT_TRUE(bmp.code_point_at(0).is_category(UnicodeCategory(UnicodeCategoryEnum::UPPERCASE_LETTER)));
    EXPECT_TRUE(bmp.code_point_at(4).is_category(UnicodeCategory(UnicodeCategoryEnum::LOWERCASE_LETTER)));

    // Test non-BMP characters (surrogate pairs)
    String nonBmp("Hello 🌟");
    auto star = nonBmp.code_point_at(6);
    EXPECT_EQ(star.value(), 0x1F31F);  // 🌟 is U+1F31F

    // Test error cases
    EXPECT_THROW({ bmp.code_point_at(5); }, StringIndexOutOfBoundsException);
    EXPECT_THROW({ bmp.code_point_at(100); }, StringIndexOutOfBoundsException);
}

TEST(SStringTest, CodePointBefore) {
    // Test BMP characters
    String bmp("Hello");
    EXPECT_EQ(bmp.code_point_before(1).value(), U'H');
    EXPECT_EQ(bmp.code_point_before(5).value(), U'o');
    EXPECT_TRUE(bmp.code_point_before(1).is_category(UnicodeCategory(UnicodeCategoryEnum::UPPERCASE_LETTER)));
    EXPECT_TRUE(bmp.code_point_before(5).is_category(UnicodeCategory(UnicodeCategoryEnum::LOWERCASE_LETTER)));

    // Test non-BMP characters (surrogate pairs)
    String nonBmp("Hello 🌟");
    auto star = nonBmp.code_point_before(8);
    EXPECT_EQ(star.value(), 0x1F31F);  // 🌟 is U+1F31F

    // Test error cases
    EXPECT_THROW({ bmp.code_point_before(0); }, StringIndexOutOfBoundsException);
    EXPECT_THROW({ bmp.code_point_before(6); }, StringIndexOutOfBoundsException);
    EXPECT_THROW({ bmp.code_point_before(100); }, StringIndexOutOfBoundsException);
}

TEST(SStringTest, CodePointCount) {
    // Test BMP characters
    String bmp("Hello");
    EXPECT_EQ(bmp.code_point_count(0, 5), 5);
    EXPECT_EQ(bmp.code_point_count(1, 4), 3);

    // Test non-BMP characters (surrogate pairs)
    String nonBmp("Hello 🌟🌍");
    EXPECT_EQ(nonBmp.code_point_count(0, 10), 8);  // 6 BMP chars + 2 surrogate pairs
    EXPECT_EQ(nonBmp.code_point_count(6, 8), 1);  // Just the 🌟

    // Test error cases
    EXPECT_THROW({ bmp.code_point_count(4, 2); }, StringIndexOutOfBoundsException);  // begin > end
    EXPECT_THROW({ bmp.code_point_count(0, 6); }, StringIndexOutOfBoundsException);  // end > length
    EXPECT_THROW({ bmp.code_point_count(100, 101); }, StringIndexOutOfBoundsException);  // both out of bounds
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

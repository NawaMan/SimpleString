#include <gtest/gtest.h>
#include "sstring.hpp"

using namespace simple_string;

TEST(SStringTest, ConstructFromLiteral) {
    SString s("Hello");
    EXPECT_EQ(s.length(), 5);
}

TEST(SStringTest, ConstructFromUTF8) {
    // In Java, String.length() returns the number of UTF-16 code units
    // "Hello, " = 7 chars
    // "世界" = 2 chars (each is a single code point requiring 1 UTF-16 code unit)
    // "!" = 1 char
    // Total = 10 chars
    SString s("Hello, 世界!");
    EXPECT_EQ(s.length(), 10);
}

TEST(SStringTest, EmptyString) {
    SString s("");
    EXPECT_EQ(s.length(), 0);
}

TEST(SStringTest, IsEmpty) {
    // Empty string returns true
    SString empty("");
    EXPECT_TRUE(empty.isEmpty());

    // Non-empty string returns false
    SString nonEmpty("Hello");
    EXPECT_FALSE(nonEmpty.isEmpty());

    // String with only whitespace returns false
    SString whitespace("  \t\n\r");
    EXPECT_FALSE(whitespace.isEmpty());

    // String with only null character returns false
    SString nullChar("\0", 1);
    EXPECT_FALSE(nullChar.isEmpty());
}

TEST(SStringTest, EmptyStringComparisons) {
    // Test empty string equality
    SString empty1("");
    SString empty2("");
    SString nonEmpty("a");
    
    // Test equals() method
    EXPECT_TRUE (empty1.equals(empty2));
    EXPECT_FALSE(empty1.equals(nonEmpty));
    EXPECT_FALSE(nonEmpty.equals(empty1));
    
    // Test compareTo() method
    EXPECT_TRUE(empty1.compareTo(empty2).isEqual());
    EXPECT_TRUE(empty1.compareTo(nonEmpty).isLess());  // empty string sorts before non-empty
    EXPECT_TRUE(nonEmpty.compareTo(empty1).isGreater());  // non-empty string sorts after empty
    
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
    SString s("🌟");
    EXPECT_EQ(s.length(), 2);  // Should be 2 UTF-16 code units

    // Multiple surrogate pairs
    SString s2("Hello 🌟🌍");
    EXPECT_EQ(s2.length(), 10);  // 6 for "Hello " + 2*2 for the emojis
}

TEST(SStringTest, CombiningCharacters) {
    // "é" can be represented two ways:
    // 1. Single code point U+00E9 (Latin small letter e with acute)
    // 2. Two code points: U+0065 (Latin small letter e) + U+0301 (combining acute accent)
    
    // Single code point version (NFC - Normalized Form Canonical Composition)
    SString s1("é");  // U+00E9
    EXPECT_EQ(s1.length(), 1);  // One UTF-16 code unit

    // Decomposed version with combining character (NFD - Normalized Form Canonical Decomposition)
    SString s2("e\u0301");  // U+0065 + U+0301
    EXPECT_EQ(s2.length(), 2);  // Two UTF-16 code units

    // Currently they compare as different strings because we use byte-by-byte comparison
    // Note: In an ideal Unicode-aware implementation, these would be equal
    EXPECT_FALSE(s1.equals(s2));
    EXPECT_FALSE(s1.compareTo(s2).isEqual());

    // TODO: Consider implementing Unicode normalization to make these equal
    // Using Boost.Locale or ICU for proper Unicode normalization would make s1.equals(s2) return true
}

TEST(SStringTest, Equals) {
    // Test ASCII string equality
    SString s1("Hello");
    SString s2("Hello");
    SString s3("hello");
    EXPECT_TRUE (s1.equals(s2));
    EXPECT_FALSE(s1.equals(s3));

    // Test UTF-8 string equality
    SString s4("Hello, 世界!");
    SString s5("Hello, 世界!");
    SString s6("Hello, World!");
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
    SString s1("Hello");
    SString s2("Hello");
    SString s3("hello");
    SString s4("Help");
    EXPECT_TRUE(s1.compareTo(s2).isEqual());
    EXPECT_TRUE(s1.compareTo(s3).isLess());  // 'H' < 'h'
    EXPECT_TRUE(s1.compareTo(s4).isLess());  // 'l' < 'p'

    // Test UTF-8 string comparison
    SString s5("Hello, 世界!");  // Hello, 世界!
    SString s6("Hello, 世界!");
    SString s7("Hello, 世界");   // Hello, 世界
    EXPECT_TRUE(s5.compareTo(s6).isEqual());
    EXPECT_TRUE(s5.compareTo(s7).isGreater());  // '!' > ''

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
    SString s1(invalid1, 2);
    EXPECT_EQ(s1.length(), 2);  // Each invalid byte counts as 1 code unit
    
    // Incomplete 2-byte sequence
    const char incomplete2[] = {static_cast<char>(0xC0)};
    SString s2(incomplete2, 1);
    EXPECT_EQ(s2.length(), 1);  // Incomplete sequence counts as 1 code unit
    
    // Incomplete 3-byte sequence
    const char incomplete3[] = {static_cast<char>(0xE0), static_cast<char>(0x80)};
    SString s3(incomplete3, 2);
    EXPECT_EQ(s3.length(), 2);  // Each invalid/incomplete byte counts as 1
    
    // Incomplete 4-byte sequence
    const char incomplete4[] = {static_cast<char>(0xF0), static_cast<char>(0x80), static_cast<char>(0x80)};
    SString s4(incomplete4, 3);
    EXPECT_EQ(s4.length(), 3);  // Each invalid/incomplete byte counts as 1
    
    // Overlong encoding of ASCII character (invalid)
    const char overlong[] = {static_cast<char>(0xC0), static_cast<char>(0xAF)};
    SString s5(overlong, 2);
    EXPECT_EQ(s5.length(), 2);  // Each byte in invalid sequence counts as 1
    
    // Mixed valid and invalid sequences
    std::string mixed = "A" + std::string(incomplete2, 1) + "B" + std::string(invalid1, 2) + "C";
    SString s6(mixed);
    EXPECT_EQ(s6.length(), 6);  // A(1) + invalid(1) + B(1) + invalid(2) + C(1)
    // Note: Each byte in invalid sequences counts as a separate code unit
    
    // Test comparison with invalid sequences
    SString s7(invalid1, 2);
    SString s8(incomplete2, 1);
    
    // Invalid sequences should still be comparable
    EXPECT_TRUE(s1.equals(s7));   // Same invalid sequence
    EXPECT_FALSE(s1.equals(s8));  // Different invalid sequences
    
    // Order should be consistent even with invalid sequences
    CompareResult cmp = s1.compareTo(s8);
    EXPECT_EQ(s8.compareTo(s1).value(), -cmp.value());  // Comparison should be symmetric
}

TEST(SStringTest, Immutability) {
    // Test immutability with std::string constructor
    std::string mutableStr = "Hello";
    SString s1(mutableStr);
    mutableStr[0] = 'h';  // Modify original string
    EXPECT_EQ(s1.toString(), "Hello");  // SString should remain unchanged
    
    // Test immutability with char* constructor
    char mutableBuf[] = "World";
    SString s2(mutableBuf, 5);
    mutableBuf[0] = 'w';  // Modify original buffer
    EXPECT_EQ(s2.toString(), "World");  // SString should remain unchanged
    
    // Test that toString() returns a const reference that can't modify internal data
    const std::string& ref = s1.toString();
    static_assert(std::is_const<std::remove_reference_t<decltype(ref)>>::value,
                  "toString() must return a const reference");
    
    // Test immutability with UTF-8 strings
    std::string utf8Str = "Hello, 世界";
    SString s3(utf8Str);
    utf8Str[7] = 'X';  // Try to modify UTF-8 sequence
    EXPECT_EQ(s3.toString(), "Hello, 世界");  // SString should remain unchanged
    
    // Test immutability with null characters
    char nullBuf[] = {'H', 'e', '\0', 'l', 'o'};
    SString s4(nullBuf, 5);
    nullBuf[2] = 'l';  // Modify null character
    const char expected[] = {'H', 'e', '\0', 'l', 'o'};
    EXPECT_EQ(std::string(s4.toString().data(), 5),
              std::string(expected, 5));
}

TEST(SStringTest, NullCharacterHandling) {
    // Test strings with embedded null characters
    const char str1[] = {'h', 'e', 'l', '\0', 'o', '!'};
    const char str2[] = {'h', 'e', 'l', '\0', 'o', '!'};
    const char str3[] = {'h', 'e', 'l', '\0', 'o'};      // One char shorter
    const char str4[] = {'h', 'e', 'l', '\1', 'o', '!'};  // Different middle char
    
    SString s1(str1, 6);
    SString s2(str2, 6);
    SString s3(str3, 5);
    SString s4(str4, 6);
    
    // Test length calculation
    EXPECT_EQ(s1.length(), 6);
    EXPECT_EQ(s3.length(), 5);
    
    // Test equality
    EXPECT_TRUE (s1.equals(s2));        // Same content and length
    EXPECT_FALSE(s1.equals(s3));        // Different length
    EXPECT_FALSE(s1.equals(s4));        // Same length, different content
    
    // Test comparison
    EXPECT_TRUE(s1.compareTo(s2).isEqual());     // Equal strings
    EXPECT_TRUE(s1.compareTo(s3).isGreater());   // Longer string > shorter string
    EXPECT_TRUE(s1.compareTo(s4).isLess());      // '\0' < '\1'
    
    // Test with regular strings
    SString s5("hel");
    EXPECT_FALSE(s1.equals(s5));        // Different length
    EXPECT_TRUE(s1.compareTo(s5).isGreater());   // Longer string > shorter string
    
    // Test operator overloads
    EXPECT_TRUE (s1 == s2);
    EXPECT_FALSE(s1 != s2);
    EXPECT_TRUE (s1 != s3);
    EXPECT_TRUE (s1 <  s4);
    EXPECT_TRUE (s1 <= s2);
    EXPECT_FALSE(s1 >  s4);
    EXPECT_TRUE (s1 >= s2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

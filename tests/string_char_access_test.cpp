#include <gtest/gtest.h>

#include "../include/string.hpp"

using namespace simple;

class SStringCharAccessTest : public ::testing::Test {
protected:
    // Helper function to create strings with raw bytes
    template<size_t N>
    std::string make_raw_string(const std::array<unsigned char, N>& bytes) {
        return std::string(reinterpret_cast<const char*>(bytes.data()), N);
    }

    // Test strings that will be used across multiple tests
    String empty{""};
    String ascii{"Hello"};
    String utf8{"Hello, 世界!"};  // Mixed ASCII and UTF-8
    String emoji{"🌟⭐"};         // Non-BMP characters (surrogate pairs)
    String mixed{"Hello 🌟 世界"}; // Mix of ASCII, surrogate pairs, and BMP

    // Invalid UTF-8 sequences
    String invalid1{make_raw_string<1>({0xFF})};
    String invalid2{make_raw_string<2>({0xC0, 0xAF})};
    String invalid3{make_raw_string<3>({0xE0, 0x80, 0xAF})};
    String invalid4{make_raw_string<4>({0xF0, 0x80, 0x80, 0xAF})};
    // Incomplete UTF-8 sequences
    String incomplete1{make_raw_string<1>({0xC2})};
    String incomplete2{make_raw_string<2>({0xE2, 0x82})};
    String incomplete3{make_raw_string<3>({0xF0, 0x9F, 0x8C})};
    // Overlong encodings
    String overlong1{make_raw_string<2>({0xC0, 0x80})};
    String overlong2{make_raw_string<3>({0xE0, 0x80, 0x80})};
    String overlong3{make_raw_string<4>({0xF0, 0x80, 0x80, 0x80})};
    // Surrogate range in UTF-8
    String surrogate{make_raw_string<3>({0xED, 0xA0, 0x80})};
    // Out of Unicode range
    String outOfRange{make_raw_string<4>({0xF4, 0x90, 0x80, 0x80})};
};

// Test empty string access
TEST_F(SStringCharAccessTest, EmptyString) {
    EXPECT_THROW(empty.char_at(0), StringIndexOutOfBoundsException);
}

// Test basic ASCII access
TEST_F(SStringCharAccessTest, BasicAsciiAccess) {
    // First character
    EXPECT_EQ(ascii.char_at(0).value(), u'H');
    
    // Middle character
    EXPECT_EQ(ascii.char_at(2).value(), u'l');
    
    // Last character
    EXPECT_EQ(ascii.char_at(4).value(), u'o');
    
    // Out of bounds
    EXPECT_THROW(ascii.char_at(5), StringIndexOutOfBoundsException);
}

// Test UTF-8 string access
TEST_F(SStringCharAccessTest, Utf8Access) {
    // ASCII part
    EXPECT_EQ(utf8.char_at(0).value(), u'H');
    EXPECT_EQ(utf8.char_at(6).value(), u' ');
    
    // UTF-8 part (世界)
    EXPECT_EQ(utf8.char_at(7).value(), u'世');
    EXPECT_EQ(utf8.char_at(8).value(), u'界');
    
    // Last character
    EXPECT_EQ(utf8.char_at(9).value(), u'!');
}

// Test surrogate pair access
TEST_F(SStringCharAccessTest, SurrogatePairAccess) {
    // First surrogate pair (🌟)
    EXPECT_EQ(emoji.char_at(0).value(), 0xD83C); // High surrogate
    EXPECT_EQ(emoji.char_at(1).value(), 0xDF1F); // Low surrogate
    
    // Second surrogate pair (⭐)
    EXPECT_EQ(emoji.char_at(2).value(), 0x2B50); // Single code unit (BMP)
}

// Test boundary conditions
TEST_F(SStringCharAccessTest, BoundaryConditions) {
    // Negative index
    EXPECT_THROW(ascii.char_at(-1), StringIndexOutOfBoundsException);
    
    // Index equal to length
    EXPECT_THROW(ascii.char_at(ascii.length()), StringIndexOutOfBoundsException);
    
    // Very large index
    EXPECT_THROW(ascii.char_at(SIZE_MAX), StringIndexOutOfBoundsException);
}

// Test mixed string access
TEST_F(SStringCharAccessTest, MixedStringAccess) {
    // ASCII part
    EXPECT_EQ(mixed.char_at(0).value(), u'H');
    EXPECT_EQ(mixed.char_at(4).value(), u'o');
    EXPECT_EQ(mixed.char_at(5).value(), u' ');
    
    // Surrogate pair (🌟)
    EXPECT_EQ(mixed.char_at(6).value(), 0xD83C); // High surrogate
    EXPECT_EQ(mixed.char_at(7).value(), 0xDF1F); // Low surrogate
    
    // Space
    EXPECT_EQ(mixed.char_at(8).value(), u' ');
    
    // UTF-8 part (世界)
    EXPECT_EQ(mixed.char_at(9).value(), u'世');
    EXPECT_EQ(mixed.char_at(10).value(), u'界');
}

// Test char_value method
// Test invalid UTF-8 sequences
TEST_F(SStringCharAccessTest, InvalidUtf8) {
    // Single invalid byte
    EXPECT_EQ(invalid1.length(), 1);
    EXPECT_EQ(invalid1.char_at(0).value(), 0xFFFD);
    
    // Invalid 2-byte sequence
    EXPECT_EQ(invalid2.length(), 2);
    EXPECT_EQ(invalid2.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(invalid2.char_at(1).value(), 0xFFFD);
    
    // Invalid 3-byte sequence
    EXPECT_EQ(invalid3.length(), 3);
    EXPECT_EQ(invalid3.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(invalid3.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(invalid3.char_at(2).value(), 0xFFFD);
    
    // Invalid 4-byte sequence
    EXPECT_EQ(invalid4.length(), 4);
    EXPECT_EQ(invalid4.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(invalid4.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(invalid4.char_at(2).value(), 0xFFFD);
    EXPECT_EQ(invalid4.char_at(3).value(), 0xFFFD);
}

// Test incomplete UTF-8 sequences
TEST_F(SStringCharAccessTest, IncompleteUtf8) {
    // Incomplete 2-byte sequence
    EXPECT_EQ(incomplete1.length(), 1);
    EXPECT_EQ(incomplete1.char_at(0).value(), 0xFFFD);
    
    // Incomplete 3-byte sequence
    EXPECT_EQ(incomplete2.length(), 2);
    EXPECT_EQ(incomplete2.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(incomplete2.char_at(1).value(), 0xFFFD);
    
    // Incomplete 4-byte sequence
    EXPECT_EQ(incomplete3.length(), 3);
    EXPECT_EQ(incomplete3.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(incomplete3.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(incomplete3.char_at(2).value(), 0xFFFD);
}

// Test overlong UTF-8 encodings
TEST_F(SStringCharAccessTest, OverlongUtf8) {
    // Overlong 2-byte sequence
    EXPECT_EQ(overlong1.length(), 2);
    EXPECT_EQ(overlong1.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(overlong1.char_at(1).value(), 0xFFFD);
    
    // Overlong 3-byte sequence
    EXPECT_EQ(overlong2.length(), 3);
    EXPECT_EQ(overlong2.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(overlong2.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(overlong2.char_at(2).value(), 0xFFFD);
    
    // Overlong 4-byte sequence
    EXPECT_EQ(overlong3.length(), 4);
    EXPECT_EQ(overlong3.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(overlong3.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(overlong3.char_at(2).value(), 0xFFFD);
    EXPECT_EQ(overlong3.char_at(3).value(), 0xFFFD);
}

// Test surrogate range and out of range UTF-8
TEST_F(SStringCharAccessTest, InvalidRanges) {
    // Surrogate range in UTF-8
    EXPECT_EQ(surrogate.length(), 3);
    EXPECT_EQ(surrogate.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(surrogate.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(surrogate.char_at(2).value(), 0xFFFD);
    
    // Out of Unicode range
    EXPECT_EQ(outOfRange.length(), 4);
    EXPECT_EQ(outOfRange.char_at(0).value(), 0xFFFD);
    EXPECT_EQ(outOfRange.char_at(1).value(), 0xFFFD);
    EXPECT_EQ(outOfRange.char_at(2).value(), 0xFFFD);
    EXPECT_EQ(outOfRange.char_at(3).value(), 0xFFFD);
}

// Test char_value method
TEST_F(SStringCharAccessTest, CharValue) {
    // Test that char_value gives same results as char_at().value()
    EXPECT_EQ(ascii.char_value(0), ascii.char_at(0).value());
    EXPECT_EQ(utf8.char_value(7), utf8.char_at(7).value());
    EXPECT_EQ(emoji.char_value(0), emoji.char_at(0).value());
    
    // Test error conditions
    EXPECT_THROW(empty.char_value(0), StringIndexOutOfBoundsException);
    EXPECT_THROW(ascii.char_value(5), StringIndexOutOfBoundsException);
    EXPECT_THROW(ascii.char_value(-1), StringIndexOutOfBoundsException);
}

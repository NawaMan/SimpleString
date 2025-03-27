#include <gtest/gtest.h>
#include "../include/sstring.hpp"
#include <array>

using namespace mosaic;

namespace {
// Helper function to create strings with raw bytes
template<size_t N>
std::string make_raw_string(const std::array<unsigned char, N>& bytes) {
    return std::string(reinterpret_cast<const char*>(bytes.data()), N);
}
}

class StringInvalidUtf8Test : public ::testing::Test {
protected:
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

// Test invalid UTF-8 sequences
TEST_F(StringInvalidUtf8Test, InvalidUtf8) {
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
TEST_F(StringInvalidUtf8Test, IncompleteUtf8) {
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
TEST_F(StringInvalidUtf8Test, OverlongUtf8) {
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
TEST_F(StringInvalidUtf8Test, InvalidRanges) {
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

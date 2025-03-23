#include <gtest/gtest.h>
#include "sstring.hpp"
#include <array>

using namespace simple_string;

namespace {
// Helper function to create strings with raw bytes
template<size_t N>
std::string make_raw_string(const std::array<unsigned char, N>& bytes) {
    return std::string(reinterpret_cast<const char*>(bytes.data()), N);
}
}

class SStringInvalidUtf8Test : public ::testing::Test {
protected:
    // Invalid UTF-8 sequences
    SString invalid1{make_raw_string<1>({0xFF})};
    SString invalid2{make_raw_string<2>({0xC0, 0xAF})};
    SString invalid3{make_raw_string<3>({0xE0, 0x80, 0xAF})};
    SString invalid4{make_raw_string<4>({0xF0, 0x80, 0x80, 0xAF})};
    // Incomplete UTF-8 sequences
    SString incomplete1{make_raw_string<1>({0xC2})};
    SString incomplete2{make_raw_string<2>({0xE2, 0x82})};
    SString incomplete3{make_raw_string<3>({0xF0, 0x9F, 0x8C})};
    // Overlong encodings
    SString overlong1{make_raw_string<2>({0xC0, 0x80})};
    SString overlong2{make_raw_string<3>({0xE0, 0x80, 0x80})};
    SString overlong3{make_raw_string<4>({0xF0, 0x80, 0x80, 0x80})};
    // Surrogate range in UTF-8
    SString surrogate{make_raw_string<3>({0xED, 0xA0, 0x80})};
    // Out of Unicode range
    SString outOfRange{make_raw_string<4>({0xF4, 0x90, 0x80, 0x80})};
};

// Test invalid UTF-8 sequences
TEST_F(SStringInvalidUtf8Test, InvalidUtf8) {
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
TEST_F(SStringInvalidUtf8Test, IncompleteUtf8) {
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
TEST_F(SStringInvalidUtf8Test, OverlongUtf8) {
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
TEST_F(SStringInvalidUtf8Test, InvalidRanges) {
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

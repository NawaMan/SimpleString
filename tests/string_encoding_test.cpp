#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <cstdint>
#include "../include/string.hpp"
#include "../include/encoding.hpp"

using namespace simple;

class StringEncodingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test strings with various Unicode characters
        ascii_string = String("Hello, World!");
        latin1_string = String("Café"); // é is in Latin-1 but not ASCII
        utf8_string = String("こんにちは世界"); // Japanese "Hello World"
        emoji_string = String("😀🌍🚀"); // Emoji characters (outside BMP)
        mixed_string = String("Hello, 世界! 😀"); // Mix of ASCII, CJK, and emoji
    }

    // Helper to compare byte vectors
    void expectEqualByteArrays(const std::vector<uint8_t>& expected, const std::vector<uint8_t>& actual) {
        ASSERT_EQ(expected.size(), actual.size()) << "Byte arrays have different sizes";
        
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(expected[i], actual[i]) << "Byte arrays differ at index " << i;
        }
    }

    // Helper to create a byte array from a hex string
    std::vector<uint8_t> hexToBytes(const std::string& hex) {
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }

    String ascii_string;
    String latin1_string;
    String utf8_string;
    String emoji_string;
    String mixed_string;
};

// Test UTF-8 encoding
TEST_F(StringEncodingTest, UTF8Encoding) {
    // ASCII string to UTF-8
    std::vector<uint8_t> expected_ascii = {
        'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'
    };
    auto actual_ascii = ascii_string.getBytes(Encoding::UTF_8);
    expectEqualByteArrays(expected_ascii, actual_ascii);

    // Test round-trip conversion
    String roundtrip = String::fromBytes(actual_ascii, Encoding::UTF_8);
    EXPECT_TRUE(ascii_string.equals(roundtrip));

    // UTF-8 with BOM
    auto with_bom = ascii_string.getBytes(Encoding::UTF_8, BOMPolicy::INCLUDE);
    EXPECT_EQ(expected_ascii.size() + 3, with_bom.size());
    EXPECT_EQ(0xEF, with_bom[0]);
    EXPECT_EQ(0xBB, with_bom[1]);
    EXPECT_EQ(0xBF, with_bom[2]);
    
    // Test complex Unicode string
    auto utf8_bytes = utf8_string.getBytes(Encoding::UTF_8);
    auto utf8_roundtrip = String::fromBytes(utf8_bytes, Encoding::UTF_8);
    EXPECT_TRUE(utf8_string.equals(utf8_roundtrip));

    // Test emoji characters (outside BMP)
    auto emoji_bytes = emoji_string.getBytes(Encoding::UTF_8);
    auto emoji_roundtrip = String::fromBytes(emoji_bytes, Encoding::UTF_8);
    EXPECT_TRUE(emoji_string.equals(emoji_roundtrip));
}

// Test UTF-16BE encoding
TEST_F(StringEncodingTest, UTF16BEEncoding) {
    // ASCII string to UTF-16BE
    std::vector<uint8_t> expected_ascii = {
        0x00, 'H', 0x00, 'e', 0x00, 'l', 0x00, 'l', 0x00, 'o', 0x00, ',', 
        0x00, ' ', 0x00, 'W', 0x00, 'o', 0x00, 'r', 0x00, 'l', 0x00, 'd', 0x00, '!'
    };
    auto actual_ascii = ascii_string.getBytes(Encoding::UTF_16BE);
    expectEqualByteArrays(expected_ascii, actual_ascii);

    // Test round-trip conversion
    String roundtrip = String::fromBytes(actual_ascii, Encoding::UTF_16BE);
    EXPECT_TRUE(ascii_string.equals(roundtrip));

    // UTF-16BE with BOM
    auto with_bom = ascii_string.getBytes(Encoding::UTF_16BE, BOMPolicy::INCLUDE);
    EXPECT_EQ(expected_ascii.size() + 2, with_bom.size());
    EXPECT_EQ(0xFE, with_bom[0]);
    EXPECT_EQ(0xFF, with_bom[1]);
    
    // Test complex Unicode string
    auto utf16be_bytes = utf8_string.getBytes(Encoding::UTF_16BE);
    auto utf16be_roundtrip = String::fromBytes(utf16be_bytes, Encoding::UTF_16BE);
    EXPECT_TRUE(utf8_string.equals(utf16be_roundtrip));

    // Test emoji characters (outside BMP)
    auto emoji_bytes = emoji_string.getBytes(Encoding::UTF_16BE);
    auto emoji_roundtrip = String::fromBytes(emoji_bytes, Encoding::UTF_16BE);
    EXPECT_TRUE(emoji_string.equals(emoji_roundtrip));
}

// Test UTF-16LE encoding
TEST_F(StringEncodingTest, UTF16LEEncoding) {
    // ASCII string to UTF-16LE
    std::vector<uint8_t> expected_ascii = {
        'H', 0x00, 'e', 0x00, 'l', 0x00, 'l', 0x00, 'o', 0x00, ',', 0x00, 
        ' ', 0x00, 'W', 0x00, 'o', 0x00, 'r', 0x00, 'l', 0x00, 'd', 0x00, '!', 0x00
    };
    auto actual_ascii = ascii_string.getBytes(Encoding::UTF_16LE);
    expectEqualByteArrays(expected_ascii, actual_ascii);

    // Test round-trip conversion
    String roundtrip = String::fromBytes(actual_ascii, Encoding::UTF_16LE);
    EXPECT_TRUE(ascii_string.equals(roundtrip));

    // UTF-16LE with BOM
    auto with_bom = ascii_string.getBytes(Encoding::UTF_16LE, BOMPolicy::INCLUDE);
    EXPECT_EQ(expected_ascii.size() + 2, with_bom.size());
    EXPECT_EQ(0xFF, with_bom[0]);
    EXPECT_EQ(0xFE, with_bom[1]);
    
    // Test complex Unicode string
    auto utf16le_bytes = utf8_string.getBytes(Encoding::UTF_16LE);
    auto utf16le_roundtrip = String::fromBytes(utf16le_bytes, Encoding::UTF_16LE);
    EXPECT_TRUE(utf8_string.equals(utf16le_roundtrip));

    // Test emoji characters (outside BMP)
    auto emoji_bytes = emoji_string.getBytes(Encoding::UTF_16LE);
    auto emoji_roundtrip = String::fromBytes(emoji_bytes, Encoding::UTF_16LE);
    EXPECT_TRUE(emoji_string.equals(emoji_roundtrip));
}

// Test UTF-32BE encoding
TEST_F(StringEncodingTest, UTF32BEEncoding) {
    // ASCII string to UTF-32BE
    auto actual_ascii = ascii_string.getBytes(Encoding::UTF_32BE);
    
    // Each ASCII character should be 4 bytes in UTF-32BE
    EXPECT_EQ(ascii_string.length() * 4, actual_ascii.size());
    
    // Check first character 'H' (0x00000048)
    EXPECT_EQ(0x00, actual_ascii[0]);
    EXPECT_EQ(0x00, actual_ascii[1]);
    EXPECT_EQ(0x00, actual_ascii[2]);
    EXPECT_EQ('H', actual_ascii[3]);

    // Test round-trip conversion
    String roundtrip = String::fromBytes(actual_ascii, Encoding::UTF_32BE);
    EXPECT_TRUE(ascii_string.equals(roundtrip));

    // UTF-32BE with BOM
    auto with_bom = ascii_string.getBytes(Encoding::UTF_32BE, BOMPolicy::INCLUDE);
    EXPECT_EQ(actual_ascii.size() + 4, with_bom.size());
    EXPECT_EQ(0x00, with_bom[0]);
    EXPECT_EQ(0x00, with_bom[1]);
    EXPECT_EQ(0xFE, with_bom[2]);
    EXPECT_EQ(0xFF, with_bom[3]);
    
    // Test complex Unicode string
    auto utf32be_bytes = utf8_string.getBytes(Encoding::UTF_32BE);
    auto utf32be_roundtrip = String::fromBytes(utf32be_bytes, Encoding::UTF_32BE);
    EXPECT_TRUE(utf8_string.equals(utf32be_roundtrip));

    // Test emoji characters (outside BMP)
    auto emoji_bytes = emoji_string.getBytes(Encoding::UTF_32BE);
    auto emoji_roundtrip = String::fromBytes(emoji_bytes, Encoding::UTF_32BE);
    EXPECT_TRUE(emoji_string.equals(emoji_roundtrip));
}

// Test UTF-32LE encoding
TEST_F(StringEncodingTest, UTF32LEEncoding) {
    // ASCII string to UTF-32LE
    auto actual_ascii = ascii_string.getBytes(Encoding::UTF_32LE);
    
    // Each ASCII character should be 4 bytes in UTF-32LE
    EXPECT_EQ(ascii_string.length() * 4, actual_ascii.size());
    
    // Check first character 'H' (0x48000000 in little endian)
    EXPECT_EQ('H', actual_ascii[0]);
    EXPECT_EQ(0x00, actual_ascii[1]);
    EXPECT_EQ(0x00, actual_ascii[2]);
    EXPECT_EQ(0x00, actual_ascii[3]);

    // Test round-trip conversion
    String roundtrip = String::fromBytes(actual_ascii, Encoding::UTF_32LE);
    EXPECT_TRUE(ascii_string.equals(roundtrip));

    // UTF-32LE with BOM
    auto with_bom = ascii_string.getBytes(Encoding::UTF_32LE, BOMPolicy::INCLUDE);
    EXPECT_EQ(actual_ascii.size() + 4, with_bom.size());
    EXPECT_EQ(0xFF, with_bom[0]);
    EXPECT_EQ(0xFE, with_bom[1]);
    EXPECT_EQ(0x00, with_bom[2]);
    EXPECT_EQ(0x00, with_bom[3]);
    
    // Test complex Unicode string
    auto utf32le_bytes = utf8_string.getBytes(Encoding::UTF_32LE);
    auto utf32le_roundtrip = String::fromBytes(utf32le_bytes, Encoding::UTF_32LE);
    EXPECT_TRUE(utf8_string.equals(utf32le_roundtrip));

    // Test emoji characters (outside BMP)
    auto emoji_bytes = emoji_string.getBytes(Encoding::UTF_32LE);
    auto emoji_roundtrip = String::fromBytes(emoji_bytes, Encoding::UTF_32LE);
    EXPECT_TRUE(emoji_string.equals(emoji_roundtrip));
}

// Test ISO-8859-1 (Latin-1) encoding
TEST_F(StringEncodingTest, ISO8859_1Encoding) {
    // ASCII string to ISO-8859-1
    std::vector<uint8_t> expected_ascii = {
        'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'
    };
    auto actual_ascii = ascii_string.getBytes(Encoding::ISO_8859_1);
    expectEqualByteArrays(expected_ascii, actual_ascii);

    // Test round-trip conversion for ASCII
    String roundtrip = String::fromBytes(actual_ascii, Encoding::ISO_8859_1);
    EXPECT_TRUE(ascii_string.equals(roundtrip));
    
    // Latin-1 string to ISO-8859-1
    auto latin1_bytes = latin1_string.getBytes(Encoding::ISO_8859_1);
    auto latin1_roundtrip = String::fromBytes(latin1_bytes, Encoding::ISO_8859_1);
    EXPECT_TRUE(latin1_string.equals(latin1_roundtrip));
    
    // Test error handling for characters outside Latin-1
    EXPECT_THROW({
        utf8_string.getBytes(Encoding::ISO_8859_1);
    }, EncodingException);
    
    // Test emoji in ASCII encoding should throw
    EXPECT_THROW({
        emoji_string.getBytes(Encoding::ASCII);
    }, EncodingException);
    
    // Test REPLACE error handling
    auto replaced_bytes = utf8_string.getBytes(Encoding::ISO_8859_1, EncodingErrorHandling::REPLACE);
    EXPECT_FALSE(replaced_bytes.empty());
    
    // Test IGNORE error handling
    auto ignored_bytes = utf8_string.getBytes(Encoding::ISO_8859_1, EncodingErrorHandling::IGNORE);
    EXPECT_FALSE(ignored_bytes.empty());
}

// Test ASCII encoding
TEST_F(StringEncodingTest, ASCIIEncoding) {
    // ASCII string to ASCII
    std::vector<uint8_t> expected_ascii = {
        'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'
    };
    auto actual_ascii = ascii_string.getBytes(Encoding::ASCII);
    expectEqualByteArrays(expected_ascii, actual_ascii);

    // Test round-trip conversion for ASCII
    String roundtrip = String::fromBytes(actual_ascii, Encoding::ASCII);
    EXPECT_TRUE(ascii_string.equals(roundtrip));
    
    // Test error handling for characters outside ASCII
    EXPECT_THROW({
        latin1_string.getBytes(Encoding::ASCII);
    }, EncodingException);
    
    // Test REPLACE error handling
    auto replaced_bytes = latin1_string.getBytes(Encoding::ASCII, EncodingErrorHandling::REPLACE);
    EXPECT_FALSE(replaced_bytes.empty());
    
    // Test IGNORE error handling
    auto ignored_bytes = latin1_string.getBytes(Encoding::ASCII, EncodingErrorHandling::IGNORE);
    EXPECT_FALSE(ignored_bytes.empty());
}

// Test BOM handling
TEST_F(StringEncodingTest, BOMHandling) {
    // UTF-8 with BOM
    auto utf8_with_bom = hexToBytes("EFBBBF48656C6C6F2C20576F726C6421"); // BOM + "Hello, World!"
    
    // Test AUTO policy (should detect and skip BOM)
    auto str1 = String::fromBytes(utf8_with_bom, Encoding::UTF_8, BOMPolicy::AUTO);
    EXPECT_TRUE(ascii_string.equals(str1));
    
    // Test INCLUDE policy (should expect and skip BOM)
    auto str2 = String::fromBytes(utf8_with_bom, Encoding::UTF_8, BOMPolicy::INCLUDE);
    EXPECT_TRUE(ascii_string.equals(str2));
    
    // Test EXCLUDE policy (should treat BOM as part of the content)
    auto str3 = String::fromBytes(utf8_with_bom, Encoding::UTF_8, BOMPolicy::EXCLUDE);
    EXPECT_FALSE(ascii_string.equals(str3));
    
    // Test INCLUDE policy with missing BOM
    std::vector<uint8_t> ascii_bytes = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    EXPECT_THROW({
        String::fromBytes(ascii_bytes, Encoding::UTF_8, BOMPolicy::INCLUDE);
    }, EncodingException);
}

// Test error handling strategies
TEST_F(StringEncodingTest, ErrorHandlingStrategies) {
    // Create invalid UTF-8 sequence
    std::vector<uint8_t> invalid_utf8 = {'H', 'e', 'l', 'l', 'o', 0xFF, 0xFF, '!'};
    
    // Test THROW strategy
    EXPECT_THROW({
        String::fromBytes(invalid_utf8, Encoding::UTF_8);
    }, EncodingException);
    
    // Test REPLACE strategy
    auto replaced = String::fromBytes(invalid_utf8, Encoding::UTF_8, EncodingErrorHandling::REPLACE);
    EXPECT_FALSE(replaced.is_empty());
    // length() counts Unicode characters (code points), not bytes
    EXPECT_EQ(8, replaced.length()); // Length should include replacement characters
    // Verify replacement character U+FFFD is present in the result
    // Create a string with the replacement character (U+FFFD)
    String replacement_char;
    {
        std::vector<uint8_t> replacement_bytes = {0xEF, 0xBF, 0xBD}; // UTF-8 for U+FFFD
        replacement_char = String::fromBytes(replacement_bytes, Encoding::UTF_8);
    }
    EXPECT_TRUE(replaced.contains(replacement_char));
    
    // Test IGNORE strategy
    auto ignored = String::fromBytes(invalid_utf8, Encoding::UTF_8, EncodingErrorHandling::IGNORE);
    EXPECT_FALSE(ignored.is_empty());
    EXPECT_EQ(6, ignored.length()); // Length should exclude invalid bytes
    // Verify replacement character is NOT present in the result
    EXPECT_FALSE(ignored.contains(replacement_char));
}

// Test null character handling
TEST_F(StringEncodingTest, NullCharacterHandling) {
    // Create a string with embedded null characters
    String with_null("abc\0def", 7);
    EXPECT_EQ(7, with_null.length()); // Verify length includes the null character
    
    // Test round-trip conversion with various encodings
    // UTF-8
    auto utf8_bytes = with_null.getBytes(Encoding::UTF_8);
    auto utf8_restored = String::fromBytes(utf8_bytes, Encoding::UTF_8);
    EXPECT_TRUE(with_null.equals(utf8_restored));
    
    // UTF-16BE
    auto utf16be_bytes = with_null.getBytes(Encoding::UTF_16BE);
    auto utf16be_restored = String::fromBytes(utf16be_bytes, Encoding::UTF_16BE);
    EXPECT_TRUE(with_null.equals(utf16be_restored));
    
    // UTF-32LE
    auto utf32le_bytes = with_null.getBytes(Encoding::UTF_32LE);
    auto utf32le_restored = String::fromBytes(utf32le_bytes, Encoding::UTF_32LE);
    EXPECT_TRUE(with_null.equals(utf32le_restored));
}

// Test convenience methods
TEST_F(StringEncodingTest, ConvenienceMethods) {
    // Test toStdString
    std::string std_str = ascii_string.toStdString();
    EXPECT_EQ("Hello, World!", std_str);
    
    // Test fromStdString
    String from_std = String::fromStdString(std_str);
    EXPECT_TRUE(ascii_string.equals(from_std));
    
    // Test round-trip conversion
    EXPECT_TRUE(ascii_string.equals(String::fromStdString(ascii_string.toStdString())));
    EXPECT_TRUE(utf8_string.equals(String::fromStdString(utf8_string.toStdString())));
    EXPECT_TRUE(emoji_string.equals(String::fromStdString(emoji_string.toStdString())));
}

// Test EncodingException details
TEST_F(StringEncodingTest, EncodingExceptionDetails) {
    try {
        // Create invalid UTF-8 sequence
        std::vector<uint8_t> invalid_utf8 = {'H', 'e', 'l', 'l', 'o', 0xFF, 0xFF, '!'};
        String::fromBytes(invalid_utf8, Encoding::UTF_8);
        FAIL() << "Expected EncodingException";
    } catch (const EncodingException& e) {
        // Test exception details
        EXPECT_EQ(Encoding::UTF_8, e.getEncoding());
        EXPECT_EQ(EncodingErrorHandling::THROW, e.getErrorHandling());
        // Byte offset may vary depending on the implementation
        
        // Verify exception message contains helpful diagnostic information
        std::string error_message = e.what();
        EXPECT_TRUE(error_message.find("UTF-8") != std::string::npos);
        EXPECT_TRUE(error_message.find("Invalid") != std::string::npos || 
                   error_message.find("invalid") != std::string::npos);
    }
}

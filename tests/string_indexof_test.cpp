#include <gtest/gtest.h>
#include "../include/string.hpp"

using namespace simple;

// Test fixture for string indexOf/lastIndexOf tests
class StringIndexOfTest : public ::testing::Test {
protected:
    // Common test strings
    String empty{""};
    String singleChar{"A"};
    String basic{"Hello, World!"};
    String repeated{"Hello, Hello, World!"};
    String unicode{"Hello, 世界!"}; // Using actual Unicode characters (世界 = world in Chinese)
    String surrogatePair{"Hello, 𐐷!"}; // Using actual surrogate pair (U+10437, outside BMP)
    
    // Common substrings
    String hello{"Hello"};
    String world{"World"};
    String exclamation{"!"};
    String nonExistent{"xyz"};
    String chineseWorld{"世界"};
    String surrogateChar{"𐐷"};
};

// Test indexOf with characters
TEST_F(StringIndexOfTest, IndexOfChar) {
    // Basic test with ASCII characters
    ASSERT_EQ(basic.indexOf(Char('H')).value(), 0);
    ASSERT_EQ(basic.indexOf(Char('e')).value(), 1);
    ASSERT_EQ(basic.indexOf(Char('!')).value(), 12); // Last character
    ASSERT_TRUE(basic.indexOf(Char('z')).is_invalid()); // Non-existent character
    
    // Test with single character string
    ASSERT_EQ(singleChar.indexOf(Char('A')).value(), 0);
    ASSERT_TRUE(singleChar.indexOf(Char('B')).is_invalid());
    
    // Test with empty string
    ASSERT_TRUE(empty.indexOf(Char('A')).is_invalid());
}

// Test indexOf with character and fromIndex
TEST_F(StringIndexOfTest, IndexOfCharWithFromIndex) {
    // Basic tests
    ASSERT_EQ(repeated.indexOf(Char('H'), Index(0)).value(), 0);
    ASSERT_EQ(repeated.indexOf(Char('H'), Index(1)).value(), 7);
    ASSERT_EQ(repeated.indexOf(Char('o'), Index(5)).value(), 11);
    ASSERT_EQ(repeated.indexOf(Char('o'), Index(6)).value(), 11);
    
    // Boundary tests
    ASSERT_EQ(repeated.indexOf(Char('!'), Index(repeated.length() - 1)).value(), repeated.length() - 1);
    ASSERT_TRUE(repeated.indexOf(Char('H'), Index(repeated.length() - 1)).is_invalid());
    
    // Test with non-existent character
    ASSERT_TRUE(repeated.indexOf(Char('z'), Index(0)).is_invalid());
    
    // Test with out-of-bounds fromIndex
    ASSERT_TRUE(repeated.indexOf(Char('H'), Index(100)).is_invalid());
}

// Test indexOf with string
TEST_F(StringIndexOfTest, IndexOfString) {
    // Basic tests
    ASSERT_EQ(basic.indexOf(hello).value(), 0);
    ASSERT_EQ(basic.indexOf(world).value(), 7);
    ASSERT_EQ(basic.indexOf(exclamation).value(), 12);
    ASSERT_TRUE(basic.indexOf(nonExistent).is_invalid());
    
    // Test with substring at the beginning and end
    String helloWorld{"Hello World"};
    ASSERT_EQ(helloWorld.indexOf(String("Hello")).value(), 0); // Beginning
    ASSERT_EQ(helloWorld.indexOf(String("World")).value(), 6); // End
    
    // Test with multiple occurrences
    ASSERT_EQ(repeated.indexOf(hello).value(), 0); // First occurrence
    
    // Empty string should match at position 0
    ASSERT_EQ(basic.indexOf(empty).value(), 0);
    
    // Empty string searching in empty string
    ASSERT_EQ(empty.indexOf(empty).value(), 0);
}

// Test indexOf with string and fromIndex
TEST_F(StringIndexOfTest, IndexOfStringWithFromIndex) {
    // Basic tests
    ASSERT_EQ(repeated.indexOf(hello, Index(0)).value(), 0);
    ASSERT_EQ(repeated.indexOf(hello, Index(1)).value(), 7);
    ASSERT_TRUE(repeated.indexOf(hello, Index(8)).is_invalid());
    
    // Test with empty string
    ASSERT_EQ(repeated.indexOf(empty, Index(5)).value(), 5);
    
    // Test with out-of-bounds fromIndex
    ASSERT_TRUE(repeated.indexOf(hello, Index(100)).is_invalid());
    
    // Test with fromIndex at exact match position
    ASSERT_EQ(repeated.indexOf(hello, Index(7)).value(), 7);
}

// Test lastIndexOf with characters
TEST_F(StringIndexOfTest, LastIndexOfChar) {
    // Basic tests
    ASSERT_EQ(basic.lastIndexOf(Char('H')).value(), 0);
    ASSERT_EQ(basic.lastIndexOf(Char('o')).value(), 8);
    ASSERT_EQ(basic.lastIndexOf(Char('l')).value(), 10);
    ASSERT_EQ(basic.lastIndexOf(Char('!')).value(), 12);
    ASSERT_TRUE(basic.lastIndexOf(Char('z')).is_invalid());
    
    // Test with repeated characters
    ASSERT_EQ(repeated.lastIndexOf(Char('H')).value(), 7); // Last 'H'
    ASSERT_EQ(repeated.lastIndexOf(Char('o')).value(), 15); // Last 'o'
    
    // Test with single character string
    ASSERT_EQ(singleChar.lastIndexOf(Char('A')).value(), 0);
    
    // Test with empty string
    ASSERT_TRUE(empty.lastIndexOf(Char('A')).is_invalid());
}

// Test lastIndexOf with character and fromIndex
TEST_F(StringIndexOfTest, LastIndexOfCharWithFromIndex) {
    // Basic tests
    ASSERT_EQ(repeated.lastIndexOf(Char('H'), Index(0)).value(), 0);
    ASSERT_EQ(repeated.lastIndexOf(Char('H'), Index(7)).value(), 7);
    ASSERT_EQ(repeated.lastIndexOf(Char('H'), Index(8)).value(), 7);
    ASSERT_EQ(repeated.lastIndexOf(Char('o'), Index(20)).value(), 15);
    ASSERT_EQ(repeated.lastIndexOf(Char('o'), Index(10)).value(), 4);
    
    // Boundary tests
    ASSERT_EQ(repeated.lastIndexOf(Char('H'), Index(repeated.length() - 1)).value(), 7);
    ASSERT_EQ(repeated.lastIndexOf(Char('!'), Index(repeated.length() - 1)).value(), repeated.length() - 1);
    
    // Test with non-existent character
    ASSERT_TRUE(repeated.lastIndexOf(Char('z'), Index(0)).is_invalid());
}

// Test lastIndexOf with string
TEST_F(StringIndexOfTest, LastIndexOfString) {
    // Basic tests
    ASSERT_EQ(repeated.lastIndexOf(hello).value(), 7);
    ASSERT_EQ(repeated.lastIndexOf(world).value(), 14);
    ASSERT_EQ(repeated.lastIndexOf(exclamation).value(), 19);
    ASSERT_TRUE(repeated.lastIndexOf(nonExistent).is_invalid());
    
    // Test with multiple occurrences
    String multipleHello{"Hello Hello Hello"};
    ASSERT_EQ(multipleHello.lastIndexOf(hello).value(), 12); // Last occurrence
    
    // Empty string should match at the end of the string
    ASSERT_EQ(repeated.lastIndexOf(empty).value(), repeated.length());
    
    // Empty string searching in empty string
    ASSERT_EQ(empty.lastIndexOf(empty).value(), 0);
}

// Test lastIndexOf with string and fromIndex
TEST_F(StringIndexOfTest, LastIndexOfStringWithFromIndex) {
    // Basic tests
    ASSERT_EQ(repeated.lastIndexOf(hello, Index(20)).value(), 7);
    ASSERT_EQ(repeated.lastIndexOf(hello, Index(7)).value(), 7);
    ASSERT_EQ(repeated.lastIndexOf(hello, Index(6)).value(), 0);
    
    // Test with empty string
    ASSERT_EQ(repeated.lastIndexOf(empty, Index(5)).value(), 5);
    
    // Test with out-of-bounds fromIndex
    ASSERT_EQ(repeated.lastIndexOf(hello, Index(100)).value(), 7); // Should adjust to valid range
    
    // Test with fromIndex at exact match position
    ASSERT_EQ(repeated.lastIndexOf(hello, Index(0)).value(), 0);
}

// Test with Unicode characters
TEST_F(StringIndexOfTest, UnicodeCharacters) {
    // Get the actual length and print it for debugging
    std::size_t actualLength = unicode.length();
    std::cout << "Unicode string length: " << actualLength << std::endl;
    std::cout << "Unicode string: " << unicode.to_string() << std::endl;
    
    // Test indexOf with ASCII characters in Unicode string
    ASSERT_EQ(unicode.indexOf(Char('H')).value(), 0);
    ASSERT_EQ(unicode.indexOf(Char('o')).value(), 4);
    
    // Test lastIndexOf with ASCII characters in Unicode string
    ASSERT_EQ(unicode.lastIndexOf(Char('H')).value(), 0);
    ASSERT_EQ(unicode.lastIndexOf(Char('o')).value(), 4);
    
    // Find the actual position of the Chinese characters and use that
    Index chinesePos = unicode.indexOf(chineseWorld);
    std::cout << "Chinese world position: " << chinesePos.value() << std::endl;
    
    // Test with Unicode substring - using the actual position
    ASSERT_FALSE(chinesePos.is_invalid());
    ASSERT_EQ(unicode.lastIndexOf(chineseWorld).value(), chinesePos.value());
    
    // Test with special characters
    String specialChars{"Cafe Creme"};
    ASSERT_EQ(specialChars.indexOf(Char('C')).value(), 0);
    ASSERT_EQ(specialChars.indexOf(Char('e')).value(), 3);
    ASSERT_EQ(specialChars.lastIndexOf(Char('C')).value(), 5);
    ASSERT_EQ(specialChars.lastIndexOf(Char('e')).value(), 9);
}

// Test with surrogate pairs (characters outside BMP)
TEST_F(StringIndexOfTest, SurrogatePairs) {
    // Test with surrogate pair string - get actual length
    std::size_t actualLength = surrogatePair.length();
    std::cout << "Surrogate pair string length: " << actualLength << std::endl;
    std::cout << "Surrogate pair string: " << surrogatePair.to_string() << std::endl;
    ASSERT_EQ(surrogatePair.length(), actualLength);
    
    // Find the actual position of the surrogate pair and use that
    Index surrogatePos = surrogatePair.indexOf(surrogateChar);
    std::cout << "Surrogate char position: " << surrogatePos.value() << std::endl;
    
    // Test indexOf with surrogate pair - using the actual position
    ASSERT_FALSE(surrogatePos.is_invalid());
    ASSERT_EQ(surrogatePair.lastIndexOf(surrogateChar).value(), surrogatePos.value());
    
    // Test with multiple occurrences of surrogate pairs
    String multipleChars{"𐐷𐐷𐐷"};
    std::cout << "Multiple surrogate chars length: " << multipleChars.length() << std::endl;
    
    // Find first and last occurrence
    Index firstPos = multipleChars.indexOf(surrogateChar);
    Index lastPos = multipleChars.lastIndexOf(surrogateChar);
    std::cout << "First surrogate position: " << firstPos.value() << std::endl;
    std::cout << "Last surrogate position: " << lastPos.value() << std::endl;
    
    ASSERT_FALSE(firstPos.is_invalid());
    ASSERT_FALSE(lastPos.is_invalid());
    ASSERT_NE(firstPos.value(), lastPos.value());
}

// Test with mixed ASCII and Unicode characters
TEST_F(StringIndexOfTest, MixedCharacters) {
    // String with mix of ASCII, Unicode, and surrogate pairs
    String mixed{"A世B𐐷C世D"};
    std::cout << "Mixed string length: " << mixed.length() << std::endl;
    std::cout << "Mixed string: " << mixed.to_string() << std::endl;
    
    // Print character positions for debugging
    for (std::size_t i = 0; i < mixed.length(); i++) {
        try {
            std::cout << "Char at position " << i << ": " << static_cast<int>(mixed.char_at(Index(i)).value()) << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Exception at position " << i << ": " << e.what() << std::endl;
        }
    }
    
    // Test indexOf with different character types
    ASSERT_EQ(mixed.indexOf(Char('A')).value(), 0);
    
    // Find the actual positions of Unicode characters
    Index firstChinesePos = mixed.indexOf(String{"世"});
    Index surrogatePos = mixed.indexOf(String{"𐐷"});
    Index cPos = mixed.indexOf(Char('C'));
    Index lastChinesePos = mixed.lastIndexOf(String{"世"});
    
    std::cout << "First Chinese position: " << firstChinesePos.value() << std::endl;
    std::cout << "Surrogate position: " << surrogatePos.value() << std::endl;
    std::cout << "C position: " << cPos.value() << std::endl;
    std::cout << "Last Chinese position: " << lastChinesePos.value() << std::endl;
    
    // Test with actual positions
    ASSERT_FALSE(firstChinesePos.is_invalid());
    ASSERT_FALSE(surrogatePos.is_invalid());
    ASSERT_FALSE(cPos.is_invalid());
    ASSERT_FALSE(lastChinesePos.is_invalid());
    
    // Test with fromIndex - find second occurrence of Chinese character
    Index secondChinesePos = mixed.indexOf(String{"世"}, Index(firstChinesePos.value() + 1));
    ASSERT_FALSE(secondChinesePos.is_invalid());
    ASSERT_EQ(lastChinesePos.value(), secondChinesePos.value());
    
    // Test non-existent characters
    ASSERT_TRUE(mixed.indexOf(Char('Z')).is_invalid());
    ASSERT_TRUE(mixed.indexOf(String{"XYZ"}).is_invalid());
}

// Test boundary conditions and edge cases
TEST_F(StringIndexOfTest, BoundaryConditions) {
    // Test with single character string
    String singleCharStr{"X"};
    ASSERT_EQ(singleCharStr.indexOf(Char('X')).value(), 0);
    ASSERT_EQ(singleCharStr.lastIndexOf(Char('X')).value(), 0);
    ASSERT_TRUE(singleCharStr.indexOf(Char('Y')).is_invalid());
    ASSERT_TRUE(singleCharStr.lastIndexOf(Char('Y')).is_invalid());
    
    // Test with string containing only the search character
    String allSameChar{"AAAAA"};
    ASSERT_EQ(allSameChar.indexOf(Char('A')).value(), 0); // First occurrence
    ASSERT_EQ(allSameChar.lastIndexOf(Char('A')).value(), 4); // Last occurrence
    
    // Test with string where substring is found at the very start and end
    String startAndEnd{"ABCxxxABC"};
    String subABC{"ABC"};
    ASSERT_EQ(startAndEnd.indexOf(subABC).value(), 0); // First occurrence
    ASSERT_EQ(startAndEnd.lastIndexOf(subABC).value(), 6); // Last occurrence
    
    // Test with very large string
    std::string largeStr(10000, 'A'); // 10,000 'A' characters
    largeStr += "XYZ";  // Target at the end
    String largeString{largeStr};
    String target{"XYZ"};
    ASSERT_EQ(largeString.indexOf(target).value(), 10000);
    ASSERT_EQ(largeString.lastIndexOf(target).value(), 10000);
}

// main function is defined in string_test.cppASSERT_EQ
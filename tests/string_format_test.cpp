#include <gtest/gtest.h>
#include "../include/string.hpp"

namespace simple {

TEST(StringFormatTest, BasicFormatting) {
    // String formatting
    EXPECT_EQ(String("Hello, World!"), String::format(String("%s, %s!"), "Hello", "World"));
    
    // Integer formatting
    EXPECT_EQ(String("The answer is 42"), String::format(String("The answer is %d"), 42));
    
    // Float formatting
    EXPECT_EQ(String("Pi is approximately 3.14"), String::format(String("Pi is approximately %.2f"), 3.14159));
    
    // Character formatting
    EXPECT_EQ(String("The first letter is A"), String::format(String("The first letter is %c"), 'A'));
    
    // Boolean formatting with actual bool values
    EXPECT_EQ(String("This is true"), String::format(String("This is %b"), true));
    EXPECT_EQ(String("This is false"), String::format(String("This is %b"), false));
    
    // Test boolean type mismatch
    EXPECT_THROW(String::format(String("Boolean as int: %b"), 42), FormatException);
    
    // Boost.Format specific features
    EXPECT_EQ(String("Hello, World!"), String::format(String("%1%, %2%!"), "Hello", "World"));
}

TEST(StringFormatTest, FormatModifiers) {
    // Width and alignment
    EXPECT_EQ(String("    Hello"), String::format(String("%10s"), "Hello"));
    EXPECT_EQ(String("Hello    "), String::format(String("%-10s"), "Hello"));
    
    // Zero padding
    EXPECT_EQ(String("00042"), String::format(String("%05d"), 42));
    
    // Precision
    EXPECT_EQ(String("3.14"), String::format(String("%.2f"), 3.14159));
}

TEST(StringFormatTest, ErrorHandling) {
    // Format string syntax error
    EXPECT_THROW(String::format(String("%")), FormatException);
    
    // Argument type mismatch
    EXPECT_THROW(String::format(String("%d"), "not an integer"), FormatException);
    
    // Missing argument
    EXPECT_THROW(String::format(String("%s %s"), "only one"), FormatException);
    
    // Test FormatException properties
    try {
        String::format(String("%s %s"), "only one");
        FAIL() << "Expected FormatException";
    } catch (const FormatException& e) {
        // Verify the exception contains the original format string
        EXPECT_EQ("%s %s", e.getFormatString());
        EXPECT_NE(std::string::npos, std::string(e.what()).find("%s %s"));
    }
}

TEST(StringFormatTest, UnicodeHandling) {
    // Test with Unicode characters
    EXPECT_EQ(String("Hello, 世界!"), String::format(String("%s, %s!"), "Hello", "世界"));
    
    // Test with emoji
    EXPECT_EQ(String("I ❤️ C++"), String::format(String("%s %s %s"), "I", "❤️", "C++"));
    
    // Test with surrogate pairs
    EXPECT_EQ(String("Surrogate pair: 𝄞"), String::format(String("Surrogate pair: %s"), "𝄞"));
}

TEST(StringFormatTest, IndexedFormatting) {
    // Test with indexed arguments
    EXPECT_EQ(String("World, Hello!"), String::format(String("%2$s, %1$s!"), "Hello", "World"));
    
    // Test with repeated indexed arguments
    EXPECT_EQ(String("Hello, Hello, World!"), String::format(String("%1$s, %1$s, %2$s!"), "Hello", "World"));
}

// TODO: Implement grouping separator support in the future
TEST(StringFormatTest, DISABLED_GroupingSeparator) {
    // Boost.Format does not natively support this; placeholder for future support
    EXPECT_EQ(String("1,234,567"), String::format(String("%,d"), 1234567));
    EXPECT_EQ(String("1'234'567"), String::format(String("%'d"), 1234567));
}

// TODO: Add performance tests for large format strings
TEST(StringFormatTest, DISABLED_PerformanceLargeFormatString) {
    // Placeholder: test formatting performance on large strings
    // For example, format a string with many substitutions
    // Create a format string with many placeholders
    std::string stdLargeFormat = "%s";
    for (int i = 0; i < 100; i++) {
        stdLargeFormat += " %s";
    }
    String largeFormat = String(stdLargeFormat);
    
    // This is just a placeholder and will need actual implementation
    // when performance testing is implemented
    std::vector<String> args;
    for (int i = 0; i < 101; i++) {
        // Combine strings using std::string first, then convert to String
        std::string argStr = "arg" + std::to_string(i);
        args.push_back(String(argStr));
    }
    
    // Uncomment when implemented:
    // String result = String::format(largeFormat, args...);
    // EXPECT_EQ(101, std::count(result.toStdString().begin(), result.toStdString().end(), 'a'));
}

} // namespace simple

#include <gtest/gtest.h>
#include <limits>
#include <cmath>
#include "../include/string.hpp"

using namespace simple;

class StringValueOfTest : public ::testing::Test {
protected:
    // Test fixture setup
    StringValueOfTest() {}
};

// Test valueOf(bool)
TEST_F(StringValueOfTest, ValueOfBool) {
    String trueStr = String::valueOf(true);
    String falseStr = String::valueOf(false);
    
    EXPECT_EQ("true", trueStr.to_string());
    EXPECT_EQ("false", falseStr.to_string());
}

// Test valueOf(char)
TEST_F(StringValueOfTest, ValueOfChar) {
    String charA = String::valueOf('A');
    String charZ = String::valueOf('Z');
    String charDigit = String::valueOf('7');
    String charSpecial = String::valueOf('!');
    
    EXPECT_EQ("A", charA.to_string());
    EXPECT_EQ("Z", charZ.to_string());
    EXPECT_EQ("7", charDigit.to_string());
    EXPECT_EQ("!", charSpecial.to_string());
}

// Test valueOf(int)
TEST_F(StringValueOfTest, ValueOfInt) {
    String zero = String::valueOf(0);
    String positive = String::valueOf(42);
    String negative = String::valueOf(-123);
    String maxInt = String::valueOf(std::numeric_limits<int>::max());
    String minInt = String::valueOf(std::numeric_limits<int>::min());
    
    EXPECT_EQ("0", zero.to_string());
    EXPECT_EQ("42", positive.to_string());
    EXPECT_EQ("-123", negative.to_string());
    EXPECT_EQ(std::to_string(std::numeric_limits<int>::max()), maxInt.to_string());
    EXPECT_EQ(std::to_string(std::numeric_limits<int>::min()), minInt.to_string());
}

// Test valueOf(long)
TEST_F(StringValueOfTest, ValueOfLong) {
    String zero = String::valueOf(0L);
    String positive = String::valueOf(42L);
    String negative = String::valueOf(-123L);
    String maxLong = String::valueOf(std::numeric_limits<long>::max());
    String minLong = String::valueOf(std::numeric_limits<long>::min());
    
    EXPECT_EQ("0", zero.to_string());
    EXPECT_EQ("42", positive.to_string());
    EXPECT_EQ("-123", negative.to_string());
    EXPECT_EQ(std::to_string(std::numeric_limits<long>::max()), maxLong.to_string());
    EXPECT_EQ(std::to_string(std::numeric_limits<long>::min()), minLong.to_string());
}

// Test valueOf(float) including special cases
TEST_F(StringValueOfTest, ValueOfFloat) {
    String zero = String::valueOf(0.0f);
    String positive = String::valueOf(3.14159f);
    String negative = String::valueOf(-2.71828f);
    String nan = String::valueOf(std::numeric_limits<float>::quiet_NaN());
    String posInf = String::valueOf(std::numeric_limits<float>::infinity());
    String negInf = String::valueOf(-std::numeric_limits<float>::infinity());
    
    EXPECT_EQ(std::to_string(0.0f), zero.to_string());
    EXPECT_EQ(std::to_string(3.14159f), positive.to_string());
    EXPECT_EQ(std::to_string(-2.71828f), negative.to_string());
    EXPECT_EQ("NaN", nan.to_string());
    EXPECT_EQ("Infinity", posInf.to_string());
    EXPECT_EQ("-Infinity", negInf.to_string());
}

// Test valueOf(double) including special cases
TEST_F(StringValueOfTest, ValueOfDouble) {
    String zero = String::valueOf(0.0);
    String positive = String::valueOf(3.14159265359);
    String negative = String::valueOf(-2.71828182846);
    String nan = String::valueOf(std::numeric_limits<double>::quiet_NaN());
    String posInf = String::valueOf(std::numeric_limits<double>::infinity());
    String negInf = String::valueOf(-std::numeric_limits<double>::infinity());
    
    EXPECT_EQ(std::to_string(0.0), zero.to_string());
    EXPECT_EQ(std::to_string(3.14159265359), positive.to_string());
    EXPECT_EQ(std::to_string(-2.71828182846), negative.to_string());
    EXPECT_EQ("NaN", nan.to_string());
    EXPECT_EQ("Infinity", posInf.to_string());
    EXPECT_EQ("-Infinity", negInf.to_string());
}

// Test generic valueOf with a custom class that has to_string method
class CustomStringable {
public:
    std::string to_string() const {
        return "CustomStringable";
    }
};

// Test generic valueOf with a custom class that has stream insertion operator
class CustomStreamable {
public:
    friend std::ostream& operator<<(std::ostream& os, const CustomStreamable&) {
        return os << "CustomStreamable";
    }
};

// Test generic valueOf with custom types
TEST_F(StringValueOfTest, ValueOfCustomTypes) {
    CustomStringable stringable;
    CustomStreamable streamable;
    
    String fromStringable = String::valueOf(stringable);
    String fromStreamable = String::valueOf(streamable);
    
    EXPECT_EQ("CustomStringable", fromStringable.to_string());
    EXPECT_EQ("CustomStreamable", fromStreamable.to_string());
}

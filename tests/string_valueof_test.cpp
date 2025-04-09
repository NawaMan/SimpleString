#include <gtest/gtest.h>
#include <limits>
#include <cmath>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <variant>
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

// Test valueOf with vector container
TEST_F(StringValueOfTest, ValueOfVector) {
    std::vector<int> emptyVector;
    std::vector<int> intVector = {1, 2, 3, 4, 5};
    std::vector<String> stringVector = {String("hello"), String("world")};
    std::vector<std::vector<int>> nestedVector = {{1, 2}, {3, 4, 5}};
    
    String emptyStr = String::valueOf(emptyVector);
    String intStr = String::valueOf(intVector);
    String stringStr = String::valueOf(stringVector);
    String nestedStr = String::valueOf(nestedVector);
    
    EXPECT_EQ("[]", emptyStr.to_string());
    EXPECT_EQ("[1, 2, 3, 4, 5]", intStr.to_string());
    EXPECT_EQ("[hello, world]", stringStr.to_string());
    EXPECT_EQ("[[1, 2], [3, 4, 5]]", nestedStr.to_string());
}

// Test valueOf with list container
TEST_F(StringValueOfTest, ValueOfList) {
    std::list<int> intList = {10, 20, 30};
    String listStr = String::valueOf(intList);
    EXPECT_EQ("[10, 20, 30]", listStr.to_string());
}

// Test valueOf with set container
TEST_F(StringValueOfTest, ValueOfSet) {
    std::set<int> intSet = {5, 2, 8, 1};  // Will be ordered as {1, 2, 5, 8}
    String setStr = String::valueOf(intSet);
    EXPECT_EQ("[1, 2, 5, 8]", setStr.to_string());
}

// Test valueOf with map container
TEST_F(StringValueOfTest, ValueOfMap) {
    std::map<String, int> scores;
    scores[String("Alice")] = 95;
    scores[String("Bob")] = 87;
    scores[String("Charlie")] = 92;
    
    String scoresStr = String::valueOf(scores);
    EXPECT_EQ("{Alice=95, Bob=87, Charlie=92}", scoresStr.to_string());
    
    // Test empty map
    std::map<int, int> emptyMap;
    String emptyMapStr = String::valueOf(emptyMap);
    EXPECT_EQ("{}", emptyMapStr.to_string());
}

// Test valueOf with unordered_map container
TEST_F(StringValueOfTest, ValueOfUnorderedMap) {
    std::unordered_map<int, String> idToName;
    idToName.insert({1, String("One")});
    idToName.insert({2, String("Two")});
    idToName.insert({3, String("Three")});
    
    String idMapStr = String::valueOf(idToName);
    
    // Since unordered_map doesn't guarantee order, we can't check the exact string
    // Instead, check that it starts with '{', ends with '}', and contains all key-value pairs
    std::string result = idMapStr.to_string();
    EXPECT_TRUE(result[0] == '{' && result[result.length()-1] == '}');
    EXPECT_TRUE(result.find("1=One") != std::string::npos);
    EXPECT_TRUE(result.find("2=Two") != std::string::npos);
    EXPECT_TRUE(result.find("3=Three") != std::string::npos);
}

// Test valueOf with nested containers
TEST_F(StringValueOfTest, ValueOfNestedContainers) {
    std::map<String, std::vector<int>> userData;
    userData[String("Alice")] = {90, 85, 95};
    userData[String("Bob")] = {70, 80, 75};
    
    String nestedStr = String::valueOf(userData);
    EXPECT_EQ("{Alice=[90, 85, 95], Bob=[70, 80, 75]}", nestedStr.to_string());
}

// Test valueOf with std::variant
TEST_F(StringValueOfTest, ValueOfVariant) {
    // Basic variant with primitive types
    std::variant<int, double, String> v1 = 42;
    std::variant<int, double, String> v2 = 3.14;
    std::variant<int, double, String> v3 = String("hello");
    
    String s1 = String::valueOf(v1);
    String s2 = String::valueOf(v2);
    String s3 = String::valueOf(v3);
    
    EXPECT_EQ("42", s1.to_string());
    EXPECT_EQ("3.140000", s2.to_string());  // Double formatting from std::to_string
    EXPECT_EQ("hello", s3.to_string());
    
    // Variant containing containers
    using VariantType = std::variant<int, std::vector<int>, std::map<String, int>>;
    
    VariantType v4 = std::vector<int>{1, 2, 3};
    String s4 = String::valueOf(v4);
    EXPECT_EQ("[1, 2, 3]", s4.to_string());
    
    std::map<String, int> scores;
    scores[String("Alice")] = 95;
    scores[String("Bob")] = 87;
    
    VariantType v5 = scores;
    String s5 = String::valueOf(v5);
    EXPECT_EQ("{Alice=95, Bob=87}", s5.to_string());
    
    // Nested variant
    std::variant<int, std::variant<double, String>> nested1 = 42;
    std::variant<int, std::variant<double, String>> nested2;
    nested2 = std::variant<double, String>(3.14);
    std::variant<int, std::variant<double, String>> nested3;
    nested3 = std::variant<double, String>(String("nested"));
    
    String ns1 = String::valueOf(nested1);
    String ns2 = String::valueOf(nested2);
    String ns3 = String::valueOf(nested3);
    
    EXPECT_EQ("42", ns1.to_string());
    EXPECT_EQ("3.140000", ns2.to_string());
    EXPECT_EQ("nested", ns3.to_string());
}

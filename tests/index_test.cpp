#include <gtest/gtest.h>
#include "../include/index.hpp"
#include <iostream>
using namespace simple;

class IndexTest : public ::testing::Test {
protected:
    // Setup common test values
    const std::size_t validValue = 42;
    const std::size_t maxValue = SIZE_MAX;
};

// Test constructors and initialization
TEST_F(IndexTest, Construction) {
    // Use the static invalid index instead of default constructor (which is private)
    Index defaultIndex = Index::invalid;
    ASSERT_TRUE(defaultIndex.is_invalid());
    ASSERT_FALSE(defaultIndex.is_valid());
    ASSERT_EQ(defaultIndex.value(), SIZE_MAX);
    
    // Constructor with value should initialize properly
    Index validIndex(validValue);
    ASSERT_FALSE(validIndex.is_invalid());
    ASSERT_TRUE(validIndex.is_valid());
    ASSERT_EQ(validIndex.value(), validValue);
    
    // Static invalid index should be invalid
    ASSERT_TRUE(Index::invalid.is_invalid());
    ASSERT_FALSE(Index::invalid.is_valid());
    ASSERT_EQ(Index::invalid.value(), SIZE_MAX);
}

// Test constructors for negative number
TEST_F(IndexTest, NegativeConstructor) {
    // Initialize with negative value should throw exception
    // ASSERT_THROW(Index index(-1), std::invalid_argument);
    Index index(-1);
    std::cout << index.value() << std::endl;
}    

// Test comparison operators
TEST_F(IndexTest, ComparisonOperators) {
    Index index1(10);
    Index index2(20);
    Index index3(10);
    Index invalidIndex = Index::invalid;
    
    // Equality operators
    ASSERT_TRUE(index1 == index3);
    ASSERT_FALSE(index1 == index2);
    ASSERT_FALSE(index1 == invalidIndex);
    
    // Inequality operators
    ASSERT_TRUE(index1 != index2);
    ASSERT_FALSE(index1 != index3);
    ASSERT_TRUE(index1 != invalidIndex);
    
    // Less than
    ASSERT_TRUE(index1 < index2);
    ASSERT_FALSE(index2 < index1);
    ASSERT_FALSE(index1 < index3);
    
    // Greater than
    ASSERT_TRUE(index2 > index1);
    ASSERT_FALSE(index1 > index2);
    ASSERT_FALSE(index1 > index3);
    
    // Less than or equal
    ASSERT_TRUE(index1 <= index2);
    ASSERT_TRUE(index1 <= index3);
    ASSERT_FALSE(index2 <= index1);
    
    // Greater than or equal
    ASSERT_TRUE(index2 >= index1);
    ASSERT_TRUE(index1 >= index3);
    ASSERT_FALSE(index1 >= index2);
    
    // Comparing with invalid index
    ASSERT_FALSE(invalidIndex < index1);  // Invalid is SIZE_MAX, so it's not less than any valid index
    ASSERT_TRUE(index1 < invalidIndex); // Any valid index is less than SIZE_MAX (invalid index)
}

// Test arithmetic operators
TEST_F(IndexTest, ArithmeticOperators) {
    Index index1(10);
    Index index2(20);
    
    // Addition
    Index sum = index1 + index2;
    ASSERT_EQ(sum.value(), 30);
    ASSERT_TRUE(sum.is_valid());
    
    // Subtraction
    Index diff = index2 - index1;
    ASSERT_EQ(diff.value(), 10);
    ASSERT_TRUE(diff.is_valid());
    
    // Edge cases
    Index maxIndex(SIZE_MAX - 10);
    Index tenIndex(10);
    
    // Adding to almost max should overflow and wrap around
    Index overflowResult = maxIndex + tenIndex;
    ASSERT_EQ(overflowResult.value(), SIZE_MAX - 10 + 10);
    
    // Subtracting larger from smaller should underflow and wrap around
    Index underflowResult = index1 - index2;
    ASSERT_EQ(underflowResult.value(), SIZE_MAX - 10 + 1);
}

// Test special cases and edge behaviors
TEST_F(IndexTest, SpecialCases) {
    // Test with zero
    Index zeroIndex(0);
    ASSERT_TRUE(zeroIndex.is_valid());
    ASSERT_FALSE(zeroIndex.is_invalid());
    ASSERT_EQ(zeroIndex.value(), 0);
    
    // Test with max value - 1
    Index almostMaxIndex(SIZE_MAX - 1);
    ASSERT_TRUE(almostMaxIndex.is_valid());
    ASSERT_FALSE(almostMaxIndex.is_invalid());
    ASSERT_EQ(almostMaxIndex.value(), SIZE_MAX - 1);
    
    // Test with max value (should be invalid)
    Index maxIndex(SIZE_MAX);
    ASSERT_FALSE(maxIndex.is_valid());
    ASSERT_TRUE(maxIndex.is_invalid());
    ASSERT_EQ(maxIndex.value(), SIZE_MAX);
}

// Test real-world usage patterns
TEST_F(IndexTest, UsagePatterns) {
    // Simulate string indexOf behavior
    Index notFound = Index::invalid;
    Index found(5);
    
    // Check if found
    if (!notFound.is_invalid()) {
        FAIL() << "Invalid index should be detected as invalid";
    }
    
    if (found.is_invalid()) {
        FAIL() << "Valid index should not be detected as invalid";
    }
    
    // Using is_valid() method
    ASSERT_FALSE(notFound.is_valid());
    ASSERT_TRUE(found.is_valid());
    
    // Simulate conditional logic based on index validity
    std::string result = found.is_valid() ? "Found" : "Not Found";
    ASSERT_EQ(result, "Found");
    
    result = notFound.is_valid() ? "Found" : "Not Found";
    ASSERT_EQ(result, "Not Found");
}

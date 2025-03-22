#include <gtest/gtest.h>
#include "compare_result.hpp"

// Task-003: String Sharing Optimization

using namespace simple_string;

TEST(CompareResultTest, StaticConstants) {
    // Test static constant values
    EXPECT_TRUE (CompareResult::LESS.isLess());
    EXPECT_FALSE(CompareResult::LESS.isEqual());
    EXPECT_FALSE(CompareResult::LESS.isGreater());

    EXPECT_FALSE(CompareResult::EQUAL.isLess());
    EXPECT_TRUE (CompareResult::EQUAL.isEqual());
    EXPECT_FALSE(CompareResult::EQUAL.isGreater());

    EXPECT_FALSE(CompareResult::GREATER.isLess());
    EXPECT_FALSE(CompareResult::GREATER.isEqual());
    EXPECT_TRUE (CompareResult::GREATER.isGreater());

    // Test comparison with integers
    EXPECT_TRUE(CompareResult::LESS    <  0);
    EXPECT_TRUE(CompareResult::EQUAL   == 0);
    EXPECT_TRUE(CompareResult::GREATER >  0);
}

TEST(CompareResultTest, StaticFactoryMethod) {
    CompareResult less = CompareResult::fromInt(-1);
    CompareResult equal = CompareResult::fromInt(0);
    CompareResult greater = CompareResult::fromInt(1);

    EXPECT_TRUE (less.isLess());
    EXPECT_FALSE(less.isEqual());
    EXPECT_FALSE(less.isGreater());
    EXPECT_TRUE (less.isLessOrEqual());
    EXPECT_FALSE(less.isGreaterOrEqual());

    EXPECT_FALSE(equal.isLess());
    EXPECT_TRUE (equal.isEqual());
    EXPECT_FALSE(equal.isGreater());
    EXPECT_TRUE (equal.isLessOrEqual());
    EXPECT_TRUE (equal.isGreaterOrEqual());

    EXPECT_FALSE(greater.isLess());
    EXPECT_FALSE(greater.isEqual());
    EXPECT_TRUE (greater.isGreater());
    EXPECT_FALSE(greater.isLessOrEqual());
    EXPECT_TRUE (greater.isGreaterOrEqual());
}

TEST(CompareResultTest, ArbitraryValues) {
    // Any negative value should be "less"
    CompareResult veryNegative = CompareResult::fromInt(-100);
    EXPECT_TRUE(veryNegative.isLess());
    EXPECT_TRUE(veryNegative.isLessOrEqual());
    EXPECT_FALSE(veryNegative.isGreater());
    EXPECT_FALSE(veryNegative.isGreaterOrEqual());
    EXPECT_FALSE(veryNegative.isEqual());

    // Any positive value should be "greater"
    CompareResult veryPositive(100);
    EXPECT_FALSE(veryPositive.isLess());
    EXPECT_FALSE(veryPositive.isLessOrEqual());
    EXPECT_TRUE(veryPositive.isGreater());
    EXPECT_TRUE(veryPositive.isGreaterOrEqual());
    EXPECT_FALSE(veryPositive.isEqual());
}

TEST(CompareResultTest, RawValue) {
    // Test that value() returns the original comparison value
    CompareResult less = CompareResult::fromInt(-42);
    CompareResult equal = CompareResult::fromInt(0);
    CompareResult greater = CompareResult::fromInt(42);

    EXPECT_EQ(less.value(), -42);
    EXPECT_EQ(equal.value(), 0);
    EXPECT_EQ(greater.value(), 42);
}

TEST(CompareResultTest, IntegerComparison) {
    CompareResult less = CompareResult::fromInt(-1);
    CompareResult equal = CompareResult::fromInt(0);
    CompareResult greater = CompareResult::fromInt(1);

    // Test CompareResult op int
    EXPECT_TRUE (less <  0);
    EXPECT_TRUE (less <= 0);
    EXPECT_FALSE(less >  0);
    EXPECT_FALSE(less >= 0);
    EXPECT_FALSE(less == 0);
    EXPECT_TRUE (less != 0);

    EXPECT_FALSE(equal <  0);
    EXPECT_TRUE (equal <= 0);
    EXPECT_FALSE(equal >  0);
    EXPECT_TRUE (equal >= 0);
    EXPECT_TRUE (equal == 0);
    EXPECT_FALSE(equal != 0);

    EXPECT_FALSE(greater <  0);
    EXPECT_FALSE(greater <= 0);
    EXPECT_TRUE (greater >  0);
    EXPECT_TRUE (greater >= 0);
    EXPECT_FALSE(greater == 0);
    EXPECT_TRUE (greater != 0);

    // Test int op CompareResult
    EXPECT_TRUE (0 >  less);
    EXPECT_TRUE (0 >= less);
    EXPECT_FALSE(0 <  less);
    EXPECT_FALSE(0 <= less);
    EXPECT_FALSE(0 == less);
    EXPECT_TRUE (0 != less);

    EXPECT_FALSE(0 >  equal);
    EXPECT_TRUE (0 >= equal);
    EXPECT_FALSE(0 <  equal);
    EXPECT_TRUE (0 <= equal);
    EXPECT_TRUE (0 == equal);
    EXPECT_FALSE(0 != equal);

    EXPECT_FALSE(0 >  greater);
    EXPECT_FALSE(0 >= greater);
    EXPECT_TRUE (0 <  greater);
    EXPECT_TRUE (0 <= greater);
    EXPECT_FALSE(0 == greater);
    EXPECT_TRUE (0 != greater);
}

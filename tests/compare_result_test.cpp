#include <gtest/gtest.h>
#include "compare_result.hpp"

using namespace simple_string;

TEST(CompareResultTest, ImplicitConversion) {
    CompareResult less(-1);
    CompareResult equal(0);
    CompareResult greater(1);

    EXPECT_TRUE(less.isLess());
    EXPECT_FALSE(less.isEqual());
    EXPECT_FALSE(less.isGreater());
    EXPECT_TRUE(less.isLessOrEqual());
    EXPECT_FALSE(less.isGreaterOrEqual());

    EXPECT_FALSE(equal.isLess());
    EXPECT_TRUE(equal.isEqual());
    EXPECT_FALSE(equal.isGreater());
    EXPECT_TRUE(equal.isLessOrEqual());
    EXPECT_TRUE(equal.isGreaterOrEqual());

    EXPECT_FALSE(greater.isLess());
    EXPECT_FALSE(greater.isEqual());
    EXPECT_TRUE(greater.isGreater());
    EXPECT_FALSE(greater.isLessOrEqual());
    EXPECT_TRUE(greater.isGreaterOrEqual());
}

TEST(CompareResultTest, ArbitraryValues) {
    // Any negative value should be "less"
    CompareResult veryNegative(-100);
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
    CompareResult less(-42);
    CompareResult equal(0);
    CompareResult greater(42);

    EXPECT_EQ(less.value(), -42);
    EXPECT_EQ(equal.value(), 0);
    EXPECT_EQ(greater.value(), 42);
}

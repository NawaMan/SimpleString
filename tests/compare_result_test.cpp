#include <gtest/gtest.h>
#include "compare_result.hpp"

// Task-003: String Sharing Optimization

using namespace simple_string;

TEST(CompareResultTest, StaticConstants) {
    // Test static constant values
    EXPECT_TRUE (CompareResult::LESS.is_less());
    EXPECT_FALSE(CompareResult::LESS.is_equal());
    EXPECT_FALSE(CompareResult::LESS.is_greater());

    EXPECT_FALSE(CompareResult::EQUAL.is_less());
    EXPECT_TRUE (CompareResult::EQUAL.is_equal());
    EXPECT_FALSE(CompareResult::EQUAL.is_greater());

    EXPECT_FALSE(CompareResult::GREATER.is_less());
    EXPECT_FALSE(CompareResult::GREATER.is_equal());
    EXPECT_TRUE (CompareResult::GREATER.is_greater());

    // Test comparison with integers
    EXPECT_TRUE(CompareResult::LESS    <  0);
    EXPECT_TRUE(CompareResult::EQUAL   == 0);
    EXPECT_TRUE(CompareResult::GREATER >  0);
}

TEST(CompareResultTest, StaticFactoryMethod) {
    CompareResult less    = CompareResult::from_int(-1);
    CompareResult equal   = CompareResult::from_int(0);
    CompareResult greater = CompareResult::from_int(1);

    EXPECT_TRUE (less.is_less());
    EXPECT_FALSE(less.is_equal());
    EXPECT_FALSE(less.is_greater());
    EXPECT_TRUE (less.is_less_or_equal());
    EXPECT_FALSE(less.is_greater_or_equal());

    EXPECT_FALSE(equal.is_less());
    EXPECT_TRUE (equal.is_equal());
    EXPECT_FALSE(equal.is_greater());
    EXPECT_TRUE (equal.is_less_or_equal());
    EXPECT_TRUE (equal.is_greater_or_equal());

    EXPECT_FALSE(greater.is_less());
    EXPECT_FALSE(greater.is_equal());
    EXPECT_TRUE (greater.is_greater());
    EXPECT_FALSE(greater.is_less_or_equal());
    EXPECT_TRUE (greater.is_greater_or_equal());
}

TEST(CompareResultTest, ArbitraryValues) {
    // Any negative value should be "less"
    CompareResult very_negative = CompareResult::from_int(-100);
    EXPECT_TRUE(very_negative.is_less());
    EXPECT_TRUE(very_negative.is_less_or_equal());
    EXPECT_FALSE(very_negative.is_greater());
    EXPECT_FALSE(very_negative.is_greater_or_equal());
    EXPECT_FALSE(very_negative.is_equal());

    // Any positive value should be "greater"
    CompareResult very_positive = CompareResult::from_int(100);
    EXPECT_FALSE(very_positive.is_less());
    EXPECT_FALSE(very_positive.is_less_or_equal());
    EXPECT_TRUE(very_positive.is_greater());
    EXPECT_TRUE(very_positive.is_greater_or_equal());
    EXPECT_FALSE(very_positive.is_equal());
}

TEST(CompareResultTest, RawValue) {
    // Test that value() returns the original comparison value
    CompareResult less    = CompareResult::from_int(-42);
    CompareResult equal   = CompareResult::from_int(0);
    CompareResult greater = CompareResult::from_int(42);

    EXPECT_EQ(less.value(), -42);
    EXPECT_EQ(equal.value(), 0);
    EXPECT_EQ(greater.value(), 42);
}

TEST(CompareResultTest, IntegerComparison) {
    CompareResult less    = CompareResult::from_int(-1);
    CompareResult equal   = CompareResult::from_int(0);
    CompareResult greater = CompareResult::from_int(1);

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

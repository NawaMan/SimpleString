#ifndef SIMPLE_STRING_COMPARE_RESULT_HPP
#define SIMPLE_STRING_COMPARE_RESULT_HPP

namespace simple_string {

/**
 * CompareResult - A wrapper for string comparison results that provides
 * readable methods for checking the comparison outcome.
 * 
 * This class implicitly converts from the int value returned by compareTo()
 * and provides methods like isLess(), isGreater(), etc. for better readability.
 */
class CompareResult {
public:
    // Implicit conversion constructor from int
    CompareResult(int value) : value_(value) {}

    // Comparison check methods
    bool isLess()           const { return value_ <  0; }
    bool isGreater()        const { return value_ >  0; }
    bool isEqual()          const { return value_ == 0; }
    bool isLessOrEqual()    const { return value_ <= 0; }
    bool isGreaterOrEqual() const { return value_ >= 0; }

    // Get the raw comparison value
    int value() const { return value_; }

private:
    int value_;  // The raw comparison result
};

} // namespace simple_string

#endif // SIMPLE_STRING_COMPARE_RESULT_HPP

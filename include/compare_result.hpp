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
    // Static factory method to create CompareResult from int (Java-style)
    static CompareResult fromInt(int value) {
        return CompareResult(value);
    }

    // Constructor is now explicit to prevent implicit conversions
    explicit CompareResult(int value) : value_(value) {}

    // Comparison check methods
    bool isLess()           const { return value_ <  0; }
    bool isGreater()        const { return value_ >  0; }
    bool isEqual()          const { return value_ == 0; }
    bool isLessOrEqual()    const { return value_ <= 0; }
    bool isGreaterOrEqual() const { return value_ >= 0; }

    // Get the raw comparison value
    int value() const { return value_; }

    // Comparison operators for comparing with integers
    bool operator==(int other) const { return value_ == other; }
    bool operator!=(int other) const { return value_ != other; }
    bool operator< (int other) const { return value_ <  other; }
    bool operator<=(int other) const { return value_ <= other; }
    bool operator> (int other) const { return value_ >  other; }
    bool operator>=(int other) const { return value_ >= other; }

    // Friend operators for reverse comparisons (int op CompareResult)
    friend bool operator==(int lhs, const CompareResult& rhs) { return rhs == lhs; }
    friend bool operator!=(int lhs, const CompareResult& rhs) { return rhs != lhs; }
    friend bool operator< (int lhs, const CompareResult& rhs) { return rhs >  lhs; }
    friend bool operator<=(int lhs, const CompareResult& rhs) { return rhs >= lhs; }
    friend bool operator> (int lhs, const CompareResult& rhs) { return rhs <  lhs; }
    friend bool operator>=(int lhs, const CompareResult& rhs) { return rhs <= lhs; }

private:
    int value_;  // The raw comparison result
};

} // namespace simple_string

#endif // SIMPLE_STRING_COMPARE_RESULT_HPP

#ifndef MOSAIC_COMPARE_RESULT_HPP
#define MOSAIC_COMPARE_RESULT_HPP

namespace mosaic {

/**
 * CompareResult - A wrapper for string comparison results that provides
 * readable methods for checking the comparison outcome.
 * 
 * This class implicitly converts from the int value returned by compareTo()
 * and provides methods like isLess(), isGreater(), etc. for better readability.
 */
class CompareResult {
public:
    // Static constant instances for common comparison results
    static const CompareResult LESS;
    static const CompareResult EQUAL;
    static const CompareResult GREATER;

    // Static factory method to create CompareResult from int (Java-style)
    static CompareResult from_int(int value) {
        return CompareResult(value);
    }

    // Constructor is now explicit to prevent implicit conversions
    explicit CompareResult(int value) : value_(value) {}

    // Comparison check methods
    bool is_less()             const { return value_ <  0; }
    bool is_greater()          const { return value_ >  0; }
    bool is_equal()            const { return value_ == 0; }
    bool is_less_or_equal()    const { return value_ <= 0; }
    bool is_greater_or_equal() const { return value_ >= 0; }

    // Get the raw comparison value
    int value() const { return value_; }

    // Comparison operators for comparing with integers
    bool operator==(int other) const { return value_ == other; }
    bool operator!=(int other) const { return value_ != other; }
    bool operator< (int other) const { return value_ <  other; }
    bool operator<=(int other) const { return value_ <= other; }
    bool operator> (int other) const { return value_ >  other; }
    bool operator>=(int other) const { return value_ >= other; }

    // Comparison with other CompareResult instances
    bool operator==(const CompareResult& other) const { return value_ == other.value_; }
    bool operator!=(const CompareResult& other) const { return value_ != other.value_; }
    bool operator< (const CompareResult& other) const { return value_ <  other.value_; }
    bool operator<=(const CompareResult& other) const { return value_ <= other.value_; }
    bool operator> (const CompareResult& other) const { return value_ >  other.value_; }
    bool operator>=(const CompareResult& other) const { return value_ >= other.value_; }

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

} // namespace mosaic

#endif // MOSAIC_COMPARE_RESULT_HPP

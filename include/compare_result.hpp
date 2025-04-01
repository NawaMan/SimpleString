#pragma once

namespace simple {

/**
 * CompareResult - A wrapper for string comparison results that provides
 * readable methods for checking the comparison outcome.
 * 
 * This class encapsulates the integer result of a string comparison operation
 * and provides descriptive methods for checking the comparison outcome.
 * 
 * Instead of checking raw integer values like:
 * @code
 * if (str1.compare_to(str2) < 0) { ... }
 * @endcode
 * 
 * You can use more readable code:
 * @code
 * if (str1.compare_to(str2).is_less()) { ... }
 * @endcode
 * 
 * The class also provides static constants for common comparison results
 * (LESS, EQUAL, GREATER) and a factory method for creating instances.
 */
class CompareResult {
public:
    /**
     * Static constant instances for common comparison results.
     * These provide convenient access to frequently used comparison results.
     */
    static const CompareResult LESS;    ///< Represents a 'less than' comparison result (-1)
    static const CompareResult EQUAL;   ///< Represents an 'equal to' comparison result (0)
    static const CompareResult GREATER; ///< Represents a 'greater than' comparison result (1)

    /**
     * Static factory method to create a CompareResult from an integer value.
     * 
     * This method follows the Java-style factory pattern for creating objects.
     * 
     * @param value The integer comparison value to wrap
     * @return A CompareResult instance representing the given value
     */
    static CompareResult from_int(int value);

    /**
     * Constructor that creates a CompareResult from an integer value.
     * 
     * This constructor is explicit to prevent unintended implicit conversions
     * from integer values to CompareResult objects.
     * 
     * @param value The integer comparison value to wrap
     */
    explicit CompareResult(int value) : value_(value) {}

    /**
     * Comparison check methods that provide readable ways to check the comparison result.
     */
    bool is_less()             const { return value_ <  0; } ///< @return true if this result represents 'less than'
    bool is_greater()          const { return value_ >  0; } ///< @return true if this result represents 'greater than'
    bool is_equal()            const { return value_ == 0; } ///< @return true if this result represents 'equal to'
    bool is_less_or_equal()    const { return value_ <= 0; } ///< @return true if this result represents 'less than or equal to'
    bool is_greater_or_equal() const { return value_ >= 0; } ///< @return true if this result represents 'greater than or equal to'

    /**
     * Gets the raw integer comparison value.
     * 
     * @return The underlying integer value of this comparison result
     */
    int value() const { return value_; }

    /**
     * Comparison operators for comparing with integers.
     * These allow direct comparison between a CompareResult and an integer.
     */
    bool operator==(int other) const { return value_ == other; } ///< Equality comparison with an integer
    bool operator!=(int other) const { return value_ != other; } ///< Inequality comparison with an integer
    bool operator< (int other) const { return value_ <  other; } ///< Less than comparison with an integer
    bool operator<=(int other) const { return value_ <= other; } ///< Less than or equal comparison with an integer
    bool operator> (int other) const { return value_ >  other; } ///< Greater than comparison with an integer
    bool operator>=(int other) const { return value_ >= other; } ///< Greater than or equal comparison with an integer

    /**
     * Comparison operators for comparing with other CompareResult instances.
     * These allow direct comparison between two CompareResult objects.
     */
    bool operator==(const CompareResult& other) const { return value_ == other.value_; } ///< Equality comparison with another CompareResult
    bool operator!=(const CompareResult& other) const { return value_ != other.value_; } ///< Inequality comparison with another CompareResult
    bool operator< (const CompareResult& other) const { return value_ <  other.value_; } ///< Less than comparison with another CompareResult
    bool operator<=(const CompareResult& other) const { return value_ <= other.value_; } ///< Less than or equal comparison with another CompareResult
    bool operator> (const CompareResult& other) const { return value_ >  other.value_; } ///< Greater than comparison with another CompareResult
    bool operator>=(const CompareResult& other) const { return value_ >= other.value_; } ///< Greater than or equal comparison with another CompareResult

    /**
     * Friend operators for reverse comparisons (int op CompareResult).
     * These allow direct comparison between an integer and a CompareResult.
     */
    friend bool operator==(int lhs, const CompareResult& rhs) { return rhs == lhs; } ///< Equality comparison (int == CompareResult)
    friend bool operator!=(int lhs, const CompareResult& rhs) { return rhs != lhs; } ///< Inequality comparison (int != CompareResult)
    friend bool operator< (int lhs, const CompareResult& rhs) { return rhs >  lhs; } ///< Less than comparison (int < CompareResult)
    friend bool operator<=(int lhs, const CompareResult& rhs) { return rhs >= lhs; } ///< Less than or equal comparison (int <= CompareResult)
    friend bool operator> (int lhs, const CompareResult& rhs) { return rhs <  lhs; } ///< Greater than comparison (int > CompareResult)
    friend bool operator>=(int lhs, const CompareResult& rhs) { return rhs <= lhs; } ///< Greater than or equal comparison (int >= CompareResult)

private:
    int value_;  ///< The raw integer comparison result
};

} // namespace simple

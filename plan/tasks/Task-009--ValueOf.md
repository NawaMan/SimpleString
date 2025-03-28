# Task-009: Implement toString/valueOf Methods for Primitive Types

## Objective
Implement Java-like static valueOf() and toString() methods for converting primitive types to String objects.

## Requirements

1. Core Methods (Java API)
   - [ ] `static String valueOf(bool b)`: Returns a String representation of the boolean argument
   - [ ] `static String valueOf(char c)`: Returns a String representation of the char argument
   - [ ] `static String valueOf(int i)`: Returns a String representation of the int argument
   - [ ] `static String valueOf(long l)`: Returns a String representation of the long argument
   - [ ] `static String valueOf(float f)`: Returns a String representation of the float argument
   - [ ] `static String valueOf(double d)`: Returns a String representation of the double argument


2. Behavior Requirements
   - [ ] Match Java's string conversion behavior for each primitive type
   - [ ] Handle special cases (NaN, infinity) for floating-point types
   - [ ] Provide consistent formatting across platforms
   - [ ] Maintain immutability of String objects

## Implementation Plan

1. Public Interface
   ```cpp
   class String {
   public:
       // Static valueOf methods for primitive types
       static String valueOf(bool b);
       static String valueOf(char c);
       static String valueOf(int i);
       static String valueOf(long l);
       static String valueOf(float f);
       static String valueOf(double d);
       
       // Generic valueOf method for any type that can be converted to string
       template<typename T>
       static String valueOf(const T& obj) {
           return String(to_string(obj));
       }
       
       // Helper function to convert any type to string (will be specialized for different types)
       template<typename T>
       static std::string to_string(const T& obj);
   };
   ```

2. Implementation Strategy
   - Use `std::to_string()` as the base for numeric conversions
   - Implement custom formatting for floating-point types to match Java's behavior
   - Use C++20 concepts to constrain the template parameters:
     ```cpp
     // Concept for types that have a to_string method
     template<typename T>
     concept HasToString = requires(const T& t) {
         { t.to_string() } -> std::convertible_to<std::string>;
     };
     
     // Concept for types that support stream insertion
     template<typename T>
     concept Streamable = requires(std::ostream& os, const T& t) {
         { os << t } -> std::convertible_to<std::ostream&>;
     };
     
     // Specializations based on concepts
     template<HasToString T>
     static std::string to_string(const T& obj) {
         return obj.to_string();
     }
     
     template<Streamable T>
     static std::string to_string(const T& obj) {
         std::ostringstream oss;
         oss << obj;
         return oss.str();
     }
     ```

## Testing Strategy

1. Basic Tests
   - Test conversion of each primitive type
   - Verify string representation matches Java's output
   - Test boundary values (min/max values for numeric types)

2. Special Cases
   - Test NaN and infinity for float and double
   - Test various precision values for floating-point types

3. Edge Cases
   - Test very large and very small numeric values

## Implementation Details

1. For Boolean Values
   ```cpp
   String String::valueOf(bool b) {
       return b ? String("true") : String("false");
   }
   ```

2. For Numeric Types
   ```cpp
   String String::valueOf(int i) {
       return String(std::to_string(i));
   }
   ```

3. For Floating-Point Types (requiring special handling)
   ```cpp
   String String::valueOf(double d) {
       if (std::isnan(d)) {
           return String("NaN");
       } else if (std::isinf(d)) {
           return d > 0 ? String("Infinity") : String("-Infinity");
       } else {
           // Format to match Java's behavior
           // This will need custom implementation
           return String(std::to_string(d));
       }
   }
   ```

4. For Generic Types (using concepts)
   ```cpp
   // Primary template (fallback for types that don't match any concept)
   template<typename T>
   std::string String::to_string(const T& obj) {
       // Default implementation could use type_info name or static_assert
       static_assert(always_false<T>::value, "No conversion to string available for this type");
   }
   
   // Specialization for types with to_string method
   template<HasToString T>
   std::string String::to_string(const T& obj) {
       return obj.to_string();
   }
   
   // Specialization for streamable types
   template<Streamable T>
   std::string String::to_string(const T& obj) {
       std::ostringstream oss;
       oss << obj;
       return oss.str();
   }
   ```

## References

1. Java API Documentation
   - [String.valueOf(boolean)](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/String.html#valueOf(boolean))
   - [String.valueOf(int)](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/String.html#valueOf(int))
   - [String.valueOf(double)](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/String.html#valueOf(double))

2. C++ Standard Library
   - [std::to_string](https://en.cppreference.com/w/cpp/string/basic_string/to_string)
   - [std::isnan, std::isinf](https://en.cppreference.com/w/cpp/numeric/math/isnan)

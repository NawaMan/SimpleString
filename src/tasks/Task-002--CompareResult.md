# Task-002: CompareResult

## Motivation

`compareTo()` returns an integer that indicates the relative order of the strings
  which is sometimes hard to remember.
It should be good to have a `CompareResult` class to make the code more readable.

## Description

Add CompareResult class that can implicitly convert from int -- so that it can be used in the same way as the return value of `compareTo()`. 
Add some method to make the code more readable such as:

- isLess()
- isGreater()
- isEqual()
- isLessOrEqual()
- isGreaterOrEqual()


## Acceptance Criteria

- [x] Add CompareResult class
- [x] Add implicit conversion from int to CompareResult (later changed to explicit with fromInt())
- [x] Add isLess() method
- [x] Add isGreater() method
- [x] Add isEqual() method
- [x] Add isLessOrEqual() method
- [x] Add isGreaterOrEqual() method


## Additional Completed Features

- [x] Added comparison operators (==, !=, <, <=, >, >=) for comparing with integers
- [x] Added friend operators for reverse comparisons (int op CompareResult)
- [x] Added static factory method fromInt() for Java-style creation
- [x] Added static constants (LESS, EQUAL, GREATER) for readability
- [x] Comprehensive test suite covering all functionality
- [x] Documentation for all methods and constants

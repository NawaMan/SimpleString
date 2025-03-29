# Task-011: Implement String Matching Methods

## Objective
Implement Java-like string matching methods (contains, startsWith, endsWith) for the String class, providing efficient ways to check if a string contains, starts with, or ends with a specific substring.

## Requirements

1. Core Methods (Java API)
   - [ ] `bool contains(const String& str) const`: Returns true if and only if this string contains the specified sequence of char values
   - [ ] `bool startsWith(const String& prefix) const`: Tests if this string starts with the specified prefix
   - [ ] `bool startsWith(const String& prefix, Index offset) const`: Tests if the substring of this string beginning at the specified index starts with the specified prefix
   - [ ] `bool endsWith(const String& suffix) const`: Tests if this string ends with the specified suffix

2. Behavior Requirements
   - [ ] Match Java's UTF-16 code unit indexing for all operations
   - [ ] Properly handle empty strings and empty search strings
   - [ ] Handle surrogate pairs correctly
   - [ ] Maintain consistent behavior with Java's String implementation
   - [ ] Optimize for performance where possible

## Implementation Plan

1. Public Interface
   ```cpp
   class String {
   public:
       /**
        * Returns true if and only if this string contains the specified sequence of char values.
        * 
        * @param str the sequence to search for
        * @return true if this string contains the specified sequence, false otherwise
        */
       bool contains(const String& str) const;
       
       /**
        * Tests if this string starts with the specified prefix.
        * 
        * @param prefix the prefix
        * @return true if the character sequence represented by the argument is a prefix of the
        *         character sequence represented by this string; false otherwise
        */
       bool startsWith(const String& prefix) const;
       
       /**
        * Tests if the substring of this string beginning at the specified index starts with the specified prefix.
        * 
        * @param prefix the prefix
        * @param offset where to begin looking in this string
        * @return true if the character sequence represented by the argument is a prefix of the
        *         substring of this object starting at index offset; false otherwise
        * @throws StringIndexOutOfBoundsException if offset is negative or greater than the length of this string
        */
       bool startsWith(const String& prefix, Index offset) const;
       
       /**
        * Tests if this string ends with the specified suffix.
        * 
        * @param suffix the suffix
        * @return true if the character sequence represented by the argument is a suffix of the
        *         character sequence represented by this object; false otherwise
        */
       bool endsWith(const String& suffix) const;
   };
   ```

2. Implementation Strategy
   - Leverage existing methods where appropriate (e.g., indexOf for contains)
   - Implement startsWith and endsWith with direct character comparisons
   - Ensure proper bounds checking and exception handling
   - Optimize for common cases (empty strings, single character matches)

3. Testing Strategy
   - Unit tests for each method with various inputs
   - Edge cases: empty strings, single character strings, surrogate pairs
   - Performance tests for large strings
   - Verify behavior matches Java's implementation for various scenarios

## Implementation Details

1. Contains Implementation
   - Utilize the existing indexOf method: `return indexOf(str) != Index::invalid;`
   - This reuses the existing substring search logic

2. StartsWith Implementation
   - For the no-offset version: `return startsWith(prefix, Index(0));`
   - For the offset version:
     - Check bounds and throw if offset is invalid
     - Compare characters directly from the offset position
     - Return early if lengths don't match

3. EndsWith Implementation
   - Calculate the starting position for comparison: `this.length() - suffix.length()`
   - Return false if suffix is longer than the string
   - Compare characters directly from the calculated position

## Optimization Considerations
   - Cache the UTF-16 representation to avoid repeated conversions
   - Consider special handling for ASCII-only strings
   - Use early returns for common cases (empty strings, length mismatches)

## Implementation Steps

1. Add method declarations to string.hpp
2. Implement methods in string.cpp
3. Add comprehensive unit tests in tests/string_matching_test.cpp
4. Optimize the implementation for performance
5. Document the behavior and usage in comments

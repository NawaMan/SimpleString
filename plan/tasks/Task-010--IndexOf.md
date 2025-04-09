# Task-010: Implement Java String Index Functions

## Objective
Implement Java's String index functions (indexOf, lastIndexOf) for the String class, matching Java's behavior for finding the first or last occurrence of a substring or character.

## Requirements

1. Core Methods (Java API)
   - [x] `indexOf(Char ch)`: Returns the index within this string of the first occurrence of the specified character
   - [x] `indexOf(Char ch, Size fromIndex)`: Returns the index within this string of the first occurrence of the specified character, starting the search at the specified index
   - [x] `indexOf(const String& str)`: Returns the index within this string of the first occurrence of the specified substring
   - [x] `indexOf(const String& str, Size fromIndex)`: Returns the index within this string of the first occurrence of the specified substring, starting at the specified index
   - [x] `lastIndexOf(Char ch)`: Returns the index within this string of the last occurrence of the specified character
   - [x] `lastIndexOf(Char ch, Size fromIndex)`: Returns the index within this string of the last occurrence of the specified character, searching backward starting at the specified index
   - [x] `lastIndexOf(const String& str)`: Returns the index within this string of the last occurrence of the specified substring
   - [x] `lastIndexOf(const String& str, Size fromIndex)`: Returns the index within this string of the last occurrence of the specified substring, searching backward starting at the specified index

2. Behavior Requirements
   - [x] Match Java's UTF-16 code unit indexing
   - [x] Return Size::npos when the substring or character is not found
   - [x] Handle surrogate pairs correctly
   - [x] Properly handle empty strings and empty search strings
   - [x] Maintain consistent behavior with Java's String implementation

## Implementation Plan

1. Public Interface
   ```cpp
   class String {
   public:
       /**
        * Returns the index within this string of the first occurrence of the specified character.
        * 
        * @param ch a character
        * @return the index of the first occurrence of the character in the string,
        *         or Size::npos if the character does not occur
        */
       Size indexOf(Char ch) const;
       
       /**
        * Returns the index within this string of the first occurrence of the specified character,
        * starting the search at the specified index.
        *
        * @param ch a character
        * @param fromIndex the index to start the search from
        * @return the index of the first occurrence of the character in the string,
        *         or Size::npos if the character does not occur
        */
       Size indexOf(Char ch, Size fromIndex) const;
       
       /**
        * Returns the index within this string of the first occurrence of the specified substring.
        *
        * @param str the substring to search for
        * @return the index of the first occurrence of the specified substring,
        *         or Size::npos if there is no such occurrence
        */
       Size indexOf(const String& str) const;
       
       /**
        * Returns the index within this string of the first occurrence of the specified substring,
        * starting at the specified index.
        *
        * @param str the substring to search for
        * @param fromIndex the index to start the search from
        * @return the index of the first occurrence of the specified substring,
        *         starting at the specified index, or Size::npos if there is no such occurrence
        */
       Size indexOf(const String& str, Size fromIndex) const;
       
       /**
        * Returns the index within this string of the last occurrence of the specified character.
        *
        * @param ch a character
        * @return the index of the last occurrence of the character in the string,
        *         or Size::npos if the character does not occur
        */
       Size lastIndexOf(Char ch) const;
       
       /**
        * Returns the index within this string of the last occurrence of the specified character,
        * searching backward starting at the specified index.
        *
        * @param ch a character
        * @param fromIndex the index to start the search from
        * @return the index of the last occurrence of the character in the string,
        *         or Size::npos if the character does not occur
        */
       Size lastIndexOf(Char ch, Size fromIndex) const;
       
       /**
        * Returns the index within this string of the last occurrence of the specified substring.
        *
        * @param str the substring to search for
        * @return the index of the last occurrence of the specified substring,
        *         or Size::npos if there is no such occurrence
        */
       Size lastIndexOf(const String& str) const;
       
       /**
        * Returns the index within this string of the last occurrence of the specified substring,
        * searching backward starting at the specified index.
        *
        * @param str the substring to search for
        * @param fromIndex the index to start the search from
        * @return the index of the last occurrence of the specified substring,
        *         starting at the specified index, or Size::npos if there is no such occurrence
        */
       Size lastIndexOf(const String& str, Size fromIndex) const;
   };
   ```

## Testing Strategy

1. Basic Tests
   - Empty string handling
   - Single character search
   - Substring search
   - Multiple occurrences
   - No occurrences

2. Unicode Tests
   - Surrogate pair handling
   - Mixed ASCII and Unicode content
   - Emoji and other non-BMP characters

3. Edge Cases
   - Search at beginning of string
   - Search at end of string
   - Search with fromIndex at various positions
   - Empty search string (should return the fromIndex)
   - Overlapping occurrences

4. Comparison Tests
   - Verify behavior matches Java's implementation for various scenarios

## Implementation Details

1. Character Search Implementation
   - For `indexOf(Char ch)`, search through the UTF-16 representation
   - For surrogate pairs, ensure proper handling when searching for a character
   - Implement `lastIndexOf` by searching backward from the end or specified position

2. String Search Implementation
   - For substring search, consider using the Boyer-Moore or Knuth-Morris-Pratt algorithm for efficiency
   - Ensure proper handling of UTF-16 encoded strings
   - Reuse the character search implementation where appropriate

3. Optimization Considerations
   - Cache the UTF-16 representation to avoid repeated conversions
   - Consider implementing specialized search for short substrings
   - Ensure efficient handling of large strings and repeated searches

## Implementation Steps

1. Implement the character search methods first (indexOf and lastIndexOf for characters)
2. Implement the string search methods building on the character search functionality
3. Add comprehensive tests for all edge cases and Unicode handling
4. Optimize the implementation for performance
5. Ensure all Java behavior is correctly matched

## References

- [Java String indexOf documentation](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/String.html#indexOf(int))
- [Java String lastIndexOf documentation](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/String.html#lastIndexOf(int))

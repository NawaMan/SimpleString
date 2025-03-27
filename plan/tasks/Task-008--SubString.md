# Task-009: Implement Java String Substring Methods

## Objective
Implement Java's String substring operations for the SString class, matching Java's behavior exactly.

## Requirements

1. Core Methods (Java API)
   - [ ] `substring(int beginIndex)`: Returns a string that is a substring of this string, starting with the character at the specified index and extending to the end
   - [ ] `substring(int beginIndex, int endIndex)`: Returns a string that contains a subsequence of characters currently contained in this string, starting at beginIndex and extending to endIndex - 1
   - [ ] `subSequence(int beginIndex, int endIndex)`: Returns a new character sequence that is a subsequence of this sequence (part of CharSequence interface)

2. Behavior Requirements
   - [ ] Match Java's UTF-16 code unit indexing
   - [ ] Throw StringIndexOutOfBoundsException for invalid indices
   - [ ] Handle surrogate pairs correctly at boundaries
   - [ ] Maintain string immutability

## Implementation Plan

1. Public Interface
   ```cpp
   class String {
   public:
       /**
        * Returns a string that is a substring of this string.
        * The substring begins with the character at the specified index and extends to the end.
        *
        * @param beginIndex the beginning index, inclusive
        * @return the substring
        * @throws StringIndexOutOfBoundsException if beginIndex is negative or larger than length()
        */
        String substring(std::size_t beginIndex) const;
       
       /**
        * Returns a string that is a substring of this string.
        * The substring begins at beginIndex and extends to endIndex - 1.
        *
        * @param beginIndex the beginning index, inclusive
        * @param endIndex the ending index, exclusive
        * @return the substring
        * @throws StringIndexOutOfBoundsException if:
        *         beginIndex is negative
        *         endIndex is larger than length()
        *         beginIndex is larger than endIndex
        */
       String substring(std::size_t beginIndex, std::size_t endIndex) const;
       
   };
   ```

2. Make sure all existing methods are adjusted to account for offset and length

## Testing Strategy

1. Basic Tests
   - Empty string handling
   - Full string substring
   - Single character extraction
   - Middle section extraction
   - End section extraction

2. Unicode Tests
   - Surrogate pair handling at boundaries
   - Mixed ASCII and Unicode content
   - Emoji and other non-BMP characters

3. Edge Cases
   - Zero-length substrings
   - Adjacent indices
   - Maximum length substrings
   - Invalid index combinations

4. Error Cases
   - Negative indices
   - Out of bounds indices
   - beginIndex > endIndex
   - endIndex > length

## Implementation Details

1. Memory Management
   ```cpp
   class String {
   private:
       std::shared_ptr<const std::string> data_;    // Shared string data
       std::size_t offset_{0};                      // Start offset in data_
       std::size_t length_{0};                      // Length of this substring
       mutable std::shared_ptr<const std::u16string> utf16_cache_;
   };
   ```

   Key Points:
   - Add offset_ and length_ fields to enable zero-copy substrings
   - All existing methods must account for offset when accessing data_
   - length() returns length_ instead of data_->length()
   - Constructors need to initialize offset_ and length_ appropriately
   - substring() operations just create new SString with adjusted offset/length

2. Optimization Benefits
   - Zero-copy substring operations
   - Memory efficient - multiple substrings share same data_
   - O(1) substring creation time
   - Automatic memory cleanup through shared_ptr

3. UTF Handling
   - Validate UTF-16 boundaries considering offset_
   - Preserve surrogate pairs at substring boundaries
   - Handle invalid UTF-8 sequences
   - UTF-16 cache needs to be segment-aware

## Acceptance Criteria
1. All methods match Java String behavior exactly
2. Unicode characters are handled properly
3. Performance is comparable to Java String
4. Memory usage is optimized
5. All test cases pass
6. Edge cases are handled gracefully
7. Documentation matches Java's API docs

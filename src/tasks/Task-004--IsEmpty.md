# Task-004: Add isEmpty() Method

## Motivation

Add a simple but useful method `isEmpty()` to check if a string is empty, similar to Java's String.isEmpty().
This makes the code more readable than checking `length() == 0`.

## Description

Add an `isEmpty()` method to SString that:
- Returns true if the string is empty
- Returns false otherwise
- Matches Java's String.isEmpty() behavior

## Acceptance Criteria

- [x] Add isEmpty() method to SString class
- [x] Add tests for:
    - [x] Empty string returns true
    - [x] Non-empty string returns false
    - [x] String with only whitespace returns false
    - [x] String with only null character returns false

## Implementation Details

1. Method Signature:
   ```cpp
   bool isEmpty() const;
   ```

2. Implementation:
   - Uses std::string::empty() internally
   - Constant time complexity O(1)
   - Thread-safe (const method on immutable data)

3. Behavior:
   - Returns true only for strings of length 0
   - Matches Java's String.isEmpty() exactly
   - Properly handles edge cases:
     * Whitespace strings return false
     * Null character strings return false
     * UTF-8 strings behave correctly

4. Documentation:
   - Clear method documentation in header
   - Explicit note about whitespace/null handling
   - Follows Java-style documentation format

## Expected Benefits

1. Improved readability:
   - More expressive than length() == 0
   - Matches Java's String API
   - Self-documenting code

2. Consistency:
   - Aligns with Java String behavior
   - Works with existing string comparison methods

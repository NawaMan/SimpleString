## Task-007--CodePoinAt.md

### Motivation
I want to add code point at functionality.

- [ ] int code_point_at(int index)
- [ ] int code_point_before(int index)
- [ ] int code_point_count(int begin_index, int end_index)

### Acceptance Criteria

- [ ] Add code point at functionality
- [ ] Add tests
- [ ] Add documentation

### Implementation Details

1. Method Signature:
   ```cpp
   char32_t code_point_at(std::size_t pos) const;
   ```

2. Implementation:
   - Uses std::string::code_point_at() internally
   - Constant time complexity O(1)
   - Thread-safe (const method on immutable data)

3. Behavior:
   - Returns the code point at the specified position
   - Throws std::out_of_range if pos is out of bounds
   - Properly handles edge cases:
     * Returns the correct code point for valid positions
     * Throws exception for out-of-bounds positions
     * Handles UTF-8 encoding correctly

4. Documentation:
   - Clear method documentation in header
   - Explicit note about exception handling
   - Follows Java-style documentation format

5. Expected Benefits:
   - Improved functionality:
     * More expressive than charAt()
     * Matches Java's String.codePointAt() behavior
     * Self-documenting code

### Notes

- Use `std::string::code_point_at()`
- Use `std::out_of_range` for out-of-bounds positions
- Handle UTF-8 encoding correctly

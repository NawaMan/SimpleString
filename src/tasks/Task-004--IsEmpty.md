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

- [ ] Add isEmpty() method to SString class
- [ ] Add tests for:
    - [ ] Empty string returns true
    - [ ] Non-empty string returns false
    - [ ] String with only whitespace returns false
    - [ ] String with only null character returns false

## Expected Benefits

1. Improved readability:
   - More expressive than length() == 0
   - Matches Java's String API
   - Self-documenting code

2. Consistency:
   - Aligns with Java String behavior
   - Works with existing string comparison methods

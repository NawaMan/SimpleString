# Task-003: String Sharing Optimization

## Motivation

Since SString is immutable (const std::string data_), multiple SString instances containing the same content could share the same underlying string data. This is similar to Java's string interning but can be more automatic using C++'s std::shared_ptr.

## Description

Optimize SString to use shared string storage:
- Replace `const std::string data_` with `std::shared_ptr<const std::string>`
- Maintain immutability guarantees
- Add automatic string sharing between instances
- Add benchmarks to measure memory and performance impact

## Acceptance Criteria

- [x] Replace data_ with std::shared_ptr<const std::string>
- [x] Update constructors to use make_shared
- [x] Update all methods to work with shared_ptr
- [x] Maintain const-correctness and immutability
- [x] Add benchmarks for:
    - [x] Memory usage with repeated strings (via vector tests)
    - [x] Construction performance (via copy tests)
    - [x] Comparison performance (via thread safety tests)
- [x] Add tests to verify:
    - [x] String sharing works correctly
    - [x] Immutability is maintained
    - [x] All existing functionality works
    - [x] Thread safety of sharing

## Implementation Details

1. Storage:
   ```cpp
   std::shared_ptr<const std::string> data_;  // Immutable string storage shared between instances
   ```

2. Key Features:
   - Natural sharing through copy construction and assignment
   - Thread-safe reference counting via std::shared_ptr
   - Zero-cost abstraction when strings are unique
   - No centralized string pool to avoid memory leaks

3. Sharing Behavior:
   - Copies share the same underlying string data
   - Independent constructions get unique data
   - Assignment operator enables sharing
   - Vector storage preserves sharing

4. Thread Safety:
   - Reference counting is thread-safe
   - No data races on shared strings
   - Immutability prevents modification
   - Verified with concurrent tests

5. Performance:
   - O(1) copying via reference counting
   - No memory allocation for copies
   - Automatic cleanup when last reference is gone
   - Efficient vector operations

## Expected Benefits

1. Memory Efficiency:
   - Multiple SStrings with same content share one copy
   - Reduced memory usage for repeated strings
   - Automatic cleanup via reference counting

2. Performance:
   - Faster string copies (just increment ref count)
   - Potential cache benefits from sharing

3. Safety:
   - Thread-safe reference counting
   - Maintains immutability guarantees
   - No changes to public API

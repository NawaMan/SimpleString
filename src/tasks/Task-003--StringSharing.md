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

- [ ] Replace data_ with std::shared_ptr<const std::string>
- [ ] Update constructors to use make_shared
- [ ] Update all methods to work with shared_ptr
- [ ] Maintain const-correctness and immutability
- [ ] Add benchmarks for:
    - [ ] Memory usage with repeated strings
    - [ ] Construction performance
    - [ ] Comparison performance
- [ ] Add tests to verify:
    - [ ] String sharing works correctly
    - [ ] Immutability is maintained
    - [ ] All existing functionality works
    - [ ] Thread safety of sharing

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

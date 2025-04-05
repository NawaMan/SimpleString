# Task 017: Remove Boost dependency from regex.hpp

## Objective
Remove direct Boost header dependencies from the `regex.hpp` file while maintaining all functionality. This will allow users of the SimpleString library to compile their applications without needing Boost headers installed.

## Background
Currently, the `regex.hpp` header file directly includes Boost headers, which forces users of the SimpleString library to have Boost development headers installed on their system, even though the library itself is built as a static library with Boost statically linked.

## Requirements
1. Remove all direct Boost header includes from `regex.hpp`
2. Maintain all existing functionality and API compatibility
3. Use proper abstraction techniques to hide the Boost implementation details
4. Ensure all unit tests continue to pass

## Implementation Approach
1. Create appropriate abstraction layers or wrapper classes to hide Boost implementation details
2. Move Boost-specific code to implementation files (`.cpp`) where possible
3. Use the PIMPL (Pointer to Implementation) idiom or similar techniques to hide implementation details
4. Update the implementation files to maintain the same functionality

## Acceptance Criteria
1. No Boost headers are directly included in `regex.hpp`
2. All existing functionality continues to work as before
3. All unit tests pass
4. Applications using the SimpleString library can be compiled without Boost headers installed

## Dependencies
- None

## Estimated Effort
Medium - This task requires careful refactoring to maintain API compatibility while removing direct Boost dependencies.

## Notes
- This is part of an effort to make the SimpleString library more self-contained and reduce external dependencies for users.
- The implementation files can still use Boost internally, but the headers exposed to users should not require Boost.

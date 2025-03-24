# TODO List for SString Implementation

## Testing Improvements
1. Add move semantics tests:
   - Test move constructor
   - Test move assignment operator
   - Verify no unnecessary copies
   - Test with large strings

2. Add more UTF-8 tests:
   - Test with more complex Unicode sequences
   - Test with invalid UTF-8 sequences at different positions
   - Test with mixed valid/invalid sequences
   - Test with different Unicode normalization forms

## Future Enhancements
1. Consider implementing:
   - Substring extraction
   - String concatenation (+ operator)
   - Character access by index
   - Index functions (indexOf, lastIndexOf)
   - String manipulation methods (replace, split, join)
   - Regular expressions support

2. Performance Optimizations:
   - Consider small string optimization
   - Evaluate UTF-8/UTF-16 conversion strategies
   - Profile and optimize hot paths

3. Unicode Normalization:
   - Implement Unicode normalization (NFC, NFD, etc.) vis Normalizer class (like in Java)
   - Add methods for Unicode normalization
   - Add tests for Unicode normalization
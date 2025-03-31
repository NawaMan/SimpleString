#ifndef SIMPLE_REGEX_HPP
#define SIMPLE_REGEX_HPP

#include <boost/regex.hpp>
#include <stdexcept>
#include <vector>
#include "string.hpp"

namespace simple {

/**
 * A class for regular expression pattern matching and manipulation.
 * This class wraps around Boost.Regex and provides integration with the String class.
 */
class RegEx {
public:
    /**
     * Creates a new RegEx object with the given pattern.
     * @param pattern the regular expression pattern
     * @throws RegExSyntaxException if the pattern is invalid
     */
    explicit RegEx(const String& pattern);

    /**
     * Creates a new RegEx object with the given pattern and flags.
     * @param pattern the regular expression pattern
     * @param flags the regex flags (combination of RegEx::Flag values)
     * @throws RegExSyntaxException if the pattern is invalid
     */
    RegEx(const String& pattern, int flags);
    
    /**
     * Returns the flags used by this regex pattern.
     * @return the regex flags
     */
    int flags() const;

    /**
     * Returns a RegEx that will exactly match the given string.
     * @param str the string to match exactly
     * @return a RegEx that matches the string exactly
     */
    static RegEx forExactString(const String& str);

    /**
     * Returns a RegEx that will exactly match the given character.
     * @param ch the character to match exactly
     * @return a RegEx that matches the character exactly
     */
    static RegEx forExactChar(const Char& ch);

    /**
     * Tests if this regex matches the entire input string.
     * @param input the string to match against this pattern
     * @return true if the input string matches this pattern
     */
    bool matches(const String& input) const;

    /**
     * Tests if this regex matches a region within the input string.
     * @param input the string to match against this pattern
     * @return true if this pattern matches a region within the input string
     */
    bool find(const String& input) const;

    /**
     * Replaces all matches of this pattern in the input string with the replacement string.
     * @param input the string to process
     * @param replacement the replacement string
     * @return the resulting string after replacement
     */
    String replaceAll(const String& input, const String& replacement) const;

    /**
     * Replaces the first match of this pattern in the input string with the replacement string.
     * @param input the string to process
     * @param replacement the replacement string
     * @return the resulting string after replacement
     */
    String replaceFirst(const String& input, const String& replacement) const;

    /**
     * Splits the input string around matches of this pattern.
     * @param input the string to split
     * @return an array of strings computed by splitting the input around matches of this pattern
     */
    std::vector<String> split(const String& input) const;

    /**
     * Splits the input string around matches of this pattern with a limit on the number of splits.
     * @param input the string to split
     * @param limit the result threshold (if positive, limits the number of splits; if zero, no limit; if negative, trailing empty strings are not included)
     * @return an array of strings computed by splitting the input around matches of this pattern
     */
    std::vector<String> split(const String& input, int limit) const;

    // Regex flags - these map to Boost.Regex flags
    enum Flag {
        CASE_INSENSITIVE = boost::regex_constants::icase,
        MULTILINE = boost::regex_constants::normal,  // Boost doesn't have direct multiline flag
        DOTALL = boost::regex_constants::normal,     // Use normal and set the flag in pattern
        EXTENDED = boost::regex_constants::extended,
        ECMAScript = boost::regex_constants::ECMAScript
    };

private:
    // Implementation details using Boost.Regex
    boost::regex pattern_;
    int originalFlags_; // Store original flags for retrieval

    // Helper methods
    static String escapeRegexMetacharacters(const String& str);
};

/**
 * Exception thrown when a regular expression syntax error occurs.
 */
class RegExSyntaxException : public std::runtime_error {
public:
    explicit RegExSyntaxException(const std::string& message);
};

} // namespace simple

#endif // SIMPLE_REGEX_HPP

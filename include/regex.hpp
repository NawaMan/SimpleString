#ifndef SIMPLE_REGEX_HPP
#define SIMPLE_REGEX_HPP

#include <boost/regex.hpp>
#include <stdexcept>
#include <vector>
#include "string.hpp"

namespace simple {

/**
 * @brief A class for regular expression pattern matching and manipulation.
 *
 * This class provides a wrapper around Boost.Regex to work with the String class,
 * offering pattern matching, replacement, and string splitting functionality.
 * It handles Unicode characters and provides Java-like regex behavior.
 */
class RegEx {
public:
    /**
     * @brief Creates a new RegEx object with the given pattern.
     *
     * Initializes a regular expression with default ECMAScript syntax.
     * 
     * @param pattern The regular expression pattern to compile
     * @throws RegExSyntaxException If the pattern contains invalid regex syntax
     */
    explicit RegEx(const String& pattern);

    /**
     * @brief Creates a new RegEx object with the given pattern and flags.
     *
     * Initializes a regular expression with specific flags to control matching behavior.
     * 
     * @param pattern The regular expression pattern to compile
     * @param flags The regex flags (combination of RegEx::Flag values)
     * @throws RegExSyntaxException If the pattern contains invalid regex syntax
     * @see RegEx::Flag for available flag options
     */
    RegEx(const String& pattern, int flags);
    
    /**
     * @brief Returns the flags used by this regex pattern.
     *
     * Retrieves the original flags that were used to create this RegEx object.
     * 
     * @return The regex flags as an integer (combination of RegEx::Flag values)
     */
    int flags() const;

    /**
     * @brief Returns a RegEx that will exactly match the given string.
     *
     * Creates a pattern that escapes all regex metacharacters in the input string,
     * ensuring the resulting RegEx will only match the exact string.
     * 
     * @param str The string to match exactly
     * @return A RegEx object that matches the string exactly
     */
    static RegEx forExactString(const String& str);

    /**
     * @brief Returns a RegEx that will exactly match the given character.
     *
     * Creates a pattern that matches a single character exactly, handling
     * Unicode characters and escaping any regex metacharacters.
     * 
     * @param ch The character to match exactly
     * @return A RegEx object that matches the character exactly
     */
    static RegEx forExactChar(const Char& ch);

    /**
     * @brief Tests if this regex matches the entire input string.
     *
     * Checks if the entire input string matches this regular expression pattern.
     * This is equivalent to checking if the pattern matches from the beginning
     * to the end of the string.
     * 
     * @param input The string to match against this pattern
     * @return true if the entire input string matches this pattern, false otherwise
     */
    bool matches(const String& input) const;

    /**
     * @brief Tests if this regex matches a region within the input string.
     *
     * Checks if this pattern matches any substring within the input string.
     * Unlike matches(), this method returns true if the pattern is found
     * anywhere in the string, not just when it matches the entire string.
     * 
     * @param input The string to search for matches
     * @return true if this pattern matches any region within the input string, false otherwise
     */
    bool find(const String& input) const;

    /**
     * @brief Replaces all matches of this pattern in the input string with the replacement string.
     *
     * Finds all occurrences of this pattern in the input string and replaces
     * each with the specified replacement string.
     * 
     * @param input The string to process
     * @param replacement The string to substitute for each match
     * @return A new string with all matches replaced
     * @throws RegExSyntaxException If an error occurs during pattern matching
     */
    String replaceAll(const String& input, const String& replacement) const;

    /**
     * @brief Replaces the first match of this pattern in the input string with the replacement string.
     *
     * Finds the first occurrence of this pattern in the input string and replaces
     * it with the specified replacement string. Subsequent matches are left unchanged.
     * 
     * @param input The string to process
     * @param replacement The string to substitute for the first match
     * @return A new string with the first match replaced
     * @throws RegExSyntaxException If an error occurs during pattern matching
     */
    String replaceFirst(const String& input, const String& replacement) const;

    /**
     * @brief Splits the input string around matches of this pattern.
     *
     * Divides the input string into substrings at the positions where this
     * pattern matches, and returns an array of these substrings. This method
     * is equivalent to calling split(input, 0).
     * 
     * @param input The string to split
     * @return A vector of strings computed by splitting the input around matches of this pattern
     * @throws RegExSyntaxException If an error occurs during pattern matching
     * @see split(const String&, int) for more control over the result
     */
    std::vector<String> split(const String& input) const;

    /**
     * @brief Splits the input string around matches of this pattern with a limit on the number of splits.
     *
     * Divides the input string into substrings at the positions where this
     * pattern matches, with control over the maximum number of splits and
     * handling of trailing empty strings.
     * 
     * @param input The string to split
     * @param limit Controls the number of splits:
     *             - If positive, limits the number of splits (resulting array length <= limit)
     *             - If zero, no limit on splits (default behavior)
     *             - If negative, no limit and trailing empty strings are included
     * @return A vector of strings computed by splitting the input around matches of this pattern
     * @throws RegExSyntaxException If an error occurs during pattern matching
     */
    std::vector<String> split(const String& input, int limit) const;

    /**
     * @brief Regex flags to control matching behavior.
     *
     * These flags map to Boost.Regex flags and can be combined using bitwise OR.
     * For example: RegEx::CASE_INSENSITIVE | RegEx::EXTENDED
     */
    enum Flag {
        CASE_INSENSITIVE = boost::regex_constants::icase,    ///< Case-insensitive matching
        MULTILINE = boost::regex_constants::normal,          ///< Multiline mode (^ and $ match at line breaks)
        DOTALL = boost::regex_constants::normal,             ///< Dot matches all characters including newlines
        EXTENDED = boost::regex_constants::extended,         ///< Extended syntax with whitespace ignored
        ECMAScript = boost::regex_constants::ECMAScript      ///< ECMAScript (JavaScript) syntax
    };

private:
    /**
     * @brief The compiled regex pattern using Boost.Regex.
     */
    boost::regex pattern_;
    
    /**
     * @brief Original flags used to create this regex.
     */
    int originalFlags_;

    /**
     * @brief Escapes regex metacharacters in a string.
     *
     * Helper method that adds backslashes before any regex metacharacters
     * to ensure they are interpreted literally rather than as special characters.
     *
     * @param str The string to escape
     * @return A new string with all regex metacharacters escaped
     */
    static String escapeRegexMetacharacters(const String& str);
};

/**
 * @brief Exception thrown when a regular expression syntax error occurs.
 *
 * This exception is thrown when an invalid regex pattern is provided
 * or when an error occurs during regex operations.
 */
class RegExSyntaxException : public std::runtime_error {
public:
    /**
     * @brief Constructs a RegExSyntaxException with the specified error message.
     *
     * @param message The error message describing the syntax error
     */
    explicit RegExSyntaxException(const std::string& message);
};

} // namespace simple

#endif // SIMPLE_REGEX_HPP

// The minimum headers needed for Java-like Unicode support
#include <iostream>
#include <string>
#include <iomanip>     // For formatting output
#include <vector>      // For test_chars vector
#include <boost/locale.hpp>

// Initialize Unicode support (makes C++ strings behave more like Java Strings)
void initUnicode() {
    // Create a UTF-8 aware environment (similar to Java's default)
    boost::locale::generator gen;
    
    // Set up the locale for the entire program (like Java's JVM)
    auto loc = gen("en_US.UTF-8");
    std::locale::global(loc);
    std::cout.imbue(loc);  // For console output (like System.out)
}

int main() {
    // Initialize Unicode support once at the start
    initUnicode();

    // Unicode string example
    // Unlike Java, C++ std::string doesn't automatically handle Unicode
    // We use Boost.Locale to provide similar functionality to Java's String
    std::string text = "Hello, 世界! Привет, мир! 🌍";
    
    // Case conversion (similar to String.toUpperCase() and toLowerCase() in Java)
    // In C++, we use the << operator for stream output (instead of Java's +)
    std::cout << "Original text: " << text << std::endl;  // std::endl is like System.out.println()
    std::cout << "Uppercase: " << boost::locale::to_upper(text) << std::endl;
    std::cout << "Lowercase: " << boost::locale::to_lower(text) << std::endl;
    
    // Unicode normalization
    // Similar to Java's Normalizer.normalize()
    // This ensures that equivalent Unicode sequences are represented consistently
    // For example, é can be represented as a single character or as e + ´
    std::string normalized = boost::locale::normalize(text);
    std::cout << "Normalized: " << normalized << std::endl;
    
    // Character classification
    // In Java, you would use Character.isLowerCase(), isUpperCase(), etc.
    // In C++/Boost, we need to create a locale-specific environment first
    std::cout << "\nCharacter classification demonstration:\n";
    
    // Create a vector (similar to ArrayList<String> in Java)
    // In C++, we use {} for initialization lists
    std::vector<std::string> test_chars = {
        "a",    // Latin lowercase
        "B",    // Latin uppercase
        "1",    // Digit
        "!",    // Punctuation
        "世",   // Chinese character (世)
        "界",   // Chinese character (界)
        "й",    // Cyrillic lowercase (й)
        "Ф"     // Cyrillic uppercase (Ф)
    };

    // Create a character classifier using Boost.Locale
    for (const auto& c : test_chars) {
        // Convert string to UTF-32 for proper Unicode handling
        auto utf32 = boost::locale::conv::utf_to_utf<char32_t>(c);
        char32_t ch = utf32[0];

        // Format and output character information
        std::cout << "Character '" << c << "' (U+" 
                  << std::hex << std::uppercase << std::setw(4)
                  << std::setfill('0') << static_cast<int>(ch) << "):\n"
                  << std::dec << std::nouppercase;

        try {
            // Use wide character classification with proper locale
            // Convert the return values to bool (any non-zero value means true)
            std::cout << "  is letter: " << std::boolalpha 
                      << (std::iswalpha(ch) > 0) << "\n"
                      << "  is lower: " << (std::iswlower(ch) > 0) << "\n"
                      << "  is upper: " << (std::iswupper(ch) > 0) << "\n"
                      << "  is digit: " << (std::iswdigit(ch) > 0) << "\n"
                      << "  is punct: " << (std::iswpunct(ch) > 0) << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  Error classifying character: " << e.what() << std::endl;
        }
    }

    // Word boundary analysis
    // Similar to Java's BreakIterator.getWordInstance()
    // This helps identify word boundaries in text, which is locale-dependent
    boost::locale::boundary::ssegment_index words(
        boost::locale::boundary::word,     // We want to break into words
        text.begin(),                      // Start of the text
        text.end(),                        // End of the text
        std::locale()                      // Use current locale (already UTF-8 aware)
    );

    std::cout << "\nWord boundaries:\n";
    // Traditional iterator-based for loop (similar to Java Iterator)
    // C++ iterators are similar to Java Iterators but more powerful
    for (boost::locale::boundary::ssegment_index::iterator it = words.begin(); it != words.end(); ++it) {
        // Check if this segment is actually a word (not whitespace or punctuation)
        if (it->rule() & boost::locale::boundary::word_any)
            std::cout << "Word: " << *it << std::endl;  // '*it' dereferences the iterator
    }
    
    // Note: In modern C++, we could use auto for the iterator type:
    // for (auto it = words.begin(); it != words.end(); ++it)
    // This is similar to Java's var keyword

    // In C++, main() must return an integer
    // 0 indicates successful execution (similar to Java's System.exit(0))
    return 0;
}  // End of main()

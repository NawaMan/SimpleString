#include <iostream>
#include <string>
#include <boost/locale.hpp>
#include <boost/locale/boundary.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/locale/info.hpp>

int main() {
    // Initialize boost locale generator
    boost::locale::generator gen;
    std::locale::global(gen(""));
    std::cout.imbue(std::locale());

    // Unicode string examples
    std::string text = "Hello, 世界! Привет, мир! 🌍";
    
    // Convert case
    std::cout << "Original text: " << text << std::endl;
    std::cout << "Uppercase: " << boost::locale::to_upper(text) << std::endl;
    std::cout << "Lowercase: " << boost::locale::to_lower(text) << std::endl;
    
    // Normalize Unicode text (compose/decompose)
    std::string normalized = boost::locale::normalize(text);
    std::cout << "Normalized: " << normalized << std::endl;
    
    // Unicode character classification
    std::cout << "\nCharacter classification demonstration:\n";
    boost::locale::generator gen_loc;
    std::locale loc = gen_loc("en_US.UTF-8");
    
    // Test characters in UTF-8
    std::vector<std::string> test_chars = {
        "a",    // Latin lowercase
        "B",    // Latin uppercase
        "1",    // Digit
        "!",    // Punctuation
        "世",   // Chinese character
        "界",   // Chinese character
        "й",    // Cyrillic lowercase
        "Ф"     // Cyrillic uppercase
    };

    for (const auto& c : test_chars) {
        // Convert to UTF-32 for proper character classification
        auto utf32 = boost::locale::conv::utf_to_utf<char32_t>(c);
        char32_t ch = utf32[0];

        std::cout << "Character '" << c << "' (U+" 
                  << std::hex << std::uppercase << std::setw(4) 
                  << std::setfill('0') << static_cast<int>(ch) << "):\n"
                  << std::dec << std::nouppercase
                  << "  is letter: " << std::boolalpha 
                  << (std::isalpha(ch, loc) || std::iswidechar(ch, loc)) << "\n"
                  << "  is lower: " << std::islower(ch, loc) << "\n"
                  << "  is upper: " << std::isupper(ch, loc) << "\n"
                  << "  is digit: " << std::isdigit(ch, loc) << "\n"
                  << "  is punct: " << std::ispunct(ch, loc) << std::endl;
    }

    // Unicode text boundaries
    boost::locale::boundary::ssegment_index words(
        boost::locale::boundary::word, 
        text.begin(), 
        text.end(),
        gen("en_US.UTF-8")
    );

    std::cout << "\nWord boundaries:\n";
    for (boost::locale::boundary::ssegment_index::iterator it = words.begin(); it != words.end(); ++it) {
        if (it->rule() & boost::locale::boundary::word_any)
            std::cout << "Word: " << *it << std::endl;
    }

    return 0;
}

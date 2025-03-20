#include <iostream>
#include <string>
#include <boost/locale.hpp>

// One-time setup (like Java's JVM initialization)
void initUnicode() {
    auto loc = boost::locale::generator()("en_US.UTF-8");
    std::locale::global(loc);
    std::cout.imbue(loc);
}

int main() {
    initUnicode();  // Do this once at program start

    // After this, you can use strings almost like in Java:
    std::string text = "Hello, 世界!";  // UTF-8 just works
    
    // String operations work like Java's String methods
    std::cout << "Original: " << text << std::endl;
    std::cout << "Uppercase: " << boost::locale::to_upper(text) << std::endl;  // like toUpperCase()
    std::cout << "Lowercase: " << boost::locale::to_lower(text) << std::endl;  // like toLowerCase()

    // Word iteration (like Java's BreakIterator)
    auto words = boost::locale::boundary::ssegment_index(
        boost::locale::boundary::word,
        text.begin(), text.end()
    );

    std::cout << "\nWords:" << std::endl;
    for (const auto& word : words) {  // Range-based for loop (like Java's enhanced for)
        if (word.rule() & boost::locale::boundary::word_any) {
            std::cout << word << std::endl;
        }
    }

    return 0;
}

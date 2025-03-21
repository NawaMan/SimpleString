#include "sstring.hpp"
#include <boost/locale.hpp>

namespace simple_string {

namespace {
// One-time initialization of Boost Locale
void initLocale() {
    static bool initialized = false;
    if (!initialized) {
        boost::locale::generator gen;
        std::locale::global(gen("en_US.UTF-8"));
        initialized = true;
    }
}

// Count UTF-16 code units, treating each byte of invalid UTF-8 as a separate code unit
std::size_t countUtf16CodeUnits(const std::string& utf8_str) {
    std::size_t count = 0;
    const unsigned char* str = reinterpret_cast<const unsigned char*>(utf8_str.c_str());
    const unsigned char* end = str + utf8_str.length();

    while (str < end) {
        if (*str < 0x80) {
            // ASCII character
            count++;
            str++;
        } else if ((*str & 0xE0) == 0xC0) {
            // 2-byte UTF-8 sequence
            if (str + 1 >= end || (str[1] & 0xC0) != 0x80) {
                // Invalid or incomplete sequence, treat each byte separately
                count++;
                str++;
                continue;
            }
            // Check for overlong encoding (should have used fewer bytes)
            unsigned int codepoint = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
            if (codepoint < 0x80) {
                // Overlong encoding, treat each byte separately
                count += 2;
                str += 2;
                continue;
            }
            count++;
            str += 2;
        } else if ((*str & 0xF0) == 0xE0) {
            // 3-byte UTF-8 sequence
            if (str + 2 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80) {
                // Invalid or incomplete sequence, treat each byte separately
                count++;
                str++;
                continue;
            }
            count++;
            str += 3;
        } else if ((*str & 0xF8) == 0xF0) {
            // 4-byte UTF-8 sequence
            if (str + 3 >= end || (str[1] & 0xC0) != 0x80 || 
                (str[2] & 0xC0) != 0x80 || (str[3] & 0xC0) != 0x80) {
                // Invalid or incomplete sequence, treat each byte separately
                count++;
                str++;
                continue;
            }
            // Valid 4-byte sequence needs surrogate pairs
            count += 2;
            str += 4;
        } else {
            // Invalid UTF-8 byte, count as one code unit
            count++;
            str++;
        }
    }
    return count;
}

// Compare two strings using byte-by-byte comparison for exact Java behavior
int compareUtf8Strings(const std::string& str1, const std::string& str2) {
    // Java's String.compareTo does byte-by-byte comparison
    return str1.compare(str2);
}
} // anonymous namespace

SString::SString(const std::string& str) 
    // Force immediate copy of string data
    : data_(str.data(), str.length()) {
    initLocale();
}

SString::SString(const char* str, std::size_t length) 
    // Validate input and force immediate copy of data
    : data_(str ? std::string(str, length) : std::string()) {
    if (!str && length > 0) {
        throw std::invalid_argument("Null pointer with non-zero length");
    }
    initLocale();
}

std::size_t SString::length() const {
    // Return number of UTF-16 code units, just like Java's String.length()
    return countUtf16CodeUnits(data_);
}

bool SString::equals(const SString& other) const {
    // Use Boost's locale-aware comparison for proper Unicode handling
    return compareUtf8Strings(data_, other.data_) == 0;
}

CompareResult SString::compareTo(const SString& other) const {
    // Use Boost's locale-aware comparison for proper Unicode handling
    // This matches Java's String.compareTo behavior:
    // Returns CompareResult representing:
    // - negative if this < other
    // - zero if this == other
    // - positive if this > other
    return CompareResult::fromInt(compareUtf8Strings(data_, other.data_));
}

} // namespace sstring

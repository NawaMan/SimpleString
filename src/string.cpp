#include "../include/string.hpp"
#include <boost/locale/encoding.hpp>
#include <cmath>

namespace simple {

auto String::char_at(std::size_t index) const -> Char {
    const auto& utf16 = get_utf16();
    if (index >= utf16.length()) {
        throw StringIndexOutOfBoundsException("Index out of bounds");
    }
    return Char(utf16[index]);
}

auto String::char_value(std::size_t index) const -> char16_t {
    return char_at(index).value();
}

// Implementation of valueOf methods for primitive types
String String::valueOf(bool b) {
    return b ? String("true") : String("false");
}

String String::valueOf(char c) {
    return String(std::string(1, c));
}

String String::valueOf(int i) {
    return String(std::to_string(i));
}

String String::valueOf(long l) {
    return String(std::to_string(l));
}

String String::valueOf(float f) {
    if (std::isnan(f)) {
        return String("NaN");
    } else if (std::isinf(f)) {
        return f > 0 ? String("Infinity") : String("-Infinity");
    }
    return String(std::to_string(f));
}

String String::valueOf(double d) {
    if (std::isnan(d)) {
        return String("NaN");
    } else if (std::isinf(d)) {
        return d > 0 ? String("Infinity") : String("-Infinity");
    }
    return String(std::to_string(d));
}

// Implementation of methods moved from header
std::size_t String::length() const {
    // Initialize locale if needed
    detail::init_locale();
    
    // Use cached UTF-16 representation if available
    if (utf16_cache_) {
        return utf16_cache_->length();
    }
    
    // Otherwise count UTF-16 code units from UTF-8, considering offset and length
    if (length_ == 0) {
        return 0;
    }
    
    // Create a substring view for counting
    std::string_view view(data_->c_str() + offset_, length_);
    return detail::count_utf16_code_units(std::string(view));
}

bool String::is_empty() const {
    return length_ == 0;
}

bool String::equals(const String& other) const {
    // Fast path: check if strings share data and have same offset/length
    if (shares_data_with(other) && offset_ == other.offset_ && length_ == other.length_) {
        return true;
    }
    
    // Otherwise do a byte-by-byte comparison of the substrings
    // This is more efficient than creating full string copies with to_string()
    const char* this_data = data_->c_str() + offset_;
    const char* other_data = other.data_->c_str() + other.offset_;
    std::size_t min_length = std::min(length_, other.length_);
    
    // First check if lengths are different
    if (length_ != other.length_) {
        return false;
    }
    
    // Compare bytes until we find a difference or reach the end
    for (std::size_t i = 0; i < min_length; ++i) {
        if (this_data[i] != other_data[i]) {
            return false;
        }
    }
    
    return true;
}

simple::CompareResult String::compare_to(const String& other) const {
    // Fast path: check if strings share data and have same offset/length
    if (shares_data_with(other) && offset_ == other.offset_ && length_ == other.length_) {
        return simple::CompareResult::EQUAL;
    }
    
    // Otherwise compare the actual substrings
    std::string this_str = to_string();
    std::string other_str = other.to_string();
    int result = detail::compare_utf8_strings(this_str, other_str);
    if (result < 0) return simple::CompareResult::LESS;
    if (result > 0) return simple::CompareResult::GREATER;
    return simple::CompareResult::EQUAL;
}

Char String::operator[](std::size_t index) const { 
    return char_at(index); 
}

simple::CodePoint String::code_point_at(std::size_t index) const {
    const auto& utf16 = get_utf16();
    if (index >= utf16.length()) {
        throw StringIndexOutOfBoundsException("Index out of bounds");
    }
    char16_t first = utf16[index];
    if (first >= 0xD800 && first <= 0xDBFF && index + 1 < utf16.length()) {
        char16_t second = utf16[index + 1];
        if (second >= 0xDC00 && second <= 0xDFFF) {
            return CodePoint(0x10000 + ((first - 0xD800) << 10) + (second - 0xDC00));
        }
    }
    return CodePoint(first);
}

simple::CodePoint String::code_point_before(std::size_t index) const {
    const auto& utf16 = get_utf16();
    if (index == 0 || index > utf16.length()) {
        throw StringIndexOutOfBoundsException("Index out of bounds");
    }
    char16_t second = utf16[index - 1];
    if (second >= 0xDC00 && second <= 0xDFFF && index >= 2) {
        char16_t first = utf16[index - 2];
        if (first >= 0xD800 && first <= 0xDBFF) {
            return CodePoint(0x10000 + ((first - 0xD800) << 10) + (second - 0xDC00));
        }
    }
    return CodePoint(second);
}

std::size_t String::code_point_count(std::size_t begin_index, std::size_t end_index) const {
    const auto& utf16 = get_utf16();
    if (begin_index > end_index || end_index > utf16.length()) {
        throw StringIndexOutOfBoundsException("Invalid range");
    }
    std::size_t count = 0;
    for (std::size_t i = begin_index; i < end_index; ++i) {
        char16_t ch = utf16[i];
        if (ch >= 0xD800 && ch <= 0xDBFF && i + 1 < end_index) {
            char16_t next = utf16[i + 1];
            if (next >= 0xDC00 && next <= 0xDFFF) {
                ++i;  // Skip low surrogate
            }
        }
        ++count;
    }
    return count;
}

const std::string& String::to_string() const { 
    if (offset_ == 0 && length_ == data_->length()) {
        return *data_;  // Return reference to original for full strings
    }
    // For substrings, create a new string with just the substring portion
    static thread_local std::string result;
    result = data_->substr(offset_, length_);
    return result;
}

String String::substring(std::size_t beginIndex) const {
    const auto len = length();
    // Check if beginIndex is out of bounds
    if (beginIndex > len) {
        throw StringIndexOutOfBoundsException("beginIndex is out of bounds");
    }
    return substring(beginIndex, len);
}

String String::substring(std::size_t beginIndex, std::size_t endIndex) const {
    // Validate indices
    const auto& utf16 = get_utf16();
    const auto len = utf16.length();
    
    // Check for out of bounds conditions with more specific error messages
    if (beginIndex > len) {
        throw StringIndexOutOfBoundsException("beginIndex is out of bounds");
    }
    if (endIndex > len) {
        throw StringIndexOutOfBoundsException("endIndex is out of bounds");
    }
    if (beginIndex > endIndex) {
        throw StringIndexOutOfBoundsException("beginIndex cannot be larger than endIndex");
    }
    
    // If we're requesting the entire string, return this
    if (beginIndex == 0 && endIndex == utf16.length()) {
        return *this;
    }
    
    // If empty substring, return empty string
    if (beginIndex == endIndex) {
        return String("");
    }
    
    // Convert UTF-16 indices to UTF-8 byte offsets
    // We need to find the UTF-8 byte offsets that correspond to the UTF-16 indices
    std::size_t utf8_begin = 0;
    std::size_t utf8_end = 0;
    
    // Find the UTF-8 byte offset for beginIndex
    if (beginIndex > 0) {
        const unsigned char* str = reinterpret_cast<const unsigned char*>(data_->c_str() + offset_);
        const unsigned char* end = str + length_;
        std::size_t utf16_index = 0;
        
        while (str < end && utf16_index < beginIndex) {
            if (*str < 0x80) {
                // ASCII character (1 byte in UTF-8, 1 unit in UTF-16)
                str++;
                utf16_index++;
            } else if ((*str & 0xE0) == 0xC0) {
                // 2-byte UTF-8 sequence
                if (str + 1 >= end || (str[1] & 0xC0) != 0x80) {
                    // Invalid sequence, treat as 1 byte
                    str++;
                    utf16_index++;
                    continue;
                }
                str += 2;
                utf16_index++;
            } else if ((*str & 0xF0) == 0xE0) {
                // 3-byte UTF-8 sequence
                if (str + 2 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80) {
                    // Invalid sequence, treat as 1 byte
                    str++;
                    utf16_index++;
                    continue;
                }
                str += 3;
                utf16_index++;
            } else if ((*str & 0xF8) == 0xF0) {
                // 4-byte UTF-8 sequence (surrogate pair in UTF-16)
                if (str + 3 >= end || (str[1] & 0xC0) != 0x80 || 
                    (str[2] & 0xC0) != 0x80 || (str[3] & 0xC0) != 0x80) {
                    // Invalid sequence, treat as 1 byte
                    str++;
                    utf16_index++;
                    continue;
                }
                str += 4;
                utf16_index += 2;  // Surrogate pair counts as 2 UTF-16 code units
            } else {
                // Invalid UTF-8 byte
                str++;
                utf16_index++;
            }
        }
        
        utf8_begin = str - reinterpret_cast<const unsigned char*>(data_->c_str() + offset_);
    }
    
    // Find the UTF-8 byte offset for endIndex
    if (endIndex > 0) {
        const unsigned char* str = reinterpret_cast<const unsigned char*>(data_->c_str() + offset_);
        const unsigned char* end = str + length_;
        std::size_t utf16_index = 0;
        
        while (str < end && utf16_index < endIndex) {
            if (*str < 0x80) {
                // ASCII character (1 byte in UTF-8, 1 unit in UTF-16)
                str++;
                utf16_index++;
            } else if ((*str & 0xE0) == 0xC0) {
                // 2-byte UTF-8 sequence
                if (str + 1 >= end || (str[1] & 0xC0) != 0x80) {
                    // Invalid sequence, treat as 1 byte
                    str++;
                    utf16_index++;
                    continue;
                }
                str += 2;
                utf16_index++;
            } else if ((*str & 0xF0) == 0xE0) {
                // 3-byte UTF-8 sequence
                if (str + 2 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80) {
                    // Invalid sequence, treat as 1 byte
                    str++;
                    utf16_index++;
                    continue;
                }
                str += 3;
                utf16_index++;
            } else if ((*str & 0xF8) == 0xF0) {
                // 4-byte UTF-8 sequence (surrogate pair in UTF-16)
                if (str + 3 >= end || (str[1] & 0xC0) != 0x80 || 
                    (str[2] & 0xC0) != 0x80 || (str[3] & 0xC0) != 0x80) {
                    // Invalid sequence, treat as 1 byte
                    str++;
                    utf16_index++;
                    continue;
                }
                str += 4;
                utf16_index += 2;  // Surrogate pair counts as 2 UTF-16 code units
            } else {
                // Invalid UTF-8 byte
                str++;
                utf16_index++;
            }
        }
        
        utf8_end = str - reinterpret_cast<const unsigned char*>(data_->c_str() + offset_);
    }
    
    // Create a new String with the same data but adjusted offset and length
    return String(data_, offset_ + utf8_begin, utf8_end - utf8_begin);
}

// Operator overloads
bool String::operator==(const String& other) const { 
    return equals(other); 
}

bool String::operator!=(const String& other) const { 
    return !equals(other); 
}

bool String::operator<(const String& other) const { 
    return compare_to(other).is_less(); 
}

bool String::operator<=(const String& other) const { 
    return compare_to(other).is_less_or_equal(); 
}

bool String::operator>(const String& other) const { 
    return compare_to(other).is_greater(); 
}

bool String::operator>=(const String& other) const { 
    return compare_to(other).is_greater_or_equal(); 
}

// Private methods
const std::u16string& String::get_utf16() const {
    // Double-checked locking pattern with atomic operations
    auto cache = utf16_cache_;
    if (!cache) {
        // First check failed, acquire the data and create cache
        std::u16string result;
        const unsigned char* str = reinterpret_cast<const unsigned char*>(data_->c_str() + offset_);
        const unsigned char* end = str + length_;
        
        while (str < end) {
            if (*str < 0x80) {
                // ASCII character
                result.push_back(*str);
                str++;
            } else if ((*str & 0xE0) == 0xC0) {
                // 2-byte UTF-8 sequence
                if (str + 1 >= end || (str[1] & 0xC0) != 0x80) {
                    // Invalid or incomplete sequence
                    result.push_back(0xFFFD);
                    str++;
                    continue;
                }
                // Check for overlong encoding
                unsigned int codepoint = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
                if (codepoint < 0x80) {
                    // Overlong encoding
                    result.push_back(0xFFFD);
                    result.push_back(0xFFFD);
                    str += 2;
                    continue;
                }
                result.push_back(codepoint);
                str += 2;
            } else if ((*str & 0xF0) == 0xE0) {
                // 3-byte UTF-8 sequence
                if (str + 2 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80) {
                    // Invalid or incomplete sequence
                    result.push_back(0xFFFD);
                    str++;
                    continue;
                }
                // Check for overlong encoding and surrogate range
                unsigned int codepoint = ((str[0] & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F);
                if (codepoint < 0x800 || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
                    // Overlong encoding or surrogate range
                    result.push_back(0xFFFD);
                    result.push_back(0xFFFD);
                    result.push_back(0xFFFD);
                    str += 3;
                    continue;
                }
                result.push_back(codepoint);
                str += 3;
            } else if ((*str & 0xF8) == 0xF0) {
                // 4-byte UTF-8 sequence
                if (str + 3 >= end || (str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80 || (str[3] & 0xC0) != 0x80) {
                    // Invalid or incomplete sequence
                    result.push_back(0xFFFD);
                    str++;
                    continue;
                }
                // Check for overlong encoding and valid Unicode range
                unsigned int codepoint = ((str[0] & 0x07) << 18) | ((str[1] & 0x3F) << 12) |
                                        ((str[2] & 0x3F) << 6)  | (str[3] & 0x3F);
                if (codepoint < 0x10000 || codepoint > 0x10FFFF) {
                    // Overlong encoding or outside Unicode range
                    result.push_back(0xFFFD);
                    result.push_back(0xFFFD);
                    result.push_back(0xFFFD);
                    result.push_back(0xFFFD);
                    str += 4;
                    continue;
                }
                // Convert to surrogate pair
                codepoint -= 0x10000;
                result.push_back(0xD800 + (codepoint >> 10));
                result.push_back(0xDC00 + (codepoint & 0x3FF));
                str += 4;
            } else {
                // Invalid UTF-8 byte
                result.push_back(0xFFFD);
                str++;
            }
        }
        
        cache = std::make_shared<const std::u16string>(std::move(result));
        utf16_cache_ = cache;
    }
    return *cache;
}

bool String::shares_data_with(const String& other) const { 
    return data_ == other.data_; 
}

} // namespace simple

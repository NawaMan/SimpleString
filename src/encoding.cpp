#include "../include/encoding.hpp"

namespace simple {

EncodingException::EncodingException(const std::string& message)
    : std::runtime_error(message),
      encoding_(Encoding::UTF_8),
      byteOffset_(0),
      errorHandling_(EncodingErrorHandling::THROW),
      hasContext_(false),
      fullMessage_(message) {
}

EncodingException::EncodingException(const std::string& message, Encoding encoding,
                                   size_t byteOffset, EncodingErrorHandling errorHandling)
    : std::runtime_error(message),
      encoding_(encoding),
      byteOffset_(byteOffset),
      errorHandling_(errorHandling),
      hasContext_(true) {
    // Construct a detailed message
    fullMessage_ = message;
    fullMessage_ += " (Encoding: " + to_string(encoding) + ", Byte offset: " + std::to_string(byteOffset) + ")";
}

Encoding EncodingException::getEncoding() const {
    return encoding_;
}

size_t EncodingException::getByteOffset() const {
    return byteOffset_;
}

EncodingErrorHandling EncodingException::getErrorHandling() const {
    return errorHandling_;
}

const char* EncodingException::what() const noexcept {
    return hasContext_ ? fullMessage_.c_str() : std::runtime_error::what();
}

std::string to_string(Encoding encoding) {
    switch (encoding) {
        case Encoding::UTF_8: return "UTF-8";
        case Encoding::UTF_16BE: return "UTF-16BE";
        case Encoding::UTF_16LE: return "UTF-16LE";
        case Encoding::UTF_32BE: return "UTF-32BE";
        case Encoding::UTF_32LE: return "UTF-32LE";
        case Encoding::ISO_8859_1: return "ISO-8859-1";
        case Encoding::ASCII: return "ASCII";
        default: return "Unknown Encoding";
    }
}

std::string to_string(EncodingErrorHandling errorHandling) {
    switch (errorHandling) {
        case EncodingErrorHandling::THROW: return "THROW";
        case EncodingErrorHandling::REPLACE: return "REPLACE";
        case EncodingErrorHandling::IGNORE: return "IGNORE";
        default: return "Unknown Error Handling";
    }
}

std::string to_string(BOMPolicy bomPolicy) {
    switch (bomPolicy) {
        case BOMPolicy::AUTO: return "AUTO";
        case BOMPolicy::INCLUDE: return "INCLUDE";
        case BOMPolicy::EXCLUDE: return "EXCLUDE";
        default: return "Unknown BOM Policy";
    }
}

} // namespace simple

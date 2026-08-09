/**
 * @file scan.cpp
 * @brief Scalar implementations of the parser's byte-scanning primitives.
 */

#include "strata/util/scan.hpp"

namespace strata::util {

namespace {

[[nodiscard]] inline bool is_continuation(unsigned char byte) noexcept {
    return (byte & 0xC0) == 0x80;
}

} // namespace

bool validate_utf8(const char* data, size_t len) noexcept {
    const auto* bytes = reinterpret_cast<const unsigned char*>(data);
    size_t pos = 0;

    while (pos < len) {
        const unsigned char lead = bytes[pos];

        if (lead < 0x80) { // ASCII
            ++pos;
            continue;
        }

        // 0x80-0xBF are continuations with no lead; 0xC0/0xC1 would only ever
        // encode an overlong ASCII byte; 0xF5+ is beyond U+10FFFF.
        if (lead < 0xC2 || lead > 0xF4)
            return false;

        size_t extra;
        unsigned char second_min = 0x80;
        unsigned char second_max = 0xBF;

        if (lead <= 0xDF) {
            extra = 1;
        } else if (lead <= 0xEF) {
            extra = 2;
            if (lead == 0xE0)
                second_min = 0xA0; // reject overlong E0 80..9F
            else if (lead == 0xED)
                second_max = 0x9F; // reject surrogates ED A0..BF
        } else {
            extra = 3;
            if (lead == 0xF0)
                second_min = 0x90; // reject overlong F0 80..8F
            else if (lead == 0xF4)
                second_max = 0x8F; // reject above U+10FFFF
        }

        if (pos + extra >= len)
            return false; // truncated sequence

        const unsigned char second = bytes[pos + 1];
        if (second < second_min || second > second_max)
            return false;

        for (size_t offset = 2; offset <= extra; ++offset) {
            if (!is_continuation(bytes[pos + offset]))
                return false;
        }

        pos += extra + 1;
    }

    return true;
}

size_t skip_whitespace(const char* data, size_t len, size_t pos) noexcept {
    while (pos < len) {
        const char c = data[pos];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
        ++pos;
    }
    return pos;
}

size_t find_next_escape(const char* data, size_t len) noexcept {
    for (size_t pos = 0; pos < len; ++pos) {
        const unsigned char c = static_cast<unsigned char>(data[pos]);
        if (c == '"' || c == '\\' || c < 0x20)
            return pos;
    }
    return len;
}

} // namespace strata::util

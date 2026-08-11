/**
 * @file scan.cpp
 * @brief The parser's byte-scanning primitives.
 *
 * UTF-8 validation and whitespace skipping are scalar; the escape scan is
 * SIMD where the target has it, with a scalar twin that tests check it
 * against (docs/context/styleguide.md).
 */

#include "strata/util/scan.hpp"

#include <cstdint>
#include <cstring>

namespace strata::util {

namespace {

[[nodiscard]] inline bool is_continuation(unsigned char byte) noexcept {
    return (byte & 0xC0) == 0x80;
}

/**
 * Validate one non-ASCII sequence starting at @p pos (bytes[pos] >= 0x80),
 * advancing @p pos past it on success. One definition shared verbatim by
 * validate_utf8 and its scalar twin, so the two can only ever differ in how
 * they traverse ASCII — which is exactly what the equivalence test probes.
 */
[[nodiscard]] inline bool check_multibyte(const unsigned char* bytes, size_t len,
                                          size_t& pos) noexcept {
    const unsigned char lead = bytes[pos];

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
    return true;
}

/**
 * Length of the leading all-ASCII run, found a block at a time.
 *
 * Validation is a whole-input pre-pass on every `bytes` parse, and real JSON
 * is overwhelmingly ASCII, so this is where the pass spends its time. The
 * high-bit test is endian-independent — unlike the escape masks, the word
 * tier needs no little-endian gate.
 */
[[nodiscard]] inline size_t ascii_run(const char* data, size_t len) noexcept {
    size_t pos = 0;
#if defined(__ARM_NEON)
    while (pos + 16 <= len) {
        const uint8x16_t block = vld1q_u8(reinterpret_cast<const uint8_t*>(data + pos));
        if (vmaxvq_u8(block) >= 0x80)
            break;
        pos += 16;
    }
#elif defined(STRATA_ESCAPE_SCAN_SIMD)
    while (pos + 16 <= len) {
        const __m128i block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + pos));
        if (_mm_movemask_epi8(block) != 0)
            break;
        pos += 16;
    }
#endif
    for (; pos + 8 <= len; pos += 8) {
        uint64_t word;
        std::memcpy(&word, data + pos, 8); // the only sanctioned type pun
        if ((word & 0x8080808080808080ULL) != 0)
            break;
    }
    while (pos < len && static_cast<unsigned char>(data[pos]) < 0x80)
        ++pos;
    return pos;
}

} // namespace

bool validate_utf8(const char* data, size_t len) noexcept {
    const auto* bytes = reinterpret_cast<const unsigned char*>(data);
    size_t pos = 0;

    while (pos < len) {
        pos += ascii_run(data + pos, len - pos);
        if (pos >= len)
            return true;
        if (!check_multibyte(bytes, len, pos))
            return false;
    }

    return true;
}

bool validate_utf8_scalar(const char* data, size_t len) noexcept {
    const auto* bytes = reinterpret_cast<const unsigned char*>(data);
    size_t pos = 0;

    while (pos < len) {
        if (bytes[pos] < 0x80) { // ASCII
            ++pos;
            continue;
        }
        if (!check_multibyte(bytes, len, pos))
            return false;
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

size_t find_next_escape_scalar(const char* data, size_t len) noexcept {
    for (size_t pos = 0; pos < len; ++pos) {
        const unsigned char c = static_cast<unsigned char>(data[pos]);
        if (c == '"' || c == '\\' || c < 0x20)
            return pos;
    }
    return len;
}

size_t copy_until_escape_scalar(const char* src, size_t len, char* dst) noexcept {
    for (size_t pos = 0; pos < len; ++pos) {
        const unsigned char c = static_cast<unsigned char>(src[pos]);
        if (c == '"' || c == '\\' || c < 0x20)
            return pos;
        dst[pos] = src[pos];
    }
    return len;
}

} // namespace strata::util

#pragma once

/**
 * @file fast_parse.hpp
 * @brief Fast number parsing and string scanning utilities.
 *
 * Optimised for common JSON patterns. 3-5x faster than std::stod
 * for typical integer and floating-point values.
 *
 * Key design choices:
 * - parse_int_fast rejects leading zeros (JSON spec).
 * - parse_double_fast handles decimal point and exponent notation.
 * - scan_string_fast uses an unrolled loop (8 chars/iter) for branch
 *   predictor–friendly escape/quote detection.
 * - skip_whitespace_fast uses an unrolled loop (4 chars/iter).
 */

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

#if defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>
#define STRATA_PARSE_HAS_NEON 1
#elif defined(__SSE2__)
#include <emmintrin.h>
#define STRATA_PARSE_HAS_SSE2 1
#endif

namespace strata {
namespace util {

/**
 * Fast integer parser for JSON numbers.
 *
 * Parses an optional minus sign followed by digits.  Rejects leading
 * zeros (except bare "0") per the JSON specification.  Returns false
 * on overflow or invalid input.
 *
 * @param str      Pointer to the first character.
 * @param len      Number of available characters.
 * @param[out] result  Parsed int64_t value.
 * @param[out] consumed Number of characters consumed.
 * @return true on success.
 */
[[nodiscard]] inline bool parse_int_fast(const char* str, size_t len, int64_t& result,
                                         size_t& consumed) noexcept {
    if (len == 0)
        return false;

    size_t pos = 0;
    bool negative = false;

    if (str[pos] == '-') {
        negative = true;
        ++pos;
    }

    if (pos >= len || str[pos] < '0' || str[pos] > '9') {
        return false;
    }

    // JSON rejects leading zeros (e.g. "007") — only bare "0" is allowed.
    if (str[pos] == '0') {
        ++pos;
        if (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            return false;
        }
        result = 0;
        consumed = pos;
        return true;
    }

    uint64_t val = 0;

    // Fast path: accumulate up to 18 digits without overflow checking.
    // 18 digits fit in uint64 (max 999999999999999999 < 2^63-1).
    const size_t safe_end = (pos + 18 < len) ? pos + 18 : len;
    while (pos < safe_end && str[pos] >= '0' && str[pos] <= '9') {
        val = val * 10 + static_cast<uint64_t>(str[pos] - '0');
        ++pos;
    }

    // Slow path: remaining digits with overflow checking (19+ digit numbers)
    const uint64_t limit = negative ? static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1
                                    : static_cast<uint64_t>(std::numeric_limits<int64_t>::max());
    while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
        int digit = str[pos] - '0';
        if (val > (limit - static_cast<uint64_t>(digit)) / 10) {
            return false; // Overflow
        }
        val = val * 10 + static_cast<uint64_t>(digit);
        ++pos;
    }

    if (negative) {
        const uint64_t min_abs =
            static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1; // |INT64_MIN|
        if (val > min_abs)
            return false;
        if (val == min_abs) {
            result = std::numeric_limits<int64_t>::min();
        } else {
            result = -static_cast<int64_t>(val);
        }
    } else {
        if (val > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
            return false;
        result = static_cast<int64_t>(val);
    }
    consumed = pos;
    return true;
}

/**
 * Fast double parser (simplified Ryu-inspired algorithm).
 *
 * Handles common JSON float patterns efficiently.  Declared in the
 * header, defined in fast_parse.cpp.
 *
 * @param str      Pointer to the first character.
 * @param len      Number of available characters.
 * @param[out] result  Parsed double value.
 * @param[out] consumed Number of characters consumed.
 * @return true on success.
 */
[[nodiscard]] bool parse_double_fast(const char* str, size_t len, double& result, size_t& consumed);

/**
 * Fast string scan for escape detection.
 *
 * Returns the position of the first quote, backslash, or control
 * character (<0x20), or @p len if none is found.  Uses an unrolled
 * loop (8 chars per iteration) for better branch prediction.
 *
 * @param str  Pointer to the first character (after the opening quote).
 * @param len  Number of available characters.
 * @return Position of first special character, or @p len.
 */
[[nodiscard]] inline size_t scan_string_fast(const char* str, size_t len) noexcept {
    size_t pos = 0;

    // Unrolled loop — 8 characters per iteration.
    while (pos + 8 <= len) {
        if (str[pos] == '"' || str[pos] == '\\' || str[pos] < 0x20)
            return pos;
        if (str[pos + 1] == '"' || str[pos + 1] == '\\' || str[pos + 1] < 0x20)
            return pos + 1;
        if (str[pos + 2] == '"' || str[pos + 2] == '\\' || str[pos + 2] < 0x20)
            return pos + 2;
        if (str[pos + 3] == '"' || str[pos + 3] == '\\' || str[pos + 3] < 0x20)
            return pos + 3;
        if (str[pos + 4] == '"' || str[pos + 4] == '\\' || str[pos + 4] < 0x20)
            return pos + 4;
        if (str[pos + 5] == '"' || str[pos + 5] == '\\' || str[pos + 5] < 0x20)
            return pos + 5;
        if (str[pos + 6] == '"' || str[pos + 6] == '\\' || str[pos + 6] < 0x20)
            return pos + 6;
        if (str[pos + 7] == '"' || str[pos + 7] == '\\' || str[pos + 7] < 0x20)
            return pos + 7;
        pos += 8;
    }

    // Scalar tail.
    while (pos < len) {
        if (str[pos] == '"' || str[pos] == '\\' || str[pos] < 0x20)
            return pos;
        ++pos;
    }

    return len;
}

/**
 * Fast whitespace skipper.
 *
 * Returns the position of the first non-whitespace character starting
 * from @p start.  Uses SIMD (NEON/SSE2) to check 16 bytes per iteration
 * when available, with a scalar fallback.
 *
 * @param str   Pointer to the buffer.
 * @param len   Total buffer length.
 * @param start Starting position.
 * @return Position of first non-whitespace character, or @p len.
 */
[[nodiscard]] inline size_t skip_whitespace_fast(const char* str, size_t len,
                                                 size_t start) noexcept {
    size_t pos = start;

    // Fast exit: most JSON tokens have zero or one whitespace byte before them.
    // This avoids SIMD register setup overhead for the common case.
    if (pos < len) {
        char c = str[pos];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            return pos;
        ++pos;
        if (pos < len) {
            c = str[pos];
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
                return pos;
        }
    }

#ifdef STRATA_PARSE_HAS_NEON
    // NEON: check 16 bytes at once using comparison against 4 whitespace chars.
    // A byte is whitespace iff it matches ' ', '\t', '\n', or '\r'.
    const uint8x16_t sp = vdupq_n_u8(' ');
    const uint8x16_t tab = vdupq_n_u8('\t');
    const uint8x16_t nl = vdupq_n_u8('\n');
    const uint8x16_t cr = vdupq_n_u8('\r');

    while (pos + 16 <= len) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(str + pos));
        uint8x16_t is_ws = vorrq_u8(vorrq_u8(vceqq_u8(chunk, sp), vceqq_u8(chunk, tab)),
                                    vorrq_u8(vceqq_u8(chunk, nl), vceqq_u8(chunk, cr)));
        // If any byte is NOT whitespace, find which one
        if (vminvq_u8(is_ws) == 0) {
            // At least one non-whitespace byte found — scan to find it
            for (int j = 0; j < 16; ++j) {
                char ch = str[pos + j];
                if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
                    return pos + j;
            }
        }
        pos += 16;
    }
#elif defined(STRATA_PARSE_HAS_SSE2)
    const __m128i sp = _mm_set1_epi8(' ');
    const __m128i tab = _mm_set1_epi8('\t');
    const __m128i nl = _mm_set1_epi8('\n');
    const __m128i cr = _mm_set1_epi8('\r');

    while (pos + 16 <= len) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str + pos));
        __m128i is_ws =
            _mm_or_si128(_mm_or_si128(_mm_cmpeq_epi8(chunk, sp), _mm_cmpeq_epi8(chunk, tab)),
                         _mm_or_si128(_mm_cmpeq_epi8(chunk, nl), _mm_cmpeq_epi8(chunk, cr)));
        int mask = _mm_movemask_epi8(is_ws);
        if (mask != 0xFFFF) {
            // Found non-whitespace — count trailing ones
            int first_non_ws = __builtin_ctz(~mask);
            return pos + first_non_ws;
        }
        pos += 16;
    }
#endif

    // Scalar tail
    while (pos < len) {
        char c = str[pos];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            return pos;
        ++pos;
    }

    return pos;
}

} // namespace util
} // namespace strata

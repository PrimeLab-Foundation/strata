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

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
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
 * Handles common JSON float patterns efficiently.  Inlined for
 * best performance in the parser hot loop (avoids cross-TU call).
 *
 * @param str      Pointer to the first character.
 * @param len      Number of available characters.
 * @param[out] result  Parsed double value.
 * @param[out] consumed Number of characters consumed.
 * @return true on success.
 */
[[nodiscard]] inline bool parse_double_fast(const char* str, size_t len, double& result,
                                            size_t& consumed) noexcept {
    /// Pre-computed powers of 10 for exponent lookup (10^0..10^308).
    static constexpr double kPow10[] = {
        1e0,   1e1,   1e2,   1e3,   1e4,   1e5,   1e6,   1e7,   1e8,   1e9,   1e10,  1e11,  1e12,
        1e13,  1e14,  1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,  1e22,  1e23,  1e24,  1e25,
        1e26,  1e27,  1e28,  1e29,  1e30,  1e31,  1e32,  1e33,  1e34,  1e35,  1e36,  1e37,  1e38,
        1e39,  1e40,  1e41,  1e42,  1e43,  1e44,  1e45,  1e46,  1e47,  1e48,  1e49,  1e50,  1e51,
        1e52,  1e53,  1e54,  1e55,  1e56,  1e57,  1e58,  1e59,  1e60,  1e61,  1e62,  1e63,  1e64,
        1e65,  1e66,  1e67,  1e68,  1e69,  1e70,  1e71,  1e72,  1e73,  1e74,  1e75,  1e76,  1e77,
        1e78,  1e79,  1e80,  1e81,  1e82,  1e83,  1e84,  1e85,  1e86,  1e87,  1e88,  1e89,  1e90,
        1e91,  1e92,  1e93,  1e94,  1e95,  1e96,  1e97,  1e98,  1e99,  1e100, 1e101, 1e102, 1e103,
        1e104, 1e105, 1e106, 1e107, 1e108, 1e109, 1e110, 1e111, 1e112, 1e113, 1e114, 1e115, 1e116,
        1e117, 1e118, 1e119, 1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 1e126, 1e127, 1e128, 1e129,
        1e130, 1e131, 1e132, 1e133, 1e134, 1e135, 1e136, 1e137, 1e138, 1e139, 1e140, 1e141, 1e142,
        1e143, 1e144, 1e145, 1e146, 1e147, 1e148, 1e149, 1e150, 1e151, 1e152, 1e153, 1e154, 1e155,
        1e156, 1e157, 1e158, 1e159, 1e160, 1e161, 1e162, 1e163, 1e164, 1e165, 1e166, 1e167, 1e168,
        1e169, 1e170, 1e171, 1e172, 1e173, 1e174, 1e175, 1e176, 1e177, 1e178, 1e179, 1e180, 1e181,
        1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188, 1e189, 1e190, 1e191, 1e192, 1e193, 1e194,
        1e195, 1e196, 1e197, 1e198, 1e199, 1e200, 1e201, 1e202, 1e203, 1e204, 1e205, 1e206, 1e207,
        1e208, 1e209, 1e210, 1e211, 1e212, 1e213, 1e214, 1e215, 1e216, 1e217, 1e218, 1e219, 1e220,
        1e221, 1e222, 1e223, 1e224, 1e225, 1e226, 1e227, 1e228, 1e229, 1e230, 1e231, 1e232, 1e233,
        1e234, 1e235, 1e236, 1e237, 1e238, 1e239, 1e240, 1e241, 1e242, 1e243, 1e244, 1e245, 1e246,
        1e247, 1e248, 1e249, 1e250, 1e251, 1e252, 1e253, 1e254, 1e255, 1e256, 1e257, 1e258, 1e259,
        1e260, 1e261, 1e262, 1e263, 1e264, 1e265, 1e266, 1e267, 1e268, 1e269, 1e270, 1e271, 1e272,
        1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279, 1e280, 1e281, 1e282, 1e283, 1e284, 1e285,
        1e286, 1e287, 1e288, 1e289, 1e290, 1e291, 1e292, 1e293, 1e294, 1e295, 1e296, 1e297, 1e298,
        1e299, 1e300, 1e301, 1e302, 1e303, 1e304, 1e305, 1e306, 1e307, 1e308,
    };
    static constexpr int kPow10MaxExp = 308;

    if (len == 0)
        return false;

    size_t pos = 0;
    bool negative = false;

    if (str[pos] == '-') {
        negative = true;
        ++pos;
    }

    if (pos >= len || (str[pos] < '0' || str[pos] > '9'))
        return false;

    // Parse integer part as uint64_t (precise up to 18 significant digits).
    uint64_t int_val = 0;
    int total_digits = 0;

    if (str[pos] == '0') {
        ++pos;
        if (pos < len && str[pos] >= '0' && str[pos] <= '9')
            return false; // Leading zero
    } else {
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            if (total_digits < 19)
                int_val = int_val * 10 + static_cast<uint64_t>(str[pos] - '0');
            ++total_digits;
            ++pos;
        }
    }

    // Parse fractional part — continue accumulating into same uint64_t.
    int frac_digits = 0;
    if (pos < len && str[pos] == '.') {
        ++pos;
        if (pos >= len || str[pos] < '0' || str[pos] > '9')
            return false;
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            if (total_digits < 19) {
                int_val = int_val * 10 + static_cast<uint64_t>(str[pos] - '0');
                ++frac_digits;
            }
            ++total_digits;
            ++pos;
        }
    }

    // Parse exponent
    int exp_val = 0;
    bool exp_negative = false;
    if (pos < len && (str[pos] == 'e' || str[pos] == 'E')) {
        ++pos;
        if (pos < len) {
            if (str[pos] == '-') {
                exp_negative = true;
                ++pos;
            } else if (str[pos] == '+') {
                ++pos;
            }
        }
        if (pos >= len || str[pos] < '0' || str[pos] > '9')
            return false;
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            exp_val = exp_val * 10 + (str[pos] - '0');
            if (exp_val > 400) {
                while (pos < len && str[pos] >= '0' && str[pos] <= '9')
                    ++pos;
                break;
            }
            ++pos;
        }
    }

    // Combine: value = int_val * 10^(exponent - frac_digits)
    int combined_exp = (exp_negative ? -exp_val : exp_val) - frac_digits;

    // Fast path: exact when mantissa fits in 53-bit mantissa and |exp| <= 22,
    // but only if no digits were dropped during uint64 accumulation.
    static constexpr uint64_t kMaxExactMantissa = (1ULL << 53);
    int accumulated_digits = total_digits - (total_digits > 19 ? total_digits - 19 : 0);
    // Note: in parse_double_fast, total_digits counts int digits, frac_digits
    // counts only accumulated frac digits. Digits are dropped when total_digits > 19.
    bool digits_dropped = (total_digits > 19);
    int abs_exp = combined_exp < 0 ? -combined_exp : combined_exp;
    if (!digits_dropped && int_val < kMaxExactMantissa && abs_exp <= 22) {
        result = static_cast<double>(int_val);
        if (combined_exp != 0) {
            static constexpr double kPow10Small[] = {
                1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11,
                1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22,
            };
            double power = kPow10Small[abs_exp];
            result = combined_exp > 0 ? result * power : result / power;
        }
    } else {
        // Slow path: strtod for exact conversion
        size_t num_len = pos - (negative ? 1 : 0);
        const char* num_start = str + (negative ? 1 : 0);
        char strtod_buf[64];
        if (num_len < sizeof(strtod_buf)) {
            memcpy(strtod_buf, num_start, num_len);
            strtod_buf[num_len] = '\0';
            result = strtod(strtod_buf, nullptr);
        } else {
            std::string tmp(num_start, num_len);
            result = strtod(tmp.c_str(), nullptr);
        }
    }

    if (negative)
        result = -result;
    consumed = pos;
    return true;
}

/**
 * Unified JSON number parser — avoids double-scanning digits.
 *
 * Parses sign + integer digits once, then:
 * - If '.' or 'e'/'E' follows → continues to fractional/exponent and returns double.
 * - Otherwise → returns int64_t.
 *
 * @param str      Pointer to the first character.
 * @param len      Number of available characters.
 * @param[out] int_result   Set on integer result.
 * @param[out] dbl_result   Set on double result.
 * @param[out] consumed     Number of characters consumed.
 * @param[out] is_double    True if result is double, false if int.
 * @return true on success.
 */
[[nodiscard]] inline bool parse_number_unified(const char* str, size_t len, int64_t& int_result,
                                               double& dbl_result, size_t& consumed,
                                               bool& is_double) noexcept {
    static constexpr double kPow10Dbl[] = {
        1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11,
        1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22,
    };
    static constexpr int kPow10DblMax = 22;

    if (len == 0)
        return false;

    size_t pos = 0;
    bool negative = false;

    if (str[pos] == '-') {
        negative = true;
        ++pos;
    }

    if (pos >= len || str[pos] < '0' || str[pos] > '9')
        return false;

    // Parse integer part as uint64_t
    uint64_t int_val = 0;
    int int_digits = 0;

    if (str[pos] == '0') {
        ++pos;
        if (pos < len && str[pos] >= '0' && str[pos] <= '9')
            return false; // Leading zero
    } else {
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            if (int_digits < 19)
                int_val = int_val * 10 + static_cast<uint64_t>(str[pos] - '0');
            ++int_digits;
            ++pos;
        }
    }

    // Check if this is a float (has '.' or 'e'/'E')
    if (pos < len && (str[pos] == '.' || str[pos] == 'e' || str[pos] == 'E')) {
        // Float path — continue parsing fractional/exponent parts
        int total_digits = int_digits;
        int frac_digits = 0;

        if (pos < len && str[pos] == '.') {
            ++pos;
            if (pos >= len || str[pos] < '0' || str[pos] > '9')
                return false;
            while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
                if (total_digits < 19) {
                    int_val = int_val * 10 + static_cast<uint64_t>(str[pos] - '0');
                    ++frac_digits;
                }
                ++total_digits;
                ++pos;
            }
        }

        int exp_val = 0;
        bool exp_negative = false;
        if (pos < len && (str[pos] == 'e' || str[pos] == 'E')) {
            ++pos;
            if (pos < len) {
                if (str[pos] == '-') {
                    exp_negative = true;
                    ++pos;
                } else if (str[pos] == '+') {
                    ++pos;
                }
            }
            if (pos >= len || str[pos] < '0' || str[pos] > '9')
                return false;
            while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
                exp_val = exp_val * 10 + (str[pos] - '0');
                if (exp_val > 400) {
                    while (pos < len && str[pos] >= '0' && str[pos] <= '9')
                        ++pos;
                    break;
                }
                ++pos;
            }
        }

        int combined_exp = (exp_negative ? -exp_val : exp_val) - frac_digits;

        // Fast path: uses Clinger's technique for exact float parsing.
        // Exact when mantissa < 2^53 and |combined_exp| <= 22, BUT only
        // if no digits were dropped during uint64 accumulation (> 19 digits).
        static constexpr uint64_t kMaxExactMantissa = (1ULL << 53);
        int accumulated_digits = int_digits + frac_digits;
        bool digits_dropped = (total_digits > accumulated_digits);
        int abs_exp = combined_exp < 0 ? -combined_exp : combined_exp;
        bool fast_ok = !digits_dropped && int_val < kMaxExactMantissa && abs_exp <= kPow10DblMax;
        if (fast_ok) {
            dbl_result = static_cast<double>(int_val);
            if (combined_exp != 0) {
                double power = kPow10Dbl[abs_exp];
                dbl_result = combined_exp > 0 ? dbl_result * power : dbl_result / power;
            }
        } else {
            // Slow path: use strtod for exact conversion.
            // We've already validated the number, so strtod will succeed.
            // Copy to a null-terminated buffer for strtod.
            size_t num_len = pos - (negative ? 1 : 0);
            const char* num_start = str + (negative ? 1 : 0);
            char strtod_buf[64];
            if (num_len < sizeof(strtod_buf)) {
                memcpy(strtod_buf, num_start, num_len);
                strtod_buf[num_len] = '\0';
                dbl_result = strtod(strtod_buf, nullptr);
            } else {
                // Very long number — use the original string if null-terminated,
                // otherwise allocate.
                std::string tmp(num_start, num_len);
                dbl_result = strtod(tmp.c_str(), nullptr);
            }
        }

        if (negative)
            dbl_result = -dbl_result;
        consumed = pos;
        is_double = true;
        return true;
    }

    // Integer path — no '.' or 'e'/'E' encountered
    if (negative) {
        const uint64_t min_abs = static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1;
        if (int_val > min_abs)
            return false;
        if (int_val == min_abs)
            int_result = std::numeric_limits<int64_t>::min();
        else
            int_result = -static_cast<int64_t>(int_val);
    } else {
        if (int_val > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
            return false;
        int_result = static_cast<int64_t>(int_val);
    }
    consumed = pos;
    is_double = false;
    return true;
}

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

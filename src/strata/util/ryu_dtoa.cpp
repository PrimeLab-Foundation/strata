#include "strata/util/ryu_dtoa.hpp"

#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace strata {
namespace util {

// Maximum safe integer that can be exactly represented in double (2^53)
static constexpr double kMaxSafeInteger = 9007199254740992.0;
static constexpr double kMinSafeInteger = -9007199254740992.0;

// Fast integer output for values that fit in int64_t
static inline int ryu_format_integer(int64_t value, char* buffer) {
    if (value == 0) {
        buffer[0] = '0';
        return 1;
    }

    char* p = buffer;
    bool negative = value < 0;
    if (negative) {
        *p++ = '-';
        if (value == INT64_MIN) {
            uint64_t abs_val = static_cast<uint64_t>(-(value + 1)) + 1;
            char digits[20];
            int len = 0;
            while (abs_val > 0) {
                digits[len++] = static_cast<char>('0' + abs_val % 10);
                abs_val /= 10;
            }
            for (int i = len - 1; i >= 0; --i) {
                *p++ = digits[i];
            }
            return static_cast<int>(p - buffer);
        }
        value = -value;
    }

    char digits[20];
    int len = 0;
    while (value > 0) {
        digits[len++] = static_cast<char>('0' + value % 10);
        value /= 10;
    }
    for (int i = len - 1; i >= 0; --i) {
        *p++ = digits[i];
    }
    return static_cast<int>(p - buffer);
}

int ryu_d2s_buffered(double value, char* result) {
    // Handle special cases FIRST (before sign extraction)
    if (std::isnan(value) || std::isinf(value)) {
        memcpy(result, "null", 4);
        return 4;
    }

    // Zero is an exact integer
    if (value == 0.0) {
        result[0] = '0';
        return 1;
    }

    // Check if value is an exact integer within safe range
    if (value >= kMinSafeInteger && value <= kMaxSafeInteger) {
        double int_part;
        double frac = std::modf(value, &int_part);
        if (frac == 0.0) {
            return ryu_format_integer(static_cast<int64_t>(int_part), result);
        }
    }

    char* p = result;

    // Handle sign
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
    auto res = std::to_chars(p, result + 31, value, std::chars_format::general);
    if (res.ec == std::errc()) {
        return static_cast<int>(res.ptr - result);
    }
#endif

    // Fallback: snprintf (e.g. when std::to_chars for double is unavailable)
    int len = snprintf(p, 32, "%.17g", value);
    return static_cast<int>(p - result) + len;
}

int ryu_d2s(double value, char* buffer) { return ryu_d2s_buffered(value, buffer); }

} // namespace util
} // namespace strata

#include "strata/util/ryu_dtoa.hpp"

#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace strata {
namespace util {

int ryu_d2s_buffered(double value, char* result) {
    // Handle special cases FIRST (before sign extraction)
    if (std::isnan(value) || std::isinf(value)) {
        memcpy(result, "null", 4);
        return 4;
    }
    if (value == 0.0) {
        result[0] = '0';
        result[1] = '.';
        result[2] = '0';
        return 3;
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
        int len = static_cast<int>(res.ptr - p);
        bool has_dot = false;
        bool has_e = false;
        for (int i = 0; i < len; ++i) {
            if (p[i] == '.')
                has_dot = true;
            if (p[i] == 'e' || p[i] == 'E')
                has_e = true;
        }
        if (!has_dot && !has_e) {
            p[len++] = '.';
            p[len++] = '0';
        }
        return static_cast<int>(p - result) + len;
    }
#endif

    // Fallback: snprintf (e.g. when std::to_chars for double is unavailable)
    int len = snprintf(p, 32, "%.17g", value);
    bool has_dot = false;
    bool has_e = false;
    for (int i = 0; i < len; ++i) {
        if (p[i] == '.')
            has_dot = true;
        if (p[i] == 'e' || p[i] == 'E')
            has_e = true;
    }
    if (!has_dot && !has_e) {
        p[len++] = '.';
        p[len++] = '0';
    }
    return static_cast<int>(p - result) + len;
}

int ryu_d2s(double value, char* buffer) { return ryu_d2s_buffered(value, buffer); }

} // namespace util
} // namespace strata

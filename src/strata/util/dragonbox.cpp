#include "strata/util/dragonbox.hpp"

#include "strata/util/ryu_dtoa.hpp"

#include <charconv>
#include <cmath>
#include <cstring>

namespace strata {
namespace util {

int dragonbox_d2s(double value, char* buffer) {
    // Fast path: zero
    if (value == 0.0) {
        if (std::signbit(value)) {
            buffer[0] = '-';
            buffer[1] = '0';
            buffer[2] = '.';
            buffer[3] = '0';
            return 4;
        }
        buffer[0] = '0';
        buffer[1] = '.';
        buffer[2] = '0';
        return 3;
    }

    char* p = buffer;
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    auto res = std::to_chars(p, buffer + 32, value, std::chars_format::general);
    if (res.ec == std::errc()) {
        int len = static_cast<int>(res.ptr - p);
        // Check if dot/exponent present
        bool needs_dot = true;
        for (int i = 0; i < len; ++i) {
            char c = p[i];
            if (c == '.' || c == 'e' || c == 'E') {
                needs_dot = false;
                break;
            }
        }
        if (needs_dot) {
            p[len] = '.';
            p[len + 1] = '0';
            return static_cast<int>(p - buffer) + len + 2;
        }
        return static_cast<int>(p - buffer) + len;
    }
    return ryu_d2s(value, buffer);
}

} // namespace util
} // namespace strata

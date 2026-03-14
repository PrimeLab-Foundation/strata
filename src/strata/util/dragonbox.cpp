#include "strata/util/dragonbox.hpp"

#include "strata/util/ryu_dtoa.hpp"

#include <charconv>
#include <cmath>
#include <cstring>

namespace strata {
namespace util {

int dragonbox_d2s(double value, char* buffer) {
    char* p = buffer;
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }
    auto res = std::to_chars(p, buffer + 32, value, std::chars_format::general);
    if (res.ec == std::errc()) {
        const size_t len = static_cast<size_t>(res.ptr - p);
        bool has_dot = false;
        bool has_e = false;
        for (size_t i = 0; i < len; ++i) {
            if (p[i] == '.')
                has_dot = true;
            if (p[i] == 'e' || p[i] == 'E')
                has_e = true;
        }
        if (!has_dot && !has_e && len > 0) {
            p[len] = '.';
            p[len + 1] = '0';
            return static_cast<int>(p - buffer) + static_cast<int>(len) + 2;
        }
        return static_cast<int>(res.ptr - buffer);
    }
    return ryu_d2s(value, buffer);
}

} // namespace util
} // namespace strata

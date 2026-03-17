#include "strata/util/dragonbox.hpp"

#include "strata/third_party/dragonbox/dragonbox.h"

#include <cmath>
#include <cstdint>
#include <cstring>

namespace strata {
namespace util {

DecimalResult dragonbox_d2d(double value) {
    // Caller guarantees: value > 0, finite, non-zero.
    // Use the real Dragonbox algorithm for d2d conversion.
    auto result = jkj::dragonbox::to_decimal(value, jkj::dragonbox::policy::sign::ignore,
                                             jkj::dragonbox::policy::trailing_zero::remove);

    return {static_cast<uint64_t>(result.significand), static_cast<int32_t>(result.exponent)};
}

int dragonbox_d2s(double value, char* buffer) {
    // Handle zero
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

    // Handle NaN/Inf (caller should handle, but be safe)
    if (__builtin_expect(std::isnan(value) || std::isinf(value), 0)) {
        memcpy(buffer, "null", 4);
        return 4;
    }

    char* p = buffer;

    // Handle sign
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    // Use Dragonbox d2d + inline formatter
    DecimalResult dec = dragonbox_d2d(value);
    int len = dragonbox_format(dec, p);
    return static_cast<int>(p - buffer) + len;
}

} // namespace util
} // namespace strata

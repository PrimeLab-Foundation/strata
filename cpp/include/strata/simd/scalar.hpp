#pragma once

#include <cstddef>

namespace strata::simd::scalar {
    inline const char *find_string_special(const char *cur, const char *end) {
        while (cur < end) {
            char c = *cur;
            if (c == '"' || c == '\\' || static_cast<unsigned char>(c) < 0x20)
                return cur;
            ++cur;
        }
        return end;
    }

    inline const char *skip_digits(const char *cur, const char *end) {
        while (cur < end && static_cast<unsigned>(*cur - '0') < 10)
            ++cur;
        return cur;
    }

    inline const char *skip_ws(const char *cur, const char *end) {
        while (cur < end) {
            switch (*cur) {
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    ++cur;
                    continue;
                default:
                    return cur;
            }
        }
        return cur;
    }

    inline uint16_t digit_mask_16(const char *p) {
        uint16_t mask = 0;
        for (int i = 0; i < 16; i++)
            if (static_cast<unsigned>(p[i] - '0') < 10)
                mask |= static_cast<uint16_t>(1u << i);
        return mask;
    }
}
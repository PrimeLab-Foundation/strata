#include "strata/util/fast_parse.hpp"

#include <cmath>
#include <cstring>

namespace strata {
namespace util {

bool parse_double_fast(const char* str, size_t len, double& result, size_t& consumed) {
    if (len == 0)
        return false;

    size_t pos = 0;
    bool negative = false;

    // Handle sign
    if (str[pos] == '-') {
        negative = true;
        ++pos;
    }

    if (pos >= len || (str[pos] < '0' || str[pos] > '9')) {
        return false;
    }

    // Parse integer part (check for leading zeros)
    double int_part = 0.0;
    if (str[pos] == '0') {
        ++pos;
        // After '0', must be '.', 'e', 'E', or end
        if (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            return false; // Leading zero not allowed
        }
    } else {
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            int_part = int_part * 10.0 + (str[pos] - '0');
            ++pos;
        }
    }

    // Parse fractional part
    double frac_part = 0.0;
    if (pos < len && str[pos] == '.') {
        ++pos;
        // Must have at least one digit after '.'
        if (pos >= len || str[pos] < '0' || str[pos] > '9') {
            return false;
        }
        double frac_val = 0.0;
        double divisor = 1.0;
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            frac_val = frac_val * 10.0 + (str[pos] - '0');
            divisor *= 10.0;
            ++pos;
        }
        frac_part = frac_val / divisor;
    }

    // Parse exponent
    double exp_part = 1.0;
    if (pos < len && (str[pos] == 'e' || str[pos] == 'E')) {
        ++pos;
        bool exp_negative = false;

        if (pos < len) {
            if (str[pos] == '-') {
                exp_negative = true;
                ++pos;
            } else if (str[pos] == '+') {
                ++pos;
            }
        }

        if (pos >= len || str[pos] < '0' || str[pos] > '9') {
            return false;
        }

        int exp_val = 0;
        while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            exp_val = exp_val * 10 + (str[pos] - '0');
            ++pos;
        }

        exp_part = std::pow(10.0, exp_negative ? -exp_val : exp_val);
    }

    result = (int_part + frac_part) * exp_part;
    if (negative)
        result = -result;
    consumed = pos;
    return true;
}

} // namespace util
} // namespace strata

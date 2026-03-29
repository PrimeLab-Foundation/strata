#include "strata/primitives/parse_number.hpp"

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace strata;

static Result<Number> run(const char* input) {
    return parse<Number>(input, input + std::strlen(input));
}

static bool near(double a, double b) {
    if (a == b) return true;
    double diff = std::abs(a - b);
    double largest = std::max(std::abs(a), std::abs(b));
    return diff <= largest * 1e-15;
}

int main() {
    printf("test_parse_number\n");

    // --- integers: single-pass accumulation ---

    printf("  zero                          ");
    {
        auto r = run("0");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 0);
    }
    printf("ok\n");

    printf("  positive                      ");
    {
        auto r = run("42");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 42);
    }
    printf("ok\n");

    printf("  negative                      ");
    {
        auto r = run("-7");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == -7);
    }
    printf("ok\n");

    printf("  single digit                  ");
    {
        auto r = run("5");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 5);
    }
    printf("ok\n");

    printf("  negative zero                 ");
    {
        auto r = run("-0");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 0);
    }
    printf("ok\n");

    printf("  INT64_MAX                     ");
    {
        auto r = run("9223372036854775807");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == INT64_MAX);
    }
    printf("ok\n");

    printf("  INT64_MIN                     ");
    {
        auto r = run("-9223372036854775808");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == INT64_MIN);
    }
    printf("ok\n");

    printf("  INT64_MAX - 1                 ");
    {
        auto r = run("9223372036854775806");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == INT64_MAX - 1);
    }
    printf("ok\n");

    printf("  19 digit int                  ");
    {
        auto r = run("1000000000000000000");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 1000000000000000000LL);
    }
    printf("ok\n");

    printf("  int + trailing                ");
    {
        auto r = run("123,");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 123);
        assert(*r->rest == ',');
    }
    printf("ok\n");

    printf("  int + trailing brace          ");
    {
        auto r = run("99}");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 99);
        assert(*r->rest == '}');
    }
    printf("ok\n");

    // --- floats: exact pow10 table (exp <= 22) ---

    printf("  simple float                  ");
    {
        auto r = run("3.14");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::abs(std::get<double>(r->value) - 3.14) < 1e-12);
    }
    printf("ok\n");

    printf("  negative float                ");
    {
        auto r = run("-0.5");
        assert(r.has_value());
        assert(std::abs(std::get<double>(r->value) + 0.5) < 1e-12);
    }
    printf("ok\n");

    printf("  0.0 is double                 ");
    {
        auto r = run("0.0");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::get<double>(r->value) == 0.0);
    }
    printf("ok\n");

    printf("  -0.0 is negative zero         ");
    {
        auto r = run("-0.0");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::get<double>(r->value) == 0.0);
        assert(std::signbit(std::get<double>(r->value)));
    }
    printf("ok\n");

    printf("  many fraction digits          ");
    {
        auto r = run("0.123456789012345");
        assert(r.has_value());
        assert(std::abs(std::get<double>(r->value) - 0.123456789012345) < 1e-15);
    }
    printf("ok\n");

    printf("  exponent within table (e10)   ");
    {
        auto r = run("1e10");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::get<double>(r->value) == 1e10);
    }
    printf("ok\n");

    printf("  exponent within table (e22)   ");
    {
        auto r = run("1e22");
        assert(r.has_value());
        assert(std::get<double>(r->value) == 1e22);
    }
    printf("ok\n");

    printf("  negative exponent (e-3)       ");
    {
        auto r = run("1.5E-3");
        assert(r.has_value());
        assert(std::abs(std::get<double>(r->value) - 1.5e-3) < 1e-15);
    }
    printf("ok\n");

    printf("  negative exponent (e-22)      ");
    {
        auto r = run("1e-22");
        assert(r.has_value());
        assert(std::abs(std::get<double>(r->value) - 1e-22) < 1e-34);
    }
    printf("ok\n");

    printf("  positive exponent sign        ");
    {
        auto r = run("2e+5");
        assert(r.has_value());
        assert(std::get<double>(r->value) == 2e5);
    }
    printf("ok\n");

    printf("  zero exponent                 ");
    {
        auto r = run("5e0");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::get<double>(r->value) == 5.0);
    }
    printf("ok\n");

    // --- floats: split multiply path (exp > 22) ---

    printf("  exponent beyond table (e23)   ");
    {
        auto r = run("1e23");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e23));
    }
    printf("ok\n");

    printf("  exponent beyond table (e50)   ");
    {
        auto r = run("1e50");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e50));
    }
    printf("ok\n");

    printf("  exponent beyond table (e100)  ");
    {
        auto r = run("1e100");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e100));
    }
    printf("ok\n");

    printf("  exponent 308 (near DBL_MAX)   ");
    {
        auto r = run("1e308");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e308));
        assert(std::isfinite(std::get<double>(r->value)));
    }
    printf("ok\n");

    printf("  exponent 309 → infinity       ");
    {
        auto r = run("1e309");
        assert(r.has_value());
        assert(std::isinf(std::get<double>(r->value)));
    }
    printf("ok\n");

    printf("  large negative exp → near 0   ");
    {
        auto r = run("1e-300");
        assert(r.has_value());
        assert(std::get<double>(r->value) > 0.0);
        assert(std::get<double>(r->value) < 1e-299);
    }
    printf("ok\n");

    printf("  huge negative exp → zero      ");
    {
        auto r = run("1e-400");
        assert(r.has_value());
        assert(std::get<double>(r->value) == 0.0);
    }
    printf("ok\n");

    // --- floats: significand clamping (>19 digits) ---

    printf("  20+ sig digits (fraction)     ");
    {
        auto r = run("1.23456789012345678901");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::abs(std::get<double>(r->value) - 1.2345678901234568) < 1e-14);
    }
    printf("ok\n");

    printf("  20+ sig digits (integer)      ");
    {
        auto r = run("12345678901234567890.0");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::get<double>(r->value) > 1.2e19);
    }
    printf("ok\n");

    printf("  fraction + large exponent     ");
    {
        auto r = run("1.23e45");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1.23e45));
    }
    printf("ok\n");

    printf("  float + trailing              ");
    {
        auto r = run("1.5]");
        assert(r.has_value());
        assert(std::get<double>(r->value) == 1.5);
        assert(*r->rest == ']');
    }
    printf("ok\n");

    // --- errors ---

    printf("  leading zero                  ");
    {
        auto r = run("007");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::LeadingZero);
    }
    printf("ok\n");

    printf("  leading zero -0x              ");
    {
        auto r = run("00.5");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::LeadingZero);
    }
    printf("ok\n");

    printf("  bare minus                    ");
    {
        auto r = run("-");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  dot without integer           ");
    {
        auto r = run(".5");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    printf("  trailing dot                  ");
    {
        auto r = run("1.");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    printf("  empty exponent                ");
    {
        auto r = run("1e");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    printf("  exponent sign only            ");
    {
        auto r = run("1e+");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    printf("  plus prefix                   ");
    {
        auto r = run("+5");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    printf("  overflow INT64_MAX + 1        ");
    {
        auto r = run("9223372036854775808");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::NumberOverflow);
    }
    printf("ok\n");

    printf("  overflow 20 digits            ");
    {
        auto r = run("99999999999999999999");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::NumberOverflow);
    }
    printf("ok\n");

    printf("  overflow negative             ");
    {
        auto r = run("-9223372036854775809");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::NumberOverflow);
    }
    printf("ok\n");

    printf("  empty input                   ");
    {
        auto r = run("");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  not a number                  ");
    {
        auto r = run("abc");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    printf("  minus then letter             ");
    {
        auto r = run("-abc");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidNumber);
    }
    printf("ok\n");

    // --- to_double split multiply boundaries ---

    printf("  exp exactly 44 (2 loops)      ");
    {
        auto r = run("1e44");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e44));
    }
    printf("ok\n");

    printf("  exp exactly 45                ");
    {
        auto r = run("1e45");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e45));
    }
    printf("ok\n");

    printf("  exp exactly 66 (3 loops)      ");
    {
        auto r = run("1e66");
        assert(r.has_value());
        assert(near(std::get<double>(r->value), 1e66));
    }
    printf("ok\n");

    printf("  negative exp exactly -44      ");
    {
        auto r = run("1e-44");
        assert(r.has_value());
        assert(std::get<double>(r->value) > 0.0);
        assert(std::get<double>(r->value) < 1e-43);
    }
    printf("ok\n");

    printf("  negative exp exactly -66      ");
    {
        auto r = run("1e-66");
        assert(r.has_value());
        assert(std::get<double>(r->value) > 0.0);
    }
    printf("ok\n");

    printf("  exp 0 (no scaling)            ");
    {
        // 7e0 → exp10=0, should return 7.0 exactly
        auto r = run("7e0");
        assert(r.has_value());
        assert(std::get<double>(r->value) == 7.0);
    }
    printf("ok\n");

    // --- SIMD skip_digits: >19 digit float ---

    printf("  30 digit float (SIMD skip)    ");
    {
        auto r = run("1.234567890123456789012345678901");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::abs(std::get<double>(r->value) - 1.2345678901234568) < 1e-14);
    }
    printf("ok\n");

    printf("  20+ digit integer as float    ");
    {
        // 20+ digit integer → overflow → error (not float)
        auto r = run("12345678901234567890");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::NumberOverflow);
    }
    printf("ok\n");

    printf("  0 with many fraction digits   ");
    {
        auto r = run("0.0000000000000001");
        assert(r.has_value());
        assert(std::holds_alternative<double>(r->value));
        assert(std::get<double>(r->value) > 0.0);
        assert(std::get<double>(r->value) < 1e-15);
    }
    printf("ok\n");

    printf("  negative 0 int                ");
    {
        auto r = run("-0");
        assert(r.has_value());
        assert(std::get<int64_t>(r->value) == 0);
    }
    printf("ok\n");

    printf("  all passed\n");
    return 0;
}
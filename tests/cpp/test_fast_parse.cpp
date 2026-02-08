/**
 * C++ tests for SWAR-accelerated number parsing
 *
 * Tests parse_int_fast, parse_uint_fast, parse_double_fast
 * including edge cases, overflow, and JSON compliance.
 */

#include "strata/util/fast_parse.hpp"
#include "strata/util/simd_numbers.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>

using namespace strata::util;

// Test helper macros
#define TEST_PASS(name) \
    do { \
        ++passed; \
        std::cout << "PASSED: " << name << std::endl; \
    } while (0)

#define TEST_FAIL(name, msg) \
    do { \
        std::cerr << "FAILED: " << name << " - " << msg << std::endl; \
    } while (0)

// ============================================================================
// Integer parsing tests
// ============================================================================

bool test_small_integers() {
    std::cout << "\n--- Small Integer Tests (0-9999) ---\n";
    int passed = 0;
    int total = 0;

    // Single digits (0-9)
    for (int i = 0; i <= 9; ++i) {
        ++total;
        std::string s = std::to_string(i);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == i && consumed == s.size()) {
            TEST_PASS(("1-digit: " + s).c_str());
        } else {
            TEST_FAIL(("1-digit: " + s).c_str(), "wrong value or consumed");
        }
    }

    // Two digits (10-99)
    int two_digit_tests[] = {10, 11, 19, 20, 42, 50, 73, 99};
    for (int val : two_digit_tests) {
        ++total;
        std::string s = std::to_string(val);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == val && consumed == s.size()) {
            TEST_PASS(("2-digit: " + s).c_str());
        } else {
            TEST_FAIL(("2-digit: " + s).c_str(), "wrong value or consumed");
        }
    }

    // Three digits (100-999)
    int three_digit_tests[] = {100, 123, 256, 500, 789, 999};
    for (int val : three_digit_tests) {
        ++total;
        std::string s = std::to_string(val);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == val && consumed == s.size()) {
            TEST_PASS(("3-digit: " + s).c_str());
        } else {
            TEST_FAIL(("3-digit: " + s).c_str(), "wrong value or consumed");
        }
    }

    // Four digits (1000-9999) - SWAR path
    int four_digit_tests[] = {1000, 1234, 2048, 4096, 5678, 9999};
    for (int val : four_digit_tests) {
        ++total;
        std::string s = std::to_string(val);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == val && consumed == s.size()) {
            TEST_PASS(("4-digit SWAR: " + s).c_str());
        } else {
            TEST_FAIL(("4-digit SWAR: " + s).c_str(), "wrong value or consumed");
        }
    }

    std::cout << "Small integers: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_medium_integers() {
    std::cout << "\n--- Medium Integer Tests (5-8 digits) ---\n";
    int passed = 0;
    int total = 0;

    int64_t test_values[] = {
        12345,      // 5 digits
        123456,     // 6 digits
        1234567,    // 7 digits
        12345678,   // 8 digits (full SWAR)
        10000000,
        99999999,
        11111111,
        87654321
    };

    for (int64_t val : test_values) {
        ++total;
        std::string s = std::to_string(val);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == val && consumed == s.size()) {
            TEST_PASS((std::to_string(s.size()) + "-digit: " + s).c_str());
        } else {
            TEST_FAIL((std::to_string(s.size()) + "-digit: " + s).c_str(),
                     ("expected " + std::to_string(val) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Medium integers: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_large_integers() {
    std::cout << "\n--- Large Integer Tests (9-19 digits) ---\n";
    int passed = 0;
    int total = 0;

    int64_t test_values[] = {
        123456789LL,           // 9 digits
        1234567890LL,          // 10 digits
        12345678901LL,         // 11 digits
        123456789012LL,        // 12 digits
        1234567890123LL,       // 13 digits
        12345678901234LL,      // 14 digits
        123456789012345LL,     // 15 digits
        1234567890123456LL,    // 16 digits
        12345678901234567LL,   // 17 digits
        123456789012345678LL,  // 18 digits
        1234567890123456789LL, // 19 digits
        std::numeric_limits<int64_t>::max(), // INT64_MAX
    };

    for (int64_t val : test_values) {
        ++total;
        std::string s = std::to_string(val);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == val && consumed == s.size()) {
            TEST_PASS((std::to_string(s.size()) + "-digit: " + s).c_str());
        } else {
            TEST_FAIL((std::to_string(s.size()) + "-digit: " + s).c_str(),
                     ("expected " + std::to_string(val) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Large integers: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_negative_integers() {
    std::cout << "\n--- Negative Integer Tests ---\n";
    int passed = 0;
    int total = 0;

    int64_t test_values[] = {
        -1, -9, -10, -99, -100, -999,
        -1234, -9999, -12345, -99999999,
        -1234567890123456789LL,
        std::numeric_limits<int64_t>::min(), // INT64_MIN
    };

    for (int64_t val : test_values) {
        ++total;
        std::string s = std::to_string(val);
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s.c_str(), s.size(), result, consumed) &&
            result == val && consumed == s.size()) {
            TEST_PASS(("negative: " + s).c_str());
        } else {
            TEST_FAIL(("negative: " + s).c_str(),
                     ("expected " + std::to_string(val) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Negative integers: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_overflow() {
    std::cout << "\n--- Overflow Tests ---\n";
    int passed = 0;
    int total = 0;

    // Values that should overflow int64_t
    const char* overflow_cases[] = {
        "9223372036854775808",   // INT64_MAX + 1
        "9999999999999999999",   // Large overflow
        "99999999999999999999",  // 20 digits
        "999999999999999999999", // 21 digits
        "-9223372036854775809",  // INT64_MIN - 1
    };

    for (const char* s : overflow_cases) {
        ++total;
        int64_t result;
        size_t consumed;
        if (!parse_int_fast(s, std::strlen(s), result, consumed)) {
            TEST_PASS(("overflow rejected: " + std::string(s)).c_str());
        } else {
            TEST_FAIL(("overflow rejected: " + std::string(s)).c_str(), "should have failed");
        }
    }

    // Unsigned overflow tests
    const char* uint_overflow_cases[] = {
        "18446744073709551616",  // UINT64_MAX + 1
        "99999999999999999999",  // 20 digits, definitely overflow
    };

    for (const char* s : uint_overflow_cases) {
        ++total;
        uint64_t result;
        size_t consumed;
        if (!parse_uint_simd(s, std::strlen(s), result, consumed)) {
            TEST_PASS(("uint overflow rejected: " + std::string(s)).c_str());
        } else {
            TEST_FAIL(("uint overflow rejected: " + std::string(s)).c_str(), "should have failed");
        }
    }

    std::cout << "Overflow tests: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_leading_zeros() {
    std::cout << "\n--- Leading Zero Tests (JSON compliance) ---\n";
    int passed = 0;
    int total = 0;

    // Leading zeros should be rejected (except "0" itself)
    const char* invalid_cases[] = {
        "00",
        "01",
        "007",
        "0123",
        "-00",
        "-01",
    };

    for (const char* s : invalid_cases) {
        ++total;
        int64_t result;
        size_t consumed;
        if (!parse_int_fast(s, std::strlen(s), result, consumed)) {
            TEST_PASS(("leading zero rejected: " + std::string(s)).c_str());
        } else {
            TEST_FAIL(("leading zero rejected: " + std::string(s)).c_str(), "should have failed");
        }
    }

    // "0" and "-0" should be valid
    ++total;
    {
        const char* s = "0";
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s, std::strlen(s), result, consumed) && result == 0 && consumed == 1) {
            TEST_PASS("zero accepted: 0");
        } else {
            TEST_FAIL("zero accepted: 0", "should succeed");
        }
    }

    std::cout << "Leading zero tests: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_invalid_input() {
    std::cout << "\n--- Invalid Input Tests ---\n";
    int passed = 0;
    int total = 0;

    const char* invalid_cases[] = {
        "",       // empty
        "-",      // just minus
        "abc",    // non-digits
        "+123",   // plus sign (not valid in JSON)
        " 123",   // leading space
        "123 ",   // trailing space (should stop at space)
    };

    for (const char* s : invalid_cases) {
        ++total;
        int64_t result;
        size_t consumed;
        bool ok = parse_int_fast(s, std::strlen(s), result, consumed);

        // Empty, just minus, non-digits should fail
        // Numbers with trailing chars should succeed but consume only digits
        if (std::strlen(s) == 0 || std::strcmp(s, "-") == 0 ||
            std::strcmp(s, "abc") == 0 || std::strcmp(s, "+123") == 0 ||
            std::strcmp(s, " 123") == 0) {
            if (!ok) {
                TEST_PASS(("invalid rejected: '" + std::string(s) + "'").c_str());
            } else {
                TEST_FAIL(("invalid rejected: '" + std::string(s) + "'").c_str(), "should have failed");
            }
        } else {
            // "123 " should parse 123 and consume 3
            if (ok && consumed == 3 && result == 123) {
                TEST_PASS(("partial parse: '" + std::string(s) + "'").c_str());
            } else {
                TEST_FAIL(("partial parse: '" + std::string(s) + "'").c_str(), "wrong parse");
            }
        }
    }

    std::cout << "Invalid input tests: " << passed << "/" << total << std::endl;
    return passed == total;
}

// ============================================================================
// Double parsing tests
// ============================================================================

bool test_double_integers() {
    std::cout << "\n--- Double Integer Tests ---\n";
    int passed = 0;
    int total = 0;

    double test_values[] = {
        0.0, 1.0, 9.0, 10.0, 99.0, 100.0, 999.0,
        1234.0, 9999.0, 12345.0, 123456.0, 1234567.0, 12345678.0
    };

    for (double val : test_values) {
        ++total;
        std::string s = std::to_string(static_cast<int64_t>(val));
        double result;
        size_t consumed;
        if (parse_double_fast(s.c_str(), s.size(), result, consumed) &&
            std::abs(result - val) < 1e-10 && consumed == s.size()) {
            TEST_PASS(("double int: " + s).c_str());
        } else {
            TEST_FAIL(("double int: " + s).c_str(),
                     ("expected " + std::to_string(val) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Double integers: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_double_fractions() {
    std::cout << "\n--- Double Fraction Tests ---\n";
    int passed = 0;
    int total = 0;

    struct TestCase {
        const char* str;
        double expected;
    };

    TestCase test_cases[] = {
        {"0.0", 0.0},
        {"0.1", 0.1},
        {"0.5", 0.5},
        {"0.9", 0.9},
        {"1.5", 1.5},
        {"3.14", 3.14},
        {"3.14159", 3.14159},
        {"123.456", 123.456},
        {"0.001", 0.001},
        {"0.123456789", 0.123456789},
        {"99.99", 99.99},
        {"1234.5678", 1234.5678},
    };

    for (const auto& tc : test_cases) {
        ++total;
        double result;
        size_t consumed;
        if (parse_double_fast(tc.str, std::strlen(tc.str), result, consumed) &&
            std::abs(result - tc.expected) < 1e-9 && consumed == std::strlen(tc.str)) {
            TEST_PASS(("double frac: " + std::string(tc.str)).c_str());
        } else {
            TEST_FAIL(("double frac: " + std::string(tc.str)).c_str(),
                     ("expected " + std::to_string(tc.expected) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Double fractions: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_double_scientific() {
    std::cout << "\n--- Double Scientific Notation Tests ---\n";
    int passed = 0;
    int total = 0;

    struct TestCase {
        const char* str;
        double expected;
    };

    TestCase test_cases[] = {
        {"1e0", 1.0},
        {"1e1", 10.0},
        {"1e2", 100.0},
        {"1e10", 1e10},
        {"1E10", 1e10},
        {"1.5e2", 150.0},
        {"1.5e+2", 150.0},
        {"1.5e-2", 0.015},
        {"1.23e4", 12300.0},
        {"1.23e-4", 0.000123},
        {"9.99e10", 9.99e10},
        {"1.0e-10", 1.0e-10},
    };

    for (const auto& tc : test_cases) {
        ++total;
        double result;
        size_t consumed;
        double tolerance = std::abs(tc.expected) * 1e-9;
        if (tolerance < 1e-15) tolerance = 1e-15;

        if (parse_double_fast(tc.str, std::strlen(tc.str), result, consumed) &&
            std::abs(result - tc.expected) < tolerance && consumed == std::strlen(tc.str)) {
            TEST_PASS(("double sci: " + std::string(tc.str)).c_str());
        } else {
            TEST_FAIL(("double sci: " + std::string(tc.str)).c_str(),
                     ("expected " + std::to_string(tc.expected) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Double scientific: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_double_negative() {
    std::cout << "\n--- Double Negative Tests ---\n";
    int passed = 0;
    int total = 0;

    struct TestCase {
        const char* str;
        double expected;
    };

    TestCase test_cases[] = {
        {"-0.0", 0.0},  // -0.0 equals 0.0
        {"-1", -1.0},
        {"-3.14", -3.14},
        {"-99.99", -99.99},
        {"-1e5", -1e5},
        {"-1.5e-2", -0.015},
    };

    for (const auto& tc : test_cases) {
        ++total;
        double result;
        size_t consumed;
        double tolerance = std::abs(tc.expected) * 1e-9;
        if (tolerance < 1e-15) tolerance = 1e-15;

        if (parse_double_fast(tc.str, std::strlen(tc.str), result, consumed) &&
            std::abs(result - tc.expected) < tolerance && consumed == std::strlen(tc.str)) {
            TEST_PASS(("double neg: " + std::string(tc.str)).c_str());
        } else {
            TEST_FAIL(("double neg: " + std::string(tc.str)).c_str(),
                     ("expected " + std::to_string(tc.expected) + ", got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Double negative: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_double_invalid() {
    std::cout << "\n--- Double Invalid Tests ---\n";
    int passed = 0;
    int total = 0;

    const char* invalid_cases[] = {
        "",        // empty
        "-",       // just minus
        ".",       // just dot
        "-.1",     // missing integer part before dot
        "1.",      // missing fraction after dot (actually valid in some parsers but not JSON)
        "1.e5",    // missing fraction digits
        "1e",      // missing exponent
        "1e+",     // missing exponent digits
        "1e-",     // missing exponent digits
        "00.5",    // leading zero
        "01.5",    // leading zero
    };

    for (const char* s : invalid_cases) {
        ++total;
        double result;
        size_t consumed;
        if (!parse_double_fast(s, std::strlen(s), result, consumed)) {
            TEST_PASS(("double invalid rejected: '" + std::string(s) + "'").c_str());
        } else {
            TEST_FAIL(("double invalid rejected: '" + std::string(s) + "'").c_str(),
                     ("should have failed, got " + std::to_string(result)).c_str());
        }
    }

    std::cout << "Double invalid: " << passed << "/" << total << std::endl;
    return passed == total;
}

// ============================================================================
// SIMD/SWAR parity tests
// ============================================================================

bool test_simd_scalar_parity() {
    std::cout << "\n--- SIMD/Scalar Parity Tests ---\n";
    int passed = 0;
    int total = 0;

    // Test various digit counts to exercise different code paths
    int64_t test_values[] = {
        0, 1, 5, 9,                           // 1 digit
        10, 42, 99,                           // 2 digits
        100, 500, 999,                        // 3 digits
        1000, 1234, 9999,                     // 4 digits (SWAR)
        10000, 12345, 99999,                  // 5 digits
        100000, 123456, 999999,               // 6 digits
        1000000, 1234567, 9999999,            // 7 digits
        10000000, 12345678, 99999999,         // 8 digits (SWAR)
        100000000, 123456789, 999999999LL,    // 9 digits
        1234567890123456789LL,                // 19 digits
    };

    for (int64_t val : test_values) {
        ++total;
        std::string s = std::to_string(val);

        // Test signed parsing
        int64_t result_signed;
        size_t consumed_signed;
        bool ok_signed = parse_int_simd(s.c_str(), s.size(), result_signed, consumed_signed);

        // Test unsigned parsing
        uint64_t result_unsigned;
        size_t consumed_unsigned;
        bool ok_unsigned = parse_uint_simd(s.c_str(), s.size(), result_unsigned, consumed_unsigned);

        if (ok_signed && ok_unsigned &&
            result_signed == val &&
            result_unsigned == static_cast<uint64_t>(val) &&
            consumed_signed == s.size() &&
            consumed_unsigned == s.size()) {
            TEST_PASS(("parity: " + s).c_str());
        } else {
            TEST_FAIL(("parity: " + s).c_str(), "signed/unsigned mismatch");
        }
    }

    std::cout << "SIMD/Scalar parity: " << passed << "/" << total << std::endl;
    return passed == total;
}

// ============================================================================
// Edge case tests
// ============================================================================

bool test_edge_cases() {
    std::cout << "\n--- Edge Case Tests ---\n";
    int passed = 0;
    int total = 0;

    // Number followed by non-digit
    ++total;
    {
        const char* s = "123abc";
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s, std::strlen(s), result, consumed) &&
            result == 123 && consumed == 3) {
            TEST_PASS("trailing non-digit: 123abc");
        } else {
            TEST_FAIL("trailing non-digit: 123abc", "wrong parse");
        }
    }

    // Number followed by dot (for JSON parser to continue with double)
    ++total;
    {
        const char* s = "123.456";
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s, std::strlen(s), result, consumed) &&
            result == 123 && consumed == 3) {
            TEST_PASS("stop at dot: 123.456");
        } else {
            TEST_FAIL("stop at dot: 123.456", "wrong parse");
        }
    }

    // Zero followed by dot
    ++total;
    {
        const char* s = "0.5";
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s, std::strlen(s), result, consumed) &&
            result == 0 && consumed == 1) {
            TEST_PASS("zero with dot: 0.5");
        } else {
            TEST_FAIL("zero with dot: 0.5", "wrong parse");
        }
    }

    // Maximum safe integer for double
    ++total;
    {
        const char* s = "9007199254740991";  // 2^53 - 1
        int64_t result;
        size_t consumed;
        if (parse_int_fast(s, std::strlen(s), result, consumed) &&
            result == 9007199254740991LL && consumed == 16) {
            TEST_PASS("max safe int for double");
        } else {
            TEST_FAIL("max safe int for double", "wrong value");
        }
    }

    std::cout << "Edge cases: " << passed << "/" << total << std::endl;
    return passed == total;
}

bool test_simd_digit_edge_conditions() {
    std::cout << "\n--- SIMD Digit Edge Conditions ---\n";
    int passed = 0;
    int total = 0;

    ++total;
    if (count_digits_simd("", 0) == 0) {
        TEST_PASS("count_digits_simd empty");
    } else {
        TEST_FAIL("count_digits_simd empty", "expected 0");
    }

    ++total;
    {
        uint64_t result = 0;
        size_t consumed = 0;
        if (!parse_uint_simd("", 0, result, consumed)) {
            TEST_PASS("parse_uint_simd empty");
        } else {
            TEST_FAIL("parse_uint_simd empty", "should fail");
        }
    }

    ++total;
    {
        uint64_t result = 0;
        size_t consumed = 0;
        if (!parse_uint_simd("x", 1, result, consumed)) {
            TEST_PASS("parse_uint_simd invalid first char");
        } else {
            TEST_FAIL("parse_uint_simd invalid first char", "should fail");
        }
    }

    ++total;
    {
        uint64_t result = 0;
        size_t consumed = 0;
        if (parse_uint_simd("0", 1, result, consumed) &&
            result == 0 && consumed == 1) {
            TEST_PASS("parse_uint_simd zero");
        } else {
            TEST_FAIL("parse_uint_simd zero", "wrong parse");
        }
    }

    ++total;
    {
        uint64_t result = 0;
        size_t consumed = 0;
        if (!parse_uint_simd("00", 2, result, consumed)) {
            TEST_PASS("parse_uint_simd leading zero rejected");
        } else {
            TEST_FAIL("parse_uint_simd leading zero rejected", "should fail");
        }
    }

    std::cout << "SIMD digit edge conditions: " << passed << "/" << total << std::endl;
    return passed == total;
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "SWAR Number Parsing Tests (C++)" << std::endl;
    std::cout << "========================================\n" << std::endl;

    int tests_passed = 0;
    int tests_total = 0;

    // Integer tests
    ++tests_total; if (test_small_integers()) ++tests_passed;
    ++tests_total; if (test_medium_integers()) ++tests_passed;
    ++tests_total; if (test_large_integers()) ++tests_passed;
    ++tests_total; if (test_negative_integers()) ++tests_passed;
    ++tests_total; if (test_overflow()) ++tests_passed;
    ++tests_total; if (test_leading_zeros()) ++tests_passed;
    ++tests_total; if (test_invalid_input()) ++tests_passed;

    // Double tests
    ++tests_total; if (test_double_integers()) ++tests_passed;
    ++tests_total; if (test_double_fractions()) ++tests_passed;
    ++tests_total; if (test_double_scientific()) ++tests_passed;
    ++tests_total; if (test_double_negative()) ++tests_passed;
    ++tests_total; if (test_double_invalid()) ++tests_passed;

    // Parity and edge case tests
    ++tests_total; if (test_simd_scalar_parity()) ++tests_passed;
    ++tests_total; if (test_edge_cases()) ++tests_passed;
    ++tests_total; if (test_simd_digit_edge_conditions()) ++tests_passed;

    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Suites: " << tests_passed << "/" << tests_total << " passed" << std::endl;
    std::cout << "========================================" << std::endl;

    if (tests_passed == tests_total) {
        std::cout << "✅ All test suites passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "❌ " << (tests_total - tests_passed) << " test suite(s) failed" << std::endl;
        return 1;
    }
}

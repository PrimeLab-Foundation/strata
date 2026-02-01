/**
 * C++ tests for float precision (mirrors Python TestFloatPrecision)
 *
 * Validates ryu_d2s and dragonbox_d2s produce clean, round-trip correct output.
 */

#include "strata/util/dragonbox.hpp"
#include "strata/util/ryu_dtoa.hpp"

#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

using namespace strata::util;

// Test helper
bool test_float_format(double value, const char* expected_contains, const char* test_name) {
    char buf[64];
    int len = ryu_d2s(value, buf);
    buf[len] = '\0';

    std::string result(buf);
    bool passed = result.find(expected_contains) != std::string::npos;

    if (!passed) {
        std::cerr << "FAILED: " << test_name << std::endl;
        std::cerr << "  Expected to contain: " << expected_contains << std::endl;
        std::cerr << "  Got: " << result << std::endl;
        return false;
    }

    std::cout << "PASSED: " << test_name << " -> " << result << std::endl;
    return true;
}

// Test helper for round-trip accuracy
bool test_roundtrip(double value, const char* test_name) {
    char buf[64];
    int len = ryu_d2s(value, buf);
    buf[len] = '\0';

    // Parse back
    double parsed = std::stod(buf);
    double error = std::abs(parsed - value);
    double tolerance = std::abs(value) * 1e-10;

    bool passed = error <= tolerance;

    if (!passed) {
        std::cerr << "FAILED: " << test_name << " (round-trip)" << std::endl;
        std::cerr << "  Original: " << value << std::endl;
        std::cerr << "  Formatted: " << buf << std::endl;
        std::cerr << "  Parsed: " << parsed << std::endl;
        std::cerr << "  Error: " << error << " (tolerance: " << tolerance << ")" << std::endl;
        return false;
    }

    std::cout << "PASSED: " << test_name << " (round-trip) -> " << buf << std::endl;
    return true;
}

int main() {
    int passed = 0;
    int total = 0;

    std::cout << "\n=== Float Precision Tests (C++) ===\n" << std::endl;

    // Test 1: Special values
    total++;
    if (test_float_format(NAN, "null", "NaN -> null"))
        passed++;
    total++;
    if (test_float_format(INFINITY, "null", "Inf -> null"))
        passed++;
    total++;
    if (test_float_format(-INFINITY, "null", "-Inf -> null"))
        passed++;
    total++;
    if (test_float_format(0.0, "0.0", "Zero"))
        passed++;

    // Test 2: Common values (mirror Python tests)
    total++;
    if (test_roundtrip(35.31, "35.31"))
        passed++;
    total++;
    if (test_roundtrip(99.99, "99.99"))
        passed++;
    total++;
    if (test_roundtrip(19.99, "19.99"))
        passed++;
    total++;
    if (test_roundtrip(29.99, "29.99"))
        passed++;
    total++;
    if (test_roundtrip(0.01, "0.01"))
        passed++;
    total++;
    if (test_roundtrip(0.001, "0.001"))
        passed++;

    // Test 3: Negative values
    total++;
    if (test_roundtrip(-3.14, "-3.14"))
        passed++;
    total++;
    if (test_roundtrip(-99.99, "-99.99"))
        passed++;
    total++;
    if (test_roundtrip(-0.01, "-0.01"))
        passed++;

    // Test 4: Integer-like floats
    total++;
    if (test_float_format(1.0, "1.0", "1.0"))
        passed++;
    total++;
    if (test_float_format(42.0, "42.0", "42.0"))
        passed++;
    total++;
    if (test_float_format(100.0, "100.0", "100.0"))
        passed++;

    // Test 5: Very small values
    total++;
    if (test_roundtrip(1e-6, "1e-6"))
        passed++;
    total++;
    if (test_roundtrip(1e-5, "1e-5"))
        passed++;
    total++;
    if (test_roundtrip(0.00001, "0.00001"))
        passed++;

    // Test 6: Large values
    total++;
    if (test_roundtrip(1e14, "1e14"))
        passed++;
    total++;
    if (test_roundtrip(1e13, "1e13"))
        passed++;

    // Test 7: Scientific notation
    total++;
    if (test_float_format(1e20, "e", "1e20 uses sci notation"))
        passed++;
    total++;
    if (test_float_format(1e-10, "e", "1e-10 uses sci notation"))
        passed++;

    // Test 8: Precision limits
    total++;
    if (test_roundtrip(123.456789, "123.456789"))
        passed++;
    total++;
    if (test_roundtrip(0.123456789, "0.123456789"))
        passed++;

    // Test 9: Dragonbox path (same contract as ryu_d2s; used by Python dumps)
    total++;
    {
        char buf[64];
        int len = dragonbox_d2s(3.14, buf);
        buf[len] = '\0';
        double parsed = std::stod(buf);
        bool ok = (std::abs(parsed - 3.14) < 1e-10);
        if (ok) {
            passed++;
            std::cout << "PASSED: dragonbox_d2s(3.14) round-trip -> " << buf << std::endl;
        } else {
            std::cerr << "FAILED: dragonbox_d2s(3.14) -> " << buf << " parsed " << parsed
                      << std::endl;
        }
    }

    // Summary
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;

    if (passed == total) {
        std::cout << "✅ All tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "❌ " << (total - passed) << " test(s) failed" << std::endl;
        return 1;
    }
}

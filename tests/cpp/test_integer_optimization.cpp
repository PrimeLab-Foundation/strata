/**
 * test_integer_optimization.cpp - Tests for integer parsing optimization
 *
 * Parity tests for tests/py/test_integer_optimization.py
 * These tests verify that integer parsing correctly handles all integer ranges.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>

using namespace strata;

static void assert_int_value(const JsonValue& value, int64_t expected) {
    assert(value.is_int());
    assert(value.as_int() == expected);
}

static void assert_double_value(const JsonValue& value, double expected) {
    assert(value.is_double());
    assert(value.as_double() == expected);
}

// ============================================================================
// Small Integer Parsing Tests
// ============================================================================

void test_zero() {
    auto result = parse_json("0");
    assert(result.ok());
    assert_int_value(result.value, 0);
    std::cout << "✓ test_zero passed\n";
}

void test_small_positive_integers() {
    auto r1 = parse_json("1");
    assert(r1.ok());
    assert_int_value(r1.value, 1);

    auto r2 = parse_json("127");
    assert(r2.ok());
    assert_int_value(r2.value, 127);

    auto r3 = parse_json("255");
    assert(r3.ok());
    assert_int_value(r3.value, 255);

    auto r4 = parse_json("256");
    assert(r4.ok());
    assert_int_value(r4.value, 256);

    std::cout << "✓ test_small_positive_integers passed\n";
}

void test_small_negative_integers() {
    auto r1 = parse_json("-1");
    assert(r1.ok());
    assert_int_value(r1.value, -1);

    auto r2 = parse_json("-5");
    assert(r2.ok());
    assert_int_value(r2.value, -5);

    auto r3 = parse_json("-128");
    assert(r3.ok());
    assert_int_value(r3.value, -128);

    std::cout << "✓ test_small_negative_integers passed\n";
}

void test_cache_boundary_values() {
    // Values at typical cache boundaries
    auto r1 = parse_json("-6");
    assert(r1.ok());
    assert_int_value(r1.value, -6);

    auto r2 = parse_json("257");
    assert(r2.ok());
    assert_int_value(r2.value, 257);

    std::cout << "✓ test_cache_boundary_values passed\n";
}

// ============================================================================
// Integer Boundary Tests
// ============================================================================

void test_int64_max() {
    // INT64_MAX = 2^63 - 1 = 9223372036854775807
    auto result = parse_json("9223372036854775807");
    assert(result.ok());
    assert_int_value(result.value, std::numeric_limits<int64_t>::max());
    std::cout << "✓ test_int64_max passed\n";
}

void test_int64_min() {
    // INT64_MIN = -2^63 = -9223372036854775808
    auto result = parse_json("-9223372036854775808");
    assert(result.ok());
    assert_int_value(result.value, std::numeric_limits<int64_t>::min());
    std::cout << "✓ test_int64_min passed\n";
}

void test_uint64_max() {
    // UINT64_MAX = 2^64 - 1 = 18446744073709551615
    auto result = parse_json("18446744073709551615");
    assert(result.ok());
    // Very large numbers may have precision loss as double
    double expected = 18446744073709551615.0;
    assert(result.value.is_double());
    assert(std::abs(result.value.as_double() - expected) / expected < 1e-15);
    std::cout << "✓ test_uint64_max passed\n";
}

void test_int32_boundaries() {
    // INT32_MAX = 2147483647
    auto r1 = parse_json("2147483647");
    assert(r1.ok());
    assert_int_value(r1.value, 2147483647);

    // INT32_MIN = -2147483648
    auto r2 = parse_json("-2147483648");
    assert(r2.ok());
    assert_int_value(r2.value, -2147483648LL);

    std::cout << "✓ test_int32_boundaries passed\n";
}

void test_long_boundaries() {
    // Test values at typical long boundaries
    auto r1 = parse_json("2147483647");
    assert(r1.ok());
    assert_int_value(r1.value, 2147483647);

    auto r2 = parse_json("-2147483648");
    assert(r2.ok());
    assert_int_value(r2.value, -2147483648LL);

    std::cout << "✓ test_long_boundaries passed\n";
}

// ============================================================================
// Integer Arrays and Objects Tests
// ============================================================================

void test_array_of_small_integers() {
    auto result = parse_json("[0, 1, 2, 3, 127, 255, 256]");
    assert(result.ok());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 7);
    assert_int_value(arr[0], 0);
    assert_int_value(arr[1], 1);
    assert_int_value(arr[2], 2);
    assert_int_value(arr[3], 3);
    assert_int_value(arr[4], 127);
    assert_int_value(arr[5], 255);
    assert_int_value(arr[6], 256);
    std::cout << "✓ test_array_of_small_integers passed\n";
}

void test_array_of_negative_integers() {
    auto result = parse_json("[-1, -5, -6, -128, -256]");
    assert(result.ok());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 5);
    assert_int_value(arr[0], -1);
    assert_int_value(arr[1], -5);
    assert_int_value(arr[2], -6);
    assert_int_value(arr[3], -128);
    assert_int_value(arr[4], -256);
    std::cout << "✓ test_array_of_negative_integers passed\n";
}

void test_array_of_mixed_integers() {
    auto result = parse_json("[-5, 0, 256, 257, 9223372036854775807]");
    assert(result.ok());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 5);
    assert_int_value(arr[0], -5);
    assert_int_value(arr[1], 0);
    assert_int_value(arr[2], 256);
    assert_int_value(arr[3], 257);
    assert_int_value(arr[4], std::numeric_limits<int64_t>::max());
    std::cout << "✓ test_array_of_mixed_integers passed\n";
}

void test_object_with_integer_values() {
    auto result = parse_json("{\"zero\": 0, \"small\": 42, \"large\": 9223372036854775807}");
    assert(result.ok());
    const auto& obj = result.value.as_object();
    assert_int_value(obj.at("zero"), 0);
    assert_int_value(obj.at("small"), 42);
    assert_int_value(obj.at("large"), std::numeric_limits<int64_t>::max());
    std::cout << "✓ test_object_with_integer_values passed\n";
}

// ============================================================================
// Integer Round-trip Tests
// ============================================================================

void test_integer_roundtrip() {
    // Test various integer values through serialize and parse
    std::vector<int64_t> test_values = {
        0, 1, -1, -5, 256, 257, -6, 127, -128,
        2147483647LL, -2147483648LL
    };

    for (int64_t value : test_values) {
        std::string json_str = std::to_string(value);
        auto parsed = parse_json(json_str);
        assert(parsed.ok());
        assert_int_value(parsed.value, value);
    }

    std::cout << "✓ test_integer_roundtrip passed\n";
}

void test_serialize_roundtrip() {
    // Test serialization and re-parsing
    JsonValue::Object obj;
    obj["zero"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(0)));
    obj["positive"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(42)));
    obj["negative"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(-123)));
    obj["large"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(2147483647)));

    JsonValue root(JsonValue::Variant(std::move(obj)));
    std::string serialized = serialize_json(root);
    auto reparsed = parse_json(serialized);
    assert(reparsed.ok());
    assert_int_value(reparsed.value.as_object().at("zero"), 0);
    assert_int_value(reparsed.value.as_object().at("positive"), 42);
    assert_int_value(reparsed.value.as_object().at("negative"), -123);
    assert_int_value(reparsed.value.as_object().at("large"), 2147483647);

    std::cout << "✓ test_serialize_roundtrip passed\n";
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_leading_zeros_rejected() {
    // Leading zeros are invalid in JSON numbers
    auto r1 = parse_json("01");
    assert(!r1.ok());

    auto r2 = parse_json("00");
    assert(!r2.ok());

    std::cout << "✓ test_leading_zeros_rejected passed\n";
}

void test_negative_zero() {
    // -0 should be parsed as 0
    auto result = parse_json("-0");
    assert(result.ok());
    assert_int_value(result.value, 0);
    std::cout << "✓ test_negative_zero passed\n";
}

void test_scientific_notation_integers() {
    // Scientific notation for integer values
    auto r1 = parse_json("1e2");
    assert(r1.ok());
    assert_double_value(r1.value, 100.0);

    auto r2 = parse_json("1E3");
    assert(r2.ok());
    assert_double_value(r2.value, 1000.0);

    std::cout << "✓ test_scientific_notation_integers passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running Integer Optimization tests...\n\n";

    // Small integers
    std::cout << "--- Small Integer Parsing ---\n";
    test_zero();
    test_small_positive_integers();
    test_small_negative_integers();
    test_cache_boundary_values();

    // Integer boundaries
    std::cout << "\n--- Integer Boundaries ---\n";
    test_int64_max();
    test_int64_min();
    test_uint64_max();
    test_int32_boundaries();
    test_long_boundaries();

    // Arrays and objects
    std::cout << "\n--- Integer Arrays and Objects ---\n";
    test_array_of_small_integers();
    test_array_of_negative_integers();
    test_array_of_mixed_integers();
    test_object_with_integer_values();

    // Round-trip
    std::cout << "\n--- Integer Round-trip ---\n";
    test_integer_roundtrip();
    test_serialize_roundtrip();

    // Edge cases
    std::cout << "\n--- Edge Cases ---\n";
    test_leading_zeros_rejected();
    test_negative_zero();
    test_scientific_notation_integers();

    std::cout << "\n✅ All Integer Optimization tests passed!\n";
    return 0;
}

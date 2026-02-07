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

// ============================================================================
// Small Integer Parsing Tests
// ============================================================================

void test_zero() {
    auto result = parse_json("0");
    assert(result.ok());
    assert(result.value.as_number() == 0);
    std::cout << "✓ test_zero passed\n";
}

void test_small_positive_integers() {
    auto r1 = parse_json("1");
    assert(r1.ok() && r1.value.as_number() == 1);

    auto r2 = parse_json("127");
    assert(r2.ok() && r2.value.as_number() == 127);

    auto r3 = parse_json("255");
    assert(r3.ok() && r3.value.as_number() == 255);

    auto r4 = parse_json("256");
    assert(r4.ok() && r4.value.as_number() == 256);

    std::cout << "✓ test_small_positive_integers passed\n";
}

void test_small_negative_integers() {
    auto r1 = parse_json("-1");
    assert(r1.ok() && r1.value.as_number() == -1);

    auto r2 = parse_json("-5");
    assert(r2.ok() && r2.value.as_number() == -5);

    auto r3 = parse_json("-128");
    assert(r3.ok() && r3.value.as_number() == -128);

    std::cout << "✓ test_small_negative_integers passed\n";
}

void test_cache_boundary_values() {
    // Values at typical cache boundaries
    auto r1 = parse_json("-6");
    assert(r1.ok() && r1.value.as_number() == -6);

    auto r2 = parse_json("257");
    assert(r2.ok() && r2.value.as_number() == 257);

    std::cout << "✓ test_cache_boundary_values passed\n";
}

// ============================================================================
// Integer Boundary Tests
// ============================================================================

void test_int64_max() {
    // INT64_MAX = 2^63 - 1 = 9223372036854775807
    auto result = parse_json("9223372036854775807");
    assert(result.ok());
    // Note: stored as double, may lose precision at extremes
    double expected = 9223372036854775807.0;
    assert(std::abs(result.value.as_number() - expected) / expected < 1e-15);
    std::cout << "✓ test_int64_max passed\n";
}

void test_int64_min() {
    // INT64_MIN = -2^63 = -9223372036854775808
    auto result = parse_json("-9223372036854775808");
    assert(result.ok());
    double expected = -9223372036854775808.0;
    assert(std::abs(result.value.as_number() - expected) / std::abs(expected) < 1e-15);
    std::cout << "✓ test_int64_min passed\n";
}

void test_uint64_max() {
    // UINT64_MAX = 2^64 - 1 = 18446744073709551615
    auto result = parse_json("18446744073709551615");
    assert(result.ok());
    // Very large numbers may have precision loss as double
    double expected = 18446744073709551615.0;
    assert(std::abs(result.value.as_number() - expected) / expected < 1e-15);
    std::cout << "✓ test_uint64_max passed\n";
}

void test_int32_boundaries() {
    // INT32_MAX = 2147483647
    auto r1 = parse_json("2147483647");
    assert(r1.ok() && r1.value.as_number() == 2147483647);

    // INT32_MIN = -2147483648
    auto r2 = parse_json("-2147483648");
    assert(r2.ok() && r2.value.as_number() == -2147483648);

    std::cout << "✓ test_int32_boundaries passed\n";
}

void test_long_boundaries() {
    // Test values at typical long boundaries
    auto r1 = parse_json("2147483647");
    assert(r1.ok() && r1.value.as_number() == 2147483647);

    auto r2 = parse_json("-2147483648");
    assert(r2.ok() && r2.value.as_number() == -2147483648);

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
    assert(arr[0].as_number() == 0);
    assert(arr[1].as_number() == 1);
    assert(arr[2].as_number() == 2);
    assert(arr[3].as_number() == 3);
    assert(arr[4].as_number() == 127);
    assert(arr[5].as_number() == 255);
    assert(arr[6].as_number() == 256);
    std::cout << "✓ test_array_of_small_integers passed\n";
}

void test_array_of_negative_integers() {
    auto result = parse_json("[-1, -5, -6, -128, -256]");
    assert(result.ok());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 5);
    assert(arr[0].as_number() == -1);
    assert(arr[1].as_number() == -5);
    assert(arr[2].as_number() == -6);
    assert(arr[3].as_number() == -128);
    assert(arr[4].as_number() == -256);
    std::cout << "✓ test_array_of_negative_integers passed\n";
}

void test_array_of_mixed_integers() {
    auto result = parse_json("[-5, 0, 256, 257, 9223372036854775807]");
    assert(result.ok());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 5);
    assert(arr[0].as_number() == -5);
    assert(arr[1].as_number() == 0);
    assert(arr[2].as_number() == 256);
    assert(arr[3].as_number() == 257);
    // Large number with precision tolerance
    assert(std::abs(arr[4].as_number() - 9223372036854775807.0) / 9223372036854775807.0 < 1e-15);
    std::cout << "✓ test_array_of_mixed_integers passed\n";
}

void test_object_with_integer_values() {
    auto result = parse_json("{\"zero\": 0, \"small\": 42, \"large\": 9223372036854775807}");
    assert(result.ok());
    const auto& obj = result.value.as_object();
    assert(obj.at("zero").as_number() == 0);
    assert(obj.at("small").as_number() == 42);
    assert(std::abs(obj.at("large").as_number() - 9223372036854775807.0) / 9223372036854775807.0 < 1e-15);
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
        assert(static_cast<int64_t>(parsed.value.as_number()) == value);
    }

    std::cout << "✓ test_integer_roundtrip passed\n";
}

void test_serialize_roundtrip() {
    // Test serialization and re-parsing
    JsonValue::Object obj;
    obj["zero"] = JsonValue(JsonValue::Variant(0.0));
    obj["positive"] = JsonValue(JsonValue::Variant(42.0));
    obj["negative"] = JsonValue(JsonValue::Variant(-123.0));
    obj["large"] = JsonValue(JsonValue::Variant(2147483647.0));

    JsonValue root(JsonValue::Variant(std::move(obj)));
    std::string serialized = serialize_json(root);
    auto reparsed = parse_json(serialized);
    assert(reparsed.ok());
    assert(reparsed.value.as_object().at("zero").as_number() == 0);
    assert(reparsed.value.as_object().at("positive").as_number() == 42);
    assert(reparsed.value.as_object().at("negative").as_number() == -123);
    assert(reparsed.value.as_object().at("large").as_number() == 2147483647);

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
    assert(result.value.as_number() == 0);
    std::cout << "✓ test_negative_zero passed\n";
}

void test_scientific_notation_integers() {
    // Scientific notation for integer values
    auto r1 = parse_json("1e2");
    assert(r1.ok());
    assert(r1.value.as_number() == 100);

    auto r2 = parse_json("1E3");
    assert(r2.ok());
    assert(r2.value.as_number() == 1000);

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

/**
 * test_json_tape.cpp - Tests for JSON tape format (ADR-0003)
 *
 * Tests cover:
 * - TapeBuilder handling of all JSON types
 * - Tape iteration and value extraction
 * - Round-trip: JSON → Tape → DOM parity with direct JSON → DOM
 * - Edge cases: empty containers, unicode, escapes, deeply nested structures
 * - Numeric boundary cases
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_tape.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

using namespace strata;

// Helper to compare JsonValue trees for equality
bool json_equal(const JsonValue& a, const JsonValue& b) {
    if (a.is_null() && b.is_null()) return true;
    if (a.is_bool() && b.is_bool()) return a.as_bool() == b.as_bool();
    if (a.is_number() && b.is_number()) {
        double diff = std::abs(a.as_number() - b.as_number());
        if (a.as_number() == 0.0) return diff < 1e-15;
        return diff / std::abs(a.as_number()) < 1e-15;
    }
    if (a.is_string() && b.is_string()) return a.as_string() == b.as_string();
    if (a.is_array() && b.is_array()) {
        const auto& arr_a = a.as_array();
        const auto& arr_b = b.as_array();
        if (arr_a.size() != arr_b.size()) return false;
        for (size_t i = 0; i < arr_a.size(); ++i) {
            if (!json_equal(arr_a[i], arr_b[i])) return false;
        }
        return true;
    }
    if (a.is_object() && b.is_object()) {
        const auto& obj_a = a.as_object();
        const auto& obj_b = b.as_object();
        if (obj_a.size() != obj_b.size()) return false;
        for (const auto& [key, val] : obj_a) {
            auto it = obj_b.find(key);
            if (it == obj_b.end()) return false;
            if (!json_equal(val, it->second)) return false;
        }
        return true;
    }
    return false;
}

// Test helper for round-trip parity
void test_roundtrip(const char* json_str, const char* test_name) {
    // Direct parse
    auto direct_result = parse_json(json_str);
    assert(direct_result.ok());

    // Tape parse + DOM conversion
    auto tape_result = parse_to_tape(json_str);
    assert(tape_result.ok());
    auto dom_result = tape_to_dom(tape_result.value);
    assert(dom_result.ok());

    // Compare
    assert(json_equal(direct_result.value, dom_result.value));
    std::cout << "✓ " << test_name << " passed\n";
}

// ===========================================================================
// Basic Type Tests
// ===========================================================================

void test_tape_null() {
    test_roundtrip("null", "test_tape_null");
}

void test_tape_bool() {
    test_roundtrip("true", "test_tape_bool_true");
    test_roundtrip("false", "test_tape_bool_false");
}

void test_tape_numbers() {
    // Integer
    test_roundtrip("42", "test_tape_int");
    test_roundtrip("-123", "test_tape_negative_int");
    test_roundtrip("0", "test_tape_zero");

    // Float
    test_roundtrip("3.14", "test_tape_float");
    test_roundtrip("-0.5", "test_tape_negative_float");

    // Scientific notation
    test_roundtrip("1.23e10", "test_tape_scientific");
    test_roundtrip("1e-5", "test_tape_scientific_negative_exp");
}

void test_tape_strings() {
    test_roundtrip("\"hello\"", "test_tape_simple_string");
    test_roundtrip("\"\"", "test_tape_empty_string");
    test_roundtrip("\"hello world\"", "test_tape_string_with_space");
}

void test_tape_string_escapes() {
    test_roundtrip("\"line1\\nline2\"", "test_tape_newline_escape");
    test_roundtrip("\"say \\\"hello\\\"\"", "test_tape_quote_escape");
    test_roundtrip("\"a\\tb\"", "test_tape_tab_escape");
    test_roundtrip("\"back\\\\slash\"", "test_tape_backslash_escape");
    test_roundtrip("\"\\r\\n\"", "test_tape_crlf_escape");
}

void test_tape_unicode() {
    test_roundtrip("\"Hello 世界\"", "test_tape_utf8_direct");
    test_roundtrip("\"👋\"", "test_tape_emoji");
    test_roundtrip("\"\\u0041\"", "test_tape_unicode_escape_A");
    test_roundtrip("\"\\u03A9\"", "test_tape_unicode_escape_greek");
}

void test_tape_unicode_surrogate_pairs() {
    // Emoji via surrogate pair: 🎉 = U+1F389 = \uD83C\uDF89
    test_roundtrip("\"\\uD83C\\uDF89\"", "test_tape_surrogate_pair");
}

// ===========================================================================
// Container Tests
// ===========================================================================

void test_tape_empty_array() {
    test_roundtrip("[]", "test_tape_empty_array");
}

void test_tape_simple_array() {
    test_roundtrip("[1, 2, 3]", "test_tape_simple_array");
}

void test_tape_mixed_array() {
    test_roundtrip("[null, true, false, 42, \"hello\"]", "test_tape_mixed_array");
}

void test_tape_nested_arrays() {
    test_roundtrip("[[1, 2], [3, 4]]", "test_tape_nested_arrays");
    test_roundtrip("[[[1]]]", "test_tape_deeply_nested_array");
}

void test_tape_empty_object() {
    test_roundtrip("{}", "test_tape_empty_object");
}

void test_tape_simple_object() {
    test_roundtrip("{\"a\": 1, \"b\": 2}", "test_tape_simple_object");
}

void test_tape_nested_object() {
    test_roundtrip("{\"outer\": {\"inner\": 42}}", "test_tape_nested_object");
}

void test_tape_complex_structure() {
    const char* json = R"({
        "name": "test",
        "numbers": [1, 2, 3],
        "nested": {
            "flag": true,
            "value": null
        },
        "empty": [],
        "empty_obj": {}
    })";
    test_roundtrip(json, "test_tape_complex_structure");
}

// ===========================================================================
// Tape Iterator Tests
// ===========================================================================

void test_tape_iterator_primitives() {
    auto tape_result = parse_to_tape("[null, true, false, 42, 3.14, \"hello\"]");
    assert(tape_result.ok());
    const JsonTape& tape = tape_result.value;

    auto it = tape.begin();

    // StartArray
    assert(it.token() == TapeToken::StartArray);
    assert(it.container_size() == 6);
    ++it;

    // Null
    assert(it.token() == TapeToken::Null);
    ++it;

    // True
    assert(it.token() == TapeToken::True);
    ++it;

    // False
    assert(it.token() == TapeToken::False);
    ++it;

    // Int64
    assert(it.token() == TapeToken::Int64);
    assert(it.as_int64() == 42);
    ++it;

    // Double
    assert(it.token() == TapeToken::Double);
    assert(std::abs(it.as_double() - 3.14) < 0.001);
    ++it;

    // String
    assert(it.token() == TapeToken::String);
    assert(it.as_string() == "hello");
    ++it;

    // EndArray
    assert(it.token() == TapeToken::EndArray);
    ++it;

    // RootEnd
    assert(it.token() == TapeToken::RootEnd);

    std::cout << "✓ test_tape_iterator_primitives passed\n";
}

void test_tape_iterator_object() {
    auto tape_result = parse_to_tape("{\"name\": \"Alice\", \"age\": 30}");
    assert(tape_result.ok());
    const JsonTape& tape = tape_result.value;

    auto it = tape.begin();

    // StartObject
    assert(it.token() == TapeToken::StartObject);
    assert(it.container_size() == 2);
    ++it;

    // Key "name" (or "age" - order may vary)
    assert(it.token() == TapeToken::Key);
    std::string_view key1 = it.as_string();
    ++it;

    // Value for first key
    if (key1 == "name") {
        assert(it.token() == TapeToken::String);
        assert(it.as_string() == "Alice");
    } else {
        assert(it.token() == TapeToken::Int64);
        assert(it.as_int64() == 30);
    }
    ++it;

    // Key for second entry
    assert(it.token() == TapeToken::Key);
    ++it;

    // Value for second key
    assert(it.token() == TapeToken::String || it.token() == TapeToken::Int64);
    ++it;

    // EndObject
    assert(it.token() == TapeToken::EndObject);

    std::cout << "✓ test_tape_iterator_object passed\n";
}

// ===========================================================================
// Edge Cases
// ===========================================================================

void test_tape_whitespace() {
    test_roundtrip("  [  1  ,  2  ]  ", "test_tape_whitespace");
    test_roundtrip("{\n  \"a\": 1\n}", "test_tape_whitespace_object");
}

void test_tape_large_numbers() {
    // Large integers (within int64 range)
    test_roundtrip("9223372036854775807", "test_tape_max_int64");
    test_roundtrip("-9223372036854775808", "test_tape_min_int64");

    // Large doubles
    test_roundtrip("1.7976931348623157e308", "test_tape_large_double");
    test_roundtrip("2.2250738585072014e-308", "test_tape_small_double");
}

void test_tape_deeply_nested() {
    // Build deeply nested array
    std::string json = "[";
    for (int i = 0; i < 100; ++i) json += "[";
    json += "1";
    for (int i = 0; i < 100; ++i) json += "]";
    json += "]";

    test_roundtrip(json.c_str(), "test_tape_deeply_nested");
}

void test_tape_many_keys() {
    // Object with many keys
    std::string json = "{";
    for (int i = 0; i < 100; ++i) {
        if (i > 0) json += ",";
        json += "\"key" + std::to_string(i) + "\": " + std::to_string(i);
    }
    json += "}";

    test_roundtrip(json.c_str(), "test_tape_many_keys");
}

void test_tape_large_array() {
    // Array with many elements
    std::string json = "[";
    for (int i = 0; i < 1000; ++i) {
        if (i > 0) json += ",";
        json += std::to_string(i);
    }
    json += "]";

    test_roundtrip(json.c_str(), "test_tape_large_array");
}

// ===========================================================================
// Memory and Stats Tests
// ===========================================================================

void test_tape_memory_stats() {
    auto tape_result = parse_to_tape("{\"name\": \"Alice\", \"values\": [1, 2, 3]}");
    assert(tape_result.ok());
    const JsonTape& tape = tape_result.value;

    // Verify tape has non-zero content
    assert(tape.token_count() > 0);
    assert(tape.value_count() > 0);
    assert(tape.string_size() > 0);
    assert(tape.memory_usage() > 0);

    std::cout << "✓ test_tape_memory_stats passed\n";
}

void test_tape_empty_check() {
    JsonTape empty_tape;
    assert(empty_tape.empty());
    assert(empty_tape.token_count() == 0);

    auto tape_result = parse_to_tape("null");
    assert(tape_result.ok());
    assert(!tape_result.value.empty());

    std::cout << "✓ test_tape_empty_check passed\n";
}

// ===========================================================================
// Error Cases
// ===========================================================================

void test_tape_invalid_json() {
    auto result = parse_to_tape("{invalid}");
    assert(!result.ok());

    auto result2 = parse_to_tape("[1, 2,]");
    assert(!result2.ok());

    auto result3 = parse_to_tape("");
    assert(!result3.ok());

    std::cout << "✓ test_tape_invalid_json passed\n";
}

void test_tape_to_dom_empty() {
    JsonTape empty_tape;
    auto result = tape_to_dom(empty_tape);
    assert(!result.ok());

    std::cout << "✓ test_tape_to_dom_empty passed\n";
}

// ===========================================================================
// Main
// ===========================================================================

int main() {
    std::cout << "=== JSON Tape Tests ===\n\n";

    // Basic types
    test_tape_null();
    test_tape_bool();
    test_tape_numbers();
    test_tape_strings();
    test_tape_string_escapes();
    test_tape_unicode();
    test_tape_unicode_surrogate_pairs();

    // Containers
    test_tape_empty_array();
    test_tape_simple_array();
    test_tape_mixed_array();
    test_tape_nested_arrays();
    test_tape_empty_object();
    test_tape_simple_object();
    test_tape_nested_object();
    test_tape_complex_structure();

    // Iterator
    test_tape_iterator_primitives();
    test_tape_iterator_object();

    // Edge cases
    test_tape_whitespace();
    test_tape_large_numbers();
    test_tape_deeply_nested();
    test_tape_many_keys();
    test_tape_large_array();

    // Memory/stats
    test_tape_memory_stats();
    test_tape_empty_check();

    // Errors
    test_tape_invalid_json();
    test_tape_to_dom_empty();

    std::cout << "\n=== All JSON Tape Tests Passed! ===\n";
    return 0;
}

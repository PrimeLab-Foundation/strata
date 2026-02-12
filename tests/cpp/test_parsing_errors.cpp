/**
 * test_parsing_errors.cpp - Tests for error handling of invalid JSON
 *
 * Parity tests for tests/py/test_parsing_errors.py
 * Ensures that invalid JSON and edge cases are properly rejected with errors.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace strata;

// ============================================================================
// Invalid JSON Tests
// ============================================================================

void test_empty_string() {
    auto result = parse_json("");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_empty_string passed\n";
}

void test_only_whitespace() {
    auto result = parse_json("   ");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_only_whitespace passed\n";
}

void test_incomplete_object() {
    auto result = parse_json("{");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_incomplete_object passed\n";
}

void test_incomplete_array() {
    auto result = parse_json("[");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_incomplete_array passed\n";
}

void test_incomplete_string() {
    auto result = parse_json("\"incomplete");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_incomplete_string passed\n";
}

void test_trailing_comma_in_array() {
    auto result = parse_json("[1, 2,]");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_trailing_comma_in_array passed\n";
}

void test_trailing_comma_in_object() {
    auto result = parse_json("{\"a\": 1,}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_trailing_comma_in_object passed\n";
}

void test_missing_colon_in_object() {
    auto result = parse_json("{\"key\" \"value\"}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_missing_colon_in_object passed\n";
}

void test_missing_comma_in_array() {
    auto result = parse_json("[1 2 3]");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_missing_comma_in_array passed\n";
}

void test_missing_comma_in_object() {
    auto result = parse_json("{\"a\": 1 \"b\": 2}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_missing_comma_in_object passed\n";
}

void test_unquoted_key() {
    auto result = parse_json("{key: \"value\"}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_unquoted_key passed\n";
}

void test_single_quotes_rejected() {
    auto result = parse_json("{'key': 'value'}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_single_quotes_rejected passed\n";
}

void test_trailing_text() {
    auto result = parse_json("{\"key\": \"value\"} extra");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_trailing_text passed\n";
}

void test_multiple_top_level_values() {
    auto result = parse_json("{\"a\": 1} {\"b\": 2}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_multiple_top_level_values passed\n";
}

void test_invalid_keyword() {
    auto result = parse_json("undefined");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_keyword passed\n";
}

void test_invalid_escape_sequence() {
    auto result = parse_json("\"invalid \\x escape\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_escape_sequence passed\n";
}

void test_invalid_unicode_escape_short() {
    auto result = parse_json("\"\\u12\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_unicode_escape_short passed\n";
}

void test_invalid_unicode_escape_hex() {
    auto result = parse_json("\"\\u12X4\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_unicode_escape_hex passed\n";
}

void test_truncated_escape_sequence() {
    auto result = parse_json("\"abc\\");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_truncated_escape_sequence passed\n";
}

void test_invalid_surrogate_missing_u() {
    auto result = parse_json("\"\\uD800\\q\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_surrogate_missing_u passed\n";
}

void test_invalid_surrogate_short_low() {
    auto result = parse_json("\"\\uD800\\u12\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_surrogate_short_low passed\n";
}

void test_invalid_surrogate_out_of_range() {
    auto result = parse_json("\"\\uD800\\u0000\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_surrogate_out_of_range passed\n";
}

void test_unclosed_array_nested() {
    auto result = parse_json("{\"array\": [1, 2}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_unclosed_array_nested passed\n";
}

void test_mismatched_brackets() {
    auto result = parse_json("[}");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_mismatched_brackets passed\n";
}

void test_mismatched_braces() {
    auto result = parse_json("{]");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_mismatched_braces passed\n";
}

void test_control_characters_rejected() {
    // Control character (U+0001) in string
    auto result = parse_json("\"\x01\x62\x61\x64\"");  // "\x01bad"
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_control_characters_rejected passed\n";
}

// ============================================================================
// Number Parsing Error Tests
// ============================================================================

void test_leading_zero_not_allowed() {
    auto result1 = parse_json("01");
    assert(!result1.ok());
    assert(result1.status == Status::ParseError);

    auto result2 = parse_json("00");
    assert(!result2.ok());
    assert(result2.status == Status::ParseError);

    std::cout << "✓ test_leading_zero_not_allowed passed\n";
}

void test_plus_sign_not_allowed() {
    auto result = parse_json("+42");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_plus_sign_not_allowed passed\n";
}

void test_hex_not_allowed() {
    auto result = parse_json("0x1A");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_hex_not_allowed passed\n";
}

void test_octal_not_allowed() {
    auto result = parse_json("0o17");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_octal_not_allowed passed\n";
}

void test_binary_not_allowed() {
    auto result = parse_json("0b1010");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_binary_not_allowed passed\n";
}

void test_dot_without_fraction() {
    auto result = parse_json("3.");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_dot_without_fraction passed\n";
}

void test_exponent_without_number() {
    auto result = parse_json("e10");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_exponent_without_number passed\n";
}

void test_incomplete_scientific_notation() {
    auto result1 = parse_json("1e");
    assert(!result1.ok());
    assert(result1.status == Status::ParseError);

    auto result2 = parse_json("1e+");
    assert(!result2.ok());
    assert(result2.status == Status::ParseError);

    std::cout << "✓ test_incomplete_scientific_notation passed\n";
}

// ============================================================================
// String Parsing Error Tests
// ============================================================================

void test_unclosed_string() {
    auto result = parse_json("\"unclosed");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_unclosed_string passed\n";
}

void test_invalid_escape() {
    auto result = parse_json("\"invalid \\x escape\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_escape passed\n";
}

void test_backslash_at_end_unescaped() {
    // Backslash followed by closing quote without escaping it
    auto result = parse_json("\"ends with backslash\\\"");
    // This is actually a valid string containing: ends with backslash"
    // The backslash escapes the quote, making it not the end
    // So this should be an unclosed string error
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_backslash_at_end_unescaped passed\n";
}

// ============================================================================
// Edge Cases Tests
// ============================================================================

void test_extremely_deep_nesting() {
    // Test very deep nesting (may hit recursion limits)
    const int depth = 1000;
    std::string json_str;
    for (int i = 0; i < depth; ++i) json_str += "[";
    json_str += "null";
    for (int i = 0; i < depth; ++i) json_str += "]";

    auto result = parse_json(json_str);
    // It's acceptable to either succeed or fail on extremely deep nesting
    if (result.ok()) {
        // Verify it's an array
        assert(result.value.is_array());
    }
    // If it fails, that's also acceptable behavior
    std::cout << "✓ test_extremely_deep_nesting passed\n";
}

void test_depth_limit_exceeded() {
    // Exceed parser depth limit (kMaxNestingDepth = 10000)
    const int depth = 10001;
    std::string json;
    json.reserve(static_cast<size_t>(depth) * 2 + 2);
    for (int i = 0; i < depth; ++i) json.push_back('[');
    json += "0";
    for (int i = 0; i < depth; ++i) json.push_back(']');

    auto result = parse_json(json);
    assert(!result.ok());

    std::cout << "✓ test_depth_limit_exceeded passed\n";
}

void test_truncated_json_boundaries() {
    const std::string json = R"({"a":[1,2,{"b":true},"x"],"c":null})";
    for (size_t i = 1; i < json.size(); ++i) {
        auto result = parse_json(json.substr(0, i));
        assert(!result.ok());
    }

    std::cout << "✓ test_truncated_json_boundaries passed\n";
}

void test_large_invalid_array_missing_comma() {
    // Ensure size is large enough to trigger structural tape path (>= 4KB).
    const size_t count = 3000;
    std::string json;
    json.reserve(count * 2 + 2);
    json.push_back('[');
    for (size_t i = 0; i < count; ++i) {
        json += "1";
        if (i + 1 < count) {
            if (i == 100) {
                json.push_back(' '); // Missing comma between elements
            } else {
                json.push_back(',');
            }
        }
    }
    json.push_back(']');

    auto result = parse_json(json);
    assert(!result.ok());

    std::cout << "✓ test_large_invalid_array_missing_comma passed\n";
}

void test_large_invalid_object_missing_colon() {
    const size_t count = 800;
    std::string json;
    json.reserve(count * 12 + 2);
    json.push_back('{');
    for (size_t i = 0; i < count; ++i) {
        if (i > 0) {
            json.push_back(',');
        }
        json += "\"k";
        json += std::to_string(i);
        json += "\"";
        if (i == 200) {
            json += " 1"; // Missing colon before value
        } else {
            json += ":1";
        }
    }
    json.push_back('}');

    auto result = parse_json(json);
    assert(!result.ok());

    std::cout << "✓ test_large_invalid_object_missing_colon passed\n";
}

void test_empty_key_in_object() {
    // Empty string as key is valid JSON
    auto result = parse_json("{\"\" : \"empty key\"}");
    assert(result.ok());
    assert(result.value.as_object().at("").as_string() == "empty key");
    std::cout << "✓ test_empty_key_in_object passed\n";
}

void test_duplicate_keys() {
    // JSON allows duplicate keys (behavior is implementation-defined)
    auto result = parse_json("{\"key\": 1, \"key\": 2}");
    assert(result.ok());
    // Should not crash, and should have some value
    double value = result.value.as_object().at("key").as_number();
    assert(value == 1 || value == 2);  // Either is acceptable
    std::cout << "✓ test_duplicate_keys passed\n";
}

void test_duplicate_key_policy_first_wins() {
    // Save current policy and restore after test
    auto original_policy = get_duplicate_key_policy();

    set_duplicate_key_policy(DuplicateKeyPolicy::FirstWins);
    auto result = parse_json("{\"key\": 1, \"key\": 2}");
    assert(result.ok());
    assert(result.value.as_object().at("key").as_number() == 1);

    set_duplicate_key_policy(original_policy);
    std::cout << "✓ test_duplicate_key_policy_first_wins passed\n";
}

void test_duplicate_key_policy_last_wins() {
    auto original_policy = get_duplicate_key_policy();

    set_duplicate_key_policy(DuplicateKeyPolicy::LastWins);
    auto result = parse_json("{\"key\": 1, \"key\": 2}");
    assert(result.ok());
    assert(result.value.as_object().at("key").as_number() == 2);

    set_duplicate_key_policy(original_policy);
    std::cout << "✓ test_duplicate_key_policy_last_wins passed\n";
}

void test_duplicate_key_policy_warn() {
    auto original_policy = get_duplicate_key_policy();

    set_duplicate_key_policy(DuplicateKeyPolicy::Warn);
    auto result = parse_json("{\"key\": 1, \"key\": 2}");
    assert(result.ok());
    // Warn policy should succeed and return first value (like FirstWins)
    assert(result.value.as_object().at("key").as_number() == 1);

    set_duplicate_key_policy(original_policy);
    std::cout << "✓ test_duplicate_key_policy_warn passed\n";
}

void test_duplicate_key_policy_error() {
    auto original_policy = get_duplicate_key_policy();

    set_duplicate_key_policy(DuplicateKeyPolicy::Error);
    auto result = parse_json("{\"key\": 1, \"key\": 2}");
    assert(!result.ok());  // Should fail with duplicate key

    set_duplicate_key_policy(original_policy);
    std::cout << "✓ test_duplicate_key_policy_error passed\n";
}

// ============================================================================
// Valid JSON Sanity Checks
// ============================================================================

void test_valid_null() {
    auto result = parse_json("null");
    assert(result.ok());
    assert(result.value.is_null());
    std::cout << "✓ test_valid_null passed\n";
}

void test_valid_bool() {
    auto result_true = parse_json("true");
    assert(result_true.ok());
    assert(result_true.value.is_bool());
    assert(result_true.value.as_bool() == true);

    auto result_false = parse_json("false");
    assert(result_false.ok());
    assert(result_false.value.is_bool());
    assert(result_false.value.as_bool() == false);

    std::cout << "✓ test_valid_bool passed\n";
}

void test_valid_number() {
    auto result = parse_json("42");
    assert(result.ok());
    assert(result.value.is_number());
    assert(result.value.as_number() == 42.0);
    std::cout << "✓ test_valid_number passed\n";
}

void test_valid_string() {
    auto result = parse_json("\"hello\"");
    assert(result.ok());
    assert(result.value.is_string());
    assert(result.value.as_string() == "hello");
    std::cout << "✓ test_valid_string passed\n";
}

void test_valid_array() {
    auto result = parse_json("[1, 2, 3]");
    assert(result.ok());
    assert(result.value.is_array());
    assert(result.value.as_array().size() == 3);
    std::cout << "✓ test_valid_array passed\n";
}

void test_valid_object() {
    auto result = parse_json("{\"a\": 1, \"b\": 2}");
    assert(result.ok());
    assert(result.value.is_object());
    assert(result.value.as_object().size() == 2);
    std::cout << "✓ test_valid_object passed\n";
}

void test_valid_zero() {
    // Zero is valid
    auto result = parse_json("0");
    assert(result.ok());
    assert(result.value.as_number() == 0.0);
    std::cout << "✓ test_valid_zero passed\n";
}

void test_valid_negative_number() {
    auto result = parse_json("-123");
    assert(result.ok());
    assert(result.value.as_number() == -123.0);
    std::cout << "✓ test_valid_negative_number passed\n";
}

void test_valid_scientific_notation() {
    auto result = parse_json("1.23e10");
    assert(result.ok());
    assert(result.value.as_number() == 1.23e10);
    std::cout << "✓ test_valid_scientific_notation passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running Parsing Errors tests...\n\n";

    // Invalid JSON
    std::cout << "--- Invalid JSON ---\n";
    test_empty_string();
    test_only_whitespace();
    test_incomplete_object();
    test_incomplete_array();
    test_incomplete_string();
    test_trailing_comma_in_array();
    test_trailing_comma_in_object();
    test_missing_colon_in_object();
    test_missing_comma_in_array();
    test_missing_comma_in_object();
    test_unquoted_key();
    test_single_quotes_rejected();
    test_trailing_text();
    test_multiple_top_level_values();
    test_invalid_keyword();
    test_invalid_escape_sequence();
    test_invalid_unicode_escape_short();
    test_invalid_unicode_escape_hex();
    test_truncated_escape_sequence();
    test_invalid_surrogate_missing_u();
    test_invalid_surrogate_short_low();
    test_invalid_surrogate_out_of_range();
    test_unclosed_array_nested();
    test_mismatched_brackets();
    test_mismatched_braces();
    test_control_characters_rejected();

    // Number parsing errors
    std::cout << "\n--- Number Parsing Errors ---\n";
    test_leading_zero_not_allowed();
    test_plus_sign_not_allowed();
    test_hex_not_allowed();
    test_octal_not_allowed();
    test_binary_not_allowed();
    test_dot_without_fraction();
    test_exponent_without_number();
    test_incomplete_scientific_notation();

    // String parsing errors
    std::cout << "\n--- String Parsing Errors ---\n";
    test_unclosed_string();
    test_invalid_escape();
    test_backslash_at_end_unescaped();

    // Edge cases
    std::cout << "\n--- Edge Cases ---\n";
    test_extremely_deep_nesting();
    test_depth_limit_exceeded();
    test_truncated_json_boundaries();
    test_large_invalid_array_missing_comma();
    test_large_invalid_object_missing_colon();
    test_empty_key_in_object();
    test_duplicate_keys();
    test_duplicate_key_policy_first_wins();
    test_duplicate_key_policy_last_wins();
    test_duplicate_key_policy_warn();
    test_duplicate_key_policy_error();

    // Valid JSON sanity checks
    std::cout << "\n--- Valid JSON Sanity Checks ---\n";
    test_valid_null();
    test_valid_bool();
    test_valid_number();
    test_valid_string();
    test_valid_array();
    test_valid_object();
    test_valid_zero();
    test_valid_negative_number();
    test_valid_scientific_notation();

    std::cout << "\n✅ All Parsing Errors tests passed!\n";
    return 0;
}

/**
 * test_search.cpp - Tests for JSONPath queries
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"
#include "strata/search/search.hpp"
#include "strata/search/search_ndjson_fused.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace strata;

std::vector<std::string> serialize_values(const std::vector<JsonValue>& values) {
    std::vector<std::string> out;
    out.reserve(values.size());
    for (const auto& v : values) {
        out.push_back(serialize_json(v));
    }
    return out;
}

static std::string make_object_with_keys(size_t total_keys, const std::string& target_key,
                                         const std::string& target_value) {
    std::string json;
    json.reserve(32 + total_keys * 16);
    json += "{";
    if (total_keys == 0) {
        json += "}";
        return json;
    }
    for (size_t i = 0; i < total_keys; ++i) {
        if (i > 0) {
            json += ",";
        }
        if (i == total_keys - 1) {
            json += "\"";
            json += target_key;
            json += "\":\"";
            json += target_value;
            json += "\"";
        } else {
            json += "\"k";
            json += std::to_string(i);
            json += "\":";
            json += std::to_string(i);
        }
    }
    json += "}";
    return json;
}

void test_compile_root() {
    auto result = compile_search_path("$");
    assert(result.ok());
    std::cout << "✓ test_compile_root passed\n";
}

void test_compile_field() {
    auto result = compile_search_path("$.name");
    assert(result.ok());
    std::cout << "✓ test_compile_field passed\n";
}

void test_compile_nested() {
    auto result = compile_search_path("$.user.name");
    assert(result.ok());
    std::cout << "✓ test_compile_nested passed\n";
}

void test_compile_wildcard() {
    auto result = compile_search_path("$.*");
    assert(result.ok());
    std::cout << "✓ test_compile_wildcard passed\n";
}

void test_compile_index() {
    auto result = compile_search_path("$[0]");
    assert(result.ok());
    std::cout << "✓ test_compile_index passed\n";
}

void test_compile_recursive() {
    auto result = compile_search_path("$..name");
    assert(result.ok());
    std::cout << "✓ test_compile_recursive passed\n";
}

void test_compile_slice() {
    auto result = compile_search_path("$[0:5]");
    assert(result.ok());
    std::cout << "✓ test_compile_slice passed\n";
}

void test_compile_filter() {
    auto result = compile_search_path("$[?(@.age > 30)]");
    assert(result.ok());
    std::cout << "✓ test_compile_filter passed\n";
}

void test_compile_errors() {
    // Invalid path
    auto result_bad1 = compile_search_path("invalid");
    assert(!result_bad1.ok());

    // Empty
    auto result_bad2 = compile_search_path("");
    assert(!result_bad2.ok());

    // Just whitespace
    auto result_bad3 = compile_search_path("   ");
    assert(!result_bad3.ok());

    // Invalid bracket expression
    auto result_bad4 = compile_search_path("$[abc]");
    assert(!result_bad4.ok());

    std::cout << "✓ test_compile_errors passed\n";
}

void test_compile_quoted_strings() {
    // Single-quoted string
    auto r1 = compile_search_path("$['hello']");
    assert(r1.ok());

    // Double-quoted string
    auto r2 = compile_search_path("$[\"world\"]");
    assert(r2.ok());

    // Mixed path with quoted key (hyphenated name)
    auto r3 = compile_search_path("$.user['first-name']");
    assert(r3.ok());

    // Quoted key with space
    auto r4 = compile_search_path("$['key with space']");
    assert(r4.ok());

    // Quoted key with special characters
    auto r5 = compile_search_path("$['key.with.dots']");
    assert(r5.ok());

    auto r6 = compile_search_path("$['key:with:colons']");
    assert(r6.ok());

    // Nested quoted access
    auto r7 = compile_search_path("$['outer']['inner']");
    assert(r7.ok());

    // Multiple quoted keys in sequence
    auto r8 = compile_search_path("$['a']['b']['c']");
    assert(r8.ok());

    // Unclosed quoted string - should fail (currently throws)
    bool caught_unclosed = false;
    try {
        auto r9 = compile_search_path("$['unclosed");
        (void)r9;  // Shouldn't reach here
    } catch (const std::runtime_error&) {
        caught_unclosed = true;
    }
    assert(caught_unclosed);

    std::cout << "✓ test_compile_quoted_strings passed\n";
}

void test_compile_quoted_escape_sequences() {
    // Test escape sequences within quoted JSONPath strings
    // These exercise the escape handling in parse_quoted_string()

    // Newline escape
    auto r1 = compile_search_path("$['hello\\nworld']");
    assert(r1.ok());

    // Tab escape
    auto r2 = compile_search_path("$['tab\\there']");
    assert(r2.ok());

    // Carriage return escape
    auto r3 = compile_search_path("$['line\\rreturn']");
    assert(r3.ok());

    // Backslash escape
    auto r4 = compile_search_path("$['back\\\\slash']");
    assert(r4.ok());

    // Quote escape in single-quoted string
    auto r5 = compile_search_path("$['it\\'s']");
    assert(r5.ok());

    // Quote escape in double-quoted string
    auto r6 = compile_search_path("$[\"say\\\"hello\\\"\"]");
    assert(r6.ok());

    // Other escape (default case - passes through)
    auto r7 = compile_search_path("$['hello\\xworld']");
    assert(r7.ok());

    // Multiple escapes in one string
    auto r8 = compile_search_path("$['line1\\nline2\\ttab']");
    assert(r8.ok());

    std::cout << "✓ test_compile_quoted_escape_sequences passed\n";
}

void test_compile_negative_numbers() {
    // Negative array index
    auto r1 = compile_search_path("$[-1]");
    assert(r1.ok());

    auto r2 = compile_search_path("$[-10]");
    assert(r2.ok());

    // Negative in slice
    auto r3 = compile_search_path("$[-3:-1]");
    assert(r3.ok());

    // Slice with negative start
    auto r4 = compile_search_path("$[-5:]");
    assert(r4.ok());

    // Slice with negative end
    auto r5 = compile_search_path("$[:-2]");
    assert(r5.ok());

    // Filter with negative number
    auto r6 = compile_search_path("$[?(@.value > -5)]");
    assert(r6.ok());

    auto r7 = compile_search_path("$[?(@.temp < -273)]");
    assert(r7.ok());

    std::cout << "✓ test_compile_negative_numbers passed\n";
}

void test_compile_float_literals() {
    // Simple float
    auto r1 = compile_search_path("$[?(@.price > 9.99)]");
    assert(r1.ok());

    // Float with scientific notation
    auto r2 = compile_search_path("$[?(@.value < 1e10)]");
    assert(r2.ok());

    auto r3 = compile_search_path("$[?(@.value > 1.5e-3)]");
    assert(r3.ok());

    auto r4 = compile_search_path("$[?(@.value < 2.5E+6)]");
    assert(r4.ok());

    // Negative float
    auto r5 = compile_search_path("$[?(@.temp < -273.15)]");
    assert(r5.ok());

    // Very small float
    auto r6 = compile_search_path("$[?(@.epsilon > 1e-10)]");
    assert(r6.ok());

    // Float comparison operators
    auto r7 = compile_search_path("$[?(@.val >= 0.5)]");
    assert(r7.ok());

    auto r8 = compile_search_path("$[?(@.val <= 99.99)]");
    assert(r8.ok());

    auto r9 = compile_search_path("$[?(@.val == 3.14159)]");
    assert(r9.ok());

    auto r10 = compile_search_path("$[?(@.val != 0.0)]");
    assert(r10.ok());

    std::cout << "✓ test_compile_float_literals passed\n";
}

void test_compile_filter_errors() {
    // Test various filter parsing error cases to improve coverage

    // Filter without opening paren
    auto r1 = compile_search_path("$[?@.age > 5]");
    assert(!r1.ok());

    // Filter without @
    auto r2 = compile_search_path("$[?(value > 5)]");
    assert(!r2.ok());

    // Filter with @ but no . or [
    auto r3 = compile_search_path("$[?(@value > 5)]");
    assert(!r3.ok());

    // Filter with @. but empty field
    auto r4 = compile_search_path("$[?(@. > 5)]");
    assert(!r4.ok());

    // Filter with @[ but not a quoted string
    auto r5 = compile_search_path("$[?(@[123] > 5)]");
    assert(!r5.ok());

    // Filter with @['field' but missing ]
    auto r6 = compile_search_path("$[?(@['field' > 5)]");
    assert(!r6.ok());

    // Filter with invalid operator (single =)
    auto r7 = compile_search_path("$[?(@.age = 5)]");
    assert(!r7.ok());

    // Filter with single ! (not followed by =)
    auto r8 = compile_search_path("$[?(@.age !5)]");
    assert(!r8.ok());

    // Filter existence check (now valid)
    auto r9 = compile_search_path("$[?(@.age)]");
    assert(r9.ok());

    // Filter value that's not string or number (identifier)
    auto r10 = compile_search_path("$[?(@.age > xyz)]");
    assert(!r10.ok());

    // Filter with missing value after operator
    auto r10b = compile_search_path("$[?(@.age > )]");
    assert(!r10b.ok());

    // Filter without closing paren
    auto r11 = compile_search_path("$[?(@.age > 5]");
    assert(!r11.ok());

    // Recursive descent with empty field name
    auto r12 = compile_search_path("$..");
    assert(!r12.ok());

    // Empty field after dot
    auto r13 = compile_search_path("$.");
    assert(!r13.ok());

    // Invalid numeric literal in filter (missing digits after '-')
    auto r14 = compile_search_path("$[?(@.val > -)]");
    assert(!r14.ok());

    // Missing digits in numeric literal
    auto r15 = compile_search_path("$[?(@.val > .)]");
    assert(!r15.ok());

    std::cout << "✓ test_compile_filter_errors passed\n";
}

void test_compile_bracket_and_slice_errors() {
    // Missing closing bracket after filter
    auto r1 = compile_search_path("$[?(@.age > 5)");
    assert(!r1.ok());

    // Wildcard missing closing bracket
    auto r2 = compile_search_path("$[*");
    assert(!r2.ok());

    // Quoted field missing closing bracket
    auto r3 = compile_search_path("$['a'");
    assert(!r3.ok());

    // Index missing closing bracket
    auto r4 = compile_search_path("$[0");
    assert(!r4.ok());

    // Slice missing closing bracket
    auto r5 = compile_search_path("$[1:2");
    assert(!r5.ok());

    // Slice with junk after end
    auto r6 = compile_search_path("$[1:foo]");
    assert(!r6.ok());

    // Slice with explicit step but no digits (defaults to 1)
    auto r7 = compile_search_path("$[1:3:]");
    assert(r7.ok());

    // Invalid number token inside brackets
    auto r8 = compile_search_path("$[-x]");
    assert(!r8.ok());

    // Unknown character after root
    auto r9 = compile_search_path("$#");
    assert(!r9.ok());

    std::cout << "✓ test_compile_bracket_and_slice_errors passed\n";
}

void test_simple_field_extraction_flags() {
    auto root_field = compile_search_path("$.name");
    assert(root_field.ok());
    assert(root_field.value.is_simple_field_extraction());

    auto wildcard_field = compile_search_path("$[*].id");
    assert(wildcard_field.ok());
    assert(wildcard_field.value.is_simple_field_extraction());

    auto non_simple = compile_search_path("$.a.b");
    assert(non_simple.ok());
    assert(!non_simple.value.is_simple_field_extraction());

    std::cout << "✓ test_simple_field_extraction_flags passed\n";
}

void test_eval_root() {
    auto doc_result = JsonDocument::from_string("{\"name\": \"Alice\"}");
    assert(doc_result.ok());

    auto path = compile_search_path("$");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_object());

    std::cout << "✓ test_eval_root passed\n";
}

void test_eval_field() {
    auto doc_result = JsonDocument::from_string("{\"name\": \"Alice\", \"age\": 30}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$.name");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_string());
    assert(results[0].as_string() == "Alice");

    std::cout << "✓ test_eval_field passed\n";
}

void test_eval_nested() {
    auto doc_result = JsonDocument::from_string("{\"user\": {\"name\": \"Bob\"}}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$.user.name");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_string() == "Bob");

    std::cout << "✓ test_eval_nested passed\n";
}

void test_eval_wildcard() {
    auto doc_result = JsonDocument::from_string("{\"a\": 1, \"b\": 2, \"c\": 3}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$.*");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 3);

    std::cout << "✓ test_eval_wildcard passed\n";
}

void test_eval_array_index() {
    auto doc_result = JsonDocument::from_string("[1, 2, 3, 4, 5]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$[0]");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_number() == 1.0);

    std::cout << "✓ test_eval_array_index passed\n";
}

void test_eval_array_wildcard() {
    auto doc_result = JsonDocument::from_string("[1, 2, 3]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$[*]");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 3);

    std::cout << "✓ test_eval_array_wildcard passed\n";
}

void test_eval_slice() {
    auto doc_result = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$[2:5]");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 3);
    assert(results[0].as_number() == 2.0);
    assert(results[1].as_number() == 3.0);
    assert(results[2].as_number() == 4.0);

    std::cout << "✓ test_eval_slice passed\n";
}

void test_eval_filter_numeric() {
    auto doc_result = JsonDocument::from_string("[{\"age\": 25}, {\"age\": 35}, {\"age\": 45}]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$[?(@.age > 30)]");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 2);
    assert(results[0].as_object().at("age").as_number() == 35.0);
    assert(results[1].as_object().at("age").as_number() == 45.0);

    std::cout << "✓ test_eval_filter_numeric passed\n";
}

void test_eval_filter_all_operators() {
    // Test all comparison operators: ==, !=, >=, <, <=
    auto doc_result = JsonDocument::from_string("[{\"val\": 10}, {\"val\": 20}, {\"val\": 30}, {\"val\": 40}]");
    assert(doc_result.ok());

    // Equal (==)
    auto path_eq = compile_search_path("$[?(@.val == 20)]");
    assert(path_eq.ok());
    auto results_eq = eval_search_path(doc_result.value, path_eq.value);
    assert(results_eq.size() == 1);
    assert(results_eq[0].as_object().at("val").as_number() == 20.0);

    // Not equal (!=)
    auto path_ne = compile_search_path("$[?(@.val != 20)]");
    assert(path_ne.ok());
    auto results_ne = eval_search_path(doc_result.value, path_ne.value);
    assert(results_ne.size() == 3);

    // Greater equal (>=)
    auto path_ge = compile_search_path("$[?(@.val >= 30)]");
    assert(path_ge.ok());
    auto results_ge = eval_search_path(doc_result.value, path_ge.value);
    assert(results_ge.size() == 2);

    // Less than (<)
    auto path_lt = compile_search_path("$[?(@.val < 25)]");
    assert(path_lt.ok());
    auto results_lt = eval_search_path(doc_result.value, path_lt.value);
    assert(results_lt.size() == 2);

    // Less equal (<=)
    auto path_le = compile_search_path("$[?(@.val <= 20)]");
    assert(path_le.ok());
    auto results_le = eval_search_path(doc_result.value, path_le.value);
    assert(results_le.size() == 2);

    std::cout << "✓ test_eval_filter_all_operators passed\n";
}

void test_eval_filter_string() {
    // Test string comparisons in filters
    auto doc_result = JsonDocument::from_string(
        "[{\"name\": \"Alice\"}, {\"name\": \"Bob\"}, {\"name\": \"Charlie\"}]");
    assert(doc_result.ok());

    // String equal
    auto path_eq = compile_search_path("$[?(@.name == \"Bob\")]");
    assert(path_eq.ok());
    auto results_eq = eval_search_path(doc_result.value, path_eq.value);
    assert(results_eq.size() == 1);
    assert(results_eq[0].as_object().at("name").as_string() == "Bob");

    // String not equal
    auto path_ne = compile_search_path("$[?(@.name != \"Bob\")]");
    assert(path_ne.ok());
    auto results_ne = eval_search_path(doc_result.value, path_ne.value);
    assert(results_ne.size() == 2);

    std::cout << "✓ test_eval_filter_string passed\n";
}

void test_eval_filter_missing_field() {
    // Test filter on missing field - should not match
    auto doc_result = JsonDocument::from_string(
        "[{\"age\": 25}, {\"name\": \"Bob\"}, {\"age\": 35}]");
    assert(doc_result.ok());

    auto path = compile_search_path("$[?(@.age > 20)]");
    assert(path.ok());
    auto results = eval_search_path(doc_result.value, path.value);
    // Only items with 'age' field that passes filter
    assert(results.size() == 2);

    std::cout << "✓ test_eval_filter_missing_field passed\n";
}

void test_eval_recursive() {
    auto doc_result =
        JsonDocument::from_string("{\"a\": {\"name\": \"Alice\"}, \"b\": {\"name\": \"Bob\"}}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$..name");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 2);

    std::cout << "✓ test_eval_recursive passed\n";
}

void test_eval_complex() {
    const char* json_str = R"({
        "users": [
            {"name": "Alice", "age": 30},
            {"name": "Bob", "age": 25},
            {"name": "Charlie", "age": 35}
        ]
    })";

    auto doc_result = JsonDocument::from_string(json_str);
    assert(doc_result.ok());

    // Get users array
    auto path = compile_search_path("$.users[*].name");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 3);

    std::cout << "✓ test_eval_complex passed\n";
}

void test_eval_missing_field() {
    auto doc_result = JsonDocument::from_string("{\"name\": \"Alice\"}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_search_path("$.age");
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.empty());

    std::cout << "✓ test_eval_missing_field passed\n";
}

void test_eval_field_key_counts() {
    std::string target_key = "name";
    std::string target_value = "hit";
    std::vector<size_t> counts = {1, 5, 20, 100};

    for (size_t count : counts) {
        std::string json = make_object_with_keys(count, target_key, target_value);
        auto doc_result = JsonDocument::from_string(json);
        assert(doc_result.ok());

        std::string path_expr = "$['" + target_key + "']";
        auto path = compile_search_path(path_expr);
        assert(path.ok());

        auto results = eval_search_path(doc_result.value, path.value);
        assert(results.size() == 1);
        assert(results[0].is_string());
        assert(results[0].as_string() == target_value);
    }

    std::cout << "✓ test_eval_field_key_counts passed\n";
}

void test_eval_field_key_lengths() {
    std::string key_len1 = "a";
    std::string key_len8 = "field008";
    std::string key_len16 = "field_name_len16";
    std::string key_len32 = "abcdefghijklmnopqrstuvwxyzabcdef";

    assert(key_len1.size() == 1);
    assert(key_len8.size() == 8);
    assert(key_len16.size() == 16);
    assert(key_len32.size() == 32);

    std::vector<std::string> keys = {key_len1, key_len8, key_len16, key_len32};
    for (const auto& key : keys) {
        std::string json = make_object_with_keys(20, key, "value");
        auto doc_result = JsonDocument::from_string(json);
        assert(doc_result.ok());

        std::string path_expr = "$['" + key + "']";
        auto path = compile_search_path(path_expr);
        assert(path.ok());

        auto results = eval_search_path(doc_result.value, path.value);
        assert(results.size() == 1);
        assert(results[0].is_string());
        assert(results[0].as_string() == "value");
    }

    std::cout << "✓ test_eval_field_key_lengths passed\n";
}

void test_eval_field_unicode_key() {
    std::string key = "caf\xc3\xa9"; // UTF-8 for "café"
    std::string json = "{\"" + key + "\":\"espresso\"}";
    auto doc_result = JsonDocument::from_string(json);
    assert(doc_result.ok());

    std::string path_expr = "$['" + key + "']";
    auto path = compile_search_path(path_expr);
    assert(path.ok());

    auto results = eval_search_path(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_string());
    assert(results[0].as_string() == "espresso");

    std::cout << "✓ test_eval_field_unicode_key passed\n";
}

void test_eval_recursive_with_limit() {
    // Test early termination with limit parameter
    const char* json_str = R"({
        "a": {"price": 10},
        "b": {"price": 20},
        "c": {"price": 30},
        "d": {"price": 40},
        "e": {"price": 50}
    })";

    auto doc_result = JsonDocument::from_string(json_str);
    assert(doc_result.ok());

    auto path = compile_search_path("$..price");
    assert(path.ok());

    // Test with limit = 1 (early termination after first result)
    auto results_1 = eval_search_path(doc_result.value, path.value, 1);
    assert(results_1.size() == 1);

    // Test with limit = 3
    auto results_3 = eval_search_path(doc_result.value, path.value, 3);
    assert(results_3.size() == 3);

    // Test with limit = 10 (more than available)
    auto results_10 = eval_search_path(doc_result.value, path.value, 10);
    assert(results_10.size() == 5);

    // Test with limit = 0 (should return empty)
    auto results_0 = eval_search_path(doc_result.value, path.value, 0);
    assert(results_0.empty());

    std::cout << "✓ test_eval_recursive_with_limit passed\n";
}

void test_eval_deep_recursive_with_limit() {
    // Test recursive descent on deeply nested structure
    const char* json_str = R"({
        "level1": {
            "price": 100,
            "level2": {
                "price": 200,
                "level3": {
                    "price": 300,
                    "items": [
                        {"price": 400},
                        {"price": 500}
                    ]
                }
            }
        }
    })";

    auto doc_result = JsonDocument::from_string(json_str);
    assert(doc_result.ok());

    auto path = compile_search_path("$..price");
    assert(path.ok());

    // Get all prices (no limit)
    auto all_results = eval_search_path(doc_result.value, path.value);
    assert(all_results.size() == 5);

    // Get first price only
    auto results_1 = eval_search_path(doc_result.value, path.value, 1);
    assert(results_1.size() == 1);

    // Get first 3 prices
    auto results_3 = eval_search_path(doc_result.value, path.value, 3);
    assert(results_3.size() == 3);

    std::cout << "✓ test_eval_deep_recursive_with_limit passed\n";
}

void test_eval_array_recursive_with_limit() {
    // Test recursive descent with array at root
    const char* json_str = R"([
        {"name": "Alice", "metadata": {"tag": "A"}},
        {"name": "Bob", "metadata": {"tag": "B"}},
        {"name": "Charlie", "metadata": {"tag": "C"}}
    ])";

    auto doc_result = JsonDocument::from_string(json_str);
    assert(doc_result.ok());

    auto path = compile_search_path("$..tag");
    assert(path.ok());

    // Get all tags
    auto all_results = eval_search_path(doc_result.value, path.value);
    assert(all_results.size() == 3);

    // Get first 2 tags with limit
    auto results_2 = eval_search_path(doc_result.value, path.value, 2);
    assert(results_2.size() == 2);

    std::cout << "✓ test_eval_array_recursive_with_limit passed\n";
}

void test_eval_limit_with_cursor() {
    const char* json_str = R"({"users": [{"id": 1}, {"id": 2}, {"id": 3}, {"id": 4}]})";

    auto doc_result = JsonDocument::from_string(json_str);
    assert(doc_result.ok());

    auto path = compile_search_path("$.users[*].id");
    assert(path.ok());

    JsonCursor cursor(doc_result.value.root());
    auto results = eval_search_path(cursor, path.value, 2);
    assert(results.size() == 2);

    auto serialized = serialize_values(results);
    assert(serialized.size() == 2);
    assert(serialized[0] == "1");
    assert(serialized[1] == "2");

    std::cout << "✓ test_eval_limit_with_cursor passed\n";
}

// ============================================================================
// Additional coverage tests for uncovered eval paths
// ============================================================================

void test_eval_filter_on_non_objects() {
    // Filter applied to array containing non-object elements (scalars)
    // This tests the is_object() check in eval_filter (line 50-51)
    auto doc = JsonDocument::from_string("[1, 2, {\"age\": 30}, \"hello\", {\"age\": 40}]");
    assert(doc.ok());
    auto path = compile_search_path("$[?(@.age > 25)]");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    // Only the two objects should be evaluated, scalars skipped
    assert(results.size() == 2);
    std::cout << "✓ test_eval_filter_on_non_objects passed\n";
}

void test_eval_string_filter_on_numeric() {
    // String filter on a field that contains a number
    // Tests the is_string() check in eval_filter (line 80-81)
    auto doc = JsonDocument::from_string(R"([{"name": "Alice"}, {"name": 123}, {"name": "Bob"}])");
    assert(doc.ok());
    auto path = compile_search_path(R"($[?(@.name == "Alice")])");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    // Only string "Alice" matches, numeric 123 is skipped
    assert(results.size() == 1);
    assert(results[0].as_object().at("name").as_string() == "Alice");
    std::cout << "✓ test_eval_string_filter_on_numeric passed\n";
}

void test_eval_negative_index() {
    // Test negative array index (last element)
    auto doc = JsonDocument::from_string("[10, 20, 30, 40, 50]");
    assert(doc.ok());
    auto path = compile_search_path("$[-1]");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_number() == 50.0);

    // Test -2 (second to last)
    auto path2 = compile_search_path("$[-2]");
    assert(path2.ok());
    auto results2 = eval_search_path(doc.value, path2.value);
    assert(results2.size() == 1);
    assert(results2[0].as_number() == 40.0);

    std::cout << "✓ test_eval_negative_index passed\n";
}

void test_eval_collect_null() {
    // Test collecting null values through JSONPath
    // This exercises materialize() for null type (line 16)
    auto doc = JsonDocument::from_string(R"({"a": null, "b": 123})");
    assert(doc.ok());
    auto path = compile_search_path("$.a");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_null());
    std::cout << "✓ test_eval_collect_null passed\n";
}

void test_eval_collect_bool() {
    // Test collecting boolean values through JSONPath
    // This exercises materialize() for bool type (line 18)
    auto doc = JsonDocument::from_string(R"({"active": true, "deleted": false})");
    assert(doc.ok());
    auto path = compile_search_path("$.active");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_bool());
    assert(results[0].as_bool() == true);

    auto path2 = compile_search_path("$.deleted");
    assert(path2.ok());
    auto results2 = eval_search_path(doc.value, path2.value);
    assert(results2.size() == 1);
    assert(results2[0].as_bool() == false);

    std::cout << "✓ test_eval_collect_bool passed\n";
}

void test_eval_collect_array() {
    // Test collecting array values through JSONPath
    // This exercises materialize() for array type (lines 24-30)
    auto doc = JsonDocument::from_string(R"({"items": [1, 2, 3], "name": "test"})");
    assert(doc.ok());
    auto path = compile_search_path("$.items");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_array());
    assert(results[0].as_array().size() == 3);
    assert(results[0].as_array()[0].as_number() == 1.0);
    assert(results[0].as_array()[1].as_number() == 2.0);
    assert(results[0].as_array()[2].as_number() == 3.0);
    std::cout << "✓ test_eval_collect_array passed\n";
}

void test_eval_collect_nested_array() {
    // Test collecting nested arrays through recursive descent
    // Further exercises materialize() for nested structures
    auto doc = JsonDocument::from_string(R"({"data": {"nested": [[1,2], [3,4]]}})");
    assert(doc.ok());
    auto path = compile_search_path("$..nested");
    assert(path.ok());
    auto results = eval_search_path(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_array());
    assert(results[0].as_array().size() == 2);
    std::cout << "✓ test_eval_collect_nested_array passed\n";
}

void test_ndjson_fused_field_extraction() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::vector<std::string> lines = {
        R"({"id": 1, "name": "alpha"})",
        R"({"name": "beta"})",
        R"({"id": 3, "nested": {"id": 99}})",
    };

    auto compiled = compile_search_path("$.id");
    assert(compiled.ok());

    for (const auto& line : lines) {
        auto parsed = parse_json(line, options, &context);
        assert(parsed.ok());
        JsonCursor cursor(&parsed.value);
        auto expected = eval_search_path(cursor, compiled.value);

        std::vector<JsonValue> fused;
        Status status =
            extract_simple_field_matches(line, "id", SimpleFieldMode::RootField, fused, options,
                                         &context);
        assert(status == Status::Ok);

        assert(serialize_values(expected) == serialize_values(fused));
    }

    std::string array_line = R"([{"id": 10}, {"id": 20}, {"other": 30}])";
    auto compiled_wildcard = compile_search_path("$[*].id");
    assert(compiled_wildcard.ok());

    auto parsed = parse_json(array_line, options, &context);
    assert(parsed.ok());
    JsonCursor cursor(&parsed.value);
    auto expected = eval_search_path(cursor, compiled_wildcard.value);

    std::vector<JsonValue> fused;
    Status status = extract_simple_field_matches(array_line, "id",
                                                 SimpleFieldMode::RootWildcardField, fused,
                                                 options, &context);
    assert(status == Status::Ok);

    assert(serialize_values(expected) == serialize_values(fused));

    std::cout << "✓ test_ndjson_fused_field_extraction passed\n";
}

void test_ndjson_fused_root_field_abort_and_invalid_tail() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"({"id": 42, "other": [1, 2, 3], "tail": )"; // invalid tail
    std::vector<JsonValue> matches;
    Status status =
        extract_simple_field_matches(line, "id", SimpleFieldMode::RootField, matches, options,
                                     &context);
    assert(status == Status::Ok);
    assert(matches.size() == 1);
    assert(matches[0].as_number() == 42.0);

    std::cout << "✓ test_ndjson_fused_root_field_abort_and_invalid_tail passed\n";
}

void test_ndjson_fused_root_field_escaped_key_and_value() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = "{\"a\\tb\": \"line1\\nline2\"}";
    std::vector<JsonValue> matches;
    Status status = extract_simple_field_matches(line, "a\tb", SimpleFieldMode::RootField,
                                                 matches, options, &context);
    assert(status == Status::Ok);
    assert(matches.size() == 1);
    assert(matches[0].is_string());
    assert(matches[0].as_string().find('\n') != std::string::npos);

    std::cout << "✓ test_ndjson_fused_root_field_escaped_key_and_value passed\n";
}

void test_ndjson_fused_root_wildcard_array_and_object() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string array_line = R"([{"id": 1}, {"id": 2}, {"other": 3}])";
    std::vector<JsonValue> matches_array;
    Status status_array = extract_simple_field_matches(array_line, "id",
                                                       SimpleFieldMode::RootWildcardField,
                                                       matches_array, options, &context);
    assert(status_array == Status::Ok);
    assert(matches_array.size() == 2);

    std::string obj_line = R"({"a": {"id": 10}, "b": {"id": 20}, "c": {}})";
    std::vector<JsonValue> matches_obj;
    Status status_obj = extract_simple_field_matches(obj_line, "id",
                                                     SimpleFieldMode::RootWildcardField,
                                                     matches_obj, options, &context);
    assert(status_obj == Status::Ok);
    assert(matches_obj.size() == 2);

    std::cout << "✓ test_ndjson_fused_root_wildcard_array_and_object passed\n";
}

void test_ndjson_fused_capture_array_and_object() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"({"items": [1, 2, 3], "obj": {"x": 1}})";

    std::vector<JsonValue> array_matches;
    Status status_array = extract_simple_field_matches(line, "items", SimpleFieldMode::RootField,
                                                       array_matches, options, &context);
    assert(status_array == Status::Ok);
    assert(array_matches.size() == 1);
    assert(array_matches[0].is_array());
    assert(array_matches[0].as_array().size() == 3);

    std::vector<JsonValue> object_matches;
    Status status_obj = extract_simple_field_matches(line, "obj", SimpleFieldMode::RootField,
                                                     object_matches, options, &context);
    assert(status_obj == Status::Ok);
    assert(object_matches.size() == 1);
    assert(object_matches[0].is_object());
    assert(object_matches[0].as_object().at("x").as_number() == 1.0);

    std::cout << "✓ test_ndjson_fused_capture_array_and_object passed\n";
}

void test_ndjson_fused_large_uint_value() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"({"id": 9223372036854775808})";
    std::vector<JsonValue> matches;
    Status status =
        extract_simple_field_matches(line, "id", SimpleFieldMode::RootField, matches, options,
                                     &context);
    assert(status == Status::Ok);
    assert(matches.size() == 1);
    assert(matches[0].is_number());
    assert(matches[0].as_number() > 9.22e18);

    std::cout << "✓ test_ndjson_fused_large_uint_value passed\n";
}

void test_ndjson_fused_invalid_json() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = "{bad json";
    std::vector<JsonValue> matches;
    Status status =
        extract_simple_field_matches(line, "id", SimpleFieldMode::RootField, matches, options,
                                     &context);
    assert(status == Status::ParseError);
    assert(matches.empty());

    std::cout << "✓ test_ndjson_fused_invalid_json passed\n";
}

void test_ndjson_fused_root_field_non_object() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"([{"id": 1}, {"id": 2}])";
    std::vector<JsonValue> matches;
    Status status =
        extract_simple_field_matches(line, "id", SimpleFieldMode::RootField, matches, options,
                                     &context);
    assert(status == Status::Ok);
    assert(matches.empty());

    std::cout << "✓ test_ndjson_fused_root_field_non_object passed\n";
}

void test_ndjson_fused_root_wildcard_ignores_nested() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"([{"id": 1, "nested": {"id": 99}}])";
    std::vector<JsonValue> matches;
    Status status = extract_simple_field_matches(line, "id",
                                                 SimpleFieldMode::RootWildcardField, matches,
                                                 options, &context);
    assert(status == Status::Ok);
    assert(matches.size() == 1);
    assert(matches[0].as_number() == 1.0);

    std::cout << "✓ test_ndjson_fused_root_wildcard_ignores_nested passed\n";
}

void test_ndjson_fused_capture_deep_nested_container() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"({"deep": {"a": [ {"b": 1}, {"b": 2} ] }})";
    std::vector<JsonValue> matches;
    Status status =
        extract_simple_field_matches(line, "deep", SimpleFieldMode::RootField, matches, options,
                                     &context);
    assert(status == Status::Ok);
    assert(matches.size() == 1);
    assert(matches[0].is_object());
    assert(matches[0].as_object().count("a") == 1);

    std::cout << "✓ test_ndjson_fused_capture_deep_nested_container passed\n";
}

void test_ndjson_fused_primitive_values() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"({"n": null, "b": true, "d": 1.25, "s": "plain"})";

    std::vector<JsonValue> n_matches;
    Status n_status =
        extract_simple_field_matches(line, "n", SimpleFieldMode::RootField, n_matches, options,
                                     &context);
    assert(n_status == Status::Ok);
    assert(n_matches.size() == 1);
    assert(n_matches[0].is_null());

    std::vector<JsonValue> b_matches;
    Status b_status =
        extract_simple_field_matches(line, "b", SimpleFieldMode::RootField, b_matches, options,
                                     &context);
    assert(b_status == Status::Ok);
    assert(b_matches.size() == 1);
    assert(b_matches[0].is_bool());

    std::vector<JsonValue> d_matches;
    Status d_status =
        extract_simple_field_matches(line, "d", SimpleFieldMode::RootField, d_matches, options,
                                     &context);
    assert(d_status == Status::Ok);
    assert(d_matches.size() == 1);
    assert(d_matches[0].is_number());

    std::vector<JsonValue> s_matches;
    Status s_status =
        extract_simple_field_matches(line, "s", SimpleFieldMode::RootField, s_matches, options,
                                     &context);
    assert(s_status == Status::Ok);
    assert(s_matches.size() == 1);
    assert(s_matches[0].is_string());

    std::cout << "✓ test_ndjson_fused_primitive_values passed\n";
}

void test_ndjson_fused_object_with_escaped_key() {
    ParseSaxOptions options;
    ParseSaxContext context;

    std::string line = R"({"obj": {"a\\tb": 1}})";
    std::vector<JsonValue> matches;
    Status status =
        extract_simple_field_matches(line, "obj", SimpleFieldMode::RootField, matches, options,
                                     &context);
    assert(status == Status::Ok);
    assert(matches.size() == 1);
    assert(matches[0].is_object());
    const auto& obj = matches[0].as_object();
    bool has_key = (obj.count("a\tb") == 1) || (obj.count("a\\tb") == 1);
    assert(has_key);

    std::cout << "✓ test_ndjson_fused_object_with_escaped_key passed\n";
}

int main() {
    std::cout << "Running JSONPath tests...\n\n";
    // Compilation tests
    test_compile_root();
    test_compile_field();
    test_compile_nested();
    test_compile_wildcard();
    test_compile_index();
    test_compile_recursive();
    test_compile_slice();
    test_compile_filter();
    test_compile_errors();
    test_compile_quoted_strings();
    test_compile_quoted_escape_sequences();
    test_compile_negative_numbers();
    test_compile_float_literals();
    test_compile_filter_errors();
    test_compile_bracket_and_slice_errors();
    test_simple_field_extraction_flags();

    // Evaluation tests
    test_eval_root();
    test_eval_field();
    test_eval_nested();
    test_eval_wildcard();
    test_eval_array_index();
    test_eval_array_wildcard();
    test_eval_slice();
    test_eval_filter_numeric();
    test_eval_filter_all_operators();
    test_eval_filter_string();
    test_eval_filter_missing_field();
    test_eval_recursive();
    test_eval_complex();
    test_eval_missing_field();
    test_eval_field_key_counts();
    test_eval_field_key_lengths();
    test_eval_field_unicode_key();

    // Limit/early termination tests
    test_eval_recursive_with_limit();
    test_eval_deep_recursive_with_limit();
    test_eval_array_recursive_with_limit();
    test_eval_limit_with_cursor();

    // Additional coverage tests
    test_eval_filter_on_non_objects();
    test_eval_string_filter_on_numeric();
    test_eval_negative_index();
    test_eval_collect_null();
    test_eval_collect_bool();
    test_eval_collect_array();
    test_eval_collect_nested_array();
    test_ndjson_fused_field_extraction();
    test_ndjson_fused_root_field_abort_and_invalid_tail();
    test_ndjson_fused_root_field_escaped_key_and_value();
    test_ndjson_fused_root_wildcard_array_and_object();
    test_ndjson_fused_capture_array_and_object();
    test_ndjson_fused_large_uint_value();
    test_ndjson_fused_invalid_json();
    test_ndjson_fused_root_field_non_object();
    test_ndjson_fused_root_wildcard_ignores_nested();
    test_ndjson_fused_capture_deep_nested_container();
    test_ndjson_fused_primitive_values();
    test_ndjson_fused_object_with_escaped_key();

    std::cout << "\n✅ All JSONPath tests passed!\n";
    return 0;
}

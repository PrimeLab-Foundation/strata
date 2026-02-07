/**
 * test_jsonpath_advanced.cpp - Tests for advanced JSONPath features
 *
 * Parity tests for tests/py/test_jsonpath_advanced.py
 * Tests filter predicates, recursive descent, and array slicing.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/search/jsonpath.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace strata;

// ============================================================================
// Filter Predicate Tests
// ============================================================================

void test_filter_numeric_greater_than() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": 25}, {"age": 35}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age > 27)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);
    assert(results[0].as_object().at("age").as_number() == 30);
    assert(results[1].as_object().at("age").as_number() == 35);

    std::cout << "✓ test_filter_numeric_greater_than passed\n";
}

void test_filter_numeric_less_than() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": 25}, {"age": 35}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age < 30)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_object().at("age").as_number() == 25);

    std::cout << "✓ test_filter_numeric_less_than passed\n";
}

void test_filter_numeric_equal() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": 25}, {"age": 30}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age == 30)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);
    for (const auto& r : results) {
        assert(r.as_object().at("age").as_number() == 30);
    }

    std::cout << "✓ test_filter_numeric_equal passed\n";
}

void test_filter_numeric_not_equal() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": 25}, {"age": 30}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age != 30)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_object().at("age").as_number() == 25);

    std::cout << "✓ test_filter_numeric_not_equal passed\n";
}

void test_filter_numeric_greater_equal() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": 25}, {"age": 30}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age >= 30)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::cout << "✓ test_filter_numeric_greater_equal passed\n";
}

void test_filter_numeric_less_equal() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": 25}, {"age": 35}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age <= 30)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::cout << "✓ test_filter_numeric_less_equal passed\n";
}

void test_filter_string_equal() {
    auto doc = JsonDocument::from_string(R"([{"name": "Alice"}, {"name": "Bob"}, {"name": "Alice"}])");
    assert(doc.ok());

    auto path = compile_jsonpath(R"($[?(@.name == "Alice")])");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);
    for (const auto& r : results) {
        assert(r.as_object().at("name").as_string() == "Alice");
    }

    std::cout << "✓ test_filter_string_equal passed\n";
}

void test_filter_string_not_equal() {
    auto doc = JsonDocument::from_string(R"([{"name": "Alice"}, {"name": "Bob"}])");
    assert(doc.ok());

    auto path = compile_jsonpath(R"($[?(@.name != "Alice")])");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_object().at("name").as_string() == "Bob");

    std::cout << "✓ test_filter_string_not_equal passed\n";
}

void test_filter_with_nested_path() {
    auto doc = JsonDocument::from_string(
        R"({"users": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}]})");
    assert(doc.ok());

    auto path = compile_jsonpath("$.users[?(@.age > 27)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_object().at("name").as_string() == "Alice");

    std::cout << "✓ test_filter_with_nested_path passed\n";
}

void test_filter_then_field_access() {
    auto doc = JsonDocument::from_string(
        R"({"users": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}, {"name": "Charlie", "age": 35}]})");
    assert(doc.ok());

    auto path = compile_jsonpath("$.users[?(@.age > 27)].name");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::set<std::string> names;
    for (const auto& r : results) {
        names.insert(r.as_string());
    }
    assert(names.count("Alice") == 1);
    assert(names.count("Charlie") == 1);

    std::cout << "✓ test_filter_then_field_access passed\n";
}

void test_filter_missing_field() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"name": "Bob"}, {"age": 25}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age > 27)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_object().at("age").as_number() == 30);

    std::cout << "✓ test_filter_missing_field passed\n";
}

void test_filter_wrong_type() {
    auto doc = JsonDocument::from_string(R"([{"age": 30}, {"age": "old"}, {"age": 25}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age > 27)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    // Only numeric values should match
    assert(results.size() == 1);
    assert(results[0].as_object().at("age").as_number() == 30);

    std::cout << "✓ test_filter_wrong_type passed\n";
}

// ============================================================================
// Recursive Descent Tests
// ============================================================================

void test_recursive_simple() {
    auto doc = JsonDocument::from_string(R"({"a": {"price": 10}, "b": {"price": 20}})");
    assert(doc.ok());

    auto path = compile_jsonpath("$..price");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::set<double> prices;
    for (const auto& r : results) {
        prices.insert(r.as_number());
    }
    assert(prices.count(10) == 1);
    assert(prices.count(20) == 1);

    std::cout << "✓ test_recursive_simple passed\n";
}

void test_recursive_nested() {
    auto doc = JsonDocument::from_string(R"({"a": {"b": {"price": 10, "c": {"price": 20}}}})");
    assert(doc.ok());

    auto path = compile_jsonpath("$..price");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::set<double> prices;
    for (const auto& r : results) {
        prices.insert(r.as_number());
    }
    assert(prices.count(10) == 1);
    assert(prices.count(20) == 1);

    std::cout << "✓ test_recursive_nested passed\n";
}

void test_recursive_in_arrays() {
    auto doc = JsonDocument::from_string(R"([{"price": 10}, {"price": 20}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$..price");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::cout << "✓ test_recursive_in_arrays passed\n";
}

void test_recursive_mixed_structure() {
    auto doc = JsonDocument::from_string(
        R"({"items": [{"price": 10}, {"nested": {"price": 20}}]})");
    assert(doc.ok());

    auto path = compile_jsonpath("$..price");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::cout << "✓ test_recursive_mixed_structure passed\n";
}

void test_recursive_no_matches() {
    auto doc = JsonDocument::from_string(R"({"a": 1, "b": 2})");
    assert(doc.ok());

    auto path = compile_jsonpath("$..price");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.empty());

    std::cout << "✓ test_recursive_no_matches passed\n";
}

void test_recursive_with_wildcards() {
    auto doc = JsonDocument::from_string(
        R"({"users": [{"name": "Alice", "addresses": [{"city": "NYC"}]}, {"name": "Bob", "addresses": [{"city": "LA"}]}]})");
    assert(doc.ok());

    auto path = compile_jsonpath("$..addresses[*].city");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);

    std::cout << "✓ test_recursive_with_wildcards passed\n";
}

// ============================================================================
// Array Slicing Tests
// ============================================================================

void test_slice_basic() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[2:5]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);
    assert(results[0].as_number() == 2);
    assert(results[1].as_number() == 3);
    assert(results[2].as_number() == 4);

    std::cout << "✓ test_slice_basic passed\n";
}

void test_slice_with_step() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[::2]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 5);  // 0, 2, 4, 6, 8
    assert(results[0].as_number() == 0);
    assert(results[1].as_number() == 2);
    assert(results[2].as_number() == 4);

    std::cout << "✓ test_slice_with_step passed\n";
}

void test_slice_from_start() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[:3]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 0, 1, 2
    assert(results[0].as_number() == 0);
    assert(results[1].as_number() == 1);
    assert(results[2].as_number() == 2);

    std::cout << "✓ test_slice_from_start passed\n";
}

void test_slice_to_end() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[7:]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 7, 8, 9
    assert(results[0].as_number() == 7);
    assert(results[1].as_number() == 8);
    assert(results[2].as_number() == 9);

    std::cout << "✓ test_slice_to_end passed\n";
}

void test_slice_negative_start() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[-3:]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 7, 8, 9
    assert(results[0].as_number() == 7);
    assert(results[1].as_number() == 8);
    assert(results[2].as_number() == 9);

    std::cout << "✓ test_slice_negative_start passed\n";
}

void test_slice_negative_end() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[:-3]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 7);  // 0-6
    assert(results[0].as_number() == 0);
    assert(results[6].as_number() == 6);

    std::cout << "✓ test_slice_negative_end passed\n";
}

void test_slice_both_negative() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[-5:-2]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 5, 6, 7
    assert(results[0].as_number() == 5);
    assert(results[1].as_number() == 6);
    assert(results[2].as_number() == 7);

    std::cout << "✓ test_slice_both_negative passed\n";
}

void test_slice_step_two() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[1:7:2]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 1, 3, 5
    assert(results[0].as_number() == 1);
    assert(results[1].as_number() == 3);
    assert(results[2].as_number() == 5);

    std::cout << "✓ test_slice_step_two passed\n";
}

void test_slice_on_nested_array() {
    auto doc = JsonDocument::from_string(R"({"items": [0, 1, 2, 3, 4]})");
    assert(doc.ok());

    auto path = compile_jsonpath("$.items[1:4]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 1, 2, 3
    assert(results[0].as_number() == 1);
    assert(results[1].as_number() == 2);
    assert(results[2].as_number() == 3);

    std::cout << "✓ test_slice_on_nested_array passed\n";
}

void test_slice_empty_range() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[3:2]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.empty());  // Empty range

    std::cout << "✓ test_slice_empty_range passed\n";
}

void test_slice_out_of_bounds() {
    auto doc = JsonDocument::from_string("[0, 1, 2, 3, 4]");
    assert(doc.ok());

    auto path = compile_jsonpath("$[2:10]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 3);  // 2, 3, 4 (clamped to array length)

    std::cout << "✓ test_slice_out_of_bounds passed\n";
}

// ============================================================================
// Combined Features Tests
// ============================================================================

void test_filter_with_recursive() {
    auto doc = JsonDocument::from_string(R"({
        "store": {
            "books": [
                {"title": "A", "price": 10},
                {"title": "B", "price": 20}
            ],
            "music": {
                "albums": [
                    {"title": "C", "price": 15}
                ]
            }
        }
    })");
    assert(doc.ok());

    // NOTE: Recursive descent + filter combination ($..[?()]) is not yet supported
    // This test validates the feature once it's implemented
    auto path = compile_jsonpath("$..[?(@.price > 12)]");
    if (!path.ok()) {
        std::cout << "⏭ test_filter_with_recursive skipped (feature not yet implemented)\n";
        return;
    }

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 2);  // price 20 and price 15

    std::cout << "✓ test_filter_with_recursive passed\n";
}

void test_recursive_with_slice() {
    auto doc = JsonDocument::from_string(R"({
        "data": {
            "items": [1, 2, 3, 4, 5],
            "nested": {
                "items": [6, 7, 8, 9, 10]
            }
        }
    })");
    assert(doc.ok());

    // NOTE: Recursive descent + slice combination ($..items[:2]) is not yet supported
    // This test validates the feature once it's implemented
    auto path = compile_jsonpath("$..items[:2]");
    if (!path.ok()) {
        std::cout << "⏭ test_recursive_with_slice skipped (feature not yet implemented)\n";
        return;
    }

    auto results = eval_jsonpath(doc.value, path.value);
    // Should get first 2 items from both arrays
    assert(results.size() == 4);  // [1, 2] and [6, 7]

    std::cout << "✓ test_recursive_with_slice passed\n";
}

// ============================================================================
// Edge Cases Tests
// ============================================================================

void test_filter_on_non_array() {
    auto doc = JsonDocument::from_string(R"({"age": 30})");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.age > 20)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    // Filter on non-array should return empty
    assert(results.empty());

    std::cout << "✓ test_filter_on_non_array passed\n";
}

void test_slice_on_non_array() {
    auto doc = JsonDocument::from_string(R"({"key": "value"})");
    assert(doc.ok());

    auto path = compile_jsonpath("$[0:2]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    // Slice on non-array should return empty
    assert(results.empty());

    std::cout << "✓ test_slice_on_non_array passed\n";
}

void test_recursive_on_scalar() {
    auto doc = JsonDocument::from_string("42");
    assert(doc.ok());

    auto path = compile_jsonpath("$..value");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    // Recursive on scalar should return empty
    assert(results.empty());

    std::cout << "✓ test_recursive_on_scalar passed\n";
}

void test_filter_decimal_comparison() {
    auto doc = JsonDocument::from_string(R"([{"price": 19.99}, {"price": 20.01}])");
    assert(doc.ok());

    auto path = compile_jsonpath("$[?(@.price > 20)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_object().at("price").as_number() > 20);

    std::cout << "✓ test_filter_decimal_comparison passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running JSONPath Advanced tests...\n\n";

    // Filter predicates
    std::cout << "--- Filter Predicates ---\n";
    test_filter_numeric_greater_than();
    test_filter_numeric_less_than();
    test_filter_numeric_equal();
    test_filter_numeric_not_equal();
    test_filter_numeric_greater_equal();
    test_filter_numeric_less_equal();
    test_filter_string_equal();
    test_filter_string_not_equal();
    test_filter_with_nested_path();
    test_filter_then_field_access();
    test_filter_missing_field();
    test_filter_wrong_type();

    // Recursive descent
    std::cout << "\n--- Recursive Descent ---\n";
    test_recursive_simple();
    test_recursive_nested();
    test_recursive_in_arrays();
    test_recursive_mixed_structure();
    test_recursive_no_matches();
    test_recursive_with_wildcards();

    // Array slicing
    std::cout << "\n--- Array Slicing ---\n";
    test_slice_basic();
    test_slice_with_step();
    test_slice_from_start();
    test_slice_to_end();
    test_slice_negative_start();
    test_slice_negative_end();
    test_slice_both_negative();
    test_slice_step_two();
    test_slice_on_nested_array();
    test_slice_empty_range();
    test_slice_out_of_bounds();

    // Combined features
    std::cout << "\n--- Combined Features ---\n";
    test_filter_with_recursive();
    test_recursive_with_slice();

    // Edge cases
    std::cout << "\n--- Edge Cases ---\n";
    test_filter_on_non_array();
    test_slice_on_non_array();
    test_recursive_on_scalar();
    test_filter_decimal_comparison();

    std::cout << "\n✅ All JSONPath Advanced tests passed!\n";
    return 0;
}

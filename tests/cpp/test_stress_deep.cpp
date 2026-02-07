/**
 * test_stress_deep.cpp - Stress tests for deeply nested JSON.
 *
 * Mirrors Python tests/py/test_stress_deep.py (5k nesting).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace strata;

// C++ parser now uses iterative stack-based parsing, enabling deep nesting.
// Match Python tests depth of 5000.
constexpr int kDeepListDepth = 5000;
constexpr int kDeepDictDepth = 5000;

void test_deep_list_5000() {
    std::string json;
    for (int i = 0; i < kDeepListDepth; ++i)
        json += "[";
    for (int i = 0; i < kDeepListDepth; ++i)
        json += "]";

    auto result = parse_json(json);
    assert(result.ok());
    const auto* current = &result.value;
    for (int i = 0; i < kDeepListDepth - 1; ++i) {
        assert(current->is_array());
        assert(current->as_array().size() == 1);
        current = &current->as_array()[0];
    }
    assert(current->is_array() && current->as_array().empty());

    std::string back = serialize_json(result.value);
    auto result2 = parse_json(back);
    assert(result2.ok());

    std::cout << "✓ test_deep_list_5000 passed\n";
}

void test_deep_dict_5000() {
    std::string json = "{\"leaf\":1}";
    for (int i = 0; i < kDeepDictDepth - 1; ++i) {
        json = "{\"a\":" + json + "}";
    }

    auto result = parse_json(json);
    assert(result.ok());
    const auto* current = &result.value;
    for (int i = 0; i < kDeepDictDepth - 1; ++i) {
        assert(current->is_object());
        assert(current->as_object().count("a") == 1);
        current = &current->as_object().at("a");
    }
    assert(current->is_object());
    assert(current->as_object().at("leaf").as_number() == 1.0);

    std::cout << "✓ test_deep_dict_5000 passed\n";
}

int main() {
    std::cout << "Running deep-nesting stress tests (iterative stack-based; depth=5000)...\n\n";
    test_deep_list_5000();
    test_deep_dict_5000();
    std::cout << "\n✅ All deep-nesting stress tests passed!\n";
    return 0;
}

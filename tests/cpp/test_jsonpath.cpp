/**
 * test_jsonpath.cpp - Tests for JSONPath queries
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/search/jsonpath.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace strata;

void test_compile_root() {
    auto result = compile_jsonpath("$");
    assert(result.ok());
    std::cout << "✓ test_compile_root passed\n";
}

void test_compile_field() {
    auto result = compile_jsonpath("$.name");
    assert(result.ok());
    std::cout << "✓ test_compile_field passed\n";
}

void test_compile_nested() {
    auto result = compile_jsonpath("$.user.name");
    assert(result.ok());
    std::cout << "✓ test_compile_nested passed\n";
}

void test_compile_wildcard() {
    auto result = compile_jsonpath("$.*");
    assert(result.ok());
    std::cout << "✓ test_compile_wildcard passed\n";
}

void test_compile_index() {
    auto result = compile_jsonpath("$[0]");
    assert(result.ok());
    std::cout << "✓ test_compile_index passed\n";
}

void test_compile_recursive() {
    auto result = compile_jsonpath("$..name");
    assert(result.ok());
    std::cout << "✓ test_compile_recursive passed\n";
}

void test_compile_slice() {
    auto result = compile_jsonpath("$[0:5]");
    assert(result.ok());
    std::cout << "✓ test_compile_slice passed\n";
}

void test_compile_filter() {
    auto result = compile_jsonpath("$[?(@.age > 30)]");
    assert(result.ok());
    std::cout << "✓ test_compile_filter passed\n";
}

void test_compile_errors() {
    // Invalid path
    auto result_bad1 = compile_jsonpath("invalid");
    assert(!result_bad1.ok());

    // Empty
    auto result_bad2 = compile_jsonpath("");
    assert(!result_bad2.ok());

    std::cout << "✓ test_compile_errors passed\n";
}

void test_eval_root() {
    auto doc_result = JsonDocument::from_string("{\"name\": \"Alice\"}");
    assert(doc_result.ok());

    auto path = compile_jsonpath("$");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_object());

    std::cout << "✓ test_eval_root passed\n";
}

void test_eval_field() {
    auto doc_result = JsonDocument::from_string("{\"name\": \"Alice\", \"age\": 30}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$.name");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].is_string());
    assert(results[0].as_string() == "Alice");

    std::cout << "✓ test_eval_field passed\n";
}

void test_eval_nested() {
    auto doc_result = JsonDocument::from_string("{\"user\": {\"name\": \"Bob\"}}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$.user.name");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_string() == "Bob");

    std::cout << "✓ test_eval_nested passed\n";
}

void test_eval_wildcard() {
    auto doc_result = JsonDocument::from_string("{\"a\": 1, \"b\": 2, \"c\": 3}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$.*");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 3);

    std::cout << "✓ test_eval_wildcard passed\n";
}

void test_eval_array_index() {
    auto doc_result = JsonDocument::from_string("[1, 2, 3, 4, 5]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$[0]");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 1);
    assert(results[0].as_number() == 1.0);

    std::cout << "✓ test_eval_array_index passed\n";
}

void test_eval_array_wildcard() {
    auto doc_result = JsonDocument::from_string("[1, 2, 3]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$[*]");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 3);

    std::cout << "✓ test_eval_array_wildcard passed\n";
}

void test_eval_slice() {
    auto doc_result = JsonDocument::from_string("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$[2:5]");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
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

    auto path = compile_jsonpath("$[?(@.age > 30)]");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 2);
    assert(results[0].as_object().at("age").as_number() == 35.0);
    assert(results[1].as_object().at("age").as_number() == 45.0);

    std::cout << "✓ test_eval_filter_numeric passed\n";
}

void test_eval_recursive() {
    auto doc_result =
        JsonDocument::from_string("{\"a\": {\"name\": \"Alice\"}, \"b\": {\"name\": \"Bob\"}}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$..name");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
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
    auto path = compile_jsonpath("$.users[*].name");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.size() == 3);

    std::cout << "✓ test_eval_complex passed\n";
}

void test_eval_missing_field() {
    auto doc_result = JsonDocument::from_string("{\"name\": \"Alice\"}");
    assert(doc_result.ok());

    assert(doc_result.ok());

    auto path = compile_jsonpath("$.age");
    assert(path.ok());

    auto results = eval_jsonpath(doc_result.value, path.value);
    assert(results.empty());

    std::cout << "✓ test_eval_missing_field passed\n";
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

    // Evaluation tests
    test_eval_root();
    test_eval_field();
    test_eval_nested();
    test_eval_wildcard();
    test_eval_array_index();
    test_eval_array_wildcard();
    test_eval_slice();
    test_eval_filter_numeric();
    test_eval_recursive();
    test_eval_complex();
    test_eval_missing_field();

    std::cout << "\n✅ All JSONPath tests passed!\n";
    return 0;
}

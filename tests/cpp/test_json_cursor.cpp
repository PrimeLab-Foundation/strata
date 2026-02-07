/**
 * test_json_cursor.cpp - Tests for JSON cursor navigation and value access
 *
 * Tests JsonCursor functionality including:
 * - Basic navigation (fields, array indices)
 * - Type predicates
 * - Value accessors (get_bool, get_int64, get_uint64, get_double, get_string)
 * - Type mismatch error handling
 * - Throwing methods (*_or_throw)
 * - Edge cases
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_document.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace strata;

// ============================================================================
// Basic navigation tests
// ============================================================================

void test_basic_navigation() {
    // Build a small JSON object manually:
    // {
    //   "name": "Alice",
    //   "age":  30,
    //   "tags": ["dev", "json"]
    // }
    JsonValue root(JsonValue::Object{});
    auto& obj = root.as_object();
    obj["name"] = JsonValue(std::string("Alice"));
    obj["age"] = JsonValue(JsonValue::Number(30));
    obj["tags"] = JsonValue(JsonValue::Array{
        JsonValue(std::string("dev")),
        JsonValue(std::string("json")),
    });

    JsonCursor c(&root);

    // Object checks
    assert(c.is_object());

    // Field "name"
    auto name_res = c.get_field("name");
    assert(name_res.ok());
    auto name_str = name_res.value.get_string();
    assert(name_str.ok());
    assert(name_str.value == "Alice");

    // Field "age"
    auto age_res = c.get_field("age");
    assert(age_res.ok());
    auto age_num = age_res.value.get_int64();
    assert(age_num.ok());
    assert(age_num.value == 30);

    // Field "tags[1]"
    auto tags_res = c.get_field("tags");
    assert(tags_res.ok());
    auto tag1 = tags_res.value.get_at(1);
    assert(tag1.ok());
    auto tag1_str = tag1.value.get_string();
    assert(tag1_str.ok());
    assert(tag1_str.value == "json");

    std::cout << "✓ test_basic_navigation passed\n";
}

// ============================================================================
// Type mismatch error tests
// ============================================================================

void test_type_mismatch_errors() {
    auto doc = JsonDocument::from_string("{\"name\": \"Alice\", \"age\": 30, \"active\": true}");
    assert(doc.ok());

    auto root = doc.value.root();
    auto name_cursor = root.get_field("name");
    assert(name_cursor.ok());

    // Try to get bool from string - should fail
    auto bool_result = name_cursor.value.get_bool();
    assert(!bool_result.ok());
    assert(bool_result.status == Status::TypeMismatch);

    // Try to get int from string - should fail
    auto int_result = name_cursor.value.get_int64();
    assert(!int_result.ok());
    assert(int_result.status == Status::TypeMismatch);

    // Try to get uint64 from string - should fail
    auto uint_result = name_cursor.value.get_uint64();
    assert(!uint_result.ok());
    assert(uint_result.status == Status::TypeMismatch);

    // Try to get double from string - should fail
    auto double_result = name_cursor.value.get_double();
    assert(!double_result.ok());
    assert(double_result.status == Status::TypeMismatch);

    // Try to get string from number - should fail
    auto age_cursor = root.get_field("age");
    assert(age_cursor.ok());
    auto str_result = age_cursor.value.get_string();
    assert(!str_result.ok());
    assert(str_result.status == Status::TypeMismatch);

    // Try to get array element from object - should fail
    auto at_result = root.get_at(0);
    assert(!at_result.ok());
    assert(at_result.status == Status::TypeMismatch);

    // Try to get field from array
    auto arr_doc = JsonDocument::from_string("[1, 2, 3]");
    assert(arr_doc.ok());
    auto arr = arr_doc.value.root();
    auto field_result = arr.get_field("name");
    assert(!field_result.ok());
    assert(field_result.status == Status::TypeMismatch);

    std::cout << "✓ test_type_mismatch_errors passed\n";
}

// ============================================================================
// Throwing methods tests
// ============================================================================

void test_throwing_methods() {
    auto doc = JsonDocument::from_string("{\"str\": \"hello\", \"num\": 42, \"flag\": true}");
    assert(doc.ok());
    auto root = doc.value.root();

    // get_bool_or_throw on non-bool
    bool caught = false;
    try {
        root.get_field("str").value.get_bool_or_throw();
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);

    // get_int on non-number (string)
    caught = false;
    try {
        int64_t val = root.get_field("str").value.get_int();
        (void)val;
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);

    // get_float on non-number (string)
    caught = false;
    try {
        double val = root.get_field("str").value.get_float();
        (void)val;
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);

    // get_str on non-string (number)
    caught = false;
    try {
        auto val = root.get_field("num").value.get_str();
        (void)val;
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);

    // field() on missing field (KeyNotFound -> std::out_of_range)
    caught = false;
    try {
        root.field("nonexistent");
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);

    // field() on non-object (TypeMismatch -> std::runtime_error)
    auto arr_doc = JsonDocument::from_string("[1, 2, 3]");
    assert(arr_doc.ok());
    caught = false;
    try {
        arr_doc.value.root().field("test");
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);

    // at() out of bounds (IndexOutOfBounds -> std::out_of_range)
    caught = false;
    try {
        arr_doc.value.root().at(10);
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);

    // at() on non-array (TypeMismatch -> std::runtime_error)
    caught = false;
    try {
        root.at(0);
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);

    std::cout << "✓ test_throwing_methods passed\n";
}

// ============================================================================
// Successful throwing method calls
// ============================================================================

void test_throwing_methods_success() {
    auto doc = JsonDocument::from_string("{\"str\": \"hello\", \"num\": 42, \"flag\": true}");
    assert(doc.ok());
    auto root = doc.value.root();

    // get_bool_or_throw on bool
    bool flag = root.get_field("flag").value.get_bool_or_throw();
    assert(flag == true);

    // get_int on number
    int64_t num = root.get_field("num").value.get_int();
    assert(num == 42);

    // get_float on number
    double num_f = root.get_field("num").value.get_float();
    assert(num_f == 42.0);

    // get_str on string
    std::string str = root.get_field("str").value.get_str();
    assert(str == "hello");

    // field() success
    auto str_cursor = root.field("str");
    assert(str_cursor.is_string());

    // at() success
    auto arr_doc = JsonDocument::from_string("[1, 2, 3]");
    assert(arr_doc.ok());
    auto elem = arr_doc.value.root().at(1);
    assert(elem.is_number());

    std::cout << "✓ test_throwing_methods_success passed\n";
}

// ============================================================================
// uint64 edge cases
// ============================================================================

void test_uint64_edge_cases() {
    auto doc = JsonDocument::from_string("{\"pos\": 42, \"neg\": -5, \"frac\": 3.5, \"big\": 1e30, \"zero\": 0}");
    assert(doc.ok());
    auto root = doc.value.root();

    // Positive integer - should work
    auto pos = root.get_field("pos").value.get_uint64();
    assert(pos.ok());
    assert(pos.value == 42);

    // Zero - should work
    auto zero = root.get_field("zero").value.get_uint64();
    assert(zero.ok());
    assert(zero.value == 0);

    // Negative - should fail
    auto neg = root.get_field("neg").value.get_uint64();
    assert(!neg.ok());
    assert(neg.status == Status::TypeMismatch);

    // Fractional - should fail
    auto frac = root.get_field("frac").value.get_uint64();
    assert(!frac.ok());
    assert(frac.status == Status::TypeMismatch);

    // Too large - should fail
    auto big = root.get_field("big").value.get_uint64();
    assert(!big.ok());
    assert(big.status == Status::TypeMismatch);

    // NaN - should fail (tests isfinite check)
    JsonValue nan_val(JsonValue::Number(std::nan("")));
    JsonCursor nan_cursor(&nan_val);
    auto nan_result = nan_cursor.get_uint64();
    assert(!nan_result.ok());
    assert(nan_result.status == Status::TypeMismatch);

    // Infinity - should fail (tests isfinite check)
    double inf_num = std::numeric_limits<double>::infinity();
    JsonValue inf_val{JsonValue::Number(inf_num)};
    JsonCursor inf_cursor(&inf_val);
    auto inf_result = inf_cursor.get_uint64();
    assert(!inf_result.ok());
    assert(inf_result.status == Status::TypeMismatch);

    std::cout << "✓ test_uint64_edge_cases passed\n";
}

// ============================================================================
// int64 edge cases
// ============================================================================

void test_int64_edge_cases() {
    auto doc = JsonDocument::from_string("{\"pos\": 42, \"neg\": -42, \"frac\": 3.5, \"big\": 1e30, \"inf\": 1e999}");
    assert(doc.ok());
    auto root = doc.value.root();

    // Positive integer - should work
    auto pos = root.get_field("pos").value.get_int64();
    assert(pos.ok());
    assert(pos.value == 42);

    // Negative integer - should work
    auto neg = root.get_field("neg").value.get_int64();
    assert(neg.ok());
    assert(neg.value == -42);

    // Fractional - should fail
    auto frac = root.get_field("frac").value.get_int64();
    assert(!frac.ok());
    assert(frac.status == Status::TypeMismatch);

    // Too large - should fail
    auto big = root.get_field("big").value.get_int64();
    assert(!big.ok());
    assert(big.status == Status::TypeMismatch);

    // Infinity - should fail (not finite)
    auto inf_cursor = root.get_field("inf");
    if (inf_cursor.ok()) {
        auto inf = inf_cursor.value.get_int64();
        if (!std::isfinite(inf_cursor.value.get_double().value)) {
            assert(!inf.ok());
        }
    }

    // NaN - should fail (tests isfinite check directly)
    JsonValue nan_val(JsonValue::Number(std::nan("")));
    JsonCursor nan_cursor(&nan_val);
    auto nan_result = nan_cursor.get_int64();
    assert(!nan_result.ok());
    assert(nan_result.status == Status::TypeMismatch);

    // Explicit infinity test
    double inf_num = std::numeric_limits<double>::infinity();
    JsonValue inf_val2{JsonValue::Number(inf_num)};
    JsonCursor inf_cursor2(&inf_val2);
    auto inf_result = inf_cursor2.get_int64();
    assert(!inf_result.ok());
    assert(inf_result.status == Status::TypeMismatch);

    std::cout << "✓ test_int64_edge_cases passed\n";
}

// ============================================================================
// raw() method test
// ============================================================================

void test_raw_method() {
    auto doc = JsonDocument::from_string("[1, 2, 3]");
    assert(doc.ok());
    auto cursor = doc.value.root();

    const JsonValue* raw = cursor.raw();
    assert(raw != nullptr);
    assert(raw->is_array());

    // Empty cursor
    JsonCursor empty(nullptr);
    assert(empty.raw() == nullptr);

    std::cout << "✓ test_raw_method passed\n";
}

// ============================================================================
// size methods tests
// ============================================================================

void test_size_methods() {
    // Empty cursor
    JsonCursor empty(nullptr);
    assert(empty.array_size() == 0);
    assert(empty.object_size() == 0);

    // Array cursor
    auto arr_doc = JsonDocument::from_string("[1, 2, 3]");
    assert(arr_doc.ok());
    auto arr = arr_doc.value.root();
    assert(arr.array_size() == 3);
    assert(arr.object_size() == 0);  // Not an object

    // Object cursor
    auto obj_doc = JsonDocument::from_string("{\"a\": 1, \"b\": 2}");
    assert(obj_doc.ok());
    auto obj = obj_doc.value.root();
    assert(obj.object_size() == 2);
    assert(obj.array_size() == 0);  // Not an array

    std::cout << "✓ test_size_methods passed\n";
}

// ============================================================================
// object_keys method test
// ============================================================================

void test_object_keys() {
    // Object cursor
    auto obj_doc = JsonDocument::from_string("{\"name\": \"Alice\", \"age\": 30, \"city\": \"NYC\"}");
    assert(obj_doc.ok());
    auto obj = obj_doc.value.root();
    auto keys = obj.object_keys();
    assert(keys.size() == 3);
    // Keys may be in any order, check they exist
    bool has_name = false, has_age = false, has_city = false;
    for (const auto& key : keys) {
        if (key == "name") has_name = true;
        if (key == "age") has_age = true;
        if (key == "city") has_city = true;
    }
    assert(has_name && has_age && has_city);

    // Non-object cursor should return empty keys
    auto arr_doc = JsonDocument::from_string("[1, 2, 3]");
    assert(arr_doc.ok());
    auto arr_keys = arr_doc.value.root().object_keys();
    assert(arr_keys.empty());

    // Empty cursor
    JsonCursor empty(nullptr);
    auto empty_keys = empty.object_keys();
    assert(empty_keys.empty());

    std::cout << "✓ test_object_keys passed\n";
}

// ============================================================================
// Type predicates tests
// ============================================================================

void test_type_predicates() {
    auto doc = JsonDocument::from_string(
        "{\"null_val\": null, \"bool_val\": true, \"num_val\": 42, "
        "\"str_val\": \"hello\", \"arr_val\": [1], \"obj_val\": {}}"
    );
    assert(doc.ok());
    auto root = doc.value.root();

    // null
    auto null_cursor = root.get_field("null_val").value;
    assert(null_cursor.is_null());
    assert(!null_cursor.is_bool());
    assert(!null_cursor.is_number());
    assert(!null_cursor.is_string());
    assert(!null_cursor.is_array());
    assert(!null_cursor.is_object());

    // bool
    auto bool_cursor = root.get_field("bool_val").value;
    assert(!bool_cursor.is_null());
    assert(bool_cursor.is_bool());
    assert(!bool_cursor.is_number());
    assert(!bool_cursor.is_string());
    assert(!bool_cursor.is_array());
    assert(!bool_cursor.is_object());

    // number
    auto num_cursor = root.get_field("num_val").value;
    assert(!num_cursor.is_null());
    assert(!num_cursor.is_bool());
    assert(num_cursor.is_number());
    assert(!num_cursor.is_string());
    assert(!num_cursor.is_array());
    assert(!num_cursor.is_object());

    // string
    auto str_cursor = root.get_field("str_val").value;
    assert(!str_cursor.is_null());
    assert(!str_cursor.is_bool());
    assert(!str_cursor.is_number());
    assert(str_cursor.is_string());
    assert(!str_cursor.is_array());
    assert(!str_cursor.is_object());

    // array
    auto arr_cursor = root.get_field("arr_val").value;
    assert(!arr_cursor.is_null());
    assert(!arr_cursor.is_bool());
    assert(!arr_cursor.is_number());
    assert(!arr_cursor.is_string());
    assert(arr_cursor.is_array());
    assert(!arr_cursor.is_object());

    // object
    auto obj_cursor = root.get_field("obj_val").value;
    assert(!obj_cursor.is_null());
    assert(!obj_cursor.is_bool());
    assert(!obj_cursor.is_number());
    assert(!obj_cursor.is_string());
    assert(!obj_cursor.is_array());
    assert(obj_cursor.is_object());

    // Empty cursor - all false
    JsonCursor empty(nullptr);
    assert(!empty.is_null());
    assert(!empty.is_bool());
    assert(!empty.is_number());
    assert(!empty.is_string());
    assert(!empty.is_array());
    assert(!empty.is_object());

    std::cout << "✓ test_type_predicates passed\n";
}

// ============================================================================
// get_number method test (alias for get_double)
// ============================================================================

void test_get_number() {
    auto doc = JsonDocument::from_string("{\"val\": 3.14}");
    assert(doc.ok());
    auto root = doc.value.root();

    auto num = root.get_field("val").value.get_number();
    assert(num.ok());
    assert(std::abs(num.value - 3.14) < 0.001);

    // On non-number
    auto doc2 = JsonDocument::from_string("{\"val\": \"text\"}");
    assert(doc2.ok());
    auto num2 = doc2.value.root().get_field("val").value.get_number();
    assert(!num2.ok());

    std::cout << "✓ test_get_number passed\n";
}

// ============================================================================
// KeyNotFound tests
// ============================================================================

void test_key_not_found() {
    auto doc = JsonDocument::from_string("{\"name\": \"Alice\"}");
    assert(doc.ok());
    auto root = doc.value.root();

    auto result = root.get_field("nonexistent");
    assert(!result.ok());
    assert(result.status == Status::KeyNotFound);

    std::cout << "✓ test_key_not_found passed\n";
}

// ============================================================================
// IndexOutOfBounds tests
// ============================================================================

void test_index_out_of_bounds() {
    auto doc = JsonDocument::from_string("[1, 2, 3]");
    assert(doc.ok());
    auto arr = doc.value.root();

    // Valid index
    auto valid = arr.get_at(0);
    assert(valid.ok());

    // Out of bounds
    auto oob = arr.get_at(10);
    assert(!oob.ok());
    assert(oob.status == Status::IndexOutOfBounds);

    // Exactly at size
    auto at_size = arr.get_at(3);
    assert(!at_size.ok());
    assert(at_size.status == Status::IndexOutOfBounds);

    std::cout << "✓ test_index_out_of_bounds passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running JSON Cursor tests...\n\n";

    test_basic_navigation();
    test_type_mismatch_errors();
    test_throwing_methods();
    test_throwing_methods_success();
    test_uint64_edge_cases();
    test_int64_edge_cases();
    test_raw_method();
    test_size_methods();
    test_object_keys();
    test_type_predicates();
    test_get_number();
    test_key_not_found();
    test_index_out_of_bounds();

    std::cout << "\n✅ All JSON Cursor tests passed!\n";
    return 0;
}

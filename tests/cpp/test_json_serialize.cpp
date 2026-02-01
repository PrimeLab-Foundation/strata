/**
 * test_json_serialize.cpp - Tests for JSON serialization
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

using namespace strata;

void test_serialize_null() {
    JsonValue value;
    std::string result = serialize_json(value);
    assert(result == "null");
    std::cout << "✓ test_serialize_null passed\n";
}

void test_serialize_bool() {
    JsonValue value_true(JsonValue::Variant(true));
    assert(serialize_json(value_true) == "true");

    JsonValue value_false(JsonValue::Variant(false));
    assert(serialize_json(value_false) == "false");
    std::cout << "✓ test_serialize_bool passed\n";
}

void test_serialize_number() {
    // Integer-like float
    JsonValue int_val(JsonValue::Variant(42.0));
    std::string result = serialize_json(int_val);
    assert(result == "42.0");

    // Float
    JsonValue float_val(JsonValue::Variant(3.14));
    result = serialize_json(float_val);
    assert(result.find("3.14") != std::string::npos);

    // Negative
    JsonValue neg_val(JsonValue::Variant(-123.0));
    result = serialize_json(neg_val);
    assert(result == "-123.0");

    std::cout << "✓ test_serialize_number passed\n";
}

void test_serialize_string() {
    JsonValue value(JsonValue::Variant(std::string("hello")));
    assert(serialize_json(value) == "\"hello\"");

    // Empty string
    JsonValue empty(JsonValue::Variant(std::string("")));
    assert(serialize_json(empty) == "\"\"");

    std::cout << "✓ test_serialize_string passed\n";
}

void test_serialize_string_escaping() {
    // Quotes
    JsonValue quotes(JsonValue::Variant(std::string("say \"hello\"")));
    std::string result = serialize_json(quotes);
    assert(result.find("\\\"") != std::string::npos);

    // Newline
    JsonValue newline(JsonValue::Variant(std::string("line1\nline2")));
    result = serialize_json(newline);
    assert(result.find("\\n") != std::string::npos);

    // Tab
    JsonValue tab(JsonValue::Variant(std::string("a\tb")));
    result = serialize_json(tab);
    assert(result.find("\\t") != std::string::npos);

    // Backspace and formfeed
    JsonValue ctrl(JsonValue::Variant(std::string("a\b\f")));
    result = serialize_json(ctrl);
    assert(result.find("\\b") != std::string::npos);
    assert(result.find("\\f") != std::string::npos);

    std::cout << "✓ test_serialize_string_escaping passed\n";
}

void test_serialize_array() {
    // Empty array
    JsonValue::Array empty_arr;
    JsonValue empty{JsonValue::Variant(empty_arr)};
    assert(serialize_json(empty) == "[]");

    // Simple array
    JsonValue::Array arr;
    arr.push_back(JsonValue(JsonValue::Variant(1.0)));
    arr.push_back(JsonValue(JsonValue::Variant(2.0)));
    arr.push_back(JsonValue(JsonValue::Variant(3.0)));
    JsonValue value(JsonValue::Variant(std::move(arr)));
    assert(serialize_json(value) == "[1.0,2.0,3.0]");

    std::cout << "✓ test_serialize_array passed\n";
}

void test_serialize_object() {
    // Empty object
    JsonValue::Object empty_obj;
    JsonValue empty{JsonValue::Variant(empty_obj)};
    assert(serialize_json(empty) == "{}");

    // Simple object
    JsonValue::Object obj;
    obj["name"] = JsonValue(JsonValue::Variant(std::string("Alice")));
    obj["age"] = JsonValue(JsonValue::Variant(30.0));
    JsonValue value(JsonValue::Variant(std::move(obj)));
    std::string result = serialize_json(value);

    // Check contains expected parts (order may vary)
    assert(result.find("\"name\"") != std::string::npos);
    assert(result.find("\"Alice\"") != std::string::npos);
    assert(result.find("\"age\"") != std::string::npos);
    assert(result.find("30") != std::string::npos);

    std::cout << "✓ test_serialize_object passed\n";
}

void test_serialize_nested() {
    // Nested structure
    JsonValue::Object obj;
    JsonValue::Array arr;
    arr.push_back(JsonValue(JsonValue::Variant(1.0)));
    arr.push_back(JsonValue(JsonValue::Variant(2.0)));

    obj["numbers"] = JsonValue(JsonValue::Variant(std::move(arr)));
    obj["active"] = JsonValue(JsonValue::Variant(true));

    JsonValue value(JsonValue::Variant(std::move(obj)));
    std::string result = serialize_json(value);

    assert(result.find("[1.0,2.0]") != std::string::npos);
    assert(result.find("true") != std::string::npos);

    std::cout << "✓ test_serialize_nested passed\n";
}

void test_roundtrip() {
    // Create a value, serialize it, parse it back
    JsonValue::Object obj;
    obj["name"] = JsonValue(JsonValue::Variant(std::string("Test")));
    obj["value"] = JsonValue(JsonValue::Variant(42.0));
    JsonValue original(JsonValue::Variant(std::move(obj)));

    std::string json_str = serialize_json(original);
    auto parse_result = parse_json(json_str);
    assert(parse_result.ok());

    // Verify parsed content
    const auto& parsed = parse_result.value;
    assert(parsed.is_object());
    const auto& parsed_obj = parsed.as_object();
    assert(parsed_obj.at("name").as_string() == "Test");
    assert(parsed_obj.at("value").as_number() == 42.0);

    std::cout << "✓ test_roundtrip passed\n";
}

void test_special_floats() {
    // NaN should become null
    double nan_value = std::nan("");
    JsonValue nan_val{JsonValue::Variant(nan_value)};
    assert(serialize_json(nan_val) == "null");

    // Infinity should become null
    double inf_value = INFINITY;
    JsonValue inf_val{JsonValue::Variant(inf_value)};
    assert(serialize_json(inf_val) == "null");

    // Negative infinity
    double neg_inf_value = -INFINITY;
    JsonValue neg_inf_val{JsonValue::Variant(neg_inf_value)};
    assert(serialize_json(neg_inf_val) == "null");

    std::cout << "✓ test_special_floats passed\n";
}

int main() {
    std::cout << "Running JSON serialization tests...\n\n";

    test_serialize_null();
    test_serialize_bool();
    test_serialize_number();
    test_serialize_string();
    test_serialize_string_escaping();
    test_serialize_array();
    test_serialize_object();
    test_serialize_nested();
    test_roundtrip();
    test_special_floats();

    std::cout << "\n✅ All JSON serialization tests passed!\n";
    return 0;
}

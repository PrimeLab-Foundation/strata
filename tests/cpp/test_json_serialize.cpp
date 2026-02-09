/**
 * test_json_serialize.cpp - Tests for JSON serialization
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
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
    // Integer (compact format: no ".0" suffix)
    JsonValue int_val(JsonValue::Variant(static_cast<int64_t>(42)));
    std::string result = serialize_json(int_val);
    assert(result == "42");

    // Float
    JsonValue float_val(JsonValue::Variant(3.14));
    result = serialize_json(float_val);
    assert(result.find("3.14") != std::string::npos);

    // Negative (compact format)
    JsonValue neg_val(JsonValue::Variant(static_cast<int64_t>(-123)));
    result = serialize_json(neg_val);
    assert(result == "-123");

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

    // Backslash - test escape of backslash itself
    JsonValue backslash(JsonValue::Variant(std::string("path\\to\\file")));
    result = serialize_json(backslash);
    assert(result.find("\\\\") != std::string::npos);

    // Carriage return
    JsonValue cr(JsonValue::Variant(std::string("line1\rline2")));
    result = serialize_json(cr);
    assert(result.find("\\r") != std::string::npos);

    // Control characters (non-standard, require \uXXXX encoding)
    // \x01 (SOH) should become \u0001
    std::string ctrl_str;
    ctrl_str += 'a';
    ctrl_str += '\x01';
    ctrl_str += 'b';
    JsonValue ctrl_char{JsonValue::Variant(ctrl_str)};
    result = serialize_json(ctrl_char);
    assert(result.find("\\u0001") != std::string::npos);

    // \x1F (unit separator) should become \u001f
    std::string us_str;
    us_str += 'a';
    us_str += '\x1f';
    us_str += 'b';
    JsonValue us_char{JsonValue::Variant(us_str)};
    result = serialize_json(us_char);
    assert(result.find("\\u001f") != std::string::npos);

    std::cout << "✓ test_serialize_string_escaping passed\n";
}

void test_serialize_array() {
    // Empty array
    JsonValue::Array empty_arr;
    JsonValue empty{JsonValue::Variant(empty_arr)};
    assert(serialize_json(empty) == "[]");

    // Simple array (integer-like floats use compact format)
    JsonValue::Array arr;
    arr.push_back(JsonValue(JsonValue::Variant(static_cast<int64_t>(1))));
    arr.push_back(JsonValue(JsonValue::Variant(static_cast<int64_t>(2))));
    arr.push_back(JsonValue(JsonValue::Variant(static_cast<int64_t>(3))));
    JsonValue value(JsonValue::Variant(std::move(arr)));
    assert(serialize_json(value) == "[1,2,3]");

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
    obj["age"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(30)));
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
    arr.push_back(JsonValue(JsonValue::Variant(static_cast<int64_t>(1))));
    arr.push_back(JsonValue(JsonValue::Variant(static_cast<int64_t>(2))));

    obj["numbers"] = JsonValue(JsonValue::Variant(std::move(arr)));
    obj["active"] = JsonValue(JsonValue::Variant(true));

    JsonValue value(JsonValue::Variant(std::move(obj)));
    std::string result = serialize_json(value);

    assert(result.find("[1,2]") != std::string::npos);
    assert(result.find("true") != std::string::npos);

    std::cout << "✓ test_serialize_nested passed\n";
}

void test_roundtrip() {
    // Create a value, serialize it, parse it back
    JsonValue::Object obj;
    obj["name"] = JsonValue(JsonValue::Variant(std::string("Test")));
    obj["value"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(42)));
    JsonValue original(JsonValue::Variant(std::move(obj)));

    std::string json_str = serialize_json(original);
    auto parse_result = parse_json(json_str);
    assert(parse_result.ok());

    // Verify parsed content
    const auto& parsed = parse_result.value;
    assert(parsed.is_object());
    const auto& parsed_obj = parsed.as_object();
    assert(parsed_obj.at("name").as_string() == "Test");
    assert(parsed_obj.at("value").as_int() == 42);

    std::cout << "✓ test_roundtrip passed\n";
}

void test_serialize_json_to() {
    // Test serialize_json_to function (alternative API that writes to pre-existing string)
    JsonValue::Object obj;
    obj["name"] = JsonValue(JsonValue::Variant(std::string("Test")));
    obj["value"] = JsonValue(JsonValue::Variant(static_cast<int64_t>(42)));
    JsonValue original(JsonValue::Variant(std::move(obj)));

    std::string out;
    serialize_json_to(original, out);

    assert(!out.empty());
    assert(out.find("\"name\":") != std::string::npos);
    assert(out.find("\"Test\"") != std::string::npos);
    assert(out.find("42") != std::string::npos);

    // Verify roundtrip
    auto parse_result = parse_json(out);
    assert(parse_result.ok());
    assert(parse_result.value.as_object().at("name").as_string() == "Test");

    std::cout << "✓ test_serialize_json_to passed\n";
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

void test_serialize_integer_edge_cases() {
    // Zero - tests the value == 0 path in format_integer
    JsonValue zero{JsonValue::Variant(static_cast<int64_t>(0))};
    std::string result = serialize_json(zero);
    assert(result == "0");

    // Small integers
    JsonValue one{JsonValue::Variant(static_cast<int64_t>(1))};
    assert(serialize_json(one) == "1");

    JsonValue neg_one{JsonValue::Variant(static_cast<int64_t>(-1))};
    assert(serialize_json(neg_one) == "-1");

    // Larger integers that still fit exactly in double
    JsonValue large_pos{JsonValue::Variant(static_cast<int64_t>(123456789))};
    assert(serialize_json(large_pos) == "123456789");

    JsonValue large_neg{JsonValue::Variant(static_cast<int64_t>(-123456789))};
    assert(serialize_json(large_neg) == "-123456789");

    // Max safe integer for double (2^53 - 1)
    JsonValue max_safe{JsonValue::Variant(static_cast<int64_t>(9007199254740991LL))};
    result = serialize_json(max_safe);
    assert(result.find("9007199254740991") != std::string::npos);

    // Negative large integer
    JsonValue neg_large{JsonValue::Variant(static_cast<int64_t>(-9007199254740991LL))};
    result = serialize_json(neg_large);
    assert(result.find("-9007199254740991") != std::string::npos);

    // INT64_MIN formatting
    JsonValue min_val{JsonValue::Variant(std::numeric_limits<int64_t>::min())};
    result = serialize_json(min_val);
    assert(result == "-9223372036854775808");

    std::cout << "✓ test_serialize_integer_edge_cases passed\n";
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
    test_serialize_json_to();
    test_special_floats();
    test_serialize_integer_edge_cases();
    std::cout << "\n✅ All JSON serialization tests passed!\n";
    return 0;
}

/**
 * test_json_parse.cpp - Tests for JSON parsing
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

using namespace strata;

void test_parse_null() {
    auto result = parse_json("null");
    assert(result.ok());
    assert(result.value.is_null());
    std::cout << "✓ test_parse_null passed\n";
}

void test_parse_bool() {
    auto result_true = parse_json("true");
    assert(result_true.ok());
    assert(result_true.value.is_bool());
    assert(result_true.value.as_bool() == true);

    auto result_false = parse_json("false");
    assert(result_false.ok());
    assert(result_false.value.is_bool());
    assert(result_false.value.as_bool() == false);

    std::cout << "✓ test_parse_bool passed\n";
}

void test_parse_number() {
    // Integer
    auto result_int = parse_json("42");
    assert(result_int.ok());
    assert(result_int.value.is_number());
    assert(result_int.value.is_int());
    assert(result_int.value.as_int() == 42);

    // Negative
    auto result_neg = parse_json("-123");
    assert(result_neg.ok());
    assert(result_neg.value.is_int());
    assert(result_neg.value.as_int() == -123);

    // Float
    auto result_float = parse_json("3.14");
    assert(result_float.ok());
    assert(result_float.value.is_double());
    assert(std::abs(result_float.value.as_double() - 3.14) < 0.001);

    // Scientific notation
    auto result_sci = parse_json("1.23e10");
    assert(result_sci.ok());
    assert(result_sci.value.is_double());
    assert(result_sci.value.as_double() == 1.23e10);

    std::cout << "✓ test_parse_number passed\n";
}

void test_parse_string() {
    // Simple string
    auto result = parse_json("\"hello\"");
    assert(result.ok());
    assert(result.value.is_string());
    assert(result.value.as_string() == "hello");

    // Empty string
    auto result_empty = parse_json("\"\"");
    assert(result_empty.ok());
    assert(result_empty.value.as_string() == "");

    std::cout << "✓ test_parse_string passed\n";
}

void test_parse_string_escapes() {
    // Newline
    auto result_newline = parse_json("\"line1\\nline2\"");
    assert(result_newline.ok());
    assert(result_newline.value.as_string().find('\n') != std::string::npos);

    // Quote
    auto result_quote = parse_json("\"say \\\"hello\\\"\"");
    assert(result_quote.ok());
    assert(result_quote.value.as_string().find('"') != std::string::npos);

    // Tab
    auto result_tab = parse_json("\"a\\tb\"");
    assert(result_tab.ok());
    assert(result_tab.value.as_string().find('\t') != std::string::npos);

    std::cout << "✓ test_parse_string_escapes passed\n";
}

void test_parse_unicode() {
    // Direct UTF-8
    auto result_utf8 = parse_json("\"Hello 世界\"");
    assert(result_utf8.ok());
    assert(result_utf8.value.as_string().find("世界") != std::string::npos);

    // Emoji
    auto result_emoji = parse_json("\"👋\"");
    assert(result_emoji.ok());
    assert(result_emoji.value.as_string() == "👋");

    // Unicode escape sequences
    auto result_escape = parse_json("\"\\u0041\"");
    assert(result_escape.ok());
    assert(result_escape.value.as_string() == "A");

    auto result_escape_greek = parse_json("\"\\u03A9\"");
    assert(result_escape_greek.ok());
    assert(result_escape_greek.value.as_string() == "Ω");

    auto result_surrogate = parse_json("\"\\uD83D\\uDC4B\"");
    assert(result_surrogate.ok());
    assert(result_surrogate.value.as_string() == "👋");

    auto result_bad_high = parse_json("\"\\uD800\"");
    assert(!result_bad_high.ok());

    auto result_bad_low = parse_json("\"\\uDC00\"");
    assert(!result_bad_low.ok());

    std::cout << "✓ test_parse_unicode passed\n";
}

void test_parse_array() {
    // Empty array
    auto result_empty = parse_json("[]");
    assert(result_empty.ok());
    assert(result_empty.value.is_array());
    assert(result_empty.value.as_array().empty());

    // Simple array
    auto result = parse_json("[1, 2, 3]");
    assert(result.ok());
    assert(result.value.is_array());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 3);
    assert(arr[0].as_int() == 1);
    assert(arr[1].as_int() == 2);
    assert(arr[2].as_int() == 3);

    std::cout << "✓ test_parse_array passed\n";
}

void test_parse_nested_array() {
    auto result = parse_json("[[1, 2], [3, 4]]");
    assert(result.ok());
    assert(result.value.is_array());
    const auto& arr = result.value.as_array();
    assert(arr.size() == 2);
    assert(arr[0].is_array());
    assert(arr[0].as_array().size() == 2);
    assert(arr[0].as_array()[0].as_int() == 1);

    std::cout << "✓ test_parse_nested_array passed\n";
}

void test_parse_object() {
    // Empty object
    auto result_empty = parse_json("{}");
    assert(result_empty.ok());
    assert(result_empty.value.is_object());
    assert(result_empty.value.as_object().empty());

    // Simple object
    auto result = parse_json("{\"name\": \"Alice\", \"age\": 30}");
    assert(result.ok());
    assert(result.value.is_object());
    const auto& obj = result.value.as_object();
    assert(obj.size() == 2);
    assert(obj.at("name").as_string() == "Alice");
    assert(obj.at("age").as_int() == 30);

    std::cout << "✓ test_parse_object passed\n";
}

void test_parse_escaped_key() {
    auto result = parse_json("{\"a\\tb\": 1}");
    assert(result.ok());
    const auto& obj = result.value.as_object();
    auto it = obj.find("a\tb");
    assert(it != obj.end());
    assert(it->second.as_int() == 1);
    std::cout << "✓ test_parse_escaped_key passed\n";
}

void test_parse_unsigned_large() {
    auto result = parse_json("9223372036854775808"); // INT64_MAX + 1
    assert(result.ok());
    assert(result.value.is_number());
    assert(result.value.is_double());
    assert(result.value.as_double() > 9.22e18);
    std::cout << "✓ test_parse_unsigned_large passed\n";
}

void test_duplicate_key_policies() {
    auto original_policy = get_duplicate_key_policy();

    set_duplicate_key_policy(DuplicateKeyPolicy::Warn);
    auto warn_result = parse_json("{\"a\": 1, \"a\": 2}");
    assert(warn_result.ok());
    auto warnings = consume_parse_warnings();
    assert(warnings.size() == 1);
    assert(warnings[0].find("Duplicate key encountered") != std::string::npos);
    auto warnings_cleared = consume_parse_warnings();
    assert(warnings_cleared.empty());

    set_duplicate_key_policy(DuplicateKeyPolicy::Error);
    auto error_result = parse_json("{\"a\": 1, \"a\": 2}");
    assert(!error_result.ok());

    set_duplicate_key_policy(original_policy);
    std::cout << "✓ test_duplicate_key_policies passed\n";
}

void test_parse_nested_object() {
    auto result = parse_json("{\"user\": {\"name\": \"Bob\", \"age\": 25}}");
    assert(result.ok());
    const auto& obj = result.value.as_object();
    assert(obj.at("user").is_object());
    const auto& user = obj.at("user").as_object();
    assert(user.at("name").as_string() == "Bob");
    assert(user.at("age").as_int() == 25);

    std::cout << "✓ test_parse_nested_object passed\n";
}

void test_parse_mixed_structure() {
    const char* json = R"({
        "name": "Test",
        "values": [1, 2, 3],
        "nested": {
            "flag": true,
            "data": null
        }
    })";

    auto result = parse_json(json);
    assert(result.ok());
    const auto& obj = result.value.as_object();
    assert(obj.at("name").as_string() == "Test");
    assert(obj.at("values").as_array().size() == 3);
    assert(obj.at("nested").as_object().at("flag").as_bool() == true);
    assert(obj.at("nested").as_object().at("data").is_null());

    std::cout << "✓ test_parse_mixed_structure passed\n";
}

void test_parse_whitespace() {
    // Leading/trailing whitespace
    auto result = parse_json("  { \"a\" : 1 }  ");
    assert(result.ok());
    assert(result.value.as_object().at("a").as_int() == 1);

    // Lots of whitespace
    auto result2 = parse_json("[\n  1,\n  2,\n  3\n]");
    assert(result2.ok());
    assert(result2.value.as_array().size() == 3);

    std::cout << "✓ test_parse_whitespace passed\n";
}

void test_parse_errors() {
    // Invalid JSON
    auto result_bad1 = parse_json("{invalid}");
    assert(!result_bad1.ok());
    assert(result_bad1.status == Status::ParseError);

    // Incomplete
    auto result_bad2 = parse_json("{\"a\":");
    assert(!result_bad2.ok());

    // Empty string
    auto result_bad3 = parse_json("");
    assert(!result_bad3.ok());

    std::cout << "✓ test_parse_errors passed\n";
}

void test_parse_utf8_validation() {
    // Valid UTF-8: still parses
    auto ok1 = parse_json("\"caf\xc3\xa9\""); // café
    assert(ok1.ok());
    auto ok2 = parse_json("\"\xe4\xbd\xa0\xe5\xa5\xbd\""); // 你好
    assert(ok2.ok());

    // Invalid UTF-8: rejected with ParseError (upfront SIMD validation)
    // Lone continuation byte (0x80); use separate \x so \x80b is not parsed as one escape
    auto r1 = parse_json("\"a\x80\x62\""); // "a" + 0x80 + "b"
    assert(!r1.ok() && r1.status == Status::ParseError);
    // Overlong NUL (C0 80)
    auto r2 = parse_json("\"\xc0\x80\"");
    assert(!r2.ok() && r2.status == Status::ParseError);
    // Invalid lead byte
    auto r3 = parse_json("\"\xff\"");
    assert(!r3.ok() && r3.status == Status::ParseError);
    // Truncated 2-byte (lead only)
    auto r4 = parse_json("\"\xc2\"");
    assert(!r4.ok() && r4.status == Status::ParseError);
    // Overlong E0 80 80
    auto r5 = parse_json("\"\xe0\x80\x80\"");
    assert(!r5.ok() && r5.status == Status::ParseError);
    // Surrogate ED A0 80 (U+D800)
    auto r6 = parse_json("\"\xed\xa0\x80\"");
    assert(!r6.ok() && r6.status == Status::ParseError);
    // Codepoint above U+10FFFF (F4 90 80 80)
    auto r7 = parse_json("\"\xf4\x90\x80\x80\"");
    assert(!r7.ok() && r7.status == Status::ParseError);

    std::cout << "✓ test_parse_utf8_validation passed\n";
}

void test_parse_large_numbers() {
    // Large integer
    auto result_big = parse_json("9007199254740991");
    assert(result_big.ok());
    assert(result_big.value.is_int());
    assert(result_big.value.as_int() == 9007199254740991LL);

    // Very small
    auto result_small = parse_json("0.0000001");
    assert(result_small.ok());
    assert(result_small.value.is_double());
    assert(result_small.value.as_double() > 0);

    std::cout << "✓ test_parse_large_numbers passed\n";
}

int main() {
    std::cout << "Running JSON parsing tests...\n\n";

    test_parse_null();
    test_parse_bool();
    test_parse_number();
    test_parse_string();
    test_parse_string_escapes();
    test_parse_unicode();
    test_parse_utf8_validation();
    test_parse_array();
    test_parse_nested_array();
    test_parse_object();
    test_parse_escaped_key();
    test_parse_nested_object();
    test_parse_mixed_structure();
    test_parse_whitespace();
    test_parse_errors();
    test_parse_large_numbers();
    test_parse_unsigned_large();
    test_duplicate_key_policies();

    std::cout << "\n✅ All JSON parsing tests passed!\n";
    return 0;
}

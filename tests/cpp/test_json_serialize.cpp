/**
 * @file test_json_serialize.cpp
 * @brief Contract suite for serialize_json.
 *
 * Pins the output shape (compact, insertion-ordered, RFC 8259 escaping) and
 * the round-trip law: serializing a parsed document and parsing the result
 * yields the same tree.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <string>
#include <string_view>

using strata::JsonValue;
using strata::parse_json;
using strata::serialize_json;
using strata::serialize_json_to;

namespace {

[[nodiscard]] std::string dumps(std::string_view text) {
    const auto parsed = parse_json(text);
    assert(parsed.ok());
    return serialize_json(parsed.value);
}

/// Structural equality, so the round-trip law can be stated over whole trees.
[[nodiscard]] bool equal(const JsonValue& left, const JsonValue& right) {
    if (left.is_null() || right.is_null())
        return left.is_null() && right.is_null();
    if (left.is_bool() || right.is_bool())
        return left.is_bool() && right.is_bool() && left.as_bool() == right.as_bool();
    if (left.is_number() || right.is_number()) {
        if (!left.is_number() || !right.is_number())
            return false;
        const double a = left.as_number();
        const double b = right.as_number();
        // NaN and infinity serialize as null, so they cannot survive a
        // round trip and are excluded from the law.
        return a == b;
    }
    if (left.is_string() || right.is_string())
        return left.is_string() && right.is_string() && left.as_string() == right.as_string();
    if (left.is_array() || right.is_array()) {
        if (!left.is_array() || !right.is_array())
            return false;
        if (left.as_array().size() != right.as_array().size())
            return false;
        for (size_t index = 0; index < left.as_array().size(); ++index) {
            if (!equal(left.as_array()[index], right.as_array()[index]))
                return false;
        }
        return true;
    }
    if (!left.is_object() || !right.is_object())
        return false;
    if (left.as_object().size() != right.as_object().size())
        return false;
    auto right_it = right.as_object().begin();
    for (const auto& [key, value] : left.as_object()) {
        if (right_it->first != key) // order is part of the contract
            return false;
        if (!equal(value, right_it->second))
            return false;
        ++right_it;
    }
    return true;
}

// ---------------------------------------------------------------------------

void test_scalars() {
    assert(serialize_json(JsonValue()) == "null");
    assert(serialize_json(JsonValue(JsonValue::Variant(true))) == "true");
    assert(serialize_json(JsonValue(JsonValue::Variant(false))) == "false");
    assert(serialize_json(JsonValue(JsonValue::Variant(1.5))) == "1.5");
    assert(serialize_json(JsonValue(JsonValue::Variant(std::string("hi")))) == "\"hi\"");
}

void test_output_is_compact() {
    assert(dumps(R"({ "a" : [ 1 , 2 ] , "b" : { } })") == R"({"a":[1.0,2.0],"b":{}})");
    assert(dumps("[]") == "[]");
    assert(dumps("{}") == "{}");
}

void test_object_key_order_is_insertion_order() {
    // Not sorted: the previous header claimed keys went through a std::map.
    assert(dumps(R"({"zebra":1,"alpha":2,"middle":3})") ==
           R"({"zebra":1.0,"alpha":2.0,"middle":3.0})");
}

void test_string_escaping() {
    assert(dumps(R"("quote\"here")") == R"("quote\"here")");
    assert(dumps(R"("back\\slash")") == R"("back\\slash")");
    assert(dumps(R"("\b\f\n\r\t")") == R"("\b\f\n\r\t")");

    // Control characters below 0x20 with no short escape become \u00XX, in
    // lower case, and short escapes are preferred where one exists.
    assert(dumps(R"("\u0001")") == R"("\u0001")");
    assert(dumps(R"("\u001f")") == R"("\u001f")");
    assert(dumps(R"("\u001F")") == R"("\u001f")");
    assert(dumps(R"("a\u0000b")") == R"("a\u0000b")");
    assert(dumps(R"("\u0009")") == R"("\t")");

    // 0x7f is not a control character as far as JSON is concerned.
    assert(dumps(R"("\u007f")") == "\"\x7f\"");

    // A solidus may be escaped on input but is not escaped on output.
    assert(dumps(R"("a\/b")") == R"("a/b")");
}

/// UTF-8 passes through as bytes; only what JSON requires is escaped.
void test_utf8_is_not_escaped() {
    assert(dumps("\"caf\xc3\xa9\"") == "\"caf\xc3\xa9\"");
    assert(dumps("\"\xe4\xbd\xa0\xe5\xa5\xbd\"") == "\"\xe4\xbd\xa0\xe5\xa5\xbd\"");
    assert(dumps("\"\xf0\x9f\x98\x80\"") == "\"\xf0\x9f\x98\x80\"");
    // A \u escape for a non-ASCII character comes back out as raw UTF-8.
    assert(dumps(R"("é")") == "\"\xc3\xa9\"");
    assert(dumps(R"("😀")") == "\"\xf0\x9f\x98\x80\"");
}

void test_nesting() {
    assert(dumps(R"([[1,[2,[3]]]])") == "[[1.0,[2.0,[3.0]]]]");
    assert(dumps(R"({"a":{"b":{"c":[]}}})") == R"({"a":{"b":{"c":[]}}})");
    assert(dumps(R"([{"a":1},{"a":2}])") == R"([{"a":1.0},{"a":2.0}])");
}

/// Round-trip law: parse, serialize, parse again, and the trees agree.
void test_round_trip_law() {
    const char* corpus[] = {
        "null",
        "true",
        "[]",
        "{}",
        "0",
        "-0.5",
        R"("string with \"escapes\" and \n newlines")",
        R"({"a":1,"b":[true,false,null],"c":{"d":"e"}})",
        R"([1,2.5,-3e10,0.0001,1e16,1e-5])",
        R"({"unicode":"café 你好 😀"})",
        R"({"nested":[{"deep":[{"deeper":[1,2,3]}]}]})",
        R"({"empty_string":"","empty_array":[],"empty_object":{}})",
        R"([9223372036854775807,-9223372036854775808])",
    };

    for (const char* text : corpus) {
        const auto first = parse_json(text);
        assert(first.ok());

        const std::string serialized = serialize_json(first.value);
        const auto second = parse_json(serialized);
        assert(second.ok());
        assert(equal(first.value, second.value));

        // ...and serializing again is byte-identical: the operation is stable.
        assert(serialize_json(second.value) == serialized);
    }
}

void test_serialize_to_reuses_the_buffer() {
    std::string buffer;
    serialize_json_to(JsonValue(JsonValue::Variant(std::string("first"))), buffer);
    assert(buffer == "\"first\"");

    // A second call replaces the contents rather than appending to them.
    serialize_json_to(JsonValue(JsonValue::Variant(2.0)), buffer);
    assert(buffer == "2.0");

    const size_t capacity_before = buffer.capacity();
    serialize_json_to(JsonValue(), buffer);
    assert(buffer == "null");
    assert(buffer.capacity() == capacity_before); // capacity is retained
}

void test_deeply_nested_serialization() {
    constexpr int kDepth = 100;
    std::string text;
    for (int level = 0; level < kDepth; ++level)
        text += '[';
    text += '1';
    for (int level = 0; level < kDepth; ++level)
        text += ']';

    const auto parsed = parse_json(text);
    assert(parsed.ok());
    assert(serialize_json(parsed.value) ==
           std::string(kDepth, '[') + "1.0" + std::string(kDepth, ']'));
}

} // namespace

int main() {
    test_scalars();
    test_output_is_compact();
    test_object_key_order_is_insertion_order();
    test_string_escaping();
    test_utf8_is_not_escaped();
    test_nesting();
    test_round_trip_law();
    test_serialize_to_reuses_the_buffer();
    test_deeply_nested_serialization();

    std::puts("json_serialize_tests: OK");
    return 0;
}

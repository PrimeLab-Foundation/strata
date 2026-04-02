#include "strata/parse_value.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace strata;

static Result<JsonValue> run(const char *input) {
    return parse<JsonValue>(input, input + std::strlen(input));
}

int main() {
    printf("test_parse_value\n");

    // --- dispatch: null ---

    printf("  null                          ");
    {
        auto r = run("null");
        assert(r.has_value());
        assert(std::holds_alternative<std::nullptr_t>(r->value.data));
    }
    printf("ok\n");

    // --- dispatch: bool ---

    printf("  true                          ");
    {
        auto r = run("true");
        assert(r.has_value());
        assert(std::get<bool>(r->value.data) == true);
    }
    printf("ok\n");

    printf("  false                         ");
    {
        auto r = run("false");
        assert(r.has_value());
        assert(std::get<bool>(r->value.data) == false);
    }
    printf("ok\n");

    // --- dispatch: number ---

    printf("  integer                       ");
    {
        auto r = run("42");
        assert(r.has_value());
        auto num = std::get<Number>(r->value.data);
        assert(std::get<int64_t>(num) == 42);
    }
    printf("ok\n");

    printf("  negative integer              ");
    {
        auto r = run("-7");
        assert(r.has_value());
        auto num = std::get<Number>(r->value.data);
        assert(std::get<int64_t>(num) == -7);
    }
    printf("ok\n");

    printf("  float                         ");
    {
        auto r = run("3.14");
        assert(r.has_value());
        auto num = std::get<Number>(r->value.data);
        assert(std::abs(std::get<double>(num) - 3.14) < 1e-12);
    }
    printf("ok\n");

    printf("  zero                          ");
    {
        auto r = run("0");
        assert(r.has_value());
        auto num = std::get<Number>(r->value.data);
        assert(std::get<int64_t>(num) == 0);
    }
    printf("ok\n");

    // --- dispatch: string ---

    printf("  string                        ");
    {
        auto r = run(R"("hello")");
        assert(r.has_value());
        assert(std::get<std::string>(r->value.data) == "hello");
    }
    printf("ok\n");

    printf("  empty string                  ");
    {
        auto r = run(R"("")");
        assert(r.has_value());
        assert(std::get<std::string>(r->value.data).empty());
    }
    printf("ok\n");

    // --- whitespace skipping ---

    printf("  leading space                 ");
    {
        auto r = run("  null");
        assert(r.has_value());
        assert(std::holds_alternative<std::nullptr_t>(r->value.data));
    }
    printf("ok\n");

    printf("  leading tab                   ");
    {
        auto r = run("\ttrue");
        assert(r.has_value());
        assert(std::get<bool>(r->value.data) == true);
    }
    printf("ok\n");

    printf("  leading newline               ");
    {
        auto r = run("\n42");
        assert(r.has_value());
        auto num = std::get<Number>(r->value.data);
        assert(std::get<int64_t>(num) == 42);
    }
    printf("ok\n");

    printf("  leading CR+LF                 ");
    {
        auto r = run("\r\n\"hi\"");
        assert(r.has_value());
        assert(std::get<std::string>(r->value.data) == "hi");
    }
    printf("ok\n");

    printf("  mixed whitespace              ");
    {
        auto r = run("  \t\n\r  false");
        assert(r.has_value());
        assert(std::get<bool>(r->value.data) == false);
    }
    printf("ok\n");

    // --- cursor rest ---

    printf("  rest after value              ");
    {
        auto r = run("true , next");
        assert(r.has_value());
        assert(std::get<bool>(r->value.data) == true);
        assert(*r->rest == ' ');
    }
    printf("ok\n");

    printf("  rest after string             ");
    {
        auto r = run(R"("key" : 1)");
        assert(r.has_value());
        assert(std::get<std::string>(r->value.data) == "key");
        assert(*r->rest == ' ');
    }
    printf("ok\n");

    printf("  rest after number             ");
    {
        auto r = run("123]");
        assert(r.has_value());
        auto num = std::get<Number>(r->value.data);
        assert(std::get<int64_t>(num) == 123);
        assert(*r->rest == ']');
    }
    printf("ok\n");

    // --- errors ---

    printf("  empty input                   ");
    {
        auto r = run("");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  only whitespace               ");
    {
        auto r = run("   \t\n  ");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  unexpected char               ");
    {
        auto r = run("@");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  open brace → empty obj end    ");
    {
        auto r = run("{");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  open bracket → empty arr end  ");
    {
        auto r = run("[");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  invalid literal               ");
    {
        auto r = run("nul");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  whitespace + error            ");
    {
        auto r = run("  @");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    // --- error propagation from sub-parsers ---

    printf("  null error propagates         ");
    {
        auto r = run("nope");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidLiteral);
    }
    printf("ok\n");

    printf("  bool error propagates         ");
    {
        auto r = run("tru");
        assert(!r.has_value());
    }
    printf("ok\n");

    printf("  false error propagates        ");
    {
        auto r = run("fals");
        assert(!r.has_value());
    }
    printf("ok\n");

    printf("  number error propagates       ");
    {
        auto r = run("-");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  string error propagates       ");
    {
        auto r = run(R"("unterminated)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnterminatedString);
    }
    printf("ok\n");

    // --- dispatch: all digit starts ---

    printf("  digit 0                       ");
    {
        auto r = run("0");
        assert(r.has_value());
        assert(std::get<int64_t>(std::get<Number>(r->value.data)) == 0);
    }
    printf("ok\n");

    printf("  digit 1                       ");
    {
        auto r = run("1");
        assert(r.has_value());
        assert(std::get<int64_t>(std::get<Number>(r->value.data)) == 1);
    }
    printf("ok\n");

    printf("  digit 9                       ");
    {
        auto r = run("9");
        assert(r.has_value());
        assert(std::get<int64_t>(std::get<Number>(r->value.data)) == 9);
    }
    printf("ok\n");

    printf("  ws + each type                ");
    {
        auto r1 = run("  true");
        assert(r1 && std::get<bool>(r1->value.data) == true);
        auto r2 = run("  false");
        assert(r2 && std::get<bool>(r2->value.data) == false);
        auto r3 = run("  null");
        assert(r3 && std::holds_alternative<std::nullptr_t>(r3->value.data));
        auto r4 = run(R"(  "x")");
        assert(r4 && std::get<std::string>(r4->value.data) == "x");
        auto r5 = run("  -1");
        assert(r5 && std::get<int64_t>(std::get<Number>(r5->value.data)) == -1);
    }
    printf("ok\n");

    // --- arrays ---

    printf("  empty array                   ");
    {
        auto r = run("[]");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.empty());
    }
    printf("ok\n");

    printf("  array with one int            ");
    {
        auto r = run("[1]");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 1);
        assert(std::get<int64_t>(std::get<Number>(arr[0].data)) == 1);
    }
    printf("ok\n");

    printf("  array with multiple values    ");
    {
        auto r = run("[1, 2, 3]");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 3);
        assert(std::get<int64_t>(std::get<Number>(arr[0].data)) == 1);
        assert(std::get<int64_t>(std::get<Number>(arr[1].data)) == 2);
        assert(std::get<int64_t>(std::get<Number>(arr[2].data)) == 3);
    }
    printf("ok\n");

    printf("  array mixed types             ");
    {
        auto r = run(R"([null, true, 42, "hi"])");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 4);
        assert(std::holds_alternative<std::nullptr_t>(arr[0].data));
        assert(std::get<bool>(arr[1].data) == true);
        assert(std::get<int64_t>(std::get<Number>(arr[2].data)) == 42);
        assert(std::get<std::string>(arr[3].data) == "hi");
    }
    printf("ok\n");

    printf("  nested array                  ");
    {
        auto r = run("[[1, 2], [3]]");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 2);
        auto &inner0 = std::get<JsonValue::Array>(arr[0].data);
        assert(inner0.size() == 2);
        auto &inner1 = std::get<JsonValue::Array>(arr[1].data);
        assert(inner1.size() == 1);
    }
    printf("ok\n");

    printf("  array no whitespace           ");
    {
        auto r = run("[1,2,3]");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 3);
    }
    printf("ok\n");

    printf("  array lots of whitespace      ");
    {
        auto r = run("[  1  ,  2  ,  3  ]");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 3);
    }
    printf("ok\n");

    printf("  array + trailing              ");
    {
        auto r = run("[1] , rest");
        assert(r.has_value());
        assert(*r->rest == ' ');
    }
    printf("ok\n");

    printf("  array unterminated            ");
    {
        auto r = run("[1, 2");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  array missing comma           ");
    {
        auto r = run("[1 2]");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  array empty unterminated      ");
    {
        auto r = run("[");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  array bad value               ");
    {
        auto r = run("[1, @]");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    // --- objects ---

    printf("  empty object                  ");
    {
        auto r = run("{}");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.empty());
    }
    printf("ok\n");

    printf("  one key-value                 ");
    {
        auto r = run(R"({"a": 1})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 1);
        assert(obj[0].first == "a");
        assert(std::get<int64_t>(std::get<Number>(obj[0].second.data)) == 1);
    }
    printf("ok\n");

    printf("  multiple key-values           ");
    {
        auto r = run(R"({"a": 1, "b": true, "c": "x"})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 3);
        assert(obj[0].first == "a");
        assert(obj[1].first == "b");
        assert(std::get<bool>(obj[1].second.data) == true);
        assert(obj[2].first == "c");
        assert(std::get<std::string>(obj[2].second.data) == "x");
    }
    printf("ok\n");

    printf("  nested object                 ");
    {
        auto r = run(R"({"a": {"b": 1}})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 1);
        auto &inner = std::get<JsonValue::Object>(obj[0].second.data);
        assert(inner.size() == 1);
        assert(inner[0].first == "b");
    }
    printf("ok\n");

    printf("  object with array value       ");
    {
        auto r = run(R"({"items": [1, 2, 3]})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 1);
        auto &arr = std::get<JsonValue::Array>(obj[0].second.data);
        assert(arr.size() == 3);
    }
    printf("ok\n");

    printf("  array with object elements    ");
    {
        auto r = run(R"([{"a": 1}, {"b": 2}])");
        assert(r.has_value());
        auto &arr = std::get<JsonValue::Array>(r->value.data);
        assert(arr.size() == 2);
        auto &obj0 = std::get<JsonValue::Object>(arr[0].data);
        assert(obj0[0].first == "a");
        auto &obj1 = std::get<JsonValue::Object>(arr[1].data);
        assert(obj1[0].first == "b");
    }
    printf("ok\n");

    printf("  object no whitespace          ");
    {
        auto r = run(R"({"a":1,"b":2})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 2);
    }
    printf("ok\n");

    printf("  object lots of whitespace     ");
    {
        auto r = run("{ \"a\" : 1 , \"b\" : 2 }");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 2);
    }
    printf("ok\n");

    printf("  object preserves order        ");
    {
        auto r = run(R"({"z": 1, "a": 2, "m": 3})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj[0].first == "z");
        assert(obj[1].first == "a");
        assert(obj[2].first == "m");
    }
    printf("ok\n");

    printf("  object + trailing             ");
    {
        auto r = run(R"({"a": 1} , rest)");
        assert(r.has_value());
        assert(*r->rest == ' ');
    }
    printf("ok\n");

    printf("  object unterminated           ");
    {
        auto r = run(R"({"a": 1)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  object missing colon          ");
    {
        auto r = run(R"({"a" 1})");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  object missing comma          ");
    {
        auto r = run(R"({"a": 1 "b": 2})");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  object non-string key         ");
    {
        auto r = run(R"({1: "a"})");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  object empty unterminated     ");
    {
        auto r = run("{");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  object bad value              ");
    {
        auto r = run(R"({"a": @})");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  object colon then end         ");
    {
        auto r = run(R"({"a":)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  deeply nested                 ");
    {
        auto r = run(R"({"a": [{"b": [1, [2, [3]]]}]})");
        assert(r.has_value());
        auto &obj = std::get<JsonValue::Object>(r->value.data);
        assert(obj.size() == 1);
    }
    printf("ok\n");

    // --- coverage: object edge cases ---

    printf("  object key then EOF           ");
    {
        auto r = run(R"({"a")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  object comma then EOF         ");
    {
        auto r = run(R"({"a": 1,)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  object comma ws then EOF      ");
    {
        auto r = run(R"({"a": 1,   )");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  object value then EOF         ");
    {
        auto r = run(R"({"a": 1)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  object bad key string         ");
    {
        auto r = run(R"({"abc)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnterminatedString);
    }
    printf("ok\n");

    // --- coverage: array edge cases ---

    printf("  array value then EOF          ");
    {
        auto r = run("[1");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  array comma then EOF          ");
    {
        auto r = run("[1,");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  array trailing comma          ");
    {
        auto r = run("[1,]");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  object trailing comma         ");
    {
        auto r = run(R"({"a": 1,})");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  all passed\n");
    return 0;
}
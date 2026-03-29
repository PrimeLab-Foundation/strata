#include "strata/parse_value.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace strata;

static Result<JsonValue> run(const char* input) {
    return parse<JsonValue>(input, input + std::strlen(input));
}

int main() {
    printf("test_parse_value\n");

    // --- dispatch: null ---

    printf("  null                          ");
    {
        auto r = run("null");
        assert(r.has_value());
        assert(std::holds_alternative<std::nullptr_t>(r->value));
    }
    printf("ok\n");

    // --- dispatch: bool ---

    printf("  true                          ");
    {
        auto r = run("true");
        assert(r.has_value());
        assert(std::get<bool>(r->value) == true);
    }
    printf("ok\n");

    printf("  false                         ");
    {
        auto r = run("false");
        assert(r.has_value());
        assert(std::get<bool>(r->value) == false);
    }
    printf("ok\n");

    // --- dispatch: number ---

    printf("  integer                       ");
    {
        auto r = run("42");
        assert(r.has_value());
        auto num = std::get<Number>(r->value);
        assert(std::get<int64_t>(num) == 42);
    }
    printf("ok\n");

    printf("  negative integer              ");
    {
        auto r = run("-7");
        assert(r.has_value());
        auto num = std::get<Number>(r->value);
        assert(std::get<int64_t>(num) == -7);
    }
    printf("ok\n");

    printf("  float                         ");
    {
        auto r = run("3.14");
        assert(r.has_value());
        auto num = std::get<Number>(r->value);
        assert(std::abs(std::get<double>(num) - 3.14) < 1e-12);
    }
    printf("ok\n");

    printf("  zero                          ");
    {
        auto r = run("0");
        assert(r.has_value());
        auto num = std::get<Number>(r->value);
        assert(std::get<int64_t>(num) == 0);
    }
    printf("ok\n");

    // --- dispatch: string ---

    printf("  string                        ");
    {
        auto r = run(R"("hello")");
        assert(r.has_value());
        assert(std::get<std::string>(r->value) == "hello");
    }
    printf("ok\n");

    printf("  empty string                  ");
    {
        auto r = run(R"("")");
        assert(r.has_value());
        assert(std::get<std::string>(r->value).empty());
    }
    printf("ok\n");

    // --- whitespace skipping ---

    printf("  leading space                 ");
    {
        auto r = run("  null");
        assert(r.has_value());
        assert(std::holds_alternative<std::nullptr_t>(r->value));
    }
    printf("ok\n");

    printf("  leading tab                   ");
    {
        auto r = run("\ttrue");
        assert(r.has_value());
        assert(std::get<bool>(r->value) == true);
    }
    printf("ok\n");

    printf("  leading newline               ");
    {
        auto r = run("\n42");
        assert(r.has_value());
        auto num = std::get<Number>(r->value);
        assert(std::get<int64_t>(num) == 42);
    }
    printf("ok\n");

    printf("  leading CR+LF                 ");
    {
        auto r = run("\r\n\"hi\"");
        assert(r.has_value());
        assert(std::get<std::string>(r->value) == "hi");
    }
    printf("ok\n");

    printf("  mixed whitespace              ");
    {
        auto r = run("  \t\n\r  false");
        assert(r.has_value());
        assert(std::get<bool>(r->value) == false);
    }
    printf("ok\n");

    // --- cursor rest ---

    printf("  rest after value              ");
    {
        auto r = run("true , next");
        assert(r.has_value());
        assert(std::get<bool>(r->value) == true);
        assert(*r->rest == ' ');
    }
    printf("ok\n");

    printf("  rest after string             ");
    {
        auto r = run(R"("key" : 1)");
        assert(r.has_value());
        assert(std::get<std::string>(r->value) == "key");
        assert(*r->rest == ' ');
    }
    printf("ok\n");

    printf("  rest after number             ");
    {
        auto r = run("123]");
        assert(r.has_value());
        auto num = std::get<Number>(r->value);
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

    printf("  unexpected open brace         ");
    {
        auto r = run("{");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  unexpected open bracket       ");
    {
        auto r = run("[");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
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
        assert(std::get<int64_t>(std::get<Number>(r->value)) == 0);
    }
    printf("ok\n");

    printf("  digit 1                       ");
    {
        auto r = run("1");
        assert(r.has_value());
        assert(std::get<int64_t>(std::get<Number>(r->value)) == 1);
    }
    printf("ok\n");

    printf("  digit 9                       ");
    {
        auto r = run("9");
        assert(r.has_value());
        assert(std::get<int64_t>(std::get<Number>(r->value)) == 9);
    }
    printf("ok\n");

    printf("  ws + each type                ");
    {
        auto r1 = run("  true");
        assert(r1 && std::get<bool>(r1->value) == true);
        auto r2 = run("  false");
        assert(r2 && std::get<bool>(r2->value) == false);
        auto r3 = run("  null");
        assert(r3 && std::holds_alternative<std::nullptr_t>(r3->value));
        auto r4 = run(R"(  "x")");
        assert(r4 && std::get<std::string>(r4->value) == "x");
        auto r5 = run("  -1");
        assert(r5 && std::get<int64_t>(std::get<Number>(r5->value)) == -1);
    }
    printf("ok\n");

    printf("  all passed\n");
    return 0;
}
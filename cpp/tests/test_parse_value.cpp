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

    printf("  all passed\n");
    return 0;
}

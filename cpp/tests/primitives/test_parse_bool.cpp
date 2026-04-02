#include "strata/primitives/parse_bool.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace strata;

static Result<bool> run(const char *input) {
    return parse<bool>(input, input + std::strlen(input));
}

int main() {
    printf("test_parse_bool:\n");

    printf("  true                          ");
    {
        auto r = run("true");
        assert(r.has_value());
        assert(r->value == true);
        assert(*r->rest == '\0');
    }
    printf("ok\n");

    printf("  false                         ");
    {
        auto r = run("false");
        assert(r.has_value());
        assert(r->value == false);
        assert(*r->rest == '\0');
    }
    printf("ok\n");

    printf("  true + trailing               ");
    {
        auto r = run("true,1");
        assert(r.has_value());
        assert(r->value == true);
        assert(*r->rest == ',');
    }
    printf("ok\n");

    printf("  false + trailing              ");
    {
        auto r = run("false}");
        assert(r.has_value());
        assert(r->value == false);
        assert(*r->rest == '}');
    }
    printf("ok\n");

    printf("  truncated \"tru\"               ");
    {
        auto r = run("tru");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidLiteral);
    }
    printf("ok\n");

    printf("  truncated \"fals\"              ");
    {
        auto r = run("fals");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidLiteral);
    }
    printf("ok\n");

    printf("  wrong word                    ");
    {
        auto r = run("yes");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidLiteral);
    }
    printf("ok\n");

    printf("  empty input                   ");
    {
        auto r = run("");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  single char                   ");
    {
        auto r = run("t");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidLiteral);
    }
    printf("ok\n");
    return 0;
}

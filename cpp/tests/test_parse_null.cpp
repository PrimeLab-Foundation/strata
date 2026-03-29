#include "strata/parse_null.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace strata;

static Result<std::nullptr_t> run(const char* input) {
    return parse<std::nullptr_t>(input, input + std::strlen(input));
}

int main() {
    printf("test_parse_null:\n");

    printf("  null                          ");
    {
        auto r = run("null");
        assert(r.has_value());
        assert(r->value == nullptr);
        assert(*r->rest == '\0');
    }
    printf("ok\n");

    printf("  null + trailing               ");
    {
        auto r = run("null,1");
        assert(r.has_value());
        assert(r->value == nullptr);
        assert(*r->rest == ',');
    }
    printf("ok\n");

    printf("  truncated \"nul\"               ");
    {
        auto r = run("nul");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  truncated \"nu\"                ");
    {
        auto r = run("nu");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");

    printf("  wrong literal                 ");
    {
        auto r = run("nope");
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
        auto r = run("n");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");
    return 0;
}
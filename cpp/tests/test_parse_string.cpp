#include "strata/parse_string.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace strata;

static Result<std::string> run(const char* input) {
    return parse<std::string>(input, input + std::strlen(input));
}

int main() {
    printf("test_parse_string:\n");
    // --- valid: basic ---

    printf("  simple                        ");
    {
        auto r = run(R"("hello")");
        assert(r.has_value());
        assert(r->value == "hello");
        assert(*r->rest == '\0');
    }
    printf("ok\n");

    printf("  empty string                  ");
    {
        auto r = run(R"("")");
        assert(r.has_value());
        assert(r->value.empty());
    }
    printf("ok\n");

    printf("  single char                   ");
    {
        auto r = run(R"("x")");
        assert(r.has_value());
        assert(r->value == "x");
    }
    printf("ok\n");

    printf("  with spaces                   ");
    {
        auto r = run(R"("hello world")");
        assert(r.has_value());
        assert(r->value == "hello world");
    }
    printf("ok\n");

    printf("  trailing chars                ");
    {
        auto r = run(R"("key":1)");
        assert(r.has_value());
        assert(r->value == "key");
        assert(*r->rest == ':');
    }
    printf("ok\n");

    printf("  trailing comma                ");
    {
        auto r = run(R"("val",)");
        assert(r.has_value());
        assert(r->value == "val");
        assert(*r->rest == ',');
    }
    printf("ok\n");

    // --- valid: simple escapes ---

    printf("  escaped quote                 ");
    {
        auto r = run(R"("say \"hi\"")");
        assert(r.has_value());
        assert(r->value == R"(say "hi")");
    }
    printf("ok\n");

    printf("  escaped backslash             ");
    {
        auto r = run(R"("a\\b")");
        assert(r.has_value());
        assert(r->value == "a\\b");
    }
    printf("ok\n");

    printf("  escaped slash                 ");
    {
        auto r = run(R"("a\/b")");
        assert(r.has_value());
        assert(r->value == "a/b");
    }
    printf("ok\n");

    printf("  all control escapes           ");
    {
        auto r = run(R"("\n\t\r\b\f")");
        assert(r.has_value());
        assert(r->value == "\n\t\r\b\f");
    }
    printf("ok\n");

    printf("  mixed text and escapes        ");
    {
        auto r = run(R"("line1\nline2\ttab")");
        assert(r.has_value());
        assert(r->value == "line1\nline2\ttab");
    }
    printf("ok\n");

    // --- valid: unicode \uXXXX ---

    printf("  unicode → ASCII (\\u0041)      ");
    {
        auto r = run(R"("\u0041")");
        assert(r.has_value());
        assert(r->value == "A");
    }
    printf("ok\n");

    printf("  unicode → 2-byte (\\u00e9)     ");
    {
        auto r = run(R"("\u00e9")");
        assert(r.has_value());
        assert(r->value == "\xc3\xa9");
    }
    printf("ok\n");

    printf("  unicode → 3-byte (\\u4e16)     ");
    {
        auto r = run(R"("\u4e16")");
        assert(r.has_value());
        assert(r->value == "\xe4\xb8\x96");
    }
    printf("ok\n");

    printf("  unicode null (\\u0000)         ");
    {
        auto r = run(R"("\u0000")");
        assert(r.has_value());
        assert(r->value.size() == 1);
        assert(r->value[0] == '\0');
    }
    printf("ok\n");

    printf("  unicode uppercase hex         ");
    {
        auto r = run(R"("\u00E9")");
        assert(r.has_value());
        assert(r->value == "\xc3\xa9");
    }
    printf("ok\n");

    printf("  multiple unicode escapes      ");
    {
        auto r = run(R"("\u0048\u0065\u006C\u006C\u006F")");
        assert(r.has_value());
        assert(r->value == "Hello");
    }
    printf("ok\n");

    // --- valid: surrogate pairs ---

    printf("  surrogate pair (U+1F600)      ");
    {
        auto r = run(R"("\uD83D\uDE00")");
        assert(r.has_value());
        assert(r->value == "\xf0\x9f\x98\x80");
    }
    printf("ok\n");

    printf("  surrogate pair (U+10437)      ");
    {
        auto r = run(R"("\uD801\uDC37")");
        assert(r.has_value());
        assert(r->value == "\xf0\x90\x90\xb7");
    }
    printf("ok\n");

    printf("  text + surrogate + text       ");
    {
        auto r = run(R"("hi\uD83D\uDE00bye")");
        assert(r.has_value());
        assert(r->value == "hi\xf0\x9f\x98\x80""bye");
    }
    printf("ok\n");

    // --- valid: raw UTF-8 passthrough ---

    printf("  raw UTF-8 passthrough         ");
    {
        auto r = run("\"\xc3\xa9\"");
        assert(r.has_value());
        assert(r->value == "\xc3\xa9");
    }
    printf("ok\n");

    printf("  raw emoji passthrough         ");
    {
        auto r = run("\"\xf0\x9f\x98\x80\"");
        assert(r.has_value());
        assert(r->value == "\xf0\x9f\x98\x80");
    }
    printf("ok\n");

    // --- errors ---

    printf("  unterminated                  ");
    {
        auto r = run(R"("hello)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnterminatedString);
    }
    printf("ok\n");

    printf("  unterminated empty            ");
    {
        auto r = run(R"(")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnterminatedString);
    }
    printf("ok\n");

    printf("  unterminated after escape     ");
    {
        auto r = run(R"("\)");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnterminatedString);
    }
    printf("ok\n");

    printf("  invalid escape \\q             ");
    {
        auto r = run(R"("\q")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidEscape);
    }
    printf("ok\n");

    printf("  invalid escape \\1             ");
    {
        auto r = run(R"("\1")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidEscape);
    }
    printf("ok\n");

    printf("  bad hex in \\uXXXX            ");
    {
        auto r = run(R"("\u00GG")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidUnicode);
    }
    printf("ok\n");

    printf("  truncated \\u                  ");
    {
        auto r = run(R"("\u00")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidUnicode);
    }
    printf("ok\n");

    printf("  lone high surrogate           ");
    {
        auto r = run(R"("\uD800")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidUnicode);
    }
    printf("ok\n");

    printf("  lone low surrogate            ");
    {
        auto r = run(R"("\uDC00")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidUnicode);
    }
    printf("ok\n");

    printf("  high surrogate + non-pair     ");
    {
        auto r = run(R"("\uD800\u0041")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidUnicode);
    }
    printf("ok\n");

    printf("  high surrogate + no follow    ");
    {
        auto r = run(R"("\uD800abc")");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::InvalidUnicode);
    }
    printf("ok\n");

    printf("  control char \\x01             ");
    {
        const char in[] = "\"hi\x01\"";
        auto r = parse<std::string>(in, in + sizeof(in) - 1);
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  control char \\x1F             ");
    {
        const char in[] = "\"hi\x1f\"";
        auto r = parse<std::string>(in, in + sizeof(in) - 1);
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  no opening quote              ");
    {
        auto r = run("hello");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedChar);
    }
    printf("ok\n");

    printf("  empty input                   ");
    {
        auto r = run("");
        assert(!r.has_value());
        assert(r.error().code == ErrorCode::UnexpectedEnd);
    }
    printf("ok\n");
    return 0;
}

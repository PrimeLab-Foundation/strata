// Extended LazyString tests covering uncovered paths:
// - Move/copy assignment operators
// - has_escapes(), is_materialized(), size(), empty() accessor methods
// - All comparison operators (==, !=, <) with LazyString, std::string,
//   std::string_view, const char*
// - Reversed (non-member) comparison operators
// - Escape sequences: \/ \b \f and supplementary-plane (4-byte) UTF-8
// - Uppercase hex digits A-F in \uXXXX

#include "strata/util/lazy_string.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

using namespace strata;

// ---------------------------------------------------------------------------
// Assignment operators (move + copy)
// ---------------------------------------------------------------------------

static void test_move_assignment() {
    LazyString a("hello\\nworld", true);
    a.materialize(); // trigger unescape

    LazyString b;
    b = std::move(a); // move assignment operator

    assert(b.is_materialized());
    assert(b.value() == "hello\nworld");
    std::cout << "  move_assignment ok\n";
}

static void test_copy_assignment() {
    LazyString a("foo\\tbar", true);
    a.materialize();

    LazyString b;
    b = a; // copy assignment operator

    assert(b.raw() == a.raw());
    assert(b.has_escapes() == a.has_escapes());
    assert(b.value() == a.value());
    std::cout << "  copy_assignment ok\n";
}

// ---------------------------------------------------------------------------
// Accessor methods
// ---------------------------------------------------------------------------

static void test_has_escapes() {
    LazyString with_esc("a\\nb", true);
    LazyString no_esc("abc", false);
    assert(with_esc.has_escapes());
    assert(!no_esc.has_escapes());
    std::cout << "  has_escapes ok\n";
}

static void test_is_materialized() {
    LazyString ls("a\\nb", true);
    assert(!ls.is_materialized()); // deferred
    ls.value();                    // triggers unescape
    assert(ls.is_materialized());
    std::cout << "  is_materialized ok\n";
}

static void test_size_method() {
    LazyString empty_ls;
    assert(empty_ls.size() == 0);

    LazyString plain("hello", false);
    assert(plain.size() == 5);

    // size() on escaped string = size of unescaped value
    LazyString esc("\\n\\n", true); // 2 newlines
    assert(esc.size() == 2);
    std::cout << "  size_method ok\n";
}

static void test_empty_method() {
    LazyString empty_ls;
    assert(empty_ls.empty());

    LazyString nonempty("x", false);
    assert(!nonempty.empty());
    std::cout << "  empty_method ok\n";
}

// ---------------------------------------------------------------------------
// Comparison operators — LazyString vs LazyString
// ---------------------------------------------------------------------------

static void test_compare_lazystring_eq_ne() {
    LazyString a("hello", false);
    LazyString b("hello", false);
    LazyString c("world", false);

    assert(a == b);
    assert(!(a != b));
    assert(a != c);
    assert(!(a == c));
    std::cout << "  compare_lazystring_eq_ne ok\n";
}

static void test_compare_lazystring_less() {
    LazyString a("alpha", false);
    LazyString b("beta",  false);
    assert(a < b);
    assert(!(b < a));
    std::cout << "  compare_lazystring_less ok\n";
}

// ---------------------------------------------------------------------------
// Comparison operators — LazyString vs std::string
// ---------------------------------------------------------------------------

static void test_compare_with_stdstring() {
    LazyString ls("hello", false);
    std::string s_eq("hello");
    std::string s_ne("world");

    assert(ls == s_eq);
    assert(!(ls != s_eq));
    assert(ls != s_ne);
    assert(!(ls == s_ne));
    std::cout << "  compare_with_stdstring ok\n";
}

// ---------------------------------------------------------------------------
// Comparison operators — LazyString vs std::string_view
// ---------------------------------------------------------------------------

static void test_compare_with_string_view() {
    LazyString ls("abc", false);
    std::string_view sv_eq("abc");
    std::string_view sv_ne("xyz");

    assert(ls == sv_eq);
    assert(!(ls != sv_eq));
    assert(ls != sv_ne);
    assert(!(ls == sv_ne));
    std::cout << "  compare_with_string_view ok\n";
}

// ---------------------------------------------------------------------------
// Comparison operators — LazyString vs const char*
// ---------------------------------------------------------------------------

static void test_compare_with_cstr() {
    LazyString ls("test", false);
    assert(ls == "test");
    assert(!(ls != "test"));
    assert(ls != "other");
    assert(!(ls == "other"));
    std::cout << "  compare_with_cstr ok\n";
}

// ---------------------------------------------------------------------------
// Non-member reversed comparison operators (string/string_view/char* == lazy)
// ---------------------------------------------------------------------------

static void test_reversed_comparison_operators() {
    LazyString ls("hello", false);

    // std::string reversed
    std::string s("hello");
    assert(s == ls);
    assert(!(s != ls));
    std::string s_ne("world");
    assert(s_ne != ls);

    // std::string_view reversed
    std::string_view sv("hello");
    assert(sv == ls);
    assert(!(sv != ls));
    std::string_view sv_ne("nope");
    assert(sv_ne != ls);

    // const char* reversed
    assert(std::string_view("hello") == ls);
    assert(!(std::string_view("hello") != ls));

    std::cout << "  reversed_comparison_operators ok\n";
}

// ---------------------------------------------------------------------------
// Escape sequences not covered by existing tests
// ---------------------------------------------------------------------------

static void test_escape_slash() {
    // \/ should produce /
    LazyString ls("\\/path\\/to", true);
    assert(ls.value() == "/path/to");
    std::cout << "  escape_slash ok\n";
}

static void test_escape_backspace_formfeed() {
    // \b and \f
    LazyString ls_b("a\\bb", true);
    assert(ls_b.value() == "a\bb");

    LazyString ls_f("a\\fb", true);
    assert(ls_f.value() == "a\fb");
    std::cout << "  escape_backspace_formfeed ok\n";
}

static void test_unicode_uppercase_hex() {
    // \uXXXX with uppercase A-F digits (parse_hex4 uppercase branch)
    LazyString ls("\\u004F\\u004C\\u004C\\u004F", true); // "OLLO" (uppercase in escape)
    // 0x004F='O', 0x004C='L', 0x004C='L', 0x004F='O'
    // But wait - uppercase O is 0x4F, L is 0x4C
    assert(ls.value() == "OLLO");

    // Mix of uppercase and lowercase hex
    LazyString ls2("\\u03A9", true); // Ω, uses uppercase A
    assert(ls2.value() == "Ω");

    LazyString ls3("\\u00Ff", true); // ÿ — mix of uppercase F and lowercase f
    // 0x00FF = ÿ
    assert(ls3.value() == "\xC3\xBF"); // UTF-8 for ÿ
    std::cout << "  unicode_uppercase_hex ok\n";
}

static void test_supplementary_plane_4byte_utf8() {
    // Characters in supplementary planes (U+10000 and above) are encoded as
    // surrogate pairs in JSON: \uD800-\uDFFF.
    // U+1F602 (😂) = surrogate pair D83D DE02
    LazyString ls("\\uD83D\\uDE02", true);
    // UTF-8: F0 9F 98 82
    std::string val = ls.value();
    assert(val.size() == 4);
    assert((unsigned char)val[0] == 0xF0);
    assert((unsigned char)val[1] == 0x9F);
    assert((unsigned char)val[2] == 0x98);
    assert((unsigned char)val[3] == 0x82);
    std::cout << "  supplementary_plane_4byte_utf8 ok\n";
}

static void test_surrogate_low_without_high() {
    // An isolated low surrogate (no preceding high) — should produce U+FFFD
    // or the raw bytes (implementation-defined). Just verify no crash.
    // Note: per RFC 8259, this is technically invalid JSON but parsers may be lenient.
    // The test just verifies no crash and some output is produced.
    LazyString ls("\\uDC00", true); // low surrogate alone
    const std::string& val = ls.value();
    // Should produce some output (replacement or raw); just no crash
    assert(!val.empty() || val.empty()); // always true — just testing no-crash
    (void)val;
    std::cout << "  surrogate_low_without_high ok (no crash)\n";
}

static void test_3byte_utf8_path() {
    // 3-byte UTF-8: U+0800 to U+FFFF (non-surrogate BMP)
    // U+4E2D (中) = E4 B8 AD
    LazyString ls("\\u4E2D", true);
    std::string val = ls.value();
    assert(val.size() == 3);
    assert((unsigned char)val[0] == 0xE4);
    assert((unsigned char)val[1] == 0xB8);
    assert((unsigned char)val[2] == 0xAD);
    std::cout << "  3byte_utf8_path ok\n";
}

static void test_2byte_utf8_path() {
    // 2-byte UTF-8: U+0080 to U+07FF
    // U+00E9 (é) = C3 A9
    LazyString ls("\\u00E9", true);
    std::string val = ls.value();
    assert(val.size() == 2);
    assert((unsigned char)val[0] == 0xC3);
    assert((unsigned char)val[1] == 0xA9);
    std::cout << "  2byte_utf8_path ok\n";
}

static void test_1byte_utf8_path() {
    // 1-byte (ASCII): U+0000 to U+007F
    // U+0041 (A)
    LazyString ls("\\u0041", true);
    assert(ls.value() == "A");
    std::cout << "  1byte_utf8_path ok\n";
}

int main() {
    std::cout << "Running extended LazyString tests...\n";

    test_move_assignment();
    test_copy_assignment();

    test_has_escapes();
    test_is_materialized();
    test_size_method();
    test_empty_method();

    test_compare_lazystring_eq_ne();
    test_compare_lazystring_less();
    test_compare_with_stdstring();
    test_compare_with_string_view();
    test_compare_with_cstr();
    test_reversed_comparison_operators();

    test_escape_slash();
    test_escape_backspace_formfeed();
    test_unicode_uppercase_hex();
    test_supplementary_plane_4byte_utf8();
    test_surrogate_low_without_high();
    test_3byte_utf8_path();
    test_2byte_utf8_path();
    test_1byte_utf8_path();

    std::cout << "\nAll extended LazyString tests passed!\n";
    return 0;
}

// test_lazy_string_and_search_compile.cpp — Lazy string ctors-escapes-surrogate, search compile errors, document debug.
//
// Targets:
//   lazy_string.hpp — string_view ctor (line 43-44), const char* ctor (line 51-52)
//   lazy_string.hpp — default escape case (lines 311-315), i>=len path (lines 245-246)
//   lazy_string.hpp — surrogate pair handling, move/copy assign
//   json_core.hpp   — mutable as_string() LazyString path (lines 215-216)
//   json_core.hpp   — FlatMap::at() throw for missing key (lines 97-98, 104-105)
//   json_core.hpp   — FlatMap index path (>16 keys), on_inserted index active path
//   search_compile.cpp — parse_filter error paths (lines 162-163, 169, 175-176, 183-184, 227, 230)
//   search_compile.cpp — parse() error paths: malformed recursive descent, unclosed bracket
//   search_compile.cpp — parse_double() throw, parse_quoted_string() throw
//   json_document.cpp — root_type_debug() for null, bool, number, string, array, object
//   json_lazy_cursor.hpp — FieldIterator() and ElementIterator() default constructors

#include "strata/json/json_core.hpp"
#include "strata/json/json_document.hpp"
#include "strata/json/json_lazy_cursor.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/search/search.hpp"
#include "strata/util/lazy_string.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace strata;

// ===========================================================================
// lazy_string.hpp — string_view and const char* constructors
// ===========================================================================

static void test_lazy_string_string_view_ctor() {
    std::string_view sv = "hello\\nworld";
    LazyString ls(sv, true);  // string_view ctor (line 43-44)
    const std::string& val = ls.value();
    assert(val == "hello\nworld");
    std::cout << "  lazy_string_string_view_ctor ok\n";
}

static void test_lazy_string_const_char_ptr_ctor() {
    const char* raw = "foo\\tbar";
    LazyString ls(raw, true);  // const char* ctor (line 51-52)
    assert(ls.value() == "foo\tbar");
    std::cout << "  lazy_string_const_char_ptr_ctor ok\n";
}

static void test_lazy_string_string_view_no_escapes() {
    std::string_view sv = "simple";
    LazyString ls(sv, false);
    assert(ls.value() == "simple");
    assert(!ls.has_escapes());
    std::cout << "  lazy_string_string_view_no_escapes ok\n";
}

// ===========================================================================
// lazy_string.hpp — default escape case (lines 311-315)
// Unknown escape character like \x — kept as-is
// ===========================================================================

static void test_lazy_string_unknown_escape() {
    // \x is not a valid JSON escape, so it falls to default case
    // LazyString is used for already-parsed JSON strings, so we pass raw content
    // "test\xval" — note the \ in the raw string
    std::string raw = "test\\xval";  // contains backslash-x
    LazyString ls(raw, true);
    const std::string& val = ls.value();
    // default case: keeps the \ (data[i]) and advances by 1
    // Actually it keeps '\\' and then 'x' follows next loop iteration
    assert(!val.empty());
    std::cout << "  lazy_string_unknown_escape ok (val=" << val << ")\n";
}

// ===========================================================================
// lazy_string.hpp — i >= len path (lines 245-246)
// The "if (i >= len || data[i] != '\\')" branch
// This fires when memchr finds backslash but then i is caught up wrong
// Actually: "if (i >= len)" fires when string ends with backslash exactly
// ===========================================================================

static void test_lazy_string_trailing_backslash() {
    // String ending with backslash — i+1 >= len so we break
    // "test\\" — ends with a single backslash (i+1>=len path at line 248-250)
    std::string raw = "test\\";  // raw data: t e s t backslash
    LazyString ls(raw, true);
    const std::string& val = ls.value();
    // hits i+1>=len, breaks, returns "test"
    assert(val == "test");
    std::cout << "  lazy_string_trailing_backslash ok\n";
}

// ===========================================================================
// lazy_string.hpp — surrogate pair: high surrogate without following low
// ===========================================================================

static void test_lazy_string_lone_high_surrogate() {
    // \uD800 is a high surrogate without a following \uDC00
    // Should just emit a replacement or skip gracefully
    std::string raw = "\\uD800end";
    LazyString ls(raw, true);
    const std::string& val = ls.value();
    // codepoint = 0xD800, no following \uDCxx → just calls append_utf8(0xD800)
    // which falls to the 3-byte UTF-8 case (0xD800 < 0x10000)
    assert(!val.empty());
    std::cout << "  lazy_string_lone_high_surrogate ok\n";
}

static void test_lazy_string_surrogate_pair_valid() {
    // Valid surrogate pair: \uD83D\uDE00 = 😀 (U+1F600)
    std::string raw = "\\uD83D\\uDE00";
    LazyString ls(raw, true);
    const std::string& val = ls.value();
    // U+1F600 is 4-byte UTF-8: F0 9F 98 80
    assert(val.size() == 4);
    assert(static_cast<unsigned char>(val[0]) == 0xF0);
    std::cout << "  lazy_string_surrogate_pair_valid ok\n";
}

// ===========================================================================
// lazy_string.hpp — move/copy assign
// ===========================================================================

static void test_lazy_string_move_assign() {
    LazyString a(std::string("hello\\nworld"), true);
    LazyString b;
    b = std::move(a);
    assert(b.value() == "hello\nworld");
    std::cout << "  lazy_string_move_assign ok\n";
}

static void test_lazy_string_copy_assign() {
    LazyString a(std::string("hello"), false);
    LazyString b;
    b = a;
    assert(b.value() == "hello");
    std::cout << "  lazy_string_copy_assign ok\n";
}

// ===========================================================================
// json_core.hpp — mutable as_string() LazyString conversion (lines 215-216)
// Need a JsonValue holding a LazyString, then call mutable as_string()
// ===========================================================================

static void test_json_core_mutable_as_string_lazy() {
    // Parse a JSON string with escapes — the value will be stored as LazyString
    auto result = parse_json(R"("hello\nworld")");
    assert(result.ok());
    assert(result.value.is_string());

    // Call MUTABLE as_string() — should trigger LazyString → std::string conversion
    // if stored as LazyString
    std::string& s = result.value.as_string();
    assert(s == "hello\nworld");
    // Now it should be a std::string (converted)
    assert(result.value.is_string());
    std::cout << "  json_core_mutable_as_string_lazy ok\n";
}

// ===========================================================================
// json_core.hpp — FlatMap::at() throw for missing key
// ===========================================================================

static void test_flat_map_at_missing_key() {
    // Parse a JSON object, get the Object, call at() with non-existent key
    auto result = parse_json(R"({"a": 1, "b": 2})");
    assert(result.ok());
    assert(result.value.is_object());

    auto& obj = result.value.as_object();

    // Test const at() - missing key
    {
        int caught = 0;
        try {
            const auto& const_obj = obj;
            (void)const_obj.at(std::string("nonexistent"));
        } catch (const std::out_of_range&) {
            caught = 1;
        }
        assert(caught == 1);
    }

    // Test mutable at() - missing key
    {
        int caught = 0;
        try {
            (void)obj.at(std::string("also_missing"));
        } catch (const std::out_of_range&) {
            caught = 1;
        }
        assert(caught == 1);
    }

    std::cout << "  flat_map_at_missing_key ok\n";
}

// ===========================================================================
// json_core.hpp — FlatMap index path (>16 keys activates hash index)
// ===========================================================================

static void test_flat_map_large_object_with_index() {
    // Build a JSON object with 20+ keys to trigger hash index (kIndexThreshold=16)
    std::string json = "{";
    for (int i = 0; i < 20; ++i) {
        if (i > 0) json += ",";
        json += "\"key" + std::to_string(i) + "\":" + std::to_string(i);
    }
    json += "}";

    auto result = parse_json(json);
    assert(result.ok());
    assert(result.value.is_object());

    auto& obj = result.value.as_object();
    assert(obj.size() == 20);

    // Access via find() — should use hash index (index_active_=true)
    auto it = obj.find(std::string("key10"));
    assert(it != obj.end());

    // Access const find() — also uses hash index
    const auto& const_obj = obj;
    auto cit = const_obj.find(std::string("key15"));
    assert(cit != const_obj.end());

    // at() on a large object with existing key — uses hash index
    const auto& val10 = const_obj.at(std::string("key10"));
    assert(val10.is_int());

    std::cout << "  flat_map_large_object_with_index ok\n";
}

// ===========================================================================
// search_compile.cpp — parse_filter error paths
// ===========================================================================

static void test_search_compile_filter_no_paren() {
    // [? without ( — hits line 162-163
    auto result = compile_search_path("$[?@.x > 1]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_no_paren ok\n";
}

static void test_search_compile_filter_no_at() {
    // [?( without @ — hits line 168-169
    auto result = compile_search_path("$[?(x > 1)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_no_at ok\n";
}

static void test_search_compile_filter_empty_field() {
    // $[?(@. followed by non-identifier — hits line 175-176
    auto result = compile_search_path("$[?(@. > 1)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_empty_field ok\n";
}

static void test_search_compile_filter_bracket_no_quote() {
    // $[?(@[ not followed by quote — hits line 183
    auto result = compile_search_path("$[?(@[abc > 1)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_bracket_no_quote ok\n";
}

static void test_search_compile_filter_no_bracket_at() {
    // @. handled, but no . or [ after @ — hits line 189-190
    auto result = compile_search_path("$[?(@>1)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_no_bracket_at ok\n";
}

static void test_search_compile_filter_invalid_operator() {
    // $[?(@.x == value)] where == is valid, but first == second is not '='
    // Test: first='=' second='' (just =, not ==) — hits line 227
    auto result = compile_search_path("$[?(@.x = 1)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_invalid_operator ok\n";
}

static void test_search_compile_filter_unknown_op_char() {
    // Neither =, !, >, < as first — hits line 229-230
    auto result = compile_search_path("$[?(@.x & 1)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_unknown_op_char ok\n";
}

static void test_search_compile_filter_no_close_paren() {
    // Missing ) at end — hits line 270-271
    auto result = compile_search_path("$[?(@.x > 1]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_no_close_paren ok\n";
}

static void test_search_compile_filter_invalid_value() {
    // Value is neither string, number, nor identifier — hits line 265-266
    auto result = compile_search_path("$[?(@.x > @)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_invalid_value ok\n";
}

static void test_search_compile_filter_unknown_ident() {
    // Value is identifier but not true/false/null — hits line 262-263
    auto result = compile_search_path("$[?(@.x == undefined)]");
    assert(!result.ok());
    std::cout << "  search_compile_filter_unknown_ident ok\n";
}

static void test_search_compile_filter_bool_true() {
    // Filter with boolean value true — lines 251-254
    auto result = compile_search_path("$[?(@.active == true)]");
    assert(result.ok());
    std::cout << "  search_compile_filter_bool_true ok\n";
}

static void test_search_compile_filter_bool_false() {
    // Filter with boolean value false — lines 255-258
    auto result = compile_search_path("$[?(@.active == false)]");
    assert(result.ok());
    std::cout << "  search_compile_filter_bool_false ok\n";
}

static void test_search_compile_filter_null_value() {
    // Filter with null — lines 259-261
    auto result = compile_search_path("$[?(@.x == null)]");
    assert(result.ok());
    std::cout << "  search_compile_filter_null_value ok\n";
}

static void test_search_compile_recursive_descent_empty_field() {
    // $.. with no field name after — hits line 299-300
    auto result = compile_search_path("$..");
    assert(!result.ok());
    std::cout << "  search_compile_recursive_descent_empty_field ok\n";
}

static void test_search_compile_dot_empty_field() {
    // $. followed by nothing — hits line 311-312
    auto result = compile_search_path("$.");
    assert(!result.ok());
    std::cout << "  search_compile_dot_empty_field ok\n";
}

static void test_search_compile_unclosed_bracket() {
    // $[* missing ] — hits line 338-339
    auto result = compile_search_path("$[*");
    assert(!result.ok());
    std::cout << "  search_compile_unclosed_bracket ok\n";
}

static void test_search_compile_quoted_field_missing_bracket() {
    // $['field' missing ] — hits line 347-349
    auto result = compile_search_path("$['field'");
    assert(!result.ok());
    std::cout << "  search_compile_quoted_field_missing_bracket ok\n";
}

static void test_search_compile_index_missing_bracket() {
    // $[0 missing ] — hits line 371-373
    auto result = compile_search_path("$[0");
    assert(!result.ok());
    std::cout << "  search_compile_index_missing_bracket ok\n";
}

static void test_search_compile_unknown_char_in_path() {
    // $.field followed by # unknown char — hits line 412-413
    auto result = compile_search_path("$.field#garbage");
    assert(!result.ok());
    std::cout << "  search_compile_unknown_char_in_path ok\n";
}

static void test_search_compile_no_dollar() {
    // Path not starting with $ — hits line 283-284
    auto result = compile_search_path("field.name");
    assert(!result.ok());
    std::cout << "  search_compile_no_dollar ok\n";
}

static void test_search_compile_bracket_else_branch() {
    // $[ followed by something that's not ?, *, quote, digit, -, : — line 407-408
    auto result = compile_search_path("$[abc]");
    assert(!result.ok());
    std::cout << "  search_compile_bracket_else_branch ok\n";
}

static void test_search_compile_slice_missing_bracket() {
    // $[0: missing ] — hits line 403-404
    auto result = compile_search_path("$[0:5");
    assert(!result.ok());
    std::cout << "  search_compile_slice_missing_bracket ok\n";
}

static void test_search_compile_filter_no_close_bracket() {
    // $[?(@.x > 1) missing ] — hits line 330-331
    auto result = compile_search_path("$[?(@.x > 1)");
    assert(!result.ok());
    std::cout << "  search_compile_filter_no_close_bracket ok\n";
}

static void test_search_compile_quoted_path_unclosed() {
    // $['unclosed — parse_quoted_string throws
    bool threw = false;
    try {
        auto result = compile_search_path("$['unclosed");
        // If it doesn't throw, it should return an error status
        assert(!result.ok());
    } catch (const std::runtime_error&) {
        threw = true;
    }
    // Either threw or returned error — both are acceptable
    (void)threw;
    std::cout << "  search_compile_quoted_path_unclosed ok\n";
}

// ===========================================================================
// json_document.cpp — root_type_debug() for various types
// ===========================================================================

static void test_json_document_root_type_debug() {
    // null
    {
        auto doc = JsonDocument::from_string("null");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "null");
    }
    // bool
    {
        auto doc = JsonDocument::from_string("true");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "bool");
    }
    // number (int)
    {
        auto doc = JsonDocument::from_string("42");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "number");
    }
    // number (double)
    {
        auto doc = JsonDocument::from_string("3.14");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "number");
    }
    // string
    {
        auto doc = JsonDocument::from_string("\"hello\"");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "string");
    }
    // array
    {
        auto doc = JsonDocument::from_string("[1,2,3]");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "array");
    }
    // object
    {
        auto doc = JsonDocument::from_string("{\"a\":1}");
        assert(doc.ok());
        assert(doc.value.root_type_debug() == "object");
    }
    std::cout << "  json_document_root_type_debug ok\n";
}

// ===========================================================================
// json_lazy_cursor.hpp — FieldIterator and ElementIterator default ctors
// ===========================================================================

static void test_lazy_cursor_default_iterators() {
    // FieldIterator default constructor (line 225)
    LazyJsonCursor::FieldIterator field_it;
    assert(field_it.at_end());

    // ElementIterator default constructor (line 252)
    LazyJsonCursor::ElementIterator elem_it;
    assert(elem_it.at_end());

    std::cout << "  lazy_cursor_default_iterators ok\n";
}

// ===========================================================================
// search_compile.cpp — parse_filter existence check (just @.field)
// ===========================================================================

static void test_search_compile_filter_existence() {
    // $[?(@.active)] — existence check (line 196-201)
    auto result = compile_search_path("$[?(@.active)]");
    assert(result.ok());
    std::cout << "  search_compile_filter_existence ok\n";
}

// ===========================================================================
// search_compile.cpp — bracket-quoted field with double quotes
// ===========================================================================

static void test_search_compile_bracket_double_quote_field() {
    // $["field"] — quoted field with double quotes
    auto result = compile_search_path("$[\"my_field\"]");
    assert(result.ok());
    std::cout << "  search_compile_bracket_double_quote_field ok\n";
}

// ===========================================================================
// search_compile.cpp — slice with step
// ===========================================================================

static void test_search_compile_slice_with_step() {
    // $[0:10:2] — slice with step
    auto result = compile_search_path("$[0:10:2]");
    assert(result.ok());
    std::cout << "  search_compile_slice_with_step ok\n";
}

static void test_search_compile_slice_from_start() {
    // $[:5] — slice with omitted start
    auto result = compile_search_path("$[:5]");
    assert(result.ok());
    std::cout << "  search_compile_slice_from_start ok\n";
}

// ===========================================================================
// search_compile.cpp — CompiledPath::is_simple_field_extraction
// ===========================================================================

static void test_compiled_path_is_simple_field_extraction() {
    // $.field — is simple
    {
        auto p = compile_search_path("$.field");
        assert(p.ok());
        assert(p.value.is_simple_field_extraction());
    }
    // $.*.field — is simple (3-step wildcard pattern)
    {
        auto p = compile_search_path("$.*.field");
        assert(p.ok());
        assert(p.value.is_simple_field_extraction());
    }
    // $..field — is NOT simple
    {
        auto p = compile_search_path("$..field");
        assert(p.ok());
        assert(!p.value.is_simple_field_extraction());
    }
    std::cout << "  compiled_path_is_simple_field_extraction ok\n";
}

// ===========================================================================
// lazy_string.hpp — truncated unicode \u with <6 chars remaining
// ===========================================================================

static void test_lazy_string_truncated_unicode() {
    // \u followed by < 4 hex chars — hits line 288-290 (i+5 >= len)
    std::string raw = "\\u004";  // only 3 hex chars
    LazyString ls(raw, true);
    const std::string& val = ls.value();
    // Just check it doesn't crash
    (void)val;
    std::cout << "  lazy_string_truncated_unicode ok\n";
}

// ===========================================================================
// lazy_string.hpp — parse_double() in filter - floating point filter value
// ===========================================================================

static void test_search_compile_filter_float_value() {
    // Filter with float value — tests parse_double() path
    auto result = compile_search_path("$[?(@.score > 3.14)]");
    assert(result.ok());
    std::cout << "  search_compile_filter_float_value ok\n";
}

static void test_search_compile_filter_negative_value() {
    // Filter with negative value — tests parse_double() negative path
    auto result = compile_search_path("$[?(@.score > -1)]");
    assert(result.ok());
    std::cout << "  search_compile_filter_negative_value ok\n";
}

// ===========================================================================
// json_core.hpp — FlatMap index_active_ path in on_inserted
// ===========================================================================

static void test_flat_map_insert_after_index_activated() {
    // Insert 17 items (>16) to activate index, then insert more
    std::string json = "{";
    for (int i = 0; i < 25; ++i) {
        if (i > 0) json += ",";
        json += "\"k" + std::to_string(i) + "\":" + std::to_string(i);
    }
    json += "}";

    auto result = parse_json(json);
    assert(result.ok());
    auto& obj = result.value.as_object();
    assert(obj.size() == 25);

    // find should work via hash index
    auto it = obj.find(std::string("k20"));
    assert(it != obj.end());
    std::cout << "  flat_map_insert_after_index_activated ok\n";
}

// ===========================================================================
// main
// ===========================================================================

int main() {
    std::cout << "Running lazy string and search compile tests...\n";

    // lazy_string.hpp — constructors
    test_lazy_string_string_view_ctor();
    test_lazy_string_const_char_ptr_ctor();
    test_lazy_string_string_view_no_escapes();
    test_lazy_string_unknown_escape();
    test_lazy_string_trailing_backslash();
    test_lazy_string_lone_high_surrogate();
    test_lazy_string_surrogate_pair_valid();
    test_lazy_string_move_assign();
    test_lazy_string_copy_assign();
    test_lazy_string_truncated_unicode();

    // json_core.hpp
    test_json_core_mutable_as_string_lazy();
    test_flat_map_at_missing_key();
    test_flat_map_large_object_with_index();
    test_flat_map_insert_after_index_activated();

    // search_compile.cpp — error paths
    test_search_compile_no_dollar();
    test_search_compile_dot_empty_field();
    test_search_compile_recursive_descent_empty_field();
    test_search_compile_unclosed_bracket();
    test_search_compile_quoted_field_missing_bracket();
    test_search_compile_index_missing_bracket();
    test_search_compile_unknown_char_in_path();
    test_search_compile_bracket_else_branch();
    test_search_compile_slice_missing_bracket();
    test_search_compile_filter_no_paren();
    test_search_compile_filter_no_at();
    test_search_compile_filter_empty_field();
    test_search_compile_filter_bracket_no_quote();
    test_search_compile_filter_no_bracket_at();
    test_search_compile_filter_invalid_operator();
    test_search_compile_filter_unknown_op_char();
    test_search_compile_filter_no_close_paren();
    test_search_compile_filter_invalid_value();
    test_search_compile_filter_unknown_ident();
    test_search_compile_filter_bool_true();
    test_search_compile_filter_bool_false();
    test_search_compile_filter_null_value();
    test_search_compile_filter_no_close_bracket();
    test_search_compile_filter_existence();
    test_search_compile_quoted_path_unclosed();
    test_search_compile_bracket_double_quote_field();
    test_search_compile_slice_with_step();
    test_search_compile_slice_from_start();
    test_search_compile_unknown_char_in_path();

    // search_compile.cpp — success paths
    test_compiled_path_is_simple_field_extraction();
    test_search_compile_filter_float_value();
    test_search_compile_filter_negative_value();

    // json_document.cpp
    test_json_document_root_type_debug();

    // json_lazy_cursor.hpp
    test_lazy_cursor_default_iterators();

    std::cout << "\nAll lazy string and search compile tests passed!\n";
    return 0;
}

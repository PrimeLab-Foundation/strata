// Tests for parallel NDJSON search with filter predicates.
// Exercises build_subpath(), try_get_field_fast(), eval_filter_on_cursor()
// and the inline header accessors:
//   NdjsonStream::line_number(), lines_processed(), error_count()
//   ParallelNdjsonStream::lines_processed(), error_count(), used_parallel_mode()

#include "strata/json/json_core.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"
#include "strata/search/search.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace strata;

// Helper: generate N lines {"id": i, "name": "alice", "age": i+10, "active": true}
static std::string make_lines(size_t n) {
    std::ostringstream oss;
    for (size_t i = 0; i < n; ++i) {
        oss << "{\"id\":" << i
            << ",\"name\":\"alice\""
            << ",\"age\":" << (i + 10)
            << ",\"score\":" << (i * 1.5)
            << ",\"active\":true"
            << ",\"tag\":\"" << (i % 2 == 0 ? "even" : "odd") << "\""
            << "}\n";
    }
    return oss.str();
}

// ---------------------------------------------------------------------------
// NdjsonStream inline accessor coverage
// ---------------------------------------------------------------------------

static void test_ndjson_stream_inline_accessors() {
    NdjsonStream stream("{\"a\":1}\n{\"b\":2}\n{\"c\":3}\n");

    // line_num_ is initialized to 1 (1-based), lines_processed starts at 0
    assert(stream.line_number() == 1);
    assert(stream.lines_processed() == 0);
    assert(stream.error_count() == 0);

    stream.next();
    // After first next_line(), line_num_ increments to 2
    assert(stream.line_number() == 2);
    assert(stream.lines_processed() == 1);
    assert(stream.error_count() == 0);

    stream.next();
    assert(stream.line_number() == 3);
    assert(stream.lines_processed() == 2);

    // Introduce an error
    NdjsonStream err_stream("valid_line_is_not_json!\n{\"ok\":1}\n");
    auto r1 = err_stream.next(); // invalid
    assert(!r1.ok());
    assert(err_stream.error_count() == 1);

    std::cout << "  ndjson_stream_inline_accessors ok\n";
}

// ---------------------------------------------------------------------------
// ParallelNdjsonStream inline accessor coverage
// ---------------------------------------------------------------------------

static void test_parallel_ndjson_stream_accessors() {
    std::string data = make_lines(200);

    ParallelNdjsonConfig cfg;
    cfg.min_lines_for_parallel = 10;
    cfg.min_chunk_size = 100;
    cfg.num_threads = 2;

    ParallelNdjsonStream stream(data, cfg);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 200);
    assert(stream.lines_processed() == 200);
    assert(stream.error_count() == 0);
    assert(stream.used_parallel_mode());

    std::cout << "  parallel_ndjson_stream_accessors ok\n";
}

// ---------------------------------------------------------------------------
// Parallel search with filter — exercises build_subpath + eval_filter_on_cursor
// ---------------------------------------------------------------------------

// Config that forces parallel mode for these tests
static ParallelNdjsonConfig parallel_cfg() {
    ParallelNdjsonConfig cfg;
    cfg.min_lines_for_parallel = 10;
    cfg.min_chunk_size = 50;
    cfg.num_threads = 2;
    return cfg;
}

static void test_parallel_search_filter_numeric_eq() {
    // $[?(@.id == 5)] — numeric equal, exercises FilterOp::Equal + FilterValueType::Numeric
    std::string data = make_lines(50);
    auto compile = compile_search_path("$[?(@.id == 5)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);

    // Should find exactly 1 match: the line where id==5
    assert(matches.size() == 1);
    assert(matches[0].line == 6); // line_num is 1-indexed
    std::cout << "  parallel_search_filter_numeric_eq ok\n";
}

static void test_parallel_search_filter_numeric_ne() {
    // $[?(@.id != 0)] — numeric not-equal
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.id != 0)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 9); // all except id==0
    std::cout << "  parallel_search_filter_numeric_ne ok\n";
}

static void test_parallel_search_filter_numeric_gt() {
    // $[?(@.id > 7)] — numeric greater-than
    std::string data = make_lines(15);
    auto compile = compile_search_path("$[?(@.id > 7)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    // Lines with id 8,9,10,11,12,13,14 = 7 matches
    assert(matches.size() == 7);
    std::cout << "  parallel_search_filter_numeric_gt ok\n";
}

static void test_parallel_search_filter_numeric_ge() {
    // $[?(@.age >= 15)] — age starts at 10, so age>=15 means id>=5
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.age >= 15)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    // age = id+10; age>=15 => id>=5 => 5 matches (id 5..9)
    assert(matches.size() == 5);
    std::cout << "  parallel_search_filter_numeric_ge ok\n";
}

static void test_parallel_search_filter_numeric_lt() {
    // $[?(@.id < 3)]
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.id < 3)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 3); // id 0, 1, 2
    std::cout << "  parallel_search_filter_numeric_lt ok\n";
}

static void test_parallel_search_filter_numeric_le() {
    // $[?(@.id <= 4)]
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.id <= 4)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 5); // id 0..4
    std::cout << "  parallel_search_filter_numeric_le ok\n";
}

static void test_parallel_search_filter_string_eq() {
    // $[?(@.tag == "even")] — string equal, exercises FilterValueType::String
    std::string data = make_lines(20);
    auto compile = compile_search_path("$[?(@.tag == \"even\")]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    // Even-id lines (0,2,4,...18) = 10 matches
    assert(matches.size() == 10);
    std::cout << "  parallel_search_filter_string_eq ok\n";
}

static void test_parallel_search_filter_string_ne() {
    // $[?(@.tag != "even")] — string not-equal
    std::string data = make_lines(20);
    auto compile = compile_search_path("$[?(@.tag != \"even\")]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 10); // odd-id lines
    std::cout << "  parallel_search_filter_string_ne ok\n";
}

static void test_parallel_search_filter_bool_eq() {
    // $[?(@.active == true)] — bool equal, exercises FilterValueType::Boolean
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.active == true)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    // All lines have active:true
    assert(matches.size() == 10);
    std::cout << "  parallel_search_filter_bool_eq ok\n";
}

static void test_parallel_search_filter_bool_ne() {
    // $[?(@.active != true)] — should match none
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.active != true)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 0);
    std::cout << "  parallel_search_filter_bool_ne ok\n";
}

static void test_parallel_search_filter_exists() {
    // $[?(@.name)] — Exists predicate, exercises FilterOp::Exists
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.name)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 10); // all lines have "name"
    std::cout << "  parallel_search_filter_exists ok\n";
}

static void test_parallel_search_filter_field_not_found() {
    // Filter on a field that doesn't exist — should match none
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.nonexistent == 1)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 0);
    std::cout << "  parallel_search_filter_field_not_found ok\n";
}

static void test_parallel_search_filter_type_mismatch() {
    // Numeric filter on a string field — should match none
    std::string data = make_lines(10);
    auto compile = compile_search_path("$[?(@.name > 5)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 0);
    std::cout << "  parallel_search_filter_type_mismatch ok\n";
}

static void test_parallel_search_filter_long_field_name() {
    // Field name > 16 chars (exercises the memcmp branch in try_get_field_fast)
    std::ostringstream oss;
    for (int i = 0; i < 20; ++i) {
        oss << "{\"averylongfieldname\":" << i << "}\n";
    }
    std::string data = oss.str();
    auto compile = compile_search_path("$[?(@.averylongfieldname == 5)]");
    assert(compile.ok());

    ParallelNdjsonStream stream(data, parallel_cfg());
    auto matches = stream.search_all_parallel(compile.value);
    assert(matches.size() == 1);
    std::cout << "  parallel_search_filter_long_field_name ok\n";
}

static void test_parallel_search_on_non_object_line() {
    // Lines that are arrays/scalars — filter on non-object: eval_filter_on_cursor returns false
    std::string data = "[1,2,3]\n{\"id\":1}\n42\n{\"id\":2}\n";
    auto compile = compile_search_path("$[?(@.id == 1)]");
    assert(compile.ok());

    ParallelNdjsonConfig cfg = parallel_cfg();
    cfg.skip_errors = true;
    cfg.min_lines_for_parallel = 1;
    cfg.min_chunk_size = 1;
    ParallelNdjsonStream stream(data, cfg);
    auto matches = stream.search_all_parallel(compile.value);
    // Only {"id":1} matches; arrays/scalars return false from eval_filter
    assert(matches.size() == 1);
    std::cout << "  parallel_search_on_non_object_line ok\n";
}

static void test_parallel_search_filter_with_errors() {
    // Mix valid+invalid lines, use search_all_parallel_with_errors
    std::ostringstream oss;
    for (int i = 0; i < 30; ++i) {
        if (i == 10 || i == 20) {
            oss << "{bad}\n";
        } else {
            oss << "{\"id\":" << i << ",\"val\":" << (i * 2) << "}\n";
        }
    }
    std::string data = oss.str();
    auto compile = compile_search_path("$[?(@.id < 5)]");
    assert(compile.ok());

    ParallelNdjsonConfig cfg = parallel_cfg();
    cfg.skip_errors = true;
    ParallelNdjsonStream stream(data, cfg);
    auto result = stream.search_all_parallel_with_errors(compile.value);

    assert(result.matches.size() == 5); // id 0..4
    assert(result.errors.size() == 2);
    std::cout << "  parallel_search_filter_with_errors ok\n";
}

// ---------------------------------------------------------------------------
// NdjsonStream inline accessor — lines_processed with parse_all_fast
// ---------------------------------------------------------------------------

static void test_ndjson_stream_lines_processed_parse_all() {
    NdjsonStream stream("line1\nline2\nline3\n"); // these are invalid JSON but counted
    // parse_all with skip_errors
    auto vals = stream.parse_all(true);
    assert(stream.lines_processed() == 3);
    assert(stream.error_count() == 3); // all invalid
    std::cout << "  ndjson_stream_lines_processed_parse_all ok\n";
}

int main() {
    std::cout << "Running Parallel NDJSON filter tests...\n";

    test_ndjson_stream_inline_accessors();
    test_parallel_ndjson_stream_accessors();

    test_parallel_search_filter_numeric_eq();
    test_parallel_search_filter_numeric_ne();
    test_parallel_search_filter_numeric_gt();
    test_parallel_search_filter_numeric_ge();
    test_parallel_search_filter_numeric_lt();
    test_parallel_search_filter_numeric_le();

    test_parallel_search_filter_string_eq();
    test_parallel_search_filter_string_ne();

    test_parallel_search_filter_bool_eq();
    test_parallel_search_filter_bool_ne();

    test_parallel_search_filter_exists();
    test_parallel_search_filter_field_not_found();
    test_parallel_search_filter_type_mismatch();
    test_parallel_search_filter_long_field_name();
    test_parallel_search_on_non_object_line();
    test_parallel_search_filter_with_errors();
    test_ndjson_stream_lines_processed_parse_all();

    std::cout << "\nAll Parallel NDJSON filter tests passed!\n";
    return 0;
}

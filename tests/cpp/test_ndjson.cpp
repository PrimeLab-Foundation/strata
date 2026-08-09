/**
 * @file test_ndjson.cpp
 * @brief Contract suite for NdjsonStream.
 *
 * Pins line splitting (blank lines, CRLF, a missing final newline), the
 * end-of-stream convention, and the skip_errors contract that the Python
 * `load` is built on: a malformed line stops the parse unless the caller
 * opted in to losing it (docs/context/api.md).
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/ndjson_stream.hpp"

#include <cassert>
#include <cstdio>
#include <string>

using strata::NdjsonStream;
using strata::Status;

namespace {

void test_reads_every_line() {
    const std::string text = "{\"a\":1}\n{\"a\":2}\n{\"a\":3}\n";
    NdjsonStream stream(text);

    for (int expected = 1; expected <= 3; ++expected) {
        assert(stream.has_next());
        auto record = stream.next();
        assert(record.ok());
        assert(record.value.as_object().at("a").as_number() == expected);
    }
    assert(!stream.has_next());
}

/// End of stream reuses KeyNotFound, deliberately, as the marker.
void test_end_of_stream_is_key_not_found() {
    NdjsonStream stream("{\"a\":1}\n");
    assert(stream.next().ok());
    const auto done = stream.next();
    assert(!done.ok());
    assert(done.status == Status::KeyNotFound);
    // Asking again keeps saying the same thing.
    assert(stream.next().status == Status::KeyNotFound);
}

void test_line_endings_and_blank_lines() {
    // CRLF, a missing final newline, blank and whitespace-only lines.
    const std::string text = "{\"a\":1}\r\n\n   \n{\"a\":2}\r\n\t\n{\"a\":3}";
    NdjsonStream stream(text);

    size_t seen = 0;
    for (;;) {
        auto record = stream.next();
        if (record.status == Status::KeyNotFound)
            break;
        assert(record.ok());
        ++seen;
    }
    assert(seen == 3);
}

void test_empty_input_yields_nothing() {
    for (const char* text : {"", "\n", "  \n\n  \n"}) {
        NdjsonStream stream(text);
        assert(!stream.has_next());
        assert(stream.next().status == Status::KeyNotFound);
    }
}

void test_a_malformed_line_reports_a_parse_error() {
    const std::string text = "{\"a\":1}\n{oops}\n{\"a\":3}\n";
    NdjsonStream stream(text);
    assert(stream.next().ok());
    assert(stream.next().status == Status::ParseError);
    // The stream has moved past the bad line, so reading can continue.
    assert(stream.next().ok());
}

void test_parse_all_stops_at_the_first_bad_line_by_default() {
    const std::string text = "{\"a\":1}\n{oops}\n{\"a\":3}\n";
    NdjsonStream stream(text);

    size_t failed_line = 0;
    const auto result = stream.parse_all(/*skip_errors=*/false, &failed_line);
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    assert(failed_line == 2); // 1-based, counting blank lines as lines
}

void test_parse_all_can_be_told_to_skip_bad_lines() {
    const std::string text = "{\"a\":1}\n{oops}\n{\"a\":3}\nnot json\n{\"a\":5}\n";
    NdjsonStream stream(text);

    size_t failed_line = 0;
    const auto result = stream.parse_all(/*skip_errors=*/true, &failed_line);
    assert(result.ok());
    assert(failed_line == 0);
    assert(result.value.size() == 3);
    assert(result.value[0].as_object().at("a").as_number() == 1);
    assert(result.value[1].as_object().at("a").as_number() == 3);
    assert(result.value[2].as_object().at("a").as_number() == 5);
}

void test_parse_all_on_clean_input() {
    const std::string text = "1\n\"two\"\n[3]\n{\"four\":4}\nnull\n";
    NdjsonStream stream(text);
    const auto result = stream.parse_all(false);
    assert(result.ok());
    assert(result.value.size() == 5);
    assert(result.value[0].as_number() == 1);
    assert(result.value[1].as_string() == "two");
    assert(result.value[2].as_array().size() == 1);
    assert(result.value[3].as_object().size() == 1);
    assert(result.value[4].is_null());
}

/// Each line is a whole document: trailing content on a line is still invalid.
void test_a_line_must_be_one_complete_document() {
    NdjsonStream stream("{\"a\":1} trailing\n");
    assert(stream.next().status == Status::ParseError);
}

void test_line_numbers_count_every_line() {
    NdjsonStream stream("{\"a\":1}\n\n{\"a\":2}\n");
    assert(stream.next().ok());
    assert(stream.line_number() == 1);
    assert(stream.next().ok());
    assert(stream.line_number() == 3); // the blank line counted
}

void test_many_lines() {
    std::string text;
    for (int index = 0; index < 10000; ++index)
        text += "{\"i\":" + std::to_string(index) + "}\n";

    NdjsonStream stream(text);
    const auto result = stream.parse_all(false);
    assert(result.ok());
    assert(result.value.size() == 10000);
    assert(result.value[9999].as_object().at("i").as_number() == 9999);
}

} // namespace

int main() {
    test_reads_every_line();
    test_end_of_stream_is_key_not_found();
    test_line_endings_and_blank_lines();
    test_empty_input_yields_nothing();
    test_a_malformed_line_reports_a_parse_error();
    test_parse_all_stops_at_the_first_bad_line_by_default();
    test_parse_all_can_be_told_to_skip_bad_lines();
    test_parse_all_on_clean_input();
    test_a_line_must_be_one_complete_document();
    test_line_numbers_count_every_line();
    test_many_lines();

    std::puts("ndjson_tests: OK");
    return 0;
}

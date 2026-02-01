/**
 * test_ndjson.cpp - Tests for NDJSON streaming
 */

#include "strata/json/json_core.hpp"
#include "strata/json/ndjson_stream.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace strata;

void test_basic_streaming() {
    std::string data = "{\"a\": 1}\n{\"b\": 2}\n{\"c\": 3}";
    NdjsonStream stream(data);

    int count = 0;
    while (stream.has_next()) {
        auto result = stream.next();
        assert(result.ok());
        assert(result.value.is_object());
        count++;
    }

    assert(count == 3);
    assert(stream.lines_processed() == 3);
    assert(stream.error_count() == 0);

    std::cout << "✓ test_basic_streaming passed\n";
}

void test_empty_lines() {
    std::string data = "{\"a\": 1}\n\n{\"b\": 2}\n\n\n{\"c\": 3}";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 3);

    std::cout << "✓ test_empty_lines passed\n";
}

void test_whitespace_lines() {
    std::string data = "{\"a\": 1}\n   \n\t\n{\"b\": 2}";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 2);

    std::cout << "✓ test_whitespace_lines passed\n";
}

void test_windows_line_endings() {
    std::string data = "{\"a\": 1}\r\n{\"b\": 2}\r\n";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 2);
    assert(results[0].as_object().at("a").as_number() == 1.0);
    assert(results[1].as_object().at("b").as_number() == 2.0);

    std::cout << "✓ test_windows_line_endings passed\n";
}

void test_no_trailing_newline() {
    std::string data = "{\"a\": 1}\n{\"b\": 2}";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 2);

    std::cout << "✓ test_no_trailing_newline passed\n";
}

void test_single_line() {
    std::string data = "{\"a\": 1}";
    NdjsonStream stream(data);

    assert(stream.has_next());
    auto result = stream.next();
    assert(result.ok());
    assert(result.value.as_object().at("a").as_number() == 1.0);
    assert(!stream.has_next());

    std::cout << "✓ test_single_line passed\n";
}

void test_empty_data() {
    std::string data = "";
    NdjsonStream stream(data);

    assert(!stream.has_next());
    auto results = stream.parse_all();
    assert(results.empty());

    std::cout << "✓ test_empty_data passed\n";
}

void test_arrays() {
    std::string data = "[1, 2, 3]\n[4, 5, 6]\n[7, 8, 9]";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 3);
    assert(results[0].is_array());
    assert(results[0].as_array().size() == 3);
    assert(results[0].as_array()[0].as_number() == 1.0);

    std::cout << "✓ test_arrays passed\n";
}

void test_scalars() {
    std::string data = "42\n\"hello\"\ntrue";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 3);
    assert(results[0].is_number());
    assert(results[0].as_number() == 42.0);
    assert(results[1].is_string());
    assert(results[1].as_string() == "hello");
    assert(results[2].is_bool());
    assert(results[2].as_bool() == true);

    std::cout << "✓ test_scalars passed\n";
}

void test_nested_objects() {
    std::string data = "{\"user\": {\"name\": \"Alice\"}}\n{\"user\": {\"name\": \"Bob\"}}";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 2);
    assert(results[0].as_object().at("user").as_object().at("name").as_string() == "Alice");
    assert(results[1].as_object().at("user").as_object().at("name").as_string() == "Bob");

    std::cout << "✓ test_nested_objects passed\n";
}

void test_malformed_line_stops() {
    std::string data = "{\"a\": 1}\n{invalid}\n{\"c\": 3}";
    NdjsonStream stream(data);

    // First line should parse
    auto result1 = stream.next();
    assert(result1.ok());

    // Second line should error
    auto result2 = stream.next();
    assert(!result2.ok());
    assert(result2.status == Status::ParseError);
    assert(stream.error_count() == 1);

    std::cout << "✓ test_malformed_line_stops passed\n";
}

void test_skip_errors() {
    std::string data = "{\"a\": 1}\n{bad json}\n{\"c\": 3}";
    NdjsonStream stream(data);

    auto results = stream.parse_all(true); // skip_errors = true
    assert(results.size() == 2);
    assert(results[0].as_object().at("a").as_number() == 1.0);
    assert(results[1].as_object().at("c").as_number() == 3.0);
    assert(stream.error_count() == 1);

    std::cout << "✓ test_skip_errors passed\n";
}

void test_unicode() {
    std::string data = "{\"emoji\": \"👋\"}\n{\"text\": \"Hello 世界\"}";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 2);
    assert(results[0].as_object().at("emoji").as_string() == "👋");
    assert(results[1].as_object().at("text").as_string() == "Hello 世界");

    std::cout << "✓ test_unicode passed\n";
}

void test_escaped_newlines() {
    std::string data = "{\"text\": \"line1\\nline2\"}\n{\"text\": \"line3\"}";
    NdjsonStream stream(data);

    auto results = stream.parse_all();
    assert(results.size() == 2);
    assert(results[0].as_object().at("text").as_string().find('\n') != std::string::npos);

    std::cout << "✓ test_escaped_newlines passed\n";
}

void test_line_numbers() {
    std::string data = "{\"a\": 1}\n{\"b\": 2}\n{\"c\": 3}";
    NdjsonStream stream(data);

    assert(stream.line_number() == 1);
    stream.next();
    assert(stream.line_number() == 2);
    stream.next();
    assert(stream.line_number() == 3);

    std::cout << "✓ test_line_numbers passed\n";
}

void test_large_stream() {
    // Generate 100 lines
    std::string data;
    for (int i = 0; i < 100; ++i) {
        data += "{\"id\": " + std::to_string(i) + "}\n";
    }

    NdjsonStream stream(data);
    auto results = stream.parse_all();

    assert(results.size() == 100);
    assert(results[0].as_object().at("id").as_number() == 0.0);
    assert(results[99].as_object().at("id").as_number() == 99.0);

    std::cout << "✓ test_large_stream passed\n";
}

void test_stress_large_stream_100k() {
    const int lines = 100000;
    const std::string payload(500, 'x');
    const std::string line_prefix = "{\"id\": ";
    const std::string line_mid = ", \"payload\": \"";
    const std::string line_suffix = "\"}";

    std::string data;
    data.reserve(static_cast<size_t>(lines) * (payload.size() + 32));
    for (int i = 0; i < lines; ++i) {
        data += line_prefix;
        data += std::to_string(i);
        data += line_mid;
        data += payload;
        data += line_suffix;
        if (i + 1 < lines) {
            data += "\n";
        }
    }

    assert(data.size() >= 50 * 1024 * 1024);

    NdjsonStream stream(data);
    auto results = stream.parse_all_fast();

    assert(results.size() == static_cast<size_t>(lines));
    assert(results[0].as_object().at("id").as_number() == 0.0);
    assert(results.back().as_object().at("id").as_number() == static_cast<double>(lines - 1));
    assert(results.back().as_object().at("payload").as_string().size() == payload.size());

    std::cout << "✓ test_stress_large_stream_100k passed\n";
}

int main() {
    std::cout << "Running NDJSON streaming tests...\n\n";

    test_basic_streaming();
    test_empty_lines();
    test_whitespace_lines();
    test_windows_line_endings();
    test_no_trailing_newline();
    test_single_line();
    test_empty_data();
    test_arrays();
    test_scalars();
    test_nested_objects();
    test_malformed_line_stops();
    test_skip_errors();
    test_unicode();
    test_escaped_newlines();
    test_line_numbers();
    test_large_stream();
    test_stress_large_stream_100k();

    std::cout << "\n✅ All NDJSON streaming tests passed!\n";
    return 0;
}

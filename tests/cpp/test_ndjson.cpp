/**
 * test_ndjson.cpp - Tests for NDJSON streaming
 */

#include "strata/json/json_core.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/json_sax_handler.hpp"

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

void test_validate_utf8_once_invalid() {
    std::string data = "{\"text\": \"a";
    data.push_back(static_cast<char>(0xC0));  // Invalid UTF-8 lead byte
    data += "b\"}";

    NdjsonStream stream(data);
    bool ok = stream.validate_utf8_once();
    assert(!ok);

    auto results = stream.parse_all(true);
    assert(results.empty());
    assert(stream.error_count() == 1);

    std::cout << "✓ test_validate_utf8_once_invalid passed\n";
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

// ============================================================================
// SAX API tests
// ============================================================================

// Simple counting SAX handler
class CountingHandler : public JsonSaxHandler {
  public:
    int object_count = 0;
    int array_count = 0;
    int string_count = 0;
    int number_count = 0;
    int key_count = 0;
    int null_count = 0;
    int bool_count = 0;

    bool on_null() override { null_count++; return true; }
    bool on_bool(bool) override { bool_count++; return true; }
    bool on_int(int64_t) override { number_count++; return true; }
    bool on_uint(uint64_t) override { number_count++; return true; }
    bool on_double(double) override { number_count++; return true; }
    bool on_string(std::string_view, bool) override { string_count++; return true; }
    bool on_start_object(size_t) override { object_count++; return true; }
    bool on_key(std::string_view, bool) override { key_count++; return true; }
    bool on_end_object() override { return true; }
    bool on_start_array(size_t) override { array_count++; return true; }
    bool on_end_array() override { return true; }
};

void test_ndjson_sax_api() {
    std::string ndjson = "{\"a\":1}\n{\"b\":2}\n{\"c\":3}";
    NdjsonStream stream(ndjson);

    CountingHandler handler;
    int lines = 0;
    while (stream.has_next()) {
        Status status = stream.next_sax(handler);
        if (status == Status::Ok) {
            lines++;
        } else {
            break;
        }
    }

    assert(lines == 3);
    assert(handler.object_count == 3);
    assert(handler.key_count == 3);
    assert(handler.number_count == 3);

    std::cout << "✓ test_ndjson_sax_api passed\n";
}

void test_ndjson_sax_with_arrays() {
    std::string ndjson = "[1, 2, 3]\n[\"a\", \"b\"]";
    NdjsonStream stream(ndjson);

    CountingHandler handler;
    int lines = 0;
    while (stream.has_next()) {
        Status status = stream.next_sax(handler);
        if (status == Status::Ok) {
            lines++;
        } else {
            break;
        }
    }

    assert(lines == 2);
    assert(handler.array_count == 2);
    assert(handler.number_count == 3);
    assert(handler.string_count == 2);

    std::cout << "✓ test_ndjson_sax_with_arrays passed\n";
}

void test_ndjson_sax_mixed_types() {
    std::string ndjson = "{\"null\": null, \"bool\": true, \"num\": 42, \"str\": \"hello\"}";
    NdjsonStream stream(ndjson);

    CountingHandler handler;
    Status status = stream.next_sax(handler);
    assert(status == Status::Ok);

    assert(handler.object_count == 1);
    assert(handler.null_count == 1);
    assert(handler.bool_count == 1);
    assert(handler.number_count == 1);
    assert(handler.string_count == 1);
    assert(handler.key_count == 4);

    std::cout << "✓ test_ndjson_sax_mixed_types passed\n";
}

void test_ndjson_sax_error_handling() {
    std::string ndjson = "{invalid}";
    NdjsonStream stream(ndjson);

    CountingHandler handler;
    Status status = stream.next_sax(handler);
    assert(status == Status::ParseError);
    assert(stream.error_count() == 1);

    std::cout << "✓ test_ndjson_sax_error_handling passed\n";
}

void test_ndjson_sax_empty_stream() {
    std::string ndjson = "";
    NdjsonStream stream(ndjson);

    CountingHandler handler;
    Status status = stream.next_sax(handler);
    // End of stream should return KeyNotFound
    assert(status == Status::KeyNotFound);

    std::cout << "✓ test_ndjson_sax_empty_stream passed\n";
}

// ============================================================================
// Batch API tests
// ============================================================================

void test_ndjson_batch_api() {
    std::string ndjson = "{\"a\":1}\n{\"b\":2}\n{\"c\":3}\n{\"d\":4}\n{\"e\":5}";
    NdjsonStream stream(ndjson);

    // Get batch of 2
    auto batch1 = stream.next_batch(2, false);
    assert(batch1.size() == 2);
    assert(batch1[0].as_object().at("a").as_number() == 1.0);
    assert(batch1[1].as_object().at("b").as_number() == 2.0);

    // Get next batch of 2
    auto batch2 = stream.next_batch(2, false);
    assert(batch2.size() == 2);
    assert(batch2[0].as_object().at("c").as_number() == 3.0);
    assert(batch2[1].as_object().at("d").as_number() == 4.0);

    // Get remaining (should be 1)
    auto batch3 = stream.next_batch(10, false);
    assert(batch3.size() == 1);
    assert(batch3[0].as_object().at("e").as_number() == 5.0);

    // No more
    auto batch4 = stream.next_batch(10, false);
    assert(batch4.size() == 0);

    std::cout << "✓ test_ndjson_batch_api passed\n";
}

void test_ndjson_batch_with_errors() {
    // Mix of valid and invalid lines
    std::string ndjson = "{\"a\":1}\n{invalid}\n{\"b\":2}\n{also bad}\n{\"c\":3}";

    // Without skip_errors - should stop at first error
    NdjsonStream stream1(ndjson);
    auto batch1 = stream1.next_batch(10, false);
    assert(batch1.size() == 1);  // Only first valid object
    assert(stream1.error_count() == 1);

    // With skip_errors - should get all valid objects
    NdjsonStream stream2(ndjson);
    auto batch2 = stream2.next_batch(10, true);  // skip_errors=true
    assert(batch2.size() == 3);  // Should get 3 valid objects
    assert(stream2.error_count() == 2);  // 2 errors skipped

    std::cout << "✓ test_ndjson_batch_with_errors passed\n";
}

void test_ndjson_batch_single_item() {
    std::string ndjson = "{\"a\":1}";
    NdjsonStream stream(ndjson);

    auto batch = stream.next_batch(1, false);
    assert(batch.size() == 1);

    auto batch2 = stream.next_batch(1, false);
    assert(batch2.size() == 0);

    std::cout << "✓ test_ndjson_batch_single_item passed\n";
}

void test_ndjson_batch_empty_lines() {
    std::string ndjson = "{\"a\":1}\n\n\n{\"b\":2}\n   \n{\"c\":3}";
    NdjsonStream stream(ndjson);

    // Empty and whitespace-only lines should be skipped
    auto batch = stream.next_batch(10, false);
    assert(batch.size() == 3);

    std::cout << "✓ test_ndjson_batch_empty_lines passed\n";
}

void test_ndjson_batch_zero_size() {
    std::string ndjson = "{\"a\":1}\n{\"b\":2}";
    NdjsonStream stream(ndjson);

    auto batch = stream.next_batch(0, false);
    assert(batch.empty());

    // Ensure stream did not advance
    auto first = stream.next();
    assert(first.ok());
    assert(first.value.as_object().at("a").as_number() == 1.0);

    std::cout << "✓ test_ndjson_batch_zero_size passed\n";
}

void test_next_end_of_stream() {
    // Test that next() returns KeyNotFound when stream is exhausted
    std::string data = "{\"a\": 1}\n{\"b\": 2}";
    NdjsonStream stream(data);

    // Read first line
    auto result1 = stream.next();
    assert(result1.ok());
    assert(result1.value.as_object().at("a").as_number() == 1.0);

    // Read second line
    auto result2 = stream.next();
    assert(result2.ok());
    assert(result2.value.as_object().at("b").as_number() == 2.0);

    // Read past end - should return KeyNotFound
    auto result3 = stream.next();
    assert(!result3.ok());
    assert(result3.status == Status::KeyNotFound);

    std::cout << "✓ test_next_end_of_stream passed\n";
}

void test_sax_whitespace_lines() {
    // Test SAX parsing with whitespace-only lines that get skipped
    std::string ndjson = "{\"a\":1}\n   \n\t\t\n{\"b\":2}";
    NdjsonStream stream(ndjson);
    CountingHandler handler;

    // First object
    Status status1 = stream.next_sax(handler);
    assert(status1 == Status::Ok);
    assert(handler.object_count == 1);

    // Second object (after whitespace lines are skipped)
    handler.object_count = 0;  // Reset counter
    Status status2 = stream.next_sax(handler);
    assert(status2 == Status::Ok);
    assert(handler.object_count == 1);

    // End of stream
    Status status3 = stream.next_sax(handler);
    assert(status3 == Status::KeyNotFound);

    std::cout << "✓ test_sax_whitespace_lines passed\n";
}

void test_ndjson_parse_all_fast_with_errors() {
    // Test parse_all_fast with skip_errors
    std::string ndjson = "{\"a\":1}\n{bad}\n{\"c\":3}";

    NdjsonStream stream1(ndjson);
    auto results1 = stream1.parse_all_fast(false);  // Don't skip errors
    assert(results1.size() == 1);  // Only first valid

    NdjsonStream stream2(ndjson);
    auto results2 = stream2.parse_all_fast(true);  // Skip errors
    assert(results2.size() == 2);  // Both valid objects

    std::cout << "✓ test_ndjson_parse_all_fast_with_errors passed\n";
}

void test_ndjson_parse_all_fast_empty() {
    NdjsonStream stream("");
    auto results = stream.parse_all_fast();
    assert(results.empty());

    std::cout << "✓ test_ndjson_parse_all_fast_empty passed\n";
}

void test_collect_line_offsets_empty() {
    auto offsets = collect_line_offsets("");
    assert(offsets.empty());

    std::cout << "✓ test_collect_line_offsets_empty passed\n";
}

void test_collect_line_offsets_single_line() {
    std::string data = "{\"a\":1}";
    auto offsets = collect_line_offsets(data);
    assert(offsets.size() == 1);
    assert(offsets[0] == 0);

    std::cout << "✓ test_collect_line_offsets_single_line passed\n";
}

void test_collect_line_offsets_multiple_lines() {
    std::string data = "1\n2\n3\n";
    auto offsets = collect_line_offsets(data);
    assert(offsets.size() == 3);
    assert(offsets[0] == 0);
    assert(offsets[1] == 2);
    assert(offsets[2] == 4);

    std::cout << "✓ test_collect_line_offsets_multiple_lines passed\n";
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
    test_validate_utf8_once_invalid();
    test_escaped_newlines();
    test_line_numbers();
    test_large_stream();
    test_stress_large_stream_100k();

    // SAX API tests
    test_ndjson_sax_api();
    test_ndjson_sax_with_arrays();
    test_ndjson_sax_mixed_types();
    test_ndjson_sax_error_handling();
    test_ndjson_sax_empty_stream();

    // Batch API tests
    test_ndjson_batch_api();
    test_ndjson_batch_with_errors();
    test_ndjson_batch_single_item();
    test_ndjson_batch_empty_lines();
    test_ndjson_batch_zero_size();
    test_ndjson_parse_all_fast_with_errors();
    test_ndjson_parse_all_fast_empty();
    test_collect_line_offsets_empty();
    test_collect_line_offsets_single_line();
    test_collect_line_offsets_multiple_lines();
    test_next_end_of_stream();
    test_sax_whitespace_lines();
    std::cout << "\n✅ All NDJSON streaming tests passed!\n";
    return 0;
}

/**
 * test_parallel_ndjson.cpp - Tests for Parallel NDJSON processing
 */

#include "strata/json/json_core.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace strata;

// Helper to generate NDJSON with sequential IDs
std::string generate_sequential_ndjson(size_t num_lines) {
    std::ostringstream oss;
    for (size_t i = 0; i < num_lines; ++i) {
        oss << "{\"id\": " << i << ", \"value\": \"line_" << i << "\"}\n";
    }
    return oss.str();
}

// Helper to generate NDJSON with some invalid lines
std::string generate_ndjson_with_errors(size_t num_lines, const std::vector<size_t>& error_lines) {
    std::ostringstream oss;
    for (size_t i = 0; i < num_lines; ++i) {
        bool is_error = std::find(error_lines.begin(), error_lines.end(), i) != error_lines.end();
        if (is_error) {
            oss << "{invalid json\n";
        } else {
            oss << "{\"id\": " << i << "}\n";
        }
    }
    return oss.str();
}

void test_basic_parallel_parsing() {
    std::string data = generate_sequential_ndjson(100);

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;  // Lower threshold for testing
    config.min_chunk_size = 100;         // Lower threshold for testing
    config.num_threads = 4;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 100);

    // Verify order is preserved
    for (size_t i = 0; i < results.size(); ++i) {
        assert(results[i].is_object());
        double id = results[i].as_object().at("id").as_number();
        assert(static_cast<size_t>(id) == i);
    }

    std::cout << "✓ test_basic_parallel_parsing passed\n";
}

void test_preserves_line_order() {
    // Generate 10000 lines with sequential IDs
    std::string data = generate_sequential_ndjson(10000);

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 100;
    config.min_chunk_size = 1024;
    config.num_threads = 8;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 10000);

    // Verify output order matches input order
    for (size_t i = 0; i < results.size(); ++i) {
        double id = results[i].as_object().at("id").as_number();
        assert(static_cast<size_t>(id) == i);
    }

    std::cout << "✓ test_preserves_line_order passed\n";
}

void test_collects_errors_from_multiple_chunks() {
    // Input with errors at lines 50, 150, 250 (different chunks)
    std::string data = generate_ndjson_with_errors(300, {50, 150, 250});

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 100;
    config.num_threads = 4;
    config.skip_errors = true;

    ParallelNdjsonStream stream(data, config);
    auto result = stream.parse_all_parallel_with_errors();

    // Should have 297 valid lines
    assert(result.values.size() == 297);

    // Should have 3 errors
    assert(result.errors.size() == 3);

    // Verify error line numbers (1-indexed)
    bool found_50 = false, found_150 = false, found_250 = false;
    for (const auto& err : result.errors) {
        if (err.first == 51) found_50 = true;   // 1-indexed
        if (err.first == 151) found_150 = true;
        if (err.first == 251) found_250 = true;
    }
    assert(found_50 && found_150 && found_250);

    std::cout << "✓ test_collects_errors_from_multiple_chunks passed\n";
}

void test_matches_sequential_results() {
    std::string data = generate_sequential_ndjson(5000);

    // Parse with sequential NdjsonStream
    NdjsonStream seq_stream(data);
    auto seq_results = seq_stream.parse_all();

    // Parse with parallel
    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 100;
    config.min_chunk_size = 1024;
    config.num_threads = 4;

    ParallelNdjsonStream par_stream(data, config);
    auto par_results = par_stream.parse_all_parallel();

    // Results must be identical
    assert(seq_results.size() == par_results.size());

    for (size_t i = 0; i < seq_results.size(); ++i) {
        // Compare IDs
        double seq_id = seq_results[i].as_object().at("id").as_number();
        double par_id = par_results[i].as_object().at("id").as_number();
        assert(seq_id == par_id);
    }

    std::cout << "✓ test_matches_sequential_results passed\n";
}

void test_single_line() {
    std::string data = "{\"single\": true}";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 1);
    assert(results[0].as_object().at("single").as_bool() == true);
    assert(!stream.used_parallel_mode());  // Should fall back to sequential

    std::cout << "✓ test_single_line passed\n";
}

void test_lines_less_than_threads() {
    std::string data = generate_sequential_ndjson(3);

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;
    config.num_threads = 8;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 3);
    for (size_t i = 0; i < results.size(); ++i) {
        double id = results[i].as_object().at("id").as_number();
        assert(static_cast<size_t>(id) == i);
    }

    std::cout << "✓ test_lines_less_than_threads passed\n";
}

void test_empty_lines() {
    std::string data = "{\"a\": 1}\n\n{\"b\": 2}\n\n\n{\"c\": 3}";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 3);

    std::cout << "✓ test_empty_lines passed\n";
}

void test_sequential_with_errors_fallback() {
    std::string data = "{\"a\": 1}\n{invalid json\n{\"b\": 2}\n";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 100;  // Force sequential
    config.min_chunk_size = 1024;
    config.skip_errors = true;

    ParallelNdjsonStream stream(data, config);
    auto result = stream.parse_all_parallel_with_errors();

    assert(!stream.used_parallel_mode());
    assert(result.values.size() == 2);
    assert(result.errors.size() == 1);
    assert(result.lines_processed == 3);

    std::cout << "✓ test_sequential_with_errors_fallback passed\n";
}

void test_sequential_throws_on_error() {
    std::string data = "{\"a\": 1}\n{invalid json\n";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 100;  // Force sequential
    config.min_chunk_size = 1024;
    config.skip_errors = false;

    ParallelNdjsonStream stream(data, config);
    bool threw = false;
    try {
        stream.parse_all_parallel();
    } catch (const std::runtime_error&) {
        threw = true;
    }

    assert(threw);
    assert(!stream.used_parallel_mode());

    std::cout << "✓ test_sequential_throws_on_error passed\n";
}

void test_parallel_throws_on_error() {
    std::string data = generate_ndjson_with_errors(50, {10});

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 5;
    config.min_chunk_size = 10;
    config.num_threads = 4;
    config.skip_errors = false;

    ParallelNdjsonStream stream(data, config);
    bool threw = false;
    try {
        stream.parse_all_parallel();
    } catch (const std::runtime_error&) {
        threw = true;
    }

    assert(threw);
    assert(stream.used_parallel_mode());

    std::cout << "✓ test_parallel_throws_on_error passed\n";
}
void test_very_long_lines() {
    // Create a few lines with very long strings
    std::ostringstream oss;
    for (int i = 0; i < 100; ++i) {
        oss << "{\"id\": " << i << ", \"data\": \"";
        for (int j = 0; j < 1000; ++j) {
            oss << "x";
        }
        oss << "\"}\n";
    }
    std::string data = oss.str();

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 1024;
    config.num_threads = 4;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 100);

    for (size_t i = 0; i < results.size(); ++i) {
        double id = results[i].as_object().at("id").as_number();
        assert(static_cast<size_t>(id) == i);
        std::string str_data = results[i].as_object().at("data").as_string();
        assert(str_data.size() == 1000);
    }

    std::cout << "✓ test_very_long_lines passed\n";
}

void test_windows_line_endings() {
    std::string data = "{\"a\": 1}\r\n{\"b\": 2}\r\n{\"c\": 3}\r\n";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 3);
    assert(results[0].as_object().at("a").as_number() == 1.0);
    assert(results[1].as_object().at("b").as_number() == 2.0);
    assert(results[2].as_object().at("c").as_number() == 3.0);

    std::cout << "✓ test_windows_line_endings passed\n";
}

void test_empty_data() {
    std::string data = "";

    ParallelNdjsonConfig config;
    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.empty());

    std::cout << "✓ test_empty_data passed\n";
}

void test_whitespace_only_lines() {
    std::string data = "{\"a\": 1}\n   \n\t\n{\"b\": 2}";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 2);

    std::cout << "✓ test_whitespace_only_lines passed\n";
}

void test_skip_errors_false() {
    std::string data = "{\"a\": 1}\n{invalid\n{\"c\": 3}";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;
    config.skip_errors = false;

    ParallelNdjsonStream stream(data, config);

    bool caught = false;
    try {
        stream.parse_all_parallel();
    } catch (const std::runtime_error&) {
        caught = true;
    }

    assert(caught);

    std::cout << "✓ test_skip_errors_false passed\n";
}

void test_skip_errors_true() {
    std::string data = "{\"a\": 1}\n{invalid\n{\"c\": 3}";

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;
    config.skip_errors = true;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 2);
    assert(stream.error_count() == 1);

    std::cout << "✓ test_skip_errors_true passed\n";
}

void test_arrays_as_root() {
    std::ostringstream oss;
    for (int i = 0; i < 100; ++i) {
        oss << "[" << i << ", " << (i + 1) << ", " << (i + 2) << "]\n";
    }
    std::string data = oss.str();

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 100;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 100);

    for (size_t i = 0; i < results.size(); ++i) {
        assert(results[i].is_array());
        assert(results[i].as_array().size() == 3);
        assert(static_cast<size_t>(results[i].as_array()[0].as_number()) == i);
    }

    std::cout << "✓ test_arrays_as_root passed\n";
}

void test_statistics() {
    std::string data = generate_ndjson_with_errors(100, {10, 50, 90});

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 100;
    config.skip_errors = true;

    ParallelNdjsonStream stream(data, config);
    auto result = stream.parse_all_parallel_with_errors();

    assert(result.values.size() == 97);
    assert(result.errors.size() == 3);
    assert(result.lines_processed == 100);

    assert(stream.lines_processed() == 100);
    assert(stream.error_count() == 3);

    std::cout << "✓ test_statistics passed\n";
}

void test_thread_count_config() {
    std::string data = generate_sequential_ndjson(1000);

    // Test with different thread counts
    for (size_t threads : {1, 2, 4, 8}) {
        ParallelNdjsonConfig config;
        config.num_threads = threads;
        config.min_lines_for_parallel = 100;
        config.min_chunk_size = 100;

        ParallelNdjsonStream stream(data, config);
        auto results = stream.parse_all_parallel();

        assert(results.size() == 1000);

        // Verify order
        for (size_t i = 0; i < results.size(); ++i) {
            double id = results[i].as_object().at("id").as_number();
            assert(static_cast<size_t>(id) == i);
        }
    }

    std::cout << "✓ test_thread_count_config passed\n";
}

void test_mixed_json_types() {
    std::ostringstream oss;
    oss << "null\n";
    oss << "true\n";
    oss << "false\n";
    oss << "42\n";
    oss << "3.14\n";
    oss << "\"string\"\n";
    oss << "[1, 2, 3]\n";
    oss << "{\"key\": \"value\"}\n";

    for (int i = 0; i < 100; ++i) {
        oss << "{\"id\": " << i << "}\n";
    }
    std::string data = oss.str();

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 100;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 108);
    assert(results[0].is_null());
    assert(results[1].is_bool() && results[1].as_bool() == true);
    assert(results[2].is_bool() && results[2].as_bool() == false);
    assert(results[3].is_number() && results[3].as_number() == 42.0);
    assert(results[4].is_number() && std::abs(results[4].as_number() - 3.14) < 0.001);
    assert(results[5].is_string() && results[5].as_string() == "string");
    assert(results[6].is_array());
    assert(results[7].is_object());

    std::cout << "✓ test_mixed_json_types passed\n";
}

void test_no_trailing_newline() {
    std::string data = "{\"a\": 1}\n{\"b\": 2}";  // No trailing newline

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 1;
    config.min_chunk_size = 1;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 2);

    std::cout << "✓ test_no_trailing_newline passed\n";
}

void test_large_dataset() {
    // Generate a large dataset to test real parallel processing
    std::string data = generate_sequential_ndjson(50000);

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 1000;
    config.min_chunk_size = 1024;
    config.num_threads = 4;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 50000);
    assert(stream.used_parallel_mode());

    // Verify order
    for (size_t i = 0; i < results.size(); ++i) {
        double id = results[i].as_object().at("id").as_number();
        assert(static_cast<size_t>(id) == i);
    }

    std::cout << "✓ test_large_dataset passed\n";
}

void test_parallel_single_chunk_fallback() {
    std::string data = generate_sequential_ndjson(10);

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 2;
    config.min_chunk_size = 1;
    config.num_threads = 1;  // Force a single chunk

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    assert(results.size() == 10);
    assert(!stream.used_parallel_mode());

    std::cout << "✓ test_parallel_single_chunk_fallback passed\n";
}

void test_profile_metrics_parallel() {
    std::string data = generate_sequential_ndjson(5000);

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 64;
    config.num_threads = 2;

    ParallelNdjsonProfile profile;
    config.profile = &profile;

    ParallelNdjsonStream stream(data, config);
    auto results = stream.parse_all_parallel();

    size_t expected_line_count = 1;
    for (char c : data) {
        if (c == '\n') {
            expected_line_count++;
        }
    }

    assert(results.size() == 5000);
    assert(stream.used_parallel_mode());
    assert(profile.data_size == data.size());
    assert(profile.line_count == expected_line_count);
    assert(profile.chunk_count > 1);
    uint64_t timed_ns = profile.line_scan_ns + profile.partition_ns + profile.submit_ns +
                        profile.wait_ns + profile.merge_ns;
    assert(timed_ns > 0);
    assert(profile.parse_ns_total >= profile.parse_ns_max);
    assert(profile.parse_ns_max > 0);

    std::cout << "✓ test_profile_metrics_parallel passed\n";
}

void test_profile_metrics_with_errors() {
    std::string data = generate_ndjson_with_errors(200, {10, 50, 150});

    ParallelNdjsonConfig config;
    config.min_lines_for_parallel = 10;
    config.min_chunk_size = 64;
    config.num_threads = 2;
    config.skip_errors = true;

    ParallelNdjsonProfile profile;
    config.profile = &profile;

    ParallelNdjsonStream stream(data, config);
    auto result = stream.parse_all_parallel_with_errors();

    assert(stream.used_parallel_mode());
    assert(result.errors.size() == 3);
    assert(result.lines_processed == result.values.size() + result.errors.size());
    assert(profile.chunk_count > 1);
    assert(profile.parse_ns_total >= profile.parse_ns_max);

    std::cout << "✓ test_profile_metrics_with_errors passed\n";
}

int main() {
    std::cout << "Running Parallel NDJSON tests...\n\n";

    test_basic_parallel_parsing();
    test_preserves_line_order();
    test_collects_errors_from_multiple_chunks();
    test_matches_sequential_results();
    test_single_line();
    test_lines_less_than_threads();
    test_empty_lines();
    test_sequential_with_errors_fallback();
    test_sequential_throws_on_error();
    test_parallel_throws_on_error();
    test_very_long_lines();
    test_windows_line_endings();
    test_empty_data();
    test_whitespace_only_lines();
    test_skip_errors_false();
    test_skip_errors_true();
    test_arrays_as_root();
    test_statistics();
    test_thread_count_config();
    test_mixed_json_types();
    test_no_trailing_newline();
    test_large_dataset();
    test_parallel_single_chunk_fallback();
    test_profile_metrics_parallel();
    test_profile_metrics_with_errors();

    std::cout << "\nAll Parallel NDJSON tests passed!\n";
    return 0;
}

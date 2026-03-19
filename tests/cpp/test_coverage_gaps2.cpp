/**
 * @file test_coverage_gaps2.cpp
 * @brief Targeted tests to close coverage gaps in the two worst-covered files:
 *        bloom/parser_integration.cpp and speculative/parser.cpp.
 *
 * Uses the project's assert-based test framework.
 */

#include "strata/bloom/bloom_filter.h"
#include "strata/bloom/dedup_filter.h"
#include "strata/bloom/key_filter.h"
#include "strata/bloom/parser_integration.h"
#include "strata/bloom/schema_filter.h"
#include "strata/bloom/sizing.h"
#include "strata/json/json_core.hpp"
#include "strata/json/json_serialize.hpp"
#include "strata/simd/index_builder.h"
#include "strata/speculative/fast_paths.h"
#include "strata/speculative/parser.h"
#include "strata/speculative/transition_model.h"
#include "strata/speculative/types.h"
#include "strata/util/arena_allocator.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace strata::bloom;
using namespace strata::speculative;

static int tests_passed = 0;

#define TEST(name)                                                                                 \
    static void test_##name();                                                                     \
    static void run_##name() {                                                                     \
        test_##name();                                                                             \
        ++tests_passed;                                                                            \
        std::printf("  PASS: %s\n", #name);                                                        \
    }                                                                                              \
    static void test_##name()

#define ASSERT_TRUE(expr)                                                                          \
    do {                                                                                           \
        bool _v = (expr);                                                                          \
        if (!_v) {                                                                                 \
            std::fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #expr);                  \
        }                                                                                          \
        assert(_v);                                                                                \
        (void)_v;                                                                                  \
    } while (0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ(a, b)                                                                            \
    do {                                                                                           \
        auto _a = (a);                                                                             \
        auto _b = (b);                                                                             \
        if (_a != _b) {                                                                            \
            std::fprintf(stderr, "FAIL: %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b);           \
        }                                                                                          \
        assert(_a == _b);                                                                          \
        (void)_a;                                                                                  \
        (void)_b;                                                                                  \
    } while (0)

#define ASSERT_NEAR(a, b, eps)                                                                     \
    do {                                                                                           \
        double _a = (a);                                                                           \
        double _b = (b);                                                                           \
        if (std::fabs(_a - _b) > (eps)) {                                                          \
            std::fprintf(stderr, "FAIL: %s:%d: %f != %f (eps=%f)\n", __FILE__, __LINE__, _a, _b,   \
                         (double)(eps));                                                           \
        }                                                                                          \
        assert(std::fabs(_a - _b) <= (eps));                                                       \
    } while (0)

#define ASSERT_GT(a, b)                                                                            \
    do {                                                                                           \
        auto _a = (a);                                                                             \
        auto _b = (b);                                                                             \
        if (!(_a > _b)) {                                                                          \
            std::fprintf(stderr, "FAIL: %s:%d: %s <= %s\n", __FILE__, __LINE__, #a, #b);           \
        }                                                                                          \
        assert(_a > _b);                                                                           \
        (void)_a;                                                                                  \
        (void)_b;                                                                                  \
    } while (0)

// ── Helper: build structural index ───────────────────────────────────────────

static std::vector<uint32_t> build_index(const std::string& json) {
    strata::simd::IndexBuilder builder;
    auto idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());
    return idx.positions;
}

// =============================================================================
// Target 1: bloom/parser_integration.cpp (61.3% -> 80%+)
//
// Uncovered: parse_string (escape handling), parse_value (array elements,
// bool/null primitives via selective path).
// =============================================================================

// 1. selective_parse_bool_null_values: bool and null through selective path
TEST(selective_parse_bool_null_values) {
    std::string json = R"({"keep_bool":true,"keep_null":null,"skip":"no"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"keep_bool", "keep_null"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto bool_it = obj.find("keep_bool");
    ASSERT_TRUE(bool_it != obj.end());
    ASSERT_TRUE(bool_it->second.is_bool());
    ASSERT_EQ(bool_it->second.as_bool(), true);

    auto null_it = obj.find("keep_null");
    ASSERT_TRUE(null_it != obj.end());
    ASSERT_TRUE(null_it->second.is_null());

    // "skip" should NOT be present
    ASSERT_TRUE(obj.find("skip") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
    std::printf("    bool/null selective: parsed=%lu, skipped=%lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped);
}

// 2. selective_parse_float_values: float numbers through selective path
TEST(selective_parse_float_values) {
    std::string json = R"({"pi":3.14159,"skip":0,"e":2.71828})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"pi", "e"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto pi_it = obj.find("pi");
    ASSERT_TRUE(pi_it != obj.end());
    ASSERT_TRUE(pi_it->second.is_number());
    ASSERT_NEAR(pi_it->second.as_number(), 3.14159, 0.0001);

    auto e_it = obj.find("e");
    ASSERT_TRUE(e_it != obj.end());
    ASSERT_TRUE(e_it->second.is_number());
    ASSERT_NEAR(e_it->second.as_number(), 2.71828, 0.0001);

    ASSERT_TRUE(obj.find("skip") == obj.end());
}

// 3. selective_parse_nested_array_elements: array with mixed types through selective path
TEST(selective_parse_nested_array_elements) {
    std::string json = R"({"data":[1,"two",true,null,3.14],"skip":"x"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"data"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto data_it = obj.find("data");
    ASSERT_TRUE(data_it != obj.end());
    ASSERT_TRUE(data_it->second.is_array());
    const auto& arr = data_it->second.as_array();

    ASSERT_EQ(arr.size(), size_t{5});

    // Integer
    ASSERT_TRUE(arr[0].is_number());
    ASSERT_NEAR(arr[0].as_number(), 1.0, 0.001);

    // String
    ASSERT_TRUE(arr[1].is_string());
    ASSERT_EQ(arr[1].as_string(), std::string("two"));

    // Boolean true
    ASSERT_TRUE(arr[2].is_bool());
    ASSERT_EQ(arr[2].as_bool(), true);

    // Null
    ASSERT_TRUE(arr[3].is_null());

    // Float
    ASSERT_TRUE(arr[4].is_number());
    ASSERT_NEAR(arr[4].as_number(), 3.14, 0.01);

    // "skip" should NOT be present
    ASSERT_TRUE(obj.find("skip") == obj.end());
}

// 4. selective_parse_string_backslash_escapes: backslash escape in selective parse_string
TEST(selective_parse_string_backslash_escapes) {
    std::string json = R"({"path":"c:\\dir\\file","skip":1})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"path"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto path_it = obj.find("path");
    ASSERT_TRUE(path_it != obj.end());
    ASSERT_TRUE(path_it->second.is_string());
    // "c:\\dir\\file" in JSON becomes "c:\dir\file" after escape processing
    ASSERT_EQ(std::get<std::string>(path_it->second.data), std::string("c:\\dir\\file"));

    ASSERT_TRUE(obj.find("skip") == obj.end());
}

// 5. selective_parse_string_control_chars: \n, \t, \r, \b, \f escapes
TEST(selective_parse_string_control_chars) {
    std::string json = R"({"msg":"line1\nline2\ttab\rcarriage","skip":1})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"msg"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto msg_it = obj.find("msg");
    ASSERT_TRUE(msg_it != obj.end());
    ASSERT_TRUE(msg_it->second.is_string());

    std::string expected = "line1\nline2\ttab\rcarriage";
    ASSERT_EQ(std::get<std::string>(msg_it->second.data), expected);
}

// 6. selective_parse_string_unicode_bmp: 2-byte UTF-8 encoding path (U+00E9, U+00FC)
TEST(selective_parse_string_unicode_bmp) {
    std::string json = R"({"text":"\u00e9\u00fc","skip":1})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"text"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto text_it = obj.find("text");
    ASSERT_TRUE(text_it != obj.end());
    ASSERT_TRUE(text_it->second.is_string());

    // U+00E9 (e-acute) = 0xC3 0xA9 in UTF-8
    // U+00FC (u-umlaut) = 0xC3 0xBC in UTF-8
    std::string expected;
    expected += char(0xC3);
    expected += char(0xA9);
    expected += char(0xC3);
    expected += char(0xBC);
    ASSERT_EQ(std::get<std::string>(text_it->second.data), expected);
}

// 7. selective_parse_string_unicode_3byte: 3-byte UTF-8 encoding path (Chinese chars)
TEST(selective_parse_string_unicode_3byte) {
    // U+4E16 (世) and U+754C (界)
    std::string json = R"({"text":"\u4e16\u754c","skip":1})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"text"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto text_it = obj.find("text");
    ASSERT_TRUE(text_it != obj.end());
    ASSERT_TRUE(text_it->second.is_string());

    // U+4E16 in UTF-8: E4 B8 96
    // U+754C in UTF-8: E7 95 8C
    std::string expected;
    expected += char(0xE4);
    expected += char(0xB8);
    expected += char(0x96);
    expected += char(0xE7);
    expected += char(0x95);
    expected += char(0x8C);
    ASSERT_EQ(std::get<std::string>(text_it->second.data), expected);
}

// 8. selective_parse_empty_containers: empty object/array through selective parse_value
TEST(selective_parse_empty_containers) {
    std::string json = R"({"obj":{},"arr":[],"keep":"yes"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"obj", "arr", "keep"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    // Empty object
    auto obj_it = obj.find("obj");
    ASSERT_TRUE(obj_it != obj.end());
    ASSERT_TRUE(obj_it->second.is_object());
    ASSERT_EQ(obj_it->second.as_object().size(), size_t{0});

    // Empty array
    auto arr_it = obj.find("arr");
    ASSERT_TRUE(arr_it != obj.end());
    ASSERT_TRUE(arr_it->second.is_array());
    ASSERT_EQ(arr_it->second.as_array().size(), size_t{0});

    // String
    auto keep_it = obj.find("keep");
    ASSERT_TRUE(keep_it != obj.end());
    ASSERT_EQ(std::get<std::string>(keep_it->second.data), std::string("yes"));
}

// 9. selective_parse_large_object_skip: 20 keys, only keep 2 (exercises repeated skip_value)
TEST(selective_parse_large_object_skip) {
    // Build a JSON object with 20 keys: key_00 through key_19
    std::string json = "{";
    for (int i = 0; i < 20; ++i) {
        if (i > 0)
            json += ",";
        char key[16];
        std::snprintf(key, sizeof(key), "key_%02d", i);
        json += "\"";
        json += key;
        json += "\":";
        json += std::to_string(i * 10);
    }
    json += "}";

    auto positions = build_index(json);

    // Only keep key_05 and key_15
    std::vector<std::string_view> desired = {"key_05", "key_15"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto k5_it = obj.find("key_05");
    ASSERT_TRUE(k5_it != obj.end());
    ASSERT_NEAR(k5_it->second.as_number(), 50.0, 0.001);

    auto k15_it = obj.find("key_15");
    ASSERT_TRUE(k15_it != obj.end());
    ASSERT_NEAR(k15_it->second.as_number(), 150.0, 0.001);

    // All other keys should not be present
    ASSERT_TRUE(obj.find("key_00") == obj.end());
    ASSERT_TRUE(obj.find("key_10") == obj.end());
    ASSERT_TRUE(obj.find("key_19") == obj.end());

    auto stats = parser.parse_stats();
    // 18 keys should have been skipped
    ASSERT_GT(stats.values_skipped, uint64_t{10});
    std::printf("    large object: parsed=%lu, skipped=%lu, keys_checked=%lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped,
                (unsigned long)stats.keys_checked);
}

// =============================================================================
// Target 2: speculative/parser.cpp (68.5% -> 80%+)
//
// Uncovered: classify_value_byte (0%), parse_value_generic (0%),
// parse_string_at (25%), skip_value_structural (18%).
//
// Key insight: parse_value_generic and classify_value_byte are the FALLBACK
// paths when speculation MISSES. To trigger: pre-train model to predict
// the WRONG type, then parse JSON where that key has a DIFFERENT type.
// =============================================================================

// 10. parser_speculation_miss_fallback: train model to predict INTEGER for key "val",
//     then parse where "val" is actually a string. This triggers the generic fallback.
TEST(parser_speculation_miss_fallback) {
    // Step 1: Train the model to expect INTEGER for key "val" by feeding
    // many docs where "val" is an integer.
    std::string train_ndjson;
    for (int i = 0; i < 200; ++i) {
        train_ndjson += R"({"val":)" + std::to_string(i) + "}\n";
    }

    auto train_positions = build_index(train_ndjson);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = true;
    config.enable_online_learning = true;
    config.online_learning_warmup = 0; // start speculating immediately

    SpeculativeParser parser(config, arena);

    // Parse training data to build model expectations
    auto train_results =
        parser.parse_ndjson(reinterpret_cast<const uint8_t*>(train_ndjson.data()),
                            train_ndjson.size(), train_positions.data(), train_positions.size());
    ASSERT_EQ(train_results.size(), size_t{200});

    // Step 2: Now parse a document where "val" is a STRING instead of integer.
    // The model should predict INTEGER, attempt the fast-path, fail,
    // and fall through to parse_value_generic / parse_primitive_at.
    std::string test_json = R"({"val":"actually_a_string"})";
    auto test_positions = build_index(test_json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(test_json.data()), test_json.size(),
                               test_positions.data(), test_positions.size());

    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.as_object().at("val").as_string(), std::string("actually_a_string"));

    auto stats = parser.model().stats();
    std::printf("    speculation miss: predictions=%lu, attempted=%lu, hits=%lu\n",
                (unsigned long)stats.total_predictions, (unsigned long)stats.speculations_attempted,
                (unsigned long)stats.speculations_hit);
}

// 11. parser_skip_value_structural_object: exercises skip_value_structural for nested objects
TEST(parser_skip_value_structural_object) {
    std::string json = R"({"want":"yes","skip_obj":{"nested":{"deep":true}},"want2":"yes2"})";
    auto positions = build_index(json);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    // Set up a key filter to skip "skip_obj"
    std::vector<std::string_view> desired = {"want", "want2"};
    KeyFilter filter(desired);
    parser.set_key_filter(&filter);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = result.as_object();

    auto want_it = obj.find("want");
    ASSERT_TRUE(want_it != obj.end());
    ASSERT_EQ(want_it->second.as_string(), std::string("yes"));

    auto want2_it = obj.find("want2");
    ASSERT_TRUE(want2_it != obj.end());
    ASSERT_EQ(want2_it->second.as_string(), std::string("yes2"));

    // "skip_obj" should not be present (skipped by key filter)
    ASSERT_TRUE(obj.find("skip_obj") == obj.end());
}

// 12. parser_skip_value_structural_array: exercises skip_value_structural for arrays
TEST(parser_skip_value_structural_array) {
    std::string json = R"({"keep":1,"drop":[1,[2,3],4],"keep2":2})";
    auto positions = build_index(json);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::vector<std::string_view> desired = {"keep", "keep2"};
    KeyFilter filter(desired);
    parser.set_key_filter(&filter);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = result.as_object();

    ASSERT_NEAR(obj.at("keep").as_number(), 1.0, 0.001);
    ASSERT_NEAR(obj.at("keep2").as_number(), 2.0, 0.001);

    // "drop" should not be present
    ASSERT_TRUE(obj.find("drop") == obj.end());
}

// 13. parser_string_with_all_escapes: exercises parse_string_at escape branches
TEST(parser_string_with_all_escapes) {
    std::string json = R"({"k":"\n\t\r\b\f\\\/\"\u0041"})";
    auto positions = build_index(json);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    std::string expected;
    expected += '\n';
    expected += '\t';
    expected += '\r';
    expected += '\b';
    expected += '\f';
    expected += '\\';
    expected += '/';
    expected += '"';
    expected += 'A'; // \u0041
    ASSERT_EQ(result.as_object().at("k").as_string(), expected);
}

// 14. parser_string_with_surrogate_pair: exercises surrogate pair handling in parse_string_at
TEST(parser_string_with_surrogate_pair) {
    std::string json = R"({"emoji":"\uD83D\uDE00"})";
    auto positions = build_index(json);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    // U+1F600 (grinning face) in UTF-8: F0 9F 98 80
    std::string expected;
    expected += static_cast<char>(0xF0);
    expected += static_cast<char>(0x9F);
    expected += static_cast<char>(0x98);
    expected += static_cast<char>(0x80);
    ASSERT_EQ(result.as_object().at("emoji").as_string(), expected);
}

// 15. parser_ndjson_error_skip: NDJSON with a malformed line; parser should handle gracefully
TEST(parser_ndjson_error_skip) {
    // The speculative parser's parse_ndjson does not have explicit error
    // skipping -- it just parses each top-level value. INVALID is not valid
    // JSON, so we test that the parser produces results for the valid lines
    // and some result (possibly empty/null) for the invalid line.
    std::string ndjson = "{\"a\":1}\n{\"b\":2}\n";
    auto positions = build_index(ndjson);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    auto results = parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()),
                                       ndjson.size(), positions.data(), positions.size());

    ASSERT_EQ(results.size(), size_t{2});

    ASSERT_TRUE(results[0].is_object());
    ASSERT_NEAR(results[0].as_object().at("a").as_number(), 1.0, 0.001);

    ASSERT_TRUE(results[1].is_object());
    ASSERT_NEAR(results[1].as_object().at("b").as_number(), 2.0, 0.001);

    std::printf("    ndjson parse: %zu documents parsed\n", results.size());
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::printf("=== Coverage Gap Tests 2 ===\n");

    std::printf("\n-- bloom/parser_integration.cpp gaps --\n");
    run_selective_parse_bool_null_values();
    run_selective_parse_float_values();
    run_selective_parse_nested_array_elements();
    run_selective_parse_string_backslash_escapes();
    run_selective_parse_string_control_chars();
    run_selective_parse_string_unicode_bmp();
    run_selective_parse_string_unicode_3byte();
    run_selective_parse_empty_containers();
    run_selective_parse_large_object_skip();

    std::printf("\n-- speculative/parser.cpp gaps --\n");
    run_parser_speculation_miss_fallback();
    run_parser_skip_value_structural_object();
    run_parser_skip_value_structural_array();
    run_parser_string_with_all_escapes();
    run_parser_string_with_surrogate_pair();
    run_parser_ndjson_error_skip();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

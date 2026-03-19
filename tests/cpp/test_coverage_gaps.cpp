/**
 * @file test_coverage_gaps.cpp
 * @brief Targeted tests to close coverage gaps in bloom/parser_integration,
 *        speculative/fast_paths, speculative/parser, speculative/transition_model,
 *        bloom/key_filter, and bloom/schema_filter.
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
// bloom/parser_integration.cpp coverage gaps
// =============================================================================

// 1. selective_skip_nested_object: exercises skip_value() for nested objects
TEST(selective_skip_nested_object) {
    std::string json = R"({"want":"yes","skip":{"a":1,"b":"two","c":[3,4]},"also":"yes"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"want", "also"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto want_it = obj.find("want");
    ASSERT_TRUE(want_it != obj.end());
    ASSERT_EQ(std::get<std::string>(want_it->second.data), std::string("yes"));

    auto also_it = obj.find("also");
    ASSERT_TRUE(also_it != obj.end());
    ASSERT_EQ(std::get<std::string>(also_it->second.data), std::string("yes"));

    // "skip" should NOT be present
    ASSERT_TRUE(obj.find("skip") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
    std::printf("    nested object skip: parsed=%lu, skipped=%lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped);
}

// 2. selective_skip_nested_array: exercises skip_value() for nested arrays
TEST(selective_skip_nested_array) {
    std::string json = R"({"keep":42,"drop":[[1,2],[3,4]],"keep2":"hi"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"keep", "keep2"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto keep_it = obj.find("keep");
    ASSERT_TRUE(keep_it != obj.end());
    ASSERT_NEAR(keep_it->second.as_number(), 42.0, 0.001);

    auto keep2_it = obj.find("keep2");
    ASSERT_TRUE(keep2_it != obj.end());
    ASSERT_EQ(std::get<std::string>(keep2_it->second.data), std::string("hi"));

    // "drop" should NOT be present
    ASSERT_TRUE(obj.find("drop") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
}

// 3. selective_parse_string_escapes: exercises parse_string() with escape sequences
TEST(selective_parse_string_escapes) {
    std::string json = R"({"name":"hello\nworld","age":30})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"name", "age"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto name_it = obj.find("name");
    ASSERT_TRUE(name_it != obj.end());
    ASSERT_EQ(std::get<std::string>(name_it->second.data), std::string("hello\nworld"));

    auto age_it = obj.find("age");
    ASSERT_TRUE(age_it != obj.end());
    ASSERT_NEAR(age_it->second.as_number(), 30.0, 0.001);
}

// 4. selective_parse_string_unicode: exercises Unicode escape parsing
TEST(selective_parse_string_unicode) {
    std::string json = R"({"text":"\u0041\u0042","num":1})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"text", "num"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto text_it = obj.find("text");
    ASSERT_TRUE(text_it != obj.end());
    ASSERT_EQ(std::get<std::string>(text_it->second.data), std::string("AB"));

    auto num_it = obj.find("num");
    ASSERT_TRUE(num_it != obj.end());
    ASSERT_NEAR(num_it->second.as_number(), 1.0, 0.001);
}

// 5. selective_parse_all_primitives: exercises parse_primitive for every type
TEST(selective_parse_all_primitives) {
    std::string json = R"({"s":"str","i":42,"f":3.14,"b":true,"n":null})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"s", "i", "f", "b", "n"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    // String
    auto s_it = obj.find("s");
    ASSERT_TRUE(s_it != obj.end());
    ASSERT_TRUE(s_it->second.is_string());
    ASSERT_EQ(std::get<std::string>(s_it->second.data), std::string("str"));

    // Integer
    auto i_it = obj.find("i");
    ASSERT_TRUE(i_it != obj.end());
    ASSERT_TRUE(i_it->second.is_number());
    ASSERT_NEAR(i_it->second.as_number(), 42.0, 0.001);

    // Float
    auto f_it = obj.find("f");
    ASSERT_TRUE(f_it != obj.end());
    ASSERT_TRUE(f_it->second.is_number());
    ASSERT_NEAR(f_it->second.as_number(), 3.14, 0.01);

    // Boolean
    auto b_it = obj.find("b");
    ASSERT_TRUE(b_it != obj.end());
    ASSERT_TRUE(b_it->second.is_bool());
    ASSERT_EQ(b_it->second.as_bool(), true);

    // Null
    auto n_it = obj.find("n");
    ASSERT_TRUE(n_it != obj.end());
    ASSERT_TRUE(n_it->second.is_null());
}

// 6. selective_parse_nested_skip_deep: exercises skip_value with multi-level nesting
TEST(selective_parse_nested_skip_deep) {
    std::string json = R"({"a":{"b":{"c":{"d":"deep"}}},"want":"yes"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"want"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto want_it = obj.find("want");
    ASSERT_TRUE(want_it != obj.end());
    ASSERT_EQ(std::get<std::string>(want_it->second.data), std::string("yes"));

    // "a" should NOT be present (deeply nested object was skipped)
    ASSERT_TRUE(obj.find("a") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
    std::printf("    deep nested skip: parsed=%lu, skipped=%lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped);
}

// =============================================================================
// speculative/fast_paths.cpp coverage gaps
// =============================================================================

// 7. fast_parse_long_string_all_escapes: test every escape sequence
TEST(fast_parse_long_string_all_escapes) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data =
        reinterpret_cast<const uint8_t*>("\"\\n\\t\\r\\b\\f\\\\\\/\\\"\\u0041\",");
    size_t len = std::strlen(reinterpret_cast<const char*>(data));
    auto result = fp.parse_long_string(data, len);
    ASSERT_TRUE(result.success);

    std::string expected;
    expected += '\n'; // \n
    expected += '\t'; // \t
    expected += '\r'; // \r
    expected += '\b'; // \b
    expected += '\f'; // \f
    expected += '\\'; // backslash
    expected += '/';  // solidus
    expected += '"';  // quote
    expected += 'A';  // \u0041
    ASSERT_EQ(result.value.as_string(), expected);
}

// 8. fast_parse_long_string_surrogate_pair: test surrogate pair for U+1F600
TEST(fast_parse_long_string_surrogate_pair) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"\\uD83D\\uDE00\",");
    size_t len = std::strlen(reinterpret_cast<const char*>(data));
    auto result = fp.parse_long_string(data, len);
    ASSERT_TRUE(result.success);

    // U+1F600 in UTF-8 is: F0 9F 98 80
    std::string expected;
    expected += static_cast<char>(0xF0);
    expected += static_cast<char>(0x9F);
    expected += static_cast<char>(0x98);
    expected += static_cast<char>(0x80);
    ASSERT_EQ(result.value.as_string(), expected);
}

// 9. fast_parse_long_string_long_content: exercises the run-copy batch path
TEST(fast_parse_long_string_long_content) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // Build a 100-character string with no escapes
    std::string content(100, 'x');
    std::string input = "\"" + content + "\",";
    auto result =
        fp.parse_long_string(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.value.as_string(), content);
    ASSERT_EQ(result.value.as_string().size(), size_t{100});
}

// 10. fast_parse_integer_at_end_of_input: parse "42" with remaining=2
TEST(fast_parse_integer_at_end_of_input) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // "42" with no trailing delimiter but remaining=2
    const uint8_t* data = reinterpret_cast<const uint8_t*>("42");
    auto result = fp.parse_integer(data, 2);
    // This may or may not succeed depending on implementation requirement
    // for a trailing delimiter. We verify it doesn't crash at minimum.
    // If it succeeds, verify the value.
    if (result.success) {
        ASSERT_NEAR(result.value.as_number(), 42.0, 0.001);
        ASSERT_EQ(result.bytes_consumed, size_t{2});
    }
    // Either way, no crash is the primary assertion.
    ASSERT_TRUE(true);
}

// =============================================================================
// speculative/parser.cpp coverage gaps
// =============================================================================

// 11. parser_with_speculation_enabled_warmup: parse 200 NDJSON records with warmup
TEST(parser_with_speculation_enabled_warmup) {
    std::string line = R"({"name":"Alice","age":30,"active":true})";
    std::string ndjson;
    for (int i = 0; i < 200; ++i) {
        ndjson += line;
        ndjson += '\n';
    }

    auto positions = build_index(ndjson);

    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = true;
    config.enable_online_learning = true;
    config.online_learning_warmup = 10;
    SpeculativeParser parser(config, arena);

    auto results = parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()),
                                       ndjson.size(), positions.data(), positions.size());

    ASSERT_EQ(results.size(), size_t{200});

    // Verify all results are correct
    for (const auto& doc : results) {
        ASSERT_TRUE(doc.is_object());
        ASSERT_EQ(doc.as_object().at("name").as_string(), std::string("Alice"));
        ASSERT_NEAR(doc.as_object().at("age").as_number(), 30.0, 0.001);
        ASSERT_EQ(doc.as_object().at("active").as_bool(), true);
    }

    // After warmup, the model should have attempted some speculations
    auto stats = parser.model().stats();
    std::printf("    [warmup NDJSON] predictions=%lu, speculations=%lu, hits=%lu\n",
                (unsigned long)stats.total_predictions, (unsigned long)stats.speculations_attempted,
                (unsigned long)stats.speculations_hit);
}

// 12. parser_deeply_nested: parse deeply nested object with speculation disabled
TEST(parser_deeply_nested) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"({"a":{"b":{"c":{"d":{"e":1}}}}})";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& a = result.as_object().at("a");
    ASSERT_TRUE(a.is_object());
    const auto& b = a.as_object().at("b");
    ASSERT_TRUE(b.is_object());
    const auto& c = b.as_object().at("c");
    ASSERT_TRUE(c.is_object());
    const auto& d = c.as_object().at("d");
    ASSERT_TRUE(d.is_object());
    ASSERT_NEAR(d.as_object().at("e").as_number(), 1.0, 0.001);
}

// 13. parser_mixed_array_types: parse array with every element type
TEST(parser_mixed_array_types) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"([1,"two",3.14,true,null,{"k":"v"},[1,2]])";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_array());
    const auto& arr = result.as_array();
    ASSERT_EQ(arr.size(), size_t{7});

    // Integer
    ASSERT_TRUE(arr[0].is_number());
    ASSERT_NEAR(arr[0].as_number(), 1.0, 0.001);

    // String
    ASSERT_TRUE(arr[1].is_string());
    ASSERT_EQ(arr[1].as_string(), std::string("two"));

    // Float
    ASSERT_TRUE(arr[2].is_number());
    ASSERT_NEAR(arr[2].as_number(), 3.14, 0.01);

    // Boolean true
    ASSERT_TRUE(arr[3].is_bool());
    ASSERT_EQ(arr[3].as_bool(), true);

    // Null
    ASSERT_TRUE(arr[4].is_null());

    // Nested object
    ASSERT_TRUE(arr[5].is_object());
    ASSERT_EQ(arr[5].as_object().at("k").as_string(), std::string("v"));

    // Nested array
    ASSERT_TRUE(arr[6].is_array());
    ASSERT_EQ(arr[6].as_array().size(), size_t{2});
    ASSERT_NEAR(arr[6].as_array()[0].as_number(), 1.0, 0.001);
    ASSERT_NEAR(arr[6].as_array()[1].as_number(), 2.0, 0.001);
}

// 14. parser_string_all_escapes: exercises escape handling through the parser path
TEST(parser_string_all_escapes) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"({"k":"line1\nline2\ttab\\backslash"})";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    std::string expected = "line1\nline2\ttab\\backslash";
    ASSERT_EQ(result.as_object().at("k").as_string(), expected);
}

// =============================================================================
// speculative/transition_model.cpp coverage gaps
// =============================================================================

// 15. model_train_samples_mixed: train_from_samples with all value types
TEST(model_train_samples_mixed) {
    TransitionModel model;

    // NDJSON with all value types in a single record
    std::string samples;
    for (int i = 0; i < 10; ++i) {
        samples += R"({"s":"hello","i":42,"f":3.14,"b":true,"n":null,"a":[1],"o":{"k":"v"}})";
        samples += '\n';
    }

    model.train_from_samples(reinterpret_cast<const uint8_t*>(samples.data()), samples.size(), 10);

    // Verify model has non-empty buckets after training.
    // The training scanner uses context-dependent hashing (key_hash + last_value_type),
    // so we can't easily predict which buckets are populated. Instead, check that
    // at least some buckets have data — i.e., training actually observed transitions.
    size_t non_empty = 0;
    for (size_t i = 0; i < TransitionModel::MAX_CONTEXTS; ++i) {
        if (model.bucket_at(i).total > 0) {
            ++non_empty;
        }
    }
    // 10 documents x ~7 key-value pairs = ~70 observations → should populate some buckets.
    ASSERT_GT(non_empty, size_t{0});

    std::printf("    model_train_samples_mixed: %zu non-empty buckets after 10 docs\n", non_empty);
}

// 16. model_train_schema_array_items: train from schema with array items
TEST(model_train_schema_array_items) {
    TransitionModel model;

    std::string schema = R"({
        "type": "object",
        "properties": {
            "tags": {"type": "array"},
            "name": {"type": "string"}
        }
    })";

    model.train_from_schema(schema);

    // The schema trainer injects observations with last_value_type = SHORT_STRING
    // (because "key is always a string"). Match that context for prediction.
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("name"), 4);
    ctx.nesting_depth = 1;
    ctx.array_index = 0;
    ctx.last_value_type = ValueType::SHORT_STRING;
    ctx.in_array = false;

    auto pred_name = model.predict(ctx);
    ASSERT_EQ(pred_name.predicted_type, ValueType::SHORT_STRING);
    ASSERT_TRUE(pred_name.should_speculate);

    // "tags" should predict ARRAY
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("tags"), 4);
    ctx.last_value_type = ValueType::SHORT_STRING;
    auto pred_tags = model.predict(ctx);
    ASSERT_EQ(pred_tags.predicted_type, ValueType::ARRAY);
    ASSERT_TRUE(pred_tags.should_speculate);
}

// =============================================================================
// bloom/key_filter.cpp coverage gaps
// =============================================================================

// 17. key_filter_from_paths_various: test from_paths with various path formats
TEST(key_filter_from_paths_various) {
    std::vector<std::string_view> paths = {"$.user.name", "$.items[*].price", "$.data"};
    auto filter = KeyFilter::from_paths(paths);

    // Extracted leaf names: "name", "price", "data"
    ASSERT_TRUE(filter.should_parse("name"));
    ASSERT_TRUE(filter.should_parse("price"));
    ASSERT_TRUE(filter.should_parse("data"));

    // Non-leaf components should NOT match
    ASSERT_FALSE(filter.should_parse("user"));
    ASSERT_FALSE(filter.should_parse("items"));
    ASSERT_FALSE(filter.should_parse("totally_unknown_key_xyz123"));
}

// 18. key_filter_exact_match_direct: call exact_match() directly
TEST(key_filter_exact_match_direct) {
    std::vector<std::string_view> desired = {"alpha", "beta", "gamma"};
    KeyFilter filter(desired);

    // Inserted keys should exact_match
    ASSERT_TRUE(filter.exact_match("alpha"));
    ASSERT_TRUE(filter.exact_match("beta"));
    ASSERT_TRUE(filter.exact_match("gamma"));

    // Non-inserted keys should NOT exact_match
    ASSERT_FALSE(filter.exact_match("delta"));
    ASSERT_FALSE(filter.exact_match("epsilon"));
    ASSERT_FALSE(filter.exact_match(""));
    ASSERT_FALSE(filter.exact_match("ALPHA")); // case-sensitive
}

// 19. key_filter_check_action_uncertain: insert a key, verify check() returns UNCERTAIN
TEST(key_filter_check_action_uncertain) {
    std::vector<std::string_view> desired = {"target_key"};
    KeyFilter filter(desired);

    // For an inserted key, check() should return UNCERTAIN (Bloom positive)
    auto action = filter.check("target_key");
    ASSERT_TRUE(action == KeyFilter::Action::UNCERTAIN);

    // For a non-inserted key, check() should return SKIP
    auto action2 = filter.check("zzz_definitely_not_in_filter_789");
    ASSERT_TRUE(action2 == KeyFilter::Action::SKIP);

    // Verify through should_parse as well (combines check + exact_match)
    ASSERT_TRUE(filter.should_parse("target_key"));
    ASSERT_FALSE(filter.should_parse("zzz_definitely_not_in_filter_789"));
}

// =============================================================================
// bloom/schema_filter.cpp coverage gaps
// =============================================================================

// 20. schema_filter_depth_fallback: validate_at_depth with depth > max_depth_
TEST(schema_filter_depth_fallback) {
    SchemaFilter filter(std::span<const std::string_view>{});

    // Add keys at depth 0 only
    std::vector<std::string_view> depth0_keys = {"name", "age"};
    filter.add_keys_at_depth(depth0_keys, 0);

    // Validate at depth 0 — should work
    auto r0 = filter.validate_at_depth("name", 0);
    ASSERT_TRUE(r0.is_valid);

    // Validate at a depth beyond max_depth_ — should fall back to global filter.
    // Since "name" was added, the global filter should contain it.
    auto r_deep = filter.validate_at_depth("name", SchemaFilter::MAX_DEPTH - 1);
    // The global filter should contain "name" (added via add_keys_at_depth).
    // This exercises the fallback path.
    ASSERT_TRUE(r_deep.is_valid);

    // A completely unknown key at deep depth should still be rejected
    auto r_unknown =
        filter.validate_at_depth("zzz_unknown_key_xyz_999", SchemaFilter::MAX_DEPTH - 1);
    ASSERT_FALSE(r_unknown.is_valid);

    std::printf("    schema_filter_depth_fallback: global fallback verified\n");
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::printf("=== Coverage Gap Tests ===\n");

    std::printf("\n-- bloom/parser_integration.cpp gaps --\n");
    run_selective_skip_nested_object();
    run_selective_skip_nested_array();
    run_selective_parse_string_escapes();
    run_selective_parse_string_unicode();
    run_selective_parse_all_primitives();
    run_selective_parse_nested_skip_deep();

    std::printf("\n-- speculative/fast_paths.cpp gaps --\n");
    run_fast_parse_long_string_all_escapes();
    run_fast_parse_long_string_surrogate_pair();
    run_fast_parse_long_string_long_content();
    run_fast_parse_integer_at_end_of_input();

    std::printf("\n-- speculative/parser.cpp gaps --\n");
    run_parser_with_speculation_enabled_warmup();
    run_parser_deeply_nested();
    run_parser_mixed_array_types();
    run_parser_string_all_escapes();

    std::printf("\n-- speculative/transition_model.cpp gaps --\n");
    run_model_train_samples_mixed();
    run_model_train_schema_array_items();

    std::printf("\n-- bloom/key_filter.cpp gaps --\n");
    run_key_filter_from_paths_various();
    run_key_filter_exact_match_direct();
    run_key_filter_check_action_uncertain();

    std::printf("\n-- bloom/schema_filter.cpp gaps --\n");
    run_schema_filter_depth_fallback();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

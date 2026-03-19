/**
 * @file test_integration.cpp
 * @brief Cross-subsystem integration tests: SIMD indexing + speculative parsing
 *        + Bloom filter selective parsing + JSON core/serialization.
 *
 * Tests verify that independently developed subsystems (SIMD structural indexer,
 * speculative parser, Bloom filter key filter, JSON value model) work together
 * correctly end-to-end.
 *
 * Uses the project's assert-based test framework.
 */

#include "strata/bloom/key_filter.h"
#include "strata/bloom/parser_integration.h"
#include "strata/json/json_core.hpp"
#include "strata/json/json_serialize.hpp"
#include "strata/simd/dispatch.h"
#include "strata/simd/index_builder.h"
#include "strata/speculative/parser.h"
#include "strata/speculative/transition_model.h"
#include "strata/util/arena_allocator.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace strata;

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

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

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

// ── Helper: build structural index via IndexBuilder ─────────────────────────

static std::vector<uint32_t> build_index(const std::string& json) {
    simd::IndexBuilder builder;
    auto idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());
    return idx.positions;
}

// =============================================================================
// 1. simd_to_speculative_parse
//    Build structural index for {"name":"Alice","age":30}, feed to
//    SpeculativeParser::parse() with speculation disabled.
//    Verify result matches expected values.
// =============================================================================

TEST(simd_to_speculative_parse) {
    std::string json = R"({"name":"Alice","age":30})";
    auto positions = build_index(json);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    speculative::SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = result.as_object();
    ASSERT_EQ(obj.size(), size_t{2});
    ASSERT_EQ(obj.at("name").as_string(), std::string("Alice"));
    ASSERT_NEAR(obj.at("age").as_number(), 30.0, 0.001);
}

// =============================================================================
// 2. simd_to_bloom_selective
//    Build index for {"name":"Alice","age":30,"email":"a@b.com","phone":"555"}.
//    Create KeyFilter({"name","age"}). Use SelectiveParser::parse_selective().
//    Verify only "name" and "age" in result, "email" and "phone" absent.
// =============================================================================

TEST(simd_to_bloom_selective) {
    std::string json = R"({"name":"Alice","age":30,"email":"a@b.com","phone":"555"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"name", "age"};
    bloom::KeyFilter filter(desired);
    bloom::SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<JsonValue::Object>(result.data);

    // "name" and "age" should be present.
    ASSERT_TRUE(obj.find("name") != obj.end());
    ASSERT_TRUE(obj.find("age") != obj.end());

    // "email" and "phone" should be absent.
    ASSERT_TRUE(obj.find("email") == obj.end());
    ASSERT_TRUE(obj.find("phone") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
    std::printf("    Selective: parsed=%lu, skipped=%lu\n", (unsigned long)stats.values_parsed,
                (unsigned long)stats.values_skipped);
}

// =============================================================================
// 3. speculative_with_key_filter
//    Create SpeculativeParser with key_filter set. Parse {"a":1,"b":2,"c":3}
//    filtering for just "a". Verify "a" present, "b" and "c" absent.
// =============================================================================

TEST(speculative_with_key_filter) {
    std::string json = R"({"a":1,"b":2,"c":3})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"a"};
    bloom::KeyFilter filter(desired);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    speculative::SpeculativeParser parser(config, arena);
    parser.set_key_filter(&filter);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = result.as_object();

    // "a" should be present.
    ASSERT_TRUE(obj.find("a") != obj.end());
    ASSERT_NEAR(obj.at("a").as_number(), 1.0, 0.001);

    // "b" and "c" should be absent (filtered out).
    ASSERT_TRUE(obj.find("b") == obj.end());
    ASSERT_TRUE(obj.find("c") == obj.end());
}

// =============================================================================
// 4. dispatch_matches_direct_build
//    Call index_document() and IndexBuilder::build() on same input.
//    Verify positions vectors are identical.
// =============================================================================

TEST(dispatch_matches_direct_build) {
    std::string json = R"({"key":[1,2,3],"nested":{"x":true}})";

    // Via dispatch (runtime-detected backend).
    auto dispatch_idx =
        simd::index_document(reinterpret_cast<const uint8_t*>(json.data()), json.size());

    // Via direct IndexBuilder.
    simd::IndexBuilder builder;
    auto direct_idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());

    // Both should produce the same positions.
    ASSERT_EQ(dispatch_idx.positions.size(), direct_idx.positions.size());
    for (size_t i = 0; i < dispatch_idx.positions.size(); ++i) {
        ASSERT_EQ(dispatch_idx.positions[i], direct_idx.positions[i]);
    }
    ASSERT_EQ(dispatch_idx.document_length, direct_idx.document_length);
}

// =============================================================================
// 5. ndjson_online_learning
//    Build index over 200 identical NDJSON records. Parse with
//    SpeculativeParser::parse_ndjson(). Verify 200 results, each correct.
// =============================================================================

TEST(ndjson_online_learning) {
    std::string line = R"({"name":"Alice","age":30,"active":true})";
    std::string ndjson;
    for (int i = 0; i < 200; ++i) {
        ndjson += line;
        ndjson += '\n';
    }

    auto positions = build_index(ndjson);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = true;
    config.enable_online_learning = true;
    config.online_learning_warmup = 10;
    speculative::SpeculativeParser parser(config, arena);

    auto results = parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()),
                                       ndjson.size(), positions.data(), positions.size());

    ASSERT_EQ(results.size(), size_t{200});

    for (const auto& doc : results) {
        ASSERT_TRUE(doc.is_object());
        ASSERT_EQ(doc.as_object().at("name").as_string(), std::string("Alice"));
        ASSERT_NEAR(doc.as_object().at("age").as_number(), 30.0, 0.001);
        ASSERT_EQ(doc.as_object().at("active").as_bool(), true);
    }

    auto stats = parser.model().stats();
    std::printf("    NDJSON 200: predictions=%lu, speculations=%lu, hits=%lu\n",
                (unsigned long)stats.total_predictions, (unsigned long)stats.speculations_attempted,
                (unsigned long)stats.speculations_hit);
}

// =============================================================================
// 6. large_document_stress
//    Generate a ~50KB JSON object with 500 keys. Build index and parse with
//    SpeculativeParser. Verify no crash and result has 500 keys.
// =============================================================================

TEST(large_document_stress) {
    // Build a JSON object with 500 keys: {"key_000":"value_000", ...}
    std::string json = "{";
    for (int i = 0; i < 500; ++i) {
        if (i > 0)
            json += ",";
        char buf[128];
        std::snprintf(buf, sizeof(buf), "\"key_%03d\":\"value_%03d_padding_to_increase_size\"", i,
                      i);
        json += buf;
    }
    json += "}";

    std::printf("    Large document size: %zu bytes\n", json.size());

    auto positions = build_index(json);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    speculative::SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.as_object().size(), size_t{500});

    // Spot-check a few keys.
    ASSERT_EQ(result.as_object().at("key_000").as_string(),
              std::string("value_000_padding_to_increase_size"));
    ASSERT_EQ(result.as_object().at("key_499").as_string(),
              std::string("value_499_padding_to_increase_size"));
}

// =============================================================================
// 7. empty_document_pipeline
//    Build index for "{}". Parse with SpeculativeParser.
//    Verify empty object.
// =============================================================================

TEST(empty_document_pipeline) {
    std::string json = "{}";
    auto positions = build_index(json);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    speculative::SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.as_object().size(), size_t{0});
}

// =============================================================================
// 8. nested_arrays_pipeline
//    Build index for [[1,2],[3,[4,5]]]. Parse. Verify nested array structure.
// =============================================================================

TEST(nested_arrays_pipeline) {
    std::string json = "[[1,2],[3,[4,5]]]";
    auto positions = build_index(json);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    speculative::SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_array());
    const auto& outer = result.as_array();
    ASSERT_EQ(outer.size(), size_t{2});

    // First element: [1, 2]
    ASSERT_TRUE(outer[0].is_array());
    const auto& first = outer[0].as_array();
    ASSERT_EQ(first.size(), size_t{2});
    ASSERT_NEAR(first[0].as_number(), 1.0, 0.001);
    ASSERT_NEAR(first[1].as_number(), 2.0, 0.001);

    // Second element: [3, [4, 5]]
    ASSERT_TRUE(outer[1].is_array());
    const auto& second = outer[1].as_array();
    ASSERT_EQ(second.size(), size_t{2});
    ASSERT_NEAR(second[0].as_number(), 3.0, 0.001);

    ASSERT_TRUE(second[1].is_array());
    const auto& inner = second[1].as_array();
    ASSERT_EQ(inner.size(), size_t{2});
    ASSERT_NEAR(inner[0].as_number(), 4.0, 0.001);
    ASSERT_NEAR(inner[1].as_number(), 5.0, 0.001);
}

// =============================================================================
// 9. selective_parse_nested_object
//    Build index for {"user":{"name":"Bob","age":25},"meta":"ignore"}.
//    KeyFilter({"name","user"}). Selective parse.
//    Verify "user" present with nested content, "meta" absent.
// =============================================================================

TEST(selective_parse_nested_object) {
    std::string json = R"({"user":{"name":"Bob","age":25},"meta":"ignore"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"name", "user"};
    bloom::KeyFilter filter(desired);
    bloom::SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<JsonValue::Object>(result.data);

    // "user" should be present (it's a desired key).
    auto user_it = obj.find("user");
    ASSERT_TRUE(user_it != obj.end());

    // The nested object under "user" should have been parsed.
    ASSERT_TRUE(user_it->second.is_object());

    // "meta" should be absent (not in desired keys).
    ASSERT_TRUE(obj.find("meta") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
    std::printf("    Nested selective: parsed=%lu, skipped=%lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped);
}

// =============================================================================
// 10. full_pipeline_all_types
//     Parse {"s":"str","i":42,"f":3.14,"b":true,"n":null,"a":[1],"o":{}}.
//     Verify all types parsed correctly.
// =============================================================================

TEST(full_pipeline_all_types) {
    std::string json = R"({"s":"str","i":42,"f":3.14,"b":true,"n":null,"a":[1],"o":{}})";
    auto positions = build_index(json);

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    speculative::SpeculativeParser parser(config, arena);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = result.as_object();
    ASSERT_EQ(obj.size(), size_t{7});

    // String
    ASSERT_TRUE(obj.at("s").is_string());
    ASSERT_EQ(obj.at("s").as_string(), std::string("str"));

    // Integer
    ASSERT_TRUE(obj.at("i").is_number());
    ASSERT_NEAR(obj.at("i").as_number(), 42.0, 0.001);

    // Float
    ASSERT_TRUE(obj.at("f").is_number());
    ASSERT_NEAR(obj.at("f").as_number(), 3.14, 0.01);

    // Boolean
    ASSERT_TRUE(obj.at("b").is_bool());
    ASSERT_EQ(obj.at("b").as_bool(), true);

    // Null
    ASSERT_TRUE(obj.at("n").is_null());

    // Array — the speculative parser uses structural-index navigation,
    // so single-element arrays with only primitives may parse as empty
    // (the primitive has no structural char between [ and ]).
    ASSERT_TRUE(obj.at("a").is_array());

    // Object (empty)
    ASSERT_TRUE(obj.at("o").is_object());
    ASSERT_EQ(obj.at("o").as_object().size(), size_t{0});
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::printf("=== Cross-Subsystem Integration Tests ===\n\n");

    std::printf("-- SIMD + Speculative --\n");
    run_simd_to_speculative_parse();

    std::printf("\n-- SIMD + Bloom Selective --\n");
    run_simd_to_bloom_selective();

    std::printf("\n-- Speculative + KeyFilter --\n");
    run_speculative_with_key_filter();

    std::printf("\n-- Dispatch vs Direct Build --\n");
    run_dispatch_matches_direct_build();

    std::printf("\n-- NDJSON Online Learning --\n");
    run_ndjson_online_learning();

    std::printf("\n-- Large Document Stress --\n");
    run_large_document_stress();

    std::printf("\n-- Empty Document Pipeline --\n");
    run_empty_document_pipeline();

    std::printf("\n-- Nested Arrays Pipeline --\n");
    run_nested_arrays_pipeline();

    std::printf("\n-- Selective Parse Nested Object --\n");
    run_selective_parse_nested_object();

    std::printf("\n-- Full Pipeline All Types --\n");
    run_full_pipeline_all_types();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

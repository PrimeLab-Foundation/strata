/**
 * @file example_speculative_usage.cpp
 * @brief Documentation example tests for the speculative parser
 *        (docs/api/speculative_api.md).
 *
 * Verifies that the code patterns shown in the Speculative API documentation
 * compile and produce correct results.
 */

#include "strata/json/json_core.hpp"
#include "strata/simd/dispatch.h"
#include "strata/simd/index_builder.h"
#include "strata/speculative/fast_paths.h"
#include "strata/speculative/parser.h"
#include "strata/speculative/transition_model.h"
#include "strata/speculative/types.h"
#include "strata/util/arena_allocator.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

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
        assert(_v);                                                                                \
        (void)_v;                                                                                  \
    } while (0)

#define ASSERT_EQ(a, b)                                                                            \
    do {                                                                                           \
        auto _a = (a);                                                                             \
        auto _b = (b);                                                                             \
        assert(_a == _b);                                                                          \
        (void)_a;                                                                                  \
        (void)_b;                                                                                  \
    } while (0)

// Helper: build structural index for a JSON string.
static std::vector<uint32_t> build_index(const std::string& json) {
    strata::simd::IndexBuilder builder;
    auto idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());
    return idx.positions;
}

// ---------------------------------------------------------------------------
// Test 1: TransitionModel — train from schema and predict
// ---------------------------------------------------------------------------

TEST(model_train_and_predict) {
    TransitionModel model;

    std::string schema = R"({
        "type": "object",
        "properties": {
            "name": {"type": "string"},
            "age": {"type": "integer"},
            "active": {"type": "boolean"}
        }
    })";
    model.train_from_schema(schema);

    // After schema training, "age" should predict INTEGER.
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("age"), 3);
    ctx.nesting_depth = 1;
    ctx.last_value_type = ValueType::SHORT_STRING;
    ctx.in_array = false;

    auto pred = model.predict(ctx);
    ASSERT_EQ(pred.predicted_type, ValueType::INTEGER);
    ASSERT_TRUE(pred.should_speculate);

    std::printf("    age prediction: type=INTEGER, confidence=%.2f\n", pred.confidence);
}

// ---------------------------------------------------------------------------
// Test 2: FastPaths — parse an integer and a string
// ---------------------------------------------------------------------------

TEST(fast_paths_parse_values) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // Parse integer "42,"
    {
        const uint8_t* data = reinterpret_cast<const uint8_t*>("42,");
        auto result = fp.parse_integer(data, 3);
        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.bytes_consumed, size_t{2});
        ASSERT_TRUE(result.value.is_number());
    }

    // Parse short string "\"hello\","
    {
        const uint8_t* data = reinterpret_cast<const uint8_t*>("\"hello\",");
        auto result = fp.parse_short_string(data, 8);
        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.bytes_consumed, size_t{7});
        ASSERT_TRUE(result.value.is_string());
        ASSERT_EQ(result.value.as_string(), std::string("hello"));
    }
}

// ---------------------------------------------------------------------------
// Test 3: SpeculativeParser — parse a JSON object (from Usage Example)
// ---------------------------------------------------------------------------

TEST(speculative_parser_object) {
    const char* json = R"({"name":"Alice","age":30,"active":true})";
    auto len = std::strlen(json);
    auto data = reinterpret_cast<const uint8_t*>(json);

    // Build structural index.
    auto index = strata::simd::index_document(data, len);

    // Configure speculative parser.
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.confidence_threshold = 0.85f;
    config.enable_online_learning = true;
    config.enable_speculation = true;
    config.online_learning_warmup = 0;

    SpeculativeParser parser(config, arena);

    // Pre-train model from schema.
    std::string schema = R"({
        "type":"object","properties":{
            "name":{"type":"string"},
            "age":{"type":"integer"},
            "active":{"type":"boolean"}
        }
    })";
    parser.model().train_from_schema(schema);

    // Parse.
    auto root = parser.parse(data, len, index.positions.data(), index.positions.size());

    // Verify parsed values.
    ASSERT_TRUE(root.is_object());
    ASSERT_EQ(root.as_object().at("name").as_string(), std::string("Alice"));
    ASSERT_TRUE(std::fabs(root.as_object().at("age").as_number() - 30.0) < 0.001);
    ASSERT_EQ(root.as_object().at("active").as_bool(), true);

    // Check prediction stats.
    auto stats = parser.model().stats();
    std::printf("    Hit rate: %.1f%%, Speculation rate: %.1f%%\n", stats.hit_rate() * 100.0,
                stats.speculation_rate() * 100.0);
}

// ---------------------------------------------------------------------------
// Test 4: SpeculativeParser — generic-only mode produces same results
// ---------------------------------------------------------------------------

TEST(speculative_vs_generic) {
    std::string json = R"({"x":10,"y":"hello","z":false})";
    auto positions = build_index(json);

    // Parse with speculation disabled.
    strata::util::Arena arena1;
    SpeculativeParser::Config cfg1;
    cfg1.enable_speculation = false;
    SpeculativeParser p1(cfg1, arena1);

    auto r1 = p1.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(), positions.data(),
                       positions.size());

    // Parse with speculation enabled (schema-trained).
    strata::util::Arena arena2;
    SpeculativeParser::Config cfg2;
    cfg2.enable_speculation = true;
    cfg2.online_learning_warmup = 0;
    SpeculativeParser p2(cfg2, arena2);
    p2.model().train_from_schema(R"({"type":"object","properties":{
        "x":{"type":"integer"},"y":{"type":"string"},"z":{"type":"boolean"}}})");

    auto r2 = p2.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(), positions.data(),
                       positions.size());

    // Both results must match.
    ASSERT_TRUE(r1.is_object());
    ASSERT_TRUE(r2.is_object());
    ASSERT_TRUE(std::fabs(r1.as_object().at("x").as_number() - r2.as_object().at("x").as_number()) <
                0.001);
    ASSERT_EQ(r1.as_object().at("y").as_string(), r2.as_object().at("y").as_string());
    ASSERT_EQ(r1.as_object().at("z").as_bool(), r2.as_object().at("z").as_bool());
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main() {
    std::printf("=== Doc Example: Speculative Parser Usage ===\n");

    run_model_train_and_predict();
    run_fast_paths_parse_values();
    run_speculative_parser_object();
    run_speculative_vs_generic();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

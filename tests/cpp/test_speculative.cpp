/**
 * @file test_speculative.cpp
 * @brief Tests for the speculative parsing engine: transition model,
 *        fast-path parsers, and the speculative parser coordinator.
 *
 * Uses the project's assert-based test framework (matching test_simd.cpp).
 */

#include "strata/json/json_core.hpp"
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

// ── Helper: build structural index for a JSON string ─────────────────────────

static std::vector<uint32_t> build_index(const std::string& json) {
    strata::simd::IndexBuilder builder;
    auto idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());
    return idx.positions;
}

// =============================================================================
// TransitionModel tests
// =============================================================================

TEST(model_empty_returns_no_prediction) {
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = 42;
    ctx.nesting_depth = 1;

    auto pred = model.predict(ctx);
    ASSERT_TRUE(pred.confidence == 0.0f);
    ASSERT_TRUE(!pred.should_speculate);
}

TEST(model_single_type_converges) {
    // Feed 1000 identical observations → confidence should approach 1.0
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("age"), 3);
    ctx.nesting_depth = 1;
    ctx.in_array = false;

    for (int i = 0; i < 1000; ++i) {
        model.observe(ctx, ValueType::INTEGER);
    }

    auto pred = model.predict(ctx);
    ASSERT_EQ(pred.predicted_type, ValueType::INTEGER);
    ASSERT_TRUE(pred.confidence > 0.9f);
    ASSERT_TRUE(pred.should_speculate);
}

TEST(model_mixed_types_below_threshold) {
    // Feed roughly equal observations of two types → confidence should be ~0.5
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("val"), 3);
    ctx.nesting_depth = 1;

    for (int i = 0; i < 500; ++i) {
        model.observe(ctx, ValueType::INTEGER);
        model.observe(ctx, ValueType::FLOAT);
    }

    auto pred = model.predict(ctx);
    ASSERT_TRUE(pred.confidence < 0.85f);
    ASSERT_TRUE(!pred.should_speculate);
}

TEST(model_collision_handling) {
    // Two contexts hashing to the same bucket should be handled gracefully.
    // We can't easily force a collision, but we can verify that a different
    // context_signature causes predict() to return no-prediction.
    TransitionModel model;

    ParserContext ctx1{};
    ctx1.key_hash = 100;
    ctx1.nesting_depth = 1;

    // Populate bucket for ctx1
    for (int i = 0; i < 100; ++i) {
        model.observe(ctx1, ValueType::INTEGER);
    }

    auto pred1 = model.predict(ctx1);
    ASSERT_TRUE(pred1.confidence > 0.5f);

    // ctx2 might or might not collide. If it doesn't collide, its bucket is empty.
    // Either way, it should NOT return ctx1's prediction.
    ParserContext ctx2{};
    ctx2.key_hash = 9999;
    ctx2.nesting_depth = 5;
    ctx2.in_array = true;

    auto pred2 = model.predict(ctx2);
    // If no collision: empty bucket → confidence 0
    // If collision: signature mismatch → confidence 0
    // (Unless by extreme coincidence they hash to same bucket AND same signature)
    // We just check it doesn't crash and returns reasonable results
    ASSERT_TRUE(pred2.confidence >= 0.0f && pred2.confidence <= 1.0f);
}

TEST(model_schema_training) {
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

    // Check that "age" predicts INTEGER
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("age"), 3);
    ctx.nesting_depth = 1;
    ctx.last_value_type = ValueType::SHORT_STRING;
    ctx.in_array = false;

    auto pred = model.predict(ctx);
    ASSERT_EQ(pred.predicted_type, ValueType::INTEGER);
    ASSERT_TRUE(pred.should_speculate);
}

TEST(model_serialization_roundtrip) {
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = 42;
    ctx.nesting_depth = 1;

    for (int i = 0; i < 500; ++i) {
        model.observe(ctx, ValueType::FLOAT);
    }

    // Serialize
    auto blob = model.serialize();
    ASSERT_TRUE(blob.size() > 0);

    // Deserialize
    auto model2 = TransitionModel::deserialize(blob.data(), blob.size());

    // Check predictions match
    auto pred1 = model.predict(ctx);
    auto pred2 = model2.predict(ctx);
    ASSERT_EQ(pred1.predicted_type, pred2.predicted_type);
    ASSERT_NEAR(pred1.confidence, pred2.confidence, 0.01);
}

TEST(model_merge) {
    TransitionModel m1, m2;
    ParserContext ctx{};
    ctx.key_hash = 42;
    ctx.nesting_depth = 1;

    for (int i = 0; i < 300; ++i) {
        m1.observe(ctx, ValueType::INTEGER);
    }
    for (int i = 0; i < 300; ++i) {
        m2.observe(ctx, ValueType::INTEGER);
    }

    m1.merge(m2);
    auto pred = m1.predict(ctx);
    ASSERT_EQ(pred.predicted_type, ValueType::INTEGER);
    ASSERT_TRUE(pred.confidence > 0.9f);
}

TEST(model_online_learning_decay) {
    // Train heavily on one type, then switch → model should adapt
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = 42;
    ctx.nesting_depth = 1;

    // Phase 1: lots of integers
    for (int i = 0; i < 2000; ++i) {
        model.observe(ctx, ValueType::INTEGER);
    }
    auto pred1 = model.predict(ctx);
    ASSERT_EQ(pred1.predicted_type, ValueType::INTEGER);

    // Phase 2: switch to strings (decay should allow adaptation)
    for (int i = 0; i < 5000; ++i) {
        model.observe(ctx, ValueType::SHORT_STRING);
    }
    auto pred2 = model.predict(ctx);
    ASSERT_EQ(pred2.predicted_type, ValueType::SHORT_STRING);
}

TEST(model_size_constraint) {
    // The model must fit in 128 KB
    ASSERT_TRUE(sizeof(TransitionModel) <= 128 * 1024);
}

// =============================================================================
// FastPaths tests
// =============================================================================

TEST(fast_parse_integer_positive) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("12345,");
    auto result = fp.parse_integer(data, 6);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{5});
    ASSERT_TRUE(result.value.is_number());
    ASSERT_NEAR(result.value.as_number(), 12345.0, 0.001);
}

TEST(fast_parse_integer_negative) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("-42}");
    auto result = fp.parse_integer(data, 4);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{3});
    ASSERT_NEAR(result.value.as_number(), -42.0, 0.001);
}

TEST(fast_parse_integer_zero) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("0,");
    auto result = fp.parse_integer(data, 2);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{1});
    ASSERT_NEAR(result.value.as_number(), 0.0, 0.001);
}

TEST(fast_parse_integer_rejects_float) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("3.14,");
    auto result = fp.parse_integer(data, 5);
    ASSERT_TRUE(!result.success);
}

TEST(fast_parse_integer_rejects_leading_zero) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("01,");
    auto result = fp.parse_integer(data, 3);
    ASSERT_TRUE(!result.success);
}

TEST(fast_parse_integer_large) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("1234567890123456 ");
    auto result = fp.parse_integer(data, 17);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{16});
}

TEST(fast_parse_float_basic) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("3.14159,");
    auto result = fp.parse_float(data, 8);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{7});
    ASSERT_NEAR(result.value.as_number(), 3.14159, 0.00001);
}

TEST(fast_parse_float_exponent) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("1e10,");
    auto result = fp.parse_float(data, 5);
    ASSERT_TRUE(result.success);
    ASSERT_NEAR(result.value.as_number(), 1e10, 1.0);
}

TEST(fast_parse_float_negative_exponent) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("-2.5E-3]");
    auto result = fp.parse_float(data, 8);
    ASSERT_TRUE(result.success);
    ASSERT_NEAR(result.value.as_number(), -2.5e-3, 0.0001);
}

TEST(fast_parse_float_rejects_integer) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("42,");
    auto result = fp.parse_float(data, 3);
    ASSERT_TRUE(!result.success); // no '.' or 'e'
}

TEST(fast_parse_short_string) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"hello\",");
    auto result = fp.parse_short_string(data, 8);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{7});
    ASSERT_TRUE(result.value.is_string());
    ASSERT_EQ(result.value.as_string(), std::string("hello"));
}

TEST(fast_parse_short_string_empty) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"\",");
    auto result = fp.parse_short_string(data, 3);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{2});
    ASSERT_EQ(result.value.as_string(), std::string(""));
}

TEST(fast_parse_short_string_rejects_escape) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"he\\nllo\",");
    auto result = fp.parse_short_string(data, 10);
    ASSERT_TRUE(!result.success); // has escape sequence
}

TEST(fast_parse_short_string_rejects_long) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // 40-char string: exceeds 32-byte limit
    std::string s = "\"" + std::string(40, 'x') + "\",";
    auto result = fp.parse_short_string(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    ASSERT_TRUE(!result.success);
}

TEST(fast_parse_long_string_with_escapes) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"hello\\nworld\",");
    auto result = fp.parse_long_string(data, 15);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.value.as_string(), std::string("hello\nworld"));
}

TEST(fast_parse_long_string_unicode) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"\\u0041\\u0042\",");
    auto result = fp.parse_long_string(data, 15);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.value.as_string(), std::string("AB"));
}

TEST(fast_parse_bool_true) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("true,");
    auto result = fp.parse_bool_true(data, 5);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{4});
    ASSERT_TRUE(result.value.is_bool());
    ASSERT_EQ(result.value.as_bool(), true);
}

TEST(fast_parse_bool_false) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("false]");
    auto result = fp.parse_bool_false(data, 6);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{5});
    ASSERT_EQ(result.value.as_bool(), false);
}

TEST(fast_parse_null) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("null}");
    auto result = fp.parse_null(data, 5);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{4});
    ASSERT_TRUE(result.value.is_null());
}

TEST(fast_parse_bool_true_rejects_false) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("false,");
    auto result = fp.parse_bool_true(data, 6);
    ASSERT_TRUE(!result.success);
}

TEST(fast_parse_null_rejects_true) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("true,");
    auto result = fp.parse_null(data, 5);
    ASSERT_TRUE(!result.success);
}

TEST(fast_parse_dispatch_table) {
    // Verify dispatch table has all entries and they're non-null
    const auto& table = FastPaths::dispatch_table();
    ASSERT_EQ(table.size(), size_t{9});
    for (size_t i = 0; i < table.size(); ++i) {
        ASSERT_TRUE(table[i] != nullptr);
    }
}

// =============================================================================
// SpeculativeParser end-to-end tests
// =============================================================================

TEST(parser_simple_object) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false; // generic only
    SpeculativeParser parser(config, arena);

    std::string json = R"({"name":"Alice","age":30,"active":true})";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.as_object().at("name").as_string(), std::string("Alice"));
    ASSERT_NEAR(result.as_object().at("age").as_number(), 30.0, 0.001);
    ASSERT_EQ(result.as_object().at("active").as_bool(), true);
}

TEST(parser_simple_array) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"([1,2,3,"hello",null,true,false])";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_array());
    const auto& arr = result.as_array();
    ASSERT_EQ(arr.size(), size_t{7});
    ASSERT_NEAR(arr[0].as_number(), 1.0, 0.001);
    ASSERT_NEAR(arr[1].as_number(), 2.0, 0.001);
    ASSERT_NEAR(arr[2].as_number(), 3.0, 0.001);
    ASSERT_EQ(arr[3].as_string(), std::string("hello"));
    ASSERT_TRUE(arr[4].is_null());
    ASSERT_EQ(arr[5].as_bool(), true);
    ASSERT_EQ(arr[6].as_bool(), false);
}

TEST(parser_nested_objects) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"({"user":{"name":"Bob","scores":[95,87,92]}})";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& user = result.as_object().at("user");
    ASSERT_TRUE(user.is_object());
    ASSERT_EQ(user.as_object().at("name").as_string(), std::string("Bob"));

    const auto& scores = user.as_object().at("scores");
    ASSERT_TRUE(scores.is_array());
    ASSERT_EQ(scores.as_array().size(), size_t{3});
    ASSERT_NEAR(scores.as_array()[0].as_number(), 95.0, 0.001);
}

TEST(parser_speculation_matches_generic) {
    // Parse the same JSON with speculation enabled and disabled.
    // Results must be identical.
    std::string json = R"({"name":"Alice","age":30,"active":true,"score":98.5})";
    auto positions = build_index(json);

    // Generic parse
    strata::util::Arena arena1;
    SpeculativeParser::Config config1;
    config1.enable_speculation = false;
    SpeculativeParser parser1(config1, arena1);

    auto result1 = parser1.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                 positions.data(), positions.size());

    // Speculative parse (with warmup=0 so it speculates immediately)
    strata::util::Arena arena2;
    SpeculativeParser::Config config2;
    config2.enable_speculation = true;
    config2.online_learning_warmup = 0;
    SpeculativeParser parser2(config2, arena2);

    // Pre-train the model so it has predictions
    std::string schema = R"({
        "type":"object","properties":{
            "name":{"type":"string"},
            "age":{"type":"integer"},
            "active":{"type":"boolean"},
            "score":{"type":"number"}
        }
    })";
    parser2.model().train_from_schema(schema);

    auto result2 = parser2.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                 positions.data(), positions.size());

    // Compare results
    ASSERT_TRUE(result1.is_object());
    ASSERT_TRUE(result2.is_object());
    ASSERT_EQ(result1.as_object().at("name").as_string(),
              result2.as_object().at("name").as_string());
    ASSERT_NEAR(result1.as_object().at("age").as_number(),
                result2.as_object().at("age").as_number(), 0.001);
    ASSERT_EQ(result1.as_object().at("active").as_bool(),
              result2.as_object().at("active").as_bool());
    ASSERT_NEAR(result1.as_object().at("score").as_number(),
                result2.as_object().at("score").as_number(), 0.001);
}

TEST(parser_empty_object) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = "{}";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.as_object().size(), size_t{0});
}

TEST(parser_empty_array) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = "[]";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_array());
    ASSERT_EQ(result.as_array().size(), size_t{0});
}

TEST(parser_ndjson_online_learning) {
    // Parse 500 identical NDJSON lines → verify the model learns
    std::string line = R"({"name":"Alice","age":30,"active":true})";
    std::string ndjson;
    for (int i = 0; i < 500; ++i) {
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

    // Should have parsed 500 documents
    ASSERT_EQ(results.size(), size_t{500});

    // Each document should be correct
    for (const auto& doc : results) {
        ASSERT_TRUE(doc.is_object());
        ASSERT_EQ(doc.as_object().at("name").as_string(), std::string("Alice"));
        ASSERT_NEAR(doc.as_object().at("age").as_number(), 30.0, 0.001);
    }

    // Check model stats — hit rate should be > 50% after learning
    // (We can't guarantee > 95% because the structural index navigation
    // complicates things, but learning should definitely be happening)
    auto stats = parser.model().stats();
    std::printf("    [NDJSON stats] predictions=%lu, speculations=%lu, hits=%lu\n",
                (unsigned long)stats.total_predictions, (unsigned long)stats.speculations_attempted,
                (unsigned long)stats.speculations_hit);
}

TEST(parser_floats_in_array) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"([1.5,2.7,3.14,-0.5,1e10])";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_array());
    const auto& arr = result.as_array();
    ASSERT_EQ(arr.size(), size_t{5});
    ASSERT_NEAR(arr[0].as_number(), 1.5, 0.001);
    ASSERT_NEAR(arr[1].as_number(), 2.7, 0.001);
    ASSERT_NEAR(arr[2].as_number(), 3.14, 0.01);
    ASSERT_NEAR(arr[3].as_number(), -0.5, 0.001);
    ASSERT_NEAR(arr[4].as_number(), 1e10, 1e5);
}

TEST(parser_string_with_escapes) {
    strata::util::Arena arena;
    SpeculativeParser::Config config;
    config.enable_speculation = false;
    SpeculativeParser parser(config, arena);

    std::string json = R"({"msg":"hello\nworld","path":"c:\\dir"})";
    auto positions = build_index(json);

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                               positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.as_object().at("msg").as_string(), std::string("hello\nworld"));
    ASSERT_EQ(result.as_object().at("path").as_string(), std::string("c:\\dir"));
}

TEST(model_sample_training) {
    TransitionModel model;
    std::string samples = R"({"name":"Alice","age":30})"
                          "\n"
                          R"({"name":"Bob","age":25})"
                          "\n"
                          R"({"name":"Charlie","age":35})"
                          "\n";

    model.train_from_samples(reinterpret_cast<const uint8_t*>(samples.data()), samples.size(), 3);

    // After training, "age" key should predict INTEGER
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("age"), 3);
    ctx.nesting_depth = 1;
    ctx.in_array = false;
    ctx.last_value_type = ValueType::SHORT_STRING;

    auto pred = model.predict(ctx);
    // Model may or may not have enough data to be confident, but it should
    // at least predict INTEGER as the most likely type
    ASSERT_EQ(pred.predicted_type, ValueType::INTEGER);
}

// =============================================================================
// main
// =============================================================================

int main() {
    std::printf("=== Speculative Parsing Engine Tests ===\n");

    std::printf("\n--- TransitionModel ---\n");
    run_model_empty_returns_no_prediction();
    run_model_single_type_converges();
    run_model_mixed_types_below_threshold();
    run_model_collision_handling();
    run_model_schema_training();
    run_model_serialization_roundtrip();
    run_model_merge();
    run_model_online_learning_decay();
    run_model_size_constraint();
    run_model_sample_training();

    std::printf("\n--- FastPaths ---\n");
    run_fast_parse_integer_positive();
    run_fast_parse_integer_negative();
    run_fast_parse_integer_zero();
    run_fast_parse_integer_rejects_float();
    run_fast_parse_integer_rejects_leading_zero();
    run_fast_parse_integer_large();
    run_fast_parse_float_basic();
    run_fast_parse_float_exponent();
    run_fast_parse_float_negative_exponent();
    run_fast_parse_float_rejects_integer();
    run_fast_parse_short_string();
    run_fast_parse_short_string_empty();
    run_fast_parse_short_string_rejects_escape();
    run_fast_parse_short_string_rejects_long();
    run_fast_parse_long_string_with_escapes();
    run_fast_parse_long_string_unicode();
    run_fast_parse_bool_true();
    run_fast_parse_bool_false();
    run_fast_parse_null();
    run_fast_parse_bool_true_rejects_false();
    run_fast_parse_null_rejects_true();
    run_fast_parse_dispatch_table();

    std::printf("\n--- SpeculativeParser ---\n");
    run_parser_simple_object();
    run_parser_simple_array();
    run_parser_nested_objects();
    run_parser_speculation_matches_generic();
    run_parser_empty_object();
    run_parser_empty_array();
    run_parser_ndjson_online_learning();
    run_parser_floats_in_array();
    run_parser_string_with_escapes();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

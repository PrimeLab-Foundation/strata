/**
 * @file test_speculative_edge.cpp
 * @brief Edge-case tests for the speculative parsing engine: decay boundaries,
 *        SWAR digit limits, serialization of empty models, string length
 *        boundaries, container-open validation, stats tracking, and hashing.
 *
 * Uses the project's assert-based test framework (matching test_speculative.cpp).
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

// -- Helper: build structural index for a JSON string -------------------------

[[maybe_unused]]
static std::vector<uint32_t> build_index(const std::string& json) {
    strata::simd::IndexBuilder builder;
    auto idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());
    return idx.positions;
}

// =============================================================================
// 1. decay_at_exactly_256
// =============================================================================

TEST(decay_at_exactly_256) {
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("decay"), 5);
    ctx.nesting_depth = 1;
    ctx.in_array = false;

    // Observe INTEGER exactly 256 times to trigger one decay cycle.
    for (int i = 0; i < 256; ++i) {
        model.observe(ctx, ValueType::INTEGER);
    }

    // After 256 observations the bucket should have decayed: all counts halved.
    // Pre-decay the INTEGER count was 256; after halving it should be ~128.
    uint32_t idx = context_id(ctx) & (TransitionModel::MAX_CONTEXTS - 1);
    const auto& bucket = model.bucket_at(idx);

    uint16_t int_count = bucket.counts[static_cast<size_t>(ValueType::INTEGER)];
    // After decay, the count should be approximately 128 (256 / 2).
    ASSERT_TRUE(int_count >= 126 && int_count <= 130);

    // Total should also reflect the halving.
    ASSERT_TRUE(bucket.total >= 126 && bucket.total <= 130);
}

// =============================================================================
// 2. no_decay_at_255
// =============================================================================

TEST(no_decay_at_255) {
    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("nodecay"), 7);
    ctx.nesting_depth = 1;
    ctx.in_array = false;

    // Observe INTEGER exactly 255 times -- just below the decay threshold.
    for (int i = 0; i < 255; ++i) {
        model.observe(ctx, ValueType::INTEGER);
    }

    uint32_t idx = context_id(ctx) & (TransitionModel::MAX_CONTEXTS - 1);
    const auto& bucket = model.bucket_at(idx);

    uint16_t int_count = bucket.counts[static_cast<size_t>(ValueType::INTEGER)];
    // No decay has occurred, count should be exactly 255.
    ASSERT_EQ(int_count, uint16_t{255});
    ASSERT_EQ(bucket.total, uint32_t{255});
}

// =============================================================================
// 3. swar_exactly_8_digits
// =============================================================================

TEST(swar_exactly_8_digits) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("12345678,");
    auto result = fp.parse_integer(data, 9);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{8});
    ASSERT_NEAR(result.value.as_number(), 12345678.0, 0.001);
}

// =============================================================================
// 4. swar_exactly_16_digits
// =============================================================================

TEST(swar_exactly_16_digits) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // 16 digits: exercises two full SWAR iterations.
    const uint8_t* data = reinterpret_cast<const uint8_t*>("1234567890123456,");
    auto result = fp.parse_integer(data, 17);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{16});
    ASSERT_NEAR(result.value.as_number(), 1234567890123456.0, 1.0);
}

// =============================================================================
// 5. swar_17_digits
// =============================================================================

TEST(swar_17_digits) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // 17 digits: two SWAR iterations + 1 scalar tail digit.
    const uint8_t* data = reinterpret_cast<const uint8_t*>("12345678901234567,");
    auto result = fp.parse_integer(data, 18);
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.bytes_consumed, size_t{17});
    ASSERT_NEAR(result.value.as_number(), 12345678901234567.0, 128.0);
}

// =============================================================================
// 6. integer_overflow_19_digits
// =============================================================================

TEST(integer_overflow_19_digits) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // 20 nines overflows uint64_t (max ~1.8e19, 20 nines = 1e20).
    // The accumulator overflow check should reject this.
    const uint8_t* data = reinterpret_cast<const uint8_t*>("99999999999999999999,");
    auto result = fp.parse_integer(data, 21);
    // Should fail because the value overflows uint64_t during accumulation.
    ASSERT_TRUE(!result.success);
}

// =============================================================================
// 7. serialize_deserialize_empty_model
// =============================================================================

TEST(serialize_deserialize_empty_model) {
    TransitionModel empty_model;

    // Serialize the empty model.
    auto blob = empty_model.serialize();
    ASSERT_TRUE(blob.size() > 0);

    // Deserialize into a new model.
    auto restored = TransitionModel::deserialize(blob.data(), blob.size());

    // Both should predict confidence 0 for any context.
    ParserContext ctx{};
    ctx.key_hash = 42;
    ctx.nesting_depth = 1;

    auto pred_orig = empty_model.predict(ctx);
    auto pred_rest = restored.predict(ctx);

    ASSERT_NEAR(pred_orig.confidence, 0.0, 0.001);
    ASSERT_NEAR(pred_rest.confidence, 0.0, 0.001);
    ASSERT_TRUE(!pred_orig.should_speculate);
    ASSERT_TRUE(!pred_rest.should_speculate);
}

// =============================================================================
// 8. fast_parse_short_string_exactly_32_bytes
// =============================================================================

TEST(fast_parse_short_string_exactly_32_bytes) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // 32 content bytes (no escapes) + surrounding quotes and comma.
    std::string content(32, 'A');
    std::string input = "\"" + content + "\",";
    auto result =
        fp.parse_short_string(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    // 32 bytes is the upper bound for short string -- should succeed.
    ASSERT_TRUE(result.success);
    ASSERT_EQ(result.value.as_string(), content);
}

// =============================================================================
// 9. fast_parse_short_string_33_bytes
// =============================================================================

TEST(fast_parse_short_string_33_bytes) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // 33 content bytes exceeds the 32-byte short-string limit.
    std::string content(33, 'B');
    std::string input = "\"" + content + "\",";
    auto result =
        fp.parse_short_string(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    ASSERT_TRUE(!result.success);
}

// =============================================================================
// 10. fast_parse_object_open
// =============================================================================

TEST(fast_parse_object_open) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("{");
    auto result = fp.parse_object_open(data, 1);
    ASSERT_TRUE(result.success);
}

// =============================================================================
// 11. fast_parse_array_open
// =============================================================================

TEST(fast_parse_array_open) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("[");
    auto result = fp.parse_array_open(data, 1);
    ASSERT_TRUE(result.success);
}

// =============================================================================
// 12. fast_parse_object_open_rejects_bracket
// =============================================================================

TEST(fast_parse_object_open_rejects_bracket) {
    strata::util::Arena arena;
    FastPaths fp(arena);

    // '[' is not '{' -- parse_object_open should reject it.
    const uint8_t* data = reinterpret_cast<const uint8_t*>("[");
    auto result = fp.parse_object_open(data, 1);
    ASSERT_TRUE(!result.success);
}

// =============================================================================
// 13. model_stats_tracking
// =============================================================================

TEST(model_stats_tracking) {
    TransitionModel model;

    // Reset stats to a clean baseline.
    model.reset_stats();

    auto s0 = model.stats();
    ASSERT_EQ(s0.total_predictions, uint64_t{0});
    ASSERT_EQ(s0.correct_predictions, uint64_t{0});
    ASSERT_EQ(s0.speculations_attempted, uint64_t{0});
    ASSERT_EQ(s0.speculations_hit, uint64_t{0});

    // Train the model so it has something to predict.
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("stat"), 4);
    ctx.nesting_depth = 1;
    ctx.in_array = false;

    for (int i = 0; i < 500; ++i) {
        model.observe(ctx, ValueType::INTEGER);
    }

    // Make some predictions and record outcomes.
    model.record_prediction();
    model.record_prediction();
    model.record_prediction();
    model.record_correct_prediction();
    model.record_correct_prediction();
    model.record_speculation_attempted();
    model.record_speculation_attempted();
    model.record_speculation_hit();

    auto s1 = model.stats();
    ASSERT_EQ(s1.total_predictions, uint64_t{3});
    ASSERT_EQ(s1.correct_predictions, uint64_t{2});
    ASSERT_EQ(s1.speculations_attempted, uint64_t{2});
    ASSERT_EQ(s1.speculations_hit, uint64_t{1});

    // Verify derived rates.
    ASSERT_NEAR(s1.hit_rate(), 2.0 / 3.0, 0.01);
    ASSERT_NEAR(s1.speculation_rate(), 0.5, 0.01);
}

// =============================================================================
// 14. context_id_different_for_different_inputs
// =============================================================================

TEST(context_id_different_for_different_inputs) {
    ParserContext ctx_a{};
    ctx_a.key_hash = hash_key(reinterpret_cast<const uint8_t*>("alpha"), 5);
    ctx_a.nesting_depth = 1;
    ctx_a.in_array = false;

    ParserContext ctx_b{};
    ctx_b.key_hash = hash_key(reinterpret_cast<const uint8_t*>("beta"), 4);
    ctx_b.nesting_depth = 1;
    ctx_b.in_array = false;

    ParserContext ctx_c{};
    ctx_c.key_hash = hash_key(reinterpret_cast<const uint8_t*>("alpha"), 5);
    ctx_c.nesting_depth = 2; // different depth
    ctx_c.in_array = true;   // different container

    uint32_t id_a = context_id(ctx_a);
    uint32_t id_b = context_id(ctx_b);
    uint32_t id_c = context_id(ctx_c);

    // All three contexts should produce different IDs.
    ASSERT_TRUE(id_a != id_b);
    ASSERT_TRUE(id_a != id_c);
    ASSERT_TRUE(id_b != id_c);
}

// =============================================================================
// 15. hash_key_consistency
// =============================================================================

TEST(hash_key_consistency) {
    const uint8_t* key1 = reinterpret_cast<const uint8_t*>("temperature");
    size_t len1 = 11;

    const uint8_t* key2 = reinterpret_cast<const uint8_t*>("humidity");
    size_t len2 = 8;

    // Same input must produce the same hash every time.
    uint16_t h1a = hash_key(key1, len1);
    uint16_t h1b = hash_key(key1, len1);
    ASSERT_EQ(h1a, h1b);

    uint16_t h2a = hash_key(key2, len2);
    uint16_t h2b = hash_key(key2, len2);
    ASSERT_EQ(h2a, h2b);

    // Different inputs should (with overwhelming probability) produce different hashes.
    ASSERT_TRUE(h1a != h2a);
}

// =============================================================================
// main
// =============================================================================

int main() {
    std::printf("=== Speculative Parsing Edge-Case Tests ===\n");

    std::printf("\n--- Decay Boundary ---\n");
    run_decay_at_exactly_256();
    run_no_decay_at_255();

    std::printf("\n--- SWAR Integer Parsing ---\n");
    run_swar_exactly_8_digits();
    run_swar_exactly_16_digits();
    run_swar_17_digits();
    run_integer_overflow_19_digits();

    std::printf("\n--- Model Serialization ---\n");
    run_serialize_deserialize_empty_model();

    std::printf("\n--- Short String Boundaries ---\n");
    run_fast_parse_short_string_exactly_32_bytes();
    run_fast_parse_short_string_33_bytes();

    std::printf("\n--- Container Open Validation ---\n");
    run_fast_parse_object_open();
    run_fast_parse_array_open();
    run_fast_parse_object_open_rejects_bracket();

    std::printf("\n--- Stats & Hashing ---\n");
    run_model_stats_tracking();
    run_context_id_different_for_different_inputs();
    run_hash_key_consistency();

    std::printf("\n=== All %d edge-case tests passed ===\n", tests_passed);
    return 0;
}

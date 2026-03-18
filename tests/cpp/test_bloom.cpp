/**
 * @file test_bloom.cpp
 * @brief Tests for the Bloom filter subsystem: core filter, key filter,
 *        schema filter, dedup filter, and selective parser.
 *
 * Uses the project's assert-based test framework.
 */

#include "strata/bloom/bloom_filter.h"
#include "strata/bloom/dedup_filter.h"
#include "strata/bloom/key_filter.h"
#include "strata/bloom/parser_integration.h"
#include "strata/bloom/schema_filter.h"
#include "strata/bloom/sizing.h"
#include "strata/simd/index_builder.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <random>
#include <string>
#include <vector>

using namespace strata::bloom;

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

#define ASSERT_LT(a, b)                                                                            \
    do {                                                                                           \
        auto _a = (a);                                                                             \
        auto _b = (b);                                                                             \
        if (!(_a < _b)) {                                                                          \
            std::fprintf(stderr, "FAIL: %s:%d: %s >= %s\n", __FILE__, __LINE__, #a, #b);           \
        }                                                                                          \
        assert(_a < _b);                                                                           \
        (void)_a;                                                                                  \
        (void)_b;                                                                                  \
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

// ── Helper: generate random string ───────────────────────────────────────────

static std::string random_key(std::mt19937& rng, size_t min_len = 3, size_t max_len = 20) {
    std::uniform_int_distribution<size_t> len_dist(min_len, max_len);
    std::uniform_int_distribution<int> char_dist('a', 'z');
    size_t len = len_dist(rng);
    std::string s;
    s.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        s += static_cast<char>(char_dist(rng));
    }
    return s;
}

// =============================================================================
// 1. Core Bloom Filter Tests
// =============================================================================

TEST(bloom_insert_and_find) {
    BloomFilter<256, 3> filter;

    // Insert known keys.
    filter.insert("name");
    filter.insert("age");
    filter.insert("email");
    filter.insert("address");
    filter.insert("phone");

    // All inserted keys must be found.
    ASSERT_TRUE(filter.maybe_contains("name"));
    ASSERT_TRUE(filter.maybe_contains("age"));
    ASSERT_TRUE(filter.maybe_contains("email"));
    ASSERT_TRUE(filter.maybe_contains("address"));
    ASSERT_TRUE(filter.maybe_contains("phone"));
}

TEST(bloom_no_false_negatives) {
    // Insert N keys, verify all are found.  Bloom filters guarantee no false negatives.
    BloomFilter<512, 3> filter;
    std::mt19937 rng(42);

    std::vector<std::string> keys;
    for (int i = 0; i < 30; ++i) {
        keys.push_back(random_key(rng));
        filter.insert(keys.back());
    }

    for (const auto& key : keys) {
        ASSERT_TRUE(filter.maybe_contains(key));
    }
}

TEST(bloom_false_positive_rate) {
    // Verify FPR is within 1.5x of theoretical bound.
    // For m=512 bits, k=3, n=30 keys:
    //   Theoretical FPR ≈ (1 - e^(-3*30/512))^3 ≈ (1 - e^(-0.1758))^3 ≈ 0.161^3 ≈ 0.0042
    //   Allow up to 1.5x → < 0.0063

    BloomFilter<512, 3> filter;
    std::mt19937 rng(42);

    // Insert 30 keys.
    std::vector<std::string> inserted;
    for (int i = 0; i < 30; ++i) {
        auto key = "key_" + std::to_string(i) + "_" + random_key(rng, 5, 10);
        filter.insert(key);
        inserted.push_back(key);
    }

    // Test 10000 random keys that were NOT inserted.
    size_t false_positives = 0;
    size_t total_queries = 10000;
    for (size_t i = 0; i < total_queries; ++i) {
        auto test_key = "test_" + std::to_string(i) + "_" + random_key(rng, 5, 10);
        if (filter.maybe_contains(test_key)) {
            ++false_positives;
        }
    }

    double measured_fpr = static_cast<double>(false_positives) / static_cast<double>(total_queries);

    // Theoretical FPR for m=512, k=3, n=30:
    double kn_over_m = 3.0 * 30.0 / 512.0;
    double theoretical_fpr = std::pow(1.0 - std::exp(-kn_over_m), 3.0);

    std::printf("    FPR: measured=%.4f%%, theoretical=%.4f%%, ratio=%.2f\n", measured_fpr * 100.0,
                theoretical_fpr * 100.0, measured_fpr / theoretical_fpr);

    // Must be within 1.5x (with some margin for statistical variation).
    // Use 2.0x to account for randomness in small samples.
    ASSERT_LT(measured_fpr, theoretical_fpr * 2.0);
}

TEST(bloom_empty_filter) {
    BloomFilter<256, 3> filter;
    ASSERT_FALSE(filter.maybe_contains("anything"));
    ASSERT_EQ(filter.count_set_bits(), size_t{0});
    ASSERT_NEAR(filter.fill_ratio(), 0.0, 1e-10);
}

TEST(bloom_clear) {
    BloomFilter<256, 3> filter;
    filter.insert("hello");
    ASSERT_TRUE(filter.maybe_contains("hello"));
    ASSERT_GT(filter.count_set_bits(), size_t{0});

    filter.clear();
    ASSERT_EQ(filter.count_set_bits(), size_t{0});
    // After clear, "hello" might still get false positive, but fill_ratio should be 0.
    ASSERT_NEAR(filter.fill_ratio(), 0.0, 1e-10);
}

TEST(bloom_insert_all) {
    BloomFilter<256, 3> filter;
    filter.insert_all({"alpha", "beta", "gamma", "delta"});

    ASSERT_TRUE(filter.maybe_contains("alpha"));
    ASSERT_TRUE(filter.maybe_contains("beta"));
    ASSERT_TRUE(filter.maybe_contains("gamma"));
    ASSERT_TRUE(filter.maybe_contains("delta"));
}

TEST(bloom_merge) {
    BloomFilter<256, 3> filter_a;
    BloomFilter<256, 3> filter_b;

    filter_a.insert("key_a1");
    filter_a.insert("key_a2");
    filter_b.insert("key_b1");
    filter_b.insert("key_b2");

    // Before merge: each filter only has its own keys.
    ASSERT_TRUE(filter_a.maybe_contains("key_a1"));
    ASSERT_TRUE(filter_a.maybe_contains("key_a2"));
    ASSERT_TRUE(filter_b.maybe_contains("key_b1"));
    ASSERT_TRUE(filter_b.maybe_contains("key_b2"));

    // Merge b into a.
    filter_a.merge(filter_b);

    // After merge: a has all keys (union semantics).
    ASSERT_TRUE(filter_a.maybe_contains("key_a1"));
    ASSERT_TRUE(filter_a.maybe_contains("key_a2"));
    ASSERT_TRUE(filter_a.maybe_contains("key_b1"));
    ASSERT_TRUE(filter_a.maybe_contains("key_b2"));

    // b is unchanged.
    ASSERT_TRUE(filter_b.maybe_contains("key_b1"));
    ASSERT_TRUE(filter_b.maybe_contains("key_b2"));
}

TEST(bloom_fill_ratio) {
    BloomFilter<256, 3> filter;

    // Each insert sets up to 3 bits (some might overlap).
    filter.insert("key1");
    double ratio = filter.fill_ratio();
    ASSERT_GT(ratio, 0.0);
    ASSERT_LT(ratio, 1.0);

    // Insert many keys — fill ratio should increase.
    for (int i = 0; i < 100; ++i) {
        filter.insert("key_" + std::to_string(i));
    }
    ASSERT_GT(filter.fill_ratio(), ratio);
}

TEST(bloom_count_set_bits) {
    BloomFilter<256, 3> filter;
    ASSERT_EQ(filter.count_set_bits(), size_t{0});

    filter.insert("single_key");
    size_t bits = filter.count_set_bits();
    // With 3 hash functions, we expect 1-3 bits set (some might collide).
    ASSERT_GT(bits, size_t{0});
    ASSERT_TRUE(bits <= 3);
}

// ── Blocked filter tests ─────────────────────────────────────────────────────

TEST(bloom_blocked_filter) {
    // MediumFilter uses blocked design (2048 bits > 512).
    MediumFilter filter;

    std::mt19937 rng(123);
    std::vector<std::string> keys;
    for (int i = 0; i < 100; ++i) {
        keys.push_back(random_key(rng));
        filter.insert(keys.back());
    }

    // No false negatives.
    for (const auto& key : keys) {
        ASSERT_TRUE(filter.maybe_contains(key));
    }
}

// ── wyhash test vectors ──────────────────────────────────────────────────────

TEST(wyhash_empty_string) {
    // Empty string with seed 0 should produce a consistent non-zero hash.
    uint64_t h = detail::wyhash(nullptr, 0, 0);
    // Not checking against a specific value (implementation-dependent),
    // but it must be deterministic.
    uint64_t h2 = detail::wyhash(nullptr, 0, 0);
    ASSERT_EQ(h, h2);
}

TEST(wyhash_deterministic) {
    const uint8_t data[] = "hello world";
    uint64_t h1 = detail::wyhash(data, 11, 42);
    uint64_t h2 = detail::wyhash(data, 11, 42);
    ASSERT_EQ(h1, h2);

    // Different seed → different hash.
    uint64_t h3 = detail::wyhash(data, 11, 43);
    ASSERT_TRUE(h1 != h3);
}

TEST(wyhash_different_inputs) {
    const uint8_t a[] = "hello";
    const uint8_t b[] = "world";
    uint64_t ha = detail::wyhash(a, 5, 0);
    uint64_t hb = detail::wyhash(b, 5, 0);
    ASSERT_TRUE(ha != hb);
}

TEST(wyhash_various_lengths) {
    // Test strings of various lengths to exercise all branches.
    const char* strings[] = {
        "",                                                   // 0 bytes
        "a",                                                  // 1 byte
        "ab",                                                 // 2 bytes
        "abc",                                                // 3 bytes
        "abcd",                                               // 4 bytes
        "abcdefgh",                                           // 8 bytes
        "abcdefghijklmnop",                                   // 16 bytes
        "abcdefghijklmnopqrstuvwx",                           // 24 bytes
        "abcdefghijklmnopqrstuvwxyz0123456789",               // 36 bytes
        "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMN", // 50 bytes
    };

    // Just verify they don't crash and produce different values.
    uint64_t prev = 0;
    for (const char* s : strings) {
        size_t len = std::strlen(s);
        uint64_t h = detail::wyhash(reinterpret_cast<const uint8_t*>(s), len, 0);
        if (len > 0) {
            ASSERT_TRUE(h != prev); // Extremely unlikely to collide.
        }
        prev = h;
    }
}

// =============================================================================
// 2. Sizing Tests
// =============================================================================

TEST(sizing_optimal_bits) {
    // For n=10, p=0.01: m ≈ -(10 * ln(0.01)) / (ln(2))^2 ≈ 95.85
    size_t bits = optimal_bits(10, 0.01);
    ASSERT_GT(bits, size_t{80});
    ASSERT_LT(bits, size_t{120});
}

TEST(sizing_optimal_hashes) {
    // For m=256, n=10: k ≈ (256/10) * ln(2) ≈ 17.7 → clamped to 8
    size_t k = optimal_hashes(256, 10);
    ASSERT_TRUE(k >= 2 && k <= 8);

    // For m=96, n=10: k ≈ (96/10) * 0.693 ≈ 6.65 → 7
    size_t k2 = optimal_hashes(96, 10);
    ASSERT_TRUE(k2 >= 2 && k2 <= 8);
}

TEST(sizing_align_bits) {
    ASSERT_EQ(align_bits(1), size_t{256});
    ASSERT_EQ(align_bits(256), size_t{256});
    ASSERT_EQ(align_bits(257), size_t{512});
    ASSERT_EQ(align_bits(512), size_t{512});
    ASSERT_EQ(align_bits(1000), size_t{1024});
}

TEST(sizing_presets_compile) {
    // Verify presets compile and have reasonable sizes.
    static_assert(TinyFilter::NUM_BITS >= 256, "TinyFilter too small");
    static_assert(SmallFilter::NUM_BITS >= 256, "SmallFilter too small");
    static_assert(MediumFilter::NUM_BITS >= 256, "MediumFilter too small");
    static_assert(LargeFilter::NUM_BITS >= 256, "LargeFilter too small");

    static_assert(TinyFilter::NUM_HASHES >= 2, "TinyFilter too few hashes");
    static_assert(SmallFilter::NUM_HASHES >= 2, "SmallFilter too few hashes");
    static_assert(MediumFilter::NUM_HASHES >= 2, "MediumFilter too few hashes");
    static_assert(LargeFilter::NUM_HASHES >= 2, "LargeFilter too few hashes");

    // SmallFilter should be larger than TinyFilter.
    static_assert(SmallFilter::NUM_BITS >= TinyFilter::NUM_BITS);
    // MediumFilter should be larger than SmallFilter.
    static_assert(MediumFilter::NUM_BITS >= SmallFilter::NUM_BITS);
}

// =============================================================================
// 3. KeyFilter Tests
// =============================================================================

TEST(key_filter_basic) {
    std::vector<std::string_view> desired = {"name", "age", "email"};
    KeyFilter filter(desired);

    ASSERT_TRUE(filter.should_parse("name"));
    ASSERT_TRUE(filter.should_parse("age"));
    ASSERT_TRUE(filter.should_parse("email"));
    ASSERT_FALSE(filter.should_parse("phone"));
    ASSERT_FALSE(filter.should_parse("address"));
}

TEST(key_filter_reject_rate) {
    // Build filter with 5 keys, query 500 random keys → reject rate > 98%.
    std::vector<std::string_view> desired = {"name", "age", "email", "city", "zip"};
    KeyFilter filter(desired);

    std::mt19937 rng(999);
    size_t rejected = 0;
    size_t total = 500;

    for (size_t i = 0; i < total; ++i) {
        auto key = "random_key_" + std::to_string(i) + "_" + random_key(rng, 5, 15);
        if (!filter.should_parse(key)) {
            ++rejected;
        }
    }

    double reject_rate = static_cast<double>(rejected) / static_cast<double>(total);
    std::printf("    KeyFilter reject rate: %.1f%% (%zu / %zu)\n", reject_rate * 100.0, rejected,
                total);
    ASSERT_GT(reject_rate, 0.98);
}

TEST(key_filter_stats) {
    std::vector<std::string_view> desired = {"name", "age"};
    KeyFilter filter(desired);

    (void)filter.should_parse("name");     // true positive
    (void)filter.should_parse("age");      // true positive
    (void)filter.should_parse("unknown1"); // rejected
    (void)filter.should_parse("unknown2"); // rejected

    auto stats = filter.stats();
    ASSERT_EQ(stats.total_checks, uint64_t{4});
    ASSERT_EQ(stats.exact_matches, uint64_t{2});
    // bloom_rejects should be >= 0 (depends on false positives).
    ASSERT_TRUE(stats.bloom_rejects + stats.bloom_positives == stats.total_checks);
}

TEST(key_filter_check_action) {
    std::vector<std::string_view> desired = {"name"};
    KeyFilter filter(desired);

    // "name" should get UNCERTAIN (Bloom positive).
    auto action = filter.check("name");
    ASSERT_TRUE(action == KeyFilter::Action::UNCERTAIN);

    // Random string should likely get SKIP.
    // (Very unlikely to be a false positive for a single-key filter.)
    auto action2 = filter.check("zzz_nonexistent_key_12345");
    ASSERT_TRUE(action2 == KeyFilter::Action::SKIP);
}

TEST(key_filter_from_paths) {
    std::vector<std::string_view> paths = {"$.user.name", "$.items[*].price", "$.data"};
    auto filter = KeyFilter::from_paths(paths);

    ASSERT_TRUE(filter.should_parse("name"));
    ASSERT_TRUE(filter.should_parse("price"));
    ASSERT_TRUE(filter.should_parse("data"));
    ASSERT_FALSE(filter.should_parse("unknown_field_xyz"));
}

TEST(key_filter_empty) {
    // Empty filter should accept everything.
    std::vector<std::string_view> empty;
    KeyFilter filter(empty);
    ASSERT_TRUE(filter.should_parse("anything"));
    ASSERT_TRUE(filter.should_parse("at_all"));
}

// =============================================================================
// 4. SchemaFilter Tests
// =============================================================================

TEST(schema_filter_flat_keys) {
    std::vector<std::string_view> allowed = {"name", "age", "email", "phone"};
    SchemaFilter filter(allowed);

    auto r1 = filter.validate("name");
    ASSERT_TRUE(r1.is_valid);

    auto r2 = filter.validate("unknown_field");
    // Should be rejected by Bloom (definitely not in schema).
    ASSERT_FALSE(r2.is_valid);
}

TEST(schema_filter_validate_batch) {
    std::vector<std::string_view> allowed = {"name", "age", "email"};
    SchemaFilter filter(allowed);

    std::string_view keys[] = {"name", "bad_key", "email", "another_bad"};
    uint64_t mask = filter.validate_batch(keys, 4);

    // bit 0 (name) and bit 2 (email) should be set.
    ASSERT_TRUE(mask & (uint64_t{1} << 0)); // name
    ASSERT_TRUE(mask & (uint64_t{1} << 2)); // email

    // Verify batch matches individual validate calls.
    for (size_t i = 0; i < 4; ++i) {
        bool batch_result = (mask & (uint64_t{1} << i)) != 0;
        // Bloom filter positive in batch should match individual.
        // Note: validate() does exact check, so is_valid is definitive.
        // But batch only does Bloom check, so it might have false positives.
        if (!batch_result) {
            // If batch says no, individual Bloom should also say no.
            ASSERT_FALSE(filter.validate(keys[i]).is_valid);
        }
    }
}

TEST(schema_filter_depth_aware) {
    SchemaFilter filter(std::span<const std::string_view>{});

    // Add keys at different depths.
    std::vector<std::string_view> depth0_keys = {"name", "age"};
    std::vector<std::string_view> depth1_keys = {"street", "city"};
    filter.add_keys_at_depth(depth0_keys, 0);
    filter.add_keys_at_depth(depth1_keys, 1);

    // "name" valid at depth 0.
    auto r1 = filter.validate_at_depth("name", 0);
    ASSERT_TRUE(r1.is_valid);

    // "street" valid at depth 1.
    auto r2 = filter.validate_at_depth("street", 1);
    ASSERT_TRUE(r2.is_valid);

    // "street" should NOT be valid at depth 0.
    auto r3 = filter.validate_at_depth("street", 0);
    ASSERT_FALSE(r3.is_valid);
}

TEST(schema_filter_from_json_schema) {
    // Minimal JSON Schema with properties.
    std::string schema = R"({
        "type": "object",
        "properties": {
            "name": { "type": "string" },
            "age": { "type": "integer" },
            "address": {
                "type": "object",
                "properties": {
                    "street": { "type": "string" },
                    "city": { "type": "string" }
                }
            }
        }
    })";

    SchemaFilter filter(std::string_view{schema});

    // All property names should be in the global filter.
    auto r1 = filter.validate("name");
    ASSERT_TRUE(r1.is_valid);

    auto r2 = filter.validate("age");
    ASSERT_TRUE(r2.is_valid);

    auto r3 = filter.validate("street");
    ASSERT_TRUE(r3.is_valid);

    // Unknown key should be rejected.
    auto r4 = filter.validate("zzz_not_in_schema_12345");
    ASSERT_FALSE(r4.is_valid);
}

// =============================================================================
// 5. DedupFilter Tests
// =============================================================================

TEST(dedup_no_false_trigger_unique) {
    DedupFilter dedup;

    // Insert unique keys — none should trigger duplicate suspect.
    ASSERT_FALSE(dedup.check_and_insert("key_a"));
    ASSERT_FALSE(dedup.check_and_insert("key_b"));
    ASSERT_FALSE(dedup.check_and_insert("key_c"));
    ASSERT_FALSE(dedup.check_and_insert("key_d"));
    ASSERT_FALSE(dedup.check_and_insert("key_e"));

    ASSERT_EQ(dedup.total_checks(), uint64_t{5});
    ASSERT_EQ(dedup.duplicate_suspects(), uint64_t{0});
}

TEST(dedup_detect_actual_duplicate) {
    DedupFilter dedup;

    ASSERT_FALSE(dedup.check_and_insert("name"));
    ASSERT_FALSE(dedup.check_and_insert("age"));

    // Insert "name" again — must be detected as a suspect.
    bool is_dup = dedup.check_and_insert("name");
    ASSERT_TRUE(is_dup);
    ASSERT_EQ(dedup.duplicate_suspects(), uint64_t{1});
}

TEST(dedup_push_pop_nesting) {
    DedupFilter dedup;

    // Depth 0: insert "name".
    ASSERT_FALSE(dedup.check_and_insert("name"));

    // Enter nested object (depth 1).
    dedup.push();
    ASSERT_EQ(dedup.current_depth(), uint8_t{1});

    // "name" at depth 1 should NOT be a duplicate (different scope).
    ASSERT_FALSE(dedup.check_and_insert("name"));

    // "name" at depth 1 again → suspect.
    ASSERT_TRUE(dedup.check_and_insert("name"));

    // Leave nested object.
    dedup.pop();
    ASSERT_EQ(dedup.current_depth(), uint8_t{0});

    // "name" at depth 0 again → suspect (still in depth 0 filter).
    ASSERT_TRUE(dedup.check_and_insert("name"));
}

TEST(dedup_reset) {
    DedupFilter dedup;
    (void)dedup.check_and_insert("a");
    (void)dedup.check_and_insert("b");
    dedup.push();
    (void)dedup.check_and_insert("c");

    dedup.reset();
    ASSERT_EQ(dedup.current_depth(), uint8_t{0});
    ASSERT_EQ(dedup.total_checks(), uint64_t{0});
    ASSERT_EQ(dedup.duplicate_suspects(), uint64_t{0});

    // After reset, "a" should not be a duplicate.
    ASSERT_FALSE(dedup.check_and_insert("a"));
}

TEST(dedup_deep_nesting) {
    DedupFilter dedup;

    // Push to max depth -1.
    for (int i = 0; i < 31; ++i) {
        dedup.push();
        ASSERT_FALSE(dedup.check_and_insert("nested_key"));
    }

    // Pop all the way back.
    for (int i = 0; i < 31; ++i) {
        dedup.pop();
    }
    ASSERT_EQ(dedup.current_depth(), uint8_t{0});
}

// =============================================================================
// 6. SelectiveParser Tests
// =============================================================================

TEST(selective_parser_basic_object) {
    std::string json =
        R"({"name":"Alice","age":30,"email":"alice@example.com","phone":"555-1234"})";

    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"name", "age"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    // Result should be an object with only "name" and "age".
    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto name_it = obj.find("name");
    ASSERT_TRUE(name_it != obj.end());
    ASSERT_TRUE(name_it->second.is_string());

    auto age_it = obj.find("age");
    ASSERT_TRUE(age_it != obj.end());

    // "email" and "phone" should NOT be present (skipped).
    ASSERT_TRUE(obj.find("email") == obj.end());
    ASSERT_TRUE(obj.find("phone") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_GT(stats.values_skipped, uint64_t{0});
    std::printf("    SelectiveParser: parsed=%lu, skipped=%lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped);
}

TEST(selective_parser_all_keys) {
    // When filter is empty (no desired keys), parse everything.
    std::string json = R"({"a":1,"b":2,"c":3})";
    auto positions = build_index(json);

    std::vector<std::string_view> empty;
    KeyFilter filter(empty);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);
    ASSERT_EQ(obj.size(), size_t{3});
}

TEST(selective_parser_string_values) {
    std::string json = R"({"first":"John","last":"Doe","middle":"Q"})";
    auto positions = build_index(json);

    std::vector<std::string_view> desired = {"first", "last"};
    KeyFilter filter(desired);
    SelectiveParser parser(filter);

    auto result = parser.parse_selective(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                                         positions.data(), positions.size());

    ASSERT_TRUE(result.is_object());
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);

    auto first_it = obj.find("first");
    ASSERT_TRUE(first_it != obj.end());
    ASSERT_TRUE(first_it->second.is_string());
    ASSERT_EQ(std::get<std::string>(first_it->second.data), std::string("John"));

    auto last_it = obj.find("last");
    ASSERT_TRUE(last_it != obj.end());
    ASSERT_EQ(std::get<std::string>(last_it->second.data), std::string("Doe"));

    // "middle" should be skipped.
    ASSERT_TRUE(obj.find("middle") == obj.end());
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::printf("=== Bloom Filter Subsystem Tests ===\n\n");

    std::printf("── Core Bloom Filter ──\n");
    run_bloom_insert_and_find();
    run_bloom_no_false_negatives();
    run_bloom_false_positive_rate();
    run_bloom_empty_filter();
    run_bloom_clear();
    run_bloom_insert_all();
    run_bloom_merge();
    run_bloom_fill_ratio();
    run_bloom_count_set_bits();
    run_bloom_blocked_filter();

    std::printf("\n── wyhash ──\n");
    run_wyhash_empty_string();
    run_wyhash_deterministic();
    run_wyhash_different_inputs();
    run_wyhash_various_lengths();

    std::printf("\n── Sizing ──\n");
    run_sizing_optimal_bits();
    run_sizing_optimal_hashes();
    run_sizing_align_bits();
    run_sizing_presets_compile();

    std::printf("\n── KeyFilter ──\n");
    run_key_filter_basic();
    run_key_filter_reject_rate();
    run_key_filter_stats();
    run_key_filter_check_action();
    run_key_filter_from_paths();
    run_key_filter_empty();

    std::printf("\n── SchemaFilter ──\n");
    run_schema_filter_flat_keys();
    run_schema_filter_validate_batch();
    run_schema_filter_depth_aware();
    run_schema_filter_from_json_schema();

    std::printf("\n── DedupFilter ──\n");
    run_dedup_no_false_trigger_unique();
    run_dedup_detect_actual_duplicate();
    run_dedup_push_pop_nesting();
    run_dedup_reset();
    run_dedup_deep_nesting();

    std::printf("\n── SelectiveParser ──\n");
    run_selective_parser_basic_object();
    run_selective_parser_all_keys();
    run_selective_parser_string_values();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

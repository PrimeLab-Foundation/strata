/**
 * @file test_bloom_edge.cpp
 * @brief Edge-case tests for the Bloom filter subsystem: dedup boundaries,
 *        empty keys, tiny filter saturation, schema edge cases, wyhash
 *        branch coverage, and sizing corner cases.
 *
 * Uses the project's assert-based test framework.
 */

#include "strata/bloom/bloom_filter.h"
#include "strata/bloom/dedup_filter.h"
#include "strata/bloom/key_filter.h"
#include "strata/bloom/schema_filter.h"
#include "strata/bloom/sizing.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <random>
#include <set>
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

// =============================================================================
// 1. dedup_max_depth_boundary
//    Push 31 times (MAX_DEPTH=32, last valid index=31).
//    Insert key at depth 31. Verify no crash and check_and_insert works.
// =============================================================================

TEST(dedup_max_depth_boundary) {
    DedupFilter dedup;

    // Push 31 times to reach depth 31 (last valid index).
    for (int i = 0; i < 31; ++i) {
        dedup.push();
    }
    ASSERT_EQ(dedup.current_depth(), uint8_t{31});

    // Insert a key at depth 31 — must not crash.
    ASSERT_FALSE(dedup.check_and_insert("boundary_key"));

    // Inserting the same key again should detect a possible duplicate.
    ASSERT_TRUE(dedup.check_and_insert("boundary_key"));

    // A different key should not be flagged.
    ASSERT_FALSE(dedup.check_and_insert("other_key"));

    ASSERT_EQ(dedup.total_checks(), uint64_t{3});
}

// =============================================================================
// 2. dedup_push_beyond_max_depth
//    Push 32 times. Verify depth doesn't go beyond 31.
//    The guard `if (depth_ + 1 < MAX_DEPTH)` prevents it.
// =============================================================================

TEST(dedup_push_beyond_max_depth) {
    DedupFilter dedup;

    // Push 32 times — should cap at depth 31.
    for (int i = 0; i < 32; ++i) {
        dedup.push();
    }
    // The guard prevents going beyond index 31 (MAX_DEPTH - 1).
    ASSERT_EQ(dedup.current_depth(), uint8_t{31});

    // Push more — should still be clamped.
    dedup.push();
    dedup.push();
    ASSERT_EQ(dedup.current_depth(), uint8_t{31});

    // Insertions should still work at the clamped depth.
    ASSERT_FALSE(dedup.check_and_insert("clamped_key"));
    ASSERT_TRUE(dedup.check_and_insert("clamped_key"));
}

// =============================================================================
// 3. dedup_pop_at_zero
//    Call pop() when depth is 0. Verify depth stays 0.
// =============================================================================

TEST(dedup_pop_at_zero) {
    DedupFilter dedup;

    ASSERT_EQ(dedup.current_depth(), uint8_t{0});

    // Pop at depth 0 — should be a no-op, depth stays at 0.
    dedup.pop();
    ASSERT_EQ(dedup.current_depth(), uint8_t{0});

    dedup.pop();
    dedup.pop();
    ASSERT_EQ(dedup.current_depth(), uint8_t{0});

    // Filter should still work normally after spurious pops.
    ASSERT_FALSE(dedup.check_and_insert("safe_key"));
    ASSERT_TRUE(dedup.check_and_insert("safe_key"));
}

// =============================================================================
// 4. bloom_empty_string_key
//    Insert "" (empty string) into BloomFilter.
//    Verify maybe_contains("") returns true.
// =============================================================================

TEST(bloom_empty_string_key) {
    BloomFilter<256, 3> filter;

    // Insert empty string.
    filter.insert("");
    ASSERT_TRUE(filter.maybe_contains(""));

    // The filter should have set some bits.
    ASSERT_GT(filter.count_set_bits(), size_t{0});

    // A non-empty key should likely not be found (filter is sparse).
    // (This is probabilistic but with 256 bits and 1 key, FPR is very low.)
    // We don't assert on non-empty keys since it's probabilistic.
}

// =============================================================================
// 5. bloom_tiny_filter_saturation
//    Insert 1000 keys into TinyFilter (256 bits).
//    Verify fill_ratio > 0.99 and FPR approaches 1.0.
// =============================================================================

TEST(bloom_tiny_filter_saturation) {
    TinyFilter filter;

    // Insert 1000 keys — way more than a 256-bit filter can handle.
    for (int i = 0; i < 1000; ++i) {
        filter.insert("saturation_key_" + std::to_string(i));
    }

    double ratio = filter.fill_ratio();
    std::printf("    TinyFilter saturation fill_ratio: %.4f\n", ratio);
    ASSERT_GT(ratio, 0.99);

    // Estimated FPR should be extremely high (approaching 1.0).
    double fpr = filter.estimated_false_positive_rate();
    std::printf("    TinyFilter saturation FPR: %.4f\n", fpr);
    ASSERT_GT(fpr, 0.95);

    // Any random key should be "found" (virtually all bits set).
    ASSERT_TRUE(filter.maybe_contains("never_inserted_key_xyz"));
}

// =============================================================================
// 6. schema_filter_empty_schema
//    Create SchemaFilter with empty allowed keys list.
//    Verify validate() returns false for any key.
// =============================================================================

TEST(schema_filter_empty_schema) {
    std::vector<std::string_view> empty_keys;
    std::span<const std::string_view> keys_span(empty_keys);
    SchemaFilter filter(keys_span);

    // With no allowed keys, any key should be rejected.
    auto r1 = filter.validate("name");
    ASSERT_FALSE(r1.is_valid);

    auto r2 = filter.validate("age");
    ASSERT_FALSE(r2.is_valid);

    auto r3 = filter.validate("anything_at_all");
    ASSERT_FALSE(r3.is_valid);

    auto r4 = filter.validate("");
    ASSERT_FALSE(r4.is_valid);

    ASSERT_EQ(filter.num_keys(), size_t{0});
}

// =============================================================================
// 7. schema_validate_batch_zero_count
//    Call validate_batch with count=0.
//    Verify returns 0 mask (no keys checked).
// =============================================================================

TEST(schema_validate_batch_zero_count) {
    std::vector<std::string_view> allowed = {"name", "age"};
    std::span<const std::string_view> allowed_span{allowed};
    SchemaFilter filter(allowed_span);

    // Pass count=0 with a valid pointer.
    std::string_view dummy = "dummy";
    uint64_t mask = filter.validate_batch(&dummy, 0);
    ASSERT_EQ(mask, uint64_t{0});
}

// =============================================================================
// 8. schema_validate_batch_max_count
//    Call validate_batch with count=64 on an array of 64 keys.
//    Verify no crash.
// =============================================================================

TEST(schema_validate_batch_max_count) {
    // Build a schema with some known keys.
    std::vector<std::string_view> allowed = {"key_0", "key_1", "key_2", "key_3", "key_4"};
    std::span<const std::string_view> allowed_span{allowed};
    SchemaFilter filter(allowed_span);

    // Build an array of 64 keys.
    std::vector<std::string> key_storage;
    key_storage.reserve(64);
    for (int i = 0; i < 64; ++i) {
        key_storage.push_back("key_" + std::to_string(i));
    }

    std::vector<std::string_view> keys;
    keys.reserve(64);
    for (const auto& k : key_storage) {
        keys.push_back(k);
    }

    // Call with count=64 — must not crash.
    uint64_t mask = filter.validate_batch(keys.data(), 64);

    // Bits for the allowed keys (key_0 through key_4) should be set.
    // At minimum, bit 0 through 4 should be set (Bloom filter has no false negatives).
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE((mask & (uint64_t{1} << i)) != 0);
    }

    std::printf("    validate_batch(64): mask=0x%016llx\n", (unsigned long long)mask);
}

// =============================================================================
// 9. key_filter_single_key
//    KeyFilter with just "target".
//    Verify should_parse("target")==true, should_parse("other")==false.
// =============================================================================

TEST(key_filter_single_key) {
    std::vector<std::string_view> desired = {"target"};
    KeyFilter filter(desired);

    ASSERT_TRUE(filter.should_parse("target"));
    ASSERT_FALSE(filter.should_parse("other"));
    ASSERT_FALSE(filter.should_parse(""));
    ASSERT_FALSE(filter.should_parse("TARGET")); // Case-sensitive.
    ASSERT_FALSE(filter.should_parse("target_extra"));

    auto stats = filter.stats();
    ASSERT_EQ(stats.total_checks, uint64_t{5});
    ASSERT_EQ(stats.exact_matches, uint64_t{1});
}

// =============================================================================
// 10. wyhash_all_branch_lengths
//     Test wyhash with string lengths 0,1,2,3,4,8,16,17,24,48,49,50
//     to exercise all branches in the wyhash implementation.
//     Verify determinism (same input -> same hash) and diversity
//     (different inputs -> different hashes).
// =============================================================================

TEST(wyhash_all_branch_lengths) {
    // Build test strings of specific lengths to exercise every branch:
    //   len==0:     a = b = 0  (zero-length path)
    //   len 1-3:    wyr3 path
    //   len 4-16:   wyr4 path
    //   len 17-48:  >16 bytes path (while loop for 16-byte steps, then wyr8 tail)
    //   len >48:    bulk 48-byte stride loop
    struct TestCase {
        size_t length;
        const char* label;
    };

    TestCase cases[] = {
        {0, "0 bytes"},   {1, "1 byte"},    {2, "2 bytes"},   {3, "3 bytes"},
        {4, "4 bytes"},   {8, "8 bytes"},   {16, "16 bytes"}, {17, "17 bytes"},
        {24, "24 bytes"}, {48, "48 bytes"}, {49, "49 bytes"}, {50, "50 bytes"},
    };

    // Generate deterministic test data of sufficient length.
    std::string data(64, 'A');
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<char>('A' + (i % 26));
    }

    std::set<uint64_t> hashes;

    for (const auto& tc : cases) {
        auto* ptr = reinterpret_cast<const uint8_t*>(data.data());

        // Determinism: hashing the same input twice yields the same result.
        uint64_t h1 = detail::wyhash(tc.length == 0 ? nullptr : ptr, tc.length, 0);
        uint64_t h2 = detail::wyhash(tc.length == 0 ? nullptr : ptr, tc.length, 0);
        ASSERT_EQ(h1, h2);

        hashes.insert(h1);
    }

    // Diversity: all different lengths should produce different hashes.
    // With 12 distinct inputs, we expect 12 distinct hashes.
    // (Collisions are astronomically unlikely with 64-bit hashes.)
    ASSERT_EQ(hashes.size(), size_t{12});
}

// =============================================================================
// 11. bloom_large_filter_blocked
//     Create LargeFilter (10240 bits, BLOCKED=true).
//     Insert 500 keys. Verify all found. Verify FPR below 2%.
// =============================================================================

TEST(bloom_large_filter_blocked) {
    // LargeFilter is 10240 bits with BLOCKED=true.
    static_assert(LargeFilter::BLOCKED, "LargeFilter should use blocked design");

    LargeFilter filter;

    std::mt19937 rng(777);

    // Insert 500 keys.
    std::vector<std::string> inserted;
    inserted.reserve(500);
    for (int i = 0; i < 500; ++i) {
        std::string key = "large_" + std::to_string(i) + "_";
        // Add some random suffix for diversity.
        std::uniform_int_distribution<int> dist('a', 'z');
        for (int j = 0; j < 8; ++j) {
            key += static_cast<char>(dist(rng));
        }
        filter.insert(key);
        inserted.push_back(key);
    }

    // No false negatives: all inserted keys must be found.
    for (const auto& key : inserted) {
        ASSERT_TRUE(filter.maybe_contains(key));
    }

    // Measure false positive rate with 10000 random probes.
    size_t false_positives = 0;
    size_t total_probes = 10000;
    for (size_t i = 0; i < total_probes; ++i) {
        std::string probe = "probe_" + std::to_string(i) + "_";
        std::uniform_int_distribution<int> dist('a', 'z');
        for (int j = 0; j < 8; ++j) {
            probe += static_cast<char>(dist(rng));
        }
        if (filter.maybe_contains(probe)) {
            ++false_positives;
        }
    }

    double fpr = static_cast<double>(false_positives) / static_cast<double>(total_probes);
    std::printf("    LargeFilter blocked FPR: %.4f%% (%zu / %zu)\n", fpr * 100.0, false_positives,
                total_probes);

    // FPR should be well below 2% for 500 keys in a 10240-bit filter.
    ASSERT_LT(fpr, 0.02);
}

// =============================================================================
// 12. sizing_zero_keys
//     Call optimal_bits(0, 0.01). Verify returns 256 (minimum).
// =============================================================================

TEST(sizing_zero_keys) {
    size_t bits = optimal_bits(0, 0.01);
    ASSERT_EQ(bits, size_t{256});

    // Also test optimal_hashes with 0 keys — should return default of 3.
    size_t k = optimal_hashes(256, 0);
    ASSERT_EQ(k, size_t{3});
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::printf("=== Bloom Filter Edge-Case Tests ===\n\n");

    std::printf("-- DedupFilter boundaries --\n");
    run_dedup_max_depth_boundary();
    run_dedup_push_beyond_max_depth();
    run_dedup_pop_at_zero();

    std::printf("\n-- BloomFilter edge cases --\n");
    run_bloom_empty_string_key();
    run_bloom_tiny_filter_saturation();

    std::printf("\n-- SchemaFilter edge cases --\n");
    run_schema_filter_empty_schema();
    run_schema_validate_batch_zero_count();
    run_schema_validate_batch_max_count();

    std::printf("\n-- KeyFilter edge cases --\n");
    run_key_filter_single_key();

    std::printf("\n-- wyhash branch coverage --\n");
    run_wyhash_all_branch_lengths();

    std::printf("\n-- LargeFilter blocked --\n");
    run_bloom_large_filter_blocked();

    std::printf("\n-- Sizing edge cases --\n");
    run_sizing_zero_keys();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

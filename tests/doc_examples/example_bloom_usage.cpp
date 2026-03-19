/**
 * @file example_bloom_usage.cpp
 * @brief Documentation example tests for the Bloom filter subsystem
 *        (docs/api/bloom_api.md).
 *
 * Verifies that the code patterns shown in the Bloom API documentation
 * compile and produce correct results.
 */

#include "strata/bloom/bloom_filter.h"
#include "strata/bloom/dedup_filter.h"
#include "strata/bloom/key_filter.h"
#include "strata/bloom/parser_integration.h"
#include "strata/simd/dispatch.h"
#include "strata/simd/index_builder.h"

#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
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

// ---------------------------------------------------------------------------
// Test 1: BloomFilter — insert keys and query
// ---------------------------------------------------------------------------

TEST(bloom_filter_insert_query) {
    BloomFilter<256, 3> filter;

    filter.insert("name");
    filter.insert("age");
    filter.insert("email");

    // Inserted keys must be found (no false negatives).
    ASSERT_TRUE(filter.maybe_contains("name"));
    ASSERT_TRUE(filter.maybe_contains("age"));
    ASSERT_TRUE(filter.maybe_contains("email"));

    // A key that was never inserted is very unlikely to be found.
    // (Cannot assert false due to probabilistic nature, but we can
    //  check fill ratio is reasonable.)
    ASSERT_TRUE(filter.count_set_bits() > 0);
    ASSERT_TRUE(filter.fill_ratio() < 0.5);

    std::printf("    Fill ratio after 3 inserts: %.2f%%\n", filter.fill_ratio() * 100.0);
}

// ---------------------------------------------------------------------------
// Test 2: KeyFilter — check should_parse
// ---------------------------------------------------------------------------

TEST(key_filter_should_parse) {
    // Create a key filter for only "name" and "age" (from Usage Example).
    std::array<std::string_view, 2> desired = {"name", "age"};
    KeyFilter filter(desired);

    // Desired keys pass.
    ASSERT_TRUE(filter.should_parse("name"));
    ASSERT_TRUE(filter.should_parse("age"));

    // Unrelated keys are rejected.
    ASSERT_TRUE(!filter.should_parse("email"));
    ASSERT_TRUE(!filter.should_parse("phone"));
    ASSERT_TRUE(!filter.should_parse("address"));

    auto stats = filter.stats();
    ASSERT_EQ(stats.total_checks, uint64_t{5});
    std::printf("    KeyFilter: %lu checks, %lu exact matches, %lu bloom rejects\n",
                (unsigned long)stats.total_checks, (unsigned long)stats.exact_matches,
                (unsigned long)stats.bloom_rejects);
}

// ---------------------------------------------------------------------------
// Test 3: DedupFilter — check_and_insert for duplicates
// ---------------------------------------------------------------------------

TEST(dedup_filter_duplicates) {
    DedupFilter dedup;

    // First insertion of unique keys should not trigger a duplicate suspect.
    ASSERT_TRUE(!dedup.check_and_insert("name"));
    ASSERT_TRUE(!dedup.check_and_insert("age"));
    ASSERT_TRUE(!dedup.check_and_insert("email"));

    // Inserting "name" again must be flagged as a possible duplicate.
    ASSERT_TRUE(dedup.check_and_insert("name"));

    ASSERT_EQ(dedup.total_checks(), uint64_t{4});
    ASSERT_EQ(dedup.duplicate_suspects(), uint64_t{1});

    // Nesting: push into a nested object scope.
    dedup.push();
    ASSERT_EQ(dedup.current_depth(), uint8_t{1});

    // "name" in nested scope is NOT a duplicate (different scope).
    ASSERT_TRUE(!dedup.check_and_insert("name"));

    // Pop back to parent scope.
    dedup.pop();
    ASSERT_EQ(dedup.current_depth(), uint8_t{0});
}

// ---------------------------------------------------------------------------
// Test 4: SelectiveParser — parse selective with KeyFilter (from Usage Example)
// ---------------------------------------------------------------------------

TEST(selective_parser_usage) {
    const char* json = R"({
        "name": "Alice", "age": 30, "email": "alice@example.com",
        "address": {"city": "Portland", "state": "OR", "zip": "97201"},
        "scores": [95, 87, 92], "active": true
    })";
    auto len = std::strlen(json);
    auto data = reinterpret_cast<const uint8_t*>(json);

    // Build structural index.
    auto index = strata::simd::index_document(data, len);

    // Create a key filter for only "name" and "age".
    std::array<std::string_view, 2> desired = {"name", "age"};
    KeyFilter filter(desired);

    // Parse selectively.
    SelectiveParser parser(filter);
    auto result = parser.parse_selective(data, len, index.positions.data(), index.positions.size());

    // Result should be an object containing the selected keys.
    ASSERT_TRUE(result.is_object());

    // "name" and "age" should be present.
    const auto& obj = std::get<strata::JsonValue::Object>(result.data);
    ASSERT_TRUE(obj.find("name") != obj.end());
    ASSERT_TRUE(obj.find("age") != obj.end());

    // "email", "address", "scores", "active" should be skipped.
    ASSERT_TRUE(obj.find("email") == obj.end());
    ASSERT_TRUE(obj.find("address") == obj.end());
    ASSERT_TRUE(obj.find("scores") == obj.end());
    ASSERT_TRUE(obj.find("active") == obj.end());

    auto stats = parser.parse_stats();
    ASSERT_TRUE(stats.values_skipped > 0);
    std::printf("    Parsed: %lu, Skipped: %lu, Keys checked: %lu\n",
                (unsigned long)stats.values_parsed, (unsigned long)stats.values_skipped,
                (unsigned long)stats.keys_checked);
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main() {
    std::printf("=== Doc Example: Bloom Filter Usage ===\n");

    run_bloom_filter_insert_query();
    run_key_filter_should_parse();
    run_dedup_filter_duplicates();
    run_selective_parser_usage();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

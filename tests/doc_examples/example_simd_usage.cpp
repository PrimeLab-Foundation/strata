/**
 * @file example_simd_usage.cpp
 * @brief Documentation example tests for the SIMD subsystem (docs/api/simd_api.md).
 *
 * Verifies that the code patterns shown in the SIMD API documentation
 * compile and produce correct results.
 */

#include "strata/simd/classifier.h"
#include "strata/simd/dispatch.h"
#include "strata/simd/index_builder.h"
#include "strata/simd/structural_indexer.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <string>

using namespace strata::simd;

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
// Test 1: Detect backend and print name (from Usage Example in docs)
// ---------------------------------------------------------------------------

TEST(detect_backend_and_print) {
    auto backend = detect_backend();
    const char* name = backend_name(backend);
    std::printf("    Detected backend: %s\n", name);

    // backend_name must return a non-null, non-empty string.
    ASSERT_TRUE(name != nullptr);
    ASSERT_TRUE(std::strlen(name) > 0);

    // The detected backend must be one of the defined enum values.
    ASSERT_TRUE(backend == Backend::SCALAR || backend == Backend::AVX2 ||
                backend == Backend::AVX2_BMI2 || backend == Backend::AVX512 ||
                backend == Backend::NEON || backend == Backend::SVE2 ||
                backend == Backend::WASM_SIMD || backend == Backend::RVV);
}

// ---------------------------------------------------------------------------
// Test 2: Build structural index from a JSON string (from Usage Example)
// ---------------------------------------------------------------------------

TEST(build_structural_index) {
    const char* json = R"({"name": "Alice", "age": 30, "scores": [95, 87, 92]})";
    auto len = std::strlen(json);

    auto index = index_document(reinterpret_cast<const uint8_t*>(json), len);

    ASSERT_EQ(index.document_length, len);
    ASSERT_TRUE(!index.positions.empty());

    // Every position in the index must point to a structural character.
    for (auto pos : index.positions) {
        char c = json[pos];
        ASSERT_TRUE(c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',');
    }
}

// ---------------------------------------------------------------------------
// Test 3: Verify structural positions contain expected chars
// ---------------------------------------------------------------------------

TEST(structural_positions_expected_chars) {
    const char* json = R"({"key":[1,2,3]})";
    auto len = std::strlen(json);

    auto index = index_document(reinterpret_cast<const uint8_t*>(json), len);

    std::set<uint32_t> pos_set(index.positions.begin(), index.positions.end());

    // { at offset 0
    ASSERT_TRUE(pos_set.count(0) == 1);
    ASSERT_EQ(json[0], '{');

    // : at offset 6 (after "key")
    ASSERT_TRUE(pos_set.count(6) == 1);
    ASSERT_EQ(json[6], ':');

    // [ at offset 7
    ASSERT_TRUE(pos_set.count(7) == 1);
    ASSERT_EQ(json[7], '[');

    // ] at offset 13
    ASSERT_TRUE(pos_set.count(13) == 1);
    ASSERT_EQ(json[13], ']');

    // } at offset 14
    ASSERT_TRUE(pos_set.count(14) == 1);
    ASSERT_EQ(json[14], '}');
}

// ---------------------------------------------------------------------------
// Test 4: Classify a single byte using scalar classifier
// ---------------------------------------------------------------------------

TEST(classify_scalar_byte) {
    // Structural characters must have the structural bit set.
    for (char c : {'{', '}', '[', ']', ':', ','}) {
        uint8_t cls = Classifier::classify_scalar(static_cast<uint8_t>(c));
        ASSERT_TRUE((cls & ClassMask::kStructural) != 0);
    }

    // Quote must have the quote bit set.
    ASSERT_TRUE((Classifier::classify_scalar('"') & ClassMask::kQuote) != 0);

    // Backslash must have the backslash bit set.
    ASSERT_TRUE((Classifier::classify_scalar('\\') & ClassMask::kBackslash) != 0);

    // Whitespace characters must have the whitespace bit set.
    for (char c : {' ', '\t', '\n', '\r'}) {
        ASSERT_TRUE(
            (Classifier::classify_scalar(static_cast<uint8_t>(c)) & ClassMask::kWhitespace) != 0);
    }

    // Regular ASCII letters/digits must classify as zero (no class).
    ASSERT_EQ(Classifier::classify_scalar('a'), 0);
    ASSERT_EQ(Classifier::classify_scalar('5'), 0);
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main() {
    std::printf("=== Doc Example: SIMD Usage ===\n");

    run_detect_backend_and_print();
    run_build_structural_index();
    run_structural_positions_expected_chars();
    run_classify_scalar_byte();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

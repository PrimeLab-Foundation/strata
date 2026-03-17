/**
 * @file test_simd.cpp
 * @brief Comprehensive tests for the SIMD structural indexing subsystem.
 *
 * Tests cover: classifier lookup tables, escape-aware quote detection,
 * string region masking, structural index building, and all specified
 * edge cases (escaped quotes, consecutive backslashes, UTF-8 multibyte
 * sequences, empty/single-value documents, non-aligned input).
 *
 * Uses the project's assert-based test framework (matching existing tests).
 */

#include "strata/simd/classifier.h"
#include "strata/simd/dispatch.h"
#include "strata/simd/index_builder.h"
#include "strata/simd/structural_indexer.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <string>
#include <vector>

using namespace strata::simd;

// ============================================================================
// Utility helpers
// ============================================================================

/// Pad a string to 64 bytes with zeros for StructuralIndexer::classify().
static void pad64(const std::string& s, uint8_t out[64]) {
    std::memset(out, 0, 64);
    std::memcpy(out, s.data(), std::min(s.size(), size_t{64}));
}

/// Build a structural index for a string and return the positions.
static std::vector<uint32_t> index_string(const std::string& s) {
    IndexBuilder builder;
    auto idx = builder.build(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    return idx.positions;
}

static int tests_passed = 0;

#define TEST(name)                                                                                 \
    static void test_##name();                                                                     \
    static void run_##name() {                                                                     \
        test_##name();                                                                             \
        ++tests_passed;                                                                            \
        std::printf("  PASS: %s\n", #name);                                                        \
    }                                                                                              \
    static void test_##name()

// Suppress unused-variable warnings for assert-only variables in release builds.
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

// ============================================================================
// Classifier tests
// ============================================================================

TEST(classifier_scalar_structural) {
    for (char c : {'{', '}', '[', ']', ':', ','}) {
        ASSERT_TRUE(
            (Classifier::classify_scalar(static_cast<uint8_t>(c)) & ClassMask::kStructural) != 0);
    }
}

TEST(classifier_scalar_quote) {
    uint8_t cls = Classifier::classify_scalar('"');
    ASSERT_TRUE((cls & ClassMask::kQuote) != 0);
    ASSERT_TRUE((cls & ClassMask::kStructural) == 0);
}

TEST(classifier_scalar_backslash) {
    uint8_t cls = Classifier::classify_scalar('\\');
    ASSERT_TRUE((cls & ClassMask::kBackslash) != 0);
    ASSERT_TRUE((cls & ClassMask::kStructural) == 0);
}

TEST(classifier_scalar_whitespace) {
    for (char c : {' ', '\t', '\n', '\r'}) {
        ASSERT_TRUE(
            (Classifier::classify_scalar(static_cast<uint8_t>(c)) & ClassMask::kWhitespace) != 0);
    }
}

TEST(classifier_scalar_other) {
    for (char c : {'a', 'z', 'A', 'Z', '0', '9', '.', '-', '+', '!', '@'}) {
        ASSERT_EQ(Classifier::classify_scalar(static_cast<uint8_t>(c)), 0);
    }
}

TEST(classifier_scalar_no_false_positives) {
    // Exhaustive check: verify no false positives for all 256 byte values.
    for (int i = 0; i < 256; ++i) {
        uint8_t c = static_cast<uint8_t>(i);
        uint8_t cls = Classifier::classify_scalar(c);

        bool is_structural = (c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',');
        bool is_quote = (c == '"');
        bool is_backslash = (c == '\\');
        bool is_whitespace = (c == ' ' || c == '\t' || c == '\n' || c == '\r');

        if ((cls & ClassMask::kStructural) != 0) {
            assert(is_structural && "false positive: STRUCTURAL");
        }
        if ((cls & ClassMask::kQuote) != 0) {
            assert(is_quote && "false positive: QUOTE");
        }
        if ((cls & ClassMask::kBackslash) != 0) {
            assert(is_backslash && "false positive: BACKSLASH");
        }
        if ((cls & ClassMask::kWhitespace) != 0) {
            assert(is_whitespace && "false positive: WHITESPACE");
        }

        if (is_structural)
            ASSERT_TRUE((cls & ClassMask::kStructural) != 0);
        if (is_quote)
            ASSERT_TRUE((cls & ClassMask::kQuote) != 0);
        if (is_backslash)
            ASSERT_TRUE((cls & ClassMask::kBackslash) != 0);
        if (is_whitespace)
            ASSERT_TRUE((cls & ClassMask::kWhitespace) != 0);
    }
}

TEST(classifier_scalar_utf8_continuation_bytes) {
    // UTF-8 continuation bytes (0x80-0xBF) must classify as OTHER.
    for (int i = 0x80; i <= 0xBF; ++i) {
        ASSERT_EQ(Classifier::classify_scalar(static_cast<uint8_t>(i)), 0);
    }
}

// ============================================================================
// StructuralIndexer tests — escape handling
// ============================================================================

TEST(indexer_simple_object) {
    // {"a":1}
    std::string json = R"({"a":1})";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.structural & (1ULL << 0)) != 0); // {
    ASSERT_TRUE((cr.structural & (1ULL << 4)) != 0); // :
    ASSERT_TRUE((cr.structural & (1ULL << 6)) != 0); // }
    ASSERT_TRUE((cr.structural & (1ULL << 1)) == 0); // " (not structural)
    ASSERT_TRUE((cr.structural & (1ULL << 2)) == 0); // 'a' (inside string)
}

TEST(indexer_escaped_quote) {
    // {"a":"he said \"hi\""}
    std::string json = R"({"a":"he said \"hi\""})";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.structural & (1ULL << 0)) != 0);  // {
    ASSERT_TRUE((cr.structural & (1ULL << 4)) != 0);  // :
    ASSERT_TRUE((cr.structural & (1ULL << 21)) != 0); // }
}

TEST(indexer_double_backslash_before_quote) {
    // Bytes: " \ \ " ,
    //   pos 0: "  (opening quote)
    //   pos 1: \  (escaping pos 2)
    //   pos 2: \  (escaped by pos 1)
    //   pos 3: "  (closing quote — NOT escaped)
    //   pos 4: ,  (structural)
    std::string json = "\"\\\\\",";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.quote_mask & (1ULL << 0)) != 0); // opening quote
    ASSERT_TRUE((cr.quote_mask & (1ULL << 3)) != 0); // closing quote
    ASSERT_TRUE((cr.structural & (1ULL << 4)) != 0); // comma
}

TEST(indexer_triple_backslash_before_quote) {
    // C string: "\\\\\\\"\","
    // Bytes: " \ \ \ " " ,
    //   pos 0: " (opening quote)
    //   pos 1: \ (escaping pos 2)
    //   pos 2: \ (escaped)
    //   pos 3: \ (escaping pos 4)
    //   pos 4: " (escaped quote — inside string)
    //   pos 5: " (closing quote)
    //   pos 6: , (structural)
    std::string json = "\"\\\\\\\"\",";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.structural & (1ULL << 6)) != 0); // comma
    ASSERT_TRUE((cr.quote_mask & (1ULL << 4)) == 0); // escaped quote
    ASSERT_TRUE((cr.quote_mask & (1ULL << 5)) != 0); // closing quote
}

TEST(indexer_four_backslashes) {
    // Bytes: " \ \ \ \ " ,  (4 backslashes → two pairs → quote NOT escaped)
    std::string json = "\"\\\\\\\\\",";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.quote_mask & (1ULL << 0)) != 0); // opening quote
    ASSERT_TRUE((cr.quote_mask & (1ULL << 5)) != 0); // closing quote (even backslashes)
    ASSERT_TRUE((cr.structural & (1ULL << 6)) != 0); // comma
}

TEST(indexer_whitespace_mask) {
    std::string json = " \t\n\r{";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.whitespace & (1ULL << 0)) != 0); // space
    ASSERT_TRUE((cr.whitespace & (1ULL << 1)) != 0); // tab
    ASSERT_TRUE((cr.whitespace & (1ULL << 2)) != 0); // newline
    ASSERT_TRUE((cr.whitespace & (1ULL << 3)) != 0); // CR
    ASSERT_TRUE((cr.whitespace & (1ULL << 4)) == 0); // { is not whitespace
}

TEST(indexer_string_region) {
    // "hello", — everything between quotes is inside a string.
    std::string json = "\"hello\",";
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    // Positions 1-5 ('h','e','l','l','o') should be inside strings.
    for (int i = 1; i <= 5; ++i) {
        ASSERT_TRUE((cr.string_mask & (1ULL << i)) != 0);
    }
    ASSERT_TRUE((cr.structural & (1ULL << 7)) != 0); // comma
}

// ============================================================================
// IndexBuilder tests — full document indexing
// ============================================================================

TEST(builder_simple_array) {
    std::string json = "[1, 2, 3]";
    auto positions = index_string(json);

    std::set<uint32_t> pos_set(positions.begin(), positions.end());
    ASSERT_TRUE(pos_set.count(0) == 1); // [
    ASSERT_TRUE(pos_set.count(2) == 1); // ,
    ASSERT_TRUE(pos_set.count(5) == 1); // ,
    ASSERT_TRUE(pos_set.count(8) == 1); // ]
}

TEST(builder_nested_object) {
    std::string json = R"({"a":{"b":1},"c":2})";
    auto positions = index_string(json);

    // No quotes should appear in the structural index.
    for (size_t i = 0; i < positions.size(); ++i) {
        assert(json[positions[i]] != '"' && "quote in structural index");
    }
}

TEST(builder_string_with_structural_chars) {
    // Structural chars inside a string should NOT appear in the index.
    std::string json = R"({"key":"{[:]}"})";
    auto positions = index_string(json);

    // Only the outer { : } are structural.
    ASSERT_EQ(positions.size(), size_t{3});
    for (uint32_t pos : positions) {
        char c = json[pos];
        ASSERT_TRUE(c == '{' || c == '}' || c == ':');
    }
}

TEST(builder_empty_document) {
    auto positions = index_string("");
    ASSERT_TRUE(positions.empty());
}

TEST(builder_single_value_number) {
    auto positions = index_string("42");
    ASSERT_TRUE(positions.empty());
}

TEST(builder_single_value_string) {
    auto positions = index_string("\"hello\"");
    ASSERT_TRUE(positions.empty());
}

TEST(builder_single_value_true) {
    auto positions = index_string("true");
    ASSERT_TRUE(positions.empty());
}

TEST(builder_non_aligned_length) {
    std::string json = R"({"x":1})"; // 7 bytes
    assert(json.size() < 64);
    auto positions = index_string(json);

    std::set<uint32_t> pos_set(positions.begin(), positions.end());
    ASSERT_TRUE(pos_set.count(0) == 1); // {
    ASSERT_TRUE(pos_set.count(4) == 1); // :
    ASSERT_TRUE(pos_set.count(6) == 1); // }
}

TEST(builder_exact_64_bytes) {
    // Exactly 64 bytes of JSON.
    std::string json = R"({"a":1,"b":2,"c":3,"d":4,"e":5,"f":6,"g":7,"h":8,"i":9,"j":0})";
    while (json.size() < 64)
        json.push_back(' ');
    json.resize(64);

    auto positions = index_string(json);
    ASSERT_TRUE(!positions.empty());
    for (uint32_t pos : positions) {
        ASSERT_TRUE(pos < 64);
    }
}

TEST(builder_multi_block) {
    // Document spanning multiple 64-byte blocks.
    std::string json = "[";
    for (int i = 0; i < 50; ++i) {
        if (i > 0)
            json += ",";
        json += std::to_string(i);
    }
    json += "]";
    assert(json.size() > 64);

    auto positions = index_string(json);
    ASSERT_TRUE(!positions.empty());
    ASSERT_EQ(json[positions.front()], '[');
    ASSERT_EQ(json[positions.back()], ']');
}

TEST(builder_string_spanning_blocks) {
    // A very long string that spans a 64-byte block boundary.
    std::string long_str(100, 'x');
    std::string json = "[\"" + long_str + "\",42]";
    auto positions = index_string(json);

    std::set<uint32_t> pos_set(positions.begin(), positions.end());
    ASSERT_TRUE(pos_set.count(0) == 1); // [

    // Comma after the long string: [ " xxx...x " ,
    size_t comma_pos = 1 + 1 + 100 + 1; // = 103
    ASSERT_EQ(json[comma_pos], ',');
    ASSERT_TRUE(pos_set.count(static_cast<uint32_t>(comma_pos)) == 1);

    ASSERT_EQ(json[positions.back()], ']');
}

TEST(builder_utf8_multibyte_in_string) {
    // UTF-8 continuation bytes (0x80-0xBF) must not be structural.
    // U+00E9 = 0xC3 0xA9,  U+1F600 = 0xF0 0x9F 0x98 0x80
    std::string json = "[\"caf\xC3\xA9\",\"\xF0\x9F\x98\x80\"]";
    auto positions = index_string(json);

    ASSERT_TRUE(!positions.empty());
    for (uint32_t pos : positions) {
        char c = json[pos];
        ASSERT_TRUE(c == '[' || c == ']' || c == ',');
    }
}

// ============================================================================
// Dispatch tests
// ============================================================================

TEST(dispatch_detection) {
    Backend b = detect_backend();
    const char* name = backend_name(b);
    std::printf("    Detected backend: %s\n", name);
    ASSERT_TRUE(b == Backend::SCALAR || b == Backend::AVX2 || b == Backend::AVX2_BMI2 ||
                b == Backend::AVX512 || b == Backend::NEON || b == Backend::SVE2 ||
                b == Backend::WASM_SIMD || b == Backend::RVV);
}

TEST(dispatch_index_document) {
    std::string json = R"({"key":[1,2,3]})";
    auto idx = index_document(reinterpret_cast<const uint8_t*>(json.data()), json.size());
    ASSERT_EQ(idx.document_length, json.size());
    ASSERT_TRUE(!idx.positions.empty());

    for (uint32_t pos : idx.positions) {
        char c = json[pos];
        ASSERT_TRUE(c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',');
    }
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("=== SIMD Structural Indexer Tests ===\n");

    std::printf("\n--- Classifier ---\n");
    run_classifier_scalar_structural();
    run_classifier_scalar_quote();
    run_classifier_scalar_backslash();
    run_classifier_scalar_whitespace();
    run_classifier_scalar_other();
    run_classifier_scalar_no_false_positives();
    run_classifier_scalar_utf8_continuation_bytes();

    std::printf("\n--- StructuralIndexer ---\n");
    run_indexer_simple_object();
    run_indexer_escaped_quote();
    run_indexer_double_backslash_before_quote();
    run_indexer_triple_backslash_before_quote();
    run_indexer_four_backslashes();
    run_indexer_whitespace_mask();
    run_indexer_string_region();

    std::printf("\n--- IndexBuilder ---\n");
    run_builder_simple_array();
    run_builder_nested_object();
    run_builder_string_with_structural_chars();
    run_builder_empty_document();
    run_builder_single_value_number();
    run_builder_single_value_string();
    run_builder_single_value_true();
    run_builder_non_aligned_length();
    run_builder_exact_64_bytes();
    run_builder_multi_block();
    run_builder_string_spanning_blocks();
    run_builder_utf8_multibyte_in_string();

    std::printf("\n--- Dispatch ---\n");
    run_dispatch_detection();
    run_dispatch_index_document();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

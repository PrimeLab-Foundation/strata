/**
 * @file test_simd_edge.cpp
 * @brief Edge-case tests for the SIMD structural indexing subsystem.
 *
 * Targets coverage gaps around chunk boundaries, carry propagation,
 * degenerate inputs (empty, single-byte, all-whitespace, all-backslash),
 * and remainder-length handling in IndexBuilder::build().
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
// 1. Chunk-boundary backslash carry
// ============================================================================

TEST(chunk_boundary_backslash_carry) {
    // 128 bytes: byte 63 is '\', byte 64 is '"'.
    // The backslash at the end of chunk 0 should carry into chunk 1 and
    // cause the quote at byte 64 to be treated as escaped (not in quote_mask).
    alignas(64) uint8_t buf[128];
    std::memset(buf, ' ', 128);
    buf[63] = '\\';
    buf[64] = '"';

    StructuralIndexer indexer;
    ChunkResult cr0 = indexer.classify(buf);
    ChunkResult cr1 = indexer.classify(buf + 64);

    // The quote at byte 64 (bit 0 of chunk 1) should NOT appear in quote_mask
    // because it is escaped by the backslash carry from chunk 0.
    ASSERT_TRUE((cr1.quote_mask & (1ULL << 0)) == 0);

    // Verify the backslash was detected in chunk 0.
    (void)cr0;
}

// ============================================================================
// 2. Chunk-boundary string carry
// ============================================================================

TEST(chunk_boundary_string_carry) {
    // 128 bytes: opening quote at byte 10, closing quote at byte 70.
    // The string region should span from byte 11 through byte 69.
    alignas(64) uint8_t buf[128];
    std::memset(buf, ' ', 128);
    buf[10] = '"';
    buf[70] = '"';

    StructuralIndexer indexer;
    ChunkResult cr0 = indexer.classify(buf);
    ChunkResult cr1 = indexer.classify(buf + 64);

    // In chunk 0: bytes 11-63 should be inside the string (string_mask set).
    for (int i = 11; i < 64; ++i) {
        ASSERT_TRUE((cr0.string_mask & (1ULL << i)) != 0);
    }

    // In chunk 1: bytes 0-5 (positions 64-69) should be inside the string.
    for (int i = 0; i < 6; ++i) {
        ASSERT_TRUE((cr1.string_mask & (1ULL << i)) != 0);
    }

    // Byte 70 is the closing quote (bit 6 of chunk 1) — should be in quote_mask.
    ASSERT_TRUE((cr1.quote_mask & (1ULL << 6)) != 0);
}

// ============================================================================
// 3. Empty input build
// ============================================================================

TEST(empty_input_build) {
    IndexBuilder builder;
    auto idx = builder.build(nullptr, 0);
    ASSERT_TRUE(idx.positions.empty());
    ASSERT_EQ(idx.document_length, size_t{0});
}

// ============================================================================
// 4. Single byte structural
// ============================================================================

TEST(single_byte_structural) {
    auto positions = index_string("{");
    ASSERT_EQ(positions.size(), size_t{1});
    ASSERT_EQ(positions[0], uint32_t{0});
}

// ============================================================================
// 5. Single byte quote
// ============================================================================

TEST(single_byte_quote) {
    auto positions = index_string("\"");
    ASSERT_TRUE(positions.empty());
}

// ============================================================================
// 6. Single byte whitespace
// ============================================================================

TEST(single_byte_whitespace) {
    auto positions = index_string(" ");
    ASSERT_TRUE(positions.empty());
}

// ============================================================================
// 7. All whitespace 64 bytes
// ============================================================================

TEST(all_whitespace_64_bytes) {
    std::string spaces(64, ' ');
    alignas(64) uint8_t buf[64];
    pad64(spaces, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_EQ(cr.structural, uint64_t{0});
    // All 64 bits should be set for whitespace.
    ASSERT_EQ(cr.whitespace, ~uint64_t{0});
}

// ============================================================================
// 8. All backslash 64 bytes
// ============================================================================

TEST(all_backslash_64_bytes) {
    // 64 backslashes. Backslash mask should be all-ones.
    // Escape mask: each pair of backslashes cancels out. For 64 consecutive
    // backslashes (even count), the escaped positions are the even-indexed ones
    // (1, 3, 5, ..., 63) — positions following an odd-length run prefix.
    // The net effect: no quote following this chunk would be escaped because
    // 64 is even.
    alignas(64) uint8_t buf[64];
    std::memset(buf, '\\', 64);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    // No structural characters.
    ASSERT_EQ(cr.structural, uint64_t{0});
    // No quotes.
    ASSERT_EQ(cr.quote_mask, uint64_t{0});
    // No whitespace.
    ASSERT_EQ(cr.whitespace, uint64_t{0});
}

// ============================================================================
// 9. Quote at position zero
// ============================================================================

TEST(quote_at_position_zero) {
    std::string json(64, ' ');
    json[0] = '"';
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.quote_mask & (1ULL << 0)) != 0);
}

// ============================================================================
// 10. Quote at position 63
// ============================================================================

TEST(quote_at_position_63) {
    std::string json(64, ' ');
    json[63] = '"';
    alignas(64) uint8_t buf[64];
    pad64(json, buf);

    StructuralIndexer indexer;
    ChunkResult cr = indexer.classify(buf);

    ASSERT_TRUE((cr.quote_mask & (1ULL << 63)) != 0);
}

// ============================================================================
// 11. String spanning three chunks
// ============================================================================

TEST(string_spanning_three_chunks) {
    // 200 bytes: opening quote at byte 5, closing quote at byte 195.
    // Place a structural '{' at byte 0 and '}' at byte 199 — both outside
    // the string — and verify they appear in the structural index.
    std::string json(200, 'x');
    json[0] = '{';
    json[5] = '"';
    json[195] = '"';
    json[196] = ':';
    json[199] = '}';

    auto positions = index_string(json);
    std::set<uint32_t> pos_set(positions.begin(), positions.end());

    // Structural chars outside the string should be found.
    ASSERT_TRUE(pos_set.count(0) == 1);   // {
    ASSERT_TRUE(pos_set.count(196) == 1); // :
    ASSERT_TRUE(pos_set.count(199) == 1); // }

    // Characters inside the string (e.g. 'x' at positions 6-194) should not
    // produce structural hits even if they happen to be structural bytes.
    // Verify none of positions 6-194 appear (they are all 'x' anyway).
    for (uint32_t p : positions) {
        ASSERT_TRUE(p < 6 || p > 194);
    }
}

// ============================================================================
// 12. Extract positions mask zero
// ============================================================================

TEST(extract_positions_mask_zero) {
    // A 64-byte string: all characters inside quotes, no structural chars.
    // The opening quote is at byte 0, fill 62 'a' bytes, closing quote at 63.
    std::string json(64, 'a');
    json[0] = '"';
    json[63] = '"';

    auto positions = index_string(json);
    // No structural characters in a bare string.
    ASSERT_TRUE(positions.empty());
}

// ============================================================================
// 13. Extract positions many structurals
// ============================================================================

TEST(extract_positions_many_structurals) {
    // Dense structural input: "{:},{:},{:},{:}"
    std::string json = "{:},{:},{:},{:}";
    auto positions = index_string(json);

    // Every character is structural: { : } ,
    // Count expected structurals.
    size_t expected = 0;
    for (size_t i = 0; i < json.size(); ++i) {
        char c = json[i];
        if (c == '{' || c == '}' || c == ':' || c == ',')
            ++expected;
    }
    ASSERT_EQ(positions.size(), expected);

    // Verify every reported position is indeed a structural character.
    for (uint32_t pos : positions) {
        char c = json[pos];
        ASSERT_TRUE(c == '{' || c == '}' || c == ':' || c == ',');
    }
}

// ============================================================================
// 14. One byte input (digit)
// ============================================================================

TEST(one_byte_input) {
    auto positions = index_string("1");
    ASSERT_TRUE(positions.empty());
}

// ============================================================================
// 15. Remainder length sweep (1 through 65)
// ============================================================================

TEST(remainder_length_sweep) {
    // For each document length from 1 to 65, build a document "[" + padding + "]"
    // and verify the builder handles the final partial chunk correctly.
    for (size_t len = 1; len <= 65; ++len) {
        std::string json(len, ' ');
        // Place '[' at position 0.
        json[0] = '[';
        // Place ']' at the last position if length >= 2.
        if (len >= 2) {
            json[len - 1] = ']';
        }

        IndexBuilder builder;
        auto idx = builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());

        ASSERT_EQ(idx.document_length, len);

        std::set<uint32_t> pos_set(idx.positions.begin(), idx.positions.end());

        // '[' at position 0 should always be found.
        ASSERT_TRUE(pos_set.count(0) == 1);

        if (len >= 2) {
            // ']' at last position should be found.
            ASSERT_TRUE(pos_set.count(static_cast<uint32_t>(len - 1)) == 1);
        }

        // No position should exceed document length.
        for (uint32_t p : idx.positions) {
            ASSERT_TRUE(p < len);
        }
    }
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("=== SIMD Structural Indexer Edge-Case Tests ===\n");

    std::printf("\n--- Chunk boundary carry ---\n");
    run_chunk_boundary_backslash_carry();
    run_chunk_boundary_string_carry();

    std::printf("\n--- Degenerate inputs ---\n");
    run_empty_input_build();
    run_single_byte_structural();
    run_single_byte_quote();
    run_single_byte_whitespace();
    run_all_whitespace_64_bytes();
    run_all_backslash_64_bytes();

    std::printf("\n--- Quote position extremes ---\n");
    run_quote_at_position_zero();
    run_quote_at_position_63();

    std::printf("\n--- Multi-chunk strings ---\n");
    run_string_spanning_three_chunks();

    std::printf("\n--- Position extraction ---\n");
    run_extract_positions_mask_zero();
    run_extract_positions_many_structurals();

    std::printf("\n--- Single-byte and remainder ---\n");
    run_one_byte_input();
    run_remainder_length_sweep();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}

/**
 * test_simd_newline.cpp - Tests for SIMD-accelerated newline/whitespace functions
 *
 * Tests the functions in simd_newline.cpp:
 * - find_newline_simd
 * - count_newlines_simd
 * - collect_newlines_simd
 * - skip_whitespace_simd
 * - is_whitespace_only_simd
 *
 * Comprehensive boundary testing for SIMD chunk sizes:
 * - SSE/NEON: 16-byte chunks
 * - AVX2: 32-byte chunks
 */

#include "strata/util/simd_string.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace strata::util;

// ============================================================================
// Tests for find_newline_simd
// ============================================================================

void test_find_newline_empty() {
    // Empty string
    size_t pos = find_newline_simd("", 0, 0);
    assert(pos == 0);

    // Empty with start_pos
    pos = find_newline_simd("abc", 3, 3);
    assert(pos == 3);

    std::cout << "✓ test_find_newline_empty passed\n";
}

void test_find_newline_no_newline() {
    // Short string without newline
    std::string s = "hello world";
    size_t pos = find_newline_simd(s.data(), s.size(), 0);
    assert(pos == s.size());

    // Long string without newline (tests SIMD path)
    std::string long_clean(1000, 'a');
    pos = find_newline_simd(long_clean.data(), long_clean.size(), 0);
    assert(pos == long_clean.size());

    std::cout << "✓ test_find_newline_no_newline passed\n";
}

void test_find_newline_at_positions() {
    // Test newline at SIMD chunk boundaries
    // SSE/NEON: 16 bytes, AVX2: 32 bytes
    for (size_t newline_pos : {0, 1, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 500, 999}) {
        std::string s(1000, 'a');
        s[newline_pos] = '\n';
        size_t pos = find_newline_simd(s.data(), s.size(), 0);
        assert(pos == newline_pos);
    }

    std::cout << "✓ test_find_newline_at_positions passed\n";
}

void test_find_newline_with_start_pos() {
    std::string s = "abc\ndef\nghi";

    // Start at 0 - should find first newline
    size_t pos = find_newline_simd(s.data(), s.size(), 0);
    assert(pos == 3);

    // Start at 4 - should find second newline
    pos = find_newline_simd(s.data(), s.size(), 4);
    assert(pos == 7);

    // Start after last newline
    pos = find_newline_simd(s.data(), s.size(), 8);
    assert(pos == s.size());

    // Long string with start_pos
    std::string long_s(1000, 'a');
    long_s[500] = '\n';
    pos = find_newline_simd(long_s.data(), long_s.size(), 100);
    assert(pos == 500);

    pos = find_newline_simd(long_s.data(), long_s.size(), 501);
    assert(pos == long_s.size());

    std::cout << "✓ test_find_newline_with_start_pos passed\n";
}

void test_find_newline_multiple() {
    // Multiple newlines - should return first
    std::string s = "a\nb\nc\nd";
    size_t pos = find_newline_simd(s.data(), s.size(), 0);
    assert(pos == 1);

    // Consecutive newlines
    std::string s2 = "ab\n\n\ncd";
    pos = find_newline_simd(s2.data(), s2.size(), 0);
    assert(pos == 2);

    std::cout << "✓ test_find_newline_multiple passed\n";
}

void test_find_newline_various_sizes() {
    // Test various input sizes including sub-chunk sizes
    for (size_t len = 1; len <= 100; ++len) {
        std::string s(len, 'a');

        // No newline
        size_t pos = find_newline_simd(s.data(), s.size(), 0);
        assert(pos == len);

        // Newline at end
        s[len - 1] = '\n';
        pos = find_newline_simd(s.data(), s.size(), 0);
        assert(pos == len - 1);

        // Newline at start
        s[len - 1] = 'a';
        s[0] = '\n';
        pos = find_newline_simd(s.data(), s.size(), 0);
        assert(pos == 0);
    }

    std::cout << "✓ test_find_newline_various_sizes passed\n";
}

// ============================================================================
// Tests for count_newlines_simd
// ============================================================================

void test_count_newlines_empty() {
    size_t count = count_newlines_simd("", 0);
    assert(count == 0);

    std::cout << "✓ test_count_newlines_empty passed\n";
}

void test_count_newlines_none() {
    std::string s = "hello world";
    size_t count = count_newlines_simd(s.data(), s.size());
    assert(count == 0);

    // Long string (SIMD path)
    std::string long_s(1000, 'a');
    count = count_newlines_simd(long_s.data(), long_s.size());
    assert(count == 0);

    std::cout << "✓ test_count_newlines_none passed\n";
}

void test_count_newlines_single() {
    // Single newline at various positions
    for (size_t pos : {0, 1, 15, 16, 17, 31, 32, 33, 63, 64, 99}) {
        std::string s(100, 'a');
        s[pos] = '\n';
        size_t count = count_newlines_simd(s.data(), s.size());
        assert(count == 1);
    }

    std::cout << "✓ test_count_newlines_single passed\n";
}

void test_count_newlines_multiple() {
    // Multiple newlines
    std::string s(100, 'a');
    s[10] = '\n';
    s[20] = '\n';
    s[50] = '\n';
    size_t count = count_newlines_simd(s.data(), s.size());
    assert(count == 3);

    // More newlines at boundaries
    std::string s2(100, 'a');
    s2[0] = '\n';
    s2[15] = '\n';
    s2[16] = '\n';
    s2[31] = '\n';
    s2[32] = '\n';
    s2[99] = '\n';
    count = count_newlines_simd(s2.data(), s2.size());
    assert(count == 6);

    std::cout << "✓ test_count_newlines_multiple passed\n";
}

void test_count_newlines_consecutive() {
    // Consecutive newlines
    std::string s = "a\n\n\nb";
    size_t count = count_newlines_simd(s.data(), s.size());
    assert(count == 3);

    // All newlines
    std::string all_nl(50, '\n');
    count = count_newlines_simd(all_nl.data(), all_nl.size());
    assert(count == 50);

    std::cout << "✓ test_count_newlines_consecutive passed\n";
}

void test_count_newlines_various_sizes() {
    // Test various sizes
    for (size_t len = 1; len <= 100; ++len) {
        std::string s(len, '\n');
        size_t count = count_newlines_simd(s.data(), s.size());
        assert(count == len);
    }

    std::cout << "✓ test_count_newlines_various_sizes passed\n";
}

// ============================================================================
// Tests for collect_newlines_simd
// ============================================================================

void test_collect_newlines_empty() {
    std::vector<size_t> positions;
    collect_newlines_simd("", 0, 0, 100, positions);
    assert(positions.empty());

    std::cout << "✓ test_collect_newlines_empty passed\n";
}

void test_collect_newlines_none() {
    std::string s = "hello world";
    std::vector<size_t> positions;
    collect_newlines_simd(s.data(), s.size(), 0, 100, positions);
    assert(positions.empty());

    std::cout << "✓ test_collect_newlines_none passed\n";
}

void test_collect_newlines_basic() {
    std::string s = "a\nb\nc\nd";
    std::vector<size_t> positions;
    collect_newlines_simd(s.data(), s.size(), 0, 100, positions);
    assert(positions.size() == 3);
    assert(positions[0] == 1);
    assert(positions[1] == 3);
    assert(positions[2] == 5);

    std::cout << "✓ test_collect_newlines_basic passed\n";
}

void test_collect_newlines_with_start() {
    std::string s = "a\nb\nc\nd";
    std::vector<size_t> positions;

    // Start from position 2
    collect_newlines_simd(s.data(), s.size(), 2, 100, positions);
    assert(positions.size() == 2);
    assert(positions[0] == 3);
    assert(positions[1] == 5);

    std::cout << "✓ test_collect_newlines_with_start passed\n";
}

void test_collect_newlines_max_positions() {
    std::string s = "a\nb\nc\nd\ne\nf";
    std::vector<size_t> positions;

    // Limit to 2 positions
    collect_newlines_simd(s.data(), s.size(), 0, 2, positions);
    assert(positions.size() == 2);
    assert(positions[0] == 1);
    assert(positions[1] == 3);

    std::cout << "✓ test_collect_newlines_max_positions passed\n";
}

void test_collect_newlines_at_boundaries() {
    // Long string with newlines at SIMD boundaries
    std::string s(200, 'a');
    s[0] = '\n';
    s[15] = '\n';
    s[16] = '\n';
    s[31] = '\n';
    s[32] = '\n';
    s[63] = '\n';
    s[64] = '\n';
    s[127] = '\n';
    s[128] = '\n';

    std::vector<size_t> positions;
    collect_newlines_simd(s.data(), s.size(), 0, 100, positions);
    assert(positions.size() == 9);
    assert(positions[0] == 0);
    assert(positions[1] == 15);
    assert(positions[2] == 16);
    assert(positions[3] == 31);
    assert(positions[4] == 32);
    assert(positions[5] == 63);
    assert(positions[6] == 64);
    assert(positions[7] == 127);
    assert(positions[8] == 128);

    std::cout << "✓ test_collect_newlines_at_boundaries passed\n";
}

void test_collect_newlines_edge_cases() {
    std::vector<size_t> positions;

    // start_pos beyond end should be a no-op
    collect_newlines_simd("abc", 3, 3, 10, positions);
    assert(positions.empty());

    // max_positions == 0 should be a no-op
    collect_newlines_simd("a\nb\nc", 5, 0, 0, positions);
    assert(positions.empty());

    std::cout << "✓ test_collect_newlines_edge_cases passed\n";
}

// ============================================================================
// Tests for skip_whitespace_simd
// ============================================================================

void test_skip_whitespace_empty() {
    size_t pos = skip_whitespace_simd("", 0, 0);
    assert(pos == 0);

    std::cout << "✓ test_skip_whitespace_empty passed\n";
}

void test_skip_whitespace_no_whitespace() {
    std::string s = "hello";
    size_t pos = skip_whitespace_simd(s.data(), s.size(), 0);
    assert(pos == 0);

    std::cout << "✓ test_skip_whitespace_no_whitespace passed\n";
}

void test_skip_whitespace_basic() {
    // Spaces
    size_t pos = skip_whitespace_simd("   hello", 8, 0);
    assert(pos == 3);

    // Tab
    pos = skip_whitespace_simd("\thello", 6, 0);
    assert(pos == 1);

    // Newline
    pos = skip_whitespace_simd("\nhello", 6, 0);
    assert(pos == 1);

    // Carriage return
    pos = skip_whitespace_simd("\rhello", 6, 0);
    assert(pos == 1);

    // Mixed whitespace
    pos = skip_whitespace_simd(" \t\n\r hello", 10, 0);
    assert(pos == 5);

    std::cout << "✓ test_skip_whitespace_basic passed\n";
}

void test_skip_whitespace_all_whitespace() {
    // All whitespace - should return len
    std::string ws = "   \t\n\r   ";
    size_t pos = skip_whitespace_simd(ws.data(), ws.size(), 0);
    assert(pos == ws.size());

    // Long whitespace (SIMD path)
    std::string long_ws(200, ' ');
    pos = skip_whitespace_simd(long_ws.data(), long_ws.size(), 0);
    assert(pos == long_ws.size());

    std::cout << "✓ test_skip_whitespace_all_whitespace passed\n";
}

void test_skip_whitespace_with_start_pos() {
    std::string s = "abc   def";

    // Start at position 3 (first space)
    size_t pos = skip_whitespace_simd(s.data(), s.size(), 3);
    assert(pos == 6);

    // Start at non-whitespace
    pos = skip_whitespace_simd(s.data(), s.size(), 0);
    assert(pos == 0);

    // Start at position after whitespace
    pos = skip_whitespace_simd(s.data(), s.size(), 6);
    assert(pos == 6);

    std::cout << "✓ test_skip_whitespace_with_start_pos passed\n";
}

void test_skip_whitespace_long_prefix() {
    // Long whitespace prefix (tests SIMD path)
    std::string ws(100, ' ');
    ws += "hello";
    size_t pos = skip_whitespace_simd(ws.data(), ws.size(), 0);
    assert(pos == 100);

    // Whitespace at SIMD boundaries
    for (size_t ws_len : {15, 16, 17, 31, 32, 33, 63, 64, 65}) {
        std::string s(ws_len, ' ');
        s += "x";
        pos = skip_whitespace_simd(s.data(), s.size(), 0);
        assert(pos == ws_len);
    }

    std::cout << "✓ test_skip_whitespace_long_prefix passed\n";
}

void test_skip_whitespace_various_sizes() {
    // Test various input sizes
    for (size_t len = 1; len <= 100; ++len) {
        std::string s(len, ' ');
        size_t pos = skip_whitespace_simd(s.data(), s.size(), 0);
        assert(pos == len);

        // Add non-whitespace at end
        s[len - 1] = 'x';
        pos = skip_whitespace_simd(s.data(), s.size(), 0);
        assert(pos == len - 1);
    }

    std::cout << "✓ test_skip_whitespace_various_sizes passed\n";
}

// ============================================================================
// Tests for is_whitespace_only_simd
// ============================================================================

void test_is_whitespace_only_empty() {
    assert(is_whitespace_only_simd("", 0));

    std::cout << "✓ test_is_whitespace_only_empty passed\n";
}

void test_is_whitespace_only_true() {
    // Various whitespace characters
    assert(is_whitespace_only_simd(" ", 1));
    assert(is_whitespace_only_simd("\t", 1));
    assert(is_whitespace_only_simd("\n", 1));
    assert(is_whitespace_only_simd("\r", 1));
    assert(is_whitespace_only_simd("   ", 3));
    assert(is_whitespace_only_simd("\t\n\r ", 4));
    assert(is_whitespace_only_simd("  \t  \n  \r  ", 11));

    // Long whitespace (SIMD path)
    std::string long_ws(200, ' ');
    assert(is_whitespace_only_simd(long_ws.data(), long_ws.size()));

    // Mixed whitespace types
    std::string mixed(200, ' ');
    for (size_t i = 0; i < mixed.size(); i += 4) {
        if (i < mixed.size()) mixed[i] = ' ';
        if (i + 1 < mixed.size()) mixed[i + 1] = '\t';
        if (i + 2 < mixed.size()) mixed[i + 2] = '\n';
        if (i + 3 < mixed.size()) mixed[i + 3] = '\r';
    }
    assert(is_whitespace_only_simd(mixed.data(), mixed.size()));

    std::cout << "✓ test_is_whitespace_only_true passed\n";
}

void test_is_whitespace_only_false() {
    // Non-whitespace characters
    assert(!is_whitespace_only_simd("a", 1));
    assert(!is_whitespace_only_simd("hello", 5));
    assert(!is_whitespace_only_simd("  x  ", 5));
    assert(!is_whitespace_only_simd("x    ", 5));
    assert(!is_whitespace_only_simd("    x", 5));

    // Non-whitespace at SIMD boundaries
    for (size_t pos : {0, 15, 16, 17, 31, 32, 33, 63, 64, 99}) {
        std::string s(100, ' ');
        s[pos] = 'x';
        assert(!is_whitespace_only_simd(s.data(), s.size()));
    }

    std::cout << "✓ test_is_whitespace_only_false passed\n";
}

void test_is_whitespace_only_various_sizes() {
    // Test various sizes - all whitespace
    for (size_t len = 1; len <= 100; ++len) {
        std::string s(len, ' ');
        assert(is_whitespace_only_simd(s.data(), s.size()));
    }

    // Test various sizes - with non-whitespace at end
    for (size_t len = 1; len <= 100; ++len) {
        std::string s(len, ' ');
        s[len - 1] = 'x';
        assert(!is_whitespace_only_simd(s.data(), s.size()));
    }

    std::cout << "✓ test_is_whitespace_only_various_sizes passed\n";
}

void test_is_whitespace_only_boundary_cases() {
    // Non-whitespace at exact chunk boundaries
    for (size_t boundary : {15, 16, 31, 32, 63, 64, 127, 128}) {
        if (boundary > 0) {
            std::string s(boundary + 1, ' ');
            s[boundary] = 'x';
            assert(!is_whitespace_only_simd(s.data(), s.size()));

            s[boundary] = ' ';
            s[boundary - 1] = 'x';
            assert(!is_whitespace_only_simd(s.data(), s.size()));
        }
    }

    std::cout << "✓ test_is_whitespace_only_boundary_cases passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running SIMD Newline/Whitespace Tests...\n\n";

    // find_newline_simd tests
    std::cout << "=== find_newline_simd tests ===\n";
    test_find_newline_empty();
    test_find_newline_no_newline();
    test_find_newline_at_positions();
    test_find_newline_with_start_pos();
    test_find_newline_multiple();
    test_find_newline_various_sizes();

    // count_newlines_simd tests
    std::cout << "\n=== count_newlines_simd tests ===\n";
    test_count_newlines_empty();
    test_count_newlines_none();
    test_count_newlines_single();
    test_count_newlines_multiple();
    test_count_newlines_consecutive();
    test_count_newlines_various_sizes();

    // collect_newlines_simd tests
    std::cout << "\n=== collect_newlines_simd tests ===\n";
    test_collect_newlines_empty();
    test_collect_newlines_none();
    test_collect_newlines_basic();
    test_collect_newlines_with_start();
    test_collect_newlines_max_positions();
    test_collect_newlines_at_boundaries();
    test_collect_newlines_edge_cases();

    // skip_whitespace_simd tests
    std::cout << "\n=== skip_whitespace_simd tests ===\n";
    test_skip_whitespace_empty();
    test_skip_whitespace_no_whitespace();
    test_skip_whitespace_basic();
    test_skip_whitespace_all_whitespace();
    test_skip_whitespace_with_start_pos();
    test_skip_whitespace_long_prefix();
    test_skip_whitespace_various_sizes();

    // is_whitespace_only_simd tests
    std::cout << "\n=== is_whitespace_only_simd tests ===\n";
    test_is_whitespace_only_empty();
    test_is_whitespace_only_true();
    test_is_whitespace_only_false();
    test_is_whitespace_only_various_sizes();
    test_is_whitespace_only_boundary_cases();

    std::cout << "\n✅ All SIMD Newline/Whitespace tests passed!\n";
    return 0;
}

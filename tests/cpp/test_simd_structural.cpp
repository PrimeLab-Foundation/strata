/**
 * test_simd_structural.cpp - Tests for SIMD-accelerated JSON structural scanning
 *
 * Tests the functions in simd_structural.cpp:
 * - find_next_structural_simd
 * - collect_structural_positions_simd
 *
 * Comprehensive boundary testing for SIMD chunk sizes:
 * - SSE/NEON: 16-byte chunks
 * - AVX2: 32-byte chunks
 */

#include "strata/util/simd_string.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace strata::util;

// ============================================================================
// Tests for find_next_structural_simd
// ============================================================================

void test_find_structural_empty() {
    // Empty string
    size_t pos = find_next_structural_simd("", 0, 0);
    assert(pos == 0);

    std::cout << "✓ test_find_structural_empty passed\n";
}

void test_find_structural_no_structural() {
    // String with no structural characters (only regular content)
    std::string s = "hello world";
    size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
    // Should return first non-whitespace position
    assert(pos == 0);

    // Only whitespace
    std::string ws = "     ";
    pos = find_next_structural_simd(ws.data(), ws.size(), 0);
    assert(pos == ws.size());

    std::cout << "✓ test_find_structural_no_structural passed\n";
}

void test_find_structural_basic() {
    // Opening brace
    size_t pos = find_next_structural_simd("{", 1, 0);
    assert(pos == 0);

    // Closing brace
    pos = find_next_structural_simd("}", 1, 0);
    assert(pos == 0);

    // Opening bracket
    pos = find_next_structural_simd("[", 1, 0);
    assert(pos == 0);

    // Closing bracket
    pos = find_next_structural_simd("]", 1, 0);
    assert(pos == 0);

    // Colon
    pos = find_next_structural_simd(":", 1, 0);
    assert(pos == 0);

    // Comma
    pos = find_next_structural_simd(",", 1, 0);
    assert(pos == 0);

    std::cout << "✓ test_find_structural_basic passed\n";
}

void test_find_structural_with_whitespace_prefix() {
    // Structural after whitespace
    size_t pos = find_next_structural_simd("   {", 4, 0);
    assert(pos == 3);

    pos = find_next_structural_simd("   }", 4, 0);
    assert(pos == 3);

    pos = find_next_structural_simd("\t\n[", 3, 0);
    assert(pos == 2);

    pos = find_next_structural_simd("  \r\n]", 5, 0);
    assert(pos == 4);

    pos = find_next_structural_simd("   :", 4, 0);
    assert(pos == 3);

    pos = find_next_structural_simd("   ,", 4, 0);
    assert(pos == 3);

    std::cout << "✓ test_find_structural_with_whitespace_prefix passed\n";
}

void test_find_structural_long_whitespace() {
    // Long whitespace prefix (tests SIMD path)
    for (size_t ws_len : {15, 16, 17, 31, 32, 33, 63, 64, 65, 100}) {
        std::string s(ws_len, ' ');
        s += "{";
        size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
        assert(pos == ws_len);
    }

    std::cout << "✓ test_find_structural_long_whitespace passed\n";
}

void test_find_structural_at_positions() {
    // Test structural character at various SIMD chunk boundaries
    for (char structural : {'{', '}', '[', ']', ':', ','}) {
        for (size_t struct_pos : {0, 1, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 500}) {
            std::string s(1000, ' ');  // All whitespace
            s[struct_pos] = structural;
            size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
            assert(pos == struct_pos);
        }
    }

    std::cout << "✓ test_find_structural_at_positions passed\n";
}

void test_find_structural_with_start_pos() {
    std::string s = "  {  }  [  ]";

    // Start at 0 - should find first {
    size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
    assert(pos == 2);

    // Start at 3 - should find }
    pos = find_next_structural_simd(s.data(), s.size(), 3);
    assert(pos == 5);

    // Start at 6 - should find [
    pos = find_next_structural_simd(s.data(), s.size(), 6);
    assert(pos == 8);

    // Start at 9 - should find ]
    pos = find_next_structural_simd(s.data(), s.size(), 9);
    assert(pos == 11);

    std::cout << "✓ test_find_structural_with_start_pos passed\n";
}

void test_find_structural_json_object() {
    // Realistic JSON object structure
    // Note: find_next_structural_simd returns first NON-WHITESPACE position,
    // not necessarily a structural character. Caller checks what character is there.
    std::string json = R"(  {  "key"  :  "value"  ,  "num"  :  123  }  )";

    // Should find '{' at position 2
    size_t pos = find_next_structural_simd(json.data(), json.size(), 0);
    assert(pos == 2);
    assert(json[pos] == '{');

    // After '{', next non-whitespace is '"' (start of key)
    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == '"');

    std::cout << "✓ test_find_structural_json_object passed\n";
}

void test_find_structural_json_array() {
    // Realistic JSON array structure
    // Note: find_next_structural_simd returns first NON-WHITESPACE position
    std::string json = R"(  [  1  ,  2  ,  3  ]  )";

    // Should find '[' at position 2
    size_t pos = find_next_structural_simd(json.data(), json.size(), 0);
    assert(pos == 2);
    assert(json[pos] == '[');

    // After '[', next non-whitespace should be '1'
    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == '1');

    std::cout << "✓ test_find_structural_json_array passed\n";
}

void test_find_structural_various_sizes() {
    // Test various input sizes
    for (size_t len = 1; len <= 100; ++len) {
        // All whitespace
        std::string s(len, ' ');
        size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
        assert(pos == len);

        // Structural at start
        s[0] = '{';
        pos = find_next_structural_simd(s.data(), s.size(), 0);
        assert(pos == 0);

        // Structural at end
        s[0] = ' ';
        s[len - 1] = '}';
        pos = find_next_structural_simd(s.data(), s.size(), 0);
        assert(pos == len - 1);
    }

    std::cout << "✓ test_find_structural_various_sizes passed\n";
}

void test_find_structural_mixed_content() {
    // Test with non-whitespace non-structural content
    // The function should return position of first non-whitespace
    std::string s = "   abc{";

    size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
    // Should return position of 'a' (first non-whitespace)
    assert(pos == 3);

    // Starting after 'abc'
    pos = find_next_structural_simd(s.data(), s.size(), 6);
    assert(pos == 6);  // Position of '{'

    std::cout << "✓ test_find_structural_mixed_content passed\n";
}

void test_find_structural_nested() {
    // Nested structures
    std::string json = "  {  [  {  }  ]  }  ";

    size_t pos = find_next_structural_simd(json.data(), json.size(), 0);
    assert(json[pos] == '{');  // First {

    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == '[');  // First [

    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == '{');  // Inner {

    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == '}');  // Inner }

    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == ']');  // First ]

    pos = find_next_structural_simd(json.data(), json.size(), pos + 1);
    assert(json[pos] == '}');  // First }

    std::cout << "✓ test_find_structural_nested passed\n";
}

void test_find_structural_boundary_transitions() {
    // Test transitions across SIMD chunk boundaries
    for (size_t boundary : {15, 16, 31, 32, 63, 64, 127, 128}) {
        // Whitespace up to boundary, structural at boundary
        std::string s(boundary + 1, ' ');
        s[boundary] = '{';
        size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
        assert(pos == boundary);

        // Whitespace up to boundary-1, structural at boundary-1
        if (boundary > 0) {
            s[boundary] = ' ';
            s[boundary - 1] = '}';
            pos = find_next_structural_simd(s.data(), s.size(), 0);
            assert(pos == boundary - 1);
        }

        // Whitespace up to boundary+1, structural at boundary+1
        if (boundary + 2 <= s.size()) {
            s[boundary - 1] = ' ';
            s[boundary + 1] = '[';
            pos = find_next_structural_simd(s.data(), s.size(), 0);
            assert(pos == boundary + 1);
        }
    }

    std::cout << "✓ test_find_structural_boundary_transitions passed\n";
}

void test_find_structural_all_types() {
    // All structural characters at the same position for each test
    const char structural_chars[] = {'{', '}', '[', ']', ':', ','};

    for (char c : structural_chars) {
        std::string s = "   ";
        s += c;
        s += "   ";
        size_t pos = find_next_structural_simd(s.data(), s.size(), 0);
        assert(pos == 3);
        assert(s[pos] == c);
    }

    std::cout << "✓ test_find_structural_all_types passed\n";
}

// ============================================================================
// Tests for collect_structural_positions_simd
// ============================================================================

void test_collect_structural_empty() {
    std::vector<size_t> positions;
    collect_structural_positions_simd("", 0, positions);
    assert(positions.empty());

    std::cout << "✓ test_collect_structural_empty passed\n";
}

void test_collect_structural_basic() {
    std::string json = "  {\"a\": [1,2]}";
    std::vector<size_t> positions;
    collect_structural_positions_simd(json.data(), json.size(), positions);
    std::vector<size_t> expected = {2, 3, 5, 6, 8, 10, 12, 13};
    assert(positions == expected);

    std::cout << "✓ test_collect_structural_basic passed\n";
}

void test_collect_structural_quotes() {
    std::string json = "\"a\\\"b\"";
    std::vector<size_t> positions;
    collect_structural_positions_simd(json.data(), json.size(), positions);
    std::vector<size_t> expected = {0, 3, 5};
    assert(positions == expected);

    std::cout << "✓ test_collect_structural_quotes passed\n";
}

void test_collect_structural_boundaries() {
    std::string json(40, ' ');
    json[0] = '{';
    json[15] = '[';
    json[16] = ']';
    json[31] = ',';
    json[32] = ':';
    json[39] = '"';

    std::vector<size_t> positions;
    collect_structural_positions_simd(json.data(), json.size(), positions);
    std::vector<size_t> expected = {0, 15, 16, 31, 32, 39};
    assert(positions == expected);

    std::cout << "✓ test_collect_structural_boundaries passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running SIMD Structural Tests...\n\n";

    test_find_structural_empty();
    test_find_structural_no_structural();
    test_find_structural_basic();
    test_find_structural_with_whitespace_prefix();
    test_find_structural_long_whitespace();
    test_find_structural_at_positions();
    test_find_structural_with_start_pos();
    test_find_structural_json_object();
    test_find_structural_json_array();
    test_find_structural_various_sizes();
    test_find_structural_mixed_content();
    test_find_structural_nested();
    test_find_structural_boundary_transitions();
    test_find_structural_all_types();
    test_collect_structural_empty();
    test_collect_structural_basic();
    test_collect_structural_quotes();
    test_collect_structural_boundaries();

    std::cout << "\n✅ All SIMD Structural tests passed!\n";
    return 0;
}

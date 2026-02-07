/**
 * test_simd_escape.cpp - Tests for SIMD-accelerated JSON string escaping
 *
 * Tests the escape functions in simd_escape.cpp which have low coverage
 * because they're primarily used from Python bindings.
 */

#include "strata/util/simd_string.hpp"
#include "strata/util/output_buffer.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace strata::util;

// ============================================================================
// Tests for find_next_escape_simd
// ============================================================================

void test_find_next_escape_clean() {
    // Clean string - no escapes needed
    std::string clean = "hello world";
    size_t pos = find_next_escape_simd(clean.data(), clean.size());
    assert(pos == clean.size());

    // Long clean string (tests SIMD path)
    std::string long_clean(1000, 'a');
    pos = find_next_escape_simd(long_clean.data(), long_clean.size());
    assert(pos == long_clean.size());

    // Empty string
    pos = find_next_escape_simd("", 0);
    assert(pos == 0);

    std::cout << "✓ test_find_next_escape_clean passed\n";
}

void test_find_next_escape_special_chars() {
    // String with quote
    std::string with_quote = "hello\"world";
    size_t pos = find_next_escape_simd(with_quote.data(), with_quote.size());
    assert(pos == 5);

    // String with backslash
    std::string with_backslash = "hello\\world";
    pos = find_next_escape_simd(with_backslash.data(), with_backslash.size());
    assert(pos == 5);

    // String with control character (newline)
    std::string with_newline = "hello\nworld";
    pos = find_next_escape_simd(with_newline.data(), with_newline.size());
    assert(pos == 5);

    // String with tab
    std::string with_tab = "hello\tworld";
    pos = find_next_escape_simd(with_tab.data(), with_tab.size());
    assert(pos == 5);

    // String with carriage return
    std::string with_cr = "hello\rworld";
    pos = find_next_escape_simd(with_cr.data(), with_cr.size());
    assert(pos == 5);

    std::cout << "✓ test_find_next_escape_special_chars passed\n";
}

void test_find_next_escape_control_chars() {
    // Single control character (NUL)
    char nul_str[2] = {'\x00', '\0'};
    size_t pos = find_next_escape_simd(nul_str, 1);
    assert(pos == 0);

    // Control character at end of range (0x1F)
    char last_control[2] = {'\x1F', '\0'};
    pos = find_next_escape_simd(last_control, 1);
    assert(pos == 0);

    // String starting with control character
    std::string starts_control = "\x01hello";
    pos = find_next_escape_simd(starts_control.data(), starts_control.size());
    assert(pos == 0);

    std::cout << "✓ test_find_next_escape_control_chars passed\n";
}

void test_find_next_escape_positions() {
    // Long string with escape at various positions (tests SIMD chunk boundaries)
    for (size_t escape_pos : {0, 1, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 500, 999}) {
        std::string s(1000, 'a');
        s[escape_pos] = '"';
        size_t pos = find_next_escape_simd(s.data(), s.size());
        assert(pos == escape_pos);
    }

    std::cout << "✓ test_find_next_escape_positions passed\n";
}

// ============================================================================
// Tests for try_copy_clean_string (std::string overload)
// ============================================================================

void test_try_copy_clean_string_std() {
    std::string out;

    // Clean string - should return true and copy
    bool copied = try_copy_clean_string("hello", 5, out);
    assert(copied);
    assert(out == "\"hello\"");

    // Clean long string (SIMD path)
    out.clear();
    std::string long_clean(500, 'x');
    copied = try_copy_clean_string(long_clean.data(), long_clean.size(), out);
    assert(copied);
    assert(out == "\"" + long_clean + "\"");

    // Empty string
    out.clear();
    copied = try_copy_clean_string("", 0, out);
    assert(copied);
    assert(out == "\"\"");

    // String with escapes - should return false
    out.clear();
    copied = try_copy_clean_string("hel\"lo", 6, out);
    assert(!copied);

    // String with backslash
    out.clear();
    copied = try_copy_clean_string("hel\\lo", 6, out);
    assert(!copied);

    // String with control char
    out.clear();
    copied = try_copy_clean_string("hel\nlo", 6, out);
    assert(!copied);

    std::cout << "✓ test_try_copy_clean_string_std passed\n";
}

// ============================================================================
// Tests for try_copy_clean_string (OutputBuffer overload)
// ============================================================================

void test_try_copy_clean_string_buffer() {
    OutputBuffer out;

    // Clean string - should return true and copy with quotes
    bool copied = try_copy_clean_string("hello", 5, out);
    assert(copied);
    assert(std::string(out.data(), out.size()) == "\"hello\"");

    // Clean long string (SIMD path)
    out.clear();
    std::string long_clean(500, 'x');
    copied = try_copy_clean_string(long_clean.data(), long_clean.size(), out);
    assert(copied);
    assert(std::string(out.data(), out.size()) == "\"" + long_clean + "\"");

    // Empty string
    out.clear();
    copied = try_copy_clean_string("", 0, out);
    assert(copied);
    assert(std::string(out.data(), out.size()) == "\"\"");

    // String with escapes - should return false
    out.clear();
    copied = try_copy_clean_string("hel\"lo", 6, out);
    assert(!copied);

    std::cout << "✓ test_try_copy_clean_string_buffer passed\n";
}

// ============================================================================
// Tests for escape_json_string_simd (std::string overload)
// ============================================================================

void test_escape_json_string_std() {
    std::string out;

    // Clean string
    escape_json_string_simd("hello", 5, out);
    assert(out == "\"hello\"");

    // String with quote
    out.clear();
    escape_json_string_simd("hel\"lo", 6, out);
    assert(out == "\"hel\\\"lo\"");

    // String with backslash
    out.clear();
    escape_json_string_simd("hel\\lo", 6, out);
    assert(out == "\"hel\\\\lo\"");

    // Control characters - standard escapes
    out.clear();
    escape_json_string_simd("a\nb", 3, out);
    assert(out == "\"a\\nb\"");

    out.clear();
    escape_json_string_simd("a\tb", 3, out);
    assert(out == "\"a\\tb\"");

    out.clear();
    escape_json_string_simd("a\rb", 3, out);
    assert(out == "\"a\\rb\"");

    out.clear();
    escape_json_string_simd("a\bb", 3, out);
    assert(out == "\"a\\bb\"");

    out.clear();
    escape_json_string_simd("a\fb", 3, out);
    assert(out == "\"a\\fb\"");

    // Empty string
    out.clear();
    escape_json_string_simd("", 0, out);
    assert(out == "\"\"");

    std::cout << "✓ test_escape_json_string_std passed\n";
}

void test_escape_json_string_control_chars() {
    std::string out;

    // All control characters 0x00-0x1F that need \uXXXX escaping
    for (int c = 0; c < 0x20; ++c) {
        out.clear();
        char str[3] = {'a', static_cast<char>(c), 'b'};
        escape_json_string_simd(str, 3, out);
        // Verify output starts with " and ends with "
        assert(out.size() >= 4);
        assert(out[0] == '"');
        assert(out[out.size() - 1] == '"');

        // Verify escape sequence is present
        if (c == '\n') {
            assert(out.find("\\n") != std::string::npos);
        } else if (c == '\t') {
            assert(out.find("\\t") != std::string::npos);
        } else if (c == '\r') {
            assert(out.find("\\r") != std::string::npos);
        } else if (c == '\b') {
            assert(out.find("\\b") != std::string::npos);
        } else if (c == '\f') {
            assert(out.find("\\f") != std::string::npos);
        } else {
            // Other control chars use \uXXXX format
            assert(out.find("\\u00") != std::string::npos);
        }
    }

    std::cout << "✓ test_escape_json_string_control_chars passed\n";
}

// ============================================================================
// Tests for escape_json_string_simd (OutputBuffer overload)
// ============================================================================

void test_escape_json_string_buffer() {
    OutputBuffer out;

    // Clean string
    escape_json_string_simd("hello", 5, out);
    assert(std::string(out.data(), out.size()) == "\"hello\"");

    // String with quote
    out.clear();
    escape_json_string_simd("hel\"lo", 6, out);
    assert(std::string(out.data(), out.size()) == "\"hel\\\"lo\"");

    // String with backslash
    out.clear();
    escape_json_string_simd("hel\\lo", 6, out);
    assert(std::string(out.data(), out.size()) == "\"hel\\\\lo\"");

    // Control characters
    out.clear();
    escape_json_string_simd("a\nb\tc\rd", 7, out);
    assert(std::string(out.data(), out.size()) == "\"a\\nb\\tc\\rd\"");

    // Empty string
    out.clear();
    escape_json_string_simd("", 0, out);
    assert(std::string(out.data(), out.size()) == "\"\"");

    // Long string with escapes at various positions
    for (size_t pos : {0, 15, 16, 31, 32, 100, 199}) {
        out.clear();
        std::string s(200, 'a');
        s[pos] = '\n';
        escape_json_string_simd(s.data(), s.size(), out);
        std::string result(out.data(), out.size());
        assert(result.find("\\n") != std::string::npos);
    }

    std::cout << "✓ test_escape_json_string_buffer passed\n";
}

// ============================================================================
// Tests for escape_or_copy_string_simd
// ============================================================================

void test_escape_or_copy_string_simd() {
    OutputBuffer out;

    // Clean string - should just copy with quotes
    escape_or_copy_string_simd("hello", 5, out);
    assert(std::string(out.data(), out.size()) == "\"hello\"");

    // String with quote - should escape
    out.clear();
    escape_or_copy_string_simd("hel\"lo", 6, out);
    assert(std::string(out.data(), out.size()) == "\"hel\\\"lo\"");

    // String with backslash
    out.clear();
    escape_or_copy_string_simd("hel\\lo", 6, out);
    assert(std::string(out.data(), out.size()) == "\"hel\\\\lo\"");

    // Control characters
    out.clear();
    escape_or_copy_string_simd("a\nb\tc\rd", 7, out);
    assert(std::string(out.data(), out.size()) == "\"a\\nb\\tc\\rd\"");

    // Empty string
    out.clear();
    escape_or_copy_string_simd("", 0, out);
    assert(std::string(out.data(), out.size()) == "\"\"");

    // Long clean string (tests SIMD fast path)
    out.clear();
    std::string long_clean(1000, 'a');
    escape_or_copy_string_simd(long_clean.data(), long_clean.size(), out);
    assert(std::string(out.data(), out.size()) == "\"" + long_clean + "\"");

    // Long string with escapes at various positions
    for (size_t pos : {0, 15, 16, 31, 32, 100, 500, 999}) {
        out.clear();
        std::string s(1000, 'a');
        s[pos] = '\n';
        escape_or_copy_string_simd(s.data(), s.size(), out);
        std::string result(out.data(), out.size());
        assert(result.find("\\n") != std::string::npos);
    }

    std::cout << "✓ test_escape_or_copy_string_simd passed\n";
}

void test_escape_or_copy_string_fixed_buffer() {
    char buffer[1024];
    FixedOutputBuffer out(buffer, sizeof(buffer));

    // Clean string - should just copy with quotes
    escape_or_copy_string_simd("hello", 5, out);
    assert(std::string(out.data(), out.size()) == "\"hello\"");

    // String with quote - should escape
    out.clear();
    escape_or_copy_string_simd("hel\"lo", 6, out);
    assert(std::string(out.data(), out.size()) == "\"hel\\\"lo\"");

    // Control characters
    out.clear();
    escape_or_copy_string_simd("a\nb\tc", 5, out);
    assert(std::string(out.data(), out.size()) == "\"a\\nb\\tc\"");

    // Empty string
    out.clear();
    escape_or_copy_string_simd("", 0, out);
    assert(std::string(out.data(), out.size()) == "\"\"");

    std::cout << "✓ test_escape_or_copy_string_fixed_buffer passed\n";
}

// ============================================================================
// Tests for FixedOutputBuffer overloads
// ============================================================================

void test_try_copy_clean_string_fixed() {
    char buffer[1024];
    FixedOutputBuffer out(buffer, sizeof(buffer));

    // Clean string
    bool copied = try_copy_clean_string("hello", 5, out);
    assert(copied);
    assert(std::string(out.data(), out.size()) == "\"hello\"");

    // String with escapes - should return false
    out.clear();
    copied = try_copy_clean_string("hel\"lo", 6, out);
    assert(!copied);

    std::cout << "✓ test_try_copy_clean_string_fixed passed\n";
}

void test_escape_json_string_fixed() {
    char buffer[1024];
    FixedOutputBuffer out(buffer, sizeof(buffer));

    // Clean string
    escape_json_string_simd("hello", 5, out);
    assert(std::string(out.data(), out.size()) == "\"hello\"");

    // String with quote
    out.clear();
    escape_json_string_simd("hel\"lo", 6, out);
    assert(std::string(out.data(), out.size()) == "\"hel\\\"lo\"");

    // Control characters
    out.clear();
    escape_json_string_simd("a\nb", 3, out);
    assert(std::string(out.data(), out.size()) == "\"a\\nb\"");

    std::cout << "✓ test_escape_json_string_fixed passed\n";
}

// ============================================================================
// Tests for multiple escapes in one string
// ============================================================================

void test_multiple_escapes() {
    OutputBuffer out;

    // String with multiple different escape characters
    escape_or_copy_string_simd("\"hello\\world\n\ttab\"", 18, out);
    std::string result(out.data(), out.size());
    assert(result.find("\\\"") != std::string::npos);  // escaped quote
    assert(result.find("\\\\") != std::string::npos);  // escaped backslash
    assert(result.find("\\n") != std::string::npos);   // escaped newline
    assert(result.find("\\t") != std::string::npos);   // escaped tab

    // String with all standard escapable characters
    out.clear();
    const char* all_escapes = "\"\\/\b\f\n\r\t";
    escape_json_string_simd(all_escapes, 8, out);
    result = std::string(out.data(), out.size());
    assert(result.find("\\\"") != std::string::npos);
    assert(result.find("\\\\") != std::string::npos);
    assert(result.find("\\b") != std::string::npos);
    assert(result.find("\\f") != std::string::npos);
    assert(result.find("\\n") != std::string::npos);
    assert(result.find("\\r") != std::string::npos);
    assert(result.find("\\t") != std::string::npos);

    std::cout << "✓ test_multiple_escapes passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running SIMD Escape Tests...\n\n";

    // find_next_escape_simd tests
    test_find_next_escape_clean();
    test_find_next_escape_special_chars();
    test_find_next_escape_control_chars();
    test_find_next_escape_positions();

    // try_copy_clean_string tests
    test_try_copy_clean_string_std();
    test_try_copy_clean_string_buffer();
    test_try_copy_clean_string_fixed();

    // escape_json_string_simd tests
    test_escape_json_string_std();
    test_escape_json_string_control_chars();
    test_escape_json_string_buffer();
    test_escape_json_string_fixed();

    // escape_or_copy_string_simd tests
    test_escape_or_copy_string_simd();
    test_escape_or_copy_string_fixed_buffer();

    // Multiple escapes tests
    test_multiple_escapes();

    std::cout << "\n✅ All SIMD Escape tests passed!\n";
    return 0;
}

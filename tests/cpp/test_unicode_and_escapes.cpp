/**
 * test_unicode_and_escapes.cpp - Tests for Unicode handling and escape sequences
 *
 * Parity tests for tests/py/test_unicode_and_escapes.py
 * Ensures UTF-8 support and proper escape parsing on the C++ side.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/util/simd_string.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

using namespace strata;

void test_simd_string_eq_edges() {
    using util::simd_string_eq;

    assert(simd_string_eq(std::string_view(), std::string_view()));
    assert(!simd_string_eq(std::string_view("a"), std::string_view()));
    assert(simd_string_eq(std::string_view("short"), std::string_view("short")));
    assert(!simd_string_eq(std::string_view("short"), std::string_view("sh0rt")));

    std::string len16 = "1234567890abcdef";
    assert(len16.size() == 16);
    assert(simd_string_eq(len16, len16));

    std::string len32 = "1234567890abcdef1234567890abcdef";
    assert(len32.size() == 32);
    assert(simd_string_eq(len32, len32));
    std::string len32_alt = len32;
    len32_alt[31] = 'e';
    assert(!simd_string_eq(len32, len32_alt));

    std::cout << "✓ test_simd_string_eq_edges passed\n";
}

// ============================================================================
// Escape Sequences Tests
// ============================================================================

void test_backslash_escape() {
    auto result = parse_json(R"("back\\slash")");
    assert(result.ok());
    assert(result.value.as_string() == "back\\slash");
    std::cout << "✓ test_backslash_escape passed\n";
}

void test_quote_escape() {
    auto result = parse_json(R"("quote\"here")");
    assert(result.ok());
    assert(result.value.as_string() == "quote\"here");
    std::cout << "✓ test_quote_escape passed\n";
}

void test_forward_slash_escape() {
    auto result = parse_json(R"("forward\/slash")");
    assert(result.ok());
    assert(result.value.as_string() == "forward/slash");
    std::cout << "✓ test_forward_slash_escape passed\n";
}

void test_backspace_escape() {
    auto result = parse_json(R"("back\bspace")");
    assert(result.ok());
    assert(result.value.as_string() == "back\bspace");
    std::cout << "✓ test_backspace_escape passed\n";
}

void test_formfeed_escape() {
    auto result = parse_json(R"("form\ffeed")");
    assert(result.ok());
    assert(result.value.as_string() == "form\ffeed");
    std::cout << "✓ test_formfeed_escape passed\n";
}

void test_newline_escape() {
    auto result = parse_json(R"("new\nline")");
    assert(result.ok());
    assert(result.value.as_string() == "new\nline");
    std::cout << "✓ test_newline_escape passed\n";
}

void test_carriage_return_escape() {
    auto result = parse_json(R"("carriage\rreturn")");
    assert(result.ok());
    assert(result.value.as_string() == "carriage\rreturn");
    std::cout << "✓ test_carriage_return_escape passed\n";
}

void test_tab_escape() {
    auto result = parse_json(R"("tab\there")");
    assert(result.ok());
    assert(result.value.as_string() == "tab\there");
    std::cout << "✓ test_tab_escape passed\n";
}

void test_multiple_escapes() {
    auto result = parse_json(R"("line1\nline2\ttab\r\nline3")");
    assert(result.ok());
    assert(result.value.as_string() == "line1\nline2\ttab\r\nline3");
    std::cout << "✓ test_multiple_escapes passed\n";
}

void test_escaped_quote_in_middle() {
    auto result = parse_json(R"("He said \"hello\"")");
    assert(result.ok());
    assert(result.value.as_string() == "He said \"hello\"");
    std::cout << "✓ test_escaped_quote_in_middle passed\n";
}

void test_backslash_at_end() {
    auto result = parse_json(R"("ends with \\")");
    assert(result.ok());
    assert(result.value.as_string() == "ends with \\");
    std::cout << "✓ test_backslash_at_end passed\n";
}

void test_all_valid_escapes_together() {
    auto result = parse_json(R"("\"\\\/ \b\f\n\r\t")");
    assert(result.ok());
    std::string s = result.value.as_string();
    // Verify key components are present
    assert(s.find('"') != std::string::npos);
    assert(s.find('\\') != std::string::npos);
    assert(s.find('/') != std::string::npos);
    assert(s.find('\b') != std::string::npos);
    assert(s.find('\f') != std::string::npos);
    assert(s.find('\n') != std::string::npos);
    assert(s.find('\r') != std::string::npos);
    assert(s.find('\t') != std::string::npos);
    std::cout << "✓ test_all_valid_escapes_together passed\n";
}

// ============================================================================
// Unicode Support Tests
// ============================================================================

void test_ascii_text() {
    auto result = parse_json("\"Hello, World!\"");
    assert(result.ok());
    assert(result.value.as_string() == "Hello, World!");
    std::cout << "✓ test_ascii_text passed\n";
}

void test_utf8_latin_extended() {
    auto result = parse_json("\"café\"");
    assert(result.ok());
    assert(result.value.as_string() == "café");
    std::cout << "✓ test_utf8_latin_extended passed\n";
}

void test_utf8_cyrillic() {
    auto result = parse_json("\"Привет\"");
    assert(result.ok());
    assert(result.value.as_string() == "Привет");
    std::cout << "✓ test_utf8_cyrillic passed\n";
}

void test_utf8_chinese() {
    auto result = parse_json("\"你好\"");
    assert(result.ok());
    assert(result.value.as_string() == "你好");
    std::cout << "✓ test_utf8_chinese passed\n";
}

void test_utf8_japanese() {
    auto result = parse_json("\"こんにちは\"");
    assert(result.ok());
    assert(result.value.as_string() == "こんにちは");
    std::cout << "✓ test_utf8_japanese passed\n";
}

void test_utf8_arabic() {
    auto result = parse_json("\"مرحبا\"");
    assert(result.ok());
    assert(result.value.as_string() == "مرحبا");
    std::cout << "✓ test_utf8_arabic passed\n";
}

void test_utf8_emoji() {
    auto result = parse_json("\"Hello 👋 World 🌍\"");
    assert(result.ok());
    assert(result.value.as_string() == "Hello 👋 World 🌍");
    std::cout << "✓ test_utf8_emoji passed\n";
}

void test_utf8_mixed_scripts() {
    auto result = parse_json("\"English français 中文 العربية\"");
    assert(result.ok());
    assert(result.value.as_string() == "English français 中文 العربية");
    std::cout << "✓ test_utf8_mixed_scripts passed\n";
}

void test_utf8_in_object_keys() {
    auto result = parse_json("{\"café\": \"coffee\", \"価格\": 100}");
    assert(result.ok());
    const auto& obj = result.value.as_object();
    assert(obj.at("café").as_string() == "coffee");
    assert(obj.at("価格").as_number() == 100);
    std::cout << "✓ test_utf8_in_object_keys passed\n";
}

void test_utf8_in_nested_structure() {
    auto result = parse_json("{\"user\": {\"name\": \"José\", \"city\": \"São Paulo\"}}");
    assert(result.ok());
    const auto& obj = result.value.as_object();
    assert(obj.at("user").as_object().at("name").as_string() == "José");
    assert(obj.at("user").as_object().at("city").as_string() == "São Paulo");
    std::cout << "✓ test_utf8_in_nested_structure passed\n";
}

void test_zero_width_characters() {
    // Zero-width space (U+200B) encoded as \u200b
    auto result = parse_json("\"a\\u200bb\"");
    assert(result.ok());
    std::string s = result.value.as_string();
    // UTF-8 encoding of U+200B is 3 bytes: E2 80 8B
    // So the string should be: 'a' (1 byte) + U+200B (3 bytes) + 'b' (1 byte) = 5 bytes
    assert(s.size() == 5);
    assert(s[0] == 'a');
    assert(s[4] == 'b');
    std::cout << "✓ test_zero_width_characters passed\n";
}

void test_surrogate_pairs() {
    // Mathematical bold text characters require surrogate pairs in UTF-16
    auto result = parse_json("\"𝕳𝖊𝖑𝖑𝖔\"");
    assert(result.ok());
    std::string s = result.value.as_string();
    assert(s.find("\xf0\x9d\x95\xb3") != std::string::npos); // UTF-8 for 𝕳
    std::cout << "✓ test_surrogate_pairs passed\n";
}

void test_bom_in_string() {
    // Byte order mark inside string (valid UTF-8)
    auto result = parse_json("\"\\ufeff text\"");
    assert(result.ok());
    std::string s = result.value.as_string();
    // BOM in UTF-8 is EF BB BF
    assert(s.size() > 3);
    assert((unsigned char)s[0] == 0xEF);
    assert((unsigned char)s[1] == 0xBB);
    assert((unsigned char)s[2] == 0xBF);
    std::cout << "✓ test_bom_in_string passed\n";
}

// ============================================================================
// Complex Escapes and Unicode Tests
// ============================================================================

void test_escaped_unicode_characters() {
    auto result_a = parse_json(R"("\u0041")");
    assert(result_a.ok());
    assert(result_a.value.as_string() == "A");

    auto result_omega = parse_json(R"("\u03A9")");
    assert(result_omega.ok());
    assert(result_omega.value.as_string() == "Ω");

    auto result_wave = parse_json(R"("\uD83D\uDC4B")");
    assert(result_wave.ok());
    assert(result_wave.value.as_string() == "👋");

    // Lone high surrogate should fail
    auto result_bad_high = parse_json(R"("\uD800")");
    assert(!result_bad_high.ok());

    // Lone low surrogate should fail
    auto result_bad_low = parse_json(R"("\uDC00")");
    assert(!result_bad_low.ok());

    std::cout << "✓ test_escaped_unicode_characters passed\n";
}

void test_mixed_escapes_and_unicode() {
    auto result = parse_json("\"Line 1\\n日本語\\tTab\"");
    assert(result.ok());
    std::string s = result.value.as_string();
    assert(s.find('\n') != std::string::npos);
    assert(s.find("日本語") != std::string::npos);
    assert(s.find('\t') != std::string::npos);
    std::cout << "✓ test_mixed_escapes_and_unicode passed\n";
}

void test_long_string_with_unicode() {
    // Build a long string with various Unicode characters
    std::string text;
    for (int i = 0; i < 10; ++i) {
        text += "The quick brown fox jumps over the lazy dog. ";
    }
    text += " 中文 العربية русский 日本語 한국어 हिन्दी ไทย עברית";
    std::string json_str = "\"" + text + "\"";
    auto result = parse_json(json_str);
    assert(result.ok());
    assert(result.value.as_string() == text);
    std::cout << "✓ test_long_string_with_unicode passed\n";
}

void test_special_whitespace_characters() {
    // Regular space
    auto result1 = parse_json("\"a b\"");
    assert(result1.ok());
    assert(result1.value.as_string() == "a b");

    // Non-breaking space (U+00A0)
    auto result2 = parse_json("\"a\\u00a0b\"");
    assert(result2.ok());
    std::string s = result2.value.as_string();
    // U+00A0 is 2 bytes in UTF-8: C2 A0
    assert(s.size() == 4); // 'a' + 2-byte NBSP + 'b'
    std::cout << "✓ test_special_whitespace_characters passed\n";
}

// ============================================================================
// String Boundaries Tests
// ============================================================================

void test_empty_string() {
    auto result = parse_json("\"\"");
    assert(result.ok());
    assert(result.value.as_string() == "");
    std::cout << "✓ test_empty_string passed\n";
}

void test_single_character() {
    auto result = parse_json("\"a\"");
    assert(result.ok());
    assert(result.value.as_string() == "a");
    std::cout << "✓ test_single_character passed\n";
}

void test_very_long_string() {
    std::string long_str(10000, 'a');
    std::string json_str = "\"" + long_str + "\"";
    auto result = parse_json(json_str);
    assert(result.ok());
    assert(result.value.as_string() == long_str);
    assert(result.value.as_string().size() == 10000);
    std::cout << "✓ test_very_long_string passed\n";
}

void test_string_with_many_escapes() {
    auto result = parse_json(R"("\n\n\n\n\t\t\t\t\r\r\r\r")");
    assert(result.ok());
    assert(result.value.as_string() == "\n\n\n\n\t\t\t\t\r\r\r\r");
    std::cout << "✓ test_string_with_many_escapes passed\n";
}

void test_string_all_printable_ascii() {
    // All printable ASCII characters (except quote and backslash which need escaping)
    std::string printable;
    for (int i = 32; i < 127; ++i) {
        char c = static_cast<char>(i);
        if (c != '"' && c != '\\') {
            printable += c;
        }
    }
    std::string json_str = "\"" + printable + "\"";
    auto result = parse_json(json_str);
    assert(result.ok());
    assert(result.value.as_string() == printable);
    std::cout << "✓ test_string_all_printable_ascii passed\n";
}

// ============================================================================
// Invalid UTF-8 Rejection Tests
// ============================================================================

void test_lone_continuation_byte() {
    // 0x80 is a lone continuation byte (invalid)
    auto result = parse_json("\"a\x80\x62\""); // "a" + 0x80 + "b"
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_lone_continuation_byte passed\n";
}

void test_overlong_nul() {
    // Overlong encoding of NUL (C0 80)
    auto result = parse_json("\"\xc0\x80\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_overlong_nul passed\n";
}

void test_invalid_lead_byte() {
    // 0xFF is never valid as a lead byte
    auto result = parse_json("\"\xff\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_invalid_lead_byte passed\n";
}

void test_truncated_two_byte() {
    // C2 without continuation byte
    auto result = parse_json("\"\xc2\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_truncated_two_byte passed\n";
}

void test_overlong_three_byte() {
    // Overlong encoding E0 80 80
    auto result = parse_json("\"\xe0\x80\x80\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_overlong_three_byte passed\n";
}

void test_surrogate_codepoint() {
    // ED A0 80 encodes U+D800 (surrogate)
    auto result = parse_json("\"\xed\xa0\x80\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_surrogate_codepoint passed\n";
}

void test_codepoint_above_10ffff() {
    // F4 90 80 80 would be U+110000 (above max)
    auto result = parse_json("\"\xf4\x90\x80\x80\"");
    assert(!result.ok());
    assert(result.status == Status::ParseError);
    std::cout << "✓ test_codepoint_above_10ffff passed\n";
}

// ============================================================================
// Direct SIMD UTF-8 Validation Tests
// ============================================================================

void test_validate_utf8_simd_basic() {
    using namespace strata::util;

    // Valid ASCII
    assert(validate_utf8_simd("hello", 5));
    assert(validate_utf8_simd("hello world", 11));

    // Empty string
    assert(validate_utf8_simd("", 0));

    // Single byte
    assert(validate_utf8_simd("a", 1));

    std::cout << "✓ test_validate_utf8_simd_basic passed\n";
}

void test_validate_utf8_simd_multibyte() {
    using namespace strata::util;

    // Valid 2-byte UTF-8 (U+0080)
    assert(validate_utf8_simd("\xC2\x80", 2));

    // Valid 3-byte UTF-8 (U+0800)
    assert(validate_utf8_simd("\xE0\xA0\x80", 3));

    // Valid 4-byte UTF-8 (U+10000)
    assert(validate_utf8_simd("\xF0\x90\x80\x80", 4));

    // Mixed ASCII and multibyte: "Hello café" = 9 ASCII + 2 UTF-8 bytes = 11
    const char* cafe = "Hello caf\xc3\xa9";
    assert(validate_utf8_simd(cafe, 11));

    std::cout << "✓ test_validate_utf8_simd_multibyte passed\n";
}

void test_validate_utf8_simd_invalid() {
    using namespace strata::util;

    // Lone continuation byte
    assert(!validate_utf8_simd("\x80", 1));
    assert(!validate_utf8_simd("a\x80" "b", 3));

    // Overlong encoding (NUL as 2 bytes)
    assert(!validate_utf8_simd("\xC0\x80", 2));

    // Surrogate codepoint (U+D800)
    assert(!validate_utf8_simd("\xED\xA0\x80", 3));

    // Codepoint above U+10FFFF
    assert(!validate_utf8_simd("\xF4\x90\x80\x80", 4));

    // Invalid lead byte (0xFF)
    assert(!validate_utf8_simd("\xFF", 1));

    // Truncated sequence (missing continuation)
    assert(!validate_utf8_simd("\xC2", 1));
    assert(!validate_utf8_simd("\xE0\xA0", 2));
    assert(!validate_utf8_simd("\xF0\x90\x80", 3));

    std::cout << "✓ test_validate_utf8_simd_invalid passed\n";
}

void test_validate_utf8_simd_long_strings() {
    using namespace strata::util;

    // Long ASCII string (tests SIMD path)
    std::string long_ascii(1000, 'a');
    assert(validate_utf8_simd(long_ascii.data(), long_ascii.size()));

    // Long string with multibyte at various positions
    for (size_t pos : {0, 15, 16, 31, 32, 63, 64, 127, 128, 500, 997}) {
        std::string s(1000, 'a');
        // Insert a valid 2-byte UTF-8 sequence
        s[pos] = '\xC3';
        s[pos + 1] = '\xA9';  // é
        assert(validate_utf8_simd(s.data(), s.size()));
    }

    // Long string with invalid byte at various positions
    for (size_t pos : {0, 15, 16, 31, 32, 500, 999}) {
        std::string s(1000, 'a');
        s[pos] = '\x80';  // Invalid lone continuation
        assert(!validate_utf8_simd(s.data(), s.size()));
    }

    std::cout << "✓ test_validate_utf8_simd_long_strings passed\n";
}

void test_validate_utf8_lazy() {
    using namespace strata::util;

    // Pure ASCII - should be fast (no full validation needed)
    assert(validate_utf8_lazy("hello world", 11));

    // Long ASCII string
    std::string long_ascii(1000, 'a');
    assert(validate_utf8_lazy(long_ascii.data(), long_ascii.size()));

    // Valid UTF-8 with high bytes: "Hello café" = 11 bytes
    const char* cafe = "Hello caf\xc3\xa9";
    assert(validate_utf8_lazy(cafe, 11));

    // Invalid UTF-8
    assert(!validate_utf8_lazy("a\x80" "b", 3));

    std::cout << "✓ test_validate_utf8_lazy passed\n";
}

void test_is_ascii_only_simd() {
    using namespace strata::util;

    // Pure ASCII
    assert(is_ascii_only_simd("hello world", 11));
    assert(is_ascii_only_simd("1234567890", 10));

    // Empty string
    assert(is_ascii_only_simd("", 0));

    // High byte present
    assert(!is_ascii_only_simd("\x80", 1));
    assert(!is_ascii_only_simd("hello\x80world", 11));
    assert(!is_ascii_only_simd("caf\xc3\xa9", 5));

    // Long ASCII string (SIMD path)
    std::string long_ascii(1000, 'a');
    assert(is_ascii_only_simd(long_ascii.data(), long_ascii.size()));

    // Long string with high byte at various positions
    for (size_t pos : {0, 15, 16, 31, 32, 500, 999}) {
        std::string s(1000, 'a');
        s[pos] = '\x80';
        assert(!is_ascii_only_simd(s.data(), s.size()));
    }

    std::cout << "✓ test_is_ascii_only_simd passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running Unicode and Escapes tests...\n\n";

    // SIMD string equality
    std::cout << "--- SIMD String Equality ---\n";
    test_simd_string_eq_edges();

    // Escape sequences
    std::cout << "--- Escape Sequences ---\n";
    test_backslash_escape();
    test_quote_escape();
    test_forward_slash_escape();
    test_backspace_escape();
    test_formfeed_escape();
    test_newline_escape();
    test_carriage_return_escape();
    test_tab_escape();
    test_multiple_escapes();
    test_escaped_quote_in_middle();
    test_backslash_at_end();
    test_all_valid_escapes_together();

    // Unicode support
    std::cout << "\n--- Unicode Support ---\n";
    test_ascii_text();
    test_utf8_latin_extended();
    test_utf8_cyrillic();
    test_utf8_chinese();
    test_utf8_japanese();
    test_utf8_arabic();
    test_utf8_emoji();
    test_utf8_mixed_scripts();
    test_utf8_in_object_keys();
    test_utf8_in_nested_structure();
    test_zero_width_characters();
    test_surrogate_pairs();
    test_bom_in_string();

    // Complex escapes and Unicode
    std::cout << "\n--- Complex Escapes and Unicode ---\n";
    test_escaped_unicode_characters();
    test_mixed_escapes_and_unicode();
    test_long_string_with_unicode();
    test_special_whitespace_characters();

    // String boundaries
    std::cout << "\n--- String Boundaries ---\n";
    test_empty_string();
    test_single_character();
    test_very_long_string();
    test_string_with_many_escapes();
    test_string_all_printable_ascii();

    // Invalid UTF-8 rejection
    std::cout << "\n--- Invalid UTF-8 Rejection ---\n";
    test_lone_continuation_byte();
    test_overlong_nul();
    test_invalid_lead_byte();
    test_truncated_two_byte();
    test_overlong_three_byte();
    test_surrogate_codepoint();
    test_codepoint_above_10ffff();

    // Direct SIMD UTF-8 validation tests
    std::cout << "\n--- Direct SIMD UTF-8 Validation ---\n";
    test_validate_utf8_simd_basic();
    test_validate_utf8_simd_multibyte();
    test_validate_utf8_simd_invalid();
    test_validate_utf8_simd_long_strings();
    test_validate_utf8_lazy();
    test_is_ascii_only_simd();

    std::cout << "\n✅ All Unicode and Escapes tests passed!\n";
    return 0;
}

/**
 * test_lazy_string.cpp - Tests for LazyString deferred unescape functionality
 */

#include "strata/util/lazy_string.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace strata;

// ============================================================================
// Basic functionality tests
// ============================================================================

void test_no_escapes_construction() {
    // String without escapes - raw IS the value
    LazyString ls("hello", false);

    assert(ls.raw() == "hello");
    assert(!ls.has_escapes());
    assert(ls.value() == "hello");
    assert(ls.is_materialized()); // Should be materialized immediately for no escapes

    std::cout << "✓ test_no_escapes_construction passed\n";
}

void test_empty_string() {
    LazyString empty;

    assert(empty.raw() == "");
    assert(empty.empty());
    assert(!empty.has_escapes());
    assert(empty.value() == "");

    std::cout << "✓ test_empty_string passed\n";
}

void test_with_escapes_deferred() {
    // String with escapes - unescape is deferred
    LazyString ls("hello\\nworld", true);

    assert(ls.raw() == "hello\\nworld");
    assert(ls.has_escapes());
    assert(!ls.is_materialized()); // Not yet materialized

    // Access value triggers unescaping
    const std::string& val = ls.value();
    assert(val == "hello\nworld");
    assert(ls.is_materialized()); // Now materialized

    // Second access doesn't re-unescape (same reference)
    const std::string& val2 = ls.value();
    assert(&val == &val2);

    std::cout << "✓ test_with_escapes_deferred passed\n";
}

// ============================================================================
// Escape sequence tests
// ============================================================================

void test_simple_escapes() {
    // Test all simple escape sequences
    struct TestCase {
        std::string raw;
        std::string expected;
    };

    TestCase cases[] = {
        {"\\\"", "\""},
        {"\\\\", "\\"},
        {"\\/", "/"},
        {"\\b", "\b"},
        {"\\f", "\f"},
        {"\\n", "\n"},
        {"\\r", "\r"},
        {"\\t", "\t"},
        {"a\\nb\\tc", "a\nb\tc"},
        {"\\n\\r\\t", "\n\r\t"},
    };

    for (const auto& tc : cases) {
        LazyString ls(tc.raw, true);
        assert(ls.value() == tc.expected);
    }

    std::cout << "✓ test_simple_escapes passed\n";
}

void test_unicode_escapes() {
    // Test \uXXXX escape sequences
    struct TestCase {
        std::string raw;
        std::string expected;
    };

    TestCase cases[] = {
        {"\\u0041", "A"},           // Basic ASCII
        {"\\u03A9", "Ω"},           // Greek capital omega
        {"\\u4E2D", "中"},          // Chinese character
        {"\\u0048\\u0065\\u006C\\u006C\\u006F", "Hello"}, // Multiple escapes
    };

    for (const auto& tc : cases) {
        LazyString ls(tc.raw, true);
        assert(ls.value() == tc.expected);
    }

    std::cout << "✓ test_unicode_escapes passed\n";
}

void test_surrogate_pairs() {
    // Test surrogate pair handling (emoji and supplementary characters)

    // 👋 = U+1F44B = surrogate pair D83D DC4B
    LazyString wave("\\uD83D\\uDC4B", true);
    assert(wave.value() == "👋");

    // 🎉 = U+1F389 = surrogate pair D83C DF89
    LazyString party("\\uD83C\\uDF89", true);
    assert(party.value() == "🎉");

    // 𝄞 = U+1D11E (musical G clef) = surrogate pair D834 DD1E
    LazyString clef("\\uD834\\uDD1E", true);
    assert(clef.value() == "𝄞");

    std::cout << "✓ test_surrogate_pairs passed\n";
}

void test_mixed_escapes() {
    // Test combination of different escape types
    LazyString ls("Hello\\nWorld\\u0021\\t\\\"quoted\\\"", true);
    assert(ls.value() == "Hello\nWorld!\t\"quoted\"");

    std::cout << "✓ test_mixed_escapes passed\n";
}

// ============================================================================
// Comparison tests (the key optimization)
// ============================================================================

void test_comparison_no_escapes() {
    // When neither has escapes, should compare raw bytes (fast path)
    LazyString ls1("hello", false);
    LazyString ls2("hello", false);
    LazyString ls3("world", false);

    assert(ls1 == ls2);
    assert(ls1 != ls3);

    // Also compare with std::string
    assert(ls1 == std::string("hello"));
    assert(ls1 != std::string("world"));

    // And string_view
    assert(ls1 == std::string_view("hello"));
    assert(ls1 != std::string_view("world"));

    std::cout << "✓ test_comparison_no_escapes passed\n";
}

void test_comparison_with_escapes() {
    // When one or both have escapes, must compare unescaped values
    LazyString with_escape("hello\\nworld", true);
    LazyString without_escape("hello\nworld", false);

    // These should be equal after unescaping
    assert(with_escape == without_escape);

    // Compare with std::string
    assert(with_escape == std::string("hello\nworld"));

    std::cout << "✓ test_comparison_with_escapes passed\n";
}

void test_comparison_optimization() {
    // Verify that comparison without escapes doesn't trigger unescaping
    LazyString ls1("test", false);
    LazyString ls2("test", false);

    // Both should already be materialized (no escapes)
    assert(ls1.is_materialized());
    assert(ls2.is_materialized());

    // Now test with escapes - should not trigger unescape for raw comparison
    LazyString ls3("foo", true);  // Has escapes flag but no actual escapes
    LazyString ls4("bar", true);

    assert(!ls3.is_materialized());
    assert(!ls4.is_materialized());

    // Comparing two LazyStrings with escapes WILL trigger unescaping
    // because we need the actual values to compare
    bool result = (ls3 == ls4);
    assert(!result); // "foo" != "bar"

    // After comparison, should be materialized
    assert(ls3.is_materialized());
    assert(ls4.is_materialized());

    std::cout << "✓ test_comparison_optimization passed\n";
}

// ============================================================================
// Copy and move semantics
// ============================================================================

void test_copy_construction() {
    LazyString original("hello\\nworld", true);

    // Trigger materialization
    std::string val = original.value();

    // Copy
    LazyString copy(original);

    assert(copy.raw() == original.raw());
    assert(copy.has_escapes() == original.has_escapes());
    assert(copy.value() == original.value());

    std::cout << "✓ test_copy_construction passed\n";
}

void test_move_construction() {
    LazyString original("hello\\nworld", true);
    original.materialize();

    std::string raw = std::string(original.raw());
    std::string val = original.value();

    // Move
    LazyString moved(std::move(original));

    assert(moved.raw() == raw);
    assert(moved.value() == val);
    assert(moved.is_materialized());

    std::cout << "✓ test_move_construction passed\n";
}

// ============================================================================
// from_unescaped factory method
// ============================================================================

void test_from_unescaped() {
    // Create from already-unescaped string
    std::string unescaped = "hello\nworld";
    LazyString ls = LazyString::from_unescaped(unescaped);

    assert(!ls.has_escapes());
    assert(ls.value() == unescaped);
    assert(ls.raw() == unescaped); // raw IS the unescaped value

    std::cout << "✓ test_from_unescaped passed\n";
}

// ============================================================================
// Size and utility methods
// ============================================================================

void test_size_and_empty() {
    LazyString empty;
    assert(empty.empty());
    assert(empty.size() == 0);

    LazyString ls("hello", false);
    assert(!ls.empty());
    assert(ls.size() == 5);

    // With escapes, size is of the unescaped value
    LazyString escaped("\\n\\n\\n", true);
    assert(escaped.size() == 3); // Three newline characters

    std::cout << "✓ test_size_and_empty passed\n";
}

void test_to_string() {
    LazyString ls("hello\\nworld", true);
    std::string s = ls.to_string();

    assert(s == "hello\nworld");

    std::cout << "✓ test_to_string passed\n";
}

void test_string_view_conversion() {
    LazyString ls("hello", false);
    std::string_view sv = ls; // Implicit conversion

    assert(sv == "hello");

    std::cout << "✓ test_string_view_conversion passed\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Running LazyString tests...\n\n";

    // Basic functionality
    test_no_escapes_construction();
    test_empty_string();
    test_with_escapes_deferred();

    // Escape sequences
    test_simple_escapes();
    test_unicode_escapes();
    test_surrogate_pairs();
    test_mixed_escapes();

    // Comparison (optimization focus)
    test_comparison_no_escapes();
    test_comparison_with_escapes();
    test_comparison_optimization();

    // Copy/move semantics
    test_copy_construction();
    test_move_construction();

    // Factory and utility methods
    test_from_unescaped();
    test_size_and_empty();
    test_to_string();
    test_string_view_conversion();

    std::cout << "\n✅ All LazyString tests passed!\n";
    return 0;
}

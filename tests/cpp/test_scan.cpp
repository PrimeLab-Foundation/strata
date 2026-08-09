/**
 * @file test_scan.cpp
 * @brief The SIMD escape scanner must be indistinguishable from its twin.
 *
 * `find_next_escape` is compiled to NEON or SSE2 where available. The
 * styleguide's rule for that is "every SIMD function needs a scalar twin with
 * identical observable behavior" — this suite is what turns that from a
 * promise into a check, by running the two implementations against each other
 * over inputs chosen to hit the places vectorized scans break:
 *
 *   - lengths either side of the 16-byte block, including 0 and the tail;
 *   - a match at every position, so the count-trailing-zeros lane index is
 *     verified for all 16 lanes rather than just the first;
 *   - every byte value, so the "control byte" boundary at 0x20 and the
 *     unsigned comparison for bytes >= 0x80 are both covered;
 *   - unaligned starts, since the loads are unaligned by design.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/util/scan.hpp"

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

namespace {

using strata::util::find_next_escape;
using strata::util::find_next_escape_scalar;

void check(const char* data, size_t len) {
    const size_t simd = find_next_escape(data, len);
    const size_t scalar = find_next_escape_scalar(data, len);
    if (simd != scalar) {
        std::printf("disagreement at len=%zu: simd=%zu scalar=%zu\n", len, simd, scalar);
        assert(false);
    }
    assert(simd <= len);
}

void test_clean_strings_of_every_length() {
    std::string text;
    for (size_t len = 0; len <= 200; ++len) {
        check(text.data(), text.size());
        assert(find_next_escape(text.data(), text.size()) == text.size());
        text.push_back('a');
    }
}

void test_match_at_every_position() {
    // Every escape-forcing byte, planted at every offset of a long buffer.
    for (const char needle : {'"', '\\', '\0', '\x01', '\x1f', '\n', '\t'}) {
        for (size_t len = 1; len <= 80; ++len) {
            for (size_t position = 0; position < len; ++position) {
                std::string text(len, 'x');
                text[position] = needle;
                check(text.data(), text.size());
                assert(find_next_escape(text.data(), text.size()) == position);
            }
        }
    }
}

void test_every_byte_value() {
    // 0x20 is the boundary: below escapes, at and above does not (except the
    // two literals). 0x80+ must never be flagged or UTF-8 would be mangled.
    for (int value = 0; value < 256; ++value) {
        const char byte = static_cast<char>(value);
        std::string text(40, 'x');
        text[20] = byte;
        check(text.data(), text.size());

        const bool escapes = value < 0x20 || value == '"' || value == '\\';
        const size_t found = find_next_escape(text.data(), text.size());
        assert(found == (escapes ? 20u : text.size()));
    }
}

void test_unaligned_starts() {
    // The buffer is scanned from every offset, so no implementation may assume
    // a 16-byte-aligned pointer.
    std::string text(300, 'y');
    text[250] = '"';
    for (size_t offset = 0; offset < 64; ++offset)
        check(text.data() + offset, text.size() - offset);
}

void test_multibyte_utf8_is_never_flagged() {
    // "héllo wörld 漢字 🌍" — continuation bytes are all >= 0x80.
    const std::string text = "h\xc3\xa9llo w\xc3\xb6rld \xe6\xbc\xa2\xe5\xad\x97 \xf0\x9f\x8c\x8d";
    check(text.data(), text.size());
    assert(find_next_escape(text.data(), text.size()) == text.size());

    // ...and a quote after them is still found at the right offset.
    std::string quoted = text;
    quoted.push_back('"');
    assert(find_next_escape(quoted.data(), quoted.size()) == text.size());
}

void test_dense_matches() {
    // A block that is entirely escapes must report the first one.
    const std::string text(64, '"');
    check(text.data(), text.size());
    assert(find_next_escape(text.data(), text.size()) == 0);
}

} // namespace

int main() {
    test_clean_strings_of_every_length();
    test_match_at_every_position();
    test_every_byte_value();
    test_unaligned_starts();
    test_multibyte_utf8_is_never_flagged();
    test_dense_matches();

    std::puts("scan_tests: OK");
    return 0;
}

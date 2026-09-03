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

#include "strata/util/dtoa.hpp"
#include "strata/util/scan.hpp"

#include <cassert>
#include <charconv>
#include <cstdio>
#include <cstring>
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

void test_fused_copy_matches_scan() {
    // copy_until_escape must stop exactly where find_next_escape stops, and
    // the bytes before the stop must be a faithful copy. Destination is
    // block-rounded per the contract.
    for (const char needle : {'"', '\\', '\0', '\x1f', '\n'}) {
        for (size_t len = 0; len <= 96; ++len) {
            for (size_t position = 0; position <= len; ++position) {
                std::string text(len, 'p');
                if (position < len)
                    text[position] = needle;
                std::vector<char> fused((len + 15) / 16 * 16 + 16, '\xAA');
                std::vector<char> twin(fused.size(), '\xAA');

                const size_t got = strata::util::copy_until_escape(text.data(), len, fused.data());
                const size_t want =
                    strata::util::copy_until_escape_scalar(text.data(), len, twin.data());
                assert(got == want);
                assert(got == strata::util::find_next_escape(text.data(), len));
                assert(std::memcmp(fused.data(), text.data(), got) == 0);
                assert(std::memcmp(twin.data(), text.data(), got) == 0);
            }
        }
    }
    // The short tiers (under one SIMD block) pad and overlap their words;
    // bytes at or above 0x80 -- UTF-8 lead and continuation bytes -- must
    // pass through them unflagged at every length and position, with an
    // escape anywhere still found first.
    for (const char filler : {'\x80', '\xff', '\xc3', '\x20', '\x7f'}) {
        for (size_t len = 0; len <= 40; ++len) {
            for (size_t position = 0; position <= len; ++position) {
                std::string text(len, filler);
                if (position < len)
                    text[position] = '\\';
                std::vector<char> fused((len + 15) / 16 * 16 + 16, '\xAA');
                std::vector<char> twin(fused.size(), '\xAA');
                const size_t got = strata::util::copy_until_escape(text.data(), len, fused.data());
                const size_t want =
                    strata::util::copy_until_escape_scalar(text.data(), len, twin.data());
                assert(got == want);
                assert(got == strata::util::find_next_escape(text.data(), len));
                assert(std::memcmp(fused.data(), text.data(), got) == 0);
            }
        }
    }
}

void check_utf8(const std::string& text) {
    const bool fast = strata::util::validate_utf8(text.data(), text.size());
    const bool twin = strata::util::validate_utf8_scalar(text.data(), text.size());
    if (fast != twin) {
        std::printf("utf8 disagreement at len=%zu: fast=%d twin=%d\n", text.size(), fast, twin);
        assert(false);
    }
}

void test_utf8_ascii_runs_every_length() {
    // The bulk-ASCII tier's block/word/byte boundaries, from every offset, so
    // no step size may assume alignment or a minimum remaining length.
    std::string text(200, 'a');
    for (size_t len = 0; len <= text.size(); ++len) {
        check_utf8(text.substr(0, len));
        assert(strata::util::validate_utf8(text.data(), len));
    }
    for (size_t offset = 0; offset < 64; ++offset)
        assert(strata::util::validate_utf8(text.data() + offset, text.size() - offset));
}

void test_utf8_non_ascii_at_every_position() {
    // A valid two-byte sequence, a lone continuation and a truncated lead,
    // planted at every offset of a 96-byte buffer: the fast path must hand
    // over to the precise check at exactly the right byte, wherever the block
    // scan finds it.
    for (size_t position = 0; position + 1 < 96; ++position) {
        std::string valid(96, 'x');
        valid[position] = '\xc3';
        valid[position + 1] = '\xa9'; // é
        check_utf8(valid);
        assert(strata::util::validate_utf8(valid.data(), valid.size()));

        std::string lone(96, 'x');
        lone[position] = '\x80'; // continuation with no lead
        check_utf8(lone);
        assert(!strata::util::validate_utf8(lone.data(), lone.size()));

        std::string truncated(96, 'x');
        truncated[position] = '\xc3'; // lead followed by ASCII
        check_utf8(truncated);
        assert(!strata::util::validate_utf8(truncated.data(), truncated.size()));
    }
}

void test_utf8_sequence_rules_survive_the_fast_path() {
    // The rejection contract (overlongs, surrogates, range, truncation) after
    // enough leading ASCII to engage every bulk tier first.
    const std::string prefixes[] = {"",
                                    "a",
                                    std::string(7, 'a'),
                                    std::string(8, 'a'),
                                    std::string(15, 'a'),
                                    std::string(16, 'a'),
                                    std::string(33, 'a'),
                                    std::string(64, 'a')};
    const struct {
        const char* bytes;
        bool valid;
    } cases[] = {
        {"\xc3\xa9", true},          // é
        {"\xe6\xbc\xa2", true},      // 漢
        {"\xf0\x9f\x8c\x8d", true},  // 🌍
        {"\xc0\xaf", false},         // overlong '/'
        {"\xc1\x81", false},         // overlong
        {"\xe0\x80\x80", false},     // overlong E0
        {"\xe0\x9f\xbf", false},     // overlong E0 boundary
        {"\xed\xa0\x80", false},     // surrogate half
        {"\xf0\x80\x80\x80", false}, // overlong F0
        {"\xf4\x90\x80\x80", false}, // above U+10FFFF
        {"\xf5\x80\x80\x80", false}, // lead past U+10FFFF
        {"\xc3", false},             // truncated at end of input
        {"\xe6\xbc", false},         // truncated three-byte
        {"\xf0\x9f\x8c", false},     // truncated four-byte
    };
    for (const std::string& prefix : prefixes) {
        for (const auto& item : cases) {
            const std::string text = prefix + item.bytes;
            check_utf8(text);
            assert(strata::util::validate_utf8(text.data(), text.size()) == item.valid);
            // ...and with an ASCII tail, so the sequence sits mid-input and
            // the fast path has to resume bulk scanning after it.
            const std::string framed = prefix + item.bytes + std::string(20, 'z');
            check_utf8(framed);
            assert(strata::util::validate_utf8(framed.data(), framed.size()) == item.valid);
        }
    }
}

void test_utf8_every_lead_byte() {
    // Every byte value as the first non-ASCII candidate after one block of
    // ASCII — the twin decides which are valid single-byte, which start a
    // sequence, and which can never appear.
    for (int value = 0; value < 256; ++value) {
        std::string text(16, 'a');
        text.push_back(static_cast<char>(value));
        text.append("\xbf\xbf\xbf"); // plausible continuations behind it
        check_utf8(text);
    }
}

void test_format_int64_matches_to_chars() {
    const auto check = [](long long value) {
        char ours[32];
        char reference[32];
        const size_t got = strata::util::format_int64(value, ours);
        const auto converted = std::to_chars(reference, reference + sizeof(reference), value);
        const auto want = static_cast<size_t>(converted.ptr - reference);
        assert(got == want);
        assert(std::memcmp(ours, reference, got) == 0);
    };
    check(0);
    check(-1);
    check(9223372036854775807LL);
    check(-9223372036854775807LL - 1); // INT64_MIN
    for (long long power = 1; power <= 1000000000000000000LL; power *= 10) {
        check(power - 1);
        check(power);
        check(-power);
        check(-power + 1);
    }
    unsigned long long state = 0x9E3779B97F4A7C15ULL;
    for (int trial = 0; trial < 200000; ++trial) {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        check(static_cast<long long>(state));
    }
    // The writer may store a whole word past the digits: the contract is
    // that nothing beyond kInt64BufferSize bytes from `out` is touched, and
    // nothing before it. Sentinels on both sides, exhaustively over the
    // 4-byte word tier (every value under 10^4, both signs) and at the loop
    // path's widest outputs.
    const auto check_window = [](long long value) {
        char buffer[48];
        std::memset(buffer, '\xAA', sizeof buffer);
        char* out = buffer + 8;
        const size_t got = strata::util::format_int64(value, out);
        assert(got <= strata::util::kInt64BufferSize);
        for (size_t index = 0; index < 8; ++index)
            assert(buffer[index] == '\xAA');
        for (size_t index = 8 + strata::util::kInt64BufferSize; index < sizeof buffer; ++index)
            assert(buffer[index] == '\xAA');
    };
    for (long long value = 0; value < 10000; ++value) {
        check_window(value);
        check_window(-value);
        check(value);
        check(-value);
    }
    check_window(9223372036854775807LL);
    check_window(-9223372036854775807LL - 1);
    check_window(999999999999999999LL);
    // Random 64-bit values are almost all 19 or 20 characters; the writer's
    // tiers turn on digit count, so every width from 1 to 19 digits gets its
    // own random sweep, both signs.
    unsigned long long width_limit = 10;
    for (int digits = 1; digits <= 19; ++digits, width_limit *= 10) {
        const unsigned long long low = width_limit / 10;
        for (int trial = 0; trial < 4000; ++trial) {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            const unsigned long long magnitude = low + state % (width_limit - low);
            check(static_cast<long long>(magnitude));
            check(-static_cast<long long>(magnitude));
        }
    }
}

int main() {
    test_clean_strings_of_every_length();
    test_match_at_every_position();
    test_every_byte_value();
    test_unaligned_starts();
    test_multibyte_utf8_is_never_flagged();
    test_dense_matches();
    test_fused_copy_matches_scan();
    test_utf8_ascii_runs_every_length();
    test_utf8_non_ascii_at_every_position();
    test_utf8_sequence_rules_survive_the_fast_path();
    test_utf8_every_lead_byte();
    test_format_int64_matches_to_chars();

    std::puts("scan_tests: OK");
    return 0;
}

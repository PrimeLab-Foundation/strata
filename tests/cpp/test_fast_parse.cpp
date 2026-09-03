/**
 * @file test_fast_parse.cpp
 * @brief The number scanner's word-at-a-time digit run against its scalar twin.
 *
 * `consume_digit_run` steps eight bytes at a time by word; the styleguide's
 * twin rule says such a path must be observably identical to its scalar
 * definition, and this suite checks that rather than assumes it: every run
 * length from zero to twenty-five, every leading-zero pattern, every
 * terminator class, and every distance to the end of the input — the
 * conditions that select between the word step, the cap, and the per-digit
 * tail. Then the whole number parser over the same shapes against
 * `from_chars`.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parser_inline.hpp"
#include "strata/util/fast_parse.hpp"

#include <cassert>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

using strata::util::detail::consume_digit_run;
using strata::util::detail::consume_digit_run_scalar;
using strata::util::detail::DigitAccumulator;

namespace {

struct Outcome {
    size_t consumed;
    size_t pos;
    DigitAccumulator acc;
};

[[nodiscard]] Outcome run_word(const std::string& text, size_t pos, DigitAccumulator acc) {
    Outcome out{0, pos, acc};
    out.consumed = consume_digit_run<true>(text.data(), text.size(), out.pos, out.acc);
    return out;
}

[[nodiscard]] Outcome run_scalar(const std::string& text, size_t pos, DigitAccumulator acc) {
    Outcome out{0, pos, acc};
    out.consumed = consume_digit_run_scalar(text.data(), text.size(), out.pos, out.acc);
    return out;
}

[[nodiscard]] Outcome run_word_integer(const std::string& text, size_t pos, DigitAccumulator acc) {
    Outcome out{0, pos, acc};
    out.consumed = consume_digit_run<false>(text.data(), text.size(), out.pos, out.acc);
    return out;
}

void expect_same(const std::string& text, size_t pos, const DigitAccumulator& start) {
    const Outcome a = run_word(text, pos, start);
    const Outcome b = run_scalar(text, pos, start);
    // The integer instantiation is defined only where the scalar twin would
    // count no leading zeros: a run that does not start with '0', or an
    // accumulator that already holds digits.
    if (start.mantissa != 0 || pos >= text.size() || text[pos] != '0') {
        const Outcome c = run_word_integer(text, pos, start);
        assert(c.consumed == b.consumed && c.pos == b.pos && c.acc.mantissa == b.acc.mantissa &&
               c.acc.significant == b.acc.significant &&
               c.acc.leading_zeros == b.acc.leading_zeros && c.acc.truncated == b.acc.truncated);
    }
    const bool same = a.consumed == b.consumed && a.pos == b.pos &&
                      a.acc.mantissa == b.acc.mantissa && a.acc.significant == b.acc.significant &&
                      a.acc.leading_zeros == b.acc.leading_zeros &&
                      a.acc.truncated == b.acc.truncated;
    if (!same) {
        std::printf("digit run mismatch on \"%s\" at %zu: word (%zu, %llu, %zu, %zu, %d) vs "
                    "scalar (%zu, %llu, %zu, %zu, %d)\n",
                    text.c_str(), pos, a.consumed, static_cast<unsigned long long>(a.acc.mantissa),
                    a.acc.significant, a.acc.leading_zeros, a.acc.truncated, b.consumed,
                    static_cast<unsigned long long>(b.acc.mantissa), b.acc.significant,
                    b.acc.leading_zeros, b.acc.truncated);
        assert(false);
    }
}

/// Deterministic digits: xorshift, so the sweep is reproducible without a seed source.
uint64_t next(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

/// Digits with a chosen number of leading zeros; the first significant digit
/// is never zero, every later position draws from all ten digits so interior
/// and trailing zeros ("1000000", "0.0100", "10.05") are sampled too.
std::string digits_of(uint64_t& state, size_t length, size_t leading_zeros, bool all_zero) {
    std::string out;
    for (size_t index = 0; index < length; ++index) {
        if (all_zero || index < leading_zeros) {
            out.push_back('0');
        } else if (index == leading_zeros) {
            out.push_back(static_cast<char>('0' + (next(state) % 9) + 1));
        } else {
            out.push_back(static_cast<char>('0' + (next(state) % 10)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------

/// Every run length, zero pattern, terminator and tail distance: the two
/// consumers must land on the same byte with the same accumulator.
void test_word_run_matches_scalar_twin() {
    const char terminators[] = {',',
                                ']',
                                '}',
                                ' ',
                                '.',
                                'e',
                                'E',
                                '-',
                                '+',
                                'a',
                                '/',
                                ':',
                                '"',
                                '\n',
                                '\0',
                                static_cast<char>(0x80),
                                static_cast<char>(0xFF)};
    uint64_t state = 0x9E3779B97F4A7C15ULL;
    for (size_t length = 0; length <= 25; ++length) {
        for (size_t zeros = 0; zeros <= length && zeros <= 9; ++zeros) {
            for (const bool all_zero : {false, true}) {
                const std::string digits = digits_of(state, length, zeros, all_zero);
                for (const char terminator : terminators) {
                    // Terminated, and terminated with room for a word past
                    // the run, and unterminated (the run ends the input).
                    const std::string padded = digits + terminator + std::string(9, 'x');
                    const std::string tight = digits + terminator;
                    for (const std::string& text : {padded, tight, digits}) {
                        // Preceding bytes shift the run's alignment against the
                        // eight-byte window; the accumulator arrives either
                        // empty (integer part) or already holding digits
                        // (fraction after an integer part), zero or not.
                        for (const size_t lead : {size_t{0}, size_t{1}, size_t{3}, size_t{7}}) {
                            const std::string shifted = std::string(lead, '1') + text;
                            for (const DigitAccumulator& start :
                                 {DigitAccumulator{}, DigitAccumulator{0, 0, 2, false},
                                  DigitAccumulator{123, 3, 0, false},
                                  DigitAccumulator{1234567890123456789ULL, 19, 0, false},
                                  DigitAccumulator{12345678901234567ULL, 17, 0, false}}) {
                                expect_same(shifted, lead, start);
                            }
                        }
                    }
                }
            }
        }
    }
}

[[nodiscard]] bool parse_ok(const std::string& text, strata::util::ParsedNumber& number) {
    return strata::util::parse_number_unified(text.data(), text.size(), number) &&
           number.consumed == text.size();
}

/// Integers of every width through the whole parser, against from_chars.
void test_integers_of_every_width_match_from_chars() {
    uint64_t state = 0x243F6A8885A308D3ULL;
    for (size_t length = 1; length <= 19; ++length) {
        for (int trial = 0; trial < 200; ++trial) {
            std::string digits = digits_of(state, length, 0, false);
            for (const bool negative : {false, true}) {
                const std::string text = (negative ? "-" : "") + digits;
                strata::util::ParsedNumber number;
                assert(parse_ok(text, number));
                long long expected = 0;
                const auto converted =
                    std::from_chars(text.data(), text.data() + text.size(), expected);
                if (converted.ec == std::errc::result_out_of_range) {
                    assert(number.kind == strata::util::NumberKind::BigInt);
                } else {
                    assert(converted.ec == std::errc{});
                    assert(number.kind == strata::util::NumberKind::Int64);
                    assert(number.int_value == expected);
                }
            }
        }
    }
    // Twenty-plus digits: always big, never truncated to a wrong int.
    strata::util::ParsedNumber number;
    assert(parse_ok("123456789012345678901234", number));
    assert(number.kind == strata::util::NumberKind::BigInt);
    assert(parse_ok("18446744073709551616", number));
    assert(number.kind == strata::util::NumberKind::BigInt);
    assert(parse_ok("9223372036854775807", number));
    assert(number.kind == strata::util::NumberKind::Int64);
    assert(number.int_value == 9223372036854775807LL);
    assert(parse_ok("-9223372036854775808", number));
    assert(number.kind == strata::util::NumberKind::Int64);
}

/// Fractions with leading zeros, long digit strings past the cap, and the
/// zero-mantissa shapes — the paths where the accumulator's bookkeeping
/// decides the exponent.
void test_fractions_and_leading_zeros_match_from_chars() {
    const char* texts[] = {"0.0",
                           "0.5",
                           "0.000001",
                           "0.0000000000000000000001",
                           "0.00000000000000000000000000000000000001",
                           "0.8444218515250481",
                           "0.30000000000000004",
                           "123.45678",
                           "-750.34767",
                           "1.0000000000000000000000001",
                           "100000000.00000001",
                           "0.10000000000000000555",
                           "12345678901234567890.5",
                           "1234567890.1234567890123456789",
                           "0.000000000000000000000000000000000000000000001e10",
                           "1e-320",
                           "0.000000001e-320",
                           "0.00000000000000000000000000000000000000000000000000000000001e-300",
                           "1e400",
                           "0.000000001e400",
                           "00",
                           "01",
                           "1.",
                           ".5",
                           "1e",
                           "-",
                           "1.5e+"};
    for (const char* raw : texts) {
        const std::string text(raw);
        strata::util::ParsedNumber number;
        const bool ok = parse_ok(text, number);
        double expected = 0.0;
        const auto converted =
            strata::util::from_chars_double(text.data(), text.data() + text.size(), expected);
        const bool reference_ok = converted.ec != std::errc::invalid_argument &&
                                  converted.ptr == text.data() + text.size() && text != "00" &&
                                  text != "01" && text != "1." && text != ".5" && text != "1e" &&
                                  text != "-" && text != "1.5e+";
        if (!reference_ok) {
            assert(!ok);
            continue;
        }
        assert(ok);
        if (number.kind != strata::util::NumberKind::Double)
            continue; // integral literals are integers here, by design
        if (converted.ec == std::errc::result_out_of_range) {
            // std::isinf rather than a literal 1.0 / 0.0: MSVC rejects the
            // constant division as an error (C2124), clang folds it.
            const bool infinite = std::isinf(number.double_value);
            const bool zero = number.double_value == 0.0;
            assert(infinite || zero);
            continue;
        }
        uint64_t got;
        uint64_t want;
        std::memcpy(&got, &number.double_value, sizeof(got));
        std::memcpy(&want, &expected, sizeof(want));
        if (got != want) {
            std::printf("number mismatch on %s: got %.17g want %.17g\n", raw, number.double_value,
                        expected);
            assert(false);
        }
    }
}

/// The parser's short-number head against the full scanner, bit for bit:
/// every integer and fraction width the head accepts, every sign, zeros in
/// every position, followed by each terminator the head must look past.
void test_short_number_head_matches_full_scanner() {
    struct Handler {
        int64_t last_int = 0;
        double last_double = 0.0;
        int kind = 0; // 1 int, 2 double, 3 other
        bool on_null() { return true; }
        bool on_bool(bool) { return true; }
        bool on_int(int64_t value) {
            last_int = value;
            kind = 1;
            return true;
        }
        bool on_big_int(std::string_view) {
            kind = 3;
            return true;
        }
        bool on_double(double value) {
            last_double = value;
            kind = 2;
            return true;
        }
        bool on_string(std::string_view) { return true; }
        bool on_key(std::string_view) { return true; }
        bool on_start_object() { return true; }
        bool on_end_object() { return true; }
        bool on_start_array() { return true; }
        bool on_end_array() { return true; }
    };
    uint64_t state = 0x1234567887654321ULL;
    const char* tails[] = {", 1", "]", "}", " ", "e5", "E-2", ".5", "x", ""};
    // Integer widths through 17: the head's first word covers 1..7 digits,
    // its second word 8..15, and 16 and 17 prove the hand-off to the scanner.
    // Fraction widths through 24: the head's fraction word covers 1..7, its
    // long-fraction step 8..19 (16..19 behind a lone zero only), and the
    // widths past each bound prove the hand-off.
    for (size_t int_len = 1; int_len <= 17; ++int_len) {
        for (size_t frac_len = 0; frac_len <= 24; ++frac_len) {
            for (int trial = 0; trial < 12; ++trial) {
                std::string text = digits_of(state, int_len, 0, false);
                if (trial % 4 == 1)
                    text = "0"; // lone zero integer part
                if (trial % 4 == 2 && int_len > 1)
                    text[0] = '0'; // leading zero: must be rejected
                if (frac_len > 0)
                    text += "." + digits_of(state, frac_len, trial % 3, trial % 5 == 0);
                for (const bool negative : {false, true}) {
                    for (const char* tail : tails) {
                        for (const size_t padding : {size_t{0}, size_t{9}, size_t{24}}) {
                            const std::string number = (negative ? "-" : "") + text;
                            // Room past the number: the head needs eight readable
                            // bytes from a run's first digit and the long-fraction
                            // step twenty-four from its first fraction digit, so
                            // without padding the short widths would never enter
                            // them and the comparison would be trivially true for
                            // them -- and with padding alone the near-end hand-off
                            // to the scanner would never run. Both parsers stop at
                            // the number; the padding is never consumed.
                            const std::string input = number + tail + std::string(padding, 'x');
                            // The head, through the real parser entry.
                            Handler handler;
                            strata::ParserInline<Handler> parser{input.data(), input.size(),
                                                                 handler};
                            const bool head_ok = parser.parse_number();
                            // The full scanner alone.
                            strata::util::ParsedNumber number_out;
                            const bool full_ok = strata::util::parse_number_unified(
                                input.data(), input.size(), number_out);
                            assert(head_ok == full_ok);
                            if (!full_ok)
                                continue;
                            assert(parser.i == number_out.consumed);
                            if (number_out.kind == strata::util::NumberKind::Int64) {
                                assert(handler.kind == 1 &&
                                       handler.last_int == number_out.int_value);
                            } else if (number_out.kind == strata::util::NumberKind::Double) {
                                uint64_t got;
                                uint64_t want;
                                std::memcpy(&got, &handler.last_double, sizeof(got));
                                std::memcpy(&want, &number_out.double_value, sizeof(want));
                                if (handler.kind != 2 || got != want) {
                                    std::printf("head/scanner mismatch on %s\n", input.c_str());
                                    assert(false);
                                }
                            } else {
                                assert(handler.kind == 3);
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace

// The dispatcher resolves one- to three-digit integers inline, ahead of
// parse_number; every such value in both signs, in every position an array
// puts it in, must parse exactly as the full scanner does -- and every
// malformed neighbour ("01", "1.", "1e", "1x", a lone "-") must still be
// rejected.
namespace {

struct IntCollector {
    std::vector<int64_t> ints;
    std::vector<double> doubles;
    bool on_null() { return true; }
    bool on_bool(bool) { return true; }
    bool on_int(int64_t value) {
        ints.push_back(value);
        return true;
    }
    bool on_big_int(std::string_view) { return true; }
    bool on_double(double value) {
        doubles.push_back(value);
        return true;
    }
    bool on_string(std::string_view) { return true; }
    bool on_key(std::string_view) { return true; }
    bool on_start_object() { return true; }
    bool on_end_object() { return true; }
    bool on_start_array() { return true; }
    bool on_end_array() { return true; }
};

} // namespace

// The long-fraction step's contract, pinned on the helper itself: what it
// declines (too little input, an exponent, more than nineteen significant
// digits) and that what it accepts is the scanner's double, bit for bit.
void test_long_fraction_step_contract() {
    using strata::util::detail::parse_long_fraction;
    const auto scanner_bits = [](const std::string& text) {
        strata::util::ParsedNumber number;
        assert(strata::util::parse_number_unified(text.data(), text.size(), number));
        assert(number.kind == strata::util::NumberKind::Double);
        uint64_t bits;
        std::memcpy(&bits, &number.double_value, sizeof(bits));
        return bits;
    };
    const auto step = [](const std::string& fraction, size_t available, uint32_t int_value,
                         unsigned int_digits, bool negative, double& out, size_t& digits) {
        std::string buffer = fraction + std::string(40, ']');
        uint64_t first_word;
        std::memcpy(&first_word, buffer.data(), 8);
        return parse_long_fraction(buffer.data(), available, first_word, int_value, int_digits,
                                   negative, out, digits);
    };
    double out = 0.0;
    size_t digits = 0;
    // Fewer than twenty-four readable bytes: declined whatever the digits.
    assert(!step("12345678", 23, 1, 1, false, out, digits));
    assert(step("12345678", 24, 1, 1, false, out, digits) && digits == 8);
    {
        uint64_t bits;
        std::memcpy(&bits, &out, sizeof(bits));
        assert(bits == scanner_bits("1.12345678"));
    }
    // An exponent behind the run: the scanner's.
    assert(!step("12345678e", 40, 1, 1, false, out, digits));
    assert(!step("1234567890123456E", 40, 0, 1, false, out, digits));
    // The significant-digit bound: nineteen in all, a lone zero counting none.
    assert(step("1234567890123456789", 40, 0, 1, true, out, digits) && digits == 19);
    {
        uint64_t bits;
        std::memcpy(&bits, &out, sizeof(bits));
        assert(bits == scanner_bits("-0.1234567890123456789"));
    }
    assert(!step("12345678901234567890", 40, 0, 1, false, out, digits));
    assert(step("123456789012345678", 40, 7, 1, false, out, digits) && digits == 18);
    assert(!step("1234567890123456789", 40, 7, 1, false, out, digits));
    assert(step("123456789012", 40, 1234567, 7, false, out, digits) && digits == 12);
    {
        uint64_t bits;
        std::memcpy(&bits, &out, sizeof(bits));
        assert(bits == scanner_bits("1234567.123456789012"));
    }
    assert(!step("1234567890123", 40, 1234567, 7, false, out, digits));
    // Twenty-three fraction digits: a third word ends the run, the bound declines.
    assert(!step("12345678901234567890123", 40, 0, 1, false, out, digits));
    // Zero mantissas keep the sign, as the scanner's do.
    assert(step("00000000", 40, 0, 1, true, out, digits) && std::signbit(out) && out == 0.0);
    // Hard doubles through the real entry: halfway and near-halfway
    // mantissas, trailing zeros, the largest and smallest normal magnitudes
    // the step can see -- each the scanner's bits, whichever path decided.
    for (const char* text :
         {"0.9007199254740993", "0.90071992547409930", "1.0000000000000002", "0.30000000000000004",
          "2.2250738585072014", "1.7976931348623157", "0.1000000000000000055", "0.000000001",
          "0.00000000000000001", "0.1234567890123456789", "1234567.123456789012",
          "-0.6390313938546974", "-9.9999999999999999", "0.99999999999999999", "5.0000000000000001",
          "0.50000000000000001", "0.49999999999999999", "0.12345678", "0.123456789012345"}) {
        struct Handler {
            double value = 0.0;
            int kind = 0;
            bool on_null() { return true; }
            bool on_bool(bool) { return true; }
            bool on_int(int64_t) {
                kind = 1;
                return true;
            }
            bool on_big_int(std::string_view) {
                kind = 3;
                return true;
            }
            bool on_double(double got) {
                value = got;
                kind = 2;
                return true;
            }
            bool on_string(std::string_view) { return true; }
            bool on_key(std::string_view) { return true; }
            bool on_start_object() { return true; }
            bool on_end_object() { return true; }
            bool on_start_array() { return true; }
            bool on_end_array() { return true; }
        };
        const std::string input = std::string(text) + "," + std::string(30, '0');
        Handler handler;
        strata::ParserInline<Handler> parser{input.data(), input.size(), handler};
        assert(parser.parse_number());
        assert(handler.kind == 2);
        uint64_t got;
        std::memcpy(&got, &handler.value, sizeof(got));
        assert(got == scanner_bits(text));
        assert(parser.i == std::strlen(text));
    }
}

void test_tiny_int_dispatch_matches_the_scanner() {
    // Minus zero is an integer zero (api.md), through the inline path too.
    for (const char* doc : {"-0", "[-0,0,0,0]", "{\"k\":-0}", "[-0]"}) {
        IntCollector got;
        assert(strata::parse_sax_inline(doc, got, true) == strata::Status::Ok);
        assert(!got.ints.empty() && got.ints[0] == 0 && got.doubles.empty());
    }
    for (int value = -999; value <= 999; ++value) {
        const std::string digits = std::to_string(value);
        // The inline path needs four readable bytes from the number's first
        // digit; forms with and without them, so both the inline path and
        // the full path see every value. Each form says how many copies of
        // the value it carries; anything else it holds is padding.
        const std::pair<std::string, size_t> forms[] = {
            {digits, 1},
            {digits + "    ", 1},
            {"[" + digits + "]", 1},
            {"[" + digits + "," + digits + "]", 2},
            {"[" + digits + "," + digits + ",0,0,0]", 2},
            {"[" + digits + " ]", 1},
            {"[" + digits + "    ]", 1},
            {"{\"k\":" + digits + "}", 1},
            {"{\"k\":" + digits + ",\"j\":" + digits + "}   ", 2},
            {"[" + digits + ",1.5]", 1},
        };
        for (const auto& [doc, copies] : forms) {
            IntCollector got;
            const strata::Status status = strata::parse_sax_inline(doc, got, true);
            assert(status == strata::Status::Ok);
            assert(got.ints.size() >= copies);
            for (size_t index = 0; index < copies; ++index)
                assert(got.ints[index] == value);
        }
        // With a fraction or an exponent the same digits become a double,
        // through the full path.
        IntCollector fraction;
        assert(strata::parse_sax_inline("[" + digits + ".25]", fraction, true) ==
               strata::Status::Ok);
        assert(fraction.ints.empty() && fraction.doubles.size() == 1);
        assert(fraction.doubles[0] == static_cast<double>(value) + (value < 0 ? -0.25 : 0.25));
        IntCollector exponent;
        assert(strata::parse_sax_inline("[" + digits + "e1]", exponent, true) ==
               strata::Status::Ok);
        assert(exponent.ints.empty() && exponent.doubles.size() == 1);
        assert(exponent.doubles[0] == static_cast<double>(value) * 10.0);
    }
    for (const char* bad :
         {"01",          "[01]",       "[1.]",        "[1e]",       "[1x]",       "[-]",
          "[--1]",       "[12",        "[007]",       "[1.e5]",     "[+1]",       "[01,0,0,0]",
          "[-01,0,0,0]", "[1x,0,0,0]", "[12x,0,0,0]", "[123x,0,0]", "[-12x,0,0]", "[12.,0,0]",
          "[-,0,0,0,0]", "[-a,0,0,0]", "[1e,0,0,0]"}) {
        IntCollector got;
        assert(strata::parse_sax_inline(bad, got, true) != strata::Status::Ok);
    }
    // Every byte as the terminator behind one, two and three digits of either
    // sign: the inline path's whole grammar is "any byte that is not a digit,
    // a point or an exponent ends the run", so the document's outcome must be
    // the scanner's -- the number it consumes, and then the array's own rule
    // for what may follow (blanks, then the comma).
    for (const int value : {5, 42, 123, -7, -42, -123, 0}) {
        const std::string digits = std::to_string(value);
        for (int byte = 0; byte < 256; ++byte) {
            const std::string doc = "[" + digits + static_cast<char>(byte) + ",0]";
            strata::util::ParsedNumber number;
            const bool scanned =
                strata::util::parse_number_unified(doc.data() + 1, doc.size() - 1, number);
            bool expected = scanned;
            if (scanned) {
                std::string_view rest(doc.data() + 1 + number.consumed,
                                      doc.size() - 1 - number.consumed);
                while (!rest.empty() && (rest.front() == ' ' || rest.front() == '\t' ||
                                         rest.front() == '\n' || rest.front() == '\r'))
                    rest.remove_prefix(1);
                expected = rest == ",0]";
            }
            IntCollector got;
            const bool ok = strata::parse_sax_inline(doc, got, true) == strata::Status::Ok;
            assert(ok == expected);
            if (ok) {
                assert(number.kind == strata::util::NumberKind::Int64);
                assert(got.ints.size() == 2 && got.ints[0] == number.int_value);
            }
        }
    }
}

int main() {
    test_tiny_int_dispatch_matches_the_scanner();
    test_long_fraction_step_contract();
    test_short_number_head_matches_full_scanner();
    test_word_run_matches_scalar_twin();
    test_integers_of_every_width_match_from_chars();
    test_fractions_and_leading_zeros_match_from_chars();
    std::printf("fast_parse tests passed\n");
    return 0;
}

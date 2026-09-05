#pragma once

/**
 * @file json_parser_inline.hpp
 * @brief Templated recursive-descent JSON parser with devirtualised SAX dispatch.
 *
 * ParserInline<Handler> is the engine's only parsing state machine. Because it
 * is a template over a *concrete* handler, the compiler inlines every callback
 * instead of paying an indirect call per token — which is the whole point of
 * the hybrid-SAX design: one pass builds the caller's representation directly,
 * with no intermediate tree (docs/architecture/SKILL.md).
 *
 * Entry point: parse_sax_inline<Handler>().
 *
 * **Recursion is the depth limit.** Nesting consumes C++ stack and nothing
 * caps it; that is a stated contributor invariant, and the stress suite
 * deliberately stops at depth 100.
 */

#include "strata/json/json_core.hpp"
#include "strata/util/fast_parse.hpp"
#include "strata/util/scan.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

namespace strata {

/**
 * Detects an optional handler capability: speculative key matching.
 *
 * `size_t try_match_key(const char* after_quote, size_t remaining)` lets a
 * handler predict the next object key from schema repetition. A non-zero
 * return means the handler recognised the raw bytes (up to and including the
 * closing quote), consumed them, and performed everything `on_key` would have
 * -- so the parser skips the escape scan and the key decode entirely. Zero
 * means no prediction; the parser takes the ordinary path and the handler
 * sees a normal `on_key`.
 *
 * Handlers without the method (the DOM builder, the NDJSON stream) compile to
 * exactly the code they compiled to before this hook existed.
 */
template <typename T, typename = void> struct has_try_match_key : std::false_type {};

template <typename T>
struct has_try_match_key<T, std::void_t<decltype(std::declval<T&>().try_match_key(
                                std::declval<const char*>(), std::declval<size_t>()))>>
    : std::true_type {};

/**
 * Recursive-descent SAX parser.
 *
 * @tparam Handler Concrete handler type providing the JsonSaxHandler
 *         interface. Passing a concrete type is what enables devirtualisation.
 */
template <typename Handler> struct ParserInline {
    const char* data;
    size_t len;
    Handler& handler;
    size_t i = 0;

    /// Reused buffer for strings that contain escapes; see the lifetime note
    /// in json_sax_handler.hpp.
    std::string scratch{};

    [[nodiscard]] bool eof() const noexcept { return i >= len; }
    [[nodiscard]] char peek() const noexcept { return eof() ? '\0' : data[i]; }
    char get() noexcept { return eof() ? '\0' : data[i++]; }

    void skip_ws() noexcept { i = util::skip_whitespace(data, len, i); }

    bool consume(char expected) noexcept {
        skip_ws();
        if (peek() == expected) {
            ++i;
            return true;
        }
        return false;
    }

    // --- Escapes -----------------------------------------------------------

    /// Value of a hex digit, or -1.
    [[nodiscard]] static int hex_value(char c) noexcept {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        return -1;
    }

    /// Read exactly four hex digits of a \\uXXXX escape.
    bool read_hex4(uint32_t& out) noexcept {
        if (i + 4 > len)
            return false;
        uint32_t value = 0;
        for (int digit_index = 0; digit_index < 4; ++digit_index) {
            const int digit = hex_value(data[i++]);
            if (digit < 0)
                return false;
            value = (value << 4) | static_cast<uint32_t>(digit);
        }
        out = value;
        return true;
    }

    /// Append @p codepoint to @p out as UTF-8; rejects surrogates and out-of-range.
    [[nodiscard]] static bool append_utf8(std::string& out, uint32_t codepoint) {
        if (codepoint > 0x10FFFF)
            return false;
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
            return false;
        if (codepoint <= 0x7F) {
            out.push_back(static_cast<char>(codepoint));
        } else if (codepoint <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
        return true;
    }

    // --- Values ------------------------------------------------------------

    bool parse_value() {
        skip_ws();
        if (eof())
            return false;
        switch (peek()) {
        case 'n':
            return parse_null();
        case 't':
        case 'f':
            return parse_bool();
        case '"':
            return parse_string();
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
        }
        const char c = peek();
        if (c == '-' || (c >= '0' && c <= '9')) {
            // Short integers -- up to three digits, either sign: ids, counts,
            // small values, the bulk of JSON numbers -- resolved here without
            // the out-of-line number path, whose word head costs a call, an
            // 8-byte load, a digit mask, a count and a multiply chain
            // (measured in the SAX core at 10-11 ns per such number against
            // 3.9 for a null; 6.3-7.0 here). Exit first: a number whose next
            // three bytes are all digits or points is long or a float and
            // takes parse_number on one predictable branch. The filter may
            // over-approximate -- handing a number to parse_number is always
            // right, keeping one is right only once it is proven short -- and
            // that is the invariant to keep when editing it. What remains
            // has one to three digits and a terminator; the lone-zero rule
            // holds ("01" is not taken here and is rejected by the full
            // path), and a point or an exponent after the digits still
            // takes the full path. The classes that take neither path pay
            // for the code in this function: 4-digit integers about 6%,
            // 6-decimal floats 4% and 17-digit floats 6% in the core (a
            // one-word SWAR range filter in place of the four byte loads
            // recovers none of it -- the cost is the branch, not the loads);
            // every repository dataset still gains, from 1% on flat to 12%
            // on wide_arrays.
            //
            // Every question below is one lookup in kNumberByteClass: "is
            // this byte a digit" and "may a number continue through it".
            // Written as compares, the terminator question
            // (`b != '.' && b != 'e' && b != 'E'`) is a three-value set the
            // compiler lowers to a shift mask -- nine arm64 instructions,
            // and it is on the path of every short integer. The table also
            // removes a test: a digit continues a number, so the width-3
            // arm no longer needs a separate digit check on its last byte.
            const size_t sign = static_cast<size_t>(c == '-');
            const size_t start = i + sign;
            if (start + 4 <= len) {
                const auto cls = [](char byte) noexcept {
                    return util::detail::number_class(byte);
                };
                const auto continues = [](unsigned char bits) noexcept {
                    return (bits & util::detail::kNumClassContinues) != 0;
                };
                const auto digit = [](unsigned char bits) noexcept {
                    return (bits & util::detail::kNumClassDigit) != 0;
                };
                const char b0 = data[start];
                const char b1 = data[start + 1];
                const char b2 = data[start + 2];
                const char b3 = data[start + 3];
                const unsigned char c1 = cls(b1);
                if (digit(cls(b0)) &&
                    !(continues(c1) && continues(cls(b2)) && continues(cls(b3)))) {
                    int64_t value = 0;
                    size_t width = 0;
                    if (!continues(c1)) {
                        value = b0 - '0';
                        width = 1;
                    } else if (digit(c1) && b0 != '0') {
                        const unsigned char c2 = cls(b2);
                        if (!continues(c2)) {
                            value = (b0 - '0') * 10 + (b1 - '0');
                            width = 2;
                        } else if (digit(c2) && !continues(cls(b3))) {
                            value = (b0 - '0') * 100 + (b1 - '0') * 10 + (b2 - '0');
                            width = 3;
                        }
                    }
                    if (width != 0) {
                        i = start + width;
                        return handler.on_int(sign != 0 ? -value : value);
                    }
                }
            }
            return parse_number();
        }
        return false;
    }

    bool parse_null() {
        if (i + 4 <= len && data[i + 1] == 'u' && data[i + 2] == 'l' && data[i + 3] == 'l') {
            i += 4;
            return handler.on_null();
        }
        return false;
    }

    bool parse_bool() {
        if (peek() == 't') {
            if (i + 4 <= len && data[i + 1] == 'r' && data[i + 2] == 'u' && data[i + 3] == 'e') {
                i += 4;
                return handler.on_bool(true);
            }
            return false;
        }
        if (i + 5 <= len && data[i + 1] == 'a' && data[i + 2] == 'l' && data[i + 3] == 's' &&
            data[i + 4] == 'e') {
            i += 5;
            return handler.on_bool(false);
        }
        return false;
    }

    /// Numbers are validated and classified in one pass by parse_number_unified.
    ///
    /// Kept out of line on purpose (STRATA_NOINLINE): inlined into
    /// parse_value, the head below made the dispatcher's prologue bigger for
    /// every null, bool and string too — measured +4–7% on those lists
    /// (docs/performance/SKILL.md, wave 12). Only numbers pay for it here.
    STRATA_NOINLINE bool parse_number() {
        // The short-number head: an optional sign, one to seven digits, and
        // optionally a point with one to seven more, with no exponent behind
        // — ids, counts, prices, coordinates: most of what JSON numbers are.
        // Two word loads resolve it without entering the full scanner, whose
        // prologue alone (six callee-saved register pairs, a ParsedNumber
        // round trip) cost more than these numbers' digits: per-number cost
        // read 6–13 ns over orjson before this head. The decimal case is the
        // full path's own Clinger step verbatim — an exact integer below
        // 10^14 divided by an exact power of ten is correctly rounded — so
        // the double is bit-identical to what parse_number_unified produces
        // (checked by the fast_parse suite). Eight readable bytes are
        // required past each run's start, the lone-zero rule is kept ("0"
        // and "0.5" are taken; "01" falls through to be rejected), and every
        // other shape takes the full path unchanged.
        const bool negative = data[i] == '-';
        const size_t digits = i + static_cast<size_t>(negative);
        if (digits + 8 <= len) {
            uint64_t chunk;
            std::memcpy(&chunk, data + digits, 8);
            const unsigned count = util::detail::leading_digit_count(chunk);
            if (count == 8 && data[digits] != '0' && digits + 16 <= len) {
                // Eight to fifteen digits -- ids and timestamps past the first
                // word -- from a second word: the leading count of its digits
                // extends the first word's value by a power of ten, and the
                // byte after them proves the run ends without a fraction or
                // an exponent (measured in the core: 9-15-digit integers
                // 19 -> 12 ns; the code it adds to this non-inlined head
                // taxes 5-7-digit integers about 4% and 16-digit ones 3-4%,
                // which neither reordering the branches nor likelihood hints
                // recover). Sixteen digits and up, and every exponent shape,
                // take the full path unchanged.
                uint64_t tail_chunk;
                std::memcpy(&tail_chunk, data + digits + 8, 8);
                const unsigned tail_count = util::detail::leading_digit_count(tail_chunk);
                if (tail_count < 8) {
                    const size_t after = digits + 8 + tail_count;
                    const char next = data[after];
                    if (util::detail::ends_number(next)) {
                        uint64_t value = util::detail::leading_digit_value(chunk, 8);
                        if (tail_count != 0) {
                            value = value * util::detail::kRunPow10[tail_count] +
                                    util::detail::leading_digit_value(tail_chunk, tail_count);
                        }
                        i = after;
                        return handler.on_int(negative ? -static_cast<int64_t>(value)
                                                       : static_cast<int64_t>(value));
                    }
                }
            }
            if (count != 0 && count < 8 && (data[digits] != '0' || count == 1)) {
                const size_t after = digits + count;
                const char next = data[after];
                // Written out rather than through ends_number(): here the
                // point test is *shared* with the fraction branch below, so
                // the compiler emits one compare and branches straight into
                // it, and the exponent compares only cost the integer case.
                // A table lookup hides that sharing and the fraction has to
                // re-test the byte -- measured on a Neoverse-N2 at +2.8% on
                // 4- and 7-digit integers and +1.3% on 17-digit floats,
                // against -3% on 1-3-digit integers elsewhere. The lookup
                // wins only where a whole compare chain disappears with it.
                if (next != '.' && next != 'e' && next != 'E') {
                    const auto value =
                        static_cast<int64_t>(util::detail::leading_digit_value(chunk, count));
                    i = after;
                    return handler.on_int(negative ? -value : value);
                }
                if (next == '.' && after + 9 <= len) {
                    uint64_t fraction_chunk;
                    std::memcpy(&fraction_chunk, data + after + 1, 8);
                    const unsigned fraction_count =
                        util::detail::leading_digit_count(fraction_chunk);
                    if (fraction_count != 0 && fraction_count < 8) {
                        const char tail = data[after + 1 + fraction_count];
                        if (tail != 'e' && tail != 'E') {
                            const uint64_t mantissa =
                                util::detail::leading_digit_value(chunk, count) *
                                    util::detail::kRunPow10[fraction_count] +
                                util::detail::leading_digit_value(fraction_chunk, fraction_count);
                            const double magnitude = static_cast<double>(mantissa) /
                                                     util::detail::kClingerPow10[fraction_count];
                            i = after + 1 + fraction_count;
                            return handler.on_double(negative ? -magnitude : magnitude);
                        }
                    } else if (fraction_count == 8) {
                        // Eight fraction digits and counting -- full-precision
                        // doubles -- resolve out of line from up to two more
                        // words (see parse_long_fraction); what it declines
                        // takes the full path unchanged.
                        double value;
                        size_t fraction_digits;
                        if (util::detail::parse_long_fraction(
                                data + after + 1, len - (after + 1), fraction_chunk,
                                util::detail::leading_digit_value(chunk, count), count, negative,
                                value, fraction_digits)) {
                            i = after + 1 + fraction_digits;
                            return handler.on_double(value);
                        }
                    }
                }
            }
        }
        util::ParsedNumber number;
        if (!util::parse_number_unified(data + i, len - i, number))
            return false;
        const size_t start = i;
        i += number.consumed;
        switch (number.kind) {
        case util::NumberKind::Int64:
            return handler.on_int(number.int_value);
        case util::NumberKind::BigInt:
            return handler.on_big_int(std::string_view(data + start, number.consumed));
        case util::NumberKind::Double:
            return handler.on_double(number.double_value);
        }
        return false;
    }

    /**
     * Parse a string, or an object key when @p is_key.
     *
     * Escape-free strings are handed to the handler as a view straight into
     * the input buffer — no copy. Anything containing an escape or a control
     * byte falls back to building the decoded text in @ref scratch.
     */
    bool parse_string(bool is_key = false) {
        if (get() != '"')
            return false;

        const size_t remaining = len - i;
        const size_t plain = util::find_next_escape(data + i, remaining);

        if (plain < remaining && data[i + plain] == '"') {
            const std::string_view view(data + i, plain);
            i += plain + 1;
            return is_key ? handler.on_key(view) : handler.on_string(view);
        }

        // Keep the clean prefix, then decode from the first interesting byte.
        scratch.assign(data + i, plain);
        i += plain;

        while (!eof()) {
            const char c = get();
            if (c == '"')
                return is_key ? handler.on_key(scratch) : handler.on_string(scratch);
            if (c != '\\') {
                if (static_cast<unsigned char>(c) < 0x20)
                    return false; // unescaped control character
                scratch.push_back(c);
                continue;
            }
            if (eof())
                return false;
            switch (get()) {
            case '"':
                scratch.push_back('"');
                break;
            case '\\':
                scratch.push_back('\\');
                break;
            case '/':
                scratch.push_back('/');
                break;
            case 'b':
                scratch.push_back('\b');
                break;
            case 'f':
                scratch.push_back('\f');
                break;
            case 'n':
                scratch.push_back('\n');
                break;
            case 'r':
                scratch.push_back('\r');
                break;
            case 't':
                scratch.push_back('\t');
                break;
            case 'u': {
                uint32_t codepoint = 0;
                if (!read_hex4(codepoint))
                    return false;
                if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                    // High surrogate: a low surrogate must follow, and the pair
                    // combines into one codepoint.
                    if (i + 1 >= len || data[i] != '\\' || data[i + 1] != 'u')
                        return false;
                    i += 2;
                    uint32_t low = 0;
                    if (!read_hex4(low))
                        return false;
                    if (low < 0xDC00 || low > 0xDFFF)
                        return false;
                    codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                }
                // Anything still in the surrogate range is a lone half, and
                // append_utf8 is the single place that rejects it.
                if (!append_utf8(scratch, codepoint))
                    return false;
                break;
            }
            default:
                return false; // unknown escape
            }
        }
        return false; // unterminated string
    }

    bool parse_array() {
        // Only reached through parse_value, which dispatched on this byte:
        // the opening bracket is consumed without re-scanning for it.
        ++i;
        if (!handler.on_start_array())
            return false;

        skip_ws();
        if (eof())
            return false;
        if (peek() == ']') {
            ++i;
            return handler.on_end_array();
        }

        for (;;) {
            if (!parse_value())
                return false;
            skip_ws();
            if (eof())
                return false;
            const char delimiter = get();
            if (delimiter == ']')
                return handler.on_end_array();
            if (delimiter != ',')
                return false;
        }
    }

    /// One memcmp against the handler's predicted raw key bytes; see
    /// has_try_match_key. `i` sits on the opening quote and, on a hit, lands
    /// just past the closing one.
    bool try_predicted_key() {
        if constexpr (has_try_match_key<Handler>::value) {
            const size_t consumed = handler.try_match_key(data + i + 1, len - i - 1);
            if (consumed > 0) {
                i += 1 + consumed;
                return true;
            }
        }
        return false;
    }

    bool parse_object() {
        ++i; // as in parse_array: parse_value dispatched on the brace
        if (!handler.on_start_object())
            return false;

        skip_ws();
        if (eof())
            return false;
        if (peek() == '}') {
            ++i;
            return handler.on_end_object();
        }

        for (;;) {
            skip_ws();
            if (peek() != '"')
                return false;
            if (!try_predicted_key() && !parse_string(true))
                return false;
            if (!consume(':'))
                return false;
            if (!parse_value())
                return false;
            skip_ws();
            if (eof())
                return false;
            const char delimiter = get();
            if (delimiter == '}')
                return handler.on_end_object();
            if (delimiter != ',')
                return false;
        }
    }
};

/**
 * Parse @p text, streaming events to @p handler.
 *
 * Instantiate with a concrete handler type to devirtualise the callbacks; the
 * abstract-handler wrapper lives in json_parse.hpp.
 *
 * @param validate_utf8 Check the whole input up front. Callers that create
 *        Python strings can pass false, because PyUnicode validates during
 *        creation — but note that for `bytes` input this parser is then the
 *        only validator there is (docs/context/api.md).
 * @return Status::Ok, or Status::ParseError for any malformed input, including
 *         trailing bytes after the top-level value.
 */
template <typename Handler>
[[nodiscard]] Status parse_sax_inline(std::string_view text, Handler& handler,
                                      bool validate_utf8 = true) {
    if (validate_utf8 && !util::validate_utf8(text.data(), text.size()))
        return Status::ParseError;

    ParserInline<Handler> parser{text.data(), text.size(), handler};
    if (!parser.parse_value())
        return Status::ParseError;
    parser.skip_ws();
    if (!parser.eof())
        return Status::ParseError;
    return Status::Ok;
}

} // namespace strata

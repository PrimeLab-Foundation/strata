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
#include <type_traits>

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
 * The placement token a produced value travels on: the no-capability default.
 *
 * It carries one thing — "no failure yet" — so a handler without the
 * value-cursor capability parses through exactly the boolean dispatcher it
 * always did (`parse_value(PlainCursor)` takes and returns one register).
 * @ref value_cursor_of selects it for every such handler.
 */
struct PlainCursor {
    bool live;
};

/**
 * Detects an optional handler capability: an array element cursor.
 *
 * A handler that stages array elements in its own buffer pays, per element, a
 * load of the buffer's end pointer, a capacity compare and two stores — all
 * *through the handler object*, so an opaque call (creating the element's
 * object) forces the reload every time. A handler declaring
 *
 * ```
 * struct ValueCursor { ... };            // trivially copyable, two words
 * ValueCursor open_values();             // the staging buffer's [next, limit)
 * void close_values(ValueCursor);        // publish `next` back to the handler
 * ValueCursor on_null_at(ValueCursor);   // ... one per scalar event
 * ```
 *
 * hands that pair to the parser instead, which keeps it in registers across
 * the array's whole element loop: the store becomes a store and a register
 * increment. See docs/architecture/value-cursor.md.
 *
 * The cursor is *only* ever live inside one array's element loop. Everything
 * that can reach the handler's staging buffer by any other route — a nested
 * array or object, the end of this array, any failure — is preceded by
 * `close_values`, so the handler's own view is authoritative at every point
 * where control can leave the parser. `on_*_at` returns the advanced cursor,
 * or a value-initialised one meaning "failed, and already closed".
 *
 * Handlers without the member type (the DOM builder, the NDJSON stream, the
 * streaming-search handler, any JsonSaxHandler subclass) select PlainCursor
 * and compile to exactly the code they compiled to before this hook existed.
 */
template <typename T, typename = void> struct value_cursor_of {
    using type = PlainCursor;
};

template <typename T> struct value_cursor_of<T, std::void_t<typename T::ValueCursor>> {
    using type = typename T::ValueCursor;
};

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

    /// The cursor an array's element loop runs on: the handler's own type
    /// when it has the capability, PlainCursor otherwise.
    using ArrayCursor = typename value_cursor_of<Handler>::type;

    /// True for the no-capability token, so the `if constexpr` branches below
    /// erase every cursor operation from a handler that has none.
    template <typename C> static constexpr bool is_plain = std::is_same_v<C, PlainCursor>;

    /// A value-initialised cursor is the failure token, on either kind.
    template <typename C> [[nodiscard]] static bool live(C cursor) noexcept {
        if constexpr (is_plain<C>)
            return cursor.live;
        else
            return cursor.next != nullptr;
    }

    /// Take the handler's staging range into registers for one element loop.
    [[nodiscard]] ArrayCursor open_values() {
        if constexpr (is_plain<ArrayCursor>)
            return ArrayCursor{true};
        else
            return handler.open_values();
    }

    /// Publish the cursor back to the handler: after this the handler's own
    /// view of its staging buffer is authoritative again.
    void close_values(ArrayCursor cursor) {
        if constexpr (!is_plain<ArrayCursor>)
            handler.close_values(cursor);
    }

    /// Give up on this value: close the cursor first, so every element already
    /// stored through it is accounted for by the handler before the parse
    /// unwinds, then return the failure token.
    template <typename C> [[nodiscard]] C abandon(C cursor) {
        if constexpr (!is_plain<C>)
            handler.close_values(cursor);
        return C{};
    }

    // One emitter per scalar event. The handler's `on_*_at` returns the
    // advanced cursor (or the failure token, having closed it itself); the
    // plain form wraps the handler's boolean.
    template <typename C> [[nodiscard]] C emit_null(C cursor) {
        if constexpr (is_plain<C>)
            return C{handler.on_null()};
        else
            return handler.on_null_at(cursor);
    }

    template <typename C> [[nodiscard]] C emit_bool(C cursor, bool value) {
        if constexpr (is_plain<C>)
            return C{handler.on_bool(value)};
        else
            return handler.on_bool_at(cursor, value);
    }

    template <typename C> [[nodiscard]] C emit_int(C cursor, int64_t value) {
        if constexpr (is_plain<C>)
            return C{handler.on_int(value)};
        else
            return handler.on_int_at(cursor, value);
    }

    template <typename C> [[nodiscard]] C emit_big_int(C cursor, std::string_view text) {
        if constexpr (is_plain<C>)
            return C{handler.on_big_int(text)};
        else
            return handler.on_big_int_at(cursor, text);
    }

    template <typename C> [[nodiscard]] C emit_double(C cursor, double value) {
        if constexpr (is_plain<C>)
            return C{handler.on_double(value)};
        else
            return handler.on_double_at(cursor, value);
    }

    template <typename C> [[nodiscard]] C emit_string(C cursor, std::string_view value) {
        if constexpr (is_plain<C>)
            return C{handler.on_string(value)};
        else
            return handler.on_string_at(cursor, value);
    }

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

    /// The dispatcher, in the context its value belongs to: @p cursor is an
    /// array's element cursor, or PlainCursor for an object member, the root,
    /// and every handler without the capability. Returns the advanced cursor,
    /// or the failure token — which always implies the cursor was closed.
    template <typename C> [[nodiscard]] C parse_value(C cursor) {
        skip_ws();
        if (eof())
            return abandon(cursor);
        switch (peek()) {
        case 'n':
            return parse_null(cursor);
        case 't':
        case 'f':
            return parse_bool(cursor);
        case '"':
            return parse_string_value(cursor);
        case '[':
            return parse_array_value(cursor);
        case '{':
            return parse_object_value(cursor);
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
            const size_t sign = static_cast<size_t>(c == '-');
            const size_t start = i + sign;
            if (start + 4 <= len) {
                const auto digit_or_point = [](char byte) noexcept {
                    return (byte >= '0' && byte <= '9') || byte == '.';
                };
                const auto digit = [](char byte) noexcept { return byte >= '0' && byte <= '9'; };
                const char b0 = data[start];
                const char b1 = data[start + 1];
                const char b2 = data[start + 2];
                const char b3 = data[start + 3];
                if (digit(b0) &&
                    !(digit_or_point(b1) && digit_or_point(b2) && digit_or_point(b3))) {
                    const auto ends = [](char byte) noexcept {
                        return byte != '.' && byte != 'e' && byte != 'E';
                    };
                    int64_t value = 0;
                    size_t width = 0;
                    if (!digit(b1)) {
                        if (ends(b1)) {
                            value = b0 - '0';
                            width = 1;
                        }
                    } else if (b0 != '0') {
                        if (!digit(b2)) {
                            if (ends(b2)) {
                                value = (b0 - '0') * 10 + (b1 - '0');
                                width = 2;
                            }
                        } else if (!digit(b3) && ends(b3)) {
                            value = (b0 - '0') * 100 + (b1 - '0') * 10 + (b2 - '0');
                            width = 3;
                        }
                    }
                    if (width != 0) {
                        i = start + width;
                        return emit_int(cursor, sign != 0 ? -value : value);
                    }
                }
            }
            return parse_number(cursor);
        }
        return abandon(cursor);
    }

    /// The root and every object member parse in the plain context; kept as an
    /// overload so callers that never hold a cursor read as they always did.
    bool parse_value() { return parse_value(PlainCursor{true}).live; }

    template <typename C> [[nodiscard]] C parse_null(C cursor) {
        if (i + 4 <= len && data[i + 1] == 'u' && data[i + 2] == 'l' && data[i + 3] == 'l') {
            i += 4;
            return emit_null(cursor);
        }
        return abandon(cursor);
    }

    template <typename C> [[nodiscard]] C parse_bool(C cursor) {
        if (peek() == 't') {
            if (i + 4 <= len && data[i + 1] == 'r' && data[i + 2] == 'u' && data[i + 3] == 'e') {
                i += 4;
                return emit_bool(cursor, true);
            }
            return abandon(cursor);
        }
        if (i + 5 <= len && data[i + 1] == 'a' && data[i + 2] == 'l' && data[i + 3] == 's' &&
            data[i + 4] == 'e') {
            i += 5;
            return emit_bool(cursor, false);
        }
        return abandon(cursor);
    }

    /// A nested container empties the cursor first: everything inside reaches
    /// the handler's staging buffer by its own route, so the handler's view
    /// must be current before the recursion and is re-read after it.
    template <typename C> [[nodiscard]] C parse_array_value(C cursor) {
        // The plain context keeps the tail call the dispatcher always had.
        if constexpr (is_plain<C>)
            return C{parse_array()};
        else {
            handler.close_values(cursor);
            if (!parse_array())
                return C{};
            return handler.open_values();
        }
    }

    template <typename C> [[nodiscard]] C parse_object_value(C cursor) {
        // The plain context keeps the tail call the dispatcher always had.
        if constexpr (is_plain<C>)
            return C{parse_object()};
        else {
            handler.close_values(cursor);
            if (!parse_object())
                return C{};
            return handler.open_values();
        }
    }

    template <typename C> [[nodiscard]] C parse_string_value(C cursor) {
        const std::string_view text = scan_string();
        if (text.data() == nullptr)
            return abandon(cursor);
        return emit_string(cursor, text);
    }

    /// Numbers are validated and classified in one pass by parse_number_unified.
    ///
    /// Kept out of line on purpose (STRATA_NOINLINE): inlined into
    /// parse_value, the head below made the dispatcher's prologue bigger for
    /// every null, bool and string too — measured +4–7% on those lists
    /// (docs/performance/SKILL.md, wave 12). Only numbers pay for it here.
    template <typename C> STRATA_NOINLINE C parse_number(C cursor) {
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
                    if (next != '.' && next != 'e' && next != 'E') {
                        uint64_t value = util::detail::leading_digit_value(chunk, 8);
                        if (tail_count != 0) {
                            value = value * util::detail::kRunPow10[tail_count] +
                                    util::detail::leading_digit_value(tail_chunk, tail_count);
                        }
                        i = after;
                        return emit_int(cursor, negative ? -static_cast<int64_t>(value)
                                                         : static_cast<int64_t>(value));
                    }
                }
            }
            if (count != 0 && count < 8 && (data[digits] != '0' || count == 1)) {
                const size_t after = digits + count;
                const char next = data[after];
                if (next != '.' && next != 'e' && next != 'E') {
                    const auto value =
                        static_cast<int64_t>(util::detail::leading_digit_value(chunk, count));
                    i = after;
                    return emit_int(cursor, negative ? -value : value);
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
                            return emit_double(cursor, negative ? -magnitude : magnitude);
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
                            return emit_double(cursor, value);
                        }
                    }
                }
            }
        }
        util::ParsedNumber number;
        if (!util::parse_number_unified(data + i, len - i, number))
            return abandon(cursor);
        const size_t start = i;
        i += number.consumed;
        switch (number.kind) {
        case util::NumberKind::Int64:
            return emit_int(cursor, number.int_value);
        case util::NumberKind::BigInt:
            return emit_big_int(cursor, std::string_view(data + start, number.consumed));
        case util::NumberKind::Double:
            return emit_double(cursor, number.double_value);
        }
        return abandon(cursor);
    }

    /// The plain-context overload, and the entry the number suites drive.
    bool parse_number() { return parse_number(PlainCursor{true}).live; }

    /**
     * Scan one string literal and return its text.
     *
     * Escape-free strings come back as a view straight into the input buffer —
     * no copy. Anything containing an escape or a control byte is decoded into
     * @ref scratch and viewed from there. The one definition of string
     * scanning: keys and values both read it, so there is no second place
     * where a string could be accepted differently.
     *
     * Failure is a **null-data view**, which no accepted string can be: an
     * input view points into the caller's buffer and `scratch.data()` is
     * never null. Returning the view rather than filling an out-parameter is
     * what keeps it in the pair of registers a two-word POD is returned in,
     * so the plain dispatcher needs no stack slot for it.
     */
    [[nodiscard]] std::string_view scan_string() {
        if (get() != '"')
            return std::string_view();

        const size_t remaining = len - i;
        const size_t plain = util::find_next_escape(data + i, remaining);

        if (plain < remaining && data[i + plain] == '"') {
            const std::string_view view(data + i, plain);
            i += plain + 1;
            return view;
        }

        // Keep the clean prefix, then decode from the first interesting byte.
        scratch.assign(data + i, plain);
        i += plain;

        while (!eof()) {
            const char c = get();
            if (c == '"')
                return scratch;
            if (c != '\\') {
                if (static_cast<unsigned char>(c) < 0x20)
                    return std::string_view(); // unescaped control character
                scratch.push_back(c);
                continue;
            }
            if (eof())
                return std::string_view();
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
                    return std::string_view();
                if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                    // High surrogate: a low surrogate must follow, and the pair
                    // combines into one codepoint.
                    if (i + 1 >= len || data[i] != '\\' || data[i + 1] != 'u')
                        return std::string_view();
                    i += 2;
                    uint32_t low = 0;
                    if (!read_hex4(low))
                        return std::string_view();
                    if (low < 0xDC00 || low > 0xDFFF)
                        return std::string_view();
                    codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                }
                // Anything still in the surrogate range is a lone half, and
                // append_utf8 is the single place that rejects it.
                if (!append_utf8(scratch, codepoint))
                    return std::string_view();
                break;
            }
            default:
                return std::string_view(); // unknown escape
            }
        }
        return std::string_view(); // unterminated string
    }

    /// An object key: scanned exactly as a value is, then handed to on_key.
    bool parse_key() {
        const std::string_view text = scan_string();
        return text.data() != nullptr && handler.on_key(text);
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

        // The element loop runs on a cursor: for a handler that stages array
        // elements, `next` and `limit` live in registers for the whole loop
        // instead of round-tripping through the handler on every element
        // (docs/architecture/value-cursor.md). PlainCursor erases all of it.
        ArrayCursor cursor = open_values();
        bool ok = true;
        for (;;) {
            cursor = parse_value(cursor);
            if (!live(cursor))
                return false; // parse_value closed it on the way out
            skip_ws();
            if (eof()) {
                ok = false;
                break;
            }
            const char delimiter = get();
            if (delimiter == ']')
                break;
            if (delimiter != ',') {
                ok = false;
                break;
            }
        }
        // The single commit point of a successful element loop: after it the
        // handler's own view of its staging buffer is authoritative again,
        // which is what on_end_array reads.
        close_values(cursor);
        return ok && handler.on_end_array();
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
            if (!try_predicted_key() && !parse_key())
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

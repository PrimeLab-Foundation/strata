#pragma once

/**
 * @file json_parser_inline.hpp
 * @brief Templated inline JSON parser for devirtualised SAX dispatch.
 *
 * ParserInline<Handler> is the core parsing state machine.  When
 * instantiated with a **concrete** handler type (rather than the
 * abstract JsonSaxHandler), the compiler can devirtualise and inline
 * every handler callback, eliminating ~10-30 indirect calls per JSON
 * object on the hot path.
 *
 * Entry point: parse_sax_inline<Handler>().
 *
 * Supports: null, true, false, numbers (int/double with fast path),
 * strings (SIMD escape detection, UTF-16 surrogate pairs),
 * arrays, and objects.
 */

#include "strata/util/fast_parse.hpp"
#include "strata/util/simd_string.hpp"

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>

namespace strata {

/**
 * Templated SAX parser.
 *
 * @tparam Handler  Concrete SAX handler type.  Must provide the same
 *                  interface as JsonSaxHandler (on_null, on_bool, …).
 *                  Using a concrete type enables devirtualisation.
 */
template <typename Handler> struct ParserInline {
    const char* data;
    size_t len;
    Handler& handler;
    size_t i = 0;

    [[nodiscard]] bool eof() const noexcept { return i >= len; }
    [[nodiscard]] char peek() const noexcept { return eof() ? '\0' : data[i]; }
    char get() noexcept { return eof() ? '\0' : data[i++]; }

    /// Convert a hex character to its integer value (0-15), or -1.
    [[nodiscard]] static int hex_value(char c) noexcept {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        return -1;
    }

    /// Read exactly 4 hex digits for a \uXXXX escape sequence.
    bool read_hex4(uint32_t& out) {
        if (i + 4 > len)
            return false;
        uint32_t value = 0;
        for (int idx = 0; idx < 4; ++idx) {
            int digit = hex_value(get());
            if (digit < 0)
                return false;
            value = (value << 4) | static_cast<uint32_t>(digit);
        }
        out = value;
        return true;
    }

    /// Encode a Unicode codepoint as UTF-8 and append to @p out.
    [[nodiscard]] static bool append_utf8(std::string& out, uint32_t codepoint) noexcept {
        if (codepoint > 0x10FFFF)
            return false;
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
            return false;
        if (codepoint <= 0x7F) {
            out.push_back(static_cast<char>(codepoint));
            return true;
        }
        if (codepoint <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            return true;
        }
        if (codepoint <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            return true;
        }
        out.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        return true;
    }

    void skip_ws() noexcept { i = util::skip_whitespace_fast(data, len, i); }

    bool consume(char c) noexcept {
        skip_ws();
        if (peek() == c) {
            ++i;
            return true;
        }
        return false;
    }

    // --- Value dispatch ----------------------------------------------------

    bool parse_value() {
        skip_ws();
        if (eof())
            return false;
        char c = peek();
        if (c == 'n')
            return parse_null();
        if (c == 't' || c == 'f')
            return parse_bool();
        if (c == '"')
            return parse_string();
        if (c == '[')
            return parse_array();
        if (c == '{')
            return parse_object();
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c)))
            return parse_number();
        return false;
    }

    bool parse_null() {
        if (i + 4 <= len && data[i] == 'n' && data[i + 1] == 'u' && data[i + 2] == 'l' &&
            data[i + 3] == 'l') {
            i += 4;
            return handler.on_null();
        }
        return false;
    }

    bool parse_bool() {
        if (i + 4 <= len && data[i] == 't' && data[i + 1] == 'r' && data[i + 2] == 'u' &&
            data[i + 3] == 'e') {
            i += 4;
            return handler.on_bool(true);
        }
        if (i + 5 <= len && data[i] == 'f' && data[i + 1] == 'a' && data[i + 2] == 'l' &&
            data[i + 3] == 's' && data[i + 4] == 'e') {
            i += 5;
            return handler.on_bool(false);
        }
        return false;
    }

    /**
     * Parse a JSON number.
     *
     * Tries the integer fast path first (parse_int_fast); falls through
     * to double parsing only when a decimal point or exponent is found.
     */
    bool parse_number() {
        size_t start = i;

        int64_t int_val;
        size_t consumed;
        if (util::parse_int_fast(data + i, len - i, int_val, consumed)) {
            if (i + consumed < len && (data[i + consumed] == '.' || data[i + consumed] == 'e' ||
                                       data[i + consumed] == 'E')) {
                // Fall through to double parsing
            } else {
                i += consumed;
                return handler.on_int(int_val);
            }
        }

        double double_val;
        if (util::parse_double_fast(data + start, len - start, double_val, consumed)) {
            i = start + consumed;
            return handler.on_double(double_val);
        }

        return false;
    }

    /**
     * Parse a JSON string (or object key when @p is_key is true).
     *
     * Fast path: uses SIMD to scan for the first escape/quote character.
     * If the closing quote is found without escapes the string_view is
     * passed directly to the handler (zero-copy).  Otherwise a std::string
     * is built by processing escape sequences one at a time.
     */
    bool parse_string(bool is_key = false) {
        if (get() != '"')
            return false;

        // SIMD fast scan: find first escape character or closing quote.
        size_t scan_pos = util::find_next_escape_simd(data + i, len - i);

        if (scan_pos < len - i && data[i + scan_pos] == '"') {
            // No escapes — zero-copy path.
            std::string_view result(data + i, scan_pos);
            i += scan_pos + 1;
            return is_key ? handler.on_key(result) : handler.on_string(result);
        }

        // Slow path: build string with escape handling.
        std::string out;
        out.reserve(scan_pos + 16);

        while (!eof()) {
            char c = get();
            if (c == '"') {
                return is_key ? handler.on_key(out) : handler.on_string(out);
            }
            if (c == '\\') {
                if (eof())
                    return false;
                char esc = get();
                switch (esc) {
                case '"':
                    out.push_back('"');
                    break;
                case '\\':
                    out.push_back('\\');
                    break;
                case '/':
                    out.push_back('/');
                    break;
                case 'b':
                    out.push_back('\b');
                    break;
                case 'f':
                    out.push_back('\f');
                    break;
                case 'n':
                    out.push_back('\n');
                    break;
                case 'r':
                    out.push_back('\r');
                    break;
                case 't':
                    out.push_back('\t');
                    break;
                case 'u': {
                    uint32_t codepoint = 0;
                    if (!read_hex4(codepoint))
                        return false;
                    // Handle UTF-16 surrogate pairs (\uD800-\uDBFF + \uDC00-\uDFFF).
                    if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                        if (eof() || get() != '\\')
                            return false;
                        if (eof() || get() != 'u')
                            return false;
                        uint32_t low = 0;
                        if (!read_hex4(low))
                            return false;
                        if (low < 0xDC00 || low > 0xDFFF)
                            return false;
                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                    } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                        return false; // Lone low surrogate
                    }
                    if (!append_utf8(out, codepoint))
                        return false;
                    break;
                }
                default:
                    return false;
                }
            } else {
                if (static_cast<unsigned char>(c) < 0x20) {
                    return false; // Unescaped control character
                }
                out.push_back(c);
            }
        }
        return false; // Unterminated string
    }

    bool parse_array() {
        if (!consume('['))
            return false;
        if (!handler.on_start_array(0))
            return false;
        if (consume(']'))
            return handler.on_end_array();
        while (true) {
            if (!parse_value())
                return false;
            if (consume(']'))
                break;
            if (!consume(','))
                return false;
        }
        return handler.on_end_array();
    }

    bool parse_object() {
        if (!consume('{'))
            return false;
        if (!handler.on_start_object(0))
            return false;
        if (consume('}'))
            return handler.on_end_object();
        while (true) {
            skip_ws();
            if (!parse_string(true))
                return false;
            if (!consume(':'))
                return false;
            if (!parse_value())
                return false;
            if (consume('}'))
                break;
            if (!consume(','))
                return false;
        }
        return handler.on_end_object();
    }
};

/**
 * Templated SAX parse entry point.
 *
 * Instantiate with a concrete handler type to enable devirtualisation
 * and inlining of every callback.  For virtual dispatch, call
 * parse_sax() in json_parse.hpp instead.
 *
 * @tparam Handler  Concrete handler type (must provide JsonSaxHandler interface).
 * @param text          UTF-8 JSON input.
 * @param handler       SAX event sink.
 * @param validate_utf8 Run SIMD UTF-8 validation before parsing (default: true).
 * @return Status::Ok on success, Status::ParseError on failure.
 */
template <typename Handler>
[[nodiscard]] Status parse_sax_inline(std::string_view text, Handler& handler,
                                      bool validate_utf8 = true) {
    if (validate_utf8 && !text.empty() && !util::validate_utf8_simd(text.data(), text.size())) {
        return Status::ParseError;
    }
    ParserInline<Handler> p{text.data(), text.size(), handler, 0};
    if (!p.parse_value())
        return Status::ParseError;
    p.skip_ws();
    if (!p.eof())
        return Status::ParseError;
    return Status::Ok;
}

} // namespace strata

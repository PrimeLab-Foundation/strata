#pragma once

/**
 * @file json_parser_indexed.hpp
 * @brief Structural-index-driven SAX parser for devirtualised dispatch.
 *
 * ParserIndexed<Handler> navigates JSON using a pre-computed structural
 * index (array of byte offsets for { } [ ] : ,) produced by the SIMD
 * IndexBuilder. This eliminates all whitespace scanning and delimiter
 * searching from the hot path — the parser jumps directly between
 * structural positions.
 *
 * For documents < 64 bytes, falls back to ParserInline<Handler> where
 * the SIMD indexing overhead would exceed the navigation savings.
 *
 * Entry point: parse_sax_indexed<Handler>().
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parser_inline.hpp"
#include "strata/simd/index_builder.h"
#include "strata/util/fast_parse.hpp"
#include "strata/util/simd_string.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

namespace strata {

/**
 * Structural-index-driven SAX parser.
 *
 * @tparam Handler  Concrete SAX handler type (same interface as ParserInline).
 */
template <typename Handler> struct ParserIndexed {
    const char* data;
    size_t len;
    Handler& handler;
    const uint32_t* sp; // structural positions array
    size_t num_sp;      // number of structural positions
    size_t si;          // current index into sp[]

    // ── Helpers ──────────────────────────────────────────────────────────

    /// Peek at the structural character at current index.
    [[nodiscard]] char sp_char() const noexcept { return (si < num_sp) ? data[sp[si]] : '\0'; }

    /// Get the byte offset of the current structural position.
    [[nodiscard]] size_t sp_pos() const noexcept { return (si < num_sp) ? sp[si] : len; }

    /// Find the start of content (skip whitespace) after a given byte offset.
    [[nodiscard]] size_t skip_ws_from(size_t pos) const noexcept {
        while (pos < len) {
            char c = data[pos];
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
                break;
            ++pos;
        }
        return pos;
    }

    /// Find the end of content (skip trailing whitespace) before a given byte offset.
    [[nodiscard]] size_t rskip_ws_to(size_t pos) const noexcept {
        while (pos > 0) {
            char c = data[pos - 1];
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
                break;
            --pos;
        }
        return pos;
    }

    // ── String parsing ──────────────────────────────────────────────────

    /// Parse a JSON string starting at position `start` (which should be '"').
    /// Calls handler.on_string() or handler.on_key() depending on `is_key`.
    /// Returns false on parse error.
    bool parse_string_between(size_t start, bool is_key) {
        if (start >= len || data[start] != '"')
            return false;

        size_t pos = start + 1; // skip opening quote

        // SIMD fast scan for escape/quote.
        size_t scan_len = len - pos;
        size_t scan_pos = util::find_next_escape_simd(data + pos, scan_len);

        if (scan_pos < scan_len && data[pos + scan_pos] == '"') {
            // No escapes — zero-copy path.
            std::string_view result(data + pos, scan_pos);
            return is_key ? handler.on_key(result) : handler.on_string(result);
        }

        // Slow path: build string with escape handling.
        std::string out;
        out.reserve(scan_pos + 16);
        if (scan_pos > 0) {
            out.append(data + pos, scan_pos);
            pos += scan_pos;
        }

        while (pos < len) {
            char c = data[pos++];
            if (c == '"') {
                return is_key ? handler.on_key(out) : handler.on_string(out);
            }
            if (c == '\\') {
                if (pos >= len)
                    return false;
                char esc = data[pos++];
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
                    // Parse \uXXXX
                    if (pos + 4 > len)
                        return false;
                    uint32_t cp = 0;
                    for (int j = 0; j < 4; ++j) {
                        char h = data[pos++];
                        cp <<= 4;
                        if (h >= '0' && h <= '9')
                            cp |= (h - '0');
                        else if (h >= 'a' && h <= 'f')
                            cp |= (h - 'a' + 10);
                        else if (h >= 'A' && h <= 'F')
                            cp |= (h - 'A' + 10);
                        else
                            return false;
                    }
                    // Surrogate pair handling
                    if (cp >= 0xD800 && cp <= 0xDBFF) {
                        if (pos + 6 > len || data[pos] != '\\' || data[pos + 1] != 'u')
                            return false;
                        pos += 2;
                        uint32_t lo = 0;
                        for (int j = 0; j < 4; ++j) {
                            char h = data[pos++];
                            lo <<= 4;
                            if (h >= '0' && h <= '9')
                                lo |= (h - '0');
                            else if (h >= 'a' && h <= 'f')
                                lo |= (h - 'a' + 10);
                            else if (h >= 'A' && h <= 'F')
                                lo |= (h - 'A' + 10);
                            else
                                return false;
                        }
                        if (lo < 0xDC00 || lo > 0xDFFF)
                            return false;
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                    }
                    // Encode as UTF-8
                    if (cp <= 0x7F) {
                        out.push_back(static_cast<char>(cp));
                    } else if (cp <= 0x7FF) {
                        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
                        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
                    } else if (cp <= 0xFFFF) {
                        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
                        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
                    } else {
                        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
                        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
                        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
                    }
                    break;
                }
                default:
                    return false;
                }
            } else {
                if (static_cast<unsigned char>(c) < 0x20)
                    return false;
                out.push_back(c);
            }
        }
        return false; // unterminated
    }

    /// Try speculative key match (same interface as ParserInline).
    int try_predicted_key(size_t pos) {
        if constexpr (has_try_match_key<Handler>::value) {
            size_t consumed = handler.try_match_key(data + pos, len - pos);
            if (consumed > 0) {
                // Key matched. Check if colon follows immediately.
                size_t after_key = pos + consumed;
                if (after_key < len && data[after_key] == ':') {
                    return 2; // key + colon consumed
                }
                // Whitespace before colon
                size_t j = after_key;
                while (j < len &&
                       (data[j] == ' ' || data[j] == '\t' || data[j] == '\n' || data[j] == '\r'))
                    ++j;
                if (j < len && data[j] == ':')
                    return 2;
                return 1; // key only
            }
        }
        return 0;
    }

    // ── Value dispatch ──────────────────────────────────────────────────

    /// Parse a JSON value. The next structural position tells us what to expect.
    bool parse_value() {
        if (si >= num_sp) {
            // No more structural chars — might be a trailing primitive.
            // Find start of content.
            size_t pos = (si > 0) ? sp[si - 1] + 1 : 0;
            pos = skip_ws_from(pos);
            return parse_primitive_at(pos);
        }

        char c = sp_char();
        if (c == '{')
            return parse_object();
        if (c == '[')
            return parse_array();

        // Primitive value: content is between previous structural and current.
        size_t val_start;
        if (si > 0) {
            val_start = skip_ws_from(sp[si - 1] + 1);
        } else {
            val_start = skip_ws_from(0);
        }
        // Only parse primitive if it comes before the current structural char.
        if (val_start < sp_pos()) {
            return parse_primitive_at(val_start);
        }
        // Current structural IS the start — must be { or [ (already handled).
        return false;
    }

    /// Parse a value knowing it starts at `pos` in the raw data.
    /// Called for values after ':' in objects or elements in arrays.
    bool parse_value_at(size_t pos) {
        if (pos >= len)
            return false;
        char c = data[pos];
        if (c == '{' || c == '[') {
            // Container — si should point to this position.
            // Sync si to this container opening.
            while (si < num_sp && sp[si] < pos)
                ++si;
            if (c == '{')
                return parse_object();
            return parse_array();
        }
        return parse_primitive_at(pos);
    }

    /// Parse a primitive (string, number, bool, null) starting at `pos`.
    bool parse_primitive_at(size_t pos) {
        if (pos >= len)
            return false;
        char c = data[pos];

        if (c == '"') {
            return parse_string_between(pos, false);
        }
        if ((c >= '0' && c <= '9') || c == '-') {
            // Number: find end (next structural position or whitespace).
            size_t end = (si < num_sp) ? sp[si] : len;
            size_t num_len = end - pos;
            // Trim trailing whitespace from number region.
            while (num_len > 0 &&
                   (data[pos + num_len - 1] == ' ' || data[pos + num_len - 1] == '\t' ||
                    data[pos + num_len - 1] == '\n' || data[pos + num_len - 1] == '\r')) {
                --num_len;
            }

            int64_t int_val;
            double dbl_val;
            size_t consumed;
            bool is_dbl;
            if (util::parse_number_unified(data + pos, num_len, int_val, dbl_val, consumed,
                                           is_dbl)) {
                return is_dbl ? handler.on_double(dbl_val) : handler.on_int(int_val);
            }
            return false;
        }
        if (c == 't') {
            if (pos + 4 <= len && data[pos + 1] == 'r' && data[pos + 2] == 'u' &&
                data[pos + 3] == 'e')
                return handler.on_bool(true);
            return false;
        }
        if (c == 'f') {
            if (pos + 5 <= len && data[pos + 1] == 'a' && data[pos + 2] == 'l' &&
                data[pos + 3] == 's' && data[pos + 4] == 'e')
                return handler.on_bool(false);
            return false;
        }
        if (c == 'n') {
            if (pos + 4 <= len && data[pos + 1] == 'u' && data[pos + 2] == 'l' &&
                data[pos + 3] == 'l')
                return handler.on_null();
            return false;
        }
        return false;
    }

    // ── Object parsing ──────────────────────────────────────────────────

    bool parse_object() {
        if (si >= num_sp || data[sp[si]] != '{')
            return false;

        size_t open_brace = sp[si];
        ++si; // past '{'

        if (!handler.on_start_object(0))
            return false;

        // Check for empty object: next structural is '}'
        if (si < num_sp && data[sp[si]] == '}') {
            ++si; // past '}'
            return handler.on_end_object();
        }

        for (;;) {
            // Key: find the quote between previous structural and current ':'
            size_t key_region_start = (si > 0) ? sp[si - 1] + 1 : open_brace + 1;
            size_t key_start = skip_ws_from(key_region_start);

            if (key_start >= len || data[key_start] != '"')
                return false;

            // Try speculative key prediction first.
            int key_result = try_predicted_key(key_start + 1);
            if (key_result == 0) {
                // Prediction missed — full parse.
                if (!parse_string_between(key_start, true))
                    return false;
            }
            // key_result >= 1 means key was handled by try_predicted_key.

            // Current structural should be ':' — advance past it.
            if (si >= num_sp || data[sp[si]] != ':')
                return false;
            size_t colon_pos = sp[si];
            ++si; // past ':'

            // Value: starts after colon, ends at next ',' or '}'.
            size_t val_start = skip_ws_from(colon_pos + 1);
            if (!parse_value_at(val_start))
                return false;

            // After value, si should point to ',' or '}'.
            if (si >= num_sp)
                return false;

            char delim = data[sp[si]];
            if (delim == '}') {
                ++si; // past '}'
                break;
            }
            if (delim == ',') {
                ++si; // past ','
                continue;
            }
            return false; // unexpected structural char
        }

        return handler.on_end_object();
    }

    // ── Array parsing ───────────────────────────────────────────────────

    bool parse_array() {
        if (si >= num_sp || data[sp[si]] != '[')
            return false;

        size_t open_bracket = sp[si];
        ++si; // past '['

        if (!handler.on_start_array(0))
            return false;

        // Check for empty array: next structural is ']'
        if (si < num_sp && data[sp[si]] == ']') {
            ++si; // past ']'
            return handler.on_end_array();
        }

        // First element starts after '['
        size_t elem_start = skip_ws_from(open_bracket + 1);

        for (;;) {
            if (!parse_value_at(elem_start))
                return false;

            // After value, si should point to ',' or ']'.
            if (si >= num_sp)
                return false;

            char delim = data[sp[si]];
            if (delim == ']') {
                ++si; // past ']'
                break;
            }
            if (delim == ',') {
                size_t comma_pos = sp[si];
                ++si; // past ','
                elem_start = skip_ws_from(comma_pos + 1);
                continue;
            }
            return false; // unexpected
        }

        return handler.on_end_array();
    }
};

// ── Entry point ─────────────────────────────────────────────────────────────

/**
 * Structural-index-driven SAX parse entry point.
 *
 * Builds a SIMD structural index, then navigates the JSON using pre-computed
 * structural positions instead of byte-by-byte scanning. Falls back to
 * parse_sax_inline for small documents (< 64 bytes) where SIMD overhead
 * exceeds the navigation savings.
 *
 * @tparam Handler  Concrete handler type (must provide JsonSaxHandler interface).
 * @param text          UTF-8 JSON input.
 * @param handler       SAX event sink.
 * @param validate_utf8 Run SIMD UTF-8 validation before parsing (default: true).
 * @return Status::Ok on success, Status::ParseError on failure.
 */
template <typename Handler>
[[nodiscard]] Status parse_sax_indexed(std::string_view text, Handler& handler,
                                       bool validate_utf8 = true) {
    // Small documents: structural index overhead exceeds benefit.
    if (text.size() < 64) {
        return parse_sax_inline(text, handler, validate_utf8);
    }

    if (validate_utf8 && !text.empty() && !util::validate_utf8_simd(text.data(), text.size())) {
        return Status::ParseError;
    }

    // Build structural index (SIMD pass).
    simd::IndexBuilder idx_builder;
    auto idx = idx_builder.build(reinterpret_cast<const uint8_t*>(text.data()), text.size());

    if (idx.positions.empty()) {
        // No structural chars — must be a bare primitive (number, string, bool, null).
        return parse_sax_inline(text, handler, false);
    }

    ParserIndexed<Handler> p{text.data(),          text.size(),          handler,
                             idx.positions.data(), idx.positions.size(), 0};
    if (!p.parse_value())
        return Status::ParseError;
    return Status::Ok;
}

} // namespace strata

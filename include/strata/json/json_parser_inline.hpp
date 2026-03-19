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
#include <type_traits>

namespace strata {

/// SFINAE trait: detect if Handler has try_match_key(const char*, size_t) → size_t.
template <typename T, typename = void> struct has_try_match_key : std::false_type {};

template <typename T>
struct has_try_match_key<T, std::void_t<decltype(std::declval<T&>().try_match_key(
                                std::declval<const char*>(), std::declval<size_t>()))>>
    : std::true_type {};

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

    void skip_ws() noexcept {
        // Inline fast exit: for compact JSON (the common case), the next
        // character is not whitespace.  Avoids calling skip_whitespace_fast
        // (which contains SIMD code the compiler may not inline).
        if (i < len && static_cast<unsigned char>(data[i]) > ' ')
            return;
        i = util::skip_whitespace_fast(data, len, i);
    }

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
        // Dispatch order optimised for JSON data where numbers and strings
        // dominate. Digits (0-9) use a branchless range check as the first
        // test because they are the most common value start in real-world JSON
        // (ints and floats). Strings come next. Structural and literal tokens
        // (objects, arrays, null, true/false) are rarer inside values.
        unsigned char c = static_cast<unsigned char>(peek());
        if (c - '0' <= 9u)
            return parse_number();
        if (c == '"')
            return parse_string();
        if (c == '-')
            return parse_number();
        if (c == '{')
            return parse_object();
        if (c == '[')
            return parse_array();
        if (c == 'n')
            return parse_null();
        if (c == 't' || c == 'f')
            return parse_bool();
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
     * Uses a unified parser that scans digits once — if '.' or 'e'/'E'
     * is found, continues to fractional/exponent part (returns double);
     * otherwise returns int64.  Avoids the previous double-scan overhead.
     */
    bool parse_number() {
        int64_t int_val;
        double dbl_val;
        size_t consumed;
        bool is_dbl;
        if (util::parse_number_unified(data + i, len - i, int_val, dbl_val, consumed, is_dbl)) {
            i += consumed;
            return is_dbl ? handler.on_double(dbl_val) : handler.on_int(int_val);
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
        // Pre-copy the clean prefix found by SIMD scan to avoid re-scanning.
        std::string out;
        out.reserve(scan_pos + 16);
        if (scan_pos > 0) {
            out.append(data + i, scan_pos);
            i += scan_pos;
        }

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

        skip_ws();
        if (eof())
            return false;
        if (peek() == ']') {
            ++i;
            return handler.on_end_array();
        }

        // Tight element loop — inline dispatch avoids parse_value's skip_ws overhead
        for (;;) {
            // Dispatch on first character (ws already skipped)
            if (eof())
                return false;
            {
                char c = peek();
                bool ok;
                if (c == '"')
                    ok = parse_string();
                else if (c >= '0' && c <= '9') {
                    // Inline fast path for positive numbers (very common in arrays).
                    // Uses multiplication by negative powers of 10 (faster than division).
                    static constexpr double kPow10Neg[] = {
                        1e0,   1e-1,  1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,
                        1e-8,  1e-9,  1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15,
                        1e-16, 1e-17, 1e-18, 1e-19, 1e-20, 1e-21, 1e-22,
                    };
                    {
                        size_t p = i + 1;
                        if (c == '0') {
                            // "0.xxx" pattern (random.random() values) — tight path.
                            if (__builtin_expect(p < len && data[p] == '.', 1)) {
                                ++p;
                                if (p < len && data[p] >= '0' && data[p] <= '9') {
                                    uint64_t val = 0;
                                    int frac_digits = 0;
                                    // 4x unrolled digit loop — reduces branch overhead
                                    // by 75% for typical 16-digit fractional parts.
                                    while (p + 4 <= len) {
                                        unsigned d0 = static_cast<unsigned>(data[p]) - '0';
                                        if (d0 > 9u)
                                            break;
                                        unsigned d1 = static_cast<unsigned>(data[p + 1]) - '0';
                                        if (d1 > 9u) {
                                            val = val * 10 + d0;
                                            ++frac_digits;
                                            p += 1;
                                            goto frac0_tail;
                                        }
                                        unsigned d2 = static_cast<unsigned>(data[p + 2]) - '0';
                                        if (d2 > 9u) {
                                            val = val * 100 + d0 * 10 + d1;
                                            frac_digits += 2;
                                            p += 2;
                                            goto frac0_tail;
                                        }
                                        unsigned d3 = static_cast<unsigned>(data[p + 3]) - '0';
                                        if (d3 > 9u) {
                                            val = val * 1000 + d0 * 100 + d1 * 10 + d2;
                                            frac_digits += 3;
                                            p += 3;
                                            goto frac0_tail;
                                        }
                                        val = val * 10000 + d0 * 1000 + d1 * 100 + d2 * 10 + d3;
                                        frac_digits += 4;
                                        p += 4;
                                    }
                                frac0_tail:
                                    // Scalar tail for remaining 0-3 digits
                                    while (p < len && data[p] >= '0' && data[p] <= '9') {
                                        val = val * 10 + static_cast<uint64_t>(data[p] - '0');
                                        ++frac_digits;
                                        ++p;
                                    }
                                    if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                        if (val < (1ULL << 53) && frac_digits <= 22) {
                                            i = p;
                                            ok = handler.on_double(static_cast<double>(val) *
                                                                   kPow10Neg[frac_digits]);
                                            goto dispatch_done;
                                        }
                                    }
                                }
                                // Fall through to full parser for edge cases
                            } else if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                // Bare "0"
                                i = p;
                                ok = handler.on_int(int64_t{0});
                                goto dispatch_done;
                            }
                        } else {
                            // Non-zero leading digit: integer or float.
                            // Track digit count to detect uint64 overflow (max 19 safe digits).
                            uint64_t val = static_cast<uint64_t>(c - '0');
                            int ndigits = 1;
                            while (p < len && data[p] >= '0' && data[p] <= '9') {
                                val = val * 10 + static_cast<uint64_t>(data[p] - '0');
                                ++ndigits;
                                ++p;
                            }
                            if (ndigits <= 18) {
                                // Safe: no overflow possible for <= 18 digits
                                if (p < len && data[p] == '.') {
                                    ++p;
                                    if (p < len && data[p] >= '0' && data[p] <= '9') {
                                        int frac_digits = 0;
                                        while (p < len && data[p] >= '0' && data[p] <= '9') {
                                            val = val * 10 + static_cast<uint64_t>(data[p] - '0');
                                            ++frac_digits;
                                            ++p;
                                        }
                                        if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                            if (val < (1ULL << 53) && frac_digits <= 22) {
                                                i = p;
                                                ok = handler.on_double(static_cast<double>(val) *
                                                                       kPow10Neg[frac_digits]);
                                                goto dispatch_done;
                                            }
                                        }
                                    }
                                } else if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                    if (val <= static_cast<uint64_t>(
                                                   std::numeric_limits<int64_t>::max())) {
                                        i = p;
                                        ok = handler.on_int(static_cast<int64_t>(val));
                                        goto dispatch_done;
                                    }
                                }
                            }
                            // > 18 digits or didn't match fast path: fall through to full parser
                        }
                    }
                    ok = parse_number();
                } else if (c == '-') {
                    // Inline fast path for negative numbers in arrays.
                    static constexpr double kPow10Neg2[] = {
                        1e0,   1e-1,  1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,
                        1e-8,  1e-9,  1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15,
                        1e-16, 1e-17, 1e-18, 1e-19, 1e-20, 1e-21, 1e-22,
                    };
                    size_t p = i + 1;
                    if (p < len && data[p] >= '1' && data[p] <= '9') {
                        uint64_t val = static_cast<uint64_t>(data[p] - '0');
                        int ndigits = 1;
                        ++p;
                        while (p < len && data[p] >= '0' && data[p] <= '9') {
                            val = val * 10 + static_cast<uint64_t>(data[p] - '0');
                            ++ndigits;
                            ++p;
                        }
                        if (ndigits <= 18) {
                            if (p < len && data[p] == '.') {
                                ++p;
                                if (p < len && data[p] >= '0' && data[p] <= '9') {
                                    int frac = 0;
                                    while (p < len && data[p] >= '0' && data[p] <= '9') {
                                        val = val * 10 + static_cast<uint64_t>(data[p] - '0');
                                        ++frac;
                                        ++p;
                                    }
                                    if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                        if (val < (1ULL << 53) && frac <= 22) {
                                            i = p;
                                            ok = handler.on_double(
                                                -(static_cast<double>(val) * kPow10Neg2[frac]));
                                            goto dispatch_done;
                                        }
                                    }
                                }
                            } else if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                if (val <=
                                    static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) +
                                        1) {
                                    i = p;
                                    ok = handler.on_int(-static_cast<int64_t>(val));
                                    goto dispatch_done;
                                }
                            }
                        }
                    } else if (p < len && data[p] == '0') {
                        ++p;
                        if (p < len && data[p] == '.') {
                            ++p;
                            if (p < len && data[p] >= '0' && data[p] <= '9') {
                                uint64_t val = 0;
                                int frac = 0;
                                while (p < len && data[p] >= '0' && data[p] <= '9') {
                                    val = val * 10 + static_cast<uint64_t>(data[p] - '0');
                                    ++frac;
                                    ++p;
                                }
                                if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                                    if (val < (1ULL << 53) && frac <= 22) {
                                        i = p;
                                        ok = handler.on_double(
                                            -(static_cast<double>(val) * kPow10Neg2[frac]));
                                        goto dispatch_done;
                                    }
                                }
                            }
                        } else if (p >= len || (data[p] != 'e' && data[p] != 'E')) {
                            i = p;
                            ok = handler.on_int(int64_t{0}); // -0 → 0
                            goto dispatch_done;
                        }
                    }
                    ok = parse_number();
                } else if (c == '{')
                    ok = parse_object();
                else if (c == '[')
                    ok = parse_array();
                else if (c == 't') {
                    if (i + 4 <= len && data[i + 1] == 'r' && data[i + 2] == 'u' &&
                        data[i + 3] == 'e') {
                        i += 4;
                        ok = handler.on_bool(true);
                    } else {
                        return false;
                    }
                } else if (c == 'f') {
                    if (i + 5 <= len && data[i + 1] == 'a' && data[i + 2] == 'l' &&
                        data[i + 3] == 's' && data[i + 4] == 'e') {
                        i += 5;
                        ok = handler.on_bool(false);
                    } else {
                        return false;
                    }
                } else if (c == 'n')
                    ok = parse_null();
                else
                    return false;
                if (!ok)
                    return false;
            dispatch_done:;
            }
            // Delimiter — optimised for compact JSON (no whitespace).
            // Avoids 2× skip_ws calls on the common path.
            if (__builtin_expect(i < len, 1)) {
                char d = data[i];
                if (__builtin_expect(d == ',', 1)) {
                    ++i;
                    // Fast exit: next char is not whitespace (compact JSON)
                    if (__builtin_expect(i < len && static_cast<unsigned char>(data[i]) > ' ', 1))
                        continue;
                    skip_ws();
                    continue;
                }
                if (d == ']') {
                    ++i;
                    break;
                }
                if (static_cast<unsigned char>(d) <= ' ') {
                    // Whitespace before delimiter — rare in benchmarks
                    skip_ws();
                    if (i >= len)
                        return false;
                    d = data[i];
                    if (d == ']') {
                        ++i;
                        break;
                    }
                    if (d != ',')
                        return false;
                    ++i;
                    skip_ws();
                    continue;
                }
                return false; // Invalid character
            }
            return false;
        }
        return handler.on_end_array();
    }

    /// Lightweight forward scan to count object keys without parsing values.
    /// Scans from position @p start (just after '{') counting colons at depth 0.
    /// Handles nested objects/arrays by tracking bracket depth, and skips strings
    /// to avoid counting colons inside string literals.
    /// Returns 0 if the scan would be too expensive (object > 4KB).
    size_t count_object_keys(size_t start) const noexcept {
        static constexpr size_t kMaxScanBytes = 4096;
        const size_t scan_end = (start + kMaxScanBytes < len) ? start + kMaxScanBytes : len;
        size_t depth = 0;
        size_t count = 0;
        bool found_first_key = false;

        for (size_t p = start; p < scan_end; ++p) {
            const char c = data[p];
            if (c == '"') {
                // Skip string content (handle escaped quotes)
                ++p;
                while (p < scan_end) {
                    if (data[p] == '\\') {
                        ++p; // skip escaped char
                    } else if (data[p] == '"') {
                        break;
                    }
                    ++p;
                }
            } else if (c == '{' || c == '[') {
                ++depth;
            } else if (c == '}') {
                if (depth == 0) {
                    // End of this object — return count
                    return found_first_key ? count + 1 : 0;
                }
                --depth;
            } else if (c == ']') {
                if (depth > 0)
                    --depth;
            } else if (c == ':' && depth == 0) {
                found_first_key = true;
                ++count;
                // Skip the value — fast-forward to next comma or closing brace at depth 0
            }
        }
        // Exceeded scan budget — return 0 (don't pre-size)
        return 0;
    }

    /// Lightweight forward scan to count array elements.
    /// Returns 0 if scan would be too expensive (array > 4KB).
    size_t count_array_elements(size_t start) const noexcept {
        static constexpr size_t kMaxScanBytes = 4096;
        const size_t scan_end = (start + kMaxScanBytes < len) ? start + kMaxScanBytes : len;
        size_t depth = 0;
        size_t count = 0;
        bool has_element = false;

        for (size_t p = start; p < scan_end; ++p) {
            const char c = data[p];
            if (c == '"') {
                ++p;
                while (p < scan_end) {
                    if (data[p] == '\\') {
                        ++p;
                    } else if (data[p] == '"') {
                        break;
                    }
                    ++p;
                }
                has_element = true;
            } else if (c == '{' || c == '[') {
                ++depth;
                has_element = true;
            } else if (c == ']') {
                if (depth == 0) {
                    return has_element ? count + 1 : 0;
                }
                --depth;
            } else if (c == '}') {
                if (depth > 0)
                    --depth;
            } else if (c == ',' && depth == 0) {
                ++count;
                has_element = true;
            } else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                has_element = true;
            }
        }
        return 0;
    }

    /// Try to match a predicted key (speculative key parsing).
    ///
    /// When the handler supports try_match_key(), the parser can skip
    /// the SIMD escape scan and key cache lookup for predicted keys.
    /// For same-schema JSON objects, this eliminates ~10,500 SIMD scans
    /// per 500 records × 21 keys.
    ///
    /// @param pos Position just after the opening quote.
    /// @return 0 = miss, 1 = matched key only, 2 = matched key + colon.
    int try_predicted_key(size_t pos) {
        if constexpr (has_try_match_key<Handler>::value) {
            size_t consumed = handler.try_match_key(data + pos, len - pos);
            if (consumed > 0) {
                i = pos + consumed;
                // Fast colon check: for compact JSON (the common case),
                // the colon immediately follows the closing quote with no
                // whitespace. Avoids entering the whitespace loop below.
                if (LIKELY(i < len && data[i] == ':')) {
                    ++i;
                    return 2; // matched key + colon
                }
                // Slow path: whitespace before colon (rare in machine-generated JSON)
                size_t j = i;
                while (j < len &&
                       (data[j] == ' ' || data[j] == '\t' || data[j] == '\n' || data[j] == '\r'))
                    ++j;
                if (j < len && data[j] == ':') {
                    i = j + 1;
                    return 2; // matched key + colon
                }
                return 1; // matched key only
            }
        }
        return 0;
    }

    bool parse_object() {
        if (!consume('{'))
            return false;

        if (!handler.on_start_object(0))
            return false;
        if (consume('}'))
            return handler.on_end_object();
        skip_ws(); // Only for first key
        for (;;) {
            // Speculative key parsing: try the cursor-predicted key first.
            // If the prediction hits, we skip SIMD scanning and cache lookup
            // entirely — just a memcmp against the predicted key bytes.
            if (__builtin_expect(i < len && data[i] == '"', 1)) {
                int key_result = try_predicted_key(i + 1);
                if (key_result == 0) {
                    // Prediction missed or not supported — full parse.
                    if (!parse_string(true))
                        return false;
                }
                if (key_result < 2) {
                    // Key matched but colon not consumed, or full parse path
                    // Fast colon: check directly before calling consume()
                    if (__builtin_expect(i < len && data[i] == ':', 1))
                        ++i;
                    else if (!consume(':'))
                        return false;
                }
            } else {
                return false;
            }
            if (!parse_value())
                return false;
            // Delimiter — optimised for compact JSON (no whitespace).
            if (__builtin_expect(i < len, 1)) {
                char d = data[i];
                if (d == '}') {
                    ++i;
                    break;
                }
                if (__builtin_expect(d == ',', 1)) {
                    ++i;
                    // Fast skip: if next char is '"' (the key), no ws needed
                    if (__builtin_expect(i < len && data[i] == '"', 1))
                        continue;
                    skip_ws();
                    continue;
                }
                if (static_cast<unsigned char>(d) <= ' ') {
                    skip_ws();
                    if (i >= len)
                        return false;
                    d = data[i];
                    if (d == '}') {
                        ++i;
                        break;
                    }
                    if (d != ',')
                        return false;
                    ++i;
                    skip_ws();
                    continue;
                }
                return false;
            }
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

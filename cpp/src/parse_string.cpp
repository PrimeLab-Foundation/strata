// parse_string.cpp — JSON string parser
//
// JSON string grammar (RFC 8259, Section 7):
//
//   string = '"' *char '"'
//   char   = unescaped | '\' escape
//   unescaped = any Unicode char except '"', '\', or control (U+0000–U+001F)
//   escape = '"' | '\' | '/' | 'b' | 'f' | 'n' | 'r' | 't' | 'u' hex4
//   hex4   = 4HEXDIG
//
// Unicode handling:
//   \uXXXX produces a UTF-16 code unit. Characters outside the BMP
//   (U+10000+) are encoded as surrogate pairs: \uD800–\uDBFF (high)
//   followed by \uDC00–\uDFFF (low). The pair is decoded into a single
//   code point and emitted as 4-byte UTF-8.
//
//   Lone surrogates (high without low, or low without high) are errors.
//   Raw bytes outside ASCII (multi-byte UTF-8 in the source) are passed
//   through unchanged — we don't validate incoming UTF-8.
//
// Performance notes:
//   - Escape lookup via constexpr std::array — single indexed read
//   - Hex digit conversion via branchless lookup table
//   - Main loop scans for '"' and '\' — the two chars that need action.
//     Everything else is a straight push_back.

#include "strata/parse_string.hpp"

#include <array>

namespace strata {

    namespace {

        // --- escape table: maps escape char → decoded byte, 0 = invalid ---

        constexpr auto make_escape_table() {
            std::array<char, 256> t{};
            t['"']  = '"';
            t['\\'] = '\\';
            t['/']  = '/';
            t['b']  = '\b';
            t['f']  = '\f';
            t['n']  = '\n';
            t['r']  = '\r';
            t['t']  = '\t';
            return t;
        }

        constexpr auto ESCAPE = make_escape_table();

        // --- hex digit → value, -1 = invalid ---

        constexpr auto make_hex_table() {
            std::array<int8_t, 256> t{};
            for (int i = 0; i < 256; ++i) t[i] = -1;
            for (int i = '0'; i <= '9'; ++i) t[i] = static_cast<int8_t>(i - '0');
            for (int i = 'a'; i <= 'f'; ++i) t[i] = static_cast<int8_t>(i - 'a' + 10);
            for (int i = 'A'; i <= 'F'; ++i) t[i] = static_cast<int8_t>(i - 'A' + 10);
            return t;
        }

        constexpr auto HEX = make_hex_table();

        inline int hex_val(char c) {
            return HEX[static_cast<unsigned char>(c)];
        }

        // --- read 4 hex digits → 16-bit value, -1 on error ---

        int read_hex4(const char* p, const char* end) {
            if (static_cast<size_t>(end - p) < 4) return -1;
            int a = hex_val(p[0]); if (a < 0) return -1;
            int b = hex_val(p[1]); if (b < 0) return -1;
            int c = hex_val(p[2]); if (c < 0) return -1;
            int d = hex_val(p[3]); if (d < 0) return -1;
            return (a << 12) | (b << 8) | (c << 4) | d;
        }

        // --- surrogate helpers ---

        inline bool is_high_surrogate(int cp) { return cp >= 0xD800 && cp <= 0xDBFF; }
        inline bool is_low_surrogate(int cp)  { return cp >= 0xDC00 && cp <= 0xDFFF; }

        inline int decode_pair(int high, int low) {
            return 0x10000 + ((high - 0xD800) << 10) + (low - 0xDC00);
        }

        // --- encode code point as UTF-8 ---

        void utf8_encode(uint32_t cp, std::string& out) {
            if (cp < 0x80) {
                out += static_cast<char>(cp);
            } else if (cp < 0x800) {
                out += static_cast<char>(0xC0 | (cp >> 6));
                out += static_cast<char>(0x80 | (cp & 0x3F));
            } else if (cp < 0x10000) {
                out += static_cast<char>(0xE0 | (cp >> 12));
                out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                out += static_cast<char>(0x80 | (cp & 0x3F));
            } else {
                out += static_cast<char>(0xF0 | (cp >> 18));
                out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
                out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                out += static_cast<char>(0x80 | (cp & 0x3F));
            }
        }

    }

    template <>
    Result<std::string> parse<std::string>(const char* cur, const char* end) {
        if (cur >= end)
            return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

        if (*cur != '"')
            return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});

        ++cur;

        std::string out;

        while (cur < end) {
            char ch = *cur;

            if (ch == '"')
                return Parsed<std::string>{std::move(out), cur + 1};

            if (static_cast<unsigned char>(ch) < 0x20)
                return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});

            if (ch != '\\') {
                out += ch;
                ++cur;
                continue;
            }

            // escape sequence
            if (++cur >= end)
                return std::unexpected(ParseError{ErrorCode::UnterminatedString, cur});

            if (*cur == 'u') {
                ++cur;
                int cp = read_hex4(cur, end);
                if (cp < 0)
                    return std::unexpected(ParseError{ErrorCode::InvalidUnicode, cur});
                cur += 4;

                if (is_high_surrogate(cp)) {
                    if (static_cast<size_t>(end - cur) < 2 || cur[0] != '\\' || cur[1] != 'u')
                        return std::unexpected(ParseError{ErrorCode::InvalidUnicode, cur});
                    cur += 2;
                    int low = read_hex4(cur, end);
                    if (low < 0 || !is_low_surrogate(low))
                        return std::unexpected(ParseError{ErrorCode::InvalidUnicode, cur});
                    cur += 4;
                    cp = decode_pair(cp, low);
                } else if (is_low_surrogate(cp)) {
                    return std::unexpected(ParseError{ErrorCode::InvalidUnicode, cur - 4});
                }

                utf8_encode(static_cast<uint32_t>(cp), out);
                continue;
            }

            char decoded = ESCAPE[static_cast<unsigned char>(*cur)];
            if (decoded == 0)
                return std::unexpected(ParseError{ErrorCode::InvalidEscape, cur});

            out += decoded;
            ++cur;
        }

        return std::unexpected(ParseError{ErrorCode::UnterminatedString, cur});
    }

}

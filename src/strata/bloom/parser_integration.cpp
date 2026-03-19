/**
 * @file parser_integration.cpp
 * @brief Implementation of SelectiveParser — Bloom-filter-driven selective parsing.
 *
 * SelectiveParser provides a focused JSON parser that only materializes values
 * for keys present in a Bloom filter. This is the key optimization for queries
 * that only need a subset of an object's fields: unwanted keys have their
 * values skipped via structural-index navigation (O(structural_chars) for
 * containers, O(1) for primitives) instead of being fully parsed.
 *
 * Architecture overview:
 *   - parse_selective() is the public entry point; it resets stats and
 *     delegates to parse_value() with selective=true.
 *   - parse_value() is the recursive descent driver. For objects, it extracts
 *     each key, checks the Bloom filter, and either parses or skips the value.
 *     For arrays, it parses all elements (selective mode only applies to
 *     object keys, not array indices).
 *   - skip_value() is the fast-skip function: for containers it walks the
 *     structural index counting depth; for primitives it's a no-op since
 *     the structural index already points past them.
 *
 * The structural index contains byte offsets for { } [ ] : , characters.
 * For an object like {"name":"Alice","age":30}:
 *   positions: { : , : }
 *
 * Parse strategy for objects:
 *   1. After '{', the next structural should be ':' (key:value separator)
 *   2. The key string is between the previous structural position and ':'
 *   3. The value is between ':' and the next ',' or '}'
 *   4. If value is '{' or '[', recurse into it
 */

#include "strata/bloom/parser_integration.h"

#include <charconv>
#include <cstring>

namespace strata {
namespace bloom {

/// Constructs a SelectiveParser with the given Bloom-filter-based key filter.
///
/// @param filter  A KeyFilter (backed by a Bloom filter) that determines
///                which object keys should be parsed vs. skipped.
SelectiveParser::SelectiveParser(const KeyFilter& filter) : filter_(filter) {}

// ─── Top-level selective parse ───────────────────────────────────────────────

/// Entry point for selective JSON parsing.
///
/// Parses the JSON document but only materializes values for keys that pass
/// the Bloom filter check. All other keys have their values skipped via
/// structural-index navigation, avoiding the cost of string decoding,
/// number parsing, and recursive descent for unwanted subtrees.
///
/// The parse_stats_ struct is reset on each call and tracks:
///   - keys_checked: total number of object keys encountered
///   - values_parsed: number of values fully parsed
///   - values_skipped: number of values skipped by the filter
///
/// @param data                  Raw JSON byte buffer.
/// @param length                Total buffer length.
/// @param structural_positions  Byte offsets of structural characters.
/// @param num_structural        Number of structural positions.
/// @return                      The parsed JsonValue (objects will only contain
///                              keys that passed the Bloom filter).

JsonValue SelectiveParser::parse_selective(const uint8_t* data, size_t length,
                                           const uint32_t* structural_positions,
                                           size_t num_structural) {
    parse_stats_ = {}; // reset statistics for this parse run
    if (num_structural == 0) {
        return JsonValue{};
    }

    size_t pos_index = 0;
    return parse_value(data, length, structural_positions, pos_index, num_structural,
                       /*selective=*/true);
}

// ─── skip_value: O(structural_chars) with no allocation ──────────────────────

/// Skips a JSON value using only the structural index — no allocation,
/// no string decoding, no number parsing.
///
/// This is the core optimization that makes selective parsing fast. When
/// a key is not in the Bloom filter, its entire value subtree is skipped
/// by walking structural positions rather than parsing raw bytes.
///
/// For containers ({ or [):
///   Uses a depth counter to match openers with closers. Walks through
///   structural positions incrementing depth on '{'/`[` and decrementing
///   on '}'/']'. When depth returns to 0, the matching closer has been
///   found and pos_index points past it. This correctly handles arbitrarily
///   nested objects and arrays in O(n) where n is the number of structural
///   characters within the container.
///
/// For primitives (string, number, bool, null):
///   Primitives are not represented in the structural index — they sit in
///   raw data between structural chars. The pos_index already points to the
///   next structural delimiter (',' or '}' or ']') after the primitive, so
///   no advancement is needed. This makes primitive skipping O(1).
///
/// @param data           Raw JSON byte buffer (only read at structural positions).
/// @param positions      Structural positions array.
/// @param pos_index      Current index into the structural positions.
/// @param num_positions  Total number of structural positions.
/// @return               Updated pos_index pointing past the skipped value.

size_t SelectiveParser::skip_value(const uint8_t* data, const uint32_t* positions, size_t pos_index,
                                   size_t num_positions) const noexcept {
    if (pos_index >= num_positions)
        return pos_index;

    // Determine the value type from the first structural byte
    uint8_t ch = data[positions[pos_index]];

    if (ch == '{' || ch == '[') {
        // Container skip: brace/bracket-counting through the structural index.
        // Start with depth=1 for the opener, walk until depth returns to 0.
        int depth = 1;
        ++pos_index; // skip past the opener
        while (pos_index < num_positions && depth > 0) {
            uint8_t c = data[positions[pos_index]];
            if (c == '{' || c == '[')
                ++depth; // nested container opened
            else if (c == '}' || c == ']')
                --depth; // container closed
            ++pos_index;
        }
        return pos_index;
    }

    // Primitive value: no structural chars to skip over.
    // The caller's pos_index already points at the delimiter following
    // this value (',' or '}' or ']'), so we return it unchanged.
    return pos_index;
}

// ─── skip_ws ─────────────────────────────────────────────────────────────────

/// Advances past JSON whitespace characters (space, tab, LF, CR).
///
/// JSON (RFC 8259) defines exactly four whitespace characters. This simple
/// byte-by-byte scan is used to find the start of values between structural
/// characters (e.g., after ':' before a value, or after '[' before the
/// first element).
///
/// @param data    Raw JSON byte buffer.
/// @param pos     Starting position to scan from.
/// @param length  Total buffer length for bounds checking.
/// @return        Position of the first non-whitespace byte, or `length`.
size_t SelectiveParser::skip_ws(const uint8_t* data, size_t pos, size_t length) const noexcept {
    while (pos < length) {
        uint8_t c = data[pos];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
        ++pos;
    }
    return pos;
}

// ─── parse_string ────────────────────────────────────────────────────────────

/// Parses a JSON string starting at the opening double-quote character.
///
/// Handles all standard JSON escape sequences:
///   - Simple escapes: \" \\ \/ \b \f \n \r \t
///   - Unicode escapes: \uXXXX with 4 hex digits, decoded to UTF-8
///
/// For \uXXXX escapes, the 4-digit hex value is decoded into a Unicode
/// codepoint and then encoded as UTF-8:
///   - U+0000..U+007F: 1-byte UTF-8
///   - U+0080..U+07FF: 2-byte UTF-8
///   - U+0800..U+FFFF: 3-byte UTF-8
///
/// Note: Unlike the speculative parser's parse_string_at(), this version
/// does not handle UTF-16 surrogate pairs (\uD800-\uDFFF). Surrogate
/// codepoints are encoded as-is into 3-byte UTF-8 sequences. This is
/// acceptable for the selective parser's use case where strings are
/// typically short object keys.
///
/// @param data    Raw JSON byte buffer.
/// @param start   Position of the opening '"' character.
/// @param length  Total buffer length for bounds checking.
/// @return        The decoded string with escapes resolved, or empty string
///                if start is out of bounds or doesn't point to '"'.
std::string SelectiveParser::parse_string(const uint8_t* data, size_t start, size_t length) const {
    if (start >= length || data[start] != '"')
        return {};
    ++start; // skip opening quote

    std::string result;
    result.reserve(32); // pre-allocate for typical short strings

    while (start < length) {
        uint8_t c = data[start];
        if (c == '"')
            break;
        if (c == '\\') {
            ++start;
            if (start >= length)
                break;
            uint8_t esc = data[start];
            switch (esc) {
            case '"':
                result += '"';
                break;
            case '\\':
                result += '\\';
                break;
            case '/':
                result += '/';
                break;
            case 'b':
                result += '\b';
                break;
            case 'f':
                result += '\f';
                break;
            case 'n':
                result += '\n';
                break;
            case 'r':
                result += '\r';
                break;
            case 't':
                result += '\t';
                break;
            case 'u': {
                // \uXXXX Unicode escape: decode 4 hex digits into a codepoint
                if (start + 4 >= length) {
                    ++start;
                    continue;
                }
                uint32_t cp = 0;
                for (int i = 1; i <= 4; ++i) {
                    uint8_t h = data[start + i];
                    cp <<= 4;
                    if (h >= '0' && h <= '9')
                        cp |= (h - '0');
                    else if (h >= 'a' && h <= 'f')
                        cp |= (h - 'a' + 10);
                    else if (h >= 'A' && h <= 'F')
                        cp |= (h - 'A' + 10);
                }
                start += 4;
                // Encode the codepoint as UTF-8 (1, 2, or 3 bytes depending on range)
                if (cp < 0x80) {
                    // ASCII range: single byte
                    result += static_cast<char>(cp);
                } else if (cp < 0x800) {
                    // 2-byte UTF-8 (U+0080 to U+07FF)
                    result += static_cast<char>(0xC0 | (cp >> 6));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                } else {
                    // 3-byte UTF-8 (U+0800 to U+FFFF)
                    result += static_cast<char>(0xE0 | (cp >> 12));
                    result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                }
                break;
            }
            default:
                result += static_cast<char>(esc);
                break;
            }
        } else {
            result += static_cast<char>(c);
        }
        ++start;
    }
    return result;
}

// ─── parse_number ────────────────────────────────────────────────────────────

/// Parses a JSON number from the raw byte buffer.
///
/// Scans forward from `start` until a delimiter (comma, brace, bracket,
/// or whitespace) is found, then uses std::from_chars for the actual
/// numeric conversion. This approach avoids creating a temporary string
/// copy of the number text.
///
/// @param data    Raw JSON byte buffer.
/// @param start   Byte position of the first digit (or '-').
/// @param length  Total buffer length for bounds checking.
/// @return        The parsed double value.
double SelectiveParser::parse_number(const uint8_t* data, size_t start, size_t length) const {
    // Find the end of the number by scanning for the next delimiter
    size_t end = start;
    while (end < length) {
        uint8_t c = data[end];
        if (c == ',' || c == '}' || c == ']' || c == ' ' || c == '\t' || c == '\n' || c == '\r')
            break;
        ++end;
    }

    // Use std::from_chars for locale-independent, allocation-free parsing
    double value = 0.0;
    std::from_chars(reinterpret_cast<const char*>(data + start),
                    reinterpret_cast<const char*>(data + end), value);
    return value;
}

// ─── parse_primitive ─────────────────────────────────────────────────────────

/// Parses a primitive JSON value (string, number, boolean, or null) from a
/// bounded region of the raw byte buffer.
///
/// The value is located between `start` and `end` (exclusive), where `end`
/// typically points to the next structural delimiter (',' or '}' or ']').
/// The function first skips whitespace, then classifies the value by its
/// first byte and delegates to the appropriate parser.
///
/// First-byte classification:
///   '"' → string (delegate to parse_string)
///   '-' or '0'-'9' → number (delegate to parse_number)
///   't' → boolean true (verify literal "true" bytes)
///   'f' → boolean false (verify literal "false" bytes)
///   'n' → null (verify literal "null" bytes)
///
/// @param data    Raw JSON byte buffer.
/// @param start   Start of the region containing the value.
/// @param end     End of the region (position of the next structural delimiter).
/// @param length  Total buffer length for bounds checking.
/// @return        The parsed JsonValue.
JsonValue SelectiveParser::parse_primitive(const uint8_t* data, size_t start, size_t end,
                                           size_t length) const {
    start = skip_ws(data, start, length);
    if (start >= length)
        return JsonValue{};

    uint8_t c = data[start];

    if (c == '"') {
        return JsonValue{JsonValue::Variant{parse_string(data, start, length)}};
    }

    if (c == '-' || (c >= '0' && c <= '9')) {
        return JsonValue{JsonValue::Variant{parse_number(data, start, end)}};
    }

    if (c == 't' && start + 3 < length && data[start + 1] == 'r' && data[start + 2] == 'u' &&
        data[start + 3] == 'e') {
        return JsonValue{JsonValue::Variant{true}};
    }

    if (c == 'f' && start + 4 < length && data[start + 1] == 'a' && data[start + 2] == 'l' &&
        data[start + 3] == 's' && data[start + 4] == 'e') {
        return JsonValue{JsonValue::Variant{false}};
    }

    if (c == 'n' && start + 3 < length && data[start + 1] == 'u' && data[start + 2] == 'l' &&
        data[start + 3] == 'l') {
        return JsonValue{JsonValue::Variant{nullptr}};
    }

    return JsonValue{};
}

// ─── Extract key between two byte positions ──────────────────────────────────

/// Extracts a JSON object key as a string_view from the raw byte buffer.
///
/// Given the byte region between a previous structural position ('{' or ',')
/// and the colon position (':'), this function locates the quoted key string
/// within that region. It returns a view into the raw buffer (zero-copy),
/// which avoids allocation for the common case of unescaped keys.
///
/// The extraction works in two steps:
///   1. Scan forward from after_pos to find the opening '"' of the key.
///   2. Scan forward from the opening quote to find the closing '"',
///      handling escape sequences by skipping '\\' + next character pairs.
///
/// Limitations: the returned string_view points into the raw buffer and
/// includes escape sequences as-is (e.g., "na\u006de" is not decoded).
/// This is fine for Bloom filter key matching, which hashes the raw key bytes.
///
/// @param data       Raw JSON byte buffer.
/// @param after_pos  Start of the search region (byte after previous structural char).
/// @param colon_pos  End of the search region (position of the ':' character).
/// @param length     Total buffer length (unused, kept for API consistency).
/// @return           A string_view of the key content (between quotes), or
///                   empty if no quoted string is found in the region.
static std::string_view extract_key(const uint8_t* data, size_t after_pos, size_t colon_pos,
                                    size_t /*length*/) noexcept {
    // Step 1: find the opening double-quote of the key
    size_t pos = after_pos;
    while (pos < colon_pos && data[pos] != '"')
        ++pos;
    if (pos >= colon_pos)
        return {};
    ++pos; // skip opening '"'
    size_t key_start = pos;

    // Step 2: find the closing double-quote, skipping escape sequences.
    // We only need to skip '\\' + one byte — full escape decoding is not
    // necessary since we're just extracting the raw key text for hashing.
    while (pos < colon_pos) {
        if (data[pos] == '\\') {
            pos += 2; // skip escaped character
            continue;
        }
        if (data[pos] == '"')
            break;
        ++pos;
    }
    return std::string_view(reinterpret_cast<const char*>(data + key_start), pos - key_start);
}

// ─── parse_value ─────────────────────────────────────────────────────────────

/// Recursive descent parser for JSON values with selective filtering.
///
/// This is the main recursive function driving the selective parser. It
/// examines the byte at the current structural position to determine the
/// value type, then dispatches to the appropriate parsing logic:
///
///   - '{' → Object: iterates over key-value pairs. For each key, checks
///     the Bloom filter (if selective=true). Keys not in the filter have
///     their values skipped via skip_value(). Keys in the filter have their
///     values recursively parsed.
///   - '[' → Array: iterates over elements, parsing each one. Selective
///     filtering does not apply to array elements (only to object keys).
///   - Otherwise → Top-level primitive: parsed via parse_primitive().
///
/// The `selective` flag is propagated to recursive calls, so nested objects
/// also benefit from Bloom-filter-based key skipping. This means if a
/// top-level key passes the filter but contains a nested object, the nested
/// object's keys are also checked against the filter.
///
/// @param data           Raw JSON byte buffer.
/// @param length         Total buffer length.
/// @param positions      Structural positions array.
/// @param pos_index      Current index into structural positions (updated in-place).
/// @param num_positions  Total number of structural positions.
/// @param selective      If true, apply Bloom filter to object keys.
/// @return               The parsed JsonValue.

JsonValue SelectiveParser::parse_value(const uint8_t* data, size_t length,
                                       const uint32_t* positions, size_t& pos_index,
                                       size_t num_positions, bool selective) {
    if (pos_index >= num_positions)
        return JsonValue{};

    uint32_t byte_pos = positions[pos_index];
    uint8_t ch = data[byte_pos];

    // ── Object ───────────────────────────────────────────────────────────────
    if (ch == '{') {
        // Structural pattern for objects:
        //   { : , : , : }
        // After '{', alternate between ':' (colon) and ',' (comma) or '}'.
        // The key is between the previous structural char and the colon.
        // The value is between the colon and the next comma/close-brace.

        size_t open_brace_pos = byte_pos;
        ++pos_index; // consume '{'

        JsonValue::Object obj;

        // Iterate over key-value pairs until '}' is found.
        // Structural pattern: { : , : , : }
        // Each iteration expects a ':' (colon) separating key from value.
        while (pos_index < num_positions) {
            uint32_t bp = positions[pos_index];
            uint8_t c = data[bp];

            if (c == '}') {
                ++pos_index; // consume closing brace
                break;
            }

            // Expect ':' — the colon separating key from value.
            // Any other structural char here indicates malformed JSON.
            if (c != ':') {
                ++pos_index; // skip unexpected structural char
                continue;
            }

            // Extract the key: it's a quoted string in the raw data between
            // the previous structural position and this colon.
            size_t prev_end = (pos_index >= 1) ? positions[pos_index - 1] + 1 : open_brace_pos + 1;
            std::string_view key_sv = extract_key(data, prev_end, bp, length);

            ++parse_stats_.keys_checked;

            size_t colon_pos_byte = bp;
            ++pos_index; // consume ':'

            // ── Bloom filter check: skip unwanted keys ───────────────────
            // This is the core selective parsing optimization. If the key
            // is not in the Bloom filter, skip its entire value subtree
            // without parsing it.
            if (selective && !key_sv.empty() && !filter_.should_parse(key_sv)) {
                ++parse_stats_.values_skipped;
                // For container values ({, [), use structural skip.
                // For primitives, the structural index already points past them.
                if (pos_index < num_positions) {
                    uint8_t next_ch = data[positions[pos_index]];
                    if (next_ch == '{' || next_ch == '[') {
                        pos_index = skip_value(data, positions, pos_index, num_positions);
                    }
                    // Primitives: pos_index already at the delimiter — no skip needed.
                }

                // Consume the ',' separator to advance to the next key-value pair
                if (pos_index < num_positions && data[positions[pos_index]] == ',') {
                    ++pos_index;
                }
                continue;
            }

            // ── Parse the value (key passed the filter) ──────────────────
            if (pos_index < num_positions) {
                uint8_t next_ch = data[positions[pos_index]];

                if (next_ch == '{' || next_ch == '[') {
                    // Nested container — recurse with selective flag propagated
                    auto val =
                        parse_value(data, length, positions, pos_index, num_positions, selective);
                    obj[std::string(key_sv)] = std::move(val);
                    ++parse_stats_.values_parsed;
                } else {
                    // Primitive value: located between colon and next structural char
                    size_t val_start = skip_ws(data, colon_pos_byte + 1, length);
                    size_t val_end = positions[pos_index];
                    auto val = parse_primitive(data, val_start, val_end, length);
                    obj[std::string(key_sv)] = std::move(val);
                    ++parse_stats_.values_parsed;
                }
            }

            // Consume ',' separator if present (more key-value pairs follow)
            if (pos_index < num_positions && data[positions[pos_index]] == ',') {
                ++pos_index;
            }
        }

        return JsonValue{JsonValue::Variant{std::move(obj)}};
    }

    // ── Array ────────────────────────────────────────────────────────────────
    // Arrays parse all elements unconditionally (selective filtering applies
    // only to object keys, not array indices). Elements are found in the raw
    // data between structural characters: [ elem , elem , elem ]
    if (ch == '[') {
        ++pos_index; // consume '['
        JsonValue::Array arr;

        // Track the byte position of the last processed structural char.
        // Primitive elements are located between prev_byte_pos and the
        // next structural char (',' or ']').
        size_t prev_byte_pos = byte_pos; // starts at '['

        while (pos_index < num_positions) {
            uint32_t bp = positions[pos_index];
            uint8_t c = data[bp];

            if (c == ']') {
                // End of array — but there may be a trailing primitive element
                // between the last structural char and ']'. Check for it.
                size_t elem_start = skip_ws(data, prev_byte_pos + 1, length);
                if (elem_start < bp) {
                    auto val = parse_primitive(data, elem_start, bp, length);
                    // Only add non-null values, unless the text explicitly is "null"
                    if (!val.is_null() || (bp - elem_start > 0 && data[elem_start] == 'n')) {
                        arr.push_back(std::move(val));
                        ++parse_stats_.values_parsed;
                    }
                }
                ++pos_index; // consume ']'
                break;
            }

            if (c == '{' || c == '[') {
                // Nested container as array element — recurse
                auto val =
                    parse_value(data, length, positions, pos_index, num_positions, selective);
                arr.push_back(std::move(val));
                ++parse_stats_.values_parsed;
                // Update prev_byte_pos to the last consumed structural position
                prev_byte_pos = positions[pos_index > 0 ? pos_index - 1 : 0];
                continue;
            }

            if (c == ',') {
                // Comma separator — the primitive element is between
                // prev_byte_pos and this comma position.
                size_t elem_start = skip_ws(data, prev_byte_pos + 1, length);
                if (elem_start < bp) {
                    auto val = parse_primitive(data, elem_start, bp, length);
                    arr.push_back(std::move(val));
                    ++parse_stats_.values_parsed;
                }
                prev_byte_pos = bp; // advance past this comma
                ++pos_index;
                continue;
            }

            // Colon shouldn't appear inside arrays — skip (malformed JSON)
            ++pos_index;
        }

        return JsonValue{JsonValue::Variant{std::move(arr)}};
    }

    // ── Top-level primitive ──────────────────────────────────────────────────
    // If the structural char is neither '{' nor '[', it's a delimiter that
    // precedes or follows a top-level primitive value. Parse the primitive
    // from the raw data at this position.
    {
        size_t prim_start = skip_ws(data, byte_pos, length);
        // The primitive extends to the next structural position or end of buffer
        size_t prim_end = (pos_index + 1 < num_positions) ? positions[pos_index + 1] : length;
        ++pos_index;
        ++parse_stats_.values_parsed;
        return parse_primitive(data, prim_start, prim_end, length);
    }
}

} // namespace bloom
} // namespace strata

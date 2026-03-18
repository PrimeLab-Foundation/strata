/**
 * @file parser_integration.cpp
 * @brief Implementation of SelectiveParser — Bloom-filter-driven selective parsing.
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

SelectiveParser::SelectiveParser(const KeyFilter& filter) : filter_(filter) {}

// ─── Top-level selective parse ───────────────────────────────────────────────

JsonValue SelectiveParser::parse_selective(const uint8_t* data, size_t length,
                                           const uint32_t* structural_positions,
                                           size_t num_structural) {
    parse_stats_ = {};
    if (num_structural == 0) {
        return JsonValue{};
    }

    size_t pos_index = 0;
    return parse_value(data, length, structural_positions, pos_index, num_structural,
                       /*selective=*/true);
}

// ─── skip_value: O(structural_chars) with no allocation ──────────────────────

size_t SelectiveParser::skip_value(const uint8_t* data, const uint32_t* positions, size_t pos_index,
                                   size_t num_positions) const noexcept {
    if (pos_index >= num_positions)
        return pos_index;

    // Look at what comes next structurally to decide how to skip.
    uint8_t ch = data[positions[pos_index]];

    if (ch == '{' || ch == '[') {
        // Container: count depth until matching close.
        int depth = 1;
        ++pos_index;
        while (pos_index < num_positions && depth > 0) {
            uint8_t c = data[positions[pos_index]];
            if (c == '{' || c == '[')
                ++depth;
            else if (c == '}' || c == ']')
                --depth;
            ++pos_index;
        }
        return pos_index;
    }

    // Primitive value: the value sits between the current byte position
    // and the next structural char (which will be , or } or ]).
    // We don't need to advance pos_index because the caller will see
    // the ',' or '}' at the current pos_index.
    return pos_index;
}

// ─── skip_ws ─────────────────────────────────────────────────────────────────

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

std::string SelectiveParser::parse_string(const uint8_t* data, size_t start, size_t length) const {
    if (start >= length || data[start] != '"')
        return {};
    ++start; // skip opening quote

    std::string result;
    result.reserve(32);

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
                if (cp < 0x80) {
                    result += static_cast<char>(cp);
                } else if (cp < 0x800) {
                    result += static_cast<char>(0xC0 | (cp >> 6));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                } else {
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

double SelectiveParser::parse_number(const uint8_t* data, size_t start, size_t length) const {
    size_t end = start;
    while (end < length) {
        uint8_t c = data[end];
        if (c == ',' || c == '}' || c == ']' || c == ' ' || c == '\t' || c == '\n' || c == '\r')
            break;
        ++end;
    }

    double value = 0.0;
    std::from_chars(reinterpret_cast<const char*>(data + start),
                    reinterpret_cast<const char*>(data + end), value);
    return value;
}

// ─── parse_primitive ─────────────────────────────────────────────────────────

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

/// Find the JSON key string between `after_pos` (exclusive) and `colon_pos`
/// (the position of ':').  The key is a quoted string somewhere in that range.
static std::string_view extract_key(const uint8_t* data, size_t after_pos, size_t colon_pos,
                                    size_t /*length*/) noexcept {
    // Scan forward from after_pos to find the opening '"' of the key.
    size_t pos = after_pos;
    while (pos < colon_pos && data[pos] != '"')
        ++pos;
    if (pos >= colon_pos)
        return {};
    ++pos; // skip opening '"'
    size_t key_start = pos;

    // Find closing '"' (handle escapes simply — skip \\ sequences).
    while (pos < colon_pos) {
        if (data[pos] == '\\') {
            pos += 2;
            continue;
        }
        if (data[pos] == '"')
            break;
        ++pos;
    }
    return std::string_view(reinterpret_cast<const char*>(data + key_start), pos - key_start);
}

// ─── parse_value ─────────────────────────────────────────────────────────────

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

        while (pos_index < num_positions) {
            uint32_t bp = positions[pos_index];
            uint8_t c = data[bp];

            if (c == '}') {
                ++pos_index; // consume '}'
                break;
            }

            // We expect ':' next (the colon after a key).
            if (c != ':') {
                // Unexpected — skip past.
                ++pos_index;
                continue;
            }

            // Extract the key: it's between the previous structural position and ':'.
            size_t prev_end = (pos_index >= 1) ? positions[pos_index - 1] + 1 : open_brace_pos + 1;
            std::string_view key_sv = extract_key(data, prev_end, bp, length);

            ++parse_stats_.keys_checked;

            size_t colon_pos_byte = bp;
            ++pos_index; // consume ':'

            if (selective && !key_sv.empty() && !filter_.should_parse(key_sv)) {
                // Skip the value.
                ++parse_stats_.values_skipped;
                // The next structural after ':' is either the start of a container
                // ({, [) or a delimiter (,, }) if the value is primitive.
                if (pos_index < num_positions) {
                    uint8_t next_ch = data[positions[pos_index]];
                    if (next_ch == '{' || next_ch == '[') {
                        pos_index = skip_value(data, positions, pos_index, num_positions);
                    }
                    // For primitives, value sits between ':' and next ',' or '}'.
                    // pos_index already points at the ',' or '}', so nothing to skip.
                }

                // Now consume the ',' separator if present.
                if (pos_index < num_positions && data[positions[pos_index]] == ',') {
                    ++pos_index;
                }
                continue;
            }

            // Parse the value.
            if (pos_index < num_positions) {
                uint8_t next_ch = data[positions[pos_index]];

                if (next_ch == '{' || next_ch == '[') {
                    // Nested container.
                    auto val =
                        parse_value(data, length, positions, pos_index, num_positions, selective);
                    obj[std::string(key_sv)] = std::move(val);
                    ++parse_stats_.values_parsed;
                } else {
                    // Primitive value: between colon and this structural char.
                    size_t val_start = skip_ws(data, colon_pos_byte + 1, length);
                    size_t val_end = positions[pos_index];
                    auto val = parse_primitive(data, val_start, val_end, length);
                    obj[std::string(key_sv)] = std::move(val);
                    ++parse_stats_.values_parsed;
                }
            }

            // Consume ',' if present.
            if (pos_index < num_positions && data[positions[pos_index]] == ',') {
                ++pos_index;
            }
        }

        return JsonValue{JsonValue::Variant{std::move(obj)}};
    }

    // ── Array ────────────────────────────────────────────────────────────────
    if (ch == '[') {
        ++pos_index; // consume '['
        JsonValue::Array arr;

        size_t prev_byte_pos = byte_pos; // position of '['

        while (pos_index < num_positions) {
            uint32_t bp = positions[pos_index];
            uint8_t c = data[bp];

            if (c == ']') {
                // Check for a trailing primitive element between prev and ']'.
                size_t elem_start = skip_ws(data, prev_byte_pos + 1, length);
                if (elem_start < bp) {
                    auto val = parse_primitive(data, elem_start, bp, length);
                    if (!val.is_null() || (bp - elem_start > 0 && data[elem_start] == 'n')) {
                        arr.push_back(std::move(val));
                        ++parse_stats_.values_parsed;
                    }
                }
                ++pos_index;
                break;
            }

            if (c == '{' || c == '[') {
                // Nested container as array element.
                auto val =
                    parse_value(data, length, positions, pos_index, num_positions, selective);
                arr.push_back(std::move(val));
                ++parse_stats_.values_parsed;
                prev_byte_pos = positions[pos_index > 0 ? pos_index - 1 : 0];
                continue;
            }

            if (c == ',') {
                // Primitive element between prev_byte_pos and this comma.
                size_t elem_start = skip_ws(data, prev_byte_pos + 1, length);
                if (elem_start < bp) {
                    auto val = parse_primitive(data, elem_start, bp, length);
                    arr.push_back(std::move(val));
                    ++parse_stats_.values_parsed;
                }
                prev_byte_pos = bp;
                ++pos_index;
                continue;
            }

            // Colon shouldn't appear in arrays, skip.
            ++pos_index;
        }

        return JsonValue{JsonValue::Variant{std::move(arr)}};
    }

    // ── Top-level primitive ──────────────────────────────────────────────────
    {
        size_t prim_start = skip_ws(data, byte_pos, length);
        size_t prim_end = (pos_index + 1 < num_positions) ? positions[pos_index + 1] : length;
        ++pos_index;
        ++parse_stats_.values_parsed;
        return parse_primitive(data, prim_start, prim_end, length);
    }
}

} // namespace bloom
} // namespace strata

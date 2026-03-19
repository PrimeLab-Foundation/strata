// =============================================================================
// strata/speculative/parser.cpp
//
// Speculative parser coordinator implementation.
//
// This module implements the core speculative parsing strategy for strata.
// The central idea is to use a Markov-based prediction model to speculatively
// guess the type of the next JSON value before fully parsing it. When the
// prediction is correct, a type-specific fast path handles the value with
// minimal branching and no unnecessary type checks. When the prediction is
// wrong, we fall back to the generic (non-speculative) path.
//
// Speculation flow for each value:
//   1. The prediction model examines the current parser context (nesting
//      depth, key hash, array index, last value type) and produces a
//      predicted ValueType along with a confidence score.
//   2. If confidence exceeds the threshold and enough values have been
//      observed (warmup period), the parser dispatches to a type-specific
//      fast-path function via a static dispatch table.
//   3. The fast-path attempts to parse the value under the assumption that
//      the predicted type is correct. If the raw bytes confirm the guess
//      (e.g., starts with '"' for a string prediction), it returns success.
//   4. On success ("speculation hit"), the parsed value is returned directly
//      — avoiding the overhead of a full type-classification switch.
//   5. On failure ("speculation miss"), the parser falls through to the
//      generic parse_primitive_at() which handles any value type.
//
// Key design: the structural index only contains positions of { } [ ] : ,
// (outside strings). Values like numbers, strings, bools, and null are NOT
// in the structural index. The parser uses a raw-data cursor alongside the
// structural index to navigate JSON correctly:
//
//   - Structural chars ({ } [ ] : ,) are found via the structural index
//   - Values between structural chars are found by scanning raw data
//   - The cursor tracks current position in the raw byte stream
//   - pos_index tracks position in the structural index
// =============================================================================

#include <cstdlib>
#include <cstring>
#include <limits>
#include <strata/speculative/parser.h>
#include <strata/util/fast_parse.hpp>
#include <string>

namespace strata::speculative {

// ─── Construction ────────────────────────────────────────────────────────────

/// Constructs a SpeculativeParser with the given configuration and memory arena.
///
/// @param config  Controls speculation behavior: whether speculation is enabled,
///                the online-learning warmup threshold, and confidence parameters.
/// @param arena   Arena allocator used by fast-path parsers to avoid heap
///                allocations during value construction.
SpeculativeParser::SpeculativeParser(const Config& config, strata::util::Arena& arena)
    : config_(config), model_(), fast_paths_(arena), arena_(arena) {}

// ─── Whitespace skip ─────────────────────────────────────────────────────────

/// Advances `offset` past any JSON whitespace characters (space, tab, LF, CR).
///
/// JSON defines exactly four whitespace characters (RFC 8259 Section 2).
/// This function performs a simple byte-by-byte scan — for short gaps between
/// values this is faster than SIMD-based approaches due to branch prediction
/// hitting the "not whitespace" case quickly.
///
/// @param data    Pointer to the raw JSON byte buffer.
/// @param offset  Starting position to scan from.
/// @param length  Total length of the data buffer (bounds check).
/// @return        Position of the first non-whitespace byte, or `length` if
///                the remainder is entirely whitespace.
size_t SpeculativeParser::skip_ws(const uint8_t* data, size_t offset, size_t length) noexcept {
    while (offset < length) {
        uint8_t c = data[offset];
        // RFC 8259: only 0x20, 0x09, 0x0A, 0x0D are valid JSON whitespace
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
        ++offset;
    }
    return offset;
}

// ─── Value type classification ───────────────────────────────────────────────

/// Classifies a JSON value type from its first byte.
///
/// JSON values always begin with a deterministic first character:
///   '"' → string, '{' → object, '[' → array, 't' → true, 'f' → false,
///   'n' → null, and anything else (digits, '-') → number.
///
/// This is used as a fallback classifier when speculation is not applicable.
/// Note: strings default to SHORT_STRING — the actual length is determined
/// after parsing. Numbers default to INTEGER — float detection happens during
/// number parsing when a '.' or 'e'/'E' is encountered.
///
/// @param c  The first byte of the JSON value.
/// @return   The classified ValueType.
ValueType SpeculativeParser::classify_value_byte(uint8_t c) noexcept {
    switch (c) {
    case '"':
        return ValueType::SHORT_STRING;
    case '{':
        return ValueType::OBJECT;
    case '[':
        return ValueType::ARRAY;
    case 't':
        return ValueType::BOOL_TRUE;
    case 'f':
        return ValueType::BOOL_FALSE;
    case 'n':
        return ValueType::NULL_VALUE;
    default:
        // Any digit or '-' indicates a numeric value; we assume integer
        // until parse_number_unified determines otherwise.
        return ValueType::INTEGER;
    }
}

// ─── String parser helper ────────────────────────────────────────────────────

/// Parses a JSON string starting at the opening double-quote, handling all
/// standard JSON escape sequences including \uXXXX Unicode escapes and
/// UTF-16 surrogate pairs.
///
/// The function processes the string in two modes:
///   1. Escape mode: when a backslash is encountered, the next character is
///      interpreted as an escape code (\", \\, \/, \b, \f, \n, \r, \t, \uXXXX).
///   2. Fast run mode: unescaped characters are bulk-appended using a scan
///      loop that searches for the next '"' or '\\', minimizing per-byte overhead.
///
/// For \uXXXX escapes, the function decodes the 4-digit hex codepoint and
/// emits UTF-8. If the codepoint is a high surrogate (0xD800-0xDBFF), it
/// looks ahead for a \uXXXX low surrogate to form a full supplementary-plane
/// character (encoded as 4-byte UTF-8).
///
/// @param data    Raw JSON byte buffer.
/// @param offset  On entry, points to the opening '"'. On exit, points to the
///                byte immediately after the closing '"'.
/// @param length  Total buffer length for bounds checking.
/// @return        The decoded string with escape sequences resolved.
std::string SpeculativeParser::parse_string_at(const uint8_t* data, size_t& offset, size_t length) {
    ++offset; // skip opening '"'
    std::string result;
    result.reserve(32); // most JSON strings are short; avoids early reallocation

    while (offset < length) {
        uint8_t c = data[offset];
        if (c == '"') {
            ++offset;
            return result;
        }
        if (c == '\\') {
            ++offset;
            if (offset >= length)
                break;
            uint8_t esc = data[offset];
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
                // \uXXXX Unicode escape — decode 4 hex digits into a codepoint
                if (offset + 4 >= length) {
                    ++offset;
                    continue;
                }
                uint16_t cp = 0;
                for (int j = 1; j <= 4; ++j) {
                    uint8_t h = data[offset + j];
                    cp <<= 4;
                    if (h >= '0' && h <= '9')
                        cp |= (h - '0');
                    else if (h >= 'a' && h <= 'f')
                        cp |= (h - 'a' + 10);
                    else if (h >= 'A' && h <= 'F')
                        cp |= (h - 'A' + 10);
                }
                offset += 4;
                // Check for UTF-16 surrogate pair: if cp is a high surrogate
                // (0xD800-0xDBFF), look ahead for \uXXXX low surrogate to
                // form a full supplementary-plane codepoint (U+10000 and above).
                if (cp >= 0xD800 && cp <= 0xDBFF && offset + 2 < length &&
                    data[offset + 1] == '\\' && data[offset + 2] == 'u') {
                    offset += 2; // skip past the \u of the low surrogate
                    uint16_t lo = 0;
                    for (int j = 1; j <= 4; ++j) {
                        uint8_t h = data[offset + j];
                        lo <<= 4;
                        if (h >= '0' && h <= '9')
                            lo |= (h - '0');
                        else if (h >= 'a' && h <= 'f')
                            lo |= (h - 'a' + 10);
                        else if (h >= 'A' && h <= 'F')
                            lo |= (h - 'A' + 10);
                    }
                    offset += 4;
                    // Combine high and low surrogates into a full codepoint
                    uint32_t full = 0x10000 + ((uint32_t(cp - 0xD800) << 10) | (lo - 0xDC00));
                    // Encode as 4-byte UTF-8 (U+10000 to U+10FFFF)
                    result += char(0xF0 | (full >> 18));
                    result += char(0x80 | ((full >> 12) & 0x3F));
                    result += char(0x80 | ((full >> 6) & 0x3F));
                    result += char(0x80 | (full & 0x3F));
                } else if (cp < 0x80) {
                    // 1-byte UTF-8 (ASCII range)
                    result += char(cp);
                } else if (cp < 0x800) {
                    // 2-byte UTF-8 (U+0080 to U+07FF)
                    result += char(0xC0 | (cp >> 6));
                    result += char(0x80 | (cp & 0x3F));
                } else {
                    // 3-byte UTF-8 (U+0800 to U+FFFF, excluding surrogates)
                    result += char(0xE0 | (cp >> 12));
                    result += char(0x80 | ((cp >> 6) & 0x3F));
                    result += char(0x80 | (cp & 0x3F));
                }
                break;
            }
            default:
                result += char(esc);
                break;
            }
            ++offset;
        } else {
            // Fast run: scan ahead for the next '"' or '\\' and bulk-append
            // all plain characters in one operation. This avoids per-byte
            // overhead for the common case of unescaped string content.
            size_t run_start = offset;
            while (offset < length && data[offset] != '"' && data[offset] != '\\')
                ++offset;
            result.append(reinterpret_cast<const char*>(data + run_start), offset - run_start);
        }
    }
    return result;
}

// ─── Helper: advance pos_index past a given cursor position ──────────────────

/// Synchronizes the structural-index cursor (pos_index) with the raw-data
/// cursor after a primitive value has been parsed.
///
/// When we parse a primitive value (string, number, bool, null) by scanning
/// raw bytes, the structural index may contain positions that fall within the
/// bytes we just consumed (e.g., a ':' or ',' that was already handled).
/// This function advances pos_index past all structural positions that are
/// behind the current data cursor, keeping the two cursors in sync.
///
/// @param sp         Array of structural byte positions.
/// @param num_sp     Number of structural positions.
/// @param pos_index  Current position in the structural index (updated in-place).
/// @param cursor     Current position in the raw data byte stream.
static void sync_pos_index(const uint32_t* sp, size_t num_sp, size_t& pos_index, size_t cursor) {
    while (pos_index < num_sp && sp[pos_index] < cursor) {
        ++pos_index;
    }
}

// ─── Generic value parser ────────────────────────────────────────────────────

/// Parses a JSON value using the generic (non-speculative) path.
///
/// This function is the fallback entry point when speculation is not used.
/// It examines the byte at the current structural position to determine
/// whether the value is a container (object or array) and delegates
/// accordingly. For container types, the structural char IS the value opener
/// ('{' or '['). For primitives, the structural index points past the value
/// to the next delimiter — those are handled by parse_primitive_at() instead.
///
/// In practice, this function is primarily used for top-level containers.
/// Primitives inside objects/arrays are parsed via parse_value_at(), which
/// receives an explicit data cursor from the caller.
///
/// @param data       Raw JSON byte buffer.
/// @param length     Total buffer length.
/// @param sp         Structural positions array.
/// @param num_sp     Number of structural positions.
/// @param ctx        Parser context for Markov model observation.
/// @param pos_index  Current index into the structural positions array.
/// @return           The parsed JsonValue.

JsonValue SpeculativeParser::parse_value_generic(const uint8_t* data, size_t length,
                                                 const uint32_t* sp, size_t num_sp,
                                                 ParserContext& ctx, size_t& pos_index) {

    // Find value start: skip whitespace from current position
    size_t cursor;
    if (pos_index < num_sp) {
        // The next structural char tells us where we are.
        // If it's a container opener ({ [), the value IS the structural char.
        // If it's something else (} ] , :), the value is between the previous
        // and next structural positions — we need to look at raw data.
        cursor = sp[pos_index];
    } else {
        return JsonValue();
    }

    // For non-container values (numbers, strings, bools, null), the structural
    // index points to the next structural char AFTER the value. We need to
    // look at the raw data before this structural position.
    //
    // However, if the structural char IS a container opener, the value IS that char.
    uint8_t sc = data[cursor];
    if (sc == '{') {
        ValueType actual_type = ValueType::OBJECT;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;
        return parse_object(data, length, sp, num_sp, ctx, pos_index);
    }
    if (sc == '[') {
        ValueType actual_type = ValueType::ARRAY;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;
        return parse_array(data, length, sp, num_sp, ctx, pos_index);
    }

    // The structural position points to a , or } or ] or : that comes AFTER
    // the value we want. We need to find the value in the raw data by scanning
    // backwards or — better — by knowing where the previous consumed position was.
    //
    // Actually, the issue is simpler: for values inside objects/arrays, the
    // caller (parse_object/parse_array) has already positioned us correctly.
    // The value data is between the previous structural char and sp[pos_index].
    // The caller provides the region via the value_cursor parameter.
    //
    // We'll handle this by having parse_object/array pass us a data cursor.
    // For now, this function is only called for top-level values or when
    // the structural char IS the value opener. Primitive values at top level
    // don't have a surrounding structural char in the index.

    // This shouldn't normally be reached for well-structured calls.
    return JsonValue();
}

// ─── Internal: parse a primitive value from a known data position ─────────────

/// Parses a primitive JSON value (string, number, boolean, or null) starting
/// at a known byte position in the raw data.
///
/// This is the workhorse for non-container value parsing, used both as the
/// speculation fallback and for direct parsing when speculation is disabled.
/// It classifies the value by its first byte, then dispatches to the
/// appropriate parser:
///   - '"' → parse_string_at() for string values
///   - 't' → boolean true (literal match)
///   - 'f' → boolean false (literal match)
///   - 'n' → null (literal match)
///   - digit/'-' → parse_number_unified() for numeric values
///
/// After parsing, the function observes the actual value type in the Markov
/// model (if online learning is enabled) and synchronizes the structural
/// index cursor with the data cursor.
///
/// @param data       Raw JSON byte buffer.
/// @param cursor     Byte position where the value starts.
/// @param length     Total buffer length.
/// @param sp         Structural positions array.
/// @param num_sp     Number of structural positions.
/// @param ctx        Parser context for Markov model updates.
/// @param pos_index  Structural index cursor (updated in-place).
/// @return           The parsed JsonValue.

JsonValue SpeculativeParser::parse_primitive_at(const uint8_t* data, size_t cursor, size_t length,
                                                const uint32_t* sp, size_t num_sp,
                                                ParserContext& ctx, size_t& pos_index) {

    uint8_t first = data[cursor];
    ValueType actual_type;

    switch (first) {
    case '"': {
        size_t off = cursor;
        std::string s = parse_string_at(data, off, length);
        actual_type = (s.size() <= 32) ? ValueType::SHORT_STRING : ValueType::LONG_STRING;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;
        sync_pos_index(sp, num_sp, pos_index, off);
        return JsonValue(JsonValue::Variant(std::move(s)));
    }
    case 't':
        actual_type = ValueType::BOOL_TRUE;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;
        return JsonValue(JsonValue::Variant(true));
    case 'f':
        actual_type = ValueType::BOOL_FALSE;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;
        return JsonValue(JsonValue::Variant(false));
    case 'n':
        actual_type = ValueType::NULL_VALUE;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;
        return JsonValue(JsonValue::Variant(nullptr));
    default: {
        // Number — use parse_number_unified which implements fast Clinger/Eisel-Lemire
        // parsing for doubles and direct integer parsing for int64. This unified
        // approach is ~9x faster than the previous strtod-via-buffer-copy method
        // because it avoids string copies and uses hardware-friendly algorithms.
        int64_t int_val;
        double dbl_val;
        size_t consumed;
        bool is_dbl;

        if (util::parse_number_unified(reinterpret_cast<const char*>(data + cursor),
                                       length - cursor, int_val, dbl_val, consumed, is_dbl)) {
            actual_type = is_dbl ? ValueType::FLOAT : ValueType::INTEGER;
            if (config_.enable_online_learning) {
                model_.observe(ctx, actual_type);
                ++values_observed_;
            }
            ctx.last_value_type = actual_type;
            sync_pos_index(sp, num_sp, pos_index, cursor + consumed);

            double val = is_dbl ? dbl_val : static_cast<double>(int_val);
            return JsonValue(JsonValue::Variant(val));
        }

        // Unreachable for valid JSON, but handle gracefully.
        return JsonValue();
    }
    }
}

// ─── Speculative value parser at known data position ─────────────────────────

/// Parses a JSON value at a known data position, using speculation when possible.
///
/// This is the primary value-parsing entry point within containers. It implements
/// the full predict → fast-path → verify → fallback flow:
///
///   1. If the first byte is '{' or '[', delegate to parse_object/parse_array
///      (containers are always handled structurally, never speculatively).
///   2. For primitive values, if speculation is enabled and the model has
///      warmed up (enough values observed), ask the Markov model for a
///      prediction. The model considers the current context (key hash,
///      nesting depth, array index, last value type).
///   3. If the model says "should_speculate", look up the predicted type's
///      fast-path function in the static dispatch table and attempt to parse.
///   4. If the fast-path succeeds (the bytes match the predicted type),
///      record a "hit" and return the value — skipping the generic path.
///   5. If the fast-path fails (type mismatch), fall through to
///      parse_primitive_at() which handles any value type.
///
/// @param data       Raw JSON byte buffer.
/// @param cursor     Byte position where the value starts.
/// @param length     Total buffer length.
/// @param sp         Structural positions array.
/// @param num_sp     Number of structural positions.
/// @param ctx        Parser context (key hash, depth, etc.) for prediction.
/// @param pos_index  Structural index cursor (updated in-place).
/// @return           The parsed JsonValue.

JsonValue SpeculativeParser::parse_value_at(const uint8_t* data, size_t cursor, size_t length,
                                            const uint32_t* sp, size_t num_sp, ParserContext& ctx,
                                            size_t& pos_index) {

    uint8_t first = data[cursor];

    // Container openers are in the structural index — delegate directly
    // (no speculation needed; structural navigation is already optimal)
    if (first == '{') {
        // pos_index should point to this '{'
        sync_pos_index(sp, num_sp, pos_index, cursor);
        if (pos_index < num_sp && sp[pos_index] == cursor) {
            ValueType actual_type = ValueType::OBJECT;
            if (config_.enable_online_learning) {
                model_.observe(ctx, actual_type);
                ++values_observed_;
            }
            ctx.last_value_type = actual_type;
            return parse_object(data, length, sp, num_sp, ctx, pos_index);
        }
    }
    if (first == '[') {
        sync_pos_index(sp, num_sp, pos_index, cursor);
        if (pos_index < num_sp && sp[pos_index] == cursor) {
            ValueType actual_type = ValueType::ARRAY;
            if (config_.enable_online_learning) {
                model_.observe(ctx, actual_type);
                ++values_observed_;
            }
            ctx.last_value_type = actual_type;
            return parse_array(data, length, sp, num_sp, ctx, pos_index);
        }
    }

    // ── Speculation path ──────────────────────────────────────────────────
    // Only attempt speculation after the warmup period (enough values observed
    // to build reliable transition statistics in the Markov model).
    if (config_.enable_speculation && values_observed_ >= config_.online_learning_warmup) {
        // Ask the Markov model: given the current context, what type is most
        // likely next? The model returns both the predicted type and whether
        // confidence exceeds the speculation threshold.
        auto prediction = model_.predict(ctx);
        model_.record_prediction();

        if (prediction.should_speculate) {
            model_.record_speculation_attempted();

            ValueType predicted = prediction.predicted_type;
            // Container types are already handled above — only speculate on primitives
            if (predicted != ValueType::OBJECT && predicted != ValueType::ARRAY) {
                // Look up the type-specific fast-path parser from the static
                // dispatch table. Each ValueType maps to a specialized function
                // that can parse that specific type with minimal branching.
                const auto& dispatch = FastPaths::dispatch_table();
                auto fn = dispatch[static_cast<size_t>(predicted)];
                size_t remaining = length - cursor;
                ParseResult result = (fast_paths_.*fn)(data + cursor, remaining);

                if (result.success) {
                    // Speculation hit: the fast-path confirmed the predicted type
                    model_.record_speculation_hit();
                    ValueType actual = predicted;
                    if ((predicted == ValueType::SHORT_STRING ||
                         predicted == ValueType::LONG_STRING) &&
                        result.value.is_string()) {
                        actual = (result.value.as_string().size() <= 32) ? ValueType::SHORT_STRING
                                                                         : ValueType::LONG_STRING;
                    }
                    if (config_.enable_online_learning) {
                        model_.observe(ctx, actual);
                        ++values_observed_;
                    }
                    if (actual == predicted)
                        model_.record_correct_prediction();
                    ctx.last_value_type = actual;

                    // Advance the structural index past the bytes we consumed
                    size_t new_cursor = cursor + result.bytes_consumed;
                    sync_pos_index(sp, num_sp, pos_index, new_cursor);
                    return std::move(result.value);
                }
                // Speculation miss — the fast-path couldn't parse the value
                // (e.g., predicted INTEGER but found a string). Fall through
                // to the generic parser which handles all types.
            }
        }
    }

    // Generic (non-speculative) parse
    return parse_primitive_at(data, cursor, length, sp, num_sp, ctx, pos_index);
}

// ─── Object parser ───────────────────────────────────────────────────────────

/// Parses a JSON object by navigating the structural index.
///
/// The structural index for an object like {"name":"Alice","age":30} contains:
///   positions: { : , : }
///   pos_index: 0({) → 1(:) → 2(,) → 3(:) → 4(})
///
/// The parsing algorithm walks the structural index sequentially:
///   1. Consume '{' and advance pos_index.
///   2. For each key-value pair:
///      a. The next structural should be ':' — its byte position marks the
///         boundary between the key region and the value region.
///      b. The key is found by scanning raw data between the previous
///         structural position and ':' (look for the quoted string).
///      c. The value starts after ':' + whitespace. It's parsed via
///         parse_value_at() (which may use speculation).
///      d. After the value, the next structural is ',' (more pairs) or
///         '}' (end of object).
///   3. If a Bloom filter key_filter_ is set, unwanted keys are skipped
///      via skip_value_structural() — avoiding full value parsing entirely.
///
/// @param data       Raw JSON byte buffer.
/// @param length     Total buffer length.
/// @param sp         Structural positions array.
/// @param num_sp     Number of structural positions.
/// @param ctx        Parent parser context.
/// @param pos_index  Structural index cursor (updated in-place).
/// @return           A JsonValue containing the parsed object.

JsonValue SpeculativeParser::parse_object(const uint8_t* data, size_t length, const uint32_t* sp,
                                          size_t num_sp, ParserContext& ctx, size_t& pos_index) {

    // pos_index points to '{' — record its byte position and advance
    size_t brace_pos = sp[pos_index];
    ++pos_index;

    JsonValue::Object obj;

    // Create child context for values within this object.
    // The context tracks nesting depth, position, and type history
    // for the Markov prediction model.
    ParserContext child_ctx{};
    child_ctx.nesting_depth = static_cast<uint8_t>(std::min<uint32_t>(ctx.nesting_depth + 1, 255));
    child_ctx.in_array = false;
    child_ctx.array_index = 0;
    child_ctx.last_value_type = ctx.last_value_type;

    // Check for empty object: next structural is '}'
    if (pos_index < num_sp && data[sp[pos_index]] == '}') {
        ++pos_index;
        return JsonValue(JsonValue::Variant(std::move(obj)));
    }

    while (pos_index < num_sp) {
        // Next structural should be ':' (key:value separator).
        // The key is in raw data between the previous structural position and ':'.
        size_t colon_pos = sp[pos_index];
        if (data[colon_pos] != ':') {
            // Unexpected — might be '}' at end
            if (data[colon_pos] == '}') {
                ++pos_index;
                break;
            }
            break;
        }

        // Parse key: scan backwards from colon to find the key string.
        // The key starts after the previous structural char (+ whitespace).
        // Previous structural char was either '{' or ',' — find the key between.
        size_t prev_end = (pos_index > 0) ? sp[pos_index - 1] + 1 : brace_pos + 1;
        size_t key_start = skip_ws(data, prev_end, length);

        if (key_start < length && data[key_start] == '"') {
            size_t key_off = key_start;
            std::string key = parse_string_at(data, key_off, length);
            child_ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>(key.data()), key.size());

            // Advance past ':'
            ++pos_index;

            // Find value start: after ':' + whitespace
            size_t val_start = skip_ws(data, colon_pos + 1, length);
            if (val_start >= length)
                break;

            // ── Selective parsing: skip unwanted keys via Bloom filter ────
            // When a key filter is set, keys not in the filter are skipped
            // entirely — their values are never parsed, just structurally
            // skipped. This is the key optimization for selective parsing:
            // for large objects with many keys, we only pay the cost of
            // parsing the few keys the caller actually needs.
            if (key_filter_ && !key_filter_->should_parse(key)) {
                skip_value_structural(data, length, sp, num_sp, val_start, pos_index);

                // Consume the separator (',' or '}') following the skipped value
                if (pos_index < num_sp) {
                    uint8_t sep = data[sp[pos_index]];
                    if (sep == ',') {
                        ++pos_index;
                        continue;
                    }
                    if (sep == '}') {
                        ++pos_index;
                        break;
                    }
                }
                continue;
            }

            // Parse value
            JsonValue val =
                parse_value_at(data, val_start, length, sp, num_sp, child_ctx, pos_index);
            obj.emplace(std::move(key), std::move(val));

            // Next structural should be ',' or '}'
            if (pos_index < num_sp) {
                uint8_t sep = data[sp[pos_index]];
                if (sep == ',') {
                    ++pos_index;
                    continue;
                }
                if (sep == '}') {
                    ++pos_index;
                    break;
                }
            }
        }
        break;
    }

    return JsonValue(JsonValue::Variant(std::move(obj)));
}

// ─── Array parser ────────────────────────────────────────────────────────────

/// Parses a JSON array by navigating the structural index.
///
/// The structural index for an array like [1,2,"hello"] contains:
///   positions: [ , , ]
///   Values are found in raw data between structural chars.
///
/// Parsing algorithm:
///   1. Consume '[' and advance pos_index.
///   2. Parse the first element (between '[' and the next structural char).
///   3. For subsequent elements: expect ',' or ']' at each structural position.
///      If ',', parse the value between it and the next structural char.
///      If ']', the array is complete.
///
/// Optimization — homogeneous numeric array fast path:
///   After parsing the first element, if it was a number, the parser enters
///   a tight loop that calls parse_number_unified() directly for subsequent
///   elements. This bypasses all Markov model predict/observe overhead, which
///   is significant for arrays with thousands of numeric elements (e.g., the
///   wide_arrays benchmark). The fast path exits as soon as a non-numeric
///   element is encountered.
///
/// @param data       Raw JSON byte buffer.
/// @param length     Total buffer length.
/// @param sp         Structural positions array.
/// @param num_sp     Number of structural positions.
/// @param ctx        Parent parser context.
/// @param pos_index  Structural index cursor (updated in-place).
/// @return           A JsonValue containing the parsed array.

JsonValue SpeculativeParser::parse_array(const uint8_t* data, size_t length, const uint32_t* sp,
                                         size_t num_sp, ParserContext& ctx, size_t& pos_index) {

    size_t bracket_pos = sp[pos_index];
    ++pos_index; // skip '['

    JsonValue::Array arr;

    // Create child context for elements within this array.
    // key_hash is 0 (arrays have no keys); array_index tracks the element position.
    ParserContext child_ctx{};
    child_ctx.nesting_depth = static_cast<uint8_t>(std::min<uint32_t>(ctx.nesting_depth + 1, 255));
    child_ctx.in_array = true;
    child_ctx.key_hash = 0;
    child_ctx.array_index = 0;
    child_ctx.last_value_type = ctx.last_value_type;

    // Check for empty array
    if (pos_index < num_sp && data[sp[pos_index]] == ']') {
        ++pos_index;
        return JsonValue(JsonValue::Variant(std::move(arr)));
    }

    // Flag to enable the homogeneous numeric array fast path.
    // Set to true after the first element if it's a number. If any subsequent
    // element is not a number, the flag is cleared and the normal path resumes.
    bool array_fast_numbers = false;

    // Parse first element: between '[' and next structural (which is ',' or ']')
    {
        size_t val_start = skip_ws(data, bracket_pos + 1, length);
        if (val_start >= length)
            goto done;

        JsonValue val = parse_value_at(data, val_start, length, sp, num_sp, child_ctx, pos_index);
        arr.push_back(std::move(val));
        child_ctx.array_index =
            static_cast<uint8_t>(std::min<uint32_t>(child_ctx.array_index + 1, 255));
        array_fast_numbers = arr.back().is_number();
    }

    while (pos_index < num_sp) {
        uint8_t sep = data[sp[pos_index]];
        if (sep == ']') {
            ++pos_index;
            goto done;
        }
        if (sep != ',')
            break;

        size_t comma_pos = sp[pos_index];
        ++pos_index;

        // Value starts after comma + whitespace
        size_t val_start = skip_ws(data, comma_pos + 1, length);
        if (val_start >= length)
            break;

        // Fast path for homogeneous numeric arrays: bypass the full
        // parse_value_at() pipeline (Markov predict + observe + dispatch)
        // and call parse_number_unified() directly. The first-byte check
        // ensures we only use this path for actual numeric values.
        if (array_fast_numbers) {
            uint8_t fc = data[val_start];
            if (fc == '-' || (fc >= '0' && fc <= '9')) {
                int64_t int_val;
                double dbl_val;
                size_t consumed;
                bool is_dbl;
                if (util::parse_number_unified(reinterpret_cast<const char*>(data + val_start),
                                               length - val_start, int_val, dbl_val, consumed,
                                               is_dbl)) {
                    double val = is_dbl ? dbl_val : static_cast<double>(int_val);
                    arr.push_back(JsonValue(JsonValue::Variant(val)));
                    sync_pos_index(sp, num_sp, pos_index, val_start + consumed);
                    ++values_observed_;
                    child_ctx.array_index =
                        static_cast<uint8_t>(std::min<uint32_t>(child_ctx.array_index + 1, 255));
                    continue;
                }
            }
            // Not a number — exit fast path for this array.
            array_fast_numbers = false;
        }

        JsonValue val = parse_value_at(data, val_start, length, sp, num_sp, child_ctx, pos_index);
        arr.push_back(std::move(val));
        child_ctx.array_index =
            static_cast<uint8_t>(std::min<uint32_t>(child_ctx.array_index + 1, 255));
    }

done:
    return JsonValue(JsonValue::Variant(std::move(arr)));
}

// ─── Top-level parse ─────────────────────────────────────────────────────────

/// Entry point for parsing a single JSON document.
///
/// This is the main public interface for the speculative parser. It accepts
/// the raw JSON data and a pre-computed structural index (produced by the
/// SIMD-based stage 1 scanner), then initiates recursive descent parsing
/// with speculation support.
///
/// The function initializes a root parser context with default values
/// (nesting depth 0, no key, no array context) and delegates to
/// parse_value_at() which implements the full speculation pipeline.
///
/// The structural index (`sp`) must have been computed by the stage 1
/// scanner and contains byte offsets of all { } [ ] : , characters
/// outside of strings.
///
/// @param data    Raw JSON byte buffer.
/// @param length  Total buffer length.
/// @param sp      Structural positions from the stage 1 scanner.
/// @param num_sp  Number of structural positions.
/// @return        The parsed JsonValue representing the entire document.

JsonValue SpeculativeParser::parse(const uint8_t* data, size_t length, const uint32_t* sp,
                                   size_t num_sp) {

    if (num_sp == 0 || length == 0)
        return JsonValue();

    // Initialize the root-level parser context — this is the starting point
    // for the Markov model's context chain.
    ParserContext root_ctx{};
    root_ctx.key_hash = 0;
    root_ctx.nesting_depth = 0;
    root_ctx.array_index = 0;
    root_ctx.last_value_type = ValueType::NULL_VALUE;
    root_ctx.in_array = false;

    size_t pos_index = 0;

    // Top-level value: skip any leading whitespace before the document.
    // For most JSON, the first non-whitespace byte is '{' or '[', which
    // will be at sp[0]. For rare top-level primitives, we start from byte 0.
    size_t cursor = skip_ws(data, 0, length);
    if (cursor >= length)
        return JsonValue();

    return parse_value_at(data, cursor, length, sp, num_sp, root_ctx, pos_index);
}

// ─── NDJSON parse ────────────────────────────────────────────────────────────

/// Parses a newline-delimited JSON (NDJSON) stream into a vector of values.
///
/// NDJSON format contains one JSON document per line, separated by newlines.
/// This function iterates through the buffer, parsing each document
/// independently with its own fresh parser context. The structural index
/// spans the entire buffer and is shared across all documents; pos_index
/// advances continuously through it.
///
/// After parsing each document, the cursor is advanced past the consumed
/// structural chars and any trailing newlines to find the start of the
/// next document.
///
/// @param data    Raw NDJSON byte buffer containing multiple JSON documents.
/// @param length  Total buffer length.
/// @param sp      Structural positions for the entire buffer.
/// @param num_sp  Number of structural positions.
/// @return        Vector of parsed JsonValues, one per line/document.

std::vector<JsonValue> SpeculativeParser::parse_ndjson(const uint8_t* data, size_t length,
                                                       const uint32_t* sp, size_t num_sp) {

    std::vector<JsonValue> results;
    if (num_sp == 0 || length == 0)
        return results;

    size_t pos_index = 0;
    size_t cursor = 0;

    // Process documents one at a time until the buffer is exhausted
    while (cursor < length && pos_index <= num_sp) {
        cursor = skip_ws(data, cursor, length);
        if (cursor >= length)
            break;

        ParserContext root_ctx{};
        root_ctx.key_hash = 0;
        root_ctx.nesting_depth = 0;
        root_ctx.array_index = 0;
        root_ctx.last_value_type = ValueType::NULL_VALUE;
        root_ctx.in_array = false;

        size_t old_pos_index = pos_index;
        JsonValue doc = parse_value_at(data, cursor, length, sp, num_sp, root_ctx, pos_index);
        results.push_back(std::move(doc));

        // Advance cursor past this document.
        // The pos_index has been advanced past all structural chars in this doc.
        // Find the next document start after the last consumed structural.
        if (pos_index > old_pos_index && pos_index <= num_sp) {
            // Cursor is after the last structural char we consumed
            size_t last_sp = sp[pos_index - 1];
            cursor = last_sp + 1;
        } else {
            // No structural chars consumed — primitive top-level value.
            // Advance cursor past the value.
            cursor = skip_ws(data, cursor, length);
            // Skip past the primitive
            while (cursor < length && data[cursor] != '\n' && data[cursor] != '\r')
                ++cursor;
        }
        // Skip newlines between documents
        while (cursor < length && (data[cursor] == '\n' || data[cursor] == '\r'))
            ++cursor;
    }

    return results;
}

// ─── Structural value skip (for selective parsing) ──────────────────────────

/// Skips a JSON value without parsing it, using only the structural index.
///
/// This is the key optimization for selective parsing: when a key is not in
/// the desired set, we skip its value by navigating structural positions
/// rather than parsing the actual bytes. This is O(structural_chars) for
/// containers and O(1) for primitives.
///
/// For container values ({ or [):
///   1. Sync pos_index to the container's opening brace/bracket.
///   2. Use a depth counter initialized to 1.
///   3. Walk through structural positions: increment depth on '{' or '[',
///      decrement on '}' or ']'. When depth reaches 0, the entire nested
///      container has been skipped.
///   This correctly handles arbitrarily nested structures without any
///   allocation or string processing.
///
/// For primitive values (string, number, bool, null):
///   The structural index does not contain positions for primitives — they
///   live in the raw data between structural chars. Since pos_index already
///   points to the next structural char after the value (i.e., ',' or '}'
///   or ']'), no advancement is needed.
///
/// @param data       Raw JSON byte buffer (only used to check byte values
///                   at structural positions).
/// @param length     Total buffer length (unused but kept for API consistency).
/// @param sp         Structural positions array.
/// @param num_sp     Number of structural positions.
/// @param val_start  Byte position where the value to skip begins.
/// @param pos_index  Structural index cursor (updated in-place to point
///                   past the skipped value).

void SpeculativeParser::skip_value_structural(const uint8_t* data, size_t /*length*/,
                                              const uint32_t* sp, size_t num_sp, size_t val_start,
                                              size_t& pos_index) noexcept {
    uint8_t first = data[val_start];

    if (first == '{' || first == '[') {
        // Ensure pos_index is aligned with the container opener
        while (pos_index < num_sp && sp[pos_index] < val_start)
            ++pos_index;

        // Brace-counting skip: walk structural positions tracking nesting depth.
        // Every opener increments depth; every closer decrements it.
        // When depth returns to 0, the matching closer has been found.
        int depth = 1;
        ++pos_index; // skip the opener itself
        while (pos_index < num_sp && depth > 0) {
            uint8_t c = data[sp[pos_index]];
            if (c == '{' || c == '[')
                ++depth;
            else if (c == '}' || c == ']')
                --depth;
            ++pos_index;
        }
        return;
    }

    // Primitive value (string, number, bool, null):
    // Primitives are not in the structural index, so pos_index already
    // points to the delimiter following this value (',' or '}' or ']').
    // No advancement is necessary.
}

} // namespace strata::speculative

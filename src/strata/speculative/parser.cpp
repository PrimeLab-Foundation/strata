// =============================================================================
// strata/speculative/parser.cpp
//
// Speculative parser coordinator implementation.
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
#include <string>

namespace strata::speculative {

// ─── Construction ────────────────────────────────────────────────────────────

SpeculativeParser::SpeculativeParser(const Config& config, strata::util::Arena& arena)
    : config_(config), model_(), fast_paths_(arena), arena_(arena) {}

// ─── Whitespace skip ─────────────────────────────────────────────────────────

size_t SpeculativeParser::skip_ws(const uint8_t* data, size_t offset, size_t length) noexcept {
    while (offset < length) {
        uint8_t c = data[offset];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
        ++offset;
    }
    return offset;
}

// ─── Value type classification ───────────────────────────────────────────────

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
        return ValueType::INTEGER;
    }
}

// ─── String parser helper ────────────────────────────────────────────────────

std::string SpeculativeParser::parse_string_at(const uint8_t* data, size_t& offset, size_t length) {
    ++offset; // skip opening '"'
    std::string result;
    result.reserve(32);

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
                if (cp >= 0xD800 && cp <= 0xDBFF && offset + 2 < length &&
                    data[offset + 1] == '\\' && data[offset + 2] == 'u') {
                    offset += 2;
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
                    uint32_t full = 0x10000 + ((uint32_t(cp - 0xD800) << 10) | (lo - 0xDC00));
                    result += char(0xF0 | (full >> 18));
                    result += char(0x80 | ((full >> 12) & 0x3F));
                    result += char(0x80 | ((full >> 6) & 0x3F));
                    result += char(0x80 | (full & 0x3F));
                } else if (cp < 0x80) {
                    result += char(cp);
                } else if (cp < 0x800) {
                    result += char(0xC0 | (cp >> 6));
                    result += char(0x80 | (cp & 0x3F));
                } else {
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
            size_t run_start = offset;
            while (offset < length && data[offset] != '"' && data[offset] != '\\')
                ++offset;
            result.append(reinterpret_cast<const char*>(data + run_start), offset - run_start);
        }
    }
    return result;
}

// ─── Helper: advance pos_index past a given cursor position ──────────────────
// After parsing a value (string, number, etc.), we need to advance the
// structural index cursor past any structural positions that fall within
// the bytes we've consumed.
static void sync_pos_index(const uint32_t* sp, size_t num_sp, size_t& pos_index, size_t cursor) {
    while (pos_index < num_sp && sp[pos_index] < cursor) {
        ++pos_index;
    }
}

// ─── Generic value parser ────────────────────────────────────────────────────
//
// Parses a JSON value starting at `cursor` in the raw data.
// Uses the structural index for navigating containers, but finds
// primitive values by scanning raw bytes.

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
// This is the workhorse for parsing numbers, strings, bools, and null
// when we know exactly where in the raw data the value starts.

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
        // Number
        size_t nstart = cursor;
        size_t npos = cursor;
        bool is_float = false;

        if (npos < length && data[npos] == '-')
            ++npos;
        while (npos < length && data[npos] >= '0' && data[npos] <= '9')
            ++npos;
        if (npos < length && data[npos] == '.') {
            is_float = true;
            ++npos;
            while (npos < length && data[npos] >= '0' && data[npos] <= '9')
                ++npos;
        }
        if (npos < length && (data[npos] == 'e' || data[npos] == 'E')) {
            is_float = true;
            ++npos;
            if (npos < length && (data[npos] == '+' || data[npos] == '-'))
                ++npos;
            while (npos < length && data[npos] >= '0' && data[npos] <= '9')
                ++npos;
        }

        actual_type = is_float ? ValueType::FLOAT : ValueType::INTEGER;
        if (config_.enable_online_learning) {
            model_.observe(ctx, actual_type);
            ++values_observed_;
        }
        ctx.last_value_type = actual_type;

        char buf[64];
        size_t nlen = std::min(npos - nstart, size_t{63});
        std::memcpy(buf, data + nstart, nlen);
        buf[nlen] = '\0';
        double val = std::strtod(buf, nullptr);
        sync_pos_index(sp, num_sp, pos_index, npos);

        return JsonValue(JsonValue::Variant(val));
    }
    }
}

// ─── Speculative value parser at known data position ─────────────────────────

JsonValue SpeculativeParser::parse_value_at(const uint8_t* data, size_t cursor, size_t length,
                                            const uint32_t* sp, size_t num_sp, ParserContext& ctx,
                                            size_t& pos_index) {

    uint8_t first = data[cursor];

    // Container openers are in the structural index — delegate
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

    // Primitive value — try speculation first
    if (config_.enable_speculation && values_observed_ >= config_.online_learning_warmup) {
        auto prediction = model_.predict(ctx);
        model_.record_prediction();

        if (prediction.should_speculate) {
            model_.record_speculation_attempted();

            ValueType predicted = prediction.predicted_type;
            // Don't try fast-path for containers (handled above)
            if (predicted != ValueType::OBJECT && predicted != ValueType::ARRAY) {
                const auto& dispatch = FastPaths::dispatch_table();
                auto fn = dispatch[static_cast<size_t>(predicted)];
                size_t remaining = length - cursor;
                ParseResult result = (fast_paths_.*fn)(data + cursor, remaining);

                if (result.success) {
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

                    size_t new_cursor = cursor + result.bytes_consumed;
                    sync_pos_index(sp, num_sp, pos_index, new_cursor);
                    return std::move(result.value);
                }
                // Miss — fall through to generic
            }
        }
    }

    // Generic (non-speculative) parse
    return parse_primitive_at(data, cursor, length, sp, num_sp, ctx, pos_index);
}

// ─── Object parser ───────────────────────────────────────────────────────────
//
// Walking the structural index for an object like {"name":"Alice","age":30}:
//   structural positions: { : , : }
//   pos_index progression: 0({) → 1(:) → 2(,) → 3(:) → 4(})
//
// Keys and values are found in the raw data between structural chars.

JsonValue SpeculativeParser::parse_object(const uint8_t* data, size_t length, const uint32_t* sp,
                                          size_t num_sp, ParserContext& ctx, size_t& pos_index) {

    // pos_index points to '{' — record its position and advance
    size_t brace_pos = sp[pos_index];
    ++pos_index;

    JsonValue::Object obj;

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
            if (key_filter_ && !key_filter_->should_parse(key)) {
                skip_value_structural(data, length, sp, num_sp, val_start, pos_index);

                // Consume separator (, or })
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
//
// Walking the structural index for an array like [1,2,"hello"]:
//   structural positions: [ , , ]
//   Values are found in raw data between structural chars.

JsonValue SpeculativeParser::parse_array(const uint8_t* data, size_t length, const uint32_t* sp,
                                         size_t num_sp, ParserContext& ctx, size_t& pos_index) {

    size_t bracket_pos = sp[pos_index];
    ++pos_index; // skip '['

    JsonValue::Array arr;

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

    // Parse first element: between '[' and next structural (which is ',' or ']')
    {
        size_t val_start = skip_ws(data, bracket_pos + 1, length);
        if (val_start >= length)
            goto done;

        JsonValue val = parse_value_at(data, val_start, length, sp, num_sp, child_ctx, pos_index);
        arr.push_back(std::move(val));
        child_ctx.array_index =
            static_cast<uint8_t>(std::min<uint32_t>(child_ctx.array_index + 1, 255));
    }

    // Parse remaining elements
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

        JsonValue val = parse_value_at(data, val_start, length, sp, num_sp, child_ctx, pos_index);
        arr.push_back(std::move(val));
        child_ctx.array_index =
            static_cast<uint8_t>(std::min<uint32_t>(child_ctx.array_index + 1, 255));
    }

done:
    return JsonValue(JsonValue::Variant(std::move(arr)));
}

// ─── Top-level parse ─────────────────────────────────────────────────────────

JsonValue SpeculativeParser::parse(const uint8_t* data, size_t length, const uint32_t* sp,
                                   size_t num_sp) {

    if (num_sp == 0 || length == 0)
        return JsonValue();

    ParserContext root_ctx{};
    root_ctx.key_hash = 0;
    root_ctx.nesting_depth = 0;
    root_ctx.array_index = 0;
    root_ctx.last_value_type = ValueType::NULL_VALUE;
    root_ctx.in_array = false;

    size_t pos_index = 0;

    // Top-level value: if it's a structural char, it'll be at sp[0].
    // If it's a primitive (rare for top-level), find it from byte 0.
    size_t cursor = skip_ws(data, 0, length);
    if (cursor >= length)
        return JsonValue();

    return parse_value_at(data, cursor, length, sp, num_sp, root_ctx, pos_index);
}

// ─── NDJSON parse ────────────────────────────────────────────────────────────

std::vector<JsonValue> SpeculativeParser::parse_ndjson(const uint8_t* data, size_t length,
                                                       const uint32_t* sp, size_t num_sp) {

    std::vector<JsonValue> results;
    if (num_sp == 0 || length == 0)
        return results;

    size_t pos_index = 0;
    size_t cursor = 0;

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
//
// Skips a JSON value by inspecting the structural index only.
// For containers ({, [): counts depth until matching close.
// For primitives: advances pos_index until the next , or } or ].

void SpeculativeParser::skip_value_structural(const uint8_t* data, size_t /*length*/,
                                              const uint32_t* sp, size_t num_sp, size_t val_start,
                                              size_t& pos_index) noexcept {
    uint8_t first = data[val_start];

    if (first == '{' || first == '[') {
        // Sync pos_index to the container opener.
        while (pos_index < num_sp && sp[pos_index] < val_start)
            ++pos_index;

        // Count depth through the structural index.
        int depth = 1;
        ++pos_index; // skip opener
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
    // The structural index doesn't include primitive positions.
    // pos_index already points to the next structural char after ':',
    // which should be ',' or '}' or ']' — no advancement needed.
}

} // namespace strata::speculative

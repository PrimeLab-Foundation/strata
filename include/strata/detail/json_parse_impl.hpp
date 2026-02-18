/**
 * @file json_parse_impl.hpp
 * @brief Internal detail header: template SAX parser for static dispatch.
 *
 * This header provides a template version of the core JSON parser so that
 * callers with a known-concrete handler type (e.g. PythonObjectBuilder) can
 * bypass virtual dispatch entirely.  All handler->on_*() calls become direct,
 * inlinable method calls when Handler is a concrete class.
 *
 * Usage (from python_loads.cpp):
 *   #include "strata/detail/json_parse_impl.hpp"
 *   strata::detail::parse_sax_impl_t(text, my_concrete_handler, opts, ctx);
 *
 * The public API in json_parse.hpp is unchanged; json_parse.cpp instantiates
 * parse_sax_impl_t<JsonSaxHandler> to preserve the virtual-dispatch path for
 * all other callers (DomBuilderHandler, TapeBuilder, search handlers, …).
 *
 * @internal
 */

#pragma once

#include "strata/json/json_parse.hpp"
#include "strata/util/fast_parse.hpp"
#include "strata/util/lazy_string.hpp"
#include "strata/util/simd_string.hpp"

#include <cctype>
#include <cstdint>
#include <cstring>
#include <vector>

namespace strata {
namespace detail {

// ---------------------------------------------------------------------------
// Shared parser infrastructure (same constants as json_parse.cpp)
// ---------------------------------------------------------------------------

constexpr size_t kMaxNestingDepth = 10000;
constexpr size_t kAsciiSwarPrefixMax = 128;

enum class ContainerType : uint8_t { Array, Object };
enum class ContainerState : uint8_t {
    ExpectValue,
    ExpectCommaOrEnd,
    ExpectKey,
    ExpectColon,
};

struct StackFrame {
    ContainerType type;
    ContainerState state;
};

inline bool is_ascii_prefix_swar_detail(const char* data, size_t len) {
    if (!data || len == 0) {
        return true;
    }
    size_t i = 0;
    uint64_t mask = 0;
    for (; i + sizeof(uint64_t) <= len; i += sizeof(uint64_t)) {
        uint64_t chunk = 0;
        std::memcpy(&chunk, data + i, sizeof(uint64_t));
        mask |= chunk;
    }
    if (mask & 0x8080808080808080ULL) {
        return false;
    }
    for (; i < len; ++i) {
        if (static_cast<unsigned char>(data[i]) & 0x80) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Template Parser<Handler>
//
// Handler must provide the same interface as JsonSaxHandler, but calls are
// made directly (no virtual dispatch) when Handler is a concrete type.
// ---------------------------------------------------------------------------

template <typename Handler>
struct Parser {
    const char* data;
    size_t len;
    Handler& handler;
    size_t i = 0;
    std::vector<StackFrame> stack_;
    const std::vector<size_t>* structural_tape = nullptr;
    size_t tape_idx = 0;
    const std::vector<size_t>* size_hints = nullptr;
    size_t size_hint_idx = 0;
    bool aborted_ = false;
    bool use_size_hints = true;
    bool use_array_size_hints = true;
    bool use_object_size_hints = true;
    bool use_exact_size_hints = false;

    bool aborted() const { return aborted_; }

    bool call_handler(bool ok) {
        if (!ok) {
            aborted_ = true;
        }
        return ok;
    }

    bool eof() const { return i >= len; }
    char peek() const { return eof() ? '\0' : data[i]; }
    char get() { return eof() ? '\0' : data[i++]; }

    void attach_structural_tape(const std::vector<size_t>* tape) {
        structural_tape = tape;
        tape_idx = 0;
    }

    void attach_size_hints(const std::vector<size_t>* hints) {
        size_hints = hints;
        size_hint_idx = 0;
    }

    void sync_structural_tape() {
        if (!structural_tape) {
            return;
        }
        while (tape_idx < structural_tape->size() && (*structural_tape)[tape_idx] < i) {
            ++tape_idx;
        }
    }

    size_t next_structural_pos() {
        if (!structural_tape) {
            return len;
        }
        sync_structural_tape();
        if (tape_idx >= structural_tape->size()) {
            return len;
        }
        return (*structural_tape)[tape_idx];
    }

    bool next_structural_char(size_t& pos, char& c) {
        if (structural_tape) {
            size_t non_ws = util::skip_whitespace_simd(data, len, i);
            if (non_ws >= len) {
                return false;
            }
            size_t next = next_structural_pos();
            if (next != non_ws) {
                return false;
            }
            pos = next;
            c = data[pos];
            return true;
        }
        pos = util::find_next_structural_simd(data, len, i);
        if (pos >= len) {
            return false;
        }
        c = data[pos];
        return true;
    }

    void consume_structural_at(size_t pos) {
        i = pos + 1;
        if (!structural_tape) {
            return;
        }
        sync_structural_tape();
    }

    static constexpr size_t kHintMaxScan = 512;
    static constexpr size_t kHintMaxRoot = 131072;
    static constexpr size_t kHintMaxNested = 8192;

    size_t skip_ws_local(size_t pos) const {
        return util::skip_whitespace_fast(data, len, pos);
    }

    bool skip_string_local(size_t& pos, size_t limit) const {
        if (pos >= limit || data[pos] != '"') {
            return false;
        }
        ++pos;
        while (pos < limit) {
            unsigned char c = static_cast<unsigned char>(data[pos++]);
            if (c == '"') {
                return true;
            }
            if (c == '\\') {
                if (pos >= limit) {
                    return false;
                }
                unsigned char esc = static_cast<unsigned char>(data[pos++]);
                switch (esc) {
                case '"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    break;
                case 'u':
                    if (pos + 4 > limit) {
                        return false;
                    }
                    pos += 4;
                    break;
                default:
                    return false;
                }
            } else if (c < 0x20) {
                return false;
            }
        }
        return false;
    }

    bool skip_literal_local(size_t& pos, size_t limit, const char* literal,
                            size_t literal_len) const {
        if (pos + literal_len > limit) {
            return false;
        }
        if (std::memcmp(data + pos, literal, literal_len) != 0) {
            return false;
        }
        pos += literal_len;
        return true;
    }

    bool skip_number_local(size_t& pos, size_t limit) const {
        const size_t start = pos;
        if (start >= limit) {
            return false;
        }
        const size_t max_len = limit - start;
        int64_t int_val;
        size_t consumed = 0;
        if (util::parse_int_fast(data + start, max_len, int_val, consumed)) {
            if (start + consumed < limit &&
                (data[start + consumed] == '.' || data[start + consumed] == 'e' ||
                 data[start + consumed] == 'E')) {
                // Fall through to double parse.
            } else {
                pos = start + consumed;
                return true;
            }
        }

        if (data[start] != '-') {
            uint64_t uint_val;
            size_t consumed_uint = 0;
            if (util::parse_uint_fast(data + start, max_len, uint_val, consumed_uint)) {
                if (start + consumed_uint < limit &&
                    (data[start + consumed_uint] == '.' || data[start + consumed_uint] == 'e' ||
                     data[start + consumed_uint] == 'E')) {
                    // Fall through to double parse.
                } else {
                    pos = start + consumed_uint;
                    return true;
                }
            }
        }

        double double_val;
        if (util::parse_double_fast(data + start, max_len, double_val, consumed)) {
            pos = start + consumed;
            return true;
        }
        return false;
    }

    bool skip_scalar_local(size_t& pos, size_t limit) const {
        if (pos >= limit) {
            return false;
        }
        char c = data[pos];
        if (c == '"') {
            return skip_string_local(pos, limit);
        }
        if (c == '-' || (c >= '0' && c <= '9')) {
            return skip_number_local(pos, limit);
        }
        if (c == 'n') {
            return skip_literal_local(pos, limit, "null", 4);
        }
        if (c == 't') {
            return skip_literal_local(pos, limit, "true", 4);
        }
        if (c == 'f') {
            return skip_literal_local(pos, limit, "false", 5);
        }
        return false;
    }

    size_t estimate_container_size_hint(size_t start_pos, char closing, size_t max_hint) const {
        size_t limit = start_pos + kHintMaxScan;
        if (limit > len) {
            limit = len;
        }
        size_t pos = skip_ws_local(start_pos);
        if (pos >= limit) {
            return 0;
        }
        if (data[pos] == closing) {
            return 0;
        }

        size_t count = 1;
        bool in_string = false;
        bool escape = false;
        int depth = 0;

        for (; pos < limit; ++pos) {
            char c = data[pos];
            if (in_string) {
                if (escape) {
                    escape = false;
                    continue;
                }
                if (c == '\\') {
                    escape = true;
                    continue;
                }
                if (c == '"') {
                    in_string = false;
                }
                continue;
            }

            if (depth == 0) {
                if (c == '"' || c == '-' || (c >= '0' && c <= '9') || c == 'n' || c == 't' ||
                    c == 'f') {
                    size_t scalar_pos = pos;
                    if (skip_scalar_local(scalar_pos, limit)) {
                        pos = (scalar_pos > 0) ? (scalar_pos - 1) : scalar_pos;
                        continue;
                    }
                }
            }

            if (c == '"') {
                in_string = true;
                continue;
            }
            if (c == '{' || c == '[') {
                ++depth;
                continue;
            }
            if (c == '}' || c == ']') {
                if (depth == 0) {
                    size_t exact = count;
                    if (exact > max_hint) {
                        exact = max_hint;
                    }
                    return exact;
                }
                --depth;
                continue;
            }
            if (c == ',' && depth == 0) {
                ++count;
                if (count >= max_hint) {
                    return max_hint;
                }
            }
        }

        size_t scanned = pos - start_pos;
        if (scanned == 0) {
            return 0;
        }
        size_t avg = scanned / count;
        if (avg == 0) {
            return count > max_hint ? max_hint : count;
        }
        size_t remaining = len - start_pos;
        size_t estimate = remaining / avg;
        if (estimate == 0) {
            estimate = 1;
        }
        if (estimate > max_hint) {
            estimate = max_hint;
        }
        return estimate;
    }

    size_t estimate_array_size_hint(size_t start_pos, size_t max_hint) const {
        return estimate_container_size_hint(start_pos, ']', max_hint);
    }

    size_t estimate_object_size_hint(size_t start_pos, size_t max_hint) const {
        return estimate_container_size_hint(start_pos, '}', max_hint);
    }

    static int hex_value(char c) {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        return -1;
    }

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

    void skip_ws() { i = util::skip_whitespace_simd(data, len, i); }

    bool consume(char c) {
        size_t pos = 0;
        char actual = '\0';
        if (!next_structural_char(pos, actual)) {
            return false;
        }
        if (actual == c) {
            consume_structural_at(pos);
            return true;
        }
        return false;
    }

    bool parse_primitive() {
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
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c)))
            return parse_number();
        return false;
    }

    bool parse_value() {
        skip_ws();
        if (eof())
            return false;

        char c = peek();
        if (c == '[') {
            if (!start_array())
                return false;
        } else if (c == '{') {
            if (!start_object())
                return false;
        } else {
            return parse_primitive();
        }

        while (!stack_.empty()) {
            StackFrame& frame = stack_.back();

            if (frame.type == ContainerType::Array) {
                if (!process_array_state(frame))
                    return false;
            } else {
                if (!process_object_state(frame))
                    return false;
            }
        }

        return true;
    }

    bool start_array() {
        if (stack_.size() >= kMaxNestingDepth)
            return false;
        ++i;
        size_t size_hint = 0;
        bool used_exact = false;
        if (use_exact_size_hints && size_hints && size_hint_idx < size_hints->size()) {
            size_hint = (*size_hints)[size_hint_idx++];
            used_exact = true;
        }
        if (!used_exact && use_size_hints && use_array_size_hints) {
            size_t max_hint = stack_.empty() ? kHintMaxRoot : kHintMaxNested;
            size_hint = estimate_array_size_hint(i, max_hint);
        }
        if (!call_handler(handler.on_start_array(size_hint)))
            return false;
        skip_ws();
        if (peek() == ']') {
            ++i;
            return call_handler(handler.on_end_array());
        }
        stack_.push_back({ContainerType::Array, ContainerState::ExpectValue});
        return true;
    }

    bool start_object() {
        if (stack_.size() >= kMaxNestingDepth)
            return false;
        ++i;
        size_t size_hint = 0;
        bool used_exact = false;
        if (use_exact_size_hints && size_hints && size_hint_idx < size_hints->size()) {
            size_hint = (*size_hints)[size_hint_idx++];
            used_exact = true;
        }
        if (!used_exact && use_size_hints && use_object_size_hints) {
            size_t max_hint = stack_.empty() ? kHintMaxRoot : kHintMaxNested;
            size_hint = estimate_object_size_hint(i, max_hint);
        }
        if (!call_handler(handler.on_start_object(size_hint)))
            return false;
        skip_ws();
        if (peek() == '}') {
            ++i;
            return call_handler(handler.on_end_object());
        }
        stack_.push_back({ContainerType::Object, ContainerState::ExpectKey});
        return true;
    }

    bool process_array_state(StackFrame& frame) {
        switch (frame.state) {
        case ContainerState::ExpectValue: {
            skip_ws();
            char c = peek();
            if (c == '[') {
                frame.state = ContainerState::ExpectCommaOrEnd;
                return start_array();
            } else if (c == '{') {
                frame.state = ContainerState::ExpectCommaOrEnd;
                return start_object();
            } else {
                if (!parse_primitive())
                    return false;
                frame.state = ContainerState::ExpectCommaOrEnd;
                return true;
            }
        }
        case ContainerState::ExpectCommaOrEnd: {
            size_t pos = 0;
            char c = '\0';
            if (!next_structural_char(pos, c)) {
                return false;
            }
            if (c == ']') {
                consume_structural_at(pos);
                stack_.pop_back();
                return call_handler(handler.on_end_array());
            } else if (c == ',') {
                consume_structural_at(pos);
                frame.state = ContainerState::ExpectValue;
                return true;
            }
            return false;
        }
        default:
            return false;
        }
    }

    bool process_object_state(StackFrame& frame) {
        switch (frame.state) {
        case ContainerState::ExpectKey: {
            skip_ws();
            if (!parse_string(true))
                return false;
            frame.state = ContainerState::ExpectColon;
            return true;
        }
        case ContainerState::ExpectColon: {
            if (!consume(':'))
                return false;
            frame.state = ContainerState::ExpectValue;
            return true;
        }
        case ContainerState::ExpectValue: {
            skip_ws();
            char c = peek();
            if (c == '[') {
                frame.state = ContainerState::ExpectCommaOrEnd;
                return start_array();
            } else if (c == '{') {
                frame.state = ContainerState::ExpectCommaOrEnd;
                return start_object();
            } else {
                if (!parse_primitive())
                    return false;
                frame.state = ContainerState::ExpectCommaOrEnd;
                return true;
            }
        }
        case ContainerState::ExpectCommaOrEnd: {
            size_t pos = 0;
            char c = '\0';
            if (!next_structural_char(pos, c)) {
                return false;
            }
            if (c == '}') {
                consume_structural_at(pos);
                stack_.pop_back();
                return call_handler(handler.on_end_object());
            } else if (c == ',') {
                consume_structural_at(pos);
                frame.state = ContainerState::ExpectKey;
                return true;
            }
            return false;
        }
        }
        return false;
    }

    bool parse_null() {
        if (i + 4 <= len && data[i] == 'n' && data[i + 1] == 'u' && data[i + 2] == 'l' &&
            data[i + 3] == 'l') {
            i += 4;
            return call_handler(handler.on_null());
        }
        return false;
    }

    bool parse_bool() {
        if (i + 4 <= len && data[i] == 't' && data[i + 1] == 'r' && data[i + 2] == 'u' &&
            data[i + 3] == 'e') {
            i += 4;
            return call_handler(handler.on_bool(true));
        }
        if (i + 5 <= len && data[i] == 'f' && data[i + 1] == 'a' && data[i + 2] == 'l' &&
            data[i + 3] == 's' && data[i + 4] == 'e') {
            i += 5;
            return call_handler(handler.on_bool(false));
        }
        return false;
    }

    bool parse_number() {
        size_t start = i;
        bool negative = (i < len && data[i] == '-');

        int64_t int_val;
        size_t consumed;
        if (util::parse_int_fast(data + i, len - i, int_val, consumed)) {
            if (i + consumed < len && (data[i + consumed] == '.' || data[i + consumed] == 'e' ||
                                       data[i + consumed] == 'E')) {
                // Fall through to double parsing
            } else {
                i += consumed;
                return call_handler(handler.on_int(int_val));
            }
        }

        if (!negative) {
            uint64_t uint_val;
            size_t consumed_uint;
            if (util::parse_uint_fast(data + i, len - i, uint_val, consumed_uint)) {
                if (i + consumed_uint < len &&
                    (data[i + consumed_uint] == '.' || data[i + consumed_uint] == 'e' ||
                     data[i + consumed_uint] == 'E')) {
                    // Fall through to double parsing
                } else {
                    i += consumed_uint;
                    return call_handler(handler.on_uint(uint_val));
                }
            }
        }

        double double_val;
        if (util::parse_double_fast(data + start, len - start, double_val, consumed)) {
            i = start + consumed;
            return call_handler(handler.on_double(double_val));
        }

        return false;
    }

    bool parse_string(bool is_key = false) {
        if (get() != '"')
            return false;

        size_t scan_pos = util::find_next_escape_simd(data + i, len - i);

        if (scan_pos < len - i && data[i + scan_pos] == '"') {
            std::string_view result(data + i, scan_pos);
            i += scan_pos + 1;
            return call_handler(is_key ? handler.on_key(result, false)
                                       : handler.on_string(result, false));
        }

        size_t string_start = i;

        while (!eof()) {
            char c = get();
            if (c == '"') {
                std::string_view raw_str(data + string_start, i - string_start - 1);
                return call_handler(is_key ? handler.on_key(raw_str, true)
                                           : handler.on_string(raw_str, true));
            }
            if (c == '\\') {
                if (eof())
                    return false;
                char esc = get();
                switch (esc) {
                case '"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    break;
                case 'u': {
                    uint32_t codepoint = 0;
                    if (!read_hex4(codepoint))
                        return false;
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
                    } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                        return false;
                    }
                    break;
                }
                default:
                    return false;
                }
            } else {
                if (static_cast<unsigned char>(c) < 0x20) {
                    return false;
                }
            }
        }
        return false;
    }
};

// ---------------------------------------------------------------------------
// Template parse_sax_impl_t — mirrors parse_sax_impl in json_parse.cpp
// but works for any Handler type with static dispatch.
// ---------------------------------------------------------------------------

template <typename Handler>
Status parse_sax_impl_t(std::string_view text, Handler& handler,
                        const ParseSaxOptions& options, ParseSaxContext* context) {
    constexpr size_t kStructuralTapeMinSize = 4 * 1024;
    constexpr size_t kStructuralTapeReserveDiv = 8;
    constexpr size_t kStructuralTapeReserveMax = 8 * 1024 * 1024;
    const size_t size = text.size();
    if (options.validate_utf8 && size != 0) {
        if (size <= kAsciiSwarPrefixMax) {
            if (!is_ascii_prefix_swar_detail(text.data(), size) &&
                !util::validate_utf8_simd(text.data(), size)) {
                return Status::ParseError;
            }
        } else if (size < kStructuralTapeMinSize) {
            if (!util::is_ascii_only_simd(text.data(), size) &&
                !util::validate_utf8_simd(text.data(), size)) {
                return Status::ParseError;
            }
        } else {
            if (!util::validate_utf8_lazy(text.data(), size)) {
                return Status::ParseError;
            }
        }
    }
    Parser<Handler> p{text.data(), size, handler, 0, {}};
    p.use_size_hints = options.use_size_hints;
    p.use_array_size_hints = options.use_array_size_hints;
    p.use_object_size_hints = options.use_object_size_hints;
    p.use_exact_size_hints = options.use_exact_size_hints;
    if (size >= kStructuralTapeMinSize) {
        p.stack_.reserve(64);
    } else {
        p.stack_.reserve(16);
    }
    const bool collect_structural_tape =
        options.use_structural_tape && options.collect_structural_tape &&
        size >= kStructuralTapeMinSize;
    if (collect_structural_tape) {
        size_t reserve_hint = size / kStructuralTapeReserveDiv;
        if (reserve_hint > kStructuralTapeReserveMax) {
            reserve_hint = kStructuralTapeReserveMax;
        }
        if (context) {
            context->structural_tape.clear();
            if (context->structural_tape.capacity() < reserve_hint) {
                context->structural_tape.reserve(reserve_hint);
            }
            util::collect_structural_positions_simd(text.data(), size, context->structural_tape);
            p.attach_structural_tape(&context->structural_tape);
        } else {
            static thread_local std::vector<size_t> structural_tape;
            structural_tape.clear();
            if (structural_tape.capacity() < reserve_hint) {
                structural_tape.reserve(reserve_hint);
            }
            util::collect_structural_positions_simd(text.data(), size, structural_tape);
            p.attach_structural_tape(&structural_tape);
        }
    }
    if (options.use_exact_size_hints && context) {
        p.attach_size_hints(&context->size_hints);
    }
    if (!p.parse_value()) {
        if (options.allow_abort && p.aborted()) {
            return Status::Ok;
        }
        return Status::ParseError;
    }
    p.skip_ws();
    if (!p.eof())
        return Status::ParseError;
    return Status::Ok;
}

} // namespace detail
} // namespace strata

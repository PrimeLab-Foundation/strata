/**
 * @file json_serialize.cpp
 * @brief Recursive-descent serialization of a JsonValue.
 */

#include "strata/json/json_serialize.hpp"

#include "strata/util/dtoa.hpp"
#include "strata/util/scan.hpp"

#include <cmath>
#include <cstddef>
#include <cstring>
#include <string_view>

namespace strata {

namespace {

/// Starting capacity for a fresh output buffer; grows from there as needed.
constexpr size_t kSerializeInitialCapacity = 1024;

constexpr char kHexDigits[] = "0123456789abcdef";

void serialize_value(const JsonValue& value, std::string& out);

} // namespace

void append_escaped_json_string(std::string_view text, std::string& out) {
    const char* cursor = text.data();
    size_t remaining = text.size();

    // Almost every string in almost every document escapes nothing, so that
    // case gets its own exit: one scan and one copy, with no per-byte work.
    const size_t leading_clean = util::find_next_escape(cursor, remaining);
    if (leading_clean == remaining) {
        out.reserve(out.size() + remaining + 2);
        out.push_back('"');
        out.append(cursor, remaining);
        out.push_back('"');
        return;
    }

    out.push_back('"');
    out.append(cursor, leading_clean);
    cursor += leading_clean;
    remaining -= leading_clean;

    // The rest of the string escapes something. Emitting it straight into the
    // output would cost two appends per escape plus one per clean run, and
    // each append re-checks capacity; a string of a dozen escapes spent more
    // time in std::string bookkeeping than in the escaping itself. Building
    // through a stack chunk makes that one append per chunkful.
    char chunk[512];
    size_t used = 0;
    const auto flush = [&] {
        out.append(chunk, used);
        used = 0;
    };
    // The longest thing one iteration writes is a six-byte \u00xx escape.
    constexpr size_t kMaxEscapeBytes = 6;

    while (remaining > 0) {
        if (used + kMaxEscapeBytes > sizeof(chunk))
            flush();

        const char c = *cursor;
        ++cursor;
        --remaining;
        switch (c) {
        case '"':
            std::memcpy(chunk + used, "\\\"", 2);
            used += 2;
            break;
        case '\\':
            std::memcpy(chunk + used, "\\\\", 2);
            used += 2;
            break;
        case '\b':
            std::memcpy(chunk + used, "\\b", 2);
            used += 2;
            break;
        case '\f':
            std::memcpy(chunk + used, "\\f", 2);
            used += 2;
            break;
        case '\n':
            std::memcpy(chunk + used, "\\n", 2);
            used += 2;
            break;
        case '\r':
            std::memcpy(chunk + used, "\\r", 2);
            used += 2;
            break;
        case '\t':
            std::memcpy(chunk + used, "\\t", 2);
            used += 2;
            break;
        default: {
            // find_next_escape stops on exactly quote, backslash and bytes
            // below 0x20, so anything reaching here is a control character
            // with no short escape. Everything else travels in the clean runs
            // below, which is how UTF-8 passes through untouched.
            const auto byte = static_cast<unsigned char>(c);
            std::memcpy(chunk + used, "\\u00", 4);
            used += 4;
            chunk[used++] = kHexDigits[byte >> 4];
            chunk[used++] = kHexDigits[byte & 0x0F];
            break;
        }
        }

        const size_t clean = util::find_next_escape(cursor, remaining);
        if (clean == 0)
            continue;
        if (clean > sizeof(chunk) - used) {
            // A long clean run is worth an append of its own rather than
            // several trips through the chunk.
            flush();
            out.append(cursor, clean);
        } else {
            std::memcpy(chunk + used, cursor, clean);
            used += clean;
        }
        cursor += clean;
        remaining -= clean;
    }

    flush();
    out.push_back('"');
}

namespace {

void append_number(double value, std::string& out) {
    if (std::isnan(value) || std::isinf(value)) {
        out.append("null"); // JSON has no way to spell either
        return;
    }
    char buffer[util::kDoubleBufferSize];
    const size_t length = util::format_double(value, buffer, sizeof(buffer));
    out.append(buffer, length);
}

void serialize_value(const JsonValue& value, std::string& out) {
    if (value.is_null()) {
        out.append("null");
    } else if (value.is_bool()) {
        out.append(value.as_bool() ? "true" : "false");
    } else if (value.is_number()) {
        append_number(value.as_number(), out);
    } else if (value.is_string()) {
        append_escaped_json_string(value.as_string(), out);
    } else if (value.is_array()) {
        out.push_back('[');
        bool first = true;
        for (const JsonValue& element : value.as_array()) {
            if (!first)
                out.push_back(',');
            first = false;
            serialize_value(element, out);
        }
        out.push_back(']');
    } else {
        out.push_back('{');
        bool first = true;
        for (const auto& [key, member] : value.as_object()) {
            if (!first)
                out.push_back(',');
            first = false;
            append_escaped_json_string(key, out);
            out.push_back(':');
            serialize_value(member, out);
        }
        out.push_back('}');
    }
}

} // namespace

std::string serialize_json(const JsonValue& value) {
    std::string out;
    out.reserve(kSerializeInitialCapacity);
    serialize_value(value, out);
    return out;
}

void serialize_json_to(const JsonValue& value, std::string& out) {
    out.clear();
    serialize_value(value, out);
}

} // namespace strata

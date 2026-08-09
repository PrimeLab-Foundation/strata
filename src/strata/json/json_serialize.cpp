/**
 * @file json_serialize.cpp
 * @brief Recursive-descent serialization of a JsonValue.
 */

#include "strata/json/json_serialize.hpp"

#include "strata/util/dtoa.hpp"
#include "strata/util/scan.hpp"

#include <cmath>
#include <cstddef>
#include <string_view>

namespace strata {

namespace {

/// Starting capacity for a fresh output buffer; grows from there as needed.
constexpr size_t kSerializeInitialCapacity = 1024;

constexpr char kHexDigits[] = "0123456789abcdef";

void serialize_value(const JsonValue& value, std::string& out);

} // namespace

void append_escaped_json_string(std::string_view text, std::string& out) {
    out.push_back('"');

    // Most strings need no escaping at all, and most that do are mostly clean.
    // Copy each clean run in one go rather than a byte at a time; the scan that
    // finds the next interesting byte is the same one the parser uses.
    const char* cursor = text.data();
    size_t remaining = text.size();
    while (remaining > 0) {
        const size_t clean = util::find_next_escape(cursor, remaining);
        if (clean > 0) {
            out.append(cursor, clean);
            cursor += clean;
            remaining -= clean;
            if (remaining == 0)
                break;
        }

        const char c = *cursor;
        ++cursor;
        --remaining;
        switch (c) {
        case '"':
            out.append("\\\"");
            break;
        case '\\':
            out.append("\\\\");
            break;
        case '\b':
            out.append("\\b");
            break;
        case '\f':
            out.append("\\f");
            break;
        case '\n':
            out.append("\\n");
            break;
        case '\r':
            out.append("\\r");
            break;
        case '\t':
            out.append("\\t");
            break;
        default: {
            // find_next_escape stops on exactly quote, backslash and bytes
            // below 0x20, so anything reaching here is a control character
            // with no short escape. Everything else was copied in the clean
            // run above, which is how UTF-8 passes through untouched.
            const auto byte = static_cast<unsigned char>(c);
            out.append("\\u00");
            out.push_back(kHexDigits[byte >> 4]);
            out.push_back(kHexDigits[byte & 0x0F]);
            break;
        }
        }
    }

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

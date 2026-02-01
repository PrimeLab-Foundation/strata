#include "strata/json/json_serialize.hpp"

#include "strata/util/dragonbox.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace strata {

// Forward declaration
static void serialize_value(const JsonValue& value, std::string& out);

// Escape a string for JSON output
static void escape_string(std::string_view str, std::string& out) {
    out.push_back('"');

    for (char c : str) {
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
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                // Control characters: escape as \uXXXX
                char buf[7];
                std::snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
                out.append(buf);
            } else {
                // Regular UTF-8 character
                out.push_back(c);
            }
            break;
        }
    }

    out.push_back('"');
}

// Serialize a number
static void serialize_number(const JsonValue& value, std::string& out) {
    double d = value.as_number();

    // Handle special float values (NaN, Inf)
    if (std::isnan(d) || std::isinf(d)) {
        // JSON doesn't support NaN/Inf, output null
        out.append("null");
        return;
    }

    char buf[32];
    int len = util::dragonbox_d2s(d, buf);
    out.append(buf, len);
}

// Serialize an array
static void serialize_array(const JsonValue::Array& arr, std::string& out) {
    out.push_back('[');

    bool first = true;
    for (const auto& elem : arr) {
        if (!first) {
            out.push_back(',');
        }
        first = false;
        serialize_value(elem, out);
    }

    out.push_back(']');
}

// Serialize an object
static void serialize_object(const JsonValue::Object& obj, std::string& out) {
    out.push_back('{');

    bool first = true;
    for (const auto& [key, value] : obj) {
        if (!first) {
            out.push_back(',');
        }
        first = false;

        // Serialize key (always a string)
        escape_string(key, out);
        out.push_back(':');

        // Serialize value
        serialize_value(value, out);
    }

    out.push_back('}');
}

// Main serialization dispatch
static void serialize_value(const JsonValue& value, std::string& out) {
    if (value.is_null()) {
        out.append("null");
    } else if (value.is_bool()) {
        out.append(value.as_bool() ? "true" : "false");
    } else if (value.is_number()) {
        serialize_number(value, out);
    } else if (value.is_string()) {
        escape_string(value.as_string(), out);
    } else if (value.is_array()) {
        serialize_array(value.as_array(), out);
    } else if (value.is_object()) {
        serialize_object(value.as_object(), out);
    } else {
        throw std::runtime_error("Unknown JsonValue type");
    }
}

std::string serialize_json(const JsonValue& value) {
    std::string out;
    out.reserve(1024); // Initial capacity
    serialize_value(value, out);
    return out;
}

void serialize_json_to(const JsonValue& value, std::string& out) {
    out.clear();
    out.reserve(1024); // Initial capacity
    serialize_value(value, out);
}

} // namespace strata

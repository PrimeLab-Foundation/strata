/**
 * @file json_serialize.cpp
 * @brief JSON serialization (dumps) implementation.
 *
 * This file implements JSON serialization from JsonValue DOM to string output.
 * Key features:
 * - serialize_json(): Convert JsonValue to JSON string
 * - Proper string escaping (quotes, backslash, control characters)
 * - Float formatting via Dragonbox algorithm for clean, round-trip safe output
 * - Compact integer formatting (no trailing ".0" for whole numbers)
 *
 * The serializer handles:
 * - All JSON value types: null, bool, number, string, array, object
 * - Special float values (NaN, Infinity) → null
 * - Unicode passthrough (UTF-8 preserved)
 * - Control character escaping (\n, \r, \t, \b, \f)
 *
 * Dependencies:
 * - json_serialize.hpp: Public API
 * - dragonbox.hpp: Fast float-to-string conversion
 *
 * @see json_serialize.hpp for public API
 */

#include "strata/json/json_serialize.hpp"

#include "strata/util/dragonbox.hpp"
#include "strata/util/simd_string.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <system_error>

namespace strata {

// Forward declaration
static void serialize_value(const JsonValue& value, std::string& out);

// Escape a string for JSON output
static void escape_string(std::string_view str, std::string& out) {
    util::escape_or_copy_string_simd(str.data(), str.size(), out);
}

// Serialize a number
static void serialize_number(const JsonValue& value, std::string& out) {
    if (value.is_int()) {
        char buf[32];
        auto res = std::to_chars(buf, buf + sizeof(buf), value.as_int());
        if (res.ec == std::errc()) {
            out.append(buf, static_cast<size_t>(res.ptr - buf));
            return;
        }
        int len = std::snprintf(buf, sizeof(buf), "%lld", static_cast<long long>(value.as_int()));
        if (len > 0) {
            out.append(buf, static_cast<size_t>(len));
        }
        return;
    }

    double d = value.as_double();

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

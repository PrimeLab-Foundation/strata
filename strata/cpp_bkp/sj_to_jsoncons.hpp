#pragma once

#include <jsoncons/json.hpp>
#include <simdjson.h>
#include <stdexcept>

namespace strata {

/**
 * Convert simdjson::ondemand::value to jsoncons::json
 * This bridge allows JSONPath evaluation on streaming parsed data
 */
inline jsoncons::json convert_simdjson_to_jsoncons(simdjson::ondemand::value value) {
    switch (value.type()) {
    case simdjson::ondemand::json_type::object: {
        jsoncons::json obj = jsoncons::json::object();
        auto obj_val = value.get_object().value();
        for (auto field : obj_val) {
            std::string_view key_view = field.unescaped_key().value();
            std::string key(key_view);
            obj[key] = convert_simdjson_to_jsoncons(field.value());
        }
        return obj;
    }
    case simdjson::ondemand::json_type::array: {
        jsoncons::json arr = jsoncons::json::array();
        auto arr_val = value.get_array().value();
        for (auto element : arr_val) {
            arr.push_back(convert_simdjson_to_jsoncons(element.value()));
        }
        return arr;
    }
    case simdjson::ondemand::json_type::string: {
        std::string_view str_view = value.get_string().value();
        return jsoncons::json(std::string(str_view));
    }
    case simdjson::ondemand::json_type::number: {
        // Try int64 first, then uint64, then double
        int64_t i64;
        if (value.get_int64().get(i64) == simdjson::SUCCESS) {
            return jsoncons::json(i64);
        }
        uint64_t u64;
        if (value.get_uint64().get(u64) == simdjson::SUCCESS) {
            return jsoncons::json(u64);
        }
        double d;
        if (value.get_double().get(d) == simdjson::SUCCESS) {
            return jsoncons::json(d);
        }
        throw std::runtime_error("Failed to parse number");
    }
    case simdjson::ondemand::json_type::boolean: {
        return jsoncons::json(value.get_bool().value());
    }
    case simdjson::ondemand::json_type::null: {
        return jsoncons::json::null();
    }
    default:
        throw std::runtime_error("Unknown JSON type");
    }
}

/**
 * Convert simdjson::ondemand::document to jsoncons::json
 */
inline jsoncons::json convert_document_to_jsoncons(simdjson::ondemand::document& doc) {
    return convert_simdjson_to_jsoncons(doc.get_value().value());
}

/**
 * Convert simdjson::ondemand::object to jsoncons::json
 */
inline jsoncons::json convert_object_to_jsoncons(simdjson::ondemand::object obj) {
    jsoncons::json result = jsoncons::json::object();
    for (auto field : obj) {
        std::string_view key_view = field.unescaped_key().value();
        std::string key(key_view);
        result[key] = convert_simdjson_to_jsoncons(field.value());
    }
    return result;
}

} // namespace strata

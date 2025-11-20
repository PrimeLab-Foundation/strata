/**
 * simdjson_to_python.hpp - Utilities for converting simdjson values to Python objects
 *
 * This header provides reusable conversion functions to eliminate duplication
 * across the pybind11 binding code. The conversion logic handles all JSON types
 * (null, bool, number, string, array, object) with proper error handling.
 */

#pragma once

#include <cstring>
#include <pybind11/pybind11.h>
#include <simdjson.h>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace strata {
namespace detail {

namespace py = pybind11;

/**
 * Convert simdjson ondemand value to Python object (recursive)
 *
 * Handles all JSON types with proper error recovery. Returns py::none()
 * on conversion errors rather than throwing (defensive approach).
 *
 * @param v  simdjson ondemand value to convert
 * @return   Python object (dict, list, str, int, float, bool, or None)
 *
 * @note This is recursive for arrays and objects
 * @note simdjson errors are caught and return None rather than propagating
 */
inline py::object simdjson_value_to_python(simdjson::ondemand::value v) {
    try {
        auto type_result = v.type();
        if (type_result.error()) {
            return py::none();
        }
        auto type = type_result.value();

        switch (type) {
        case simdjson::ondemand::json_type::null:
            return py::none();

        case simdjson::ondemand::json_type::boolean: {
            auto b = v.get_bool();
            if (b.error()) {
                return py::none();
            }
            return py::bool_(b.value());
        }

        case simdjson::ondemand::json_type::number: {
            auto num_type_result = v.get_number_type();
            if (num_type_result.error()) {
                return py::none();
            }
            auto num_type = num_type_result.value();

            if (num_type == simdjson::ondemand::number_type::signed_integer ||
                num_type == simdjson::ondemand::number_type::unsigned_integer) {
                auto ival = v.get_int64();
                if (ival.error()) {
                    return py::none();
                }
                return py::int_(ival.value());
            } else {
                auto dval = v.get_double();
                if (dval.error()) {
                    return py::none();
                }
                return py::float_(dval.value());
            }
        }

        case simdjson::ondemand::json_type::string: {
            auto sv_result = v.get_string();
            if (sv_result.error()) {
                return py::none();
            }
            std::string_view sv = sv_result.value();
            return py::str(sv.data(), sv.size());
        }

        case simdjson::ondemand::json_type::array: {
            py::list result;
            auto arr_result = v.get_array();
            if (arr_result.error()) {
                return result;
            }
            auto arr = arr_result.value();
            for (auto elem : arr) {
                if (!elem.error()) {
                    result.append(simdjson_value_to_python(elem.value()));
                }
            }
            return result;
        }

        case simdjson::ondemand::json_type::object: {
            py::dict result;
            auto obj_result = v.get_object();
            if (obj_result.error()) {
                return result;
            }
            auto obj = obj_result.value();
            for (auto field : obj) {
                if (!field.error()) {
                    auto key_result = field.unescaped_key();
                    if (!key_result.error()) {
                        std::string_view key = key_result.value();
                        py::str py_key(key.data(), key.size());
                        auto val_result = field.value();
                        if (!val_result.error()) {
                            result[py_key] = simdjson_value_to_python(val_result.value());
                        }
                    }
                }
            }
            return result;
        }

        default:
            return py::none();
        }
    } catch (const simdjson::simdjson_error& e) {
        throw std::runtime_error(std::string("Failed to convert simdjson value to Python: ") +
                                 e.what());
    }
}

/**
 * Convert JSON slice (buffer + length) to Python object
 *
 * Parses the JSON slice with simdjson and converts to Python.
 * Automatically handles padding requirements for simdjson.
 *
 * @param data  Pointer to JSON data
 * @param len   Length of JSON data in bytes
 * @return      Python object representation of the JSON
 *
 * @throws std::runtime_error if parsing fails
 *
 * @note Allocates temporary buffer with SIMDJSON_PADDING for safety
 * @note Not suitable for very large documents (copies data)
 */
inline py::object json_slice_to_python(const char* data, size_t len) {
    // Allocate padded buffer for simdjson safety
    std::vector<uint8_t> buffer(len + simdjson::SIMDJSON_PADDING);
    std::memcpy(buffer.data(), data, len);

    // Parse with simdjson
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(buffer.data(), len, buffer.size());
    auto value_result = doc.get_value();

    if (value_result.error()) {
        throw std::runtime_error("Failed to parse JSON slice with simdjson");
    }

    return simdjson_value_to_python(value_result.value());
}

} // namespace detail
} // namespace strata

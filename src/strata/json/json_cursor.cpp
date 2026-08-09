/**
 * @file json_cursor.cpp
 * @brief JsonCursor accessors.
 *
 * The throwing accessors distinguish two failures that read the same to a
 * caller but are not the same: a lookup that missed, and a lookup attempted on
 * the wrong kind of value. Their messages are pinned by the Python error
 * contract (docs/context/api.md).
 */

#include "strata/json/json_cursor.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace strata {

namespace {

/// A number is usable as an integer only if it is integral and in range.
[[nodiscard]] bool fits_int64(double value) noexcept {
    if (!std::isfinite(value))
        return false;
    if (value != std::floor(value))
        return false;
    return value >= -9223372036854775808.0 && value < 9223372036854775808.0;
}

} // namespace

Result<bool> JsonCursor::get_bool() const {
    if (!is_bool())
        return {Status::TypeMismatch};
    return {Status::Ok, value_->as_bool()};
}

Result<int64_t> JsonCursor::get_int64() const {
    if (!is_number())
        return {Status::TypeMismatch};
    const double number = value_->as_number();
    if (!fits_int64(number))
        return {Status::TypeMismatch};
    return {Status::Ok, static_cast<int64_t>(number)};
}

Result<double> JsonCursor::get_double() const {
    if (!is_number())
        return {Status::TypeMismatch};
    return {Status::Ok, value_->as_number()};
}

Result<std::string_view> JsonCursor::get_string() const {
    if (!is_string())
        return {Status::TypeMismatch};
    return {Status::Ok, std::string_view(value_->as_string())};
}

Result<JsonCursor> JsonCursor::get_field(std::string_view key) const {
    if (!is_object())
        return {Status::TypeMismatch};
    const JsonValue::Object& object = value_->as_object();
    const auto found = object.find(std::string(key));
    if (found == object.end())
        return {Status::KeyNotFound};
    return {Status::Ok, JsonCursor(&found->second)};
}

Result<JsonCursor> JsonCursor::get_at(size_t index) const {
    if (!is_array())
        return {Status::TypeMismatch};
    const JsonValue::Array& array = value_->as_array();
    if (index >= array.size())
        return {Status::IndexOutOfBounds};
    return {Status::Ok, JsonCursor(&array[index])};
}

bool JsonCursor::get_bool_or_throw() const {
    const auto result = get_bool();
    if (!result.ok())
        throw std::runtime_error("value is not a bool");
    return result.value;
}

int64_t JsonCursor::get_int() const {
    const auto result = get_int64();
    if (!result.ok())
        throw std::runtime_error("value is not a number");
    return result.value;
}

double JsonCursor::get_float() const {
    const auto result = get_double();
    if (!result.ok())
        throw std::runtime_error("value is not a number");
    return result.value;
}

std::string JsonCursor::get_str() const {
    const auto result = get_string();
    if (!result.ok())
        throw std::runtime_error("value is not a string");
    return std::string(result.value);
}

JsonCursor JsonCursor::field(std::string_view key) const {
    const auto result = get_field(key);
    if (result.ok())
        return result.value;
    if (result.status == Status::KeyNotFound)
        throw std::out_of_range("field not found");
    throw std::runtime_error("value is not an object");
}

JsonCursor JsonCursor::at(size_t index) const {
    const auto result = get_at(index);
    if (result.ok())
        return result.value;
    if (result.status == Status::IndexOutOfBounds)
        throw std::out_of_range("index out of range");
    throw std::runtime_error("value is not an array");
}

size_t JsonCursor::array_size() const noexcept {
    return is_array() ? value_->as_array().size() : 0;
}

size_t JsonCursor::object_size() const noexcept {
    return is_object() ? value_->as_object().size() : 0;
}

std::vector<std::string> JsonCursor::object_keys() const {
    std::vector<std::string> keys;
    if (!is_object())
        return keys;
    const JsonValue::Object& object = value_->as_object();
    keys.reserve(object.size());
    for (const auto& [key, unused] : object) {
        (void)unused;
        keys.push_back(key);
    }
    return keys;
}

} // namespace strata

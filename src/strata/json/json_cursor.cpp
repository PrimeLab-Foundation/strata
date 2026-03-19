/**
 * @file json_cursor.cpp
 * @brief JsonCursor implementation — non-owning JSON value navigation.
 *
 * Integer extraction from doubles (get_int64, get_uint64) uses
 * std::modf to reject non-integer doubles rather than simple casting,
 * which would silently truncate 3.7 → 3.
 */

#include "strata/json/json_cursor.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace strata {

JsonCursor::JsonCursor(const JsonValue* v) noexcept : value_(v) {}

bool JsonCursor::is_null() const noexcept { return value_ && value_->is_null(); }
bool JsonCursor::is_bool() const noexcept { return value_ && value_->is_bool(); }
bool JsonCursor::is_number() const noexcept { return value_ && value_->is_number(); }
bool JsonCursor::is_string() const noexcept { return value_ && value_->is_string(); }
bool JsonCursor::is_array() const noexcept { return value_ && value_->is_array(); }
bool JsonCursor::is_object() const noexcept { return value_ && value_->is_object(); }

Result<bool> JsonCursor::get_bool() const {
    if (!is_bool())
        return {Status::TypeMismatch, false};
    return {Status::Ok, value_->as_bool()};
}

Result<int64_t> JsonCursor::get_int64() const {
    if (!is_number())
        return {Status::TypeMismatch, 0};
    // Fast path: Int64 variant — no precision loss.
    if (value_->is_int64())
        return {Status::Ok, value_->as_int64()};
    // Double variant — check it's an exact integer in int64 range.
    double d = value_->as_number();
    if (!std::isfinite(d))
        return {Status::TypeMismatch, 0};
    double int_part = 0.0;
    if (std::modf(d, &int_part) != 0.0)
        return {Status::TypeMismatch, 0};
    if (int_part < static_cast<double>(std::numeric_limits<int64_t>::min()) ||
        int_part > static_cast<double>(std::numeric_limits<int64_t>::max()))
        return {Status::TypeMismatch, 0};
    return {Status::Ok, static_cast<int64_t>(int_part)};
}

Result<uint64_t> JsonCursor::get_uint64() const {
    if (!is_number())
        return {Status::TypeMismatch, 0};
    // Fast path: Int64 variant.
    if (value_->is_int64()) {
        int64_t v = value_->as_int64();
        if (v < 0)
            return {Status::TypeMismatch, 0};
        return {Status::Ok, static_cast<uint64_t>(v)};
    }
    // Double variant.
    double d = value_->as_number();
    if (!std::isfinite(d) || d < 0)
        return {Status::TypeMismatch, 0};
    double int_part = 0.0;
    if (std::modf(d, &int_part) != 0.0)
        return {Status::TypeMismatch, 0};
    if (int_part > static_cast<double>(std::numeric_limits<uint64_t>::max()))
        return {Status::TypeMismatch, 0};
    return {Status::Ok, static_cast<uint64_t>(int_part)};
}

Result<double> JsonCursor::get_double() const {
    if (!is_number())
        return {Status::TypeMismatch, 0.0};
    return {Status::Ok, value_->as_number()};
}

Result<std::string_view> JsonCursor::get_string() const {
    if (!is_string())
        return {Status::TypeMismatch, std::string_view{}};
    const std::string& s = value_->as_string();
    return {Status::Ok, std::string_view(s.data(), s.size())};
}

Result<double> JsonCursor::get_number() const { return get_double(); }

Result<JsonCursor> JsonCursor::get_field(std::string_view key) const {
    if (!is_object())
        return {Status::TypeMismatch, JsonCursor(nullptr)};
    const auto& obj = value_->as_object();
    auto it = obj.find(std::string(key));
    if (it == obj.end())
        return {Status::KeyNotFound, JsonCursor(nullptr)};
    return {Status::Ok, JsonCursor(&it->second)};
}

Result<JsonCursor> JsonCursor::get_at(std::size_t index) const {
    if (!is_array())
        return {Status::TypeMismatch, JsonCursor(nullptr)};
    const auto& arr = value_->as_array();
    if (index >= arr.size())
        return {Status::IndexOutOfBounds, JsonCursor(nullptr)};
    return {Status::Ok, JsonCursor(&arr[index])};
}

// ------------------------------------------------------------------
// Throwing helpers (used by pybind, or C++ callers that prefer exceptions)
// ------------------------------------------------------------------

bool JsonCursor::get_bool_or_throw() const {
    auto r = get_bool();
    if (!r.ok())
        throw std::runtime_error("value is not a bool");
    return r.value;
}

int64_t JsonCursor::get_int() const {
    auto r = get_int64();
    if (!r.ok())
        throw std::runtime_error("value is not a number");
    return r.value;
}

double JsonCursor::get_float() const {
    auto r = get_double();
    if (!r.ok())
        throw std::runtime_error("value is not a number");
    return r.value;
}

std::string JsonCursor::get_str() const {
    auto r = get_string();
    if (!r.ok())
        throw std::runtime_error("value is not a string");
    return std::string(r.value);
}

JsonCursor JsonCursor::field(std::string_view key) const {
    auto r = get_field(key);
    if (!r.ok()) {
        if (r.status == Status::KeyNotFound)
            throw std::out_of_range("field not found");
        throw std::runtime_error("value is not an object");
    }
    return r.value;
}

JsonCursor JsonCursor::at(std::size_t index) const {
    auto r = get_at(index);
    if (!r.ok()) {
        if (r.status == Status::IndexOutOfBounds)
            throw std::out_of_range("index out of range");
        throw std::runtime_error("value is not an array");
    }
    return r.value;
}

const JsonValue* JsonCursor::raw() const noexcept { return value_; }

size_t JsonCursor::array_size() const noexcept {
    if (!is_array())
        return 0;
    return value_->as_array().size();
}

size_t JsonCursor::object_size() const noexcept {
    if (!is_object())
        return 0;
    return value_->as_object().size();
}

std::vector<std::string> JsonCursor::object_keys() const {
    std::vector<std::string> keys;
    if (is_object()) {
        const auto& obj = value_->as_object();
        keys.reserve(obj.size());
        for (const auto& pair : obj) {
            keys.push_back(pair.first);
        }
    }
    return keys;
}

} // namespace strata

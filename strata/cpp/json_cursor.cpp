#include "json_cursor.hpp"

namespace strata {

JsonCursor::JsonCursor(const JsonValue* v) : value_(v) {}

bool JsonCursor::is_null() const { return value_ && value_->is_null(); }

bool JsonCursor::is_bool() const { return value_ && value_->is_bool(); }

bool JsonCursor::is_number() const { return value_ && value_->is_number(); }

bool JsonCursor::is_string() const { return value_ && value_->is_string(); }

bool JsonCursor::is_array() const { return value_ && value_->is_array(); }

bool JsonCursor::is_object() const { return value_ && value_->is_object(); }

Result<bool> JsonCursor::get_bool() const {
    if (!is_bool())
        return {Status::TypeMismatch, false};
    return {Status::Ok, value_->as_bool()};
}

Result<int64_t> JsonCursor::get_int64() const {
    if (!is_number())
        return {Status::TypeMismatch, 0};
    double d = value_->as_number();
    return {Status::Ok, static_cast<int64_t>(d)};
}

Result<uint64_t> JsonCursor::get_uint64() const {
    if (!is_number() || value_->as_number() < 0)
        return {Status::TypeMismatch, 0};
    double d = value_->as_number();
    return {Status::Ok, static_cast<uint64_t>(d)};
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

Result<double> JsonCursor::get_number() const {
    // In this model, numbers are stored as double, so just forward.
    return get_double();
}

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

const JsonValue* JsonCursor::raw() const { return value_; }

} // namespace strata

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace strata {

// Status codes for cursor operations (no exceptions in hot path)
enum class Status { Ok, TypeMismatch, KeyNotFound, IndexOutOfBounds, ParseError };

// Result type for operations that may fail
template <typename T> struct Result {
    Status status;
    T value;

    bool ok() const { return status == Status::Ok; }
    T unwrap() const { return value; }
    T value_or(T default_val) const { return ok() ? value : default_val; }
};

// Simple in‑memory JSON value model
struct JsonValue {
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>; // Keep std::map - better cache locality
    using Number = double;                           // keep it simple for now

    using Variant = std::variant<std::nullptr_t, bool, Number, std::string, Array, Object>;

    Variant data;

    JsonValue() : data(nullptr) {}
    explicit JsonValue(Variant v) : data(std::move(v)) {}

    bool is_null() const { return std::holds_alternative<std::nullptr_t>(data); }
    bool is_bool() const { return std::holds_alternative<bool>(data); }
    bool is_number() const { return std::holds_alternative<Number>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_array() const { return std::holds_alternative<Array>(data); }
    bool is_object() const { return std::holds_alternative<Object>(data); }

    const bool& as_bool() const { return std::get<bool>(data); }
    const Number& as_number() const { return std::get<Number>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
    const Array& as_array() const { return std::get<Array>(data); }
    const Object& as_object() const { return std::get<Object>(data); }

    bool& as_bool() { return std::get<bool>(data); }
    Number& as_number() { return std::get<Number>(data); }
    std::string& as_string() { return std::get<std::string>(data); }
    Array& as_array() { return std::get<Array>(data); }
    Object& as_object() { return std::get<Object>(data); }
};

} // namespace strata

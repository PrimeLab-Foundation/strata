#pragma once

#include "json_core.hpp"

#include <string>
#include <string_view>

namespace strata {

class JsonCursor {
  public:
    JsonCursor() = default;
    explicit JsonCursor(const JsonValue* v);

    // Type predicates
    bool is_null() const;
    bool is_bool() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    // Low-level, status-code based accessors
    Result<bool> get_bool() const;
    Result<int64_t> get_int64() const;
    Result<uint64_t> get_uint64() const;
    Result<double> get_double() const;
    Result<std::string_view> get_string() const;

    // Generic number accessor (alias for get_double in this model)
    Result<double> get_number() const;

    // Object field / array index navigation
    Result<JsonCursor> get_field(std::string_view key) const;
    Result<JsonCursor> get_at(std::size_t index) const;

    // ------------------------------------------------------------------
    // High-level convenience methods expected by module_pybind.cpp
    // These throw std::runtime_error / std::out_of_range on mismatch.
    // ------------------------------------------------------------------
    bool get_bool_or_throw() const;
    int64_t get_int() const;
    double get_float() const;
    std::string get_str() const;
    JsonCursor field(std::string_view key) const;
    JsonCursor at(std::size_t index) const;

    const JsonValue* raw() const;

  private:
    const JsonValue* value_ = nullptr;
};

} // namespace strata

#pragma once

#include "json_core.hpp"

#include <string_view>

namespace strata {

/**
 * Cursor: lightweight, read-only navigation over a JsonValue tree.
 *
 * This is a pure C++ type: NO pybind11 here, NO Python types, NO simdjson.
 * It is used both by C++ tests and by the pybind11 module as a backend.
 */
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

    // Scalar accessors (status-code based, never throw)
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

    // Access to underlying JsonValue pointer (for advanced usage)
    const JsonValue* raw() const;

  private:
    const JsonValue* value_ = nullptr;
};

} // namespace strata

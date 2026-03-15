#pragma once

/**
 * @file json_cursor.hpp
 * @brief Lightweight, non-owning navigator over a JsonValue tree.
 *
 * JsonCursor wraps a borrowed const JsonValue* and provides two
 * flavours of accessors:
 *
 * 1. **Status-code API** (get_bool(), get_field(), …) — returns
 *    Result<T>, never throws.  Preferred on hot paths.
 * 2. **Throwing API** (get_bool_or_throw(), field(), at(), …) —
 *    throws std::runtime_error / std::out_of_range on mismatch.
 *    Convenient for callers that prefer exceptions.
 *
 * The cursor does NOT own the pointed-to value; the caller must
 * ensure the owning JsonDocument (or equivalent) outlives the cursor.
 */

#include "strata/json/json_core.hpp"

#include <string>
#include <string_view>

namespace strata {

class JsonCursor {
  public:
    JsonCursor() noexcept = default;
    explicit JsonCursor(const JsonValue* v) noexcept;

    // --- Type predicates (never throw) -------------------------------------
    [[nodiscard]] bool is_null() const noexcept;
    [[nodiscard]] bool is_bool() const noexcept;
    [[nodiscard]] bool is_number() const noexcept;
    [[nodiscard]] bool is_string() const noexcept;
    [[nodiscard]] bool is_array() const noexcept;
    [[nodiscard]] bool is_object() const noexcept;

    // --- Low-level, status-code based accessors ----------------------------
    [[nodiscard]] Result<bool> get_bool() const;
    [[nodiscard]] Result<int64_t> get_int64() const;
    [[nodiscard]] Result<uint64_t> get_uint64() const;
    [[nodiscard]] Result<double> get_double() const;
    [[nodiscard]] Result<std::string_view> get_string() const;

    /// Alias for get_double() (single number model).
    [[nodiscard]] Result<double> get_number() const;

    /// Navigate into an object field by key.
    [[nodiscard]] Result<JsonCursor> get_field(std::string_view key) const;
    /// Navigate into an array element by index.
    [[nodiscard]] Result<JsonCursor> get_at(std::size_t index) const;

    // --- Throwing convenience methods --------------------------------------
    [[nodiscard]] bool get_bool_or_throw() const;
    [[nodiscard]] int64_t get_int() const;
    [[nodiscard]] double get_float() const;
    [[nodiscard]] std::string get_str() const;
    [[nodiscard]] JsonCursor field(std::string_view key) const;
    [[nodiscard]] JsonCursor at(std::size_t index) const;

    // --- Iteration helpers for JSONPath ------------------------------------
    [[nodiscard]] size_t array_size() const noexcept;
    [[nodiscard]] size_t object_size() const noexcept;
    [[nodiscard]] std::vector<std::string> object_keys() const;

    /// Access the underlying raw pointer (may be nullptr).
    [[nodiscard]] const JsonValue* raw() const noexcept;

  private:
    const JsonValue* value_ = nullptr;
};

} // namespace strata

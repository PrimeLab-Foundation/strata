#pragma once

/**
 * @file json_cursor.hpp
 * @brief Non-owning navigator over a JsonValue tree.
 *
 * Two accessor flavours over the same data:
 *
 * 1. **Status-code API** (`get_bool`, `get_field`, ...) returns `Result<T>` and
 *    never throws. This is the hot-path interface.
 * 2. **Throwing API** (`get_int`, `field`, `at`, ...) raises instead, which is
 *    what the binding layer wants: the exception carries the message straight
 *    through to Python.
 *
 * The cursor **borrows**. Whatever owns the tree must outlive every cursor
 * into it — the binding layer satisfies that by having each Python cursor hold
 * a share of the document (docs/architecture/SKILL.md, invariant 2).
 */

#include "strata/json/json_core.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

class JsonCursor {
  public:
    JsonCursor() noexcept = default;
    explicit JsonCursor(const JsonValue* value) noexcept : value_(value) {}

    // --- Type predicates ---------------------------------------------------
    [[nodiscard]] bool is_null() const noexcept { return value_ != nullptr && value_->is_null(); }
    [[nodiscard]] bool is_bool() const noexcept { return value_ != nullptr && value_->is_bool(); }
    [[nodiscard]] bool is_number() const noexcept {
        return value_ != nullptr && value_->is_number();
    }
    [[nodiscard]] bool is_string() const noexcept {
        return value_ != nullptr && value_->is_string();
    }
    [[nodiscard]] bool is_array() const noexcept { return value_ != nullptr && value_->is_array(); }
    [[nodiscard]] bool is_object() const noexcept {
        return value_ != nullptr && value_->is_object();
    }

    // --- Status-code accessors ---------------------------------------------
    [[nodiscard]] Result<bool> get_bool() const;
    [[nodiscard]] Result<int64_t> get_int64() const;
    [[nodiscard]] Result<double> get_double() const;
    [[nodiscard]] Result<std::string_view> get_string() const;
    [[nodiscard]] Result<JsonCursor> get_field(std::string_view key) const;
    [[nodiscard]] Result<JsonCursor> get_at(size_t index) const;

    // --- Throwing accessors ------------------------------------------------
    // Messages are part of the Python error contract (docs/context/api.md):
    // the binding layer turns them into RuntimeError verbatim.
    [[nodiscard]] bool get_bool_or_throw() const;
    [[nodiscard]] int64_t get_int() const;
    [[nodiscard]] double get_float() const;
    [[nodiscard]] std::string get_str() const;
    [[nodiscard]] JsonCursor field(std::string_view key) const;
    [[nodiscard]] JsonCursor at(size_t index) const;

    // --- Container inspection ----------------------------------------------
    [[nodiscard]] size_t array_size() const noexcept;
    [[nodiscard]] size_t object_size() const noexcept;
    [[nodiscard]] std::vector<std::string> object_keys() const;

    /// The borrowed value, or nullptr for a default-constructed cursor.
    [[nodiscard]] const JsonValue* raw() const noexcept { return value_; }

  private:
    const JsonValue* value_ = nullptr;
};

} // namespace strata

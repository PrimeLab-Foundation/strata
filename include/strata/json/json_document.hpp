#pragma once

/**
 * @file json_document.hpp
 * @brief Owning handle for a parsed JSON document.
 *
 * JsonDocument stores the root JsonValue via shared_ptr and provides
 * a cursor interface for navigation.  Shared ownership ensures the
 * underlying value tree stays alive while any cursor references it.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"
#include "strata/json/json_parse.hpp"

#include <memory>
#include <string_view>

namespace strata {

/**
 * Owning handle for a parsed JSON document.
 *
 * Stores the root JsonValue and provides a cursor for navigation.
 */
class JsonDocument {
  public:
    /// Construct from an already-built JsonValue tree.
    explicit JsonDocument(JsonValue root);

    /// Factory: parse JSON text into a document.
    [[nodiscard]] static Result<JsonDocument> from_string(std::string_view text);

    /// Get a cursor positioned at the root value.
    [[nodiscard]] JsonCursor root() const;

    /// Access the underlying root JsonValue (const).
    [[nodiscard]] const JsonValue& root_value() const;

    /// Debug helper: returns a human-readable type name for the root.
    [[nodiscard]] std::string root_type_debug() const;

  private:
    std::shared_ptr<JsonValue> root_;
};

} // namespace strata

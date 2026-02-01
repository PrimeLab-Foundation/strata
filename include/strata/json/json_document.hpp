#pragma once

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
    /// Construct from an already-built JsonValue tree
    explicit JsonDocument(JsonValue root);

    /// Factory: parse JSON text into a document
    static Result<JsonDocument> from_string(std::string_view text);

    /// Get a cursor at the root value
    JsonCursor root() const;

    /// Access underlying JsonValue (const)
    const JsonValue& root_value() const;

    std::string root_type_debug() const;

  private:
    std::shared_ptr<JsonValue> root_;
};

} // namespace strata

#pragma once

/**
 * @file json_document.hpp
 * @brief Owning handle for a parsed JSON document.
 *
 * The root is held through a `shared_ptr`, so a cursor can keep the tree alive
 * simply by holding a copy of that share. That is how the binding layer honours
 * the document-outlives-cursor invariant without exposing it to the caller
 * (docs/context/api.md, § Cursor).
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"

#include <memory>
#include <string_view>

namespace strata {

class JsonDocument {
  public:
    /// Adopt an already-built tree.
    explicit JsonDocument(JsonValue root) : root_(std::make_shared<JsonValue>(std::move(root))) {}

    /// Parse @p text; Status::ParseError for malformed input, Status::DepthExceeded
    /// for a document nested deeper than kMaxNestingDepth containers.
    [[nodiscard]] static Result<JsonDocument> from_string(std::string_view text);

    /// A cursor at the root. Valid for as long as this document, or any share
    /// of it taken through `share()`, is alive.
    [[nodiscard]] JsonCursor root() const { return JsonCursor(root_.get()); }

    [[nodiscard]] const JsonValue& root_value() const { return *root_; }

    /// A share of the tree's ownership, for holders that outlive the document.
    [[nodiscard]] std::shared_ptr<const JsonValue> share() const { return root_; }

  private:
    std::shared_ptr<JsonValue> root_;
};

} // namespace strata

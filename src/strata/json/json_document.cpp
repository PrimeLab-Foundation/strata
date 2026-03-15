/**
 * @file json_document.cpp
 * @brief JsonDocument implementation — owning handle for parsed JSON.
 *
 * The root JsonValue is wrapped in a shared_ptr so that cursors,
 * which hold a borrowed pointer, remain valid as long as any
 * copy of the document exists.
 */

#include "strata/json/json_document.hpp"

namespace strata {

JsonDocument::JsonDocument(JsonValue root) : root_(std::make_shared<JsonValue>(std::move(root))) {}

Result<JsonDocument> JsonDocument::from_string(std::string_view text) {
    auto parsed = parse_json(text);
    if (!parsed.ok()) {
        return {parsed.status, JsonDocument(JsonValue{})};
    }
    return {Status::Ok, JsonDocument(std::move(parsed.value))};
}

JsonCursor JsonDocument::root() const { return JsonCursor(root_.get()); }

const JsonValue& JsonDocument::root_value() const { return *root_; }

std::string JsonDocument::root_type_debug() const {
    if (root_->is_null())
        return "null";
    if (root_->is_bool())
        return "bool";
    if (root_->is_number())
        return "number";
    if (root_->is_string())
        return "string";
    if (root_->is_array())
        return "array";
    if (root_->is_object())
        return "object";
    return "unknown";
}

} // namespace strata

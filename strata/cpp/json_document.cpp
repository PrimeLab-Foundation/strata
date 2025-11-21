#include "json_document.hpp"

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

} // namespace strata

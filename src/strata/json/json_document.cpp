/**
 * @file json_document.cpp
 * @brief JsonDocument construction.
 */

#include "strata/json/json_document.hpp"

#include "strata/json/json_parse.hpp"

#include <utility>

namespace strata {

Result<JsonDocument> JsonDocument::from_string(std::string_view text) {
    auto parsed = parse_json(text);
    if (!parsed.ok())
        return {parsed.status, JsonDocument(JsonValue{})};
    return {Status::Ok, JsonDocument(std::move(parsed.value))};
}

} // namespace strata

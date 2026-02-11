#pragma once

#include "strata/json/json_parse.hpp"
#include "strata/json/json_core.hpp"

#include <string_view>
#include <vector>

namespace strata {

enum class SimpleFieldMode {
    RootField,
    RootWildcardField,
};

// Extract matches for simple field paths ($.field or $[*].field) without building full DOM.
Status extract_simple_field_matches(std::string_view text, std::string_view field,
                                   SimpleFieldMode mode, std::vector<JsonValue>& matches,
                                   const ParseSaxOptions& options, ParseSaxContext* context);

} // namespace strata

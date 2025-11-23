#include "partitioner.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>

namespace strata {

// Helper: Skip whitespace
static size_t skip_whitespace(std::string_view s, size_t pos) {
    while (pos < s.size() &&
           (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) {
        ++pos;
    }
    return pos;
}

// Helper: Parse a string literal, handling escapes
// Returns position after closing quote, or s.size() if malformed
static size_t skip_string(std::string_view s, size_t pos) {
    if (pos >= s.size() || s[pos] != '"')
        return s.size();
    ++pos; // skip opening quote

    while (pos < s.size()) {
        char c = s[pos];
        if (c == '"') {
            return pos + 1; // skip closing quote
        } else if (c == '\\') {
            // Skip escape sequence
            ++pos;
            if (pos < s.size()) {
                char esc = s[pos];
                if (esc == 'u') {
                    // Unicode escape: \uXXXX
                    pos += 5; // skip 'u' + 4 hex digits
                } else {
                    ++pos; // skip escaped character
                }
            }
        } else {
            ++pos;
        }
    }
    return s.size(); // unterminated string
}

// Helper: Find the array value for a given top_key in root object
// Returns [array_start, array_end) positions, or throws if not found/malformed
static std::pair<size_t, size_t> find_topkey_array(std::string_view s, std::string_view top_key) {
    size_t pos = skip_whitespace(s, 0);

    // Expect opening {
    if (pos >= s.size() || s[pos] != '{') {
        throw std::runtime_error("Expected JSON object at root");
    }
    ++pos;

    // Search for "top_key": [ ... ]
    while (pos < s.size()) {
        pos = skip_whitespace(s, pos);

        // Check for end of object
        if (pos < s.size() && s[pos] == '}') {
            break;
        }

        // Expect a string key
        if (pos >= s.size() || s[pos] != '"') {
            throw std::runtime_error("Expected string key in root object");
        }

        size_t key_start = pos + 1;
        pos = skip_string(s, pos);
        if (pos >= s.size()) {
            throw std::runtime_error("Unterminated key string");
        }
        size_t key_end = pos - 1; // before closing quote

        std::string_view key(&s[key_start], key_end - key_start);

        // Skip colon
        pos = skip_whitespace(s, pos);
        if (pos >= s.size() || s[pos] != ':') {
            throw std::runtime_error("Expected ':' after key");
        }
        ++pos;
        pos = skip_whitespace(s, pos);

        // Check if this is our target key
        if (key == top_key) {
            // Expect array
            if (pos >= s.size() || s[pos] != '[') {
                throw std::runtime_error("Expected array value for top_key '" +
                                         std::string(top_key) + "'");
            }

            // Find matching closing bracket
            size_t array_start = pos;
            ++pos; // skip '['
            int depth = 1;

            while (pos < s.size() && depth > 0) {
                char c = s[pos];
                if (c == '"') {
                    pos = skip_string(s, pos);
                } else if (c == '[') {
                    ++depth;
                    ++pos;
                } else if (c == ']') {
                    --depth;
                    ++pos;
                } else if (c == '{') {
                    ++pos;
                } else if (c == '}') {
                    ++pos;
                } else {
                    ++pos;
                }
            }

            if (depth != 0) {
                throw std::runtime_error("Unterminated array for top_key '" + std::string(top_key) +
                                         "'");
            }

            return {array_start, pos}; // [start of '[', position after ']')
        } else {
            // Skip this value (could be object, array, string, number, etc.)
            // Simple approach: track nesting
            int depth = 0;
            bool in_string = false;

            while (pos < s.size()) {
                char c = s[pos];

                if (in_string) {
                    if (c == '"' && (pos == 0 || s[pos - 1] != '\\')) {
                        in_string = false;
                    } else if (c == '\\') {
                        ++pos; // skip next char
                        if (pos < s.size())
                            ++pos;
                        continue;
                    }
                    ++pos;
                } else {
                    if (c == '"') {
                        pos = skip_string(s, pos);
                    } else if (c == '{' || c == '[') {
                        ++depth;
                        ++pos;
                    } else if (c == '}' || c == ']') {
                        if (depth == 0)
                            break; // end of root object
                        --depth;
                        ++pos;
                    } else if (c == ',' && depth == 0) {
                        ++pos;
                        break; // next key-value pair
                    } else {
                        ++pos;
                    }
                }
            }
        }
    }

    throw std::runtime_error("Top key '" + std::string(top_key) + "' not found in root object");
}

std::vector<ElementSpan> index_topkey_array(std::string_view json, std::string_view top_key) {
    std::vector<ElementSpan> spans;

    // Find the array for top_key
    auto [array_start, array_end] = find_topkey_array(json, top_key);

    // Parse elements inside [ ... ]
    size_t pos = array_start + 1; // skip '['
    size_t elem_index = 0;

    while (pos < array_end - 1) { // -1 to not go past ']'
        pos = skip_whitespace(json, pos);
        if (pos >= array_end - 1)
            break;

        // Mark element start
        size_t elem_start = pos;
        int depth = 0;
        bool in_string = false;

        // Find element end (comma at depth 0 or closing bracket)
        while (pos < array_end - 1) {
            char c = json[pos];

            if (in_string) {
                if (c == '"' && (pos == 0 || json[pos - 1] != '\\')) {
                    in_string = false;
                    ++pos;
                } else if (c == '\\') {
                    pos += 2; // skip escape sequence
                } else {
                    ++pos;
                }
            } else {
                if (c == '"') {
                    in_string = true;
                    ++pos;
                } else if (c == '{' || c == '[') {
                    ++depth;
                    ++pos;
                } else if (c == '}' || c == ']') {
                    --depth;
                    ++pos;
                } else if (c == ',' && depth == 0) {
                    // Found element boundary
                    break;
                } else {
                    ++pos;
                }
            }
        }

        // elem_start to pos is one element (excluding trailing comma)
        size_t elem_end = pos;

        // Trim trailing whitespace from element
        while (elem_end > elem_start &&
               (json[elem_end - 1] == ' ' || json[elem_end - 1] == '\t' ||
                json[elem_end - 1] == '\n' || json[elem_end - 1] == '\r')) {
            --elem_end;
        }

        if (elem_end > elem_start) {
            spans.push_back(ElementSpan{
                .elem_index = elem_index, .offset = elem_start, .length = elem_end - elem_start});
            ++elem_index;
        }

        // Skip comma if present
        pos = skip_whitespace(json, pos);
        if (pos < array_end - 1 && json[pos] == ',') {
            ++pos;
        }
    }

    return spans;
}

ExecutionPlan pack_topkey_partitions(const std::vector<ElementSpan>& spans, size_t target_items,
                                     size_t target_bytes) {
    ExecutionPlan plan;
    if (spans.empty())
        return plan;

    // Enforce minimums
    target_items = std::max(target_items, size_t(1));
    target_bytes = std::max(target_bytes, size_t(1 << 16)); // 64 KB min

    size_t begin_idx = 0;
    size_t begin_offset = spans[0].offset;

    for (size_t i = 0; i < spans.size(); ++i) {
        size_t item_count = (i + 1) - begin_idx;
        size_t end_offset = spans[i].offset + spans[i].length;
        size_t byte_count = end_offset - begin_offset;

        bool hit_target = (item_count >= target_items) || (byte_count >= target_bytes);
        bool last_elem = (i + 1 == spans.size());

        if (hit_target || last_elem) {
            // Close partition
            plan.parts.push_back(Partition{.offset = begin_offset,
                                           .length = end_offset - begin_offset,
                                           .row_begin = begin_idx,
                                           .row_end = i + 1});

            if (!last_elem) {
                begin_idx = i + 1;
                begin_offset = spans[begin_idx].offset;
            }
        }
    }

    return plan;
}

ExecutionPlan build_topkey_plan(std::string_view json, std::string_view top_key,
                                size_t target_items, size_t target_bytes) {
    auto spans = index_topkey_array(json, top_key);
    return pack_topkey_partitions(spans, target_items, target_bytes);
}

} // namespace strata

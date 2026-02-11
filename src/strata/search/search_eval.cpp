#include "strata/search/search.hpp"

#include <algorithm>
#include <deque>
#include <limits>

namespace strata {

// ============================================================================
// JSONPath Evaluator
// ============================================================================

// Helper to materialize a JsonValue from a cursor (deep copy)
static JsonValue materialize(const JsonCursor& cursor) {
    if (cursor.is_null()) {
        return JsonValue();
    } else if (cursor.is_bool()) {
        return JsonValue(JsonValue::Variant(cursor.get_bool_or_throw()));
    } else if (cursor.is_number()) {
        const JsonValue* raw = cursor.raw();
        if (raw && raw->is_int()) {
            return JsonValue(JsonValue::Variant(raw->as_int()));
        }
        return JsonValue(JsonValue::Variant(cursor.get_float()));
    } else if (cursor.is_string()) {
        return JsonValue(JsonValue::Variant(cursor.get_str()));
    } else if (cursor.is_array()) {
        JsonValue::Array arr;
        size_t len = cursor.array_size();
        arr.reserve(len);
        for (size_t i = 0; i < len; ++i) {
            arr.push_back(materialize(cursor.at(i)));
        }
        return JsonValue(JsonValue::Variant(std::move(arr)));
    } else if (cursor.is_object()) {
        JsonValue::Object obj;
        auto keys = cursor.object_keys();
        for (const auto& key : keys) {
            obj[key] = materialize(cursor.field(key));
        }
        return JsonValue(JsonValue::Variant(std::move(obj)));
    }
    return JsonValue();
}

// Helper to collect all values from a cursor
static void collect_all_values(const JsonCursor& cursor, std::vector<JsonValue>& results) {
    results.push_back(materialize(cursor));
}

// Helper to evaluate a filter predicate on a cursor
static bool eval_filter(const JsonCursor& cursor, const FilterPredicate& filter) {
    if (!cursor.is_object()) {
        return false;
    }

    try {
        JsonCursor field_cursor = cursor.field(filter.field);

        if (filter.is_numeric) {
            if (!field_cursor.is_number()) {
                return false;
            }
            double value = field_cursor.get_float();

            switch (filter.op) {
            case FilterOp::Equal:
                return value == filter.numeric_value;
            case FilterOp::NotEqual:
                return value != filter.numeric_value;
            case FilterOp::GreaterThan:
                return value > filter.numeric_value;
            case FilterOp::GreaterEqual:
                return value >= filter.numeric_value;
            case FilterOp::LessThan:
                return value < filter.numeric_value;
            case FilterOp::LessEqual:
                return value <= filter.numeric_value;
            default:
                return false;
            }
        } else {
            if (!field_cursor.is_string()) {
                return false;
            }
            std::string value = field_cursor.get_str();

            switch (filter.op) {
            case FilterOp::Equal:
                return value == filter.string_value;
            case FilterOp::NotEqual:
                return value != filter.string_value;
            default:
                return false; // Other ops not supported for strings
            }
        }
    } catch (...) {
        // Field not found or access error
        return false;
    }
}

// Helper for recursive descent - finds all matching cursors
// Uses iterative BFS for better cache locality
// Returns true if limit was reached, false otherwise
static bool collect_recursive_cursors_bfs(const JsonCursor& cursor, const std::string& field_name,
                                          std::vector<JsonCursor>& cursors,
                                          size_t limit = std::numeric_limits<size_t>::max()) {
    // Use a queue for BFS traversal - better cache locality than DFS
    std::deque<JsonCursor> queue;
    queue.push_back(cursor);

    while (!queue.empty()) {
        // Check if we've reached the limit
        if (cursors.size() >= limit) {
            return true; // Early termination
        }

        JsonCursor current = queue.front();
        queue.pop_front();

        if (current.is_object()) {
            // Check if current node has the target field
            try {
                JsonCursor child = current.field(field_name);
                cursors.push_back(child);
                // Check limit after adding
                if (cursors.size() >= limit) {
                    return true; // Early termination
                }
            } catch (...) {
                // Field not found at this level
            }

            // Add all object values to queue for further processing
            auto keys = current.object_keys();
            for (const auto& key : keys) {
                try {
                    JsonCursor child = current.field(key);
                    queue.push_back(child);
                } catch (...) {
                    // Skip on error
                }
            }
        } else if (current.is_array()) {
            // Add all array elements to queue
            size_t len = current.array_size();
            for (size_t i = 0; i < len; ++i) {
                try {
                    JsonCursor child = current.at(i);
                    queue.push_back(child);
                } catch (...) {
                    // Skip on error
                }
            }
        }
    }

    return false; // Did not reach limit
}

// Evaluation helper with limit support
// Returns true if limit was reached (early termination)
static bool eval_step_with_limit(const JsonCursor& cursor, const std::vector<PathStep>& steps,
                                 size_t step_idx, std::vector<JsonValue>& results, size_t limit) {

    // Check limit before processing
    if (results.size() >= limit) {
        return true;
    }

    if (step_idx >= steps.size()) {
        // End of path - collect this value
        collect_all_values(cursor, results);
        return results.size() >= limit;
    }

    const PathStep& step = steps[step_idx];

    switch (step.op) {
    case PathOp::Root:
        // Just move to next step
        return eval_step_with_limit(cursor, steps, step_idx + 1, results, limit);

    case PathOp::Field:
        if (cursor.is_object()) {
            try {
                JsonCursor child = cursor.field(step.field);
                return eval_step_with_limit(child, steps, step_idx + 1, results, limit);
            } catch (...) {
                // Field not found - no results for this path
            }
        }
        break;

    case PathOp::Index:
        if (cursor.is_array()) {
            try {
                size_t len = cursor.array_size();
                int64_t idx = step.index;

                // Handle negative indices
                if (idx < 0) {
                    idx = static_cast<int64_t>(len) + idx;
                }

                if (idx >= 0 && idx < static_cast<int64_t>(len)) {
                    JsonCursor child = cursor.at(static_cast<size_t>(idx));
                    return eval_step_with_limit(child, steps, step_idx + 1, results, limit);
                }
            } catch (...) {
                // Index out of bounds - no results
            }
        }
        break;

    case PathOp::Wildcard:
        if (cursor.is_array()) {
            // Iterate all array elements
            size_t len = cursor.array_size();
            for (size_t i = 0; i < len; ++i) {
                if (results.size() >= limit) {
                    return true;
                }
                try {
                    JsonCursor child = cursor.at(i);
                    if (eval_step_with_limit(child, steps, step_idx + 1, results, limit)) {
                        return true;
                    }
                } catch (...) {
                    // Skip on error
                }
            }
        } else if (cursor.is_object()) {
            // Iterate all object values
            auto keys = cursor.object_keys();
            for (const auto& key : keys) {
                if (results.size() >= limit) {
                    return true;
                }
                try {
                    JsonCursor child = cursor.field(key);
                    if (eval_step_with_limit(child, steps, step_idx + 1, results, limit)) {
                        return true;
                    }
                } catch (...) {
                    // Skip on error
                }
            }
        }
        break;

    case PathOp::RecursiveDescent: {
        // Use BFS with limit for recursive descent
        std::vector<JsonCursor> found_cursors;

        // Calculate remaining limit for collection phase
        // If there are more steps after this, we need all matches
        // If this is the last step (before End or end of path), we can limit collection
        bool is_terminal = (step_idx + 1 >= steps.size()) ||
                           (step_idx + 1 < steps.size() && steps[step_idx + 1].op == PathOp::End);

        size_t collection_limit = is_terminal ? (limit - results.size()) : std::numeric_limits<size_t>::max();
        collect_recursive_cursors_bfs(cursor, step.field, found_cursors, collection_limit);

        // Continue evaluation with remaining steps for each found cursor
        for (const auto& found : found_cursors) {
            if (results.size() >= limit) {
                return true;
            }
            if (eval_step_with_limit(found, steps, step_idx + 1, results, limit)) {
                return true;
            }
        }
        break;
    }

    case PathOp::Slice:
        if (cursor.is_array()) {
            size_t len = cursor.array_size();
            int64_t start = step.slice_start;
            int64_t end = step.slice_end;
            int64_t step_size = step.slice_step;

            // Handle negative indices
            if (start < 0) {
                start = static_cast<int64_t>(len) + start;
            }
            if (end < 0) {
                end = static_cast<int64_t>(len) + end;
            }

            // Clamp to valid range
            start = std::max(int64_t(0), std::min(start, static_cast<int64_t>(len)));
            end = std::max(int64_t(0), std::min(end, static_cast<int64_t>(len)));

            if (step_size > 0) {
                for (int64_t i = start; i < end; i += step_size) {
                    if (results.size() >= limit) {
                        return true;
                    }
                    try {
                        JsonCursor child = cursor.at(static_cast<size_t>(i));
                        if (eval_step_with_limit(child, steps, step_idx + 1, results, limit)) {
                            return true;
                        }
                    } catch (...) {
                        // Skip on error
                    }
                }
            }
        }
        break;

    case PathOp::Filter:
        if (cursor.is_array()) {
            // Filter array elements based on predicate
            size_t len = cursor.array_size();
            for (size_t i = 0; i < len; ++i) {
                if (results.size() >= limit) {
                    return true;
                }
                try {
                    JsonCursor child = cursor.at(i);
                    if (eval_filter(child, step.filter)) {
                        if (eval_step_with_limit(child, steps, step_idx + 1, results, limit)) {
                            return true;
                        }
                    }
                } catch (...) {
                    // Skip on error
                }
            }
        }
        break;

    case PathOp::End:
        collect_all_values(cursor, results);
        return results.size() >= limit;
    }

    return results.size() >= limit;
}

// Legacy evaluation helper (no limit)
static void eval_step(const JsonCursor& cursor, const std::vector<PathStep>& steps, size_t step_idx,
                      std::vector<JsonValue>& results) {
    eval_step_with_limit(cursor, steps, step_idx, results, std::numeric_limits<size_t>::max());
}

std::vector<JsonValue> eval_search_path(const JsonDocument& doc, const CompiledPath& path) {
    return eval_search_path(doc.root(), path);
}

// Helper to check if path has early-exit potential (starts with $.field)
static bool has_root_field_access(const std::vector<PathStep>& steps, std::string& field_name) {
    if (steps.size() >= 2 &&
        steps[0].op == PathOp::Root &&
        steps[1].op == PathOp::Field) {
        field_name = steps[1].field;
        return true;
    }
    return false;
}

std::vector<JsonValue> eval_search_path(const JsonCursor& cursor, const CompiledPath& path) {
    std::vector<JsonValue> results;

    if (path.empty()) {
        return results;
    }

    // Early-exit optimization: check if root-level field exists before full evaluation
    std::string root_field;
    if (has_root_field_access(path.steps(), root_field)) {
        if (!cursor.is_object()) {
            return results;  // Not an object, field access will fail
        }
        // Use non-throwing get_field to check if field exists
        auto field_result = cursor.get_field(root_field);
        if (!field_result.ok()) {
            return results;  // Root field doesn't exist, no results
        }
    }

    eval_step(cursor, path.steps(), 0, results);
    return results;
}

std::vector<JsonValue> eval_search_path(const JsonDocument& doc, const CompiledPath& path, size_t limit) {
    return eval_search_path(doc.root(), path, limit);
}

std::vector<JsonValue> eval_search_path(const JsonCursor& cursor, const CompiledPath& path, size_t limit) {
    std::vector<JsonValue> results;

    if (path.empty() || limit == 0) {
        return results;
    }

    // Early-exit optimization: check if root-level field exists before full evaluation
    std::string root_field;
    if (has_root_field_access(path.steps(), root_field)) {
        if (!cursor.is_object()) {
            return results;  // Not an object, field access will fail
        }
        // Use non-throwing get_field to check if field exists
        auto field_result = cursor.get_field(root_field);
        if (!field_result.ok()) {
            return results;  // Root field doesn't exist, no results
        }
    }

    results.reserve(std::min(limit, size_t(64))); // Pre-allocate reasonable size
    eval_step_with_limit(cursor, path.steps(), 0, results, limit);
    return results;
}

} // namespace strata

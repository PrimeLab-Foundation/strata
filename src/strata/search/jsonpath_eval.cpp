/**
 * @file jsonpath_eval.cpp
 * @brief JSONPath evaluator — walks compiled path steps over a JsonValue tree.
 *
 * Evaluation strategy:
 * - Uses direct const JsonValue* access throughout (no cursors, no
 *   exceptions) to minimise overhead in the recursive descent.
 * - Results are **materialised** (deep-copied) into the output vector
 *   so that the caller does not need to worry about JsonValue lifetimes.
 * - RecursiveDescent collects all matching values depth-first.
 * - Filter evaluation compares a child field value against a literal;
 *   mismatches are silently skipped.
 */

#include "strata/search/jsonpath.hpp"

#include <algorithm>

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
    } else if (cursor.raw() && cursor.raw()->is_int64()) {
        return JsonValue(JsonValue::Variant(cursor.raw()->as_int64()));
    } else if (cursor.is_number()) {
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

// Helper for recursive descent - works directly with JsonValue* to avoid
// exception overhead, string allocations, and redundant lookups.
static void collect_recursive_values(const JsonValue* value, const std::string& field_name,
                                     std::vector<JsonCursor>& cursors) {
    if (!value)
        return;

    if (value->is_object()) {
        const auto& obj = value->as_object();
        for (const auto& pair : obj) {
            if (pair.first == field_name) {
                cursors.emplace_back(&pair.second);
            }
            // Recurse into all values (including matched ones - their children may also match)
            collect_recursive_values(&pair.second, field_name, cursors);
        }
    } else if (value->is_array()) {
        const auto& arr = value->as_array();
        for (const auto& elem : arr) {
            collect_recursive_values(&elem, field_name, cursors);
        }
    }
}

// Recursive evaluation helper
static void eval_step(const JsonCursor& cursor, const std::vector<PathStep>& steps, size_t step_idx,
                      std::vector<JsonValue>& results) {

    if (step_idx >= steps.size()) {
        // End of path - collect this value
        collect_all_values(cursor, results);
        return;
    }

    const PathStep& step = steps[step_idx];

    switch (step.op) {
    case PathOp::Root:
        // Just move to next step
        eval_step(cursor, steps, step_idx + 1, results);
        break;

    case PathOp::Field:
        if (cursor.is_object()) {
            try {
                JsonCursor child = cursor.field(step.field);
                eval_step(child, steps, step_idx + 1, results);
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
                    eval_step(child, steps, step_idx + 1, results);
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
                try {
                    JsonCursor child = cursor.at(i);
                    eval_step(child, steps, step_idx + 1, results);
                } catch (...) {
                    // Skip on error
                }
            }
        } else if (cursor.is_object()) {
            // Iterate all object values
            auto keys = cursor.object_keys();
            for (const auto& key : keys) {
                try {
                    JsonCursor child = cursor.field(key);
                    eval_step(child, steps, step_idx + 1, results);
                } catch (...) {
                    // Skip on error
                }
            }
        }
        break;

    case PathOp::RecursiveDescent: {
        // Recursively find all instances of the field
        std::vector<JsonCursor> found_cursors;
        collect_recursive_values(cursor.raw(), step.field, found_cursors);

        // Continue evaluation with remaining steps for each found cursor
        for (const auto& found : found_cursors) {
            eval_step(found, steps, step_idx + 1, results);
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
                    try {
                        JsonCursor child = cursor.at(static_cast<size_t>(i));
                        eval_step(child, steps, step_idx + 1, results);
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
                try {
                    JsonCursor child = cursor.at(i);
                    if (eval_filter(child, step.filter)) {
                        eval_step(child, steps, step_idx + 1, results);
                    }
                } catch (...) {
                    // Skip on error
                }
            }
        }
        break;

    case PathOp::End:
        collect_all_values(cursor, results);
        break;
    }
}

std::vector<JsonValue> eval_jsonpath(const JsonDocument& doc, const CompiledPath& path) {
    return eval_jsonpath(doc.root(), path);
}

std::vector<JsonValue> eval_jsonpath(const JsonCursor& cursor, const CompiledPath& path) {
    std::vector<JsonValue> results;

    if (path.empty()) {
        return results;
    }

    eval_step(cursor, path.steps(), 0, results);
    return results;
}

} // namespace strata

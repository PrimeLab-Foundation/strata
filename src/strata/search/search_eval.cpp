#include "strata/search/search.hpp"
#include "strata/util/simd_string.hpp"

#include <algorithm>
#include <cstring>
#include <deque>
#include <limits>

namespace strata {

// ============================================================================
// JSONPath Evaluator
// ============================================================================

static bool try_get_field_simd(const JsonCursor& cursor, std::string_view field, JsonCursor& out) {
    const JsonValue* raw = cursor.raw();
    if (!raw || !raw->is_object()) {
        return false;
    }

    const auto& obj = raw->as_object();
    const size_t target_len = field.size();
    if (target_len == 0) {
        return false;
    }

    if (target_len <= 16) {
        for (const auto& kv : obj) {
            const std::string& key = kv.first;
            if (key.size() != target_len) {
                continue;
            }
            if (util::simd_string_eq(std::string_view(key.data(), key.size()), field)) {
                out = JsonCursor(&kv.second);
                return true;
            }
        }
        return false;
    }

    for (const auto& kv : obj) {
        const std::string& key = kv.first;
        if (key.size() != target_len) {
            continue;
        }
        if (std::memcmp(key.data(), field.data(), target_len) == 0) {
            out = JsonCursor(&kv.second);
            return true;
        }
    }

    return false;
}

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

// Collector helpers
static void collect_result(const JsonCursor& cursor, std::vector<JsonValue>& results) {
    results.push_back(materialize(cursor));
}

static void collect_result(const JsonCursor& cursor, std::vector<JsonCursor>& results) {
    results.push_back(cursor);
}

// Helper to evaluate a filter predicate on a cursor
static bool eval_filter(const JsonCursor& cursor, const FilterPredicate& filter) {
    if (!cursor.is_object()) {
        return false;
    }

    if (filter.op == FilterOp::Exists) {
        JsonCursor found(nullptr);
        return try_get_field_simd(cursor, filter.field, found);
    }

    JsonCursor field_cursor(nullptr);
    if (!try_get_field_simd(cursor, filter.field, field_cursor)) {
        return false;
    }

    FilterValueType value_type = filter.value_type;
    if (value_type == FilterValueType::Unspecified) {
        value_type = filter.is_numeric ? FilterValueType::Numeric : FilterValueType::String;
    }

    switch (value_type) {
    case FilterValueType::Numeric: {
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
    }
    case FilterValueType::String: {
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
    case FilterValueType::Boolean: {
        if (!field_cursor.is_bool()) {
            return false;
        }
        bool value = field_cursor.get_bool_or_throw();
        switch (filter.op) {
        case FilterOp::Equal:
            return value == filter.bool_value;
        case FilterOp::NotEqual:
            return value != filter.bool_value;
        default:
            return false;
        }
    }
    case FilterValueType::Null: {
        switch (filter.op) {
        case FilterOp::Equal:
            return field_cursor.is_null();
        case FilterOp::NotEqual:
            return !field_cursor.is_null();
        default:
            return false;
        }
    }
    default:
        return false;
    }
}

// Helper for recursive descent - finds all matching cursors
static bool collect_recursive_cursors_bfs(const JsonCursor& cursor, const std::string& field_name,
                                          std::vector<JsonCursor>& cursors,
                                          size_t limit = std::numeric_limits<size_t>::max()) {
    std::deque<JsonCursor> queue;
    queue.push_back(cursor);

    while (!queue.empty()) {
        if (cursors.size() >= limit) {
            return true;
        }

        JsonCursor current = queue.front();
        queue.pop_front();

        if (current.is_object()) {
            JsonCursor child(nullptr);
            if (try_get_field_simd(current, field_name, child)) {
                cursors.push_back(child);
                if (cursors.size() >= limit) {
                    return true;
                }
            }

            auto keys = current.object_keys();
            for (const auto& key : keys) {
                try {
                    JsonCursor child = current.field(key);
                    queue.push_back(child);
                } catch (...) {}
            }
        } else if (current.is_array()) {
            size_t len = current.array_size();
            for (size_t i = 0; i < len; ++i) {
                try {
                    JsonCursor child = current.at(i);
                    queue.push_back(child);
                } catch (...) {}
            }
        }
    }

    return false;
}

// Evaluation helper template
template <typename T>
static bool eval_step_with_limit_impl(const JsonCursor& cursor, const std::vector<PathStep>& steps,
                                      size_t step_idx, std::vector<T>& results, size_t limit) {
    if (results.size() >= limit) {
        return true;
    }

    if (step_idx >= steps.size()) {
        collect_result(cursor, results);
        return results.size() >= limit;
    }

    const PathStep& step = steps[step_idx];

    switch (step.op) {
    case PathOp::Root:
        return eval_step_with_limit_impl(cursor, steps, step_idx + 1, results, limit);

    case PathOp::Field:
        if (cursor.is_object()) {
            JsonCursor child(nullptr);
            if (try_get_field_simd(cursor, step.field, child)) {
                return eval_step_with_limit_impl(child, steps, step_idx + 1, results, limit);
            }
        }
        break;

    case PathOp::Index:
        if (cursor.is_array()) {
            try {
                size_t len = cursor.array_size();
                int64_t idx = step.index;
                if (idx < 0) idx = static_cast<int64_t>(len) + idx;
                if (idx >= 0 && idx < static_cast<int64_t>(len)) {
                    JsonCursor child = cursor.at(static_cast<size_t>(idx));
                    return eval_step_with_limit_impl(child, steps, step_idx + 1, results, limit);
                }
            } catch (...) {}
        }
        break;

    case PathOp::Wildcard:
        if (cursor.is_array()) {
            size_t len = cursor.array_size();

            // Fused Wildcard + Field optimization
            if (step_idx + 1 < steps.size() && steps[step_idx + 1].op == PathOp::Field) {
                const std::string& field = steps[step_idx + 1].field;
                for (size_t i = 0; i < len; ++i) {
                    if (results.size() >= limit) return true;
                    JsonCursor item = cursor.at(i);
                    if (item.is_object()) {
                        JsonCursor child(nullptr);
                        if (try_get_field_simd(item, field, child)) {
                            if (eval_step_with_limit_impl(child, steps, step_idx + 2, results, limit)) {
                                return true;
                            }
                        }
                    }
                }
                return results.size() >= limit;
            }

            for (size_t i = 0; i < len; ++i) {
                if (results.size() >= limit) return true;
                try {
                    JsonCursor child = cursor.at(i);
                    if (eval_step_with_limit_impl(child, steps, step_idx + 1, results, limit)) return true;
                } catch (...) {}
            }
        } else if (cursor.is_object()) {
            auto keys = cursor.object_keys();
            for (const auto& key : keys) {
                if (results.size() >= limit) return true;
                try {
                    JsonCursor child = cursor.field(key);
                    if (eval_step_with_limit_impl(child, steps, step_idx + 1, results, limit)) return true;
                } catch (...) {}
            }
        }
        break;

    case PathOp::RecursiveDescent: {
        std::vector<JsonCursor> found_cursors;
        bool is_terminal = (step_idx + 1 >= steps.size()) || (steps[step_idx + 1].op == PathOp::End);
        size_t collection_limit = is_terminal ? (limit - results.size()) : std::numeric_limits<size_t>::max();
        collect_recursive_cursors_bfs(cursor, step.field, found_cursors, collection_limit);

        for (const auto& found : found_cursors) {
            if (results.size() >= limit) return true;
            if (eval_step_with_limit_impl(found, steps, step_idx + 1, results, limit)) return true;
        }
        break;
    }

    case PathOp::Slice:
        if (cursor.is_array()) {
            size_t len = cursor.array_size();
            int64_t start = step.slice_start;
            int64_t end = step.slice_end;
            int64_t step_size = step.slice_step;
            if (start < 0) start = static_cast<int64_t>(len) + start;
            if (end < 0) end = static_cast<int64_t>(len) + end;
            start = std::max(int64_t(0), std::min(start, static_cast<int64_t>(len)));
            end = std::max(int64_t(0), std::min(end, static_cast<int64_t>(len)));

            if (step_size > 0) {
                for (int64_t i = start; i < end; i += step_size) {
                    if (results.size() >= limit) return true;
                    try {
                        JsonCursor child = cursor.at(static_cast<size_t>(i));
                        if (eval_step_with_limit_impl(child, steps, step_idx + 1, results, limit)) return true;
                    } catch (...) {}
                }
            }
        }
        break;

    case PathOp::Filter:
        if (cursor.is_array()) {
            size_t len = cursor.array_size();
            for (size_t i = 0; i < len; ++i) {
                if (results.size() >= limit) return true;
                try {
                    JsonCursor child = cursor.at(i);
                    if (eval_filter(child, step.filter)) {
                        if (eval_step_with_limit_impl(child, steps, step_idx + 1, results, limit)) return true;
                    }
                } catch (...) {}
            }
        }
        break;

    case PathOp::End:
        collect_result(cursor, results);
        return results.size() >= limit;
    }

    return results.size() >= limit;
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

// ----------------------------------------------------------------------------
// Public API Implementations (JsonValue results)
// ----------------------------------------------------------------------------

std::vector<JsonValue> eval_search_path(const JsonDocument& doc, const CompiledPath& path) {
    return eval_search_path(doc.root(), path);
}

std::vector<JsonValue> eval_search_path(const JsonCursor& cursor, const CompiledPath& path) {
    return eval_search_path(cursor, path, std::numeric_limits<size_t>::max());
}

std::vector<JsonValue> eval_search_path(const JsonDocument& doc, const CompiledPath& path, size_t limit) {
    return eval_search_path(doc.root(), path, limit);
}

std::vector<JsonValue> eval_search_path(const JsonCursor& cursor, const CompiledPath& path, size_t limit) {
    std::vector<JsonValue> results;
    if (path.empty() || limit == 0) return results;

    std::string root_field;
    if (has_root_field_access(path.steps(), root_field)) {
        if (!cursor.is_object()) return results;
        auto field_result = cursor.get_field(root_field);
        if (!field_result.ok()) return results;
    }

    results.reserve(std::min(limit, size_t(64)));
    eval_step_with_limit_impl(cursor, path.steps(), 0, results, limit);
    return results;
}

// ----------------------------------------------------------------------------
// Public API Implementations (JsonCursor results)
// ----------------------------------------------------------------------------

std::vector<JsonCursor> eval_search_path_cursors(const JsonDocument& doc, const CompiledPath& path) {
    return eval_search_path_cursors(doc.root(), path, std::numeric_limits<size_t>::max());
}

std::vector<JsonCursor> eval_search_path_cursors(const JsonCursor& cursor, const CompiledPath& path) {
    return eval_search_path_cursors(cursor, path, std::numeric_limits<size_t>::max());
}

std::vector<JsonCursor> eval_search_path_cursors(const JsonDocument& doc, const CompiledPath& path, size_t limit) {
    return eval_search_path_cursors(doc.root(), path, limit);
}

std::vector<JsonCursor> eval_search_path_cursors(const JsonCursor& cursor, const CompiledPath& path, size_t limit) {
    std::vector<JsonCursor> results;
    if (path.empty() || limit == 0) return results;

    std::string root_field;
    if (has_root_field_access(path.steps(), root_field)) {
        if (!cursor.is_object()) return results;
        auto field_result = cursor.get_field(root_field);
        if (!field_result.ok()) return results;
    }

    results.reserve(std::min(limit, size_t(64)));
    eval_step_with_limit_impl(cursor, path.steps(), 0, results, limit);
    return results;
}

} // namespace strata

/**
 * jp_eval_pushdown.cpp - Cursor-based JSONPath Evaluator (PR #2 Phase 2)
 *
 * Implements predicate pushdown and minimal materialization using simdjson
 * OnDemand cursors. Evaluates JSONPath expressions without building a full DOM,
 * materializing only matched subtrees.
 *
 * Phase 2: Full DNF filters, slices, direct Python conversion, zero-copy internally
 */

#include "jp_eval_pushdown.hpp"

#include "jp_compile.hpp"
#include "stats.hpp"

#include <chrono>
#include <cmath>
#include <pybind11/pybind11.h>
#include <simdjson.h>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace py = pybind11;

namespace strata {

// ============================================================================
// SECTION: Helper Functions
// ============================================================================

/**
 * Materialize simdjson value to compact JSON string
 */
static std::string materialize_to_json(simdjson::ondemand::value v) {
    try {
        // Use simdjson's native to_json_string for efficient materialization
        std::stringstream ss;
        ss << simdjson::to_json_string(v);
        return ss.str();
    } catch (const simdjson::simdjson_error& e) {
        throw std::runtime_error(std::string("Failed to materialize value: ") + e.what());
    }
}

/**
 * Check if object has a field (by hash + string equality)
 */
static bool has_field(simdjson::ondemand::object& obj, const Symbol& sym) {
    try {
        for (auto field : obj) {
            std::string_view key = field.unescaped_key();
            // Check hash first (fast), then string equality (collision check)
            if (std::hash<std::string_view>{}(key) == sym.hash && key == sym.name) {
                return true;
            }
        }
    } catch (...) {
        // Field access error
    }
    return false;
}

/**
 * Get field value from object (by hash + string equality)
 */
static simdjson::simdjson_result<simdjson::ondemand::value>
get_field(simdjson::ondemand::object& obj, const Symbol& sym) {
    for (auto field : obj) {
        std::string_view key = field.unescaped_key();
        if (std::hash<std::string_view>{}(key) == sym.hash && key == sym.name) {
            return field.value();
        }
    }
    return simdjson::NO_SUCH_FIELD;
}

/**
 * Convert simdjson value to Python object (Phase 2)
 * Zero-copy internally; copies strings only at Python boundary
 */
static py::object value_to_python(simdjson::ondemand::value v) {
    try {
        auto type_result = v.type();
        if (type_result.error()) {
            return py::none();
        }
        auto type = type_result.value();

        switch (type) {
        case simdjson::ondemand::json_type::null:
            return py::none();

        case simdjson::ondemand::json_type::boolean: {
            auto b = v.get_bool();
            if (b.error())
                return py::none();
            return py::bool_(b.value());
        }

        case simdjson::ondemand::json_type::number: {
            // Check if integer or double
            auto num_type_result = v.get_number_type();
            if (num_type_result.error())
                return py::none();
            auto num_type = num_type_result.value();

            if (num_type == simdjson::ondemand::number_type::signed_integer ||
                num_type == simdjson::ondemand::number_type::unsigned_integer) {
                auto ival = v.get_int64();
                if (ival.error())
                    return py::none();
                return py::int_(ival.value());
            } else {
                auto dval = v.get_double();
                if (dval.error())
                    return py::none();
                return py::float_(dval.value());
            }
        }

        case simdjson::ondemand::json_type::string: {
            // Copy string at boundary (zero-copy internally via string_view)
            auto sv_result = v.get_string();
            if (sv_result.error())
                return py::none();
            std::string_view sv = sv_result.value();
            return py::str(sv.data(), sv.size());
        }

        case simdjson::ondemand::json_type::array: {
            py::list result;
            auto arr_result = v.get_array();
            if (arr_result.error())
                return result;
            auto arr = arr_result.value();
            for (auto elem : arr) {
                if (!elem.error()) {
                    result.append(value_to_python(elem.value()));
                }
            }
            return result;
        }

        case simdjson::ondemand::json_type::object: {
            py::dict result;
            auto obj_result = v.get_object();
            if (obj_result.error())
                return result;
            auto obj = obj_result.value();
            for (auto field : obj) {
                if (!field.error()) {
                    auto key_result = field.unescaped_key();
                    if (!key_result.error()) {
                        std::string_view key = key_result.value();
                        py::str py_key(key.data(), key.size());
                        auto val_result = field.value();
                        if (!val_result.error()) {
                            result[py_key] = value_to_python(val_result.value());
                        }
                    }
                }
            }
            return result;
        }

        default:
            return py::none();
        }
    } catch (const simdjson::simdjson_error& e) {
        throw std::runtime_error(std::string("Failed to convert to Python: ") + e.what());
    }
}

/**
 * Resolve relative path from current value (Phase 2 - for filter LHS)
 * Returns SUCCESS if path exists, stores value in out
 */
static bool resolve_rel_path(simdjson::ondemand::value v, const RelPath& path,
                             simdjson::ondemand::value& out) {
    if (path.fields.empty())
        return false;

    simdjson::ondemand::value current = v;
    for (const auto& field_sym : path.fields) {
        auto obj_result = current.get_object();
        if (obj_result.error())
            return false;

        auto obj = obj_result.value();
        auto field_result = get_field(obj, field_sym);
        if (field_result.error() != simdjson::SUCCESS)
            return false;

        current = field_result.value();
    }

    out = current;
    return true;
}

/**
 * Compare two literals (Phase 2)
 */
static int compare_literals(const Literal& a, const Literal& b) {
    // Handle null
    if (std::holds_alternative<std::nullptr_t>(a) && std::holds_alternative<std::nullptr_t>(b))
        return 0;
    if (std::holds_alternative<std::nullptr_t>(a))
        return -1;
    if (std::holds_alternative<std::nullptr_t>(b))
        return 1;

    // Handle bool
    if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b)) {
        bool av = std::get<bool>(a);
        bool bv = std::get<bool>(b);
        return (av == bv) ? 0 : (av ? 1 : -1);
    }

    // Handle numbers (int64_t and double)
    bool a_is_num = std::holds_alternative<int64_t>(a) || std::holds_alternative<double>(a);
    bool b_is_num = std::holds_alternative<int64_t>(b) || std::holds_alternative<double>(b);

    if (a_is_num && b_is_num) {
        double av = std::holds_alternative<int64_t>(a) ? static_cast<double>(std::get<int64_t>(a))
                                                       : std::get<double>(a);
        double bv = std::holds_alternative<int64_t>(b) ? static_cast<double>(std::get<int64_t>(b))
                                                       : std::get<double>(b);
        if (std::abs(av - bv) < 1e-10)
            return 0;
        return (av < bv) ? -1 : 1;
    }

    // Handle strings
    if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) {
        const std::string& av = std::get<std::string>(a);
        const std::string& bv = std::get<std::string>(b);
        return av.compare(bv);
    }

    // Type mismatch - consider unequal
    return -1;
}

/**
 * Convert simdjson value to Literal (Phase 2 - for filter comparison)
 */
static bool value_to_literal(simdjson::ondemand::value v, Literal& out) {
    try {
        auto type_result = v.type();
        if (type_result.error())
            return false;
        auto type = type_result.value();

        switch (type) {
        case simdjson::ondemand::json_type::null:
            out = nullptr;
            return true;

        case simdjson::ondemand::json_type::boolean: {
            auto b = v.get_bool();
            if (b.error())
                return false;
            out = b.value();
            return true;
        }

        case simdjson::ondemand::json_type::number: {
            auto num_type_result = v.get_number_type();
            if (num_type_result.error())
                return false;
            auto num_type = num_type_result.value();

            if (num_type == simdjson::ondemand::number_type::signed_integer ||
                num_type == simdjson::ondemand::number_type::unsigned_integer) {
                auto ival = v.get_int64();
                if (ival.error())
                    return false;
                out = ival.value();
            } else {
                auto dval = v.get_double();
                if (dval.error())
                    return false;
                out = dval.value();
            }
            return true;
        }

        case simdjson::ondemand::json_type::string: {
            auto sv_result = v.get_string();
            if (sv_result.error())
                return false;
            std::string_view sv = sv_result.value();
            out = std::string(sv);
            return true;
        }

        default:
            return false; // Arrays/objects not comparable
        }
    } catch (...) {
        return false;
    }
}

/**
 * Evaluate single filter atom (Phase 2)
 */
static bool eval_filter_atom(simdjson::ondemand::value v, const FilterAtom& atom) {
    // EXISTS check
    if (atom.op == Cmp::EXISTS) {
        simdjson::ondemand::value dummy;
        return resolve_rel_path(v, atom.lhs, dummy);
    }

    // Resolve LHS
    simdjson::ondemand::value lhs_val;
    if (!resolve_rel_path(v, atom.lhs, lhs_val)) {
        return false; // Field doesn't exist
    }

    // Convert LHS to literal
    Literal lhs_lit;
    if (!value_to_literal(lhs_val, lhs_lit)) {
        return false; // Can't compare this type
    }

    // IN operator
    if (atom.op == Cmp::IN) {
        for (const auto& rhs_item : atom.rhs_list) {
            if (compare_literals(lhs_lit, rhs_item) == 0) {
                return true;
            }
        }
        return false;
    }

    // Other operators
    int cmp = compare_literals(lhs_lit, atom.rhs);
    switch (atom.op) {
    case Cmp::EQ:
        return cmp == 0;
    case Cmp::NE:
        return cmp != 0;
    case Cmp::GT:
        return cmp > 0;
    case Cmp::GE:
        return cmp >= 0;
    case Cmp::LT:
        return cmp < 0;
    case Cmp::LE:
        return cmp <= 0;
    default:
        return false;
    }
}

/**
 * Evaluate DNF filter (Phase 2)
 * Returns true if any OR clause is satisfied
 */
static bool eval_filter_dnf(simdjson::ondemand::value v, const FilterDNF& dnf) {
    // OR of ANDs
    for (const auto& and_clause : dnf.clauses) {
        bool clause_satisfied = true;
        for (const auto& atom : and_clause) {
            if (!eval_filter_atom(v, atom)) {
                clause_satisfied = false;
                break;
            }
        }
        if (clause_satisfied) {
            return true; // Short-circuit OR
        }
    }
    return false;
}

// ============================================================================
// SECTION: Cursor Traversal
// ============================================================================

/**
 * Execute pushdown plan step-by-step on a cursor
 *
 * Collects matching values as JSON strings (materialized on match)
 */
static void execute_plan(simdjson::ondemand::value v, const std::vector<Step>& steps,
                         size_t step_idx, std::vector<std::string>& results, Stats* stats) {
    if (step_idx >= steps.size()) {
        // Reached end of plan - this value is a match
        if (stats) {
            stats->add_items_materialized(1);
        }
        results.push_back(materialize_to_json(v));
        return;
    }

    const Step& step = steps[step_idx];

    if (stats) {
        stats->add_objects_visited(1);
    }

    try {
        switch (step.op) {
        case Op::Root: {
            // Root just passes through to next step
            execute_plan(v, steps, step_idx + 1, results, stats);
            break;
        }

        case Op::Field: {
            // Access object field
            auto obj_result = v.get_object();
            if (obj_result.error())
                break; // Not an object, no match

            auto obj = obj_result.value();
            auto field_result = get_field(obj, step.field);
            if (field_result.error() == simdjson::SUCCESS) {
                execute_plan(field_result.value(), steps, step_idx + 1, results, stats);
            }
            // If field doesn't exist, no match (not an error)
            break;
        }

        case Op::Wildcard: {
            // Iterate all elements
            auto type_result = v.type();
            if (!type_result.error()) {
                auto type = type_result.value();
                if (type == simdjson::ondemand::json_type::array) {
                    auto arr = v.get_array();
                    for (auto elem : arr) {
                        execute_plan(elem.value(), steps, step_idx + 1, results, stats);
                    }
                } else if (type == simdjson::ondemand::json_type::object) {
                    auto obj = v.get_object();
                    for (auto field : obj) {
                        execute_plan(field.value(), steps, step_idx + 1, results, stats);
                    }
                }
            }
            break;
        }

        case Op::Index: {
            // Access array element by index
            auto arr = v.get_array();
            int64_t idx = 0;
            for (auto elem : arr) {
                if (idx == step.index) {
                    execute_plan(elem.value(), steps, step_idx + 1, results, stats);
                    break;
                }
                ++idx;
            }
            // If index out of range, no match (not an error)
            break;
        }

        case Op::Slice: {
            // Phase 2: Array slice [start:end:step]
            auto arr = v.get_array();
            int64_t idx = 0;
            for (auto elem : arr) {
                if (idx >= step.slice_start && idx < step.slice_end) {
                    if ((idx - step.slice_start) % step.slice_step == 0) {
                        execute_plan(elem.value(), steps, step_idx + 1, results, stats);
                    }
                }
                ++idx;
                if (idx >= step.slice_end)
                    break; // Early termination
            }
            break;
        }

        case Op::Filter: {
            // Phase 2: DNF filter evaluation
            // Apply filter to current value, continue if matches
            if (eval_filter_dnf(v, step.dnf)) {
                execute_plan(v, steps, step_idx + 1, results, stats);
            }
            break;
        }

        case Op::End:
            // Sentinel, should not be in plan
            break;
        }
    } catch (const simdjson::simdjson_error& e) {
        // Type mismatch or other simdjson error - not a match, continue
        // This is normal for non-matching paths
    }
}

// ============================================================================
// SECTION: Public API
// ============================================================================

/**
 * Execute pushdown plan step-by-step with Python object output (Phase 2)
 */
static void execute_plan_py(simdjson::ondemand::value v, const std::vector<Step>& steps,
                            size_t step_idx, py::list& results, Stats* stats) {
    if (step_idx >= steps.size()) {
        // Reached end of plan - this value is a match
        if (stats) {
            stats->add_items_materialized(1);
        }
        results.append(value_to_python(v));
        return;
    }

    const Step& step = steps[step_idx];

    if (stats) {
        stats->add_objects_visited(1);
    }

    try {
        switch (step.op) {
        case Op::Root: {
            execute_plan_py(v, steps, step_idx + 1, results, stats);
            break;
        }

        case Op::Field: {
            auto obj_result = v.get_object();
            if (obj_result.error())
                break;

            auto obj = obj_result.value();
            auto field_result = get_field(obj, step.field);
            if (field_result.error() == simdjson::SUCCESS) {
                execute_plan_py(field_result.value(), steps, step_idx + 1, results, stats);
            }
            break;
        }

        case Op::Wildcard: {
            auto type_result = v.type();
            if (!type_result.error()) {
                auto type = type_result.value();
                if (type == simdjson::ondemand::json_type::array) {
                    auto arr = v.get_array();
                    for (auto elem : arr) {
                        execute_plan_py(elem.value(), steps, step_idx + 1, results, stats);
                    }
                } else if (type == simdjson::ondemand::json_type::object) {
                    auto obj = v.get_object();
                    for (auto field : obj) {
                        execute_plan_py(field.value(), steps, step_idx + 1, results, stats);
                    }
                }
            }
            break;
        }

        case Op::Index: {
            auto arr = v.get_array();
            int64_t idx = 0;
            for (auto elem : arr) {
                if (idx == step.index) {
                    execute_plan_py(elem.value(), steps, step_idx + 1, results, stats);
                    break;
                }
                ++idx;
            }
            break;
        }

        case Op::Slice: {
            auto arr = v.get_array();
            int64_t idx = 0;
            for (auto elem : arr) {
                if (idx >= step.slice_start && idx < step.slice_end) {
                    if ((idx - step.slice_start) % step.slice_step == 0) {
                        execute_plan_py(elem.value(), steps, step_idx + 1, results, stats);
                    }
                }
                ++idx;
                if (idx >= step.slice_end)
                    break;
            }
            break;
        }

        case Op::Filter: {
            if (eval_filter_dnf(v, step.dnf)) {
                execute_plan_py(v, steps, step_idx + 1, results, stats);
            }
            break;
        }

        case Op::End:
            break;
        }
    } catch (const simdjson::simdjson_error& e) {
        // Type mismatch - not a match, continue
    }
}

/**
 * Evaluate pushdown plan on simdjson cursor
 *
 * Returns array of matches as compact JSON string
 * Format: [match1, match2, ...] or [] if no matches
 *
 * Stats tracking:
 * - objects_visited: incremented for each candidate examined
 * - items_materialized: incremented only when a match is yielded
 * - eval_ms: time spent in pushdown evaluation
 * - bytes_scanned: handled by caller (scanner)
 */
std::string eval_pushdown_to_json(simdjson::ondemand::value v, const CompiledExpr& ce,
                                  Stats* stats) {
    using clock = std::chrono::high_resolution_clock;
    auto eval_start = clock::now();

    // Verify plan supports pushdown
    if (!ce.plan().supports_pushdown) {
        throw std::runtime_error("Expression does not support pushdown");
    }

    // Collect matches
    std::vector<std::string> matches;
    execute_plan(v, ce.plan().steps, 0, matches, stats);

    // Build result array
    std::ostringstream result;
    result << "[";
    for (size_t i = 0; i < matches.size(); ++i) {
        if (i > 0)
            result << ",";
        result << matches[i];
    }
    result << "]";

    // Update stats
    auto eval_end = clock::now();
    double eval_ms = std::chrono::duration<double, std::milli>(eval_end - eval_start).count();
    if (stats) {
        stats->add_eval_ms(eval_ms);
        stats->update_peak_rss();
    }

    return result.str();
}

/**
 * Evaluate pushdown plan with direct Python conversion (Phase 2)
 *
 * Bypasses JSON serialization and json.loads() for better performance.
 * Zero-copy strings internally; copy only at Python boundary.
 */
py::list eval_pushdown_to_py(simdjson::ondemand::value v, const CompiledExpr& ce, Stats* stats) {
    using clock = std::chrono::high_resolution_clock;
    auto eval_start = clock::now();

    // Verify plan supports pushdown
    if (!ce.plan().supports_pushdown) {
        throw std::runtime_error("Expression does not support pushdown");
    }

    // Collect matches as Python objects
    py::list results;
    execute_plan_py(v, ce.plan().steps, 0, results, stats);

    // Update stats
    auto eval_end = clock::now();
    double eval_ms = std::chrono::duration<double, std::milli>(eval_end - eval_start).count();
    if (stats) {
        stats->add_eval_ms(eval_ms);
        stats->update_peak_rss();
    }

    return results;
}

} // namespace strata

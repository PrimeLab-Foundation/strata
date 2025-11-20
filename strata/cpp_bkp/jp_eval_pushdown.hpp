#pragma once

#include "jp_compile.hpp"
#include "stats.hpp"

#include <pybind11/pybind11.h>
#include <simdjson.h>
#include <string>
#include <string_view>

namespace strata {

/**
 * Cursor-based JSONPath evaluator with predicate pushdown (PR #2 Phase 2)
 *
 * Evaluates JSONPath expressions directly on simdjson OnDemand cursors
 * without materializing the full document. This enables:
 * - Early filtering (evaluate predicates before materialization)
 * - Minimal memory usage (only matched subtrees materialized)
 * - Zero-copy strings internally (string_view into simdjson buffer)
 *
 * Phase 2 features:
 * - Path operations: $, .field, [*], [n], [start:end:step]
 * - Full DNF filters: ==, !=, >, >=, <, <=, in, &&, ||
 * - Nested field paths in filters: @.a.b.c
 * - Direct Python conversion (bypasses json.loads)
 *
 * Unsupported features trigger transparent fallback to jsoncons path.
 *
 * Performance characteristics:
 * - Parse time: O(n) where n = input size (simdjson OnDemand)
 * - Eval time: O(m * f) where m = candidates, f = filter complexity
 * - Memory: O(1) per match (streaming, no full DOM)
 *
 * @param v simdjson OnDemand value to evaluate
 * @param ce Compiled expression with pushdown plan
 * @param stats Optional Stats object for metrics tracking
 * @return Compact JSON string with matches (array format)
 * @throws std::runtime_error on evaluation errors
 */
std::string eval_pushdown_to_json(simdjson::ondemand::value v, const CompiledExpr& ce,
                                  Stats* stats);

/**
 * Cursor-based evaluator with direct Python conversion (PR #2 Phase 2)
 *
 * Same as eval_pushdown_to_json but returns Python list directly,
 * bypassing JSON serialization and json.loads() for better performance.
 *
 * Zero-copy strings are converted to Python str objects at the boundary.
 *
 * @param v simdjson OnDemand value to evaluate
 * @param ce Compiled expression with pushdown plan
 * @param stats Optional Stats object for metrics tracking
 * @return Python list of matched values
 * @throws std::runtime_error on evaluation errors
 */
pybind11::list eval_pushdown_to_py(simdjson::ondemand::value v, const CompiledExpr& ce,
                                   Stats* stats);

} // namespace strata

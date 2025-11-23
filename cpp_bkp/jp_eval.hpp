#pragma once

#include "jp_compile.hpp"
#include "stats.hpp"

#include <pybind11/pybind11.h>
#include <string>
#include <string_view>
#include <vector>

namespace py = pybind11;

namespace strata {

/**
 * Normalization policy for JSONPath results
 *
 * Ensures v0.1 backward compatibility by normalizing jsoncons JSONPath output
 * to match expected v0.1 semantics. Applied post-evaluation only.
 *
 * PR #2 TODO: May bypass normalization for cursor-based pushdown views,
 *             but still enforce v0.1 semantics at materialization boundary.
 */
struct NormalizationPolicy {
    // Preserve null values in results (v0.1: true)
    // If false, convert null results to empty array []
    bool keep_nulls = true;

    // Preserve singleton array results without unpacking (v0.1: true)
    // If true: $.x where x=[1,2,3] returns [[1,2,3]]
    // If false: unpacks to [1,2,3]
    bool keep_singleton_arrays = true;

    // Unwrap single-element results (v0.1: false)
    // If true: [42] becomes 42
    // If false: keep as [42]
    bool unwrap_single_element = false;
};

/**
 * Evaluate compiled JSONPath expression on UTF-8 JSON string
 * Returns compact JSON string representation of results
 *
 * This function is useful for:
 * - Debugging and testing
 * - Network protocols that expect JSON strings
 * - Cases where Python-side JSON parsing is acceptable
 *
 * @param json_utf8 UTF-8 encoded JSON input
 * @param ce Compiled JSONPath expression
 * @param stats Optional Stats object for metrics tracking
 * @param policy Normalization policy for v0.1 compatibility (default: v0.1 semantics)
 * @return Compact JSON string with evaluation results
 * @throws std::runtime_error on parse or evaluation errors
 */
std::string eval_jsonpath_materialized(std::string_view json_utf8, const CompiledExpr& ce,
                                       Stats* stats = nullptr,
                                       const NormalizationPolicy& policy = NormalizationPolicy{});

/**
 * Evaluate compiled JSONPath expression on UTF-8 JSON string
 * Returns Python object tree (dict/list/str/int/float/bool/None)
 *
 * This is the primary API for search_bytes() in py_module.cpp.
 * It eliminates json.loads() overhead by constructing Python objects in C++.
 *
 * Performance characteristics:
 * - Parse time: O(n) where n = input size
 * - Eval time: O(m) where m = document complexity
 * - Conversion time: O(k) where k = result size
 *
 * Memory usage:
 * - Peak: ~3-4x input size (jsoncons DOM + Python objects)
 * - PR #2 will reduce this with cursor-based evaluation
 *
 * @param json_utf8 UTF-8 encoded JSON input
 * @param ce Compiled JSONPath expression
 * @param stats Optional Stats object for metrics tracking
 * @param policy Normalization policy for v0.1 compatibility (default: v0.1 semantics)
 * @return Python object representing evaluation results
 * @throws std::runtime_error on parse or evaluation errors
 */
py::object eval_jsonpath_materialized_py(std::string_view json_utf8, const CompiledExpr& ce,
                                         Stats* stats = nullptr,
                                         const NormalizationPolicy& policy = NormalizationPolicy{});

/**
 * Batch evaluation for multiple JSON documents
 *
 * Processes multiple JSON strings with the same JSONPath expression.
 * More efficient than individual calls due to amortized setup costs.
 *
 * Future optimizations (PR #3):
 * - Parallel evaluation across documents
 * - Shared arena allocation
 *
 * @param json_strings Vector of UTF-8 JSON inputs
 * @param ce Compiled JSONPath expression
 * @param stats Optional Stats object for aggregate metrics
 * @return Python list of results (one per input, None on error)
 */
py::list eval_jsonpath_batch_py(const std::vector<std::string_view>& json_strings,
                                const CompiledExpr& ce, Stats* stats = nullptr);

} // namespace strata

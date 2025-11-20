#pragma once
#include <cstddef>
#include <string_view>
#include <vector>

namespace strata {

struct Partition {
    size_t offset = 0;    // byte offset into the original buffer
    size_t length = 0;    // byte length
    size_t row_begin = 0; // NDJSON/TopKey: starting line/element index (inclusive)
    size_t row_end = 0;   // NDJSON/TopKey: ending line/element index (exclusive)
};

struct ElementSpan {
    size_t elem_index = 0; // 0-based element index
    size_t offset = 0;     // byte offset to element start
    size_t length = 0;     // byte length of element
};

struct ExecutionPlan {
    std::vector<Partition> parts;
};

/// Return byte offsets for the start of each non-empty NDJSON line.
/// Always returns at least one entry (0) for inputs with any bytes.
/// Empty inputs return an empty vector.
std::vector<size_t> index_ndjson_rows(std::string_view s);

/// Pack contiguous rows into partitions respecting target_bytes and min_rows.
/// `row_starts` must be offsets as returned by index_ndjson_rows.
/// The final partition always extends to end-of-buffer.
std::vector<Partition> pack_ndjson_partitions(std::string_view s,
                                              const std::vector<size_t>& row_starts,
                                              size_t target_bytes, size_t min_rows);

/// Build an NDJSON execution plan (index+pack) with deterministic boundaries.
ExecutionPlan build_ndjson_plan(std::string_view s, size_t target_bytes, size_t min_rows);

// ============================================================================
// PR #3c: Top-Key Array Partitioner
// ============================================================================

/// Index elements in a top-level array under a given key.
/// Example: {"items": [{"a":1}, {"b":2}]} with top_key="items" returns 2 spans.
/// Throws std::runtime_error if top_key not found or value is not an array.
std::vector<ElementSpan> index_topkey_array(std::string_view json, std::string_view top_key);

/// Pack element spans into partitions respecting target_items and target_bytes.
/// Returns ExecutionPlan with deterministic partition boundaries.
ExecutionPlan pack_topkey_partitions(const std::vector<ElementSpan>& spans, size_t target_items,
                                     size_t target_bytes);

/// Build a top-key array execution plan (index+pack) with deterministic boundaries.
ExecutionPlan build_topkey_plan(std::string_view json, std::string_view top_key,
                                size_t target_items, size_t target_bytes);

} // namespace strata

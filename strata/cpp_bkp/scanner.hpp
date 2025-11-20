#pragma once

#include "jp_compile.hpp"
#include "stats.hpp"

#include <functional>
#include <optional>
#include <string>

namespace strata {

/**
 * Configuration for scanner behavior (PR #3d: Parallel executor enabled)
 */
struct Config {
    // Memory and I/O settings
    size_t buffer_bytes = 1 << 20; // 1 MB default (unused in PR #1, reserved for future)

    // Threading settings (PR #3d: Production parallel executor)
    int threads = 1;                         // Thread count: 0=auto, 1=serial, >1=parallel
    size_t target_partition_bytes = 1 << 24; // Target bytes per partition (~16 MB)
    size_t min_partition_rows = 1 << 10;     // Minimum rows per partition (1024)
    size_t shard_lines = 1 << 10;            // Alias for min_partition_rows (NDJSON)
    size_t shard_items = 1 << 10;            // Minimum items per partition (top-key array)
    bool deterministic_merge = true;         // Preserve input order in output

    // Format detection (PR #3a: Explicit control)
    bool allow_ndjson = true;     // Enable NDJSON detection and processing
    bool allow_json_array = true; // Enable top-level array processing

    // Legacy PR #2 settings
    bool copy_strings = false; // Reserved for PR #2 zero-copy optimization
    bool stable_ids = true;    // Reserved for deterministic iteration (PR #2)
    bool use_views = false;    // Reserved for NodeView proxy (PR #2)

    // PR #4: Memory-mapped I/O settings
    bool use_mmap = true;                  // Enable memory-mapped file I/O (default: true)
    size_t mmap_threshold_bytes = 1 << 20; // Min file size for mmap (1 MB)

    // PR #4: Zero-copy writer settings
    size_t json_writer_chunk = 256 * 1024;    // Initial per-worker writer capacity (256 KB)
    size_t json_writer_cap = 8 * 1024 * 1024; // Hard cap for writer buffer (8 MB)

    // PR #3f: Executor settings
    size_t queue_capacity = 8192;     // Bounded result queue capacity
    size_t worker_batch_flush = 4096; // Emit in small batches (reserved)
    int shutdown_timeout_ms = 5000;   // Coordinator wait limit (reserved)

    /**
     * Validate and normalize configuration values (PR #3d)
     * Logs warnings for invalid settings via stderr
     */
    void validate();
};

/**
 * Yield callback function type
 *
 * For PR #1, yields compact JSON strings (not Python objects).
 * py_module.cpp will parse these strings to Python objects using jp_eval APIs.
 *
 * This keeps scanner.cpp free of Python dependencies and GIL-safe.
 */
using YieldFn = std::function<void(const std::string&)>;

/**
 * Stream over a JSON file with JSONPath evaluation
 *
 * Supports three modes (auto-detected):
 * 1. Top-key array mode: If top_key is provided, navigates to root[top_key]
 *    and iterates array elements without materializing the full array
 * 2. NDJSON mode: Detects newline-delimited JSON and processes line-by-line
 * 3. Single document mode: Fallback for regular JSON files
 *
 * For each candidate element/document:
 * - Materializes only that element to compact JSON string
 * - Evaluates JSONPath expression
 * - Yields non-empty results via callback
 *
 * Memory characteristics:
 * - O(1) for streaming modes (NDJSON, top-key array)
 * - O(n) for single document (but only candidate, not full file)
 *
 * Threading:
 * - Single-threaded in PR #1
 * - Must be called without GIL held (py_module.cpp manages GIL)
 *
 * Error handling:
 * - Throws std::runtime_error for I/O failures, parse errors
 * - Returns gracefully for empty results
 *
 * @param path File path to JSON/NDJSON file
 * @param ce Compiled JSONPath expression
 * @param top_key Optional key to navigate to top-level array
 * @param cfg Configuration (mostly unused in PR #1)
 * @param stats Optional Stats object for metrics
 * @param yield Callback for each match (receives compact JSON string)
 * @throws std::runtime_error on I/O or parse errors
 */
void search_file(const std::string& path, const CompiledExpr& ce,
                 const std::optional<std::string>& top_key, const Config& cfg, Stats* stats,
                 const YieldFn& yield);

/**
 * Evaluate JSONPath on in-memory UTF-8 JSON bytes
 *
 * Processes a single JSON document from memory.
 * Simpler than search_file since no file I/O or format detection needed.
 *
 * Flow:
 * 1. Parse JSON using simdjson
 * 2. Materialize to string
 * 3. Evaluate JSONPath (via jp_eval)
 * 4. Yield non-empty results
 *
 * @param data Pointer to UTF-8 JSON bytes
 * @param n Size of data in bytes
 * @param ce Compiled JSONPath expression
 * @param cfg Configuration (unused in PR #1)
 * @param stats Optional Stats object for metrics
 * @param yield Callback for each match (receives compact JSON string)
 * @throws std::runtime_error on parse errors
 */
void search_bytes(const char* data, size_t n, const CompiledExpr& ce, const Config& cfg,
                  Stats* stats, const YieldFn& yield);

} // namespace strata

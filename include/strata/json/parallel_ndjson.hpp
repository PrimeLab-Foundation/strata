#pragma once

#include "strata/json/json_core.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace strata {

class CompiledPath;

/**
 * Profiling output for parallel NDJSON parsing.
 *
 * All durations are in nanoseconds. overhead_ns is an approximation:
 * submit + merge + max(0, wait - max(parse_chunk)).
 */
struct ParallelNdjsonProfile {
    size_t data_size = 0;
    size_t line_count = 0;
    size_t chunk_count = 0;
    uint64_t line_scan_ns = 0;
    uint64_t partition_ns = 0;
    uint64_t submit_ns = 0;
    uint64_t wait_ns = 0;
    uint64_t merge_ns = 0;
    uint64_t parse_ns_total = 0;
    uint64_t parse_ns_max = 0;
    uint64_t overhead_ns = 0;
    uint64_t overhead_per_chunk_ns = 0;
};

/**
 * Configuration for parallel NDJSON processing.
 */
struct ParallelNdjsonConfig {
    /// Number of worker threads (0 = auto-detect based on hardware_concurrency)
    size_t num_threads = 0;

    /// Minimum bytes per thread chunk to avoid overhead for small data
    size_t min_chunk_size = 1024 * 1024;  // 1MB

    /// Minimum lines to enable parallel mode (below this, use sequential)
    size_t min_lines_for_parallel = 1000;

    /// Skip malformed lines instead of stopping on error
    bool skip_errors = false;

    /// Optional profiling output (nullptr = disabled)
    ParallelNdjsonProfile* profile = nullptr;
};

/**
 * Result of parallel NDJSON parsing with error collection.
 */
struct ParallelParseResult {
    /// Successfully parsed values in order
    std::vector<JsonValue> values;

    /// Errors encountered: (line_number, error_message)
    std::vector<std::pair<size_t, std::string>> errors;

    /// Total lines processed (including errors)
    size_t lines_processed = 0;
};

/**
 * Single NDJSON search match with line number.
 */
struct NdjsonSearchMatch {
    size_t line = 0;
    std::vector<JsonValue> matches;
};

/**
 * Result of parallel NDJSON search with error collection.
 */
struct ParallelSearchResult {
    std::vector<NdjsonSearchMatch> matches;
    std::vector<std::pair<size_t, std::string>> errors;
    size_t lines_processed = 0;
};

/**
 * Parallel NDJSON (Newline Delimited JSON) streaming parser.
 *
 * Achieves 2-4x speedup for large files by parsing independent lines
 * concurrently using multiple threads with SIMD-accelerated line boundary
 * detection.
 *
 * Order Preservation Strategy:
 * 1. SIMD scan collects all newline positions in O(n) time
 * 2. Data partitioned into chunks with sequence numbers
 * 3. Threads parse chunks in parallel
 * 4. Results merged in sequence order for deterministic output
 *
 * Features:
 * - Automatic fallback to sequential for small files
 * - Per-chunk error collection with line numbers
 * - Thread-local allocations to avoid contention
 * - Configurable thread count and chunk sizes
 */
class ParallelNdjsonStream {
  public:
    /**
     * Create parallel stream from string data.
     *
     * @param data NDJSON string data
     * @param config Configuration options
     */
    explicit ParallelNdjsonStream(std::string_view data, ParallelNdjsonConfig config = {});

    /**
     * Parse all lines in parallel, maintaining input order.
     *
     * Automatically falls back to sequential parsing for small files.
     * Throws on error if skip_errors is false.
     *
     * @return Vector of parsed JsonValues in input order
     */
    std::vector<JsonValue> parse_all_parallel();

    /**
     * Parse all lines with error collection (no exceptions).
     *
     * @return ParseResult with values, errors, and statistics
     */
    ParallelParseResult parse_all_parallel_with_errors();

    /**
     * Search all lines in parallel, maintaining input order.
     *
     * Throws on error if skip_errors is false.
     */
    std::vector<NdjsonSearchMatch> search_all_parallel(const CompiledPath& path);

    /**
     * Search all lines with error collection (no exceptions).
     */
    ParallelSearchResult search_all_parallel_with_errors(const CompiledPath& path);

    /**
     * Get total lines processed (after parsing).
     */
    size_t lines_processed() const noexcept { return lines_processed_; }

    /**
     * Get error count (after parsing).
     */
    size_t error_count() const noexcept { return error_count_; }

    /**
     * Check if parallel mode was used (vs sequential fallback).
     */
    bool used_parallel_mode() const noexcept { return used_parallel_mode_; }

  private:
    std::string_view data_;
    ParallelNdjsonConfig config_;

    // Statistics
    size_t lines_processed_ = 0;
    size_t error_count_ = 0;
    bool used_parallel_mode_ = false;
    bool utf8_checked_ = false;
    bool utf8_ok_ = true;

    // Internal chunk representation
    struct Chunk {
        size_t sequence;      // For ordering results
        size_t start_line;    // First line number (1-indexed)
        size_t end_line;      // Last line number (exclusive)
        std::string_view data;  // Chunk data
    };

    // Per-chunk parsing result
    struct ChunkResult {
        size_t sequence;      // For ordering
        std::vector<JsonValue> values;
        std::vector<std::pair<size_t, std::string>> errors;  // (line_num, message)
    };

    struct ChunkSearchResult {
        size_t sequence;      // For ordering
        std::vector<NdjsonSearchMatch> matches;
        std::vector<std::pair<size_t, std::string>> errors;  // (line_num, message)
        size_t lines_processed = 0;
    };

    // Phase 1: SIMD line boundary collection
    std::vector<size_t> collect_line_boundaries();

    // Phase 2: Partition data into chunks
    std::vector<Chunk> partition_chunks(const std::vector<size_t>& boundaries);

    // Phase 3: Parse single chunk (called by worker threads)
    ChunkResult parse_chunk(const Chunk& chunk, bool skip_utf8_validation,
                            std::vector<uint64_t>* chunk_parse_ns);

    // Phase 3: Search single chunk (called by worker threads)
    ChunkSearchResult search_chunk(const Chunk& chunk, const CompiledPath& path,
                                   bool skip_utf8_validation);

    // Phase 4: Merge results in sequence order
    void merge_results(std::vector<ChunkResult>& results, std::vector<JsonValue>& out_values,
                       std::vector<std::pair<size_t, std::string>>& out_errors);

    void merge_search_results(std::vector<ChunkSearchResult>& results,
                              std::vector<NdjsonSearchMatch>& out_matches,
                              std::vector<std::pair<size_t, std::string>>& out_errors,
                              size_t* out_lines_processed);

    // Sequential fallback for small files
    std::vector<JsonValue> parse_sequential();
    ParallelParseResult parse_sequential_with_errors();

    ParallelSearchResult search_sequential_with_errors(const CompiledPath& path);

    bool validate_utf8_once();
};

}  // namespace strata

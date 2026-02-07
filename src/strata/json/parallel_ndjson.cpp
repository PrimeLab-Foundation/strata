#include "strata/json/parallel_ndjson.hpp"

#include "strata/json/json_parse.hpp"
#include "strata/util/simd_string.hpp"
#include "strata/util/thread_pool.hpp"

#include <algorithm>
#include <future>
#include <stdexcept>
#include <thread>

namespace strata {

namespace {

// Check if a line is empty or whitespace-only
static inline bool is_json_whitespace(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

static inline bool is_whitespace_only_line(std::string_view line) {
    if (line.empty()) {
        return true;
    }

    unsigned char first = static_cast<unsigned char>(line.front());
    if (!is_json_whitespace(first)) {
        return false;
    }

    return util::is_whitespace_only_simd(line.data(), line.size());
}

// Extract line from data, handling \r\n
static inline std::string_view extract_line(const char* start, size_t len) {
    if (len > 0 && start[len - 1] == '\r') {
        --len;
    }
    return std::string_view(start, len);
}

}  // namespace

ParallelNdjsonStream::ParallelNdjsonStream(std::string_view data, ParallelNdjsonConfig config)
    : data_(data), config_(config) {
    // Validate config
    if (config_.num_threads == 0) {
        config_.num_threads = std::thread::hardware_concurrency();
        if (config_.num_threads == 0) {
            config_.num_threads = 4;  // Fallback
        }
    }
}

std::vector<size_t> ParallelNdjsonStream::collect_line_boundaries() {
    std::vector<size_t> boundaries;

    if (data_.empty()) {
        return boundaries;
    }

    // Estimate capacity based on typical line length (~100 bytes)
    size_t estimated_lines = data_.size() / 100 + 1;
    boundaries.reserve(estimated_lines);

    // Start of first line
    boundaries.push_back(0);

    // Collect newline positions using SIMD
    size_t pos = 0;
    while (pos < data_.size()) {
        size_t newline_pos = util::find_newline_simd(data_.data(), data_.size(), pos);
        if (newline_pos >= data_.size()) {
            break;
        }
        // Position after newline is start of next line
        boundaries.push_back(newline_pos + 1);
        pos = newline_pos + 1;
    }

    return boundaries;
}

std::vector<ParallelNdjsonStream::Chunk> ParallelNdjsonStream::partition_chunks(
    const std::vector<size_t>& boundaries) {
    std::vector<Chunk> chunks;

    if (boundaries.empty()) {
        return chunks;
    }

    size_t num_lines = boundaries.size();
    size_t num_threads = config_.num_threads;

    // Calculate target lines per chunk
    size_t lines_per_chunk = (num_lines + num_threads - 1) / num_threads;
    lines_per_chunk = std::max(lines_per_chunk, static_cast<size_t>(1));

    // Also ensure minimum chunk size
    if (data_.size() >= config_.min_chunk_size * num_threads) {
        // Have enough data - use calculated lines per chunk
    } else {
        // Not enough data - use fewer chunks
        size_t target_chunks = data_.size() / config_.min_chunk_size;
        if (target_chunks == 0) {
            target_chunks = 1;
        }
        lines_per_chunk = (num_lines + target_chunks - 1) / target_chunks;
    }

    // Create chunks
    size_t chunk_start_idx = 0;
    size_t sequence = 0;

    while (chunk_start_idx < num_lines) {
        size_t chunk_end_idx = std::min(chunk_start_idx + lines_per_chunk, num_lines);

        // Calculate data range for this chunk
        size_t data_start = boundaries[chunk_start_idx];
        size_t data_end;

        if (chunk_end_idx >= num_lines) {
            // Last chunk extends to end of data
            data_end = data_.size();
        } else {
            // End at start of next chunk's first line
            data_end = boundaries[chunk_end_idx];
        }

        Chunk chunk;
        chunk.sequence = sequence++;
        chunk.start_line = chunk_start_idx + 1;  // 1-indexed
        chunk.end_line = chunk_end_idx + 1;      // 1-indexed, exclusive
        chunk.data = std::string_view(data_.data() + data_start, data_end - data_start);

        chunks.push_back(chunk);
        chunk_start_idx = chunk_end_idx;
    }

    return chunks;
}

ParallelNdjsonStream::ChunkResult ParallelNdjsonStream::parse_chunk(const Chunk& chunk) {
    ChunkResult result;
    result.sequence = chunk.sequence;

    if (chunk.data.empty()) {
        return result;
    }

    // Pre-allocate based on estimated line count
    size_t estimated_lines = chunk.end_line - chunk.start_line;
    result.values.reserve(estimated_lines);

    size_t pos = 0;
    size_t line_num = chunk.start_line;

    while (pos < chunk.data.size()) {
        // Find next newline
        size_t newline_pos = util::find_newline_simd(chunk.data.data(), chunk.data.size(), pos);

        // Extract line
        size_t line_len = (newline_pos < chunk.data.size()) ? (newline_pos - pos)
                                                            : (chunk.data.size() - pos);

        std::string_view line = extract_line(chunk.data.data() + pos, line_len);

        // Skip whitespace-only lines
        if (!is_whitespace_only_line(line)) {
            auto parse_result = parse_json(line);

            if (parse_result.ok()) {
                result.values.push_back(std::move(parse_result.value));
            } else {
                // Collect error with line number
                result.errors.emplace_back(line_num, "Invalid JSON");
            }
        }

        // Move to next line
        pos = (newline_pos < chunk.data.size()) ? (newline_pos + 1) : chunk.data.size();
        line_num++;
    }

    return result;
}

void ParallelNdjsonStream::merge_results(std::vector<ChunkResult>& results,
                                          std::vector<JsonValue>& out_values,
                                          std::vector<std::pair<size_t, std::string>>& out_errors) {
    // Sort by sequence number to maintain order
    std::sort(results.begin(), results.end(),
              [](const ChunkResult& a, const ChunkResult& b) { return a.sequence < b.sequence; });

    // Calculate total size for pre-allocation
    size_t total_values = 0;
    size_t total_errors = 0;
    for (const auto& r : results) {
        total_values += r.values.size();
        total_errors += r.errors.size();
    }

    out_values.reserve(total_values);
    out_errors.reserve(total_errors);

    // Merge in sequence order
    for (auto& r : results) {
        for (auto& v : r.values) {
            out_values.push_back(std::move(v));
        }
        for (auto& e : r.errors) {
            out_errors.push_back(std::move(e));
        }
    }
}

std::vector<JsonValue> ParallelNdjsonStream::parse_sequential() {
    std::vector<JsonValue> results;

    // Count lines for pre-allocation
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;
    results.reserve(line_count);

    size_t pos = 0;
    size_t line_num = 1;

    while (pos < data_.size()) {
        size_t newline_pos = util::find_newline_simd(data_.data(), data_.size(), pos);

        size_t line_len = (newline_pos < data_.size()) ? (newline_pos - pos)
                                                       : (data_.size() - pos);

        std::string_view line = extract_line(data_.data() + pos, line_len);

        if (!is_whitespace_only_line(line)) {
            lines_processed_++;
            auto parse_result = parse_json(line);

            if (parse_result.ok()) {
                results.push_back(std::move(parse_result.value));
            } else {
                error_count_++;
                if (!config_.skip_errors) {
                    throw std::runtime_error("Parse error at line " + std::to_string(line_num));
                }
            }
        }

        pos = (newline_pos < data_.size()) ? (newline_pos + 1) : data_.size();
        line_num++;
    }

    return results;
}

ParallelParseResult ParallelNdjsonStream::parse_sequential_with_errors() {
    ParallelParseResult result;

    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;
    result.values.reserve(line_count);

    size_t pos = 0;
    size_t line_num = 1;

    while (pos < data_.size()) {
        size_t newline_pos = util::find_newline_simd(data_.data(), data_.size(), pos);

        size_t line_len = (newline_pos < data_.size()) ? (newline_pos - pos)
                                                       : (data_.size() - pos);

        std::string_view line = extract_line(data_.data() + pos, line_len);

        if (!is_whitespace_only_line(line)) {
            result.lines_processed++;
            auto parse_result = parse_json(line);

            if (parse_result.ok()) {
                result.values.push_back(std::move(parse_result.value));
            } else {
                result.errors.emplace_back(line_num, "Invalid JSON");
            }
        }

        pos = (newline_pos < data_.size()) ? (newline_pos + 1) : data_.size();
        line_num++;
    }

    lines_processed_ = result.lines_processed;
    error_count_ = result.errors.size();

    return result;
}

std::vector<JsonValue> ParallelNdjsonStream::parse_all_parallel() {
    if (data_.empty()) {
        return {};
    }

    // Count lines to decide parallelization strategy
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;

    // Fall back to sequential for small inputs
    if (line_count < config_.min_lines_for_parallel ||
        data_.size() < config_.min_chunk_size * 2) {
        used_parallel_mode_ = false;
        return parse_sequential();
    }

    used_parallel_mode_ = true;

    // Phase 1: Collect line boundaries
    std::vector<size_t> boundaries = collect_line_boundaries();

    // Phase 2: Partition into chunks
    std::vector<Chunk> chunks = partition_chunks(boundaries);

    if (chunks.size() <= 1) {
        // Single chunk - sequential is more efficient
        used_parallel_mode_ = false;
        return parse_sequential();
    }

    // Phase 3: Parse chunks in parallel
    util::ThreadPool pool(config_.num_threads);

    std::vector<std::future<ChunkResult>> futures;
    futures.reserve(chunks.size());

    for (const auto& chunk : chunks) {
        futures.push_back(pool.submit([this, &chunk]() { return parse_chunk(chunk); }));
    }

    // Collect results
    std::vector<ChunkResult> chunk_results;
    chunk_results.reserve(futures.size());

    for (auto& f : futures) {
        chunk_results.push_back(f.get());
    }

    // Phase 4: Merge results in order
    std::vector<JsonValue> values;
    std::vector<std::pair<size_t, std::string>> errors;
    merge_results(chunk_results, values, errors);

    // Update statistics
    lines_processed_ = values.size() + errors.size();
    error_count_ = errors.size();

    // Handle errors based on config
    if (!errors.empty() && !config_.skip_errors) {
        throw std::runtime_error("Parse error at line " + std::to_string(errors[0].first) + ": " +
                                 errors[0].second);
    }

    return values;
}

ParallelParseResult ParallelNdjsonStream::parse_all_parallel_with_errors() {
    ParallelParseResult result;

    if (data_.empty()) {
        return result;
    }

    // Count lines to decide parallelization strategy
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;

    // Fall back to sequential for small inputs
    if (line_count < config_.min_lines_for_parallel ||
        data_.size() < config_.min_chunk_size * 2) {
        used_parallel_mode_ = false;
        return parse_sequential_with_errors();
    }

    used_parallel_mode_ = true;

    // Phase 1: Collect line boundaries
    std::vector<size_t> boundaries = collect_line_boundaries();

    // Phase 2: Partition into chunks
    std::vector<Chunk> chunks = partition_chunks(boundaries);

    if (chunks.size() <= 1) {
        used_parallel_mode_ = false;
        return parse_sequential_with_errors();
    }

    // Phase 3: Parse chunks in parallel
    util::ThreadPool pool(config_.num_threads);

    std::vector<std::future<ChunkResult>> futures;
    futures.reserve(chunks.size());

    for (const auto& chunk : chunks) {
        futures.push_back(pool.submit([this, &chunk]() { return parse_chunk(chunk); }));
    }

    // Collect results
    std::vector<ChunkResult> chunk_results;
    chunk_results.reserve(futures.size());

    for (auto& f : futures) {
        chunk_results.push_back(f.get());
    }

    // Phase 4: Merge results in order
    merge_results(chunk_results, result.values, result.errors);

    // Update statistics
    result.lines_processed = result.values.size() + result.errors.size();
    lines_processed_ = result.lines_processed;
    error_count_ = result.errors.size();

    return result;
}

}  // namespace strata

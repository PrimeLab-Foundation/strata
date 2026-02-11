#include "strata/json/parallel_ndjson.hpp"

#include "strata/json/json_parse.hpp"
#include "strata/search/search.hpp"
#include "strata/util/simd_string.hpp"
#include "strata/util/thread_pool.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <future>
#include <stdexcept>
#include <thread>

namespace strata {

namespace {

using Clock = std::chrono::steady_clock;

static inline uint64_t duration_ns(Clock::time_point start, Clock::time_point end) {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
}

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
        size_t hw_threads = std::thread::hardware_concurrency();
        if (hw_threads == 0) {
            hw_threads = 4;  // Fallback
        }
        size_t target_threads = data_.size() / config_.min_chunk_size;
        if (target_threads == 0) {
            target_threads = 1;
        }
        config_.num_threads = std::min(hw_threads, target_threads);
    }
}

bool ParallelNdjsonStream::validate_utf8_once() {
    if (!utf8_checked_) {
        utf8_ok_ = util::validate_utf8_lazy(data_.data(), data_.size());
        utf8_checked_ = true;
    }
    return utf8_ok_;
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

ParallelNdjsonStream::ChunkResult ParallelNdjsonStream::parse_chunk(
    const Chunk& chunk,
    bool skip_utf8_validation,
    std::vector<uint64_t>* chunk_parse_ns) {
    ChunkResult result;
    result.sequence = chunk.sequence;

    if (chunk.data.empty()) {
        if (chunk_parse_ns) {
            (*chunk_parse_ns)[chunk.sequence] = 0;
        }
        return result;
    }

    Clock::time_point start;
    if (chunk_parse_ns) {
        start = Clock::now();
    }

    // Pre-allocate based on estimated line count
    size_t estimated_lines = chunk.end_line - chunk.start_line;
    result.values.reserve(estimated_lines);

    size_t pos = 0;
    size_t line_num = chunk.start_line;
    thread_local ParseSaxContext parse_context;
    ParseSaxOptions options;
    options.validate_utf8 = !skip_utf8_validation;

    while (pos < chunk.data.size()) {
        // Find next newline
        size_t newline_pos = util::find_newline_simd(chunk.data.data(), chunk.data.size(), pos);

        // Extract line
        size_t line_len = (newline_pos < chunk.data.size()) ? (newline_pos - pos)
                                                            : (chunk.data.size() - pos);

        std::string_view line = extract_line(chunk.data.data() + pos, line_len);

        // Skip whitespace-only lines
        if (!is_whitespace_only_line(line)) {
            auto parse_result = parse_json(line, options, &parse_context);

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

    if (chunk_parse_ns) {
        (*chunk_parse_ns)[chunk.sequence] = duration_ns(start, Clock::now());
    }

    return result;
}

ParallelNdjsonStream::ChunkSearchResult ParallelNdjsonStream::search_chunk(
    const Chunk& chunk,
    const CompiledPath& path,
    bool skip_utf8_validation) {
    ChunkSearchResult result;
    result.sequence = chunk.sequence;

    if (chunk.data.empty()) {
        return result;
    }

    size_t estimated_lines = chunk.end_line - chunk.start_line;
    result.matches.reserve(estimated_lines);

    size_t pos = 0;
    size_t line_num = chunk.start_line;
    thread_local ParseSaxContext parse_context;
    ParseSaxOptions options;
    options.validate_utf8 = !skip_utf8_validation;

    while (pos < chunk.data.size()) {
        size_t newline_pos = util::find_newline_simd(chunk.data.data(), chunk.data.size(), pos);

        size_t line_len = (newline_pos < chunk.data.size()) ? (newline_pos - pos)
                                                            : (chunk.data.size() - pos);

        std::string_view line = extract_line(chunk.data.data() + pos, line_len);

        if (!is_whitespace_only_line(line)) {
            result.lines_processed++;
            auto parse_result = parse_json(line, options, &parse_context);

            if (parse_result.ok()) {
                JsonCursor cursor(&parse_result.value);
                auto matches = eval_search_path(cursor, path);
                if (!matches.empty()) {
                    NdjsonSearchMatch entry;
                    entry.line = line_num;
                    entry.matches = std::move(matches);
                    result.matches.push_back(std::move(entry));
                }
            } else {
                result.errors.emplace_back(line_num, "Invalid JSON");
            }
        }

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

void ParallelNdjsonStream::merge_search_results(std::vector<ChunkSearchResult>& results,
                                                std::vector<NdjsonSearchMatch>& out_matches,
                                                std::vector<std::pair<size_t, std::string>>& out_errors,
                                                size_t* out_lines_processed) {
    std::sort(results.begin(), results.end(),
              [](const ChunkSearchResult& a, const ChunkSearchResult& b) {
                  return a.sequence < b.sequence;
              });

    size_t total_matches = 0;
    size_t total_errors = 0;
    size_t total_lines = 0;
    for (const auto& r : results) {
        total_matches += r.matches.size();
        total_errors += r.errors.size();
        total_lines += r.lines_processed;
    }

    out_matches.reserve(total_matches);
    out_errors.reserve(total_errors);
    if (out_lines_processed) {
        *out_lines_processed = total_lines;
    }

    for (auto& r : results) {
        for (auto& match : r.matches) {
            out_matches.push_back(std::move(match));
        }
        for (auto& err : r.errors) {
            out_errors.push_back(std::move(err));
        }
    }
}

std::vector<JsonValue> ParallelNdjsonStream::parse_sequential() {
    std::vector<JsonValue> results;

    // Count lines for pre-allocation
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;
    results.reserve(line_count);

    validate_utf8_once();
    ParseSaxContext parse_context;
    ParseSaxOptions options;
    options.validate_utf8 = !(utf8_checked_ && utf8_ok_);

    size_t pos = 0;
    size_t line_num = 1;

    while (pos < data_.size()) {
        size_t newline_pos = util::find_newline_simd(data_.data(), data_.size(), pos);

        size_t line_len = (newline_pos < data_.size()) ? (newline_pos - pos)
                                                       : (data_.size() - pos);

        std::string_view line = extract_line(data_.data() + pos, line_len);

        if (!is_whitespace_only_line(line)) {
            lines_processed_++;
            auto parse_result = parse_json(line, options, &parse_context);

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

ParallelSearchResult ParallelNdjsonStream::search_sequential_with_errors(
    const CompiledPath& path) {
    ParallelSearchResult result;

    validate_utf8_once();
    ParseSaxContext parse_context;
    ParseSaxOptions options;
    options.validate_utf8 = !(utf8_checked_ && utf8_ok_);

    size_t pos = 0;
    size_t line_num = 1;

    while (pos < data_.size()) {
        size_t newline_pos = util::find_newline_simd(data_.data(), data_.size(), pos);

        size_t line_len = (newline_pos < data_.size()) ? (newline_pos - pos)
                                                       : (data_.size() - pos);

        std::string_view line = extract_line(data_.data() + pos, line_len);

        if (!is_whitespace_only_line(line)) {
            result.lines_processed++;
            auto parse_result = parse_json(line, options, &parse_context);

            if (parse_result.ok()) {
                JsonCursor cursor(&parse_result.value);
                auto matches = eval_search_path(cursor, path);
                if (!matches.empty()) {
                    NdjsonSearchMatch entry;
                    entry.line = line_num;
                    entry.matches = std::move(matches);
                    result.matches.push_back(std::move(entry));
                }
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

ParallelParseResult ParallelNdjsonStream::parse_sequential_with_errors() {
    ParallelParseResult result;

    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;
    result.values.reserve(line_count);

    validate_utf8_once();
    ParseSaxContext parse_context;
    ParseSaxOptions options;
    options.validate_utf8 = !(utf8_checked_ && utf8_ok_);

    size_t pos = 0;
    size_t line_num = 1;

    while (pos < data_.size()) {
        size_t newline_pos = util::find_newline_simd(data_.data(), data_.size(), pos);

        size_t line_len = (newline_pos < data_.size()) ? (newline_pos - pos)
                                                       : (data_.size() - pos);

        std::string_view line = extract_line(data_.data() + pos, line_len);

        if (!is_whitespace_only_line(line)) {
            result.lines_processed++;
            auto parse_result = parse_json(line, options, &parse_context);

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

    validate_utf8_once();
    const bool skip_utf8_validation = utf8_checked_ && utf8_ok_;

    // Count lines to decide parallelization strategy
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;
    ParallelNdjsonProfile* profile = config_.profile;
    std::vector<uint64_t> chunk_parse_ns;
    if (profile) {
        *profile = ParallelNdjsonProfile{};
        profile->data_size = data_.size();
        profile->line_count = line_count;
    }

    // Fall back to sequential for small inputs
    if (line_count < config_.min_lines_for_parallel ||
        data_.size() < config_.min_chunk_size * 2) {
        used_parallel_mode_ = false;
        return parse_sequential();
    }

    used_parallel_mode_ = true;

    // Phase 1: Collect line boundaries
    Clock::time_point line_scan_start;
    if (profile) {
        line_scan_start = Clock::now();
    }
    std::vector<size_t> boundaries = collect_line_boundaries();
    if (profile) {
        profile->line_scan_ns = duration_ns(line_scan_start, Clock::now());
    }

    // Phase 2: Partition into chunks
    Clock::time_point partition_start;
    if (profile) {
        partition_start = Clock::now();
    }
    std::vector<Chunk> chunks = partition_chunks(boundaries);
    if (profile) {
        profile->partition_ns = duration_ns(partition_start, Clock::now());
        profile->chunk_count = chunks.size();
        chunk_parse_ns.assign(chunks.size(), 0);
    }

    if (chunks.size() <= 1) {
        // Single chunk - sequential is more efficient
        used_parallel_mode_ = false;
        return parse_sequential();
    }

    // Phase 3: Parse chunks in parallel
    util::ThreadPool pool(config_.num_threads);

    std::vector<std::function<ChunkResult()>> tasks;
    tasks.reserve(chunks.size());

    std::vector<std::future<ChunkResult>> futures;
    futures.reserve(chunks.size());

    std::vector<uint64_t>* parse_ns_out = profile ? &chunk_parse_ns : nullptr;
    for (const auto& chunk : chunks) {
        tasks.emplace_back([this, chunk, skip_utf8_validation, parse_ns_out]() {
            return parse_chunk(chunk, skip_utf8_validation, parse_ns_out);
        });
    }

    Clock::time_point submit_start;
    if (profile) {
        submit_start = Clock::now();
    }
    pool.submit_bulk(tasks, futures);
    if (profile) {
        profile->submit_ns = duration_ns(submit_start, Clock::now());
    }

    // Collect results
    std::vector<ChunkResult> chunk_results;
    chunk_results.reserve(futures.size());

    Clock::time_point wait_start;
    if (profile) {
        wait_start = Clock::now();
    }
    for (auto& f : futures) {
        chunk_results.push_back(f.get());
    }
    if (profile) {
        profile->wait_ns = duration_ns(wait_start, Clock::now());
    }

    // Phase 4: Merge results in order
    std::vector<JsonValue> values;
    std::vector<std::pair<size_t, std::string>> errors;
    Clock::time_point merge_start;
    if (profile) {
        merge_start = Clock::now();
    }
    merge_results(chunk_results, values, errors);
    if (profile) {
        profile->merge_ns = duration_ns(merge_start, Clock::now());
    }

    // Update statistics
    lines_processed_ = values.size() + errors.size();
    error_count_ = errors.size();

    // Handle errors based on config
    if (!errors.empty() && !config_.skip_errors) {
        throw std::runtime_error("Parse error at line " + std::to_string(errors[0].first) + ": " +
                                 errors[0].second);
    }

    if (profile) {
        uint64_t parse_total = 0;
        uint64_t parse_max = 0;
        for (uint64_t ns : chunk_parse_ns) {
            parse_total += ns;
            if (ns > parse_max) {
                parse_max = ns;
            }
        }
        profile->parse_ns_total = parse_total;
        profile->parse_ns_max = parse_max;
        uint64_t overhead_ns = profile->submit_ns + profile->merge_ns;
        if (profile->wait_ns > parse_max) {
            overhead_ns += (profile->wait_ns - parse_max);
        }
        profile->overhead_ns = overhead_ns;
        profile->overhead_per_chunk_ns =
            profile->chunk_count ? (overhead_ns / profile->chunk_count) : 0;
    }

    return values;
}

ParallelParseResult ParallelNdjsonStream::parse_all_parallel_with_errors() {
    ParallelParseResult result;

    if (data_.empty()) {
        return result;
    }

    validate_utf8_once();
    const bool skip_utf8_validation = utf8_checked_ && utf8_ok_;

    // Count lines to decide parallelization strategy
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;
    ParallelNdjsonProfile* profile = config_.profile;
    std::vector<uint64_t> chunk_parse_ns;
    if (profile) {
        *profile = ParallelNdjsonProfile{};
        profile->data_size = data_.size();
        profile->line_count = line_count;
    }

    // Fall back to sequential for small inputs
    if (line_count < config_.min_lines_for_parallel ||
        data_.size() < config_.min_chunk_size * 2) {
        used_parallel_mode_ = false;
        return parse_sequential_with_errors();
    }

    used_parallel_mode_ = true;

    // Phase 1: Collect line boundaries
    Clock::time_point line_scan_start;
    if (profile) {
        line_scan_start = Clock::now();
    }
    std::vector<size_t> boundaries = collect_line_boundaries();
    if (profile) {
        profile->line_scan_ns = duration_ns(line_scan_start, Clock::now());
    }

    // Phase 2: Partition into chunks
    Clock::time_point partition_start;
    if (profile) {
        partition_start = Clock::now();
    }
    std::vector<Chunk> chunks = partition_chunks(boundaries);
    if (profile) {
        profile->partition_ns = duration_ns(partition_start, Clock::now());
        profile->chunk_count = chunks.size();
        chunk_parse_ns.assign(chunks.size(), 0);
    }

    if (chunks.size() <= 1) {
        used_parallel_mode_ = false;
        return parse_sequential_with_errors();
    }

    // Phase 3: Parse chunks in parallel
    util::ThreadPool pool(config_.num_threads);

    std::vector<std::function<ChunkResult()>> tasks;
    tasks.reserve(chunks.size());

    std::vector<std::future<ChunkResult>> futures;
    futures.reserve(chunks.size());

    std::vector<uint64_t>* parse_ns_out = profile ? &chunk_parse_ns : nullptr;
    for (const auto& chunk : chunks) {
        tasks.emplace_back([this, chunk, skip_utf8_validation, parse_ns_out]() {
            return parse_chunk(chunk, skip_utf8_validation, parse_ns_out);
        });
    }

    Clock::time_point submit_start;
    if (profile) {
        submit_start = Clock::now();
    }
    pool.submit_bulk(tasks, futures);
    if (profile) {
        profile->submit_ns = duration_ns(submit_start, Clock::now());
    }

    // Collect results
    std::vector<ChunkResult> chunk_results;
    chunk_results.reserve(futures.size());

    Clock::time_point wait_start;
    if (profile) {
        wait_start = Clock::now();
    }
    for (auto& f : futures) {
        chunk_results.push_back(f.get());
    }
    if (profile) {
        profile->wait_ns = duration_ns(wait_start, Clock::now());
    }

    // Phase 4: Merge results in order
    Clock::time_point merge_start;
    if (profile) {
        merge_start = Clock::now();
    }
    merge_results(chunk_results, result.values, result.errors);
    if (profile) {
        profile->merge_ns = duration_ns(merge_start, Clock::now());
    }

    // Update statistics
    result.lines_processed = result.values.size() + result.errors.size();
    lines_processed_ = result.lines_processed;
    error_count_ = result.errors.size();

    if (profile) {
        uint64_t parse_total = 0;
        uint64_t parse_max = 0;
        for (uint64_t ns : chunk_parse_ns) {
            parse_total += ns;
            if (ns > parse_max) {
                parse_max = ns;
            }
        }
        profile->parse_ns_total = parse_total;
        profile->parse_ns_max = parse_max;
        uint64_t overhead_ns = profile->submit_ns + profile->merge_ns;
        if (profile->wait_ns > parse_max) {
            overhead_ns += (profile->wait_ns - parse_max);
        }
        profile->overhead_ns = overhead_ns;
        profile->overhead_per_chunk_ns =
            profile->chunk_count ? (overhead_ns / profile->chunk_count) : 0;
    }

    return result;
}

std::vector<NdjsonSearchMatch> ParallelNdjsonStream::search_all_parallel(
    const CompiledPath& path) {
    if (data_.empty()) {
        return {};
    }

    validate_utf8_once();
    const bool skip_utf8_validation = utf8_checked_ && utf8_ok_;

    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;

    if (line_count < config_.min_lines_for_parallel ||
        data_.size() < config_.min_chunk_size * 2) {
        used_parallel_mode_ = false;
        ParallelSearchResult seq = search_sequential_with_errors(path);
        if (!seq.errors.empty() && !config_.skip_errors) {
            throw std::runtime_error("Parse error at line " +
                                     std::to_string(seq.errors[0].first) + ": " +
                                     seq.errors[0].second);
        }
        return seq.matches;
    }

    used_parallel_mode_ = true;

    std::vector<size_t> boundaries = collect_line_boundaries();
    std::vector<Chunk> chunks = partition_chunks(boundaries);

    if (chunks.size() <= 1) {
        used_parallel_mode_ = false;
        ParallelSearchResult seq = search_sequential_with_errors(path);
        if (!seq.errors.empty() && !config_.skip_errors) {
            throw std::runtime_error("Parse error at line " +
                                     std::to_string(seq.errors[0].first) + ": " +
                                     seq.errors[0].second);
        }
        return seq.matches;
    }

    util::ThreadPool pool(config_.num_threads);
    std::vector<std::function<ChunkSearchResult()>> tasks;
    tasks.reserve(chunks.size());

    std::vector<std::future<ChunkSearchResult>> futures;
    futures.reserve(chunks.size());

    for (const auto& chunk : chunks) {
        tasks.emplace_back([this, chunk, &path, skip_utf8_validation]() {
            return search_chunk(chunk, path, skip_utf8_validation);
        });
    }

    pool.submit_bulk(tasks, futures);

    std::vector<ChunkSearchResult> chunk_results;
    chunk_results.reserve(futures.size());
    for (auto& f : futures) {
        chunk_results.push_back(f.get());
    }

    std::vector<NdjsonSearchMatch> matches;
    std::vector<std::pair<size_t, std::string>> errors;
    size_t lines_processed = 0;
    merge_search_results(chunk_results, matches, errors, &lines_processed);

    lines_processed_ = lines_processed;
    error_count_ = errors.size();

    if (!errors.empty() && !config_.skip_errors) {
        throw std::runtime_error("Parse error at line " + std::to_string(errors[0].first) +
                                 ": " + errors[0].second);
    }

    return matches;
}

ParallelSearchResult ParallelNdjsonStream::search_all_parallel_with_errors(
    const CompiledPath& path) {
    ParallelSearchResult result;

    if (data_.empty()) {
        return result;
    }

    validate_utf8_once();
    const bool skip_utf8_validation = utf8_checked_ && utf8_ok_;

    size_t line_count = util::count_newlines_simd(data_.data(), data_.size()) + 1;

    if (line_count < config_.min_lines_for_parallel ||
        data_.size() < config_.min_chunk_size * 2) {
        used_parallel_mode_ = false;
        return search_sequential_with_errors(path);
    }

    used_parallel_mode_ = true;

    std::vector<size_t> boundaries = collect_line_boundaries();
    std::vector<Chunk> chunks = partition_chunks(boundaries);

    if (chunks.size() <= 1) {
        used_parallel_mode_ = false;
        return search_sequential_with_errors(path);
    }

    util::ThreadPool pool(config_.num_threads);
    std::vector<std::function<ChunkSearchResult()>> tasks;
    tasks.reserve(chunks.size());

    std::vector<std::future<ChunkSearchResult>> futures;
    futures.reserve(chunks.size());

    for (const auto& chunk : chunks) {
        tasks.emplace_back([this, chunk, &path, skip_utf8_validation]() {
            return search_chunk(chunk, path, skip_utf8_validation);
        });
    }

    pool.submit_bulk(tasks, futures);

    std::vector<ChunkSearchResult> chunk_results;
    chunk_results.reserve(futures.size());
    for (auto& f : futures) {
        chunk_results.push_back(f.get());
    }

    merge_search_results(chunk_results, result.matches, result.errors, &result.lines_processed);

    lines_processed_ = result.lines_processed;
    error_count_ = result.errors.size();

    return result;
}

}  // namespace strata

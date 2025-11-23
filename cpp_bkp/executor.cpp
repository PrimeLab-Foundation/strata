/**
 * executor.cpp - Unified Parallel Executor (PR #3f)
 */

#include "executor.hpp"

#include "jp_eval_pushdown.hpp"
#include "partitioner.hpp"

#include <chrono>
#include <jsoncons/json.hpp>
#include <sstream>

namespace strata {

using clock = std::chrono::high_resolution_clock;

// ============================================================================
// Result Normalization (ensures serial/parallel byte-identical output)
// ============================================================================

/**
 * Normalize JSONPath evaluation result for output
 *
 * This function implements the same unwrapping logic as the serial path's
 * evaluate_and_yield() to ensure byte-for-byte identical results between
 * serial and parallel execution.
 *
 * JSONPath queries return results wrapped in arrays (e.g., $.id on {"id": 42}
 * returns "[42]"). The serial path unwraps these arrays and yields individual
 * elements. The parallel path must do the same.
 *
 * Test coverage: test_threading_stability.py::test_serial_parallel_equivalence_*
 *
 * @param result Raw JSON result from eval_pushdown_to_json
 * @param normalized_outputs Vector to append normalized results to
 */
static void normalize_result_for_output(const std::string& result,
                                        std::vector<std::string>& normalized_outputs) {
    // Empty results are skipped entirely (same as serial)
    if (result.empty() || result == "[]" || result == "null") {
        return;
    }

    try {
        jsoncons::json result_json = jsoncons::json::parse(result);

        // If result is an array, yield each element separately
        // This matches scanner.cpp::evaluate_and_yield behavior
        if (result_json.is_array()) {
            for (const auto& item : result_json.array_range()) {
                std::ostringstream oss;
                jsoncons::encode_json(item, oss, jsoncons::indenting::no_indent);
                normalized_outputs.push_back(oss.str());
            }
        } else {
            // Single value - yield as-is
            normalized_outputs.push_back(result);
        }
    } catch (...) {
        // If parsing fails, yield the raw result (same as serial fallback)
        normalized_outputs.push_back(result);
    }
}

// ============================================================================
// Coordinator
// ============================================================================

void coordinator_thread(BoundedQueue<ResultItem>* queue, const YieldFn& yield, ErrorState* errors,
                        size_t num_workers) {
    std::priority_queue<ResultItem, std::vector<ResultItem>, std::greater<ResultItem>> heap;

    uint64_t next_expected = 0;

    while (true) {
        ResultItem item;
        if (!queue->pop(item)) {
            break;
        }

        heap.push(std::move(item));

        while (!heap.empty() && heap.top().seq_id == next_expected) {
            // Normalize before yielding to ensure byte-identical output with serial
            std::vector<std::string> normalized;
            normalize_result_for_output(heap.top().payload, normalized);
            for (const auto& result : normalized) {
                yield(result);
            }
            next_expected++;
            heap.pop();
        }
    }

    // Final drain
    while (!heap.empty() && heap.top().seq_id == next_expected) {
        std::vector<std::string> normalized;
        normalize_result_for_output(heap.top().payload, normalized);
        for (const auto& result : normalized) {
            yield(result);
        }
        next_expected++;
        heap.pop();
    }
}

// ============================================================================
// NDJSON Worker
// ============================================================================

void worker_ndjson(WorkerContext ctx) {
    WorkerStats& stats = *ctx.stats;

    try {
        for (const auto& part : ctx.partitions) {
            std::string_view part_data(ctx.buffer.data() + part.offset, part.length);

            simdjson::padded_string padded(part_data.data(), part_data.size());

            auto parse_start = clock::now();
            simdjson::ondemand::document_stream stream;
            auto error = ctx.parser.iterate_many(padded).get(stream);

            if (error) {
                ctx.errors->set(std::string("Worker parse error: ") +
                                simdjson::error_message(error));
                return;
            }

            auto parse_end = clock::now();
            stats.parse_ms +=
                std::chrono::duration<double, std::milli>(parse_end - parse_start).count();
            stats.bytes_scanned += part.length;

            size_t local_idx = 0;
            for (auto doc_result : stream) {
                if (ctx.errors->raised()) {
                    return;
                }

                if (doc_result.error()) {
                    local_idx++;
                    continue;
                }

                auto doc = doc_result.value();
                stats.objects_visited++;

                simdjson::ondemand::value value;
                if (doc.get_value().get(value)) {
                    local_idx++;
                    continue;
                }

                auto eval_start = clock::now();
                std::string result = eval_pushdown_to_json(value, *ctx.expr, nullptr);
                auto eval_end = clock::now();

                stats.eval_ms +=
                    std::chrono::duration<double, std::milli>(eval_end - eval_start).count();

                // Push raw result; coordinator will normalize after ordering
                if (!result.empty() && result != "[]" && result != "null") {
                    stats.items_materialized++;
                    uint64_t seq_id = part.row_begin + local_idx;

                    if (!ctx.queue->push(ResultItem{seq_id, std::move(result)})) {
                        return;
                    }
                }

                local_idx++;
            }
        }
    } catch (const std::exception& e) {
        ctx.errors->set(std::string("Worker exception: ") + e.what());
    }
}

// ============================================================================
// Top-Key Worker
// ============================================================================

void worker_topkey(WorkerContext ctx) {
    WorkerStats& stats = *ctx.stats;

    try {
        for (const auto& part : ctx.partitions) {
            for (size_t elem_idx = part.row_begin; elem_idx < part.row_end; ++elem_idx) {
                if (ctx.errors->raised()) {
                    return;
                }

                const ElementSpan& span = (*ctx.element_spans)[elem_idx];

                std::string_view elem_data(ctx.buffer.data() + span.offset, span.length);

                auto parse_start = clock::now();
                simdjson::padded_string padded(elem_data.data(), elem_data.size());
                auto doc = ctx.parser.iterate(padded);
                auto parse_end = clock::now();

                stats.parse_ms +=
                    std::chrono::duration<double, std::milli>(parse_end - parse_start).count();
                stats.bytes_scanned += elem_data.size();
                stats.objects_visited++;

                simdjson::ondemand::value value;
                if (doc.get_value().get(value)) {
                    continue;
                }

                auto eval_start = clock::now();
                std::string result = eval_pushdown_to_json(value, *ctx.expr, nullptr);
                auto eval_end = clock::now();

                stats.eval_ms +=
                    std::chrono::duration<double, std::milli>(eval_end - eval_start).count();

                // Push raw result; coordinator will normalize after ordering
                if (!result.empty() && result != "[]" && result != "null") {
                    stats.items_materialized++;

                    if (!ctx.queue->push(ResultItem{elem_idx, std::move(result)})) {
                        return;
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        ctx.errors->set(std::string("Worker exception: ") + e.what());
    }
}

// ============================================================================
// NDJSON Parallel Executor
// ============================================================================

void execute_ndjson_parallel(simdjson::padded_string& padded, const CompiledExpr& ce,
                             const Config& cfg, Stats* stats, const YieldFn& yield) {
    std::string_view data(padded.data(), padded.size());
    ExecutionPlan plan =
        build_ndjson_plan(data, cfg.target_partition_bytes, cfg.min_partition_rows);

    if (plan.parts.empty()) {
        return;
    }

    BoundedQueue<ResultItem> queue(cfg.queue_capacity);
    ErrorState errors;
    std::vector<WorkerStats> worker_stats(cfg.threads);

    std::vector<std::vector<Partition>> worker_parts(cfg.threads);
    for (size_t i = 0; i < plan.parts.size(); ++i) {
        worker_parts[i % cfg.threads].push_back(plan.parts[i]);
    }

    std::vector<std::thread> workers;
    for (size_t i = 0; i < cfg.threads; ++i) {
        WorkerContext ctx;
        ctx.buffer = data;
        ctx.expr = &ce;
        ctx.partitions = std::move(worker_parts[i]);
        ctx.queue = &queue;
        ctx.errors = &errors;
        ctx.config = &cfg;
        ctx.worker_id = i;
        ctx.stats = &worker_stats[i];

        workers.emplace_back(worker_ndjson, std::move(ctx));
    }

    std::thread coord(coordinator_thread, &queue, yield, &errors, cfg.threads);

    for (auto& w : workers) {
        w.join();
    }

    queue.shutdown();
    coord.join();

    if (stats) {
        for (const auto& ws : worker_stats) {
            stats->add_parse_ms(ws.parse_ms);
            stats->add_eval_ms(ws.eval_ms);
            stats->add_bytes_scanned(ws.bytes_scanned);
            stats->add_objects_visited(ws.objects_visited);
            stats->add_items_materialized(ws.items_materialized);
        }
        stats->update_peak_rss();
    }

    if (errors.raised()) {
        throw std::runtime_error(errors.message());
    }
}

// ============================================================================
// Top-Key Parallel Executor
// ============================================================================

void execute_topkey_parallel(simdjson::padded_string& padded, const std::string& top_key,
                             const CompiledExpr& ce, const Config& cfg, Stats* stats,
                             const YieldFn& yield) {
    std::string_view data(padded.data(), padded.size());
    auto elements = index_topkey_array(data, top_key);

    if (elements.empty()) {
        return;
    }

    ExecutionPlan plan =
        pack_topkey_partitions(elements, cfg.target_partition_bytes, cfg.shard_items);

    if (plan.parts.empty()) {
        return;
    }

    BoundedQueue<ResultItem> queue(cfg.queue_capacity);
    ErrorState errors;
    std::vector<WorkerStats> worker_stats(cfg.threads);

    std::vector<std::vector<Partition>> worker_parts(cfg.threads);
    for (size_t i = 0; i < plan.parts.size(); ++i) {
        worker_parts[i % cfg.threads].push_back(plan.parts[i]);
    }

    std::vector<std::thread> workers;
    for (size_t i = 0; i < cfg.threads; ++i) {
        WorkerContext ctx;
        ctx.buffer = data;
        ctx.expr = &ce;
        ctx.partitions = std::move(worker_parts[i]);
        ctx.queue = &queue;
        ctx.errors = &errors;
        ctx.config = &cfg;
        ctx.worker_id = i;
        ctx.stats = &worker_stats[i];
        ctx.element_spans = &elements;
        ctx.top_key = top_key;

        workers.emplace_back(worker_topkey, std::move(ctx));
    }

    std::thread coord(coordinator_thread, &queue, yield, &errors, cfg.threads);

    for (auto& w : workers) {
        w.join();
    }

    queue.shutdown();
    coord.join();

    if (stats) {
        for (const auto& ws : worker_stats) {
            stats->add_parse_ms(ws.parse_ms);
            stats->add_eval_ms(ws.eval_ms);
            stats->add_bytes_scanned(ws.bytes_scanned);
            stats->add_objects_visited(ws.objects_visited);
            stats->add_items_materialized(ws.items_materialized);
        }
        stats->update_peak_rss();
    }

    if (errors.raised()) {
        throw std::runtime_error(errors.message());
    }
}

} // namespace strata

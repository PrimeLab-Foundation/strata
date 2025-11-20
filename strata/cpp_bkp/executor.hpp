/**
 * executor.hpp - Unified Parallel Executor (PR #3f)
 *
 * Provides deadlock-proof parallel execution infrastructure for both NDJSON
 * and Top-Key array processing with:
 * - Bounded MPSC queue with backpressure
 * - Deterministic ordering via priority heap
 * - Safe cancellation and error propagation
 * - Per-worker arenas and stats
 */

#pragma once

#include "jp_compile.hpp"
#include "json_writer.hpp"
#include "partitioner.hpp"
#include "scanner.hpp"
#include "stats.hpp"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <simdjson.h>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace strata {

// ============================================================================
// Core Types
// ============================================================================

/**
 * ResultItem - Single result with sequence ID for ordering
 */
struct ResultItem {
    uint64_t seq_id;     // Line index (NDJSON) or element index (Top-Key)
    std::string payload; // Compact JSON result (owned)

    // Priority queue needs greater comparator for min-heap
    bool operator>(const ResultItem& other) const { return seq_id > other.seq_id; }
};

/**
 * ErrorState - Thread-safe error tracking for parallel execution
 *
 * Provides lock-free error checking (raised()) and mutex-protected error setting.
 * Only the first error is recorded (subsequent set() calls are ignored).
 */
class ErrorState {
  public:
    ErrorState() noexcept : has_error_(false) {}

    // Non-copyable, non-movable (contains mutex)
    ErrorState(const ErrorState&) = delete;
    ErrorState& operator=(const ErrorState&) = delete;

    /// Record error (only first error is stored)
    /// Thread-safe: multiple threads may call concurrently
    void set(std::string msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!has_error_.load(std::memory_order_acquire)) {
            message_ = std::move(msg);
            has_error_.store(true, std::memory_order_release);
        }
    }

    /// Check if any error has been recorded (lock-free)
    [[nodiscard]] bool raised() const noexcept {
        return has_error_.load(std::memory_order_acquire);
    }

    /// Retrieve error message (makes a copy for thread safety)
    [[nodiscard]] std::string message() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return message_;
    }

  private:
    std::atomic<bool> has_error_;
    mutable std::mutex mutex_;
    std::string message_;
};

/**
 * BoundedQueue - Thread-safe MPSC queue with backpressure
 *
 * Multiple producers (workers) can push; single consumer (coordinator) pops.
 * Blocks when full (backpressure) or empty (waiting for data).
 */
template <typename T> class BoundedQueue {
  public:
    explicit BoundedQueue(size_t capacity) : capacity_(capacity), shutdown_(false) {}

    /**
     * Push item (blocks if full, returns false if shutdown)
     */
    bool push(T&& item) {
        std::unique_lock<std::mutex> lock(mutex_);

        // Wait for space or shutdown
        not_full_.wait(lock, [this] { return queue_.size() < capacity_ || shutdown_.load(); });

        if (shutdown_.load()) {
            return false;
        }

        queue_.push(std::move(item));
        not_empty_.notify_one();
        return true;
    }

    /**
     * Pop item (blocks if empty, returns false if shutdown and empty)
     */
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);

        // Wait for data or shutdown
        not_empty_.wait(lock, [this] { return !queue_.empty() || shutdown_.load(); });

        if (queue_.empty()) {
            return false; // Shutdown and no more items
        }

        item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return true;
    }

    /**
     * Shutdown queue (unblocks all waiters)
     */
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_.store(true);
        }
        not_empty_.notify_all();
        not_full_.notify_all();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    bool is_shutdown() const { return shutdown_.load(); }

  private:
    size_t capacity_;
    std::atomic<bool> shutdown_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::queue<T> queue_;
};

/**
 * WorkerStats - Per-worker counters
 */
struct WorkerStats {
    double parse_ms = 0.0;
    double eval_ms = 0.0;
    uint64_t bytes_scanned = 0;
    uint64_t objects_visited = 0;
    uint64_t items_materialized = 0;
};

/**
 * WorkerContext - Generic worker context
 */
struct WorkerContext {
    // Input data
    std::string_view buffer;
    const CompiledExpr* expr;

    // Partitions to process
    std::vector<Partition> partitions;

    // Shared state
    BoundedQueue<ResultItem>* queue;
    ErrorState* errors;
    const Config* config;

    // Per-worker state
    size_t worker_id;
    WorkerStats* stats; // Points to per-worker stats in vector

    // For Top-Key: element spans
    const std::vector<ElementSpan>* element_spans = nullptr;
    std::string top_key;

    // Per-worker resources (must be initialized before use)
    simdjson::ondemand::parser parser;
};

// ============================================================================
// Coordinator
// ============================================================================

/**
 * Coordinator thread - Deterministic merge via min-heap
 *
 * Maintains ordering by seq_id while workers produce out-of-order.
 */
void coordinator_thread(BoundedQueue<ResultItem>* queue, const YieldFn& yield, ErrorState* errors,
                        size_t num_workers);

// ============================================================================
// Worker Functions
// ============================================================================

/**
 * NDJSON worker - Process NDJSON partitions
 */
void worker_ndjson(WorkerContext ctx);

/**
 * Top-Key worker - Process top-key array partitions
 */
void worker_topkey(WorkerContext ctx);

// ============================================================================
// Executor Entry Points
// ============================================================================

/**
 * Execute NDJSON search in parallel
 */
void execute_ndjson_parallel(simdjson::padded_string& padded, const CompiledExpr& ce,
                             const Config& cfg, Stats* stats, const YieldFn& yield);

/**
 * Execute top-key array search in parallel
 */
void execute_topkey_parallel(simdjson::padded_string& padded, const std::string& top_key,
                             const CompiledExpr& ce, const Config& cfg, Stats* stats,
                             const YieldFn& yield);

} // namespace strata

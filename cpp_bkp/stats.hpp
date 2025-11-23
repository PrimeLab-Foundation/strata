#pragma once

#include "platform.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>

namespace strata {

/**
 * Performance and memory statistics for query execution
 * Thread-safe for concurrent updates
 */
class Stats {
  public:
    Stats() { reset(); }

    void reset() {
        parse_ms_ = 0.0;
        eval_ms_ = 0.0;
        bytes_scanned_ = 0;
        objects_visited_ = 0;
        items_materialized_ = 0;
        peak_rss_mb_ = 0.0;
        start_rss_bytes_ = get_current_rss_bytes();
    }

    // Getters
    double parse_ms() const { return parse_ms_.load(std::memory_order_relaxed); }
    double eval_ms() const { return eval_ms_.load(std::memory_order_relaxed); }
    uint64_t bytes_scanned() const { return bytes_scanned_.load(std::memory_order_relaxed); }
    uint64_t objects_visited() const { return objects_visited_.load(std::memory_order_relaxed); }
    uint64_t items_materialized() const {
        return items_materialized_.load(std::memory_order_relaxed);
    }
    double peak_rss_mb() const { return peak_rss_mb_.load(std::memory_order_relaxed); }

    // Setters (thread-safe)
    void add_parse_ms(double ms) {
        double current = parse_ms_.load(std::memory_order_relaxed);
        while (!parse_ms_.compare_exchange_weak(current, current + ms, std::memory_order_relaxed))
            ;
    }

    void add_eval_ms(double ms) {
        double current = eval_ms_.load(std::memory_order_relaxed);
        while (!eval_ms_.compare_exchange_weak(current, current + ms, std::memory_order_relaxed))
            ;
    }

    void add_bytes_scanned(uint64_t bytes) {
        bytes_scanned_.fetch_add(bytes, std::memory_order_relaxed);
    }

    void add_objects_visited(uint64_t count) {
        objects_visited_.fetch_add(count, std::memory_order_relaxed);
    }

    void add_items_materialized(uint64_t count) {
        items_materialized_.fetch_add(count, std::memory_order_relaxed);
    }

    void update_peak_rss() {
        size_t current_rss = get_current_rss_bytes();
        size_t peak = get_peak_rss_bytes();

        // Use the max of current and system-reported peak
        size_t rss = std::max(current_rss, peak);
        double rss_mb = static_cast<double>(rss) / (1024.0 * 1024.0);

        double current_peak = peak_rss_mb_.load(std::memory_order_relaxed);
        while (rss_mb > current_peak) {
            if (peak_rss_mb_.compare_exchange_weak(current_peak, rss_mb,
                                                   std::memory_order_relaxed)) {
                break;
            }
        }
    }

    // Derived metrics
    double materialization_ratio() const {
        uint64_t visited = objects_visited();
        return visited > 0 ? static_cast<double>(items_materialized()) / visited : 0.0;
    }

    double throughput_mbs() const {
        double total_ms = parse_ms() + eval_ms();
        if (total_ms <= 0.0)
            return 0.0;
        return (bytes_scanned() / (1024.0 * 1024.0)) / (total_ms / 1000.0);
    }

    // Export stats as JSON string (for CI artifacts)
    std::string to_json() const {
        char buf[512];
        snprintf(buf, sizeof(buf),
                 "{"
                 "\"parse_ms\":%.2f,"
                 "\"eval_ms\":%.2f,"
                 "\"bytes_scanned\":%llu,"
                 "\"objects_visited\":%llu,"
                 "\"items_materialized\":%llu,"
                 "\"peak_rss_mb\":%.2f,"
                 "\"materialization_ratio\":%.4f,"
                 "\"throughput_mbs\":%.2f"
                 "}",
                 parse_ms(), eval_ms(), (unsigned long long)bytes_scanned(),
                 (unsigned long long)objects_visited(), (unsigned long long)items_materialized(),
                 peak_rss_mb(), materialization_ratio(), throughput_mbs());
        return std::string(buf);
    }

  private:
    std::atomic<double> parse_ms_;
    std::atomic<double> eval_ms_;
    std::atomic<uint64_t> bytes_scanned_;
    std::atomic<uint64_t> objects_visited_;
    std::atomic<uint64_t> items_materialized_;
    std::atomic<double> peak_rss_mb_;
    size_t start_rss_bytes_;
};

/**
 * RAII timer for automatic time tracking
 */
class ScopedTimer {
  public:
    explicit ScopedTimer(Stats* stats, bool is_parse)
        : stats_(stats), is_parse_(is_parse), start_(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start_);

        if (stats_) {
            if (is_parse_) {
                stats_->add_parse_ms(duration.count());
            } else {
                stats_->add_eval_ms(duration.count());
            }
        }
    }

    // Non-copyable
    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

  private:
    Stats* stats_;
    bool is_parse_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

} // namespace strata

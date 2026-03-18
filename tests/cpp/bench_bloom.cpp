/**
 * @file bench_bloom.cpp
 * @brief Benchmarks for the Bloom filter subsystem.
 *
 * Measures:
 *   1. Raw Bloom filter lookup throughput (queries/sec) for various sizes
 *   2. KeyFilter::should_parse() vs. std::unordered_set::count()
 *   3. DedupFilter vs. std::unordered_set<string_view>
 *   4. False positive rates: observed vs. theoretical
 */

#include "strata/bloom/bloom_filter.h"
#include "strata/bloom/dedup_filter.h"
#include "strata/bloom/key_filter.h"
#include "strata/bloom/schema_filter.h"
#include "strata/bloom/sizing.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace strata::bloom;

// ─── Timing utility ──────────────────────────────────────────────────────────

struct Timer {
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start;

    Timer() : start(Clock::now()) {}

    double elapsed_ns() const {
        auto end = Clock::now();
        return std::chrono::duration<double, std::nano>(end - start).count();
    }

    double elapsed_ms() const { return elapsed_ns() / 1e6; }
};

// ─── Helper: generate random keys ────────────────────────────────────────────

static std::vector<std::string> generate_keys(size_t count, std::mt19937& rng, size_t min_len = 5,
                                              size_t max_len = 20) {
    std::uniform_int_distribution<size_t> len_dist(min_len, max_len);
    std::uniform_int_distribution<int> char_dist('a', 'z');

    std::vector<std::string> keys;
    keys.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        size_t len = len_dist(rng);
        std::string s;
        s.reserve(len);
        for (size_t j = 0; j < len; ++j) {
            s += static_cast<char>(char_dist(rng));
        }
        keys.push_back(std::move(s));
    }
    return keys;
}

// =============================================================================
// Benchmark 1: Raw Bloom Filter Lookup Throughput
// =============================================================================

template <size_t Bits, size_t Hashes>
static void bench_bloom_lookup(const char* label, size_t num_inserted, size_t num_queries) {
    std::mt19937 rng(42);

    BloomFilter<Bits, Hashes> filter;
    auto inserted = generate_keys(num_inserted, rng);
    for (const auto& key : inserted) {
        filter.insert(key);
    }

    auto queries = generate_keys(num_queries, rng);

    // Warm up.
    volatile bool sink = false;
    for (size_t i = 0; i < 1000 && i < queries.size(); ++i) {
        sink = filter.maybe_contains(queries[i]);
    }
    (void)sink;

    Timer t;
    size_t positives = 0;
    for (const auto& q : queries) {
        if (filter.maybe_contains(q))
            ++positives;
    }
    double elapsed = t.elapsed_ns();

    double ns_per_query = elapsed / static_cast<double>(num_queries);
    double queries_per_sec = 1e9 / ns_per_query;

    std::printf("  %-30s %8.1f ns/query  %8.1f M queries/s  (FP: %.2f%%)\n", label, ns_per_query,
                queries_per_sec / 1e6,
                100.0 * static_cast<double>(positives) / static_cast<double>(num_queries));
}

static void run_bench_bloom_lookup() {
    std::printf("\n── Benchmark: Bloom Filter Lookup Throughput ──\n");

    constexpr size_t kQueries = 1'000'000;

    bench_bloom_lookup<256, 3>("Tiny (256b, 3h, 10 keys)", 10, kQueries);
    bench_bloom_lookup<512, 3>("Small (512b, 3h, 50 keys)", 50, kQueries);
    bench_bloom_lookup<2048, 3>("Medium (2048b, 3h, 200 keys)", 200, kQueries);
    bench_bloom_lookup<10240, 3>("Large (10240b, 3h, 1000 keys)", 1000, kQueries);
}

// =============================================================================
// Benchmark 2: KeyFilter vs. unordered_set
// =============================================================================

static void bench_key_filter_vs_set(size_t num_desired, size_t num_queries) {
    std::mt19937 rng(42);
    auto desired_strings = generate_keys(num_desired, rng);
    std::vector<std::string_view> desired_svs;
    for (const auto& s : desired_strings) {
        desired_svs.push_back(s);
    }

    // Build KeyFilter.
    KeyFilter kf(desired_svs);

    // Build unordered_set.
    std::unordered_set<std::string> uset(desired_strings.begin(), desired_strings.end());

    // Generate query keys (mix of desired and random).
    auto random_queries = generate_keys(num_queries, rng);
    // Sprinkle in some desired keys.
    for (size_t i = 0; i < num_desired && i < random_queries.size(); ++i) {
        random_queries[i * (num_queries / num_desired)] = desired_strings[i];
    }

    // Benchmark KeyFilter.
    volatile bool sink = false;
    {
        Timer t;
        for (const auto& q : random_queries) {
            sink = kf.should_parse(q);
        }
        double elapsed = t.elapsed_ns();
        double ns_per = elapsed / static_cast<double>(num_queries);
        std::printf("  KeyFilter (%3zu desired): %8.1f ns/query\n", num_desired, ns_per);
    }

    // Benchmark unordered_set.
    {
        Timer t;
        for (const auto& q : random_queries) {
            sink = uset.count(q) > 0;
        }
        double elapsed = t.elapsed_ns();
        double ns_per = elapsed / static_cast<double>(num_queries);
        std::printf("  unordered_set (%3zu keys): %8.1f ns/query\n", num_desired, ns_per);
    }
    (void)sink;
}

static void run_bench_key_filter_vs_set() {
    std::printf("\n── Benchmark: KeyFilter vs. unordered_set ──\n");

    constexpr size_t kQueries = 1'000'000;
    bench_key_filter_vs_set(5, kQueries);
    bench_key_filter_vs_set(50, kQueries);
    bench_key_filter_vs_set(500, kQueries);
}

// =============================================================================
// Benchmark 3: DedupFilter vs. unordered_set
// =============================================================================

static void run_bench_dedup() {
    std::printf("\n── Benchmark: DedupFilter vs. unordered_set ──\n");

    std::mt19937 rng(42);
    constexpr size_t kKeys = 100'000;
    auto keys = generate_keys(kKeys, rng, 3, 15);

    // DedupFilter benchmark.
    {
        DedupFilter dedup;
        Timer t;
        size_t suspects = 0;
        for (const auto& key : keys) {
            if (dedup.check_and_insert(key))
                ++suspects;
        }
        double elapsed = t.elapsed_ns();
        std::printf("  DedupFilter:      %8.1f ns/op  (suspects: %zu)\n",
                    elapsed / static_cast<double>(kKeys), suspects);
    }

    // unordered_set benchmark.
    {
        std::unordered_set<std::string_view> seen;
        Timer t;
        size_t duplicates = 0;
        for (const auto& key : keys) {
            auto [it, inserted] = seen.insert(key);
            if (!inserted)
                ++duplicates;
        }
        double elapsed = t.elapsed_ns();
        std::printf("  unordered_set:    %8.1f ns/op  (duplicates: %zu)\n",
                    elapsed / static_cast<double>(kKeys), duplicates);
    }
}

// =============================================================================
// Benchmark 4: False Positive Rate — Observed vs. Theoretical
// =============================================================================

template <size_t Bits, size_t Hashes>
static void bench_fpr(const char* label, size_t num_inserted) {
    std::mt19937 rng(42);

    BloomFilter<Bits, Hashes> filter;
    auto inserted = generate_keys(num_inserted, rng);
    for (const auto& key : inserted) {
        filter.insert(key);
    }

    // Query 100k random keys not in the set.
    constexpr size_t kQueries = 100'000;
    auto queries = generate_keys(kQueries, rng, 20, 30); // different length range
    size_t fps = 0;
    for (const auto& q : queries) {
        if (filter.maybe_contains(q))
            ++fps;
    }

    double measured = static_cast<double>(fps) / static_cast<double>(kQueries);

    // Theoretical FPR.  For blocked filters (Bits > 512), each key maps to
    // one of (Bits/512) blocks, and all k probes land within that 512-bit block.
    // Effective per-block occupancy is n/num_blocks, probed against 512 bits.
    double effective_bits = static_cast<double>(Bits);
    if (Bits > 512) {
        size_t num_blocks = Bits / 512;
        double keys_per_block = static_cast<double>(num_inserted) / static_cast<double>(num_blocks);
        effective_bits = 512.0;
        double kn_m = static_cast<double>(Hashes) * keys_per_block / effective_bits;
        double theoretical = std::pow(1.0 - std::exp(-kn_m), static_cast<double>(Hashes));
        double ratio = (theoretical > 0) ? measured / theoretical : 0.0;
        std::printf("  %-30s measured=%.4f%%  theoretical=%.4f%%  ratio=%.2fx (blocked)\n", label,
                    measured * 100.0, theoretical * 100.0, ratio);
        return;
    }
    double kn_m = static_cast<double>(Hashes * num_inserted) / static_cast<double>(Bits);
    double theoretical = std::pow(1.0 - std::exp(-kn_m), static_cast<double>(Hashes));

    double ratio = (theoretical > 0) ? measured / theoretical : 0.0;

    std::printf("  %-30s measured=%.4f%%  theoretical=%.4f%%  ratio=%.2fx\n", label,
                measured * 100.0, theoretical * 100.0, ratio);
}

static void run_bench_fpr() {
    std::printf("\n── False Positive Rates: Observed vs. Theoretical ──\n");

    bench_fpr<256, 3>("Tiny (256b/3h/10 keys)", 10);
    bench_fpr<512, 3>("Small (512b/3h/50 keys)", 50);
    bench_fpr<2048, 3>("Medium (2048b/3h/200 keys)", 200);
    bench_fpr<10240, 3>("Large (10240b/3h/1000 keys)", 1000);
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::printf("=== Bloom Filter Subsystem Benchmarks ===\n");

    run_bench_bloom_lookup();
    run_bench_key_filter_vs_set();
    run_bench_dedup();
    run_bench_fpr();

    std::printf("\n=== Benchmarks complete ===\n");
    return 0;
}

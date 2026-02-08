/**
 * parallel_ndjson_bench.cpp - Benchmark for parallel NDJSON parsing
 *
 * Usage: ./parallel_ndjson_bench [num_lines] [num_threads] [payload_bytes]
 *   num_lines: Number of NDJSON lines to generate (default: 100000)
 *   num_threads: Number of threads for parallel parsing (default: 0 = auto)
 *   payload_bytes: Optional padding bytes per line (default: 0)
 */

#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace strata;
using Clock = std::chrono::high_resolution_clock;

// Generate NDJSON test data
std::string generate_ndjson(size_t num_lines, size_t payload_bytes) {
    std::string padding;
    if (payload_bytes > 0) {
        padding.assign(payload_bytes, 'x');
    }
    std::ostringstream oss;
    for (size_t i = 0; i < num_lines; ++i) {
        oss << "{\"id\": " << i << ", \"value\": \"item_" << i << "\", \"active\": true";
        if (!padding.empty()) {
            oss << ", \"padding\": \"" << padding << "\"";
        }
        oss << "}\n";
    }
    return oss.str();
}

// Format duration in human-readable form
std::string format_duration(double ms) {
    std::ostringstream oss;
    if (ms < 1.0) {
        oss << std::fixed << std::setprecision(1) << (ms * 1000) << " µs";
    } else if (ms < 1000.0) {
        oss << std::fixed << std::setprecision(2) << ms << " ms";
    } else {
        oss << std::fixed << std::setprecision(2) << (ms / 1000.0) << " s";
    }
    return oss.str();
}

// Run benchmark with multiple iterations
struct BenchResult {
    double min_ms;
    double median_ms;
    double mean_ms;
    size_t count;
};

BenchResult run_benchmark(const std::string& name, std::function<size_t()> fn, int iterations) {
    std::vector<double> times;
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        auto start = Clock::now();
        size_t count = fn();
        auto end = Clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        times.push_back(ms);

        if (count == 0) {
            std::cerr << "Warning: " << name << " returned 0 items\n";
        }
    }

    // Sort for median
    std::sort(times.begin(), times.end());

    double sum = 0;
    for (double t : times) sum += t;

    BenchResult result;
    result.min_ms = times.front();
    result.median_ms = times[times.size() / 2];
    result.mean_ms = sum / times.size();
    result.count = iterations;

    return result;
}

int main(int argc, char** argv) {
    size_t num_lines = 100000;
    size_t num_threads = 0;
    size_t payload_bytes = 0;

    if (argc > 1) {
        num_lines = std::atoi(argv[1]);
    }
    if (argc > 2) {
        num_threads = std::atoi(argv[2]);
    }
    if (argc > 3) {
        payload_bytes = std::atoi(argv[3]);
    }

    std::cout << "Parallel NDJSON Benchmark\n";
    std::cout << "=========================\n";
    std::cout << "Lines: " << num_lines << "\n";
    std::cout << "Threads: " << (num_threads == 0 ? "auto" : std::to_string(num_threads)) << "\n";
    std::cout << "Payload bytes: " << payload_bytes << "\n\n";

    // Generate test data
    std::cout << "Generating test data... ";
    std::cout.flush();
    auto gen_start = Clock::now();
    std::string data = generate_ndjson(num_lines, payload_bytes);
    auto gen_end = Clock::now();
    double gen_ms = std::chrono::duration<double, std::milli>(gen_end - gen_start).count();
    std::cout << "done (" << format_duration(gen_ms) << ")\n";
    std::cout << "Data size: " << (data.size() / 1024.0 / 1024.0) << " MB\n\n";

    const int iterations = 5;

    // Benchmark sequential parsing
    std::cout << "Sequential (NdjsonStream::parse_all):\n";
    auto seq_result = run_benchmark("sequential", [&]() {
        NdjsonStream stream(data);
        auto results = stream.parse_all();
        return results.size();
    }, iterations);
    std::cout << "  min:    " << format_duration(seq_result.min_ms) << "\n";
    std::cout << "  median: " << format_duration(seq_result.median_ms) << "\n";
    std::cout << "  mean:   " << format_duration(seq_result.mean_ms) << "\n";

    double seq_throughput = (data.size() / 1024.0 / 1024.0) / (seq_result.median_ms / 1000.0);
    std::cout << "  throughput: " << std::fixed << std::setprecision(1) << seq_throughput << " MB/s\n";
    std::cout << "  lines/s: " << std::fixed << std::setprecision(0)
              << (num_lines / (seq_result.median_ms / 1000.0)) << "\n\n";

    // Benchmark parallel parsing
    std::cout << "Parallel (ParallelNdjsonStream::parse_all_parallel):\n";
    ParallelNdjsonConfig config;
    config.num_threads = num_threads;
    config.min_lines_for_parallel = 100;  // Lower threshold for benchmark
    config.min_chunk_size = 1024;
    ParallelNdjsonProfile profile;
    config.profile = &profile;

    auto par_result = run_benchmark("parallel", [&]() {
        ParallelNdjsonStream stream(data, config);
        auto results = stream.parse_all_parallel();
        return results.size();
    }, iterations);
    std::cout << "  min:    " << format_duration(par_result.min_ms) << "\n";
    std::cout << "  median: " << format_duration(par_result.median_ms) << "\n";
    std::cout << "  mean:   " << format_duration(par_result.mean_ms) << "\n";

    double par_throughput = (data.size() / 1024.0 / 1024.0) / (par_result.median_ms / 1000.0);
    std::cout << "  throughput: " << std::fixed << std::setprecision(1) << par_throughput << " MB/s\n";
    std::cout << "  lines/s: " << std::fixed << std::setprecision(0)
              << (num_lines / (par_result.median_ms / 1000.0)) << "\n\n";
    if (profile.chunk_count > 0) {
        double overhead_us = profile.overhead_per_chunk_ns / 1000.0;
        std::cout << "  overhead/chunk: " << std::fixed << std::setprecision(2) << overhead_us
                  << " µs\n";
        std::cout << "  parse max: " << std::fixed << std::setprecision(2)
                  << (profile.parse_ns_max / 1e6) << " ms\n\n";
    }

    // Compare
    double speedup = seq_result.median_ms / par_result.median_ms;
    std::cout << "Speedup: " << std::fixed << std::setprecision(2) << speedup << "x\n";

    if (speedup >= 2.0) {
        std::cout << "✅ Target achieved: ≥2x speedup\n";
    } else if (speedup >= 1.5) {
        std::cout << "⚠️  Close to target: 1.5-2x speedup\n";
    } else if (speedup >= 1.0) {
        std::cout << "⚠️  Minimal improvement: 1.0-1.5x speedup\n";
    } else {
        std::cout << "❌ Regression: parallel is slower\n";
    }

    return 0;
}

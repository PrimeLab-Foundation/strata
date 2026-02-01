#include "strata/util/simd_string.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

std::string read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    file.seekg(0, std::ios::end);
    std::string data;
    data.resize(static_cast<size_t>(file.tellg()));
    file.seekg(0, std::ios::beg);
    file.read(data.data(), static_cast<std::streamsize>(data.size()));
    return data;
}

size_t scan_find_newline(const std::string& data) {
    size_t pos = 0;
    size_t lines = 0;
    while (pos < data.size()) {
        const char* start = data.data() + pos;
        size_t remaining = data.size() - pos;
        size_t offset = strata::util::find_newline_simd(start, remaining, 0);
        if (offset < remaining) {
            pos += offset + 1;
        } else {
            pos = data.size();
        }
        ++lines;
    }
    return lines;
}

size_t scan_collect_newlines(const std::string& data, size_t batch) {
    size_t pos = 0;
    size_t lines = 0;
    std::vector<size_t> positions;
    positions.reserve(batch);

    while (pos < data.size()) {
        strata::util::collect_newlines_simd(data.data(), data.size(), pos, batch, positions);

        if (positions.empty()) {
            ++lines;
            break;
        }

        size_t last_pos = pos;
        for (size_t newline_pos : positions) {
            ++lines;
            last_pos = newline_pos + 1;
        }
        pos = last_pos;
    }

    return lines;
}

std::vector<double> run_timings(const std::string& data, int iterations,
                                const std::function<size_t(const std::string&)>& fn) {
    std::vector<double> times;
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        volatile size_t lines = fn(data);
        (void)lines;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());
    }

    return times;
}

double median_ms(std::vector<double> times) {
    std::sort(times.begin(), times.end());
    size_t mid = times.size() / 2;
    if (times.empty()) {
        return 0.0;
    }
    if (times.size() % 2 == 0) {
        return (times[mid - 1] + times[mid]) / 2.0;
    }
    return times[mid];
}

} // namespace

int main(int argc, char** argv) {
    try {
        std::string path = "benchmarks/data/generated/users.ndjson";
        if (argc > 1) {
            path = argv[1];
        }

        std::string data = read_file(path);
        std::cout << "Dataset: " << path << " (" << data.size() / (1024 * 1024.0) << " MB)\n";

        const int iterations = 10;

        auto baseline_times = run_timings(data, iterations, scan_find_newline);
        auto chunked_times = run_timings(data, iterations, [](const std::string& input) {
            return scan_collect_newlines(input, 256);
        });

        auto min_baseline = *std::min_element(baseline_times.begin(), baseline_times.end());
        auto min_chunked = *std::min_element(chunked_times.begin(), chunked_times.end());

        std::cout << "Baseline (find_newline loop): min " << min_baseline << " ms, median "
                  << median_ms(baseline_times) << " ms\n";
        std::cout << "Chunked (collect_newlines):  min " << min_chunked << " ms, median "
                  << median_ms(chunked_times) << " ms\n";
    } catch (const std::exception& exc) {
        std::cerr << "Error: " << exc.what() << "\n";
        return 1;
    }
    return 0;
}

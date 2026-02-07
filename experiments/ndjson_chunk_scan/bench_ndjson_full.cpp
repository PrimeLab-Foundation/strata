#include "strata/json/ndjson_stream.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

double median_ms(std::vector<double> times) {
    std::sort(times.begin(), times.end());
    size_t mid = times.size() / 2;
    if (times.empty()) return 0.0;
    if (times.size() % 2 == 0) return (times[mid - 1] + times[mid]) / 2.0;
    return times[mid];
}

int main(int argc, char** argv) {
    try {
        std::string path = "benchmarks/data/generated/medium/users.ndjson";
        if (argc > 1) path = argv[1];

        std::string data = read_file(path);
        std::cout << "Dataset: " << path << " (" << data.size() / (1024.0 * 1024.0) << " MB)\n";

        const int iterations = 10;
        const int warmup = 3;

        // Warmup
        for (int i = 0; i < warmup; ++i) {
            strata::NdjsonStream stream(data);
            volatile auto results = stream.parse_all_fast(true);
            (void)results;
        }

        // Timed runs using parse_all_fast
        std::vector<double> fast_times;
        for (int i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            strata::NdjsonStream stream(data);
            auto results = stream.parse_all_fast(true);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            fast_times.push_back(elapsed.count());

            if (i == 0) {
                std::cout << "Parsed " << results.size() << " lines\n";
            }
        }

        // Timed runs using parse_all (baseline)
        std::vector<double> baseline_times;
        for (int i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            strata::NdjsonStream stream(data);
            auto results = stream.parse_all(true);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            baseline_times.push_back(elapsed.count());
        }

        auto min_fast = *std::min_element(fast_times.begin(), fast_times.end());
        auto min_baseline = *std::min_element(baseline_times.begin(), baseline_times.end());

        std::cout << "\nparse_all (baseline):     min " << min_baseline << " ms, median " << median_ms(baseline_times) << " ms\n";
        std::cout << "parse_all_fast (chunked): min " << min_fast << " ms, median " << median_ms(fast_times) << " ms\n";

        double improvement = (min_baseline - min_fast) / min_baseline * 100.0;
        std::cout << "\nImprovement: " << improvement << "%\n";

    } catch (const std::exception& exc) {
        std::cerr << "Error: " << exc.what() << "\n";
        return 1;
    }
    return 0;
}

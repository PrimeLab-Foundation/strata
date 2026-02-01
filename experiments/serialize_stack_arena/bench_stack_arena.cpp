#include "strata/util/arena_allocator.hpp"

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

struct Frame {
    std::size_t index;
    std::size_t size;
    bool first;
};

static double bench_std(std::size_t iterations, std::size_t depth, std::size_t& checksum) {
    auto start = std::chrono::steady_clock::now();
    for (std::size_t iter = 0; iter < iterations; ++iter) {
        std::vector<Frame> stack;
        stack.reserve(depth);
        for (std::size_t i = 0; i < depth; ++i) {
            stack.push_back(Frame{i, depth, i == 0});
        }
        while (!stack.empty()) {
            checksum += stack.back().index;
            stack.pop_back();
        }
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

static double bench_arena(std::size_t iterations, std::size_t depth, std::size_t& checksum) {
    strata::util::Arena arena(64 * 1024);
    auto start = std::chrono::steady_clock::now();
    for (std::size_t iter = 0; iter < iterations; ++iter) {
        arena.reset();
        using Alloc = strata::util::ArenaAllocator<Frame>;
        std::vector<Frame, Alloc> stack{Alloc(&arena)};
        stack.reserve(depth);
        for (std::size_t i = 0; i < depth; ++i) {
            stack.push_back(Frame{i, depth, i == 0});
        }
        while (!stack.empty()) {
            checksum += stack.back().index;
            stack.pop_back();
        }
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

static std::size_t parse_arg(char** begin, char** end, const char* flag, std::size_t fallback) {
    for (char** it = begin; it != end; ++it) {
        if (std::strcmp(*it, flag) == 0 && (it + 1) != end) {
            return static_cast<std::size_t>(std::strtoull(*(it + 1), nullptr, 10));
        }
    }
    return fallback;
}

int main(int argc, char** argv) {
    std::size_t iterations = parse_arg(argv, argv + argc, "--iterations", 20000);
    std::size_t depth = parse_arg(argv, argv + argc, "--depth", 512);

    std::size_t checksum = 0;

    double std_ms = bench_std(iterations, depth, checksum);
    double arena_ms = bench_arena(iterations, depth, checksum);

    std::cout << "Serialize stack alloc microbench\n";
    std::cout << "iterations=" << iterations << ", depth=" << depth << "\n";
    std::cout << "std::vector:  " << std_ms << " ms\n";
    std::cout << "arena alloc:  " << arena_ms << " ms\n";
    std::cout << "checksum=" << checksum << "\n";
    return 0;
}

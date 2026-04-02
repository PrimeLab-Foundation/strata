#include "strata/parse_value.hpp"
#include "platform.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

using namespace strata;
using Clock = std::chrono::high_resolution_clock;

struct BenchCase {
    const char *name;
    const char *input;
};

struct BenchResult {
    const char *name;
    int64_t iterations;
    double ns_per_op;
    double ops_per_sec;
};

static double measure(const char *input, int64_t iterations) {
    auto len = std::strlen(input);
    const char *end = input + len;

    // warmup
    for (int i = 0; i < 1000; ++i)
        parse<JsonValue>(input, end);

    auto start = Clock::now();
    for (int64_t i = 0; i < iterations; ++i)
        parse<JsonValue>(input, end);
    auto stop = Clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    return static_cast<double>(ns) / static_cast<double>(iterations);
}

static int64_t calibrate(const char *input, double target_ms = 500.0) {
    auto len = std::strlen(input);
    const char *end = input + len;

    int64_t n = 1000;
    while (true) {
        auto start = Clock::now();
        for (int64_t i = 0; i < n; ++i)
            parse<JsonValue>(input, end);
        auto stop = Clock::now();
        auto ms = std::chrono::duration<double, std::milli>(stop - start).count();
        if (ms >= target_ms) return n;
        n *= 2;
    }
}

int main(int argc, char *argv[]) {
    const char *output_dir = "docs/benchmarks/cpp_only";
    if (argc > 1) output_dir = argv[1];

    std::vector<BenchCase> cases = {
        // primitives
        {"null", "null"},
        {"bool_true", "true"},
        {"bool_false", "false"},
        {"int_small", "42"},
        {"int_negative", "-12345"},
        {"int_large", "9223372036854775807"},
        {"float_simple", "3.14"},
        {"float_negative", "-0.001"},
        {"float_exponent", "1.23e45"},
        {"float_small_exp", "6.022e-23"},
        {"string_short", R"("hello")"},
        {"string_medium", R"("the quick brown fox jumps over the lazy dog")"},
        {"string_escaped", R"("line1\nline2\ttab\\slash\"quote")"},
        {"string_unicode", R"("\u0048\u0065\u006C\u006C\u006F")"},
        {"string_surrogate", R"("\uD83D\uDE00")"},
        {"string_empty", R"("")"},
        {"value_ws_null", "   \n\t  null"},
        {"value_ws_number", "  \r\n  42"},
        // arrays
        {"array_empty", "[]"},
        {"array_3_ints", "[1, 2, 3]"},
        {"array_10_ints", "[1,2,3,4,5,6,7,8,9,10]"},
        {"array_nested", "[[1,2],[3,4],[5,6]]"},
        {"array_strings", R"(["alpha","beta","gamma","delta"])"},
        {"array_mixed", R"([null, true, 42, "hi", 3.14])"},
        // objects
        {"object_empty", "{}"},
        {"object_1_key", R"({"a": 1})"},
        {"object_3_keys", R"({"a": 1, "b": 2, "c": 3})"},
        {"object_nested", R"({"a": {"b": {"c": 1}}})"},
        {"object_with_array", R"({"items": [1, 2, 3], "count": 3})"},
        // realistic
        {"realistic_small", R"({"id": 1, "name": "Alice", "active": true})"},
        {
            "realistic_medium",
            R"({"id": 42, "name": "Bob Smith", "email": "bob@example.com", "age": 31, "scores": [95, 87, 92, 88], "address": {"city": "Portland", "zip": "97201"}})"
        },
        {
            "realistic_array_of_obj",
            R"([{"id":1,"v":"a"},{"id":2,"v":"b"},{"id":3,"v":"c"},{"id":4,"v":"d"},{"id":5,"v":"e"}])"
        },
    };

    printf("bench_parse\n\n");
    bench::print_platform();
    printf("  %-28s %12s %12s %14s\n", "name", "iters", "ns/op", "ops/sec");
    printf("  %-28s %12s %12s %14s\n", "----", "-----", "-----", "-------");

    std::vector<BenchResult> results;

    for (auto &c: cases) {
        auto iters = calibrate(c.input);
        auto ns = measure(c.input, iters);
        auto ops = 1e9 / ns;

        printf("  %-28s %12lld %12.1f %14.0f\n",
               c.name, static_cast<long long>(iters), ns, ops);

        results.push_back({c.name, iters, ns, ops});
    }

    // --- write history.md: append mode for tracking over time ---

    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    char timestamp[64];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    std::string history_path = std::string(output_dir) + "/history.md";

    bool is_new = false;
    {
        std::ifstream check(history_path);
        is_new = !check.good();
    }

    std::ofstream history(history_path, std::ios::app);
    if (is_new)
        history << "# Strata parse benchmark history\n\n";

    history << "## " << timestamp << "\n\n";
    history << "**" << bench::get_cpu() << "** | "
            << bench::get_os() << " | "
            << bench::get_arch() << " | "
            << bench::get_compiler() << " | "
            << bench::get_simd() << "\n\n";
    history << "| name | ns/op | ops/sec |\n";
    history << "|------|------:|--------:|\n";
    for (auto &r: results) {
        char line[128];
        std::snprintf(line, sizeof(line), "| %s | %.1f | %.0f |\n",
                      r.name, r.ns_per_op, r.ops_per_sec);
        history << line;
    }
    history << "\n";

    printf("\n  results appended to %s\n", history_path.c_str());

    // --- write latest.md: human-readable snapshot ---

    std::string latest_path = std::string(output_dir) + "/latest.md";
    std::ofstream latest(latest_path);
    latest << "# Strata parse benchmark\n\n";
    latest << "_" << timestamp << "_\n\n";
    latest << "**" << bench::get_cpu() << "** | "
            << bench::get_os() << " | "
            << bench::get_arch() << " | "
            << bench::get_compiler() << " | "
            << bench::get_simd() << "\n\n";
    latest << "| name | ns/op | ops/sec |\n";
    latest << "|------|------:|--------:|\n";
    for (auto &r: results) {
        char line[128];
        std::snprintf(line, sizeof(line), "| %s | %.1f | %.0f |\n",
                      r.name, r.ns_per_op, r.ops_per_sec);
        latest << line;
    }

    printf("  snapshot written to %s\n", latest_path.c_str());
    return 0;
}

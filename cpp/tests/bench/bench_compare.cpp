// bench_compare.cpp — compare strata with other JSON parsers
//
// Compiles with strata only by default.
// To enable comparisons, vendor the libraries and compile with:
//   -DHAS_NLOHMANN -Ivendor/nlohmann
//   -DHAS_RAPIDJSON -Ivendor/rapidjson/include
//
// Vendor instructions:
//   mkdir -p cpp/vendor
//   nlohmann/json (single header):
//     curl -L <nlohmann releases url>/json.hpp -o cpp/vendor/nlohmann/json.hpp
//   rapidjson:
//     git clone --depth 1 https://github.com/Tencent/rapidjson.git cpp/vendor/rapidjson

#include "strata/parse_value.hpp"
#include "platform.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#ifdef HAS_NLOHMANN
#include <nlohmann/json.hpp>
#endif

#ifdef HAS_RAPIDJSON
#include <rapidjson/document.h>
#endif

#ifdef HAS_SIMDJSON
#include <simdjson.h>
#endif

using Clock = std::chrono::high_resolution_clock;

struct BenchEntry {
    const char* parser;
    const char* input_name;
    double      ns_per_op;
};

static const char* INPUTS[][2] = {
    // primitives
    {"null",                 "null"},
    {"bool_true",            "true"},
    {"bool_false",           "false"},
    {"int_small",            "42"},
    {"int_negative",         "-12345"},
    {"int_large",            "9223372036854775807"},
    {"float_simple",         "3.14"},
    {"float_negative",       "-0.001"},
    {"float_exponent",       "1.23e45"},
    {"float_small_exp",      "6.022e-23"},
    {"string_short",         R"("hello")"},
    {"string_medium",        R"("the quick brown fox jumps over the lazy dog")"},
    {"string_escaped",       R"("line1\nline2\ttab\\slash\"quote")"},
    {"string_unicode",       R"("\u0048\u0065\u006C\u006C\u006F")"},
    {"string_surrogate",     R"("\uD83D\uDE00")"},
    {"string_empty",         R"("")"},
    {"value_ws_null",        "   \n\t  null"},
    {"value_ws_number",      "  \r\n  42"},
    // arrays
    {"array_empty",          "[]"},
    {"array_3_ints",         "[1, 2, 3]"},
    {"array_10_ints",        "[1,2,3,4,5,6,7,8,9,10]"},
    {"array_nested",         "[[1,2],[3,4],[5,6]]"},
    {"array_strings",        R"(["alpha","beta","gamma","delta"])"},
    {"array_mixed",          R"([null, true, 42, "hi", 3.14])"},
    // objects
    {"object_empty",         "{}"},
    {"object_1_key",         R"({"a": 1})"},
    {"object_3_keys",        R"({"a": 1, "b": 2, "c": 3})"},
    {"object_nested",        R"({"a": {"b": {"c": 1}}})"},
    {"object_with_array",    R"({"items": [1, 2, 3], "count": 3})"},
    // realistic
    {"realistic_small",      R"({"id": 1, "name": "Alice", "active": true})"},
    {"realistic_medium",     R"({"id": 42, "name": "Bob Smith", "email": "bob@example.com", "age": 31, "scores": [95, 87, 92, 88], "address": {"city": "Portland", "zip": "97201"}})"},
    {"realistic_array_of_obj", R"([{"id":1,"v":"a"},{"id":2,"v":"b"},{"id":3,"v":"c"},{"id":4,"v":"d"},{"id":5,"v":"e"}])"},
};

static constexpr int NUM_INPUTS = sizeof(INPUTS) / sizeof(INPUTS[0]);

static int64_t calibrate(auto fn, double target_ms = 300.0) {
    int64_t n = 1000;
    while (true) {
        auto start = Clock::now();
        for (int64_t i = 0; i < n; ++i)
            fn();
        auto ms = std::chrono::duration<double, std::milli>(Clock::now() - start).count();
        if (ms >= target_ms) return n;
        n *= 2;
    }
}

static double measure(auto fn, int64_t iters) {
    // warmup
    for (int i = 0; i < 1000; ++i) fn();

    auto start = Clock::now();
    for (int64_t i = 0; i < iters; ++i)
        fn();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count();
    return static_cast<double>(ns) / static_cast<double>(iters);
}

int main(int argc, char* argv[]) {
    const char* output_dir = "docs/benchmarks/cpp_only";
    if (argc > 1) output_dir = argv[1];

    printf("bench_compare\n\n");
    bench::print_platform();

    std::vector<BenchEntry> results;

    for (int i = 0; i < NUM_INPUTS; ++i) {
        const char* name = INPUTS[i][0];
        const char* input = INPUTS[i][1];
        auto len = std::strlen(input);

        printf("  %-24s (%3zu bytes)\n", name, len);

        // --- strata ---
        {
            auto fn = [&]() { strata::parse<strata::JsonValue>(input, input + len); };
            auto iters = calibrate(fn);
            auto ns = measure(fn, iters);
            printf("    strata          %8.1f ns/op\n", ns);
            results.push_back({"strata", name, ns});
        }

#ifdef HAS_NLOHMANN
        // --- nlohmann/json ---
        {
            auto fn = [&]() { auto j = nlohmann::json::parse(input, input + len, nullptr, false); };
            auto iters = calibrate(fn);
            auto ns = measure(fn, iters);
            printf("    nlohmann        %8.1f ns/op\n", ns);
            results.push_back({"nlohmann", name, ns});
        }
#endif

#ifdef HAS_RAPIDJSON
        // --- rapidjson ---
        {
            auto fn = [&]() {
                rapidjson::Document doc;
                doc.Parse(input, len);
            };
            auto iters = calibrate(fn);
            auto ns = measure(fn, iters);
            printf("    rapidjson       %8.1f ns/op\n", ns);
            results.push_back({"rapidjson", name, ns});
        }
#endif

#ifdef HAS_SIMDJSON
        // --- simdjson (DOM) ---
        {
            auto padded = simdjson::padded_string(input, len);
            auto fn = [&]() {
                simdjson::dom::parser parser;
                auto doc = parser.parse(padded);
                (void)doc;
            };
            auto iters = calibrate(fn);
            auto ns = measure(fn, iters);
            printf("    simdjson         %8.1f ns/op\n", ns);
            results.push_back({"simdjson", name, ns});
        }
#endif

        printf("\n");
    }

    // --- summary ---
    printf("  === Summary (ns/op) ===\n\n");
    printf("  %-24s %10s", "input", "strata");
#ifdef HAS_NLOHMANN
    printf(" %10s %10s", "nlohmann", "speedup");
#endif
#ifdef HAS_RAPIDJSON
    printf(" %10s %10s", "rapidjson", "ratio");
#endif
#ifdef HAS_SIMDJSON
    printf(" %10s %10s", "simdjson", "ratio");
#endif
    printf("\n");

    printf("  %-24s %10s", "-----", "------");
#ifdef HAS_NLOHMANN
    printf(" %10s %10s", "--------", "-------");
#endif
#ifdef HAS_RAPIDJSON
    printf(" %10s %10s", "---------", "-----");
#endif
#ifdef HAS_SIMDJSON
    printf(" %10s %10s", "--------", "-----");
#endif
    printf("\n");

    for (int i = 0; i < NUM_INPUTS; ++i) {
        const char* name = INPUTS[i][0];
        double strata_ns = 0;
        for (auto& r : results)
            if (r.parser == std::string("strata") && r.input_name == std::string(name))
                strata_ns = r.ns_per_op;

        printf("  %-24s %8.1f ns", name, strata_ns);

#ifdef HAS_NLOHMANN
        for (auto& r : results) {
            if (r.parser == std::string("nlohmann") && r.input_name == std::string(name)) {
                printf(" %8.1f ns %8.1fx", r.ns_per_op, r.ns_per_op / strata_ns);
            }
        }
#endif

#ifdef HAS_RAPIDJSON
        for (auto& r : results) {
            if (r.parser == std::string("rapidjson") && r.input_name == std::string(name)) {
                printf(" %8.1f ns %8.1fx", r.ns_per_op, strata_ns / r.ns_per_op);
            }
        }
#endif

#ifdef HAS_SIMDJSON
        for (auto& r : results) {
            if (r.parser == std::string("simdjson") && r.input_name == std::string(name)) {
                printf(" %8.1f ns %8.1fx", r.ns_per_op, strata_ns / r.ns_per_op);
            }
        }
#endif

        printf("\n");
    }

    // --- write compare.md ---

    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    char timestamp[64];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    std::string md_path = std::string(output_dir) + "/compare.md";
    std::ofstream md(md_path);
    md << "# Strata parser comparison\n\n";
    md << "_" << timestamp << "_\n\n";

    md << "| input | strata |";
#ifdef HAS_NLOHMANN
    md << " nlohmann | speedup |";
#endif
#ifdef HAS_RAPIDJSON
    md << " rapidjson | ratio |";
#endif
#ifdef HAS_SIMDJSON
    md << " simdjson | ratio |";
#endif
    md << "\n";

    md << "|-------|-------:|";
#ifdef HAS_NLOHMANN
    md << "--------:|--------:|";
#endif
#ifdef HAS_RAPIDJSON
    md << "---------:|------:|";
#endif
#ifdef HAS_SIMDJSON
    md << "--------:|------:|";
#endif
    md << "\n";

    for (int i = 0; i < NUM_INPUTS; ++i) {
        const char* name = INPUTS[i][0];
        double strata_ns = 0;
        for (auto& r : results)
            if (r.parser == std::string("strata") && r.input_name == std::string(name))
                strata_ns = r.ns_per_op;

        char line[256];
        std::snprintf(line, sizeof(line), "| %s | %.1f ns |", name, strata_ns);
        md << line;

#ifdef HAS_NLOHMANN
        for (auto& r : results) {
            if (r.parser == std::string("nlohmann") && r.input_name == std::string(name)) {
                std::snprintf(line, sizeof(line), " %.1f ns | %.1fx |", r.ns_per_op, r.ns_per_op / strata_ns);
                md << line;
            }
        }
#endif

#ifdef HAS_RAPIDJSON
        for (auto& r : results) {
            if (r.parser == std::string("rapidjson") && r.input_name == std::string(name)) {
                std::snprintf(line, sizeof(line), " %.1f ns | %.2fx |", r.ns_per_op, strata_ns / r.ns_per_op);
                md << line;
            }
        }
#endif

#ifdef HAS_SIMDJSON
        for (auto& r : results) {
            if (r.parser == std::string("simdjson") && r.input_name == std::string(name)) {
                std::snprintf(line, sizeof(line), " %.1f ns | %.2fx |", r.ns_per_op, strata_ns / r.ns_per_op);
                md << line;
            }
        }
#endif

        md << "\n";
    }

    printf("\n  comparison written to %s\n", md_path.c_str());
    return 0;
}
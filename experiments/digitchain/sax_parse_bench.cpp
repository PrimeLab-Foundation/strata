/**
 * @file sax_parse_bench.cpp
 * @brief The parser alone, per number class, on whichever include tree -I names.
 *
 * Hypothesis under test: the digit-count-to-value chain in
 * `include/strata/util/fast_parse.hpp` — the leading count, the alignment
 * shift, the padding word and the three pair multiplies — is longer than the
 * widths a `wide_arrays` document actually carries need, and shortening it
 * shows up as parse time on the classes those widths belong to (9-digit
 * integers, six-decimal floats).
 *
 * The measurement is the SAX parser with a counting handler: no Python
 * objects, no allocation per element, so what moves between two builds of
 * this file is the parser's own arithmetic. Compiled against two include
 * trees — the baseline's and the variant's — it reads as a before/after
 * table on one machine in one job, which is what the arm64 runner is for
 * (the development machine is an M1 and the row under study is a Neoverse-N2).
 *
 * Never linked into production; `experiments/` is off the build
 * (docs/context/convention.md).
 *
 * usage: sax_parse_bench [--check] [document.json ...]
 *
 * `--check` parses each corpus once and prints its element count instead of
 * timing it: the shape check that belongs on a shared machine.
 */

#include "strata/json/json_parser_inline.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace {

/// Counts events and folds every value in, so nothing is optimised away.
struct CountingHandler {
    uint64_t elements = 0;
    uint64_t fold = 0;

    bool on_null() {
        ++elements;
        fold += 1;
        return true;
    }
    bool on_bool(bool value) {
        ++elements;
        fold += value ? 2u : 3u;
        return true;
    }
    bool on_int(int64_t value) {
        ++elements;
        fold += static_cast<uint64_t>(value);
        return true;
    }
    bool on_big_int(std::string_view span) {
        ++elements;
        fold += span.size();
        return true;
    }
    bool on_double(double value) {
        ++elements;
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(bits));
        fold += bits;
        return true;
    }
    bool on_string(std::string_view span) {
        ++elements;
        fold += span.size();
        return true;
    }
    bool on_key(std::string_view span) {
        fold += span.size();
        return true;
    }
    bool on_start_object() { return true; }
    bool on_end_object() { return true; }
    bool on_start_array() { return true; }
    bool on_end_array() { return true; }
};

uint64_t g_sink = 0;

/// Deterministic, so both builds parse byte-identical corpora.
struct Random {
    uint64_t state = 0x9E3779B97F4A7C15ULL;
    uint64_t next() {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    }
    uint64_t below(uint64_t bound) { return next() % bound; }
    double uniform(double low, double high) {
        const double unit = static_cast<double>(next() >> 11) / 9007199254740992.0;
        return low + unit * (high - low);
    }
};

std::string list_of(size_t count, const char* (*make)(Random&, char*), Random& random) {
    std::string out = "[";
    char buffer[64];
    for (size_t index = 0; index < count; ++index) {
        if (index != 0)
            out.push_back(',');
        out += make(random, buffer);
    }
    out.push_back(']');
    return out;
}

const char* make_null(Random&, char* buffer) {
    std::strcpy(buffer, "null");
    return buffer;
}
const char* make_bool(Random& random, char* buffer) {
    std::strcpy(buffer, random.below(2) ? "true" : "false");
    return buffer;
}
const char* make_int_1(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%llu", static_cast<unsigned long long>(random.below(10)));
    return buffer;
}
const char* make_int_3(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%llu", static_cast<unsigned long long>(100 + random.below(900)));
    return buffer;
}
const char* make_int_4(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%llu", static_cast<unsigned long long>(1000 + random.below(9000)));
    return buffer;
}
const char* make_int_7(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%llu",
                  static_cast<unsigned long long>(1000000 + random.below(9000000)));
    return buffer;
}
/// wide_arrays' own shape: randint(-1e9, 1e9), so nine digits and either sign.
const char* make_int_wide(Random& random, char* buffer) {
    const long long value = static_cast<long long>(random.below(2000000001ULL)) - 1000000000LL;
    std::snprintf(buffer, 64, "%lld", value);
    return buffer;
}
const char* make_int_9_10(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%llu",
                  static_cast<unsigned long long>(100000000ULL + random.below(9900000000ULL)));
    return buffer;
}
const char* make_int_15(Random& random, char* buffer) {
    std::snprintf(
        buffer, 64, "%llu",
        static_cast<unsigned long long>(100000000000000ULL + random.below(899999999999999ULL)));
    return buffer;
}
/// wide_arrays' own shape: round(uniform(-1e3, 1e3), 6).
const char* make_float_6dp(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%.6f", random.uniform(-1000.0, 1000.0));
    return buffer;
}
const char* make_float_17(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "%.17g", random.uniform(0.0, 1.0));
    return buffer;
}
const char* make_string_4(Random& random, char* buffer) {
    std::snprintf(buffer, 64, "\"s%llu\"", static_cast<unsigned long long>(random.below(1000)));
    return buffer;
}

/// Median wall time of `rounds` windows of `reps` parses, in nanoseconds.
double time_parse(const std::string& text, size_t rounds, size_t reps) {
    std::vector<double> samples;
    samples.reserve(rounds);
    for (size_t round = 0; round < rounds; ++round) {
        const auto start = std::chrono::steady_clock::now();
        for (size_t rep = 0; rep < reps; ++rep) {
            CountingHandler handler;
            const strata::Status status =
                strata::parse_sax_inline(std::string_view(text), handler, /*validate_utf8=*/false);
            if (status != strata::Status::Ok) {
                std::printf("PARSE FAILED\n");
                std::exit(2);
            }
            g_sink += handler.fold + handler.elements;
        }
        const auto stop = std::chrono::steady_clock::now();
        samples.push_back(std::chrono::duration<double, std::nano>(stop - start).count() /
                          static_cast<double>(reps));
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

size_t element_count(const std::string& text) {
    CountingHandler handler;
    const strata::Status status =
        strata::parse_sax_inline(std::string_view(text), handler, /*validate_utf8=*/false);
    if (status != strata::Status::Ok)
        return 0;
    return static_cast<size_t>(handler.elements);
}

struct Corpus {
    const char* name;
    std::string text;
};

} // namespace

int main(int argc, char** argv) {
    constexpr size_t kElements = 2000;
    bool check_only = false;
    if (argc > 1 && std::strcmp(argv[1], "--check") == 0) {
        check_only = true;
        --argc;
        ++argv;
    }
    Random random;
    std::vector<Corpus> corpora;
    const struct {
        const char* name;
        const char* (*make)(Random&, char*);
    } classes[] = {
        {"nulls", make_null},
        {"bools", make_bool},
        {"ints 0-9", make_int_1},
        {"ints 100-999", make_int_3},
        {"ints 4dig", make_int_4},
        {"ints 7dig", make_int_7},
        {"ints wide_arrays", make_int_wide},
        {"ints 9-10dig", make_int_9_10},
        {"ints 15dig", make_int_15},
        {"floats 6dp", make_float_6dp},
        {"floats 17dig", make_float_17},
        {"strs 4ch", make_string_4},
    };
    for (const auto& entry : classes)
        corpora.push_back({entry.name, list_of(kElements, entry.make, random)});

    for (int index = 1; index < argc; ++index) {
        std::FILE* const file = std::fopen(argv[index], "rb");
        if (file == nullptr) {
            std::printf("cannot open %s\n", argv[index]);
            continue;
        }
        std::string text;
        char buffer[65536];
        size_t got;
        while ((got = std::fread(buffer, 1, sizeof(buffer), file)) > 0)
            text.append(buffer, got);
        std::fclose(file);
        corpora.push_back({argv[index], std::move(text)});
    }

    std::printf("%-20s %12s %12s %10s\n", "corpus", "ns/parse", "ns/element", "elements");
    for (const auto& corpus : corpora) {
        const size_t elements = element_count(corpus.text);
        if (check_only) {
            std::printf("%-20s %12s %12s %10zu\n", corpus.name, "-", "-", elements);
            continue;
        }
        // Enough repetitions that a window is milliseconds, not microseconds.
        const size_t reps = corpus.text.size() > 200000 ? 4 : 40;
        const double best =
            std::min(time_parse(corpus.text, 31, reps), time_parse(corpus.text, 31, reps));
        std::printf("%-20s %12.1f %12.3f %10zu\n", corpus.name, best,
                    elements ? best / static_cast<double>(elements) : 0.0, elements);
    }
    std::printf("sink %llu\n", static_cast<unsigned long long>(g_sink));
    return 0;
}

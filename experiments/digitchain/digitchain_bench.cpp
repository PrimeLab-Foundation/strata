/**
 * @file digitchain_bench.cpp
 * @brief The digit-count-to-value chain, four formulations, on the two number
 *        widths a `wide_arrays` document is made of.
 *
 * Hypothesis: the chain that turns a word of ASCII digits into a number —
 * the leading count, the alignment shift, the '0' padding word and the three
 * pair multiplies — is longer than it has to be for nine-digit integers (an
 * eight-digit word plus a one-digit second word) and six-decimal floats (a
 * one-to-three-digit integer part and a six-digit fraction), the two shapes
 * `wide_arrays` carries. Four formulations are timed against each other:
 *
 *  shipped        the pre-change chain: pad the vacated low bytes with '0'
 *                 characters, then subtract '0' from all eight.
 *  subtract_first the variant: subtract '0' before the alignment shift, so
 *                 the shift's own zero bytes already carry the value '0'
 *                 stands for and the padding word disappears — and the one
 *                 subtraction left is the leading count's own.
 *  short32        subtract_first plus a 32-bit two-multiply reduction for
 *                 runs of four digits or fewer, chosen by a branch on the
 *                 count.
 *  pad_table      the pre-change chain with its two-shift padding word read
 *                 from a per-count table instead: one load in place of the
 *                 shift chain, the candidate that trades latency for a
 *                 memory operation.
 *
 * Two modes per shape: `latency`, where each number's value picks the next
 * number (the chain's depth is what is measured), and `throughput`, where
 * they are independent (its instruction count is). The real parser sits
 * between the two: the *position* is loop-carried, the value is not.
 *
 * Every formulation is checked against `shipped` on every corpus word before
 * anything is timed — a faster wrong answer is not a result.
 *
 * Never linked into production; `experiments/` is off the build
 * (docs/context/convention.md). Read beside experiments/digitchain/sax_parse_bench.cpp,
 * which measures the adopted change through the whole parser.
 *
 * usage: digitchain_bench [--check]
 *
 * `--check` runs the agreement pass and stops: the shape check that belongs
 * on a shared machine.
 */

#include <algorithm>
#include <bit>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

constexpr uint64_t kZeros = 0x3030303030303030ULL;
constexpr uint64_t kMask = 0x000000FF000000FFULL;
constexpr uint64_t kMul1 = 100 + (1000000ULL << 32);
constexpr uint64_t kMul2 = 1 + (10000ULL << 32);

constexpr uint64_t kRunPow10[9] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
};

[[nodiscard]] inline unsigned leading_digit_count(uint64_t chunk) noexcept {
    const uint64_t non_digit =
        ((chunk + 0x4646464646464646ULL) | (chunk - kZeros)) & 0x8080808080808080ULL;
    return static_cast<unsigned>(std::countr_zero(non_digit)) >> 3;
}

[[nodiscard]] inline uint32_t reduce(uint64_t values) noexcept {
    values = (values * 10) + (values >> 8);
    values = (((values & kMask) * kMul1) + (((values >> 16) & kMask) * kMul2)) >> 32;
    return static_cast<uint32_t>(values);
}

// --- the four formulations ------------------------------------------------

[[nodiscard]] inline uint32_t value_shipped(uint64_t chunk, unsigned count) noexcept {
    const unsigned shift = (8 - count) * 8;
    const uint64_t pad = (kZeros >> (count * 8 - 1)) >> 1;
    return reduce(((chunk << shift) | pad) - kZeros);
}

[[nodiscard]] inline uint32_t value_subtract_first(uint64_t chunk, unsigned count) noexcept {
    return reduce((chunk - kZeros) << ((8 - count) * 8));
}

[[nodiscard]] inline uint32_t value_short32(uint64_t chunk, unsigned count) noexcept {
    const uint64_t values = chunk - kZeros;
    if (count <= 4) {
        uint32_t narrow = static_cast<uint32_t>(values) << ((4 - count) * 8);
        narrow = (narrow * 10) + (narrow >> 8);
        return ((narrow & 0xFFu) * 100) + ((narrow >> 16) & 0xFFu);
    }
    return reduce(values << ((8 - count) * 8));
}

/// The padding words the shipped chain computes with two shifts, tabulated.
constexpr uint64_t kPad[9] = {
    0x3030303030303030ULL, 0x0030303030303030ULL, 0x0000303030303030ULL,
    0x0000003030303030ULL, 0x0000000030303030ULL, 0x0000000000303030ULL,
    0x0000000000003030ULL, 0x0000000000000030ULL, 0x0000000000000000ULL,
};

[[nodiscard]] inline uint32_t value_pad_table(uint64_t chunk, unsigned count) noexcept {
    return reduce(((chunk << ((8 - count) * 8)) | kPad[count]) - kZeros);
}

// --- the two shapes -------------------------------------------------------

/// An eight-digit word plus a second word's leading run: `wide_arrays`' ints.
template <uint32_t (*Value)(uint64_t, unsigned)> struct IntShape {
    [[nodiscard]] static inline uint64_t apply(uint64_t first, uint64_t second) noexcept {
        const unsigned tail_count = leading_digit_count(second);
        uint64_t value = Value(first, 8);
        if (tail_count != 0 && tail_count < 8)
            value = value * kRunPow10[tail_count] + Value(second, tail_count);
        return value;
    }
};

/// An integer run and a fraction run: `wide_arrays`' floats.
template <uint32_t (*Value)(uint64_t, unsigned)> struct FloatShape {
    [[nodiscard]] static inline uint64_t apply(uint64_t first, uint64_t second) noexcept {
        const unsigned count = leading_digit_count(first);
        const unsigned fraction_count = leading_digit_count(second);
        if (count == 0 || count >= 8 || fraction_count == 0 || fraction_count >= 8)
            return 0;
        return static_cast<uint64_t>(Value(first, count)) * kRunPow10[fraction_count] +
               Value(second, fraction_count);
    }
};

// --- corpora --------------------------------------------------------------

constexpr size_t kCount = 4096; // a power of two: the latency index is a mask

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

uint64_t word_at(const std::string& text, size_t offset) {
    char buffer[8];
    for (size_t index = 0; index < 8; ++index)
        buffer[index] = offset + index < text.size() ? text[offset + index] : ' ';
    uint64_t word;
    std::memcpy(&word, buffer, 8);
    return word;
}

struct Corpus {
    const char* name;
    std::vector<uint64_t> first;
    std::vector<uint64_t> second;
};

/// The dominant `wide_arrays` integer: nine digits, so an eight-digit word
/// and a one-digit tail. `low`..`high` widen it to the mixed-width corpus.
Corpus int_corpus(Random& random, const char* name, unsigned low_digits, unsigned high_digits) {
    Corpus corpus{name, {}, {}};
    char buffer[64];
    for (size_t index = 0; index < kCount; ++index) {
        const unsigned digits =
            low_digits + static_cast<unsigned>(random.below(high_digits - low_digits + 1));
        uint64_t value = 1;
        for (unsigned step = 1; step < digits; ++step)
            value *= 10;
        value += random.below(value * 9);
        std::snprintf(buffer, sizeof(buffer), "%llu,-1234567890",
                      static_cast<unsigned long long>(value));
        const std::string text(buffer);
        corpus.first.push_back(word_at(text, 0));
        corpus.second.push_back(word_at(text, 8));
    }
    return corpus;
}

/// `wide_arrays`' floats: round(uniform(-1e3, 1e3), 6), past the sign, with
/// the second word starting at the first fraction digit.
Corpus float_corpus(Random& random) {
    Corpus corpus{"floats 6dp", {}, {}};
    char buffer[64];
    for (size_t index = 0; index < kCount; ++index) {
        std::snprintf(buffer, sizeof(buffer), "%.6f,-1234567", random.uniform(0.0, 1000.0));
        const std::string text(buffer);
        const size_t point = text.find('.');
        corpus.first.push_back(word_at(text, 0));
        corpus.second.push_back(word_at(text, point + 1));
    }
    return corpus;
}

uint64_t g_sink = 0;

template <typename Shape> double time_latency(const Corpus& corpus, size_t rounds, size_t reps) {
    std::vector<double> samples;
    for (size_t round = 0; round < rounds; ++round) {
        uint64_t acc = 0;
        size_t pick = 0;
        const auto start = std::chrono::steady_clock::now();
        for (size_t rep = 0; rep < reps; ++rep) {
            for (size_t index = 0; index < kCount; ++index) {
                const uint64_t value = Shape::apply(corpus.first[pick], corpus.second[pick]);
                // The next word is chosen by this one's value: the chain's
                // depth, not the loop's width, is what the clock sees.
                pick = (index + 1 + static_cast<size_t>(value & 1)) & (kCount - 1);
                acc += value;
            }
        }
        const auto stop = std::chrono::steady_clock::now();
        g_sink += acc;
        samples.push_back(std::chrono::duration<double, std::nano>(stop - start).count() /
                          static_cast<double>(reps * kCount));
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

template <typename Shape> double time_throughput(const Corpus& corpus, size_t rounds, size_t reps) {
    std::vector<double> samples;
    for (size_t round = 0; round < rounds; ++round) {
        uint64_t acc = 0;
        const auto start = std::chrono::steady_clock::now();
        for (size_t rep = 0; rep < reps; ++rep) {
            for (size_t index = 0; index < kCount; ++index)
                acc += Shape::apply(corpus.first[index], corpus.second[index]);
        }
        const auto stop = std::chrono::steady_clock::now();
        g_sink += acc;
        samples.push_back(std::chrono::duration<double, std::nano>(stop - start).count() /
                          static_cast<double>(reps * kCount));
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

template <typename A, typename B> bool agrees(const Corpus& corpus) {
    for (size_t index = 0; index < kCount; ++index) {
        if (A::apply(corpus.first[index], corpus.second[index]) !=
            B::apply(corpus.first[index], corpus.second[index]))
            return false;
    }
    return true;
}

constexpr size_t kRounds = 31;
constexpr size_t kReps = 40;

template <template <uint32_t (*)(uint64_t, unsigned)> class Shape>
void report(const Corpus& corpus) {
    std::printf("%-18s %-11s %13.3f %13.3f %13.3f %13.3f\n", corpus.name, "latency",
                time_latency<Shape<value_shipped>>(corpus, kRounds, kReps),
                time_latency<Shape<value_subtract_first>>(corpus, kRounds, kReps),
                time_latency<Shape<value_short32>>(corpus, kRounds, kReps),
                time_latency<Shape<value_pad_table>>(corpus, kRounds, kReps));
    std::printf("%-18s %-11s %13.3f %13.3f %13.3f %13.3f\n", corpus.name, "throughput",
                time_throughput<Shape<value_shipped>>(corpus, kRounds, kReps),
                time_throughput<Shape<value_subtract_first>>(corpus, kRounds, kReps),
                time_throughput<Shape<value_short32>>(corpus, kRounds, kReps),
                time_throughput<Shape<value_pad_table>>(corpus, kRounds, kReps));
}

template <template <uint32_t (*)(uint64_t, unsigned)> class Shape>
bool all_agree(const Corpus& corpus) {
    return agrees<Shape<value_shipped>, Shape<value_subtract_first>>(corpus) &&
           agrees<Shape<value_shipped>, Shape<value_short32>>(corpus) &&
           agrees<Shape<value_shipped>, Shape<value_pad_table>>(corpus);
}

} // namespace

int main(int argc, char** argv) {
    const bool check_only = argc > 1 && std::strcmp(argv[1], "--check") == 0;
    Random random;
    const Corpus nine = int_corpus(random, "ints 9dig", 9, 9);
    const Corpus mixed = int_corpus(random, "ints 9-15dig", 9, 15);
    const Corpus floats = float_corpus(random);

    const bool ok =
        all_agree<IntShape>(nine) && all_agree<IntShape>(mixed) && all_agree<FloatShape>(floats);
    std::printf("formulations agree on every corpus word: %s\n", ok ? "yes" : "NO");
    if (!ok)
        return 1;
    if (check_only)
        return 0;

    std::printf("%-18s %-11s %13s %13s %13s %13s\n", "corpus", "mode", "shipped", "subtract_first",
                "short32", "pad_table");
    report<IntShape>(nine);
    report<IntShape>(mixed);
    report<FloatShape>(floats);
    std::printf("(nanoseconds per number)\nsink %llu\n", static_cast<unsigned long long>(g_sink));
    return 0;
}

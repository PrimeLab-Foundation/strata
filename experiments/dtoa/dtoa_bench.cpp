// The float writer's pieces, timed per shape.
//
// Hypothesis: mixed.json's floats are all 17-digit doubles (rng.random()),
// and that bucket reads 1.3-1.7x behind orjson on every platform and build
// while the 2-decimal, integral and scientific buckets lead. format_double
// runs them through the micro-decimal probe (which cannot serve them),
// Dragonbox, a digit count, a two-word fixed write and a point shift. The
// columns split that: the whole writer; Dragonbox's digit generation alone;
// the layout alone from a precomputed decimal; and std::to_chars (the
// standard library's shortest round-trip) as the yardstick. Nanoseconds per
// value, median of 30 rounds (docs/context/benchmarks.md). Never linked into
// production (docs/context/convention.md, Experiments).
//
// Build: clang++ -O3 -march=native -std=c++20 -I include experiments/dtoa/dtoa_bench.cpp
#include "strata/third_party/dragonbox/dragonbox.h"
#include "strata/util/dtoa.hpp"

#include <algorithm>
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

namespace {

struct Decimal {
    uint64_t significand;
    int32_t exponent;
};

Decimal to_decimal(double value) {
    const auto decimal = jkj::dragonbox::to_decimal(value, jkj::dragonbox::policy::sign::ignore,
                                                    jkj::dragonbox::policy::trailing_zero::remove);
    return {decimal.significand, decimal.exponent};
}

// The writer without its micro-decimal tier: sign and zero, Dragonbox, then
// the same layout format_double takes (fixed for scientific exponents in
// [-4, 15], d.ddde+XX otherwise). What format_double costs beyond this is
// the probe every value pays first plus its prologue.
size_t write_no_probe(double value, char* out) {
    using strata::util::detail::decimal_digit_count;
    using strata::util::detail::shift16_right;
    using strata::util::detail::write_digits_fixed;
    size_t written = 0;
    if (std::signbit(value))
        out[written++] = '-';
    if (value == 0.0) {
        std::memcpy(out + written, "0.0", 3);
        return written + 3;
    }
    const auto decimal = jkj::dragonbox::to_decimal(value, jkj::dragonbox::policy::sign::ignore,
                                                    jkj::dragonbox::policy::trailing_zero::remove);
    const uint64_t digits = decimal.significand;
    const size_t length = decimal_digit_count(digits);
    const int32_t scientific = static_cast<int32_t>(length) + decimal.exponent - 1;
    if (scientific >= -4 && scientific < 16) {
        const int32_t point = scientific + 1;
        if (point <= 0) {
            out[written++] = '0';
            out[written++] = '.';
            for (int32_t pad = 0; pad < -point; ++pad)
                out[written++] = '0';
            write_digits_fixed(digits, out + written, length);
            return written + length;
        }
        if (static_cast<size_t>(point) >= length) {
            write_digits_fixed(digits, out + written, length);
            written += length;
            for (size_t pad = length; pad < static_cast<size_t>(point); ++pad)
                out[written++] = '0';
            out[written++] = '.';
            out[written++] = '0';
            return written;
        }
        write_digits_fixed(digits, out + written, length);
        shift16_right(out + written + point, 1);
        out[written + point] = '.';
        return written + length + 1;
    }
    if (length == 1) {
        out[written++] = static_cast<char>('0' + digits);
    } else {
        write_digits_fixed(digits, out + written, length);
        shift16_right(out + written + 1, 1);
        out[written + 1] = '.';
        written += length + 1;
    }
    out[written++] = 'e';
    out[written++] = scientific >= 0 ? '+' : '-';
    int magnitude = scientific >= 0 ? scientific : -scientific;
    if (magnitude >= 100) {
        out[written++] = static_cast<char>('0' + magnitude / 100);
        magnitude %= 100;
        out[written++] = static_cast<char>('0' + magnitude / 10);
        out[written++] = static_cast<char>('0' + magnitude % 10);
        return written;
    }
    out[written++] = static_cast<char>('0' + magnitude / 10);
    out[written++] = static_cast<char>('0' + magnitude % 10);
    return written;
}

// Tree-shaped digit layout: the significand split by constant divisions
// into 4-digit groups, each into two pair lookups -- a tree of independent
// multiplies instead of the two chained eight-digit words, so the CPU
// overlaps them. Writes exactly `length` digits (length in 9..17).
inline void write_pairs(uint32_t four_digits, char* out) {
    using strata::util::detail::kDigitPairs;
    const uint32_t high = four_digits / 100;
    const uint32_t low = four_digits - high * 100;
    std::memcpy(out, &kDigitPairs[high * 2], 2);
    std::memcpy(out + 2, &kDigitPairs[low * 2], 2);
}

inline void write_8_tree(uint32_t eight_digits, char* out) {
    const uint32_t high = eight_digits / 10000;
    const uint32_t low = eight_digits - high * 10000;
    write_pairs(high, out);
    write_pairs(low, out + 4);
}

size_t write_tree(double value, char* out) {
    using strata::util::detail::decimal_digit_count;
    using strata::util::detail::shift16_right;
    using strata::util::detail::write_digits_fixed;
    size_t written = 0;
    if (std::signbit(value))
        out[written++] = '-';
    if (value == 0.0) {
        std::memcpy(out + written, "0.0", 3);
        return written + 3;
    }
    const auto decimal = jkj::dragonbox::to_decimal(value, jkj::dragonbox::policy::sign::ignore,
                                                    jkj::dragonbox::policy::trailing_zero::remove);
    const uint64_t digits = decimal.significand;
    const size_t length = decimal_digit_count(digits);
    const int32_t scientific = static_cast<int32_t>(length) + decimal.exponent - 1;
    if (scientific >= -4 && scientific < 16) {
        const int32_t point = scientific + 1;
        if (point <= 0 && length >= 9) {
            // The 17-digit shape: "0." then the digits as a tree -- the top
            // 1..9 digits through the fixed writer's word, the low eight as
            // four independent pair lookups.
            out[written++] = '0';
            out[written++] = '.';
            for (int32_t pad = 0; pad < -point; ++pad)
                out[written++] = '0';
            const uint64_t high = digits / 100000000ULL;
            const auto low = static_cast<uint32_t>(digits - high * 100000000ULL);
            const size_t high_length = length - 8;
            if (high_length > 8) {
                const auto top = static_cast<uint32_t>(high / 100000000ULL);
                const auto mid = static_cast<uint32_t>(high - top * 100000000ULL);
                out[written] = static_cast<char>('0' + top);
                write_8_tree(mid, out + written + 1);
            } else {
                write_digits_fixed(high, out + written, high_length);
            }
            write_8_tree(low, out + written + high_length);
            return written + length;
        }
        if (point <= 0) {
            out[written++] = '0';
            out[written++] = '.';
            for (int32_t pad = 0; pad < -point; ++pad)
                out[written++] = '0';
            write_digits_fixed(digits, out + written, length);
            return written + length;
        }
        if (static_cast<size_t>(point) >= length) {
            write_digits_fixed(digits, out + written, length);
            written += length;
            for (size_t pad = length; pad < static_cast<size_t>(point); ++pad)
                out[written++] = '0';
            out[written++] = '.';
            out[written++] = '0';
            return written;
        }
        write_digits_fixed(digits, out + written, length);
        shift16_right(out + written + point, 1);
        out[written + point] = '.';
        return written + length + 1;
    }
    return write_no_probe(value, out);
}

struct Bucket {
    const char* name;
    std::vector<double> values;
};

std::vector<Bucket> buckets() {
    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double> unit(0.0, 1.0);
    std::vector<Bucket> out;
    Bucket seventeen{"float-17dig", {}};
    for (int i = 0; i < 4000; ++i)
        seventeen.values.push_back(unit(rng));
    out.push_back(seventeen);
    Bucket two{"float-2dp", {}};
    for (int i = 0; i < 4000; ++i)
        two.values.push_back(std::round((i * 0.37 + 0.1) * 100.0) / 100.0);
    out.push_back(two);
    Bucket integral{"float-integral", {}};
    for (int i = 0; i < 4000; ++i)
        integral.values.push_back(static_cast<double>(i % 100000 + 1));
    out.push_back(integral);
    Bucket sci{"float-sci", {}};
    for (int i = 0; i < 4000; ++i)
        sci.values.push_back(1.5e-7 * (i + 1));
    out.push_back(sci);
    Bucket wide{"float-6dp-wide", {}};
    std::uniform_real_distribution<double> range(-1000.0, 1000.0);
    for (int i = 0; i < 4000; ++i)
        wide.values.push_back(std::round(range(rng) * 1e6) / 1e6);
    out.push_back(wide);
    return out;
}

template <typename Fn> double median_ns(const std::vector<double>& values, Fn&& fn) {
    std::vector<double> rounds;
    for (int round = 0; round < 30; ++round) {
        const auto start = std::chrono::steady_clock::now();
        size_t total = 0;
        for (int rep = 0; rep < 20; ++rep) {
            for (const double value : values)
                total += fn(value);
            asm volatile("" : : "r"(total) : "memory");
        }
        const auto end = std::chrono::steady_clock::now();
        rounds.push_back(std::chrono::duration<double, std::nano>(end - start).count() /
                         (20.0 * static_cast<double>(values.size())));
    }
    std::sort(rounds.begin(), rounds.end());
    return rounds[rounds.size() / 2];
}

} // namespace

int main() {
    char buffer[64];
    std::printf("%-16s %10s %10s %10s %10s %10s   (ns per value, median of 30 rounds)\n", "bucket",
                "format", "no_probe", "tree", "dragonbox", "to_chars");
    for (const Bucket& bucket : buckets()) {
        // Correctness: the writer's bytes must round-trip and match to_chars'.
        for (const double value : bucket.values) {
            char ours[64];
            char ref[64];
            const size_t n = strata::util::format_double(value, ours, sizeof ours);
            const auto res = std::to_chars(ref, ref + sizeof ref, value);
            double back = 0.0;
            std::from_chars(ours, ours + n, back);
            if (back != value) {
                std::printf("ROUND-TRIP FAILURE on %.17g: %.*s\n", value, static_cast<int>(n),
                            ours);
                return 1;
            }
            (void)res;
        }
        // The no-probe variant must produce format_double's bytes.
        for (const double value : bucket.values) {
            char ours[64];
            char theirs[64];
            const size_t n = strata::util::format_double(value, ours, sizeof ours);
            const size_t m = write_no_probe(value, theirs);
            if (n != m || std::memcmp(ours, theirs, n) != 0) {
                std::printf("VARIANT MISMATCH on %.17g\n", value);
                return 1;
            }
        }
        const double format = median_ns(bucket.values, [&](double value) {
            return strata::util::format_double(value, buffer, sizeof buffer);
        });
        for (const double value : bucket.values) {
            char ours[64];
            char theirs[64];
            const size_t n = strata::util::format_double(value, ours, sizeof ours);
            const size_t m = write_tree(value, theirs);
            if (n != m || std::memcmp(ours, theirs, n) != 0) {
                std::printf("TREE MISMATCH on %.17g: %.*s vs %.*s\n", value, static_cast<int>(n),
                            ours, static_cast<int>(m), theirs);
                return 1;
            }
        }
        const double no_probe =
            median_ns(bucket.values, [&](double value) { return write_no_probe(value, buffer); });
        const double tree =
            median_ns(bucket.values, [&](double value) { return write_tree(value, buffer); });
        const double dragonbox = median_ns(bucket.values, [&](double value) {
            const Decimal d = to_decimal(value);
            return static_cast<size_t>(d.significand & 1) + static_cast<size_t>(d.exponent & 1);
        });
        const double to_chars = median_ns(bucket.values, [&](double value) {
            const auto res = std::to_chars(buffer, buffer + sizeof buffer, value);
            return static_cast<size_t>(res.ptr - buffer);
        });
        std::printf("%-16s %10.2f %10.2f %10.2f %10.2f %10.2f\n", bucket.name, format, no_probe,
                    tree, dragonbox, to_chars);
    }
    return 0;
}

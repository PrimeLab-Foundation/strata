// Integer writer variants, timed per width bucket.
//
// Hypothesis: the x86 legs read the serializer's int paths 1.27x behind
// orjson under both compilers while arm64 reads 0.96x, and `format_int64`
// is a digit count plus a backwards divmod-100 pair loop -- a loop-carried
// 64-bit multiply chain on x86. The variants below keep the digit count and
// replace the loop: a 32-bit chain where the value allows it (A), one SWAR
// eight-digit word per group (B), and a straight-line pair split by digit
// count (C). Each bucket is 4000 values of one width, the shape
// benchmarks/decompose_dumps_mixed.py measures through Python; the number
// printed is nanoseconds per value, the median of 15 rounds. Read on the
// Linux profile leg (clang, the extension's compiler there) and, where the
// job builds it, under MSVC.
//
// Build: clang++ -O3 -march=native -std=c++20 -I include experiments/itoa/itoa_bench.cpp
// Never linked into production (docs/context/convention.md, Experiments).
#include "strata/util/dtoa.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

using strata::util::detail::decimal_digit_count;
using strata::util::detail::eight_digits_word;
using strata::util::detail::kDigitPairs;
using strata::util::detail::store_digit_word;

// Current: digit count + backwards divmod-100 pairs (strata::util::format_int64).
size_t write_current(int64_t value, char* out) noexcept {
    return strata::util::format_int64(value, out);
}

// A: the same loop on 32-bit arithmetic when the magnitude allows it.
size_t write_narrow(int64_t value, char* out) noexcept {
    auto magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude;
    }
    const size_t digits = decimal_digit_count(magnitude);
    char* cursor = out + written + digits;
    if (magnitude < 1000000000ULL) {
        auto rest = static_cast<uint32_t>(magnitude);
        while (rest >= 100) {
            const uint32_t pair = (rest % 100) * 2;
            rest /= 100;
            *--cursor = kDigitPairs[pair + 1];
            *--cursor = kDigitPairs[pair];
        }
        if (rest >= 10) {
            const uint32_t pair = rest * 2;
            *--cursor = kDigitPairs[pair + 1];
            *--cursor = kDigitPairs[pair];
        } else {
            *--cursor = static_cast<char>('0' + rest);
        }
        return written + digits;
    }
    (void)strata::util::detail::fill_u64_backwards(magnitude, out + written + digits);
    return written + digits;
}

// B: eight-digit SWAR words, the low group stored whole and the high group
// (if any) shifted so its leading zeros fall off; one multiply chain per
// group, no loop-carried division.
size_t write_words(int64_t value, char* out) noexcept {
    auto magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude;
    }
    const size_t digits = decimal_digit_count(magnitude);
    char* cursor = out + written;
    if (digits <= 8) {
        const uint64_t word = eight_digits_word(static_cast<uint32_t>(magnitude));
        store_digit_word(cursor, word >> ((8 - digits) * 8));
        return written + digits;
    }
    const uint64_t high = magnitude / 100000000ULL;
    const auto low = static_cast<uint32_t>(magnitude - high * 100000000ULL);
    if (digits <= 16) {
        const size_t high_digits = digits - 8;
        const uint64_t high_word = eight_digits_word(static_cast<uint32_t>(high));
        store_digit_word(cursor, high_word >> ((8 - high_digits) * 8));
        store_digit_word(cursor + high_digits, eight_digits_word(low));
        return written + digits;
    }
    const uint64_t top = high / 100000000ULL;
    const auto middle = static_cast<uint32_t>(high - top * 100000000ULL);
    const size_t top_digits = digits - 16;
    const uint64_t top_word = eight_digits_word(static_cast<uint32_t>(top));
    store_digit_word(cursor, top_word >> ((8 - top_digits) * 8));
    store_digit_word(cursor + top_digits, eight_digits_word(middle));
    store_digit_word(cursor + top_digits + 8, eight_digits_word(low));
    return written + digits;
}

// C: straight-line pairs selected by digit count -- no loop, every pair's
// divisor a compile-time constant (the jeaiii shape without the fixed-point
// trick), for values under 10^10; the loop for the rest.
size_t write_switch(int64_t value, char* out) noexcept {
    auto magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude;
    }
    char* cursor = out + written;
    if (magnitude >= 10000000000ULL) {
        const size_t digits = decimal_digit_count(magnitude);
        (void)strata::util::detail::fill_u64_backwards(magnitude, cursor + digits);
        return written + digits;
    }
    auto v = static_cast<uint32_t>(magnitude >= 4294967296ULL ? 0 : magnitude);
    const uint64_t wide = magnitude; // 10-digit values above uint32 use the wide path
    if (magnitude >= 4294967296ULL) {
        const uint32_t hi = static_cast<uint32_t>(wide / 100000000ULL);
        const uint32_t lo = static_cast<uint32_t>(wide - hi * 100000000ULL);
        std::memcpy(cursor, &kDigitPairs[hi * 2], 2);
        store_digit_word(cursor + 2, eight_digits_word(lo));
        return written + 10;
    }
    if (v < 100) {
        if (v < 10) {
            cursor[0] = static_cast<char>('0' + v);
            return written + 1;
        }
        std::memcpy(cursor, &kDigitPairs[v * 2], 2);
        return written + 2;
    }
    if (v < 10000) {
        const uint32_t hi = v / 100;
        const uint32_t lo = v - hi * 100;
        if (v < 1000) {
            cursor[0] = static_cast<char>('0' + hi);
            std::memcpy(cursor + 1, &kDigitPairs[lo * 2], 2);
            return written + 3;
        }
        std::memcpy(cursor, &kDigitPairs[hi * 2], 2);
        std::memcpy(cursor + 2, &kDigitPairs[lo * 2], 2);
        return written + 4;
    }
    if (v < 1000000) {
        const uint32_t hi = v / 10000;
        const uint32_t rest = v - hi * 10000;
        const uint32_t mid = rest / 100;
        const uint32_t lo = rest - mid * 100;
        if (v < 100000) {
            cursor[0] = static_cast<char>('0' + hi);
            std::memcpy(cursor + 1, &kDigitPairs[mid * 2], 2);
            std::memcpy(cursor + 3, &kDigitPairs[lo * 2], 2);
            return written + 5;
        }
        std::memcpy(cursor, &kDigitPairs[hi * 2], 2);
        std::memcpy(cursor + 2, &kDigitPairs[mid * 2], 2);
        std::memcpy(cursor + 4, &kDigitPairs[lo * 2], 2);
        return written + 6;
    }
    if (v < 100000000) {
        const uint32_t hi = v / 1000000;
        const uint32_t rest = v - hi * 1000000;
        const uint32_t a = rest / 10000;
        const uint32_t rest2 = rest - a * 10000;
        const uint32_t b = rest2 / 100;
        const uint32_t c = rest2 - b * 100;
        if (v < 10000000) {
            cursor[0] = static_cast<char>('0' + hi);
            std::memcpy(cursor + 1, &kDigitPairs[a * 2], 2);
            std::memcpy(cursor + 3, &kDigitPairs[b * 2], 2);
            std::memcpy(cursor + 5, &kDigitPairs[c * 2], 2);
            return written + 7;
        }
        std::memcpy(cursor, &kDigitPairs[hi * 2], 2);
        std::memcpy(cursor + 2, &kDigitPairs[a * 2], 2);
        std::memcpy(cursor + 4, &kDigitPairs[b * 2], 2);
        std::memcpy(cursor + 6, &kDigitPairs[c * 2], 2);
        return written + 8;
    }
    const uint32_t hi = v / 100000000;
    const uint32_t lo = v - hi * 100000000;
    if (v < 1000000000) {
        cursor[0] = static_cast<char>('0' + hi);
        store_digit_word(cursor + 1, eight_digits_word(lo));
        return written + 9;
    }
    std::memcpy(cursor, &kDigitPairs[hi * 2], 2);
    store_digit_word(cursor + 2, eight_digits_word(lo));
    return written + 10;
}

// E: two word tiers under nine digits -- a four-digit word (two pair lookups,
// one 4-byte store) for 1..4 digits and the eight-digit word for 5..8 -- so a
// value's digit count selects one of two straight-line shapes instead of a
// loop trip count; the loop above 10^8.
size_t write_tiers(int64_t value, char* out) noexcept {
    auto magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude;
    }
    const size_t digits = decimal_digit_count(magnitude);
    char* cursor = out + written;
    if (digits <= 4) {
        const auto v = static_cast<uint32_t>(magnitude);
        const uint32_t hi = v / 100;
        const uint32_t lo = v - hi * 100;
        uint32_t word;
        char pairs[4];
        std::memcpy(pairs, &kDigitPairs[hi * 2], 2);
        std::memcpy(pairs + 2, &kDigitPairs[lo * 2], 2);
        std::memcpy(&word, pairs, 4);
        word >>= (4 - digits) * 8;
        std::memcpy(cursor, &word, 4);
        return written + digits;
    }
    if (digits <= 8) {
        const uint64_t word = eight_digits_word(static_cast<uint32_t>(magnitude));
        store_digit_word(cursor, word >> ((8 - digits) * 8));
        return written + digits;
    }
    (void)strata::util::detail::fill_u64_backwards(magnitude, cursor + digits);
    return written + digits;
}

// F: the tiers with the nine-plus-digit test first, so the loop path pays
// one compare instead of the digit count plus two.
size_t write_tiers2(int64_t value, char* out) noexcept {
    auto magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude;
    }
    char* cursor = out + written;
    if (magnitude >= 100000000ULL) {
        const size_t digits = decimal_digit_count(magnitude);
        (void)strata::util::detail::fill_u64_backwards(magnitude, cursor + digits);
        return written + digits;
    }
    const auto v = static_cast<uint32_t>(magnitude);
    if (v < 10000) {
        const size_t digits = v >= 1000 ? 4 : v >= 100 ? 3 : v >= 10 ? 2 : 1;
        const uint32_t high = v / 100;
        const uint32_t low = v - high * 100;
        char pairs[4];
        std::memcpy(pairs, &kDigitPairs[high * 2], 2);
        std::memcpy(pairs + 2, &kDigitPairs[low * 2], 2);
        uint32_t word;
        std::memcpy(&word, pairs, 4);
        word >>= (4 - digits) * 8;
        std::memcpy(cursor, &word, 4);
        return written + digits;
    }
    const size_t digits = decimal_digit_count(v);
    const uint64_t word = eight_digits_word(v);
    store_digit_word(cursor, word >> ((8 - digits) * 8));
    return written + digits;
}

using Writer = size_t (*)(int64_t, char*);

struct Variant {
    const char* name;
    Writer write;
};

struct Bucket {
    const char* name;
    std::vector<int64_t> values;
};

std::vector<Bucket> buckets() {
    std::vector<Bucket> out;
    auto make = [&](const char* name, auto fn) {
        Bucket bucket{name, {}};
        for (int64_t i = 0; i < 4000; ++i)
            bucket.values.push_back(fn(i));
        out.push_back(std::move(bucket));
    };
    make("int-1-3dig", [](int64_t i) { return i % 1000; });
    make("int-4-6dig", [](int64_t i) { return 1000 + (i * 7919) % 999000; });
    make("int-7dig", [](int64_t i) { return 1000000 + (i * 7919) % 9000000; });
    make("int-8dig", [](int64_t i) { return 10000000 + (i * 7919) % 90000000; });
    make("int-9-10dig", [](int64_t i) { return 100000000 + (i * 104729) % 9900000000LL; });
    make("int-neg-7dig", [](int64_t i) { return -(1000000 + (i * 7919) % 9000000); });
    make("int-19dig", [](int64_t i) { return 1000000000000000000LL + i; });
    // Mixed widths, the shape real data has: mixed.json's ints are indexes,
    // randint(0, 10**6) values and small counts side by side, so the pair
    // loop's trip count changes from one element to the next and its exit
    // branch mispredicts -- which the same-width buckets above never show.
    make("int-mixed-1-7", [](int64_t i) {
        const int64_t h = (i * 2654435761LL) & 0x7fffffff;
        const int width = static_cast<int>(h % 7) + 1;
        int64_t limit = 1;
        for (int d = 0; d < width; ++d)
            limit *= 10;
        return (h / 7) % limit;
    });
    return out;
}

} // namespace

int main() {
    const Variant variants[] = {
        {"current", write_current}, {"narrow32", write_narrow}, {"words", write_words},
        {"switch", write_switch},   {"tiers", write_tiers},     {"tiers2", write_tiers2},
    };
    std::vector<char> sink(4000 * 24);
    // Correctness first: every variant must produce the current writer's bytes.
    for (const Bucket& bucket : buckets()) {
        for (const int64_t value : bucket.values) {
            char want[24];
            char got[24];
            const size_t want_len = write_current(value, want);
            for (const Variant& variant : variants) {
                std::memset(got, 0, sizeof got);
                const size_t got_len = variant.write(value, got);
                if (got_len != want_len || std::memcmp(got, want, want_len) != 0) {
                    std::printf("MISMATCH %s on %lld: %.*s vs %.*s\n", variant.name,
                                static_cast<long long>(value), static_cast<int>(got_len), got,
                                static_cast<int>(want_len), want);
                    return 1;
                }
            }
        }
    }
    std::printf("%-14s", "bucket");
    for (const Variant& variant : variants)
        std::printf(" %10s", variant.name);
    std::printf("   (ns per value, median of 15 rounds)\n");
    for (const Bucket& bucket : buckets()) {
        std::printf("%-14s", bucket.name);
        for (const Variant& variant : variants) {
            std::vector<double> rounds;
            for (int round = 0; round < 15; ++round) {
                const auto start = std::chrono::steady_clock::now();
                size_t total = 0;
                for (int rep = 0; rep < 50; ++rep) {
                    char* cursor = sink.data();
                    for (const int64_t value : bucket.values) {
                        cursor += variant.write(value, cursor);
                        *cursor++ = ',';
                    }
                    total += static_cast<size_t>(cursor - sink.data());
                    asm volatile("" : : "r"(sink.data()), "r"(total) : "memory");
                }
                const auto end = std::chrono::steady_clock::now();
                const double ns = std::chrono::duration<double, std::nano>(end - start).count();
                rounds.push_back(ns / (50.0 * static_cast<double>(bucket.values.size())));
            }
            std::sort(rounds.begin(), rounds.end());
            std::printf(" %10.2f", rounds[rounds.size() / 2]);
        }
        std::printf("\n");
    }
    return 0;
}

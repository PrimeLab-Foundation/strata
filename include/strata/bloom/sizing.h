#pragma once

/**
 * @file sizing.h
 * @brief Compile-time and runtime utilities for optimal Bloom filter sizing.
 *
 * Given n expected keys and target false-positive rate p:
 *   m (bits)   = -(n * ln(p)) / (ln(2))^2
 *   k (hashes) = (m / n) * ln(2)
 *
 * All sizes are rounded up to the nearest multiple of 256 for AVX2 alignment.
 *
 * Common presets:
 *   TinyFilter   — 10 keys, 1% FPR  → 256 bits,  3 hashes
 *   SmallFilter  — 50 keys, 1% FPR  → 512 bits,  3 hashes
 *   MediumFilter — 200 keys, 1% FPR → 2048 bits, 3 hashes
 *   LargeFilter  — 1000 keys, 1% FPR → 10240 bits, 3 hashes
 */

#include "strata/bloom/bloom_filter.h"

#include <cmath>
#include <cstddef>

namespace strata {
namespace bloom {

// ─── constexpr math helpers (C++20 std::log is not constexpr everywhere) ─────

namespace detail_sizing {

/// constexpr natural log approximation using the series expansion
/// ln(x) = 2 * sum_{k=0}^{N} (1/(2k+1)) * ((x-1)/(x+1))^(2k+1)
/// Accurate to ~12 digits for x in [0.01, 100].
constexpr double cx_log(double x) noexcept {
    // Use the identity: ln(x) = ln(x/2^k) + k*ln(2)
    // to bring x into [0.5, 2.0] range for better convergence.
    //
    // But for our use case (small integer n and p in [0.001, 0.1]),
    // we can use a direct approach with enough terms.
    if (x <= 0.0)
        return -1e18; // sentinel for invalid input
    if (x == 1.0)
        return 0.0;

    // Reduce to [0.5, 2] range
    double shifts = 0.0;
    double v = x;
    constexpr double kLn2 = 0.6931471805599453;
    while (v > 2.0) {
        v /= 2.0;
        shifts += 1.0;
    }
    while (v < 0.5) {
        v *= 2.0;
        shifts -= 1.0;
    }

    // Series: ln(v) = 2 * atanh((v-1)/(v+1))
    double t = (v - 1.0) / (v + 1.0);
    double t2 = t * t;
    double term = t;
    double sum = t;
    for (int i = 1; i <= 30; ++i) {
        term *= t2;
        sum += term / (2.0 * i + 1.0);
    }
    return 2.0 * sum + shifts * kLn2;
}

constexpr double kLn2 = 0.6931471805599453;
constexpr double kLn2Sq = kLn2 * kLn2; // (ln 2)^2 ≈ 0.4804530139

} // namespace detail_sizing

/**
 * Compute optimal number of bits for a Bloom filter.
 *
 * m = -(n * ln(p)) / (ln(2))^2
 *
 * @param num_keys           Expected number of keys to insert.
 * @param false_positive_rate Target FPR (e.g., 0.01 for 1%).
 * @return Optimal number of bits (not yet aligned).
 */
constexpr size_t optimal_bits(size_t num_keys, double false_positive_rate) noexcept {
    if (num_keys == 0)
        return 256; // minimum useful size
    double m = -(static_cast<double>(num_keys) * detail_sizing::cx_log(false_positive_rate)) /
               detail_sizing::kLn2Sq;
    return static_cast<size_t>(m + 0.5); // round to nearest
}

/**
 * Compute optimal number of hash functions.
 *
 * k = (m / n) * ln(2)
 *
 * @param num_bits Total bits in the filter.
 * @param num_keys Expected number of keys.
 * @return Optimal number of hash functions, clamped to [2, 8].
 */
constexpr size_t optimal_hashes(size_t num_bits, size_t num_keys) noexcept {
    if (num_keys == 0)
        return 3;
    double k =
        (static_cast<double>(num_bits) / static_cast<double>(num_keys)) * detail_sizing::kLn2;
    size_t ki = static_cast<size_t>(k + 0.5);
    if (ki < 2)
        ki = 2;
    if (ki > 8)
        ki = 8;
    return ki;
}

/// Round up to the nearest multiple of 256 (for AVX2 alignment).
constexpr size_t align_bits(size_t bits) noexcept { return (bits + 255) & ~size_t{255}; }

/**
 * Pre-computed filter configuration from expected key count and FPR.
 *
 * Usage:
 *   using MyFilter = bloom::Preset<50, 10>::type;  // 50 keys, 1% FPR
 *
 * @tparam ExpectedKeys       Number of expected keys.
 * @tparam FalsePositivePermille  FPR * 1000 (e.g., 10 = 1%, 1 = 0.1%).
 */
template <size_t ExpectedKeys, int FalsePositivePermille = 10> struct Preset {
    static constexpr double fpr_value = static_cast<double>(FalsePositivePermille) / 1000.0;
    static constexpr size_t raw_bits = optimal_bits(ExpectedKeys, fpr_value);
    static constexpr size_t bits = align_bits(raw_bits < 256 ? 256 : raw_bits);
    static constexpr size_t hashes = optimal_hashes(bits, ExpectedKeys);
    using type = BloomFilter<bits, hashes>;
};

// ─── Common presets as type aliases ──────────────────────────────────────────

/// 10 keys, 1% FPR → 256 bits, 3 hashes (single cache line)
using TinyFilter = Preset<10, 10>::type;

/// 50 keys, 1% FPR → 512 bits, 3 hashes
using SmallFilter = Preset<50, 10>::type;

/// 200 keys, 1% FPR → 2048 bits, 3 hashes (blocked, 4 cache lines)
using MediumFilter = Preset<200, 10>::type;

/// 1000 keys, 1% FPR → 10240 bits, 3 hashes (blocked, ~20 cache lines)
using LargeFilter = Preset<1000, 10>::type;

} // namespace bloom
} // namespace strata

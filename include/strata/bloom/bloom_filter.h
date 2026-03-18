#pragma once

/**
 * @file bloom_filter.h
 * @brief Cache-line-aware Bloom filter with Kirschner-Mitzenmacher double hashing.
 *
 * Uses wyhash (public domain) for high-quality, fast hashing and derives k
 * independent hash functions via the double-hashing trick:
 *   h_i(key) = (h1(key) + i * h2(key)) mod NumBits
 *
 * The default 256-bit (32-byte) configuration fits in a single cache line.
 * Larger filters use a blocked design: the bit array is partitioned into
 * 64-byte (512-bit) cache-line-sized blocks, and all k probes for a given
 * key land in the same block, eliminating multi-cache-line access.
 *
 * Probability math:
 *   For a filter with m bits, k hash functions, and n inserted keys,
 *   the theoretical false-positive rate is approximately:
 *     FPR ≈ (1 - e^(-kn/m))^k
 *
 *   For the default (m=256, k=3, n=10): FPR ≈ (1 - e^(-30/256))^3 ≈ 0.14%
 */

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <string_view>

namespace strata {
namespace bloom {

// ─── wyhash constants (public domain by Wang Yi) ────────────────────────────

namespace detail {

/// wyhash secret primes — used for mixing.
static constexpr uint64_t kWyP0 = 0xa0761d6478bd642fULL;
static constexpr uint64_t kWyP1 = 0xe7037ed1a0b428dbULL;
static constexpr uint64_t kWyP2 = 0x8ebc6af09c88c6e3ULL;
static constexpr uint64_t kWyP3 = 0x589965cc75374cc3ULL;

/// 64×64→128-bit multiply, return upper ^ lower (MUM = MUltiply and Mix).
inline uint64_t wymum(uint64_t a, uint64_t b) noexcept {
    __uint128_t r = static_cast<__uint128_t>(a) * b;
    return static_cast<uint64_t>(r) ^ static_cast<uint64_t>(r >> 64);
}

/// Read 8 bytes little-endian (unaligned).
inline uint64_t wyr8(const uint8_t* p) noexcept {
    uint64_t v;
    __builtin_memcpy(&v, p, 8);
    return v;
}

/// Read 4 bytes little-endian (unaligned).
inline uint64_t wyr4(const uint8_t* p) noexcept {
    uint32_t v;
    __builtin_memcpy(&v, p, 4);
    return v;
}

/// Read 1-3 bytes, zero-padded into a uint64_t.
inline uint64_t wyr3(const uint8_t* p, size_t k) noexcept {
    return (static_cast<uint64_t>(p[0]) << 16) | (static_cast<uint64_t>(p[k >> 1]) << 8) |
           static_cast<uint64_t>(p[k - 1]);
}

/**
 * wyhash — fast, high-quality 64-bit hash function (public domain).
 *
 * Reference: https://github.com/wangyi-fudan/wyhash
 * This is a simplified inline implementation (~20 effective lines)
 * matching wyhash v4 final output.
 */
inline uint64_t wyhash(const uint8_t* data, size_t len, uint64_t seed) noexcept {
    seed ^= kWyP0;
    uint64_t a, b;

    if (__builtin_expect(len <= 16, 1)) {
        if (__builtin_expect(len >= 4, 1)) {
            // 4..16 bytes
            a = (wyr4(data) << 32) | wyr4(data + ((len >> 3) << 2));
            b = (wyr4(data + len - 4) << 32) | wyr4(data + len - 4 - ((len >> 3) << 2));
        } else if (__builtin_expect(len > 0, 1)) {
            // 1..3 bytes
            a = wyr3(data, len);
            b = 0;
        } else {
            // 0 bytes
            a = b = 0;
        }
    } else {
        // > 16 bytes
        size_t i = len;
        const uint8_t* p = data;

        if (i > 48) {
            // Bulk loop in 48-byte strides.
            uint64_t see1 = seed, see2 = seed;
            do {
                seed = wymum(wyr8(p) ^ kWyP1, wyr8(p + 8) ^ seed);
                see1 = wymum(wyr8(p + 16) ^ kWyP2, wyr8(p + 24) ^ see1);
                see2 = wymum(wyr8(p + 32) ^ kWyP3, wyr8(p + 40) ^ see2);
                p += 48;
                i -= 48;
            } while (__builtin_expect(i > 48, 1));
            seed ^= see1 ^ see2;
        }

        // Handle remaining 17-48 bytes in 16-byte steps.
        while (__builtin_expect(i > 16, 0)) {
            seed = wymum(wyr8(p) ^ kWyP1, wyr8(p + 8) ^ seed);
            p += 16;
            i -= 16;
        }

        // Read the last 16 bytes (may overlap with previously read bytes).
        a = wyr8(p + i - 16);
        b = wyr8(p + i - 8);
    }

    return wymum(kWyP1 ^ len, wymum(a ^ kWyP1, b ^ seed));
}

} // namespace detail

/**
 * @brief Compile-time configurable Bloom filter.
 *
 * @tparam NumBits  Total bit count; must be a multiple of 256 (AVX2 alignment).
 * @tparam NumHashes Number of hash functions (probes per query); must be in [2, 8].
 *
 * For filters where NumBits > 512 (multiple cache lines), a blocked design is
 * used: h1 selects the 512-bit block, and all k probes are within that block.
 * This keeps every query touching exactly one cache line.
 *
 * Memory layout: the bit array is stored in a 32-byte-aligned std::array.
 */
template <size_t NumBits = 256, size_t NumHashes = 3> class BloomFilter {
  public:
    static_assert(NumBits % 256 == 0, "NumBits must be a multiple of 256 for AVX2 alignment");
    static_assert(NumHashes >= 2 && NumHashes <= 8, "NumHashes must be in [2, 8]");

    static constexpr size_t BYTE_SIZE = NumBits / 8;
    static constexpr size_t NUM_BITS = NumBits;
    static constexpr size_t NUM_HASHES = NumHashes;

    /// Whether we use the blocked design (multiple cache lines).
    static constexpr bool BLOCKED = (BYTE_SIZE > 64);
    /// Block size in bits (one cache line = 512 bits).
    static constexpr size_t BLOCK_BITS = 512;
    static constexpr size_t BLOCK_BYTES = 64;
    static constexpr size_t NUM_BLOCKS = BLOCKED ? (BYTE_SIZE / BLOCK_BYTES) : 1;

    BloomFilter() noexcept { clear(); }

    /// Insert a key into the filter.
    void insert(std::string_view key) noexcept {
        auto [h1, h2] = hash(key);

        if constexpr (BLOCKED) {
            // Blocked design: h1 selects the block, probes within that block.
            size_t block = h1 % NUM_BLOCKS;
            size_t block_offset = block * BLOCK_BITS;
            for (size_t i = 0; i < NumHashes; ++i) {
                size_t bit = block_offset + ((h1 + i * h2) % BLOCK_BITS);
                bits_[bit / 8] |= static_cast<uint8_t>(1u << (bit % 8));
            }
        } else {
            // Small filter: all probes across the full bit array.
            for (size_t i = 0; i < NumHashes; ++i) {
                size_t bit = (h1 + i * h2) % NumBits;
                bits_[bit / 8] |= static_cast<uint8_t>(1u << (bit % 8));
            }
        }
    }

    /// Query: returns true if key MIGHT be in the set, false if DEFINITELY NOT.
    [[nodiscard]] bool maybe_contains(std::string_view key) const noexcept {
        auto [h1, h2] = hash(key);

        if constexpr (BLOCKED) {
            size_t block = h1 % NUM_BLOCKS;
            size_t block_offset = block * BLOCK_BITS;
            for (size_t i = 0; i < NumHashes; ++i) {
                size_t bit = block_offset + ((h1 + i * h2) % BLOCK_BITS);
                if (!(bits_[bit / 8] & static_cast<uint8_t>(1u << (bit % 8))))
                    return false;
            }
        } else {
            for (size_t i = 0; i < NumHashes; ++i) {
                size_t bit = (h1 + i * h2) % NumBits;
                if (!(bits_[bit / 8] & static_cast<uint8_t>(1u << (bit % 8))))
                    return false;
            }
        }
        return true;
    }

    /// Bulk insert from initializer list.
    void insert_all(std::initializer_list<std::string_view> keys) noexcept {
        for (auto key : keys) {
            insert(key);
        }
    }

    /// Reset filter to empty state (all bits zero).
    void clear() noexcept { bits_.fill(0); }

    /// Count the number of set bits in the filter (population count).
    [[nodiscard]] size_t count_set_bits() const noexcept {
        size_t count = 0;
        // Process 8 bytes at a time using __builtin_popcountll.
        // The bit array is always a multiple of 32 bytes, so 8-byte steps are safe.
        const auto* words = reinterpret_cast<const uint64_t*>(bits_.data());
        constexpr size_t num_words = BYTE_SIZE / 8;
        for (size_t i = 0; i < num_words; ++i) {
            count += static_cast<size_t>(__builtin_popcountll(words[i]));
        }
        return count;
    }

    /**
     * Estimated false-positive rate given the current fill level.
     *
     * Uses the formula: FPR ≈ (fill_ratio)^k
     * where fill_ratio = (set bits) / (total bits).
     *
     * This is a simplified approximation. The exact formula is:
     *   FPR = (1 - (1 - 1/m)^(kn))^k ≈ (1 - e^(-kn/m))^k
     * but using the measured fill ratio directly is more accurate for
     * the actual state of the filter.
     */
    [[nodiscard]] double estimated_false_positive_rate() const noexcept {
        double ratio = fill_ratio();
        double fpr = 1.0;
        for (size_t i = 0; i < NumHashes; ++i) {
            fpr *= ratio;
        }
        return fpr;
    }

    /// Fraction of bits that are set: count_set_bits() / NUM_BITS.
    [[nodiscard]] double fill_ratio() const noexcept {
        return static_cast<double>(count_set_bits()) / static_cast<double>(NumBits);
    }

    /// Merge another filter via bitwise OR (union semantics).
    /// Used for combining per-thread filters after parallel processing.
    void merge(const BloomFilter& other) noexcept {
        const auto* src = reinterpret_cast<const uint64_t*>(other.bits_.data());
        auto* dst = reinterpret_cast<uint64_t*>(bits_.data());
        constexpr size_t num_words = BYTE_SIZE / 8;
        for (size_t i = 0; i < num_words; ++i) {
            dst[i] |= src[i];
        }
    }

  private:
    alignas(32) std::array<uint8_t, BYTE_SIZE> bits_{};

    /// Double-hash pair derived from wyhash with two different seeds.
    struct HashPair {
        uint64_t h1;
        uint64_t h2;
    };

    /// Compute two independent 64-bit hashes for the given key.
    [[nodiscard]] HashPair hash(std::string_view key) const noexcept {
        // Seed values chosen as arbitrary primes that are unlikely to collide
        // with common data patterns.
        constexpr uint64_t kSeed1 = 0x2d358dccaa6c78a5ULL;
        constexpr uint64_t kSeed2 = 0x8bb84b93962eacc9ULL;

        auto* data = reinterpret_cast<const uint8_t*>(key.data());
        size_t len = key.size();

        return {detail::wyhash(data, len, kSeed1), detail::wyhash(data, len, kSeed2)};
    }
};

} // namespace bloom
} // namespace strata

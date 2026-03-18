#pragma once

/**
 * @file dedup_filter.h
 * @brief Duplicate key detector using stacked Bloom filters.
 *
 * Detects duplicate keys within a single JSON object without maintaining
 * a hash set.  Uses a stack of TinyFilter (256-bit) Bloom filters to
 * support nested objects — each nesting level gets its own filter.
 *
 * Memory budget: each level is exactly 32 bytes (256-bit filter) + 2 bytes
 * (key count), so the entire stack for depth 32 is 32 × 34 = 1088 bytes.
 *
 * On a positive (possible duplicate), the caller should do an exact check
 * against the already-parsed keys for that object.  The false-positive rate
 * for TinyFilter with ≤10 keys is ~0.14%, so exact checks are rare.
 */

#include "strata/bloom/sizing.h"

#include <array>
#include <cstdint>

namespace strata {
namespace bloom {

class DedupFilter {
  public:
    DedupFilter() noexcept { reset(); }

    /**
     * Check if key was already seen in the current object, then insert it.
     *
     * @return true if key is POSSIBLY a duplicate (Bloom filter positive).
     *         On true, caller should do exact check against already-parsed keys.
     */
    [[nodiscard]] bool check_and_insert(std::string_view key) noexcept {
        ++total_checks_;
        auto& level = stack_[depth_];

        bool maybe_dup = level.filter.maybe_contains(key);
        level.filter.insert(key);
        ++level.key_count;

        if (maybe_dup) {
            ++suspects_;
        }
        return maybe_dup;
    }

    /// Reset for a new document (clears all levels, returns to depth 0).
    void reset() noexcept {
        depth_ = 0;
        total_checks_ = 0;
        suspects_ = 0;
        stack_[0].filter.clear();
        stack_[0].key_count = 0;
    }

    /// Entering a nested object: push a new filter level.
    void push() noexcept {
        if (depth_ + 1 < MAX_DEPTH) {
            ++depth_;
            stack_[depth_].filter.clear();
            stack_[depth_].key_count = 0;
        }
    }

    /// Leaving a nested object: pop back to the parent level.
    void pop() noexcept {
        if (depth_ > 0) {
            --depth_;
        }
    }

    [[nodiscard]] uint64_t total_checks() const noexcept { return total_checks_; }
    [[nodiscard]] uint64_t duplicate_suspects() const noexcept { return suspects_; }
    [[nodiscard]] uint8_t current_depth() const noexcept { return depth_; }

  private:
    static constexpr size_t MAX_DEPTH = 32;

    struct Level {
        TinyFilter filter;      ///< 256-bit Bloom filter (32 bytes)
        uint16_t key_count = 0; ///< Number of keys inserted at this level
    };

    std::array<Level, MAX_DEPTH> stack_;
    uint8_t depth_ = 0;
    uint64_t total_checks_ = 0;
    uint64_t suspects_ = 0;
};

} // namespace bloom
} // namespace strata

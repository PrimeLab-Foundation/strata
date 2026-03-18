#pragma once

/**
 * @file key_filter.h
 * @brief Selective key extraction filter for the parser hot path.
 *
 * Wraps a Bloom filter with an exact-match fallback to provide the
 * "should I parse this value?" decision.  The typical workflow:
 *
 *   1. User specifies desired keys (e.g., from a JSONPath query).
 *   2. KeyFilter builds a Bloom filter over those keys.
 *   3. For each key encountered during parsing:
 *        a) Bloom filter rejects ~99% of unwanted keys instantly (no string cmp).
 *        b) For the ~1% false positives, binary search on the sorted key set
 *           provides the definitive answer.
 *
 * The critical insight: when parsing a JSON object with 500 keys but the user
 * only wants 5 specific fields, the Bloom filter rejects ~99% of keys without
 * any string comparison.  For the ~1% false positives, we fall through to
 * exact match.  The value-skipping logic (advancing past a value we don't
 * need) uses the structural index to jump directly to the next key.
 */

#include "strata/bloom/sizing.h"

#include <algorithm>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace strata {
namespace bloom {

class KeyFilter {
  public:
    /// Action the parser should take for a given key.
    enum class Action : uint8_t {
        PARSE,    ///< Key is wanted — parse the value.
        SKIP,     ///< Key is definitely not wanted — skip the value.
        UNCERTAIN ///< Bloom filter says maybe — do exact check.
    };

    /// Build from a list of desired keys (JSONPath field names).
    explicit KeyFilter(std::span<const std::string_view> desired_keys);

    /// Build from JSONPath-like expressions: "$.user.name", "$.items[*].price".
    /// Extracts leaf field names from each path.
    static KeyFilter from_paths(std::span<const std::string_view> paths);

    /**
     * Fast Bloom-only check: returns SKIP if key is definitely not wanted,
     * UNCERTAIN if the Bloom filter returned positive (needs exact check),
     * or PARSE if the filter is empty (accept all).
     */
    [[nodiscard]] Action check(std::string_view key) const noexcept;

    /// For UNCERTAIN results, do the exact string comparison via binary search.
    [[nodiscard]] bool exact_match(std::string_view key) const noexcept;

    /**
     * Combined check: Bloom filter first, then exact match on positive.
     * This is the hot-path function the parser calls.
     *
     * @return true if the key is in the desired set; false otherwise.
     */
    [[nodiscard]] bool should_parse(std::string_view key) const noexcept;

    /// Runtime statistics for profiling filter effectiveness.
    struct Stats {
        uint64_t total_checks = 0;    ///< Total calls to should_parse / check
        uint64_t bloom_rejects = 0;   ///< Definite negatives (saved work)
        uint64_t bloom_positives = 0; ///< Passed to exact match
        uint64_t exact_matches = 0;   ///< True positives
        uint64_t false_positives = 0; ///< Bloom said yes, exact said no

        [[nodiscard]] double reject_rate() const noexcept {
            return total_checks > 0
                       ? static_cast<double>(bloom_rejects) / static_cast<double>(total_checks)
                       : 0.0;
        }

        [[nodiscard]] double false_positive_rate() const noexcept {
            return bloom_positives > 0
                       ? static_cast<double>(false_positives) / static_cast<double>(bloom_positives)
                       : 0.0;
        }
    };

    [[nodiscard]] Stats stats() const noexcept { return stats_; }
    void reset_stats() noexcept { stats_ = {}; }

  private:
    /// Bloom filter for fast rejection — sized for typical use (≤50 desired keys).
    SmallFilter bloom_;

    /// Sorted list of desired keys for binary-search exact match.
    std::vector<std::string> desired_keys_;

    /// Mutable stats counter (logically const — updated during const queries).
    mutable Stats stats_;
};

} // namespace bloom
} // namespace strata

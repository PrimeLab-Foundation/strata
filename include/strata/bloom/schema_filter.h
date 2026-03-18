#pragma once

/**
 * @file schema_filter.h
 * @brief Schema validation filter for fast-rejecting unknown JSON keys.
 *
 * Provides two levels of filtering:
 *   1. Global filter: checks if a key exists anywhere in the schema.
 *   2. Depth-aware filter: checks if a key is valid at a specific nesting depth.
 *
 * The depth-aware filter is useful for nested JSON schemas where different
 * properties are valid at different levels (e.g., "name" is valid at depth 1
 * but "street" is only valid at depth 2 inside an "address" object).
 *
 * Batch validation via validate_batch() processes up to 64 keys at once,
 * returning a bitmask of which keys passed the Bloom filter.
 */

#include "strata/bloom/sizing.h"

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace strata {
namespace bloom {

class SchemaFilter {
  public:
    /// Build from a JSON Schema string.
    /// Extracts all "properties" keys at all nesting levels.
    explicit SchemaFilter(std::string_view json_schema);

    /// Build from a flat list of allowed keys (all depths).
    explicit SchemaFilter(std::span<const std::string_view> allowed_keys);

    struct ValidationResult {
        bool is_valid;          ///< false if key is definitely not in schema
        bool needs_exact_check; ///< true if Bloom filter returned positive
        uint16_t key_hash;      ///< cached hash for downstream use
    };

    /// Validate a key against the global schema (any depth).
    [[nodiscard]] ValidationResult validate(std::string_view key) const noexcept;

    /**
     * Batch validate: check multiple keys at once.
     *
     * Returns a bitmask where bit i is set if key i passes (might be valid).
     * Processes up to 64 keys; count must be ≤ 64.
     */
    [[nodiscard]] uint64_t validate_batch(const std::string_view* keys,
                                          size_t count) const noexcept;

    /**
     * Validate a key at a specific nesting depth.
     *
     * If depth exceeds max_depth_, falls back to the global filter.
     */
    [[nodiscard]] ValidationResult validate_at_depth(std::string_view key,
                                                     uint8_t depth) const noexcept;

    /// Maximum nesting depth for which we have a per-depth filter.
    static constexpr size_t MAX_DEPTH = 16;

    /// Number of schema keys stored.
    [[nodiscard]] size_t num_keys() const noexcept { return all_keys_.size(); }

    /// Add keys valid at a specific depth (for programmatic schema building).
    void add_keys_at_depth(std::span<const std::string_view> keys, uint8_t depth);

  private:
    MediumFilter global_filter_;                                 ///< All keys at any depth
    std::array<SmallFilter, MAX_DEPTH> depth_filters_;           ///< Per-depth Bloom filters
    std::array<std::vector<std::string>, MAX_DEPTH> depth_keys_; ///< Per-depth exact keys (sorted)
    std::vector<std::string> all_keys_;                          ///< For exact confirmation
    uint8_t max_depth_ = 0;

    /// Compute a 16-bit hash for caching (truncated wyhash).
    [[nodiscard]] static uint16_t short_hash(std::string_view key) noexcept;

    /// Parse a JSON Schema string and populate filters.
    void parse_schema(std::string_view json_schema);
};

} // namespace bloom
} // namespace strata

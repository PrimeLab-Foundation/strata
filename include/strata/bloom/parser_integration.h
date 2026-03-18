#pragma once

/**
 * @file parser_integration.h
 * @brief Integration of Bloom filter subsystem with the structural-index parser.
 *
 * SelectiveParser combines the structural index (byte-offset array of { } [ ] : ,)
 * with a KeyFilter to parse only desired key-value pairs from a JSON object,
 * skipping unwanted values at near-zero cost.
 *
 * The skip_value() function is the key optimization:
 *   - For primitives: advance to the next structural character (, } ])
 *   - For objects/arrays: count braces/brackets in the structural index until
 *     depth returns to the entry level
 *   - This is O(structural_chars_in_value) but does ZERO actual value parsing —
 *     just pointer arithmetic on the structural index array.
 *
 * Usage:
 *   auto idx = builder.build(data, length);
 *   KeyFilter filter(desired_keys);
 *   SelectiveParser sp(filter);
 *   auto result = sp.parse_selective(data, length,
 *                                     idx.positions.data(), idx.positions.size());
 */

#include "strata/bloom/key_filter.h"
#include "strata/json/json_core.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace strata {
namespace bloom {

/**
 * @brief Parses only selected fields from JSON objects using Bloom filter
 *        rejection and structural index skipping.
 */
class SelectiveParser {
  public:
    explicit SelectiveParser(const KeyFilter& filter);

    /**
     * Parse a JSON document, extracting only desired keys.
     *
     * @param data                  Raw JSON bytes.
     * @param length                Length in bytes.
     * @param structural_positions  Array of byte offsets of structural chars.
     * @param num_structural        Number of entries in structural_positions.
     * @return JsonValue containing only matched key-value pairs for objects.
     *         Arrays and non-object top-level values are parsed fully.
     */
    [[nodiscard]] JsonValue parse_selective(const uint8_t* data, size_t length,
                                            const uint32_t* structural_positions,
                                            size_t num_structural);

    /// Statistics from the last parse.
    struct ParseStats {
        uint64_t values_parsed = 0;  ///< Values actually parsed
        uint64_t values_skipped = 0; ///< Values skipped via structural index
        uint64_t keys_checked = 0;   ///< Keys passed through KeyFilter
    };

    [[nodiscard]] ParseStats parse_stats() const noexcept { return parse_stats_; }
    void reset_stats() noexcept { parse_stats_ = {}; }

  private:
    const KeyFilter& filter_;
    ParseStats parse_stats_;

    /**
     * Skip over a JSON value using only the structural index.
     *
     * No actual parsing — just counting braces/brackets to find the end.
     *
     * @param data       Raw JSON bytes (for peeking at structural char type).
     * @param positions  Structural position array.
     * @param pos_index  Current index into positions (pointing at the start
     *                   of the value to skip).
     * @param num_positions Total number of structural positions.
     * @return Index into positions[] after the skipped value.
     */
    [[nodiscard]] size_t skip_value(const uint8_t* data, const uint32_t* positions,
                                    size_t pos_index, size_t num_positions) const noexcept;

    /**
     * Parse a JSON value starting at the given structural position.
     *
     * @param data       Raw JSON bytes.
     * @param length     Document length.
     * @param positions  Structural position array.
     * @param pos_index  Current index (updated on return).
     * @param num_positions Total structural positions.
     * @param selective  If true, apply key filtering to nested objects.
     * @return Parsed JsonValue.
     */
    JsonValue parse_value(const uint8_t* data, size_t length, const uint32_t* positions,
                          size_t& pos_index, size_t num_positions, bool selective);

    /// Parse a JSON string starting after the opening quote.
    /// Returns the byte position after the closing quote.
    [[nodiscard]] std::string parse_string(const uint8_t* data, size_t start, size_t length) const;

    /// Parse a JSON number starting at the given position.
    [[nodiscard]] double parse_number(const uint8_t* data, size_t start, size_t length) const;

    /// Skip whitespace from a position.
    [[nodiscard]] size_t skip_ws(const uint8_t* data, size_t pos, size_t length) const noexcept;

    /// Parse a primitive JSON value (string, number, bool, null) from raw bytes.
    [[nodiscard]] JsonValue parse_primitive(const uint8_t* data, size_t start, size_t end,
                                            size_t length) const;
};

} // namespace bloom
} // namespace strata

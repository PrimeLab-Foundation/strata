#pragma once

// =============================================================================
// strata/speculative/parser.h
//
// Speculative parser coordinator. Ties together the Markov transition model,
// fast-path parsers, and the structural index to parse JSON/NDJSON with
// statistical prediction of value types.
//
// Flow:
//   1. Look at next byte in the structural index
//   2. Ask TransitionModel for a prediction
//   3. If confident, dispatch to the predicted fast-path parser
//   4. If fast-path succeeds (hit), use the result
//   5. If fast-path fails (miss) or model not confident, use generic parser
//   6. Update model with actual observed type (online learning)
//
// The coordinator maintains a ParserContext that tracks position in the JSON
// tree (key hash, nesting depth, array index) to feed the Markov model.
// =============================================================================

#include <cstdint>
#include <strata/bloom/key_filter.h>
#include <strata/json/json_core.hpp>
#include <strata/speculative/fast_paths.h>
#include <strata/speculative/transition_model.h>
#include <strata/speculative/types.h>
#include <strata/util/arena_allocator.hpp>
#include <vector>

namespace strata::speculative {

class SpeculativeParser {
  public:
    struct Config {
        float confidence_threshold = 0.85f;
        bool enable_online_learning = true;
        bool enable_speculation = true;      // false = always generic (for benchmarking)
        size_t online_learning_warmup = 100; // observe N values before speculating
    };

    SpeculativeParser(const Config& config, strata::util::Arena& arena);

    // Parse a complete JSON document using speculative optimization.
    // structural_positions: output from SIMD IndexBuilder (byte offsets of structural chars).
    // num_structural: number of entries in structural_positions.
    // Returns root JsonValue (allocated on stack/arena as needed).
    JsonValue parse(const uint8_t* data, size_t length, const uint32_t* structural_positions,
                    size_t num_structural);

    // Parse NDJSON: multiple newline-delimited JSON documents.
    // The model learns across documents, improving predictions over time.
    std::vector<JsonValue> parse_ndjson(const uint8_t* data, size_t length,
                                        const uint32_t* structural_positions,
                                        size_t num_structural);

    // Access model for serialization / stats / merging
    [[nodiscard]] const TransitionModel& model() const noexcept { return model_; }
    [[nodiscard]] TransitionModel& model() noexcept { return model_; }

    /// Set an optional key filter for selective parsing.
    /// When set, the parser skips values of keys not in the filter.
    /// The filter pointer must remain valid for the lifetime of the parse call.
    void set_key_filter(const bloom::KeyFilter* filter) noexcept { key_filter_ = filter; }

  private:
    Config config_;
    TransitionModel model_;
    FastPaths fast_paths_;
    [[maybe_unused]] strata::util::Arena& arena_;
    uint64_t values_observed_ = 0;                 // for warmup tracking
    const bloom::KeyFilter* key_filter_ = nullptr; // optional selective parsing filter

    // ── Core recursive-descent with speculation ─────────────────────────

    // Parse a value at `cursor` in raw data. Tries speculation first,
    // falls back to generic parsing on miss.
    JsonValue parse_value_at(const uint8_t* data, size_t cursor, size_t length,
                             const uint32_t* structural_positions, size_t num_structural,
                             ParserContext& ctx, size_t& pos_index);

    // Generic (non-speculative) value parser — the fallback.
    JsonValue parse_value_generic(const uint8_t* data, size_t length,
                                  const uint32_t* structural_positions, size_t num_structural,
                                  ParserContext& ctx, size_t& pos_index);

    // Parse a primitive (non-container) value at known cursor position.
    JsonValue parse_primitive_at(const uint8_t* data, size_t cursor, size_t length,
                                 const uint32_t* structural_positions, size_t num_structural,
                                 ParserContext& ctx, size_t& pos_index);

    // ── Helpers ─────────────────────────────────────────────────────────

    // Parse an object: expects to be called with data[offset] == '{'
    JsonValue parse_object(const uint8_t* data, size_t length, const uint32_t* structural_positions,
                           size_t num_structural, ParserContext& ctx, size_t& pos_index);

    // Parse an array: expects to be called with data[offset] == '['
    JsonValue parse_array(const uint8_t* data, size_t length, const uint32_t* structural_positions,
                          size_t num_structural, ParserContext& ctx, size_t& pos_index);

    // Skip whitespace starting from offset, return new offset
    static size_t skip_ws(const uint8_t* data, size_t offset, size_t length) noexcept;

    // Skip a JSON value using the structural index (no parsing, just brace counting).
    // Used by selective parsing to jump past unwanted key values.
    void skip_value_structural(const uint8_t* data, size_t length, const uint32_t* sp,
                               size_t num_sp, size_t val_start, size_t& pos_index) noexcept;

    // Determine the actual ValueType from the first byte of a value
    static ValueType classify_value_byte(uint8_t c) noexcept;

    // Parse a JSON string starting at data[offset] (which should be '"').
    // Returns the string content and advances offset past the closing quote.
    std::string parse_string_at(const uint8_t* data, size_t& offset, size_t length);
};

} // namespace strata::speculative

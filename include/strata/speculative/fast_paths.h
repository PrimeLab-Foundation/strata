#pragma once

// =============================================================================
// strata/speculative/fast_paths.h
//
// Specialized, optimized parsers for each predicted value type.
// Each fast-path assumes the predicted type and parses WITHOUT generic
// dispatch. If the assumption is violated, returns {.success = false}
// immediately so the caller can fall back to the generic parser.
//
// Key techniques:
//   - SWAR (SIMD Within A Register) for 8-digit-at-a-time integer parsing
//   - Single AVX2/NEON load for short strings (≤32 bytes)
//   - Branchless 4/5-byte literal comparison for bool/null
//   - Lemire-style fast_float for doubles
// =============================================================================

#include <array>
#include <cstdint>
#include <cstring>
#include <strata/json/json_core.hpp>
#include <strata/speculative/types.h>
#include <strata/util/arena_allocator.hpp>

namespace strata::speculative {

// ── Parse result ─────────────────────────────────────────────────────────────
struct ParseResult {
    bool success;          // false = speculation failed, use generic parser
    size_t bytes_consumed; // number of bytes consumed from data
    JsonValue value;       // parsed value (valid only if success == true)
};

// ── Fast-path parser collection ──────────────────────────────────────────────
class FastPaths {
  public:
    explicit FastPaths(strata::util::Arena& arena) noexcept : arena_(arena) {}

    // ── Integer parsing (SWAR technique) ────────────────────────────────
    //
    // Load 8 bytes at a time into a uint64_t.
    // Subtract 0x3030303030303030 (ASCII '0' from each byte).
    // Check if any byte > 9 → marks end of digit run.
    // Multiply accumulated digits using SWAR reduction.
    //
    // Targets 4+ GB/s on typical integer-heavy JSON.
    [[nodiscard]] ParseResult parse_integer(const uint8_t* data, size_t remaining) noexcept;

    // ── Float parsing ───────────────────────────────────────────────────
    //
    // Expects digits with decimal point and/or exponent.
    // Uses strtod-fast-path (Lemire's algorithm via fast_float).
    [[nodiscard]] ParseResult parse_float(const uint8_t* data, size_t remaining) noexcept;

    // ── Short string (≤32 bytes, no escapes) ────────────────────────────
    //
    // Single 32-byte load (or 16-byte on NEON), scan for closing quote
    // and backslash simultaneously. If no escape found and quote within
    // range → direct copy to arena.
    [[nodiscard]] ParseResult parse_short_string(const uint8_t* data, size_t remaining) noexcept;

    // ── Long string (>32 bytes or with escapes) ─────────────────────────
    //
    // SIMD scan for backslash and quote. Processes escape sequences
    // using a lookup table for common escapes (\n, \t, \\, \", etc.).
    [[nodiscard]] ParseResult parse_long_string(const uint8_t* data, size_t remaining) noexcept;

    // ── Literal parsers (branchless) ────────────────────────────────────
    //
    // Compare 4 or 5 bytes as a single 32-bit or 64-bit integer.
    [[nodiscard]] ParseResult parse_bool_true(const uint8_t* data, size_t remaining) noexcept;
    [[nodiscard]] ParseResult parse_bool_false(const uint8_t* data, size_t remaining) noexcept;
    [[nodiscard]] ParseResult parse_null(const uint8_t* data, size_t remaining) noexcept;

    // ── Container openers ───────────────────────────────────────────────
    // These don't parse the container — they just confirm the opening
    // character matches prediction, enabling the coordinator to proceed.
    [[nodiscard]] ParseResult parse_object_open(const uint8_t* data, size_t remaining) noexcept;
    [[nodiscard]] ParseResult parse_array_open(const uint8_t* data, size_t remaining) noexcept;

    // ── Dispatch table ──────────────────────────────────────────────────
    using ParseFn = ParseResult (FastPaths::*)(const uint8_t*, size_t) noexcept;

    static constexpr size_t NUM_TYPES = static_cast<size_t>(ValueType::NUM_TYPES);

    // Jump table indexed by ValueType — enables branchless dispatch.
    // OBJECT and ARRAY entries just validate the opening char; the
    // coordinator handles recursive descent.
    static const std::array<ParseFn, NUM_TYPES>& dispatch_table() noexcept;

  private:
    [[maybe_unused]] strata::util::Arena& arena_;

    // ── SWAR helpers ────────────────────────────────────────────────────

    // Parse up to 8 digits from a uint64_t word.
    // Returns (value, digit_count). digit_count==0 means no valid digits.
    static std::pair<uint64_t, int> parse_8_digits(uint64_t word) noexcept;

    // Check if a byte is a JSON delimiter (whitespace, comma, ], }, etc.)
    static bool is_structural_or_ws(uint8_t c) noexcept {
        return c <= ' ' || c == ',' || c == ']' || c == '}' || c == ':';
    }
};

} // namespace strata::speculative

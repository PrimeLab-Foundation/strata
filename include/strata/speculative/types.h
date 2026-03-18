#pragma once

// =============================================================================
// strata/speculative/types.h
//
// Value type taxonomy and parser context for the speculative parsing engine.
// The Markov transition model predicts the next ValueType given a ParserContext,
// allowing the parser to skip generic dispatch and jump directly to a
// specialized fast-path.
// =============================================================================

#include <cstdint>
#include <cstring>

namespace strata::speculative {

// ── Value types that the speculative engine predicts ────────────────────────
enum class ValueType : uint8_t {
    INTEGER = 0,      // JSON number, no decimal point or exponent
    FLOAT = 1,        // JSON number with '.' or 'e'/'E'
    SHORT_STRING = 2, // string <= 32 bytes, no escapes
    LONG_STRING = 3,  // string > 32 bytes or contains escapes
    BOOL_TRUE = 4,
    BOOL_FALSE = 5,
    NULL_VALUE = 6,
    OBJECT = 7, // nested {
    ARRAY = 8,  // nested [
    NUM_TYPES = 9
};

// ── Context for Markov model ────────────────────────────────────────────────
// Encodes "where we are" in the JSON structure so the transition model can
// learn position-dependent patterns (e.g., "the 'age' key always maps to int").
struct ParserContext {
    uint16_t key_hash;         // hash of current key (0 if in array)
    uint8_t nesting_depth;     // current depth (capped at 255)
    uint8_t array_index;       // position in array (capped at 255)
    ValueType last_value_type; // what we just parsed
    bool in_array;             // true if parent is array, false if object
};

// ── Compact context ID for transition table lookup ──────────────────────────
// Maps ParserContext -> a 32-bit integer used as a hash key.
//
// We mix the most predictive fields: key_hash (dominates in objects),
// array_index (dominates in arrays), and nesting_depth (disambiguates levels).
// last_value_type adds sequential correlation.
//
// Uses a fast integer mixing function (murmurhash3 finalizer variant) to
// distribute contexts uniformly across the bucket space.
inline uint32_t context_id(const ParserContext& ctx) noexcept {
    // Pack the fields into a 32-bit value for mixing.
    // key_hash:        16 bits  (most important for object fields)
    // nesting_depth:    5 bits  (capped contribution, deep nesting is rare)
    // array_index:      5 bits  (first 32 positions matter most)
    // last_value_type:  4 bits  (9 types fit in 4 bits)
    // in_array:         1 bit
    uint32_t packed = static_cast<uint32_t>(ctx.key_hash);
    packed ^= static_cast<uint32_t>(ctx.nesting_depth & 0x1F) << 16;
    packed ^= static_cast<uint32_t>(ctx.array_index & 0x1F) << 21;
    packed ^= static_cast<uint32_t>(ctx.last_value_type) << 26;
    packed ^= static_cast<uint32_t>(ctx.in_array) << 30;

    // murmurhash3 32-bit finalizer — excellent avalanche properties
    packed ^= packed >> 16;
    packed *= 0x85ebca6b;
    packed ^= packed >> 13;
    packed *= 0xc2b2ae35;
    packed ^= packed >> 16;
    return packed;
}

// ── Fast key hashing ────────────────────────────────────────────────────────
// FNV-1a 16-bit hash for JSON keys. Collisions are acceptable — they just
// reduce prediction accuracy for colliding keys, never cause incorrect parses.
inline uint16_t hash_key(const uint8_t* key, size_t len) noexcept {
    uint32_t h = 0x811c9dc5u; // FNV offset basis
    for (size_t i = 0; i < len; ++i) {
        h ^= key[i];
        h *= 0x01000193u; // FNV prime
    }
    // Fold 32-bit hash to 16 bits via XOR-folding
    return static_cast<uint16_t>((h >> 16) ^ (h & 0xFFFF));
}

} // namespace strata::speculative

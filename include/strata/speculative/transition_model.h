#pragma once

// =============================================================================
// strata/speculative/transition_model.h
//
// Markov transition model for speculative JSON parsing. Maintains a lossy
// hash table of (context -> value-type distribution) buckets. Each bucket
// stores uint16_t counts per ValueType, enabling fixed-point probability
// computation without floating-point arithmetic in the hot path.
//
// Design constraints:
//   - predict() < 20 cycles: no division, no branches in common path
//   - observe() < 50 cycles: simple increment + periodic decay
//   - Total model size < 128 KB (fits in L1/L2 cache)
//   - NOT thread-safe: each thread gets its own copy; merge() after
// =============================================================================

#include <array>
#include <cstdint>
#include <cstring>
#include <strata/speculative/types.h>
#include <vector>

namespace strata::speculative {

class TransitionModel {
  public:
    // 4096 buckets * 30 bytes/bucket ≈ 120 KB (fits in L2 cache)
    static constexpr size_t MAX_CONTEXTS = 4096;

    // Minimum probability to attempt speculative parsing.
    // 0.85 in fixed-point: (0.85 * 65536) = 55706
    static constexpr uint32_t CONFIDENCE_THRESHOLD_FP = 55706;
    static constexpr float CONFIDENCE_THRESHOLD = 0.85f;

    // ── Prediction result ───────────────────────────────────────────────
    struct Prediction {
        ValueType predicted_type;
        float confidence;      // 0.0 - 1.0
        bool should_speculate; // confidence >= CONFIDENCE_THRESHOLD
    };

    TransitionModel() noexcept;

    // Predict next value type given current context.
    // Hot path: finds max count in bucket using unrolled loop, compares
    // against threshold using fixed-point arithmetic. No division — uses
    // multiply-and-shift instead.
    [[nodiscard]] Prediction predict(const ParserContext& ctx) const noexcept;

    // Update model after observing an actual value type.
    // Increments the count for `actual` in the matching bucket.
    // Every 256 observations per bucket, applies exponential decay
    // (right-shift all counts by 1) to adapt to changing patterns.
    void observe(const ParserContext& ctx, ValueType actual) noexcept;

    // ── Batch training ──────────────────────────────────────────────────

    // Pre-populate transition probabilities from a simplified JSON Schema.
    // Recognizes: {"type":"object","properties":{"key":{"type":"..."},...}}
    // and {"type":"array","items":{"type":"..."}}
    void train_from_schema(std::string_view schema_json);

    // Parse sample documents and observe all transitions.
    // `data` may contain `num_docs` newline-delimited JSON documents.
    void train_from_samples(const uint8_t* data, size_t length, size_t num_docs);

    // ── Serialization ───────────────────────────────────────────────────

    // Serialize to a compact binary format for caching trained models.
    [[nodiscard]] std::vector<uint8_t> serialize() const;

    // Deserialize from binary format produced by serialize().
    static TransitionModel deserialize(const uint8_t* data, size_t length);

    // ── Multi-thread model merging ──────────────────────────────────────

    // Merge another model's observations into this one.
    // Used after parallel NDJSON processing to combine per-thread models.
    void merge(const TransitionModel& other) noexcept;

    // ── Statistics ───────────────────────────────────────────────────────

    struct Stats {
        uint64_t total_predictions = 0;
        uint64_t correct_predictions = 0;
        uint64_t speculations_attempted = 0;
        uint64_t speculations_hit = 0;

        double hit_rate() const noexcept {
            return total_predictions > 0 ? static_cast<double>(correct_predictions) /
                                               static_cast<double>(total_predictions)
                                         : 0.0;
        }
        double speculation_rate() const noexcept {
            return speculations_attempted > 0 ? static_cast<double>(speculations_hit) /
                                                    static_cast<double>(speculations_attempted)
                                              : 0.0;
        }
    };

    [[nodiscard]] Stats stats() const noexcept { return stats_; }
    void reset_stats() noexcept { stats_ = {}; }

    // Direct stats manipulation (used by SpeculativeParser to record
    // prediction outcomes without exposing the full Stats struct).
    void record_prediction() noexcept { ++stats_.total_predictions; }
    void record_correct_prediction() noexcept { ++stats_.correct_predictions; }
    void record_speculation_attempted() noexcept { ++stats_.speculations_attempted; }
    void record_speculation_hit() noexcept { ++stats_.speculations_hit; }

    // ── Direct access (for testing) ─────────────────────────────────────
    static constexpr size_t NUM_TYPES = static_cast<size_t>(ValueType::NUM_TYPES);

    struct ContextBucket {
        uint16_t counts[NUM_TYPES] = {};
        uint32_t total = 0;
        uint32_t context_signature = 0; // for collision detection
    };

    const ContextBucket& bucket_at(size_t idx) const noexcept { return table_[idx]; }

  private:
    // ── Transition table ────────────────────────────────────────────────
    // Lossy cache: collisions cause the losing context to share the bucket,
    // degrading prediction quality but never correctness. This avoids
    // chaining / open-addressing overhead.
    alignas(64) std::array<ContextBucket, MAX_CONTEXTS> table_;

    Stats stats_;

    // Decay period: every 256 observations per bucket, halve all counts.
    // This implements exponential decay with effective learning rate ~0.01
    // (each observation's influence halves every ~256 subsequent observations).
    static constexpr uint32_t DECAY_PERIOD = 256;

    // Map context to bucket index (modular reduction by AND since MAX_CONTEXTS is power-of-2)
    uint32_t bucket_index(const ParserContext& ctx) const noexcept {
        return context_id(ctx) & (MAX_CONTEXTS - 1);
    }

    // Signature for collision detection (upper bits of context_id)
    static uint32_t make_signature(const ParserContext& ctx) noexcept {
        return context_id(ctx); // full hash — bucket_index uses low bits, sig uses all
    }
};

// Compile-time size check: model must fit in 128 KB
static_assert(sizeof(TransitionModel) <= 128 * 1024,
              "TransitionModel exceeds 128 KB L1/L2 cache budget");

} // namespace strata::speculative

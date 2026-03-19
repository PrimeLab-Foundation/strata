// =============================================================================
// strata/speculative/transition_model.cpp
//
// Implementation of the Markov transition model for speculative JSON parsing.
// =============================================================================

#include <algorithm>
#include <cmath>
#include <cstring>
#include <strata/speculative/transition_model.h>
#include <string>

namespace strata::speculative {

// ─── Construction ────────────────────────────────────────────────────────────

TransitionModel::TransitionModel() noexcept : table_{}, stats_{} {
    // Zero-initialized by aggregate init above. Every bucket starts with
    // total=0 and context_signature=0.
}

// ─── Prediction (hot path, < 20 cycles target) ──────────────────────────────
//
// Algorithm:
// 1. Look up bucket via context hash (AND mask — 1 cycle)
// 2. Check collision via signature (1 compare — 1 cycle)
// 3. Find max count via unrolled loop over 9 uint16_t values (~9 cycles)
// 4. Compare max * 65536 against threshold * total (multiply-shift, no div)
//
// Total: ~15 cycles in the common (no-collision, has-data) path.

TransitionModel::Prediction TransitionModel::predict(const ParserContext& ctx) const noexcept {
    const uint32_t idx = bucket_index(ctx);
    const ContextBucket& b = table_[idx];

    // Empty bucket or collision → no prediction
    const uint32_t sig = make_signature(ctx);
    if (b.total == 0 || (b.context_signature != 0 && b.context_signature != sig)) {
        return {ValueType::INTEGER, 0.0f, false};
    }

    // Find the type with the maximum count (unrolled, branchless argmax).
    // With only 9 elements, a linear scan is faster than SIMD setup overhead.
    uint16_t max_count = b.counts[0];
    uint8_t max_idx = 0;

    // Unrolled comparison: compiler will emit CMOVs (branchless)
    for (uint8_t i = 1; i < NUM_TYPES; ++i) {
        if (b.counts[i] > max_count) {
            max_count = b.counts[i];
            max_idx = i;
        }
    }

    // Fixed-point confidence check:
    //   confidence = max_count / total
    //   should_speculate = confidence >= 0.85
    //
    // Rearranged to avoid division:
    //   max_count * 65536 >= CONFIDENCE_THRESHOLD_FP * total
    //
    // Both sides fit in uint64_t (max_count <= 65535, total <= ~2^32).
    const uint64_t lhs = static_cast<uint64_t>(max_count) * 65536ULL;
    const uint64_t rhs =
        static_cast<uint64_t>(CONFIDENCE_THRESHOLD_FP) * static_cast<uint64_t>(b.total);
    const bool should_spec = lhs >= rhs;

    // Compute float confidence only for the return value (not on hot path
    // of the caller's branch — the caller checks should_speculate first).
    const float confidence = static_cast<float>(max_count) / static_cast<float>(b.total);

    return {static_cast<ValueType>(max_idx), confidence, should_spec};
}

// ─── Observation (< 50 cycles target) ────────────────────────────────────────
//
// Algorithm:
// 1. Look up bucket (AND mask — 1 cycle)
// 2. Claim empty bucket or verify signature (1 compare)
// 3. Increment counts[actual] (1 add)
// 4. Increment total (1 add)
// 5. Every DECAY_PERIOD observations, halve all counts (9 shifts ≈ 9 cycles)
//
// Total: ~10 cycles common path, ~25 cycles on decay path (amortized ~10).

void TransitionModel::observe(const ParserContext& ctx, ValueType actual) noexcept {
    const uint32_t idx = bucket_index(ctx);
    ContextBucket& b = table_[idx];
    const uint32_t sig = make_signature(ctx);

    // Claim empty bucket or verify ownership
    if (b.total == 0) {
        b.context_signature = sig;
    } else if (b.context_signature != sig) {
        // Collision: if the existing bucket is cold (low total), evict it.
        // Otherwise, just skip the update — lossy cache semantics.
        if (b.total < 16) {
            // Evict: reset and claim for new context
            std::memset(b.counts, 0, sizeof(b.counts));
            b.total = 0;
            b.context_signature = sig;
        } else {
            return; // silently drop — prediction for both contexts degrades
        }
    }

    // Increment count for observed type
    const auto type_idx = static_cast<size_t>(actual);
    if (type_idx < NUM_TYPES) {
        // Saturate at uint16_t max to avoid overflow
        if (b.counts[type_idx] < UINT16_MAX) {
            b.counts[type_idx]++;
        }
        b.total++;
    }

    // Periodic decay: every DECAY_PERIOD observations, halve all counts.
    // This implements exponential moving average behavior — recent observations
    // carry more weight. The effective window is ~256 observations.
    if ((b.total & (DECAY_PERIOD - 1)) == 0 && b.total > 0) {
        uint32_t new_total = 0;
        for (size_t i = 0; i < NUM_TYPES; ++i) {
            b.counts[i] >>= 1;
            new_total += b.counts[i];
        }
        b.total = new_total;
    }
}

// ─── Schema training ─────────────────────────────────────────────────────────
//
// Simplified JSON Schema support. Recognizes:
//   {"type":"object","properties":{"key":{"type":"string"|"integer"|...},...}}
//   {"type":"array","items":{"type":"..."}}
//
// For each property, we inject a high-confidence observation so that
// the first document parsed already benefits from speculation.

namespace {

// Map JSON Schema type strings to our ValueType enum
ValueType schema_type_to_value_type(std::string_view type_str) {
    if (type_str == "string")
        return ValueType::SHORT_STRING;
    if (type_str == "integer")
        return ValueType::INTEGER;
    if (type_str == "number")
        return ValueType::FLOAT;
    if (type_str == "boolean")
        return ValueType::BOOL_TRUE;
    if (type_str == "null")
        return ValueType::NULL_VALUE;
    if (type_str == "object")
        return ValueType::OBJECT;
    if (type_str == "array")
        return ValueType::ARRAY;
    return ValueType::SHORT_STRING; // default
}

// Minimal JSON string extractor: find value for a key in a simple flat object.
// This is NOT a full JSON parser — it handles the simple schema format only.
std::string_view extract_string_value(std::string_view json, std::string_view key) {
    auto kpos = json.find(key);
    if (kpos == std::string_view::npos)
        return {};

    // Find the colon after the key
    auto colon = json.find(':', kpos + key.size());
    if (colon == std::string_view::npos)
        return {};

    // Find opening quote of value
    auto qstart = json.find('"', colon + 1);
    if (qstart == std::string_view::npos)
        return {};

    // Find closing quote
    auto qend = json.find('"', qstart + 1);
    if (qend == std::string_view::npos)
        return {};

    return json.substr(qstart + 1, qend - qstart - 1);
}

// Find a sub-object by key name, returning the brace-delimited content.
std::string_view extract_object(std::string_view json, std::string_view key) {
    auto kpos = json.find(key);
    if (kpos == std::string_view::npos)
        return {};

    auto brace = json.find('{', kpos + key.size());
    if (brace == std::string_view::npos)
        return {};

    int depth = 1;
    size_t i = brace + 1;
    while (i < json.size() && depth > 0) {
        if (json[i] == '{')
            ++depth;
        else if (json[i] == '}')
            --depth;
        ++i;
    }
    if (depth != 0)
        return {};
    return json.substr(brace, i - brace);
}

} // anonymous namespace

void TransitionModel::train_from_schema(std::string_view schema_json) {
    // Extract "properties" object
    auto props = extract_object(schema_json, "\"properties\"");
    if (props.empty())
        return;

    // Parse each property: find "key":{"type":"..."} pairs
    // Iterate through the properties object looking for quoted keys
    size_t pos = 1; // skip opening brace
    while (pos < props.size()) {
        // Find key
        auto key_start = props.find('"', pos);
        if (key_start == std::string_view::npos || key_start >= props.size() - 1)
            break;
        auto key_end = props.find('"', key_start + 1);
        if (key_end == std::string_view::npos)
            break;

        std::string_view key_name = props.substr(key_start + 1, key_end - key_start - 1);

        // Find the value object for this key
        auto val_brace = props.find('{', key_end + 1);
        if (val_brace == std::string_view::npos)
            break;

        // Find matching close brace
        int depth = 1;
        size_t end = val_brace + 1;
        while (end < props.size() && depth > 0) {
            if (props[end] == '{')
                ++depth;
            else if (props[end] == '}')
                --depth;
            ++end;
        }

        std::string_view val_obj = props.substr(val_brace, end - val_brace);
        auto type_str = extract_string_value(val_obj, "\"type\"");

        if (!type_str.empty()) {
            ValueType vt = schema_type_to_value_type(type_str);
            uint16_t kh =
                hash_key(reinterpret_cast<const uint8_t*>(key_name.data()), key_name.size());

            // Inject synthetic observations to pre-seed the model.
            // 200 observations gives ~0.96 confidence in a fresh bucket.
            ParserContext pctx{};
            pctx.key_hash = kh;
            pctx.nesting_depth = 1;
            pctx.array_index = 0;
            pctx.last_value_type = ValueType::SHORT_STRING; // key is always a string
            pctx.in_array = false;

            for (int i = 0; i < 200; ++i) {
                observe(pctx, vt);
            }
        }

        pos = end;
    }
}

// ─── Sample training ─────────────────────────────────────────────────────────
//
// Parses sample documents using a lightweight recursive scanner (not the full
// speculative parser — we just observe transitions to build the model).

namespace {

// Skip whitespace
inline size_t skip_ws(const uint8_t* data, size_t pos, size_t len) {
    while (pos < len &&
           (data[pos] == ' ' || data[pos] == '\t' || data[pos] == '\n' || data[pos] == '\r')) {
        ++pos;
    }
    return pos;
}

// Skip a JSON string (assumes pos points to opening quote), returns pos after closing quote
inline size_t skip_string(const uint8_t* data, size_t pos, size_t len) {
    ++pos; // skip opening quote
    while (pos < len) {
        if (data[pos] == '\\') {
            pos += 2;
            continue;
        }
        if (data[pos] == '"')
            return pos + 1;
        ++pos;
    }
    return pos;
}

// Extract string content (assumes pos points to opening quote)
inline std::pair<const uint8_t*, size_t> extract_string_content(const uint8_t* data, size_t pos,
                                                                size_t len) {
    const uint8_t* start = data + pos + 1;
    size_t spos = pos + 1;
    while (spos < len) {
        if (data[spos] == '\\') {
            spos += 2;
            continue;
        }
        if (data[spos] == '"')
            return {start, spos - (pos + 1)};
        ++spos;
    }
    return {start, 0};
}

} // anonymous namespace

// Recursive training scanner — lightweight, just collects transitions
namespace {

size_t train_scan_value(TransitionModel& model, const uint8_t* data, size_t pos, size_t len,
                        ParserContext& ctx);

size_t train_scan_object(TransitionModel& model, const uint8_t* data, size_t pos, size_t len,
                         ParserContext& ctx) {
    pos = skip_ws(data, pos + 1, len); // skip '{'
    if (pos < len && data[pos] == '}')
        return pos + 1;

    ParserContext child_ctx{};
    child_ctx.nesting_depth = static_cast<uint8_t>(std::min<uint32_t>(ctx.nesting_depth + 1, 255));
    child_ctx.in_array = false;
    child_ctx.array_index = 0;

    while (pos < len) {
        pos = skip_ws(data, pos, len);
        if (pos >= len || data[pos] != '"')
            break;

        // Extract key for hashing
        auto [key_ptr, key_len] = extract_string_content(data, pos, len);
        child_ctx.key_hash = hash_key(key_ptr, key_len);
        pos = skip_string(data, pos, len);

        pos = skip_ws(data, pos, len);
        if (pos < len && data[pos] == ':')
            ++pos;
        pos = skip_ws(data, pos, len);

        pos = train_scan_value(model, data, pos, len, child_ctx);

        pos = skip_ws(data, pos, len);
        if (pos < len && data[pos] == ',')
            ++pos;
        else
            break;
    }
    pos = skip_ws(data, pos, len);
    if (pos < len && data[pos] == '}')
        ++pos;
    return pos;
}

size_t train_scan_array(TransitionModel& model, const uint8_t* data, size_t pos, size_t len,
                        ParserContext& ctx) {
    pos = skip_ws(data, pos + 1, len); // skip '['
    if (pos < len && data[pos] == ']')
        return pos + 1;

    ParserContext child_ctx{};
    child_ctx.nesting_depth = static_cast<uint8_t>(std::min<uint32_t>(ctx.nesting_depth + 1, 255));
    child_ctx.in_array = true;
    child_ctx.key_hash = 0;
    child_ctx.array_index = 0;

    while (pos < len) {
        pos = skip_ws(data, pos, len);
        pos = train_scan_value(model, data, pos, len, child_ctx);
        child_ctx.array_index =
            static_cast<uint8_t>(std::min<uint32_t>(child_ctx.array_index + 1, 255));

        pos = skip_ws(data, pos, len);
        if (pos < len && data[pos] == ',')
            ++pos;
        else
            break;
    }
    pos = skip_ws(data, pos, len);
    if (pos < len && data[pos] == ']')
        ++pos;
    return pos;
}

size_t train_scan_value(TransitionModel& model, const uint8_t* data, size_t pos, size_t len,
                        ParserContext& ctx) {
    pos = skip_ws(data, pos, len);
    if (pos >= len)
        return pos;

    ValueType observed;
    size_t new_pos;

    switch (data[pos]) {
    case '"': {
        // Determine SHORT_STRING vs LONG_STRING
        size_t end = pos + 1;
        bool has_escape = false;
        while (end < len && data[end] != '"') {
            if (data[end] == '\\') {
                has_escape = true;
                end += 2;
                continue;
            }
            ++end;
        }
        size_t slen = end - pos - 1;
        observed = (slen <= 32 && !has_escape) ? ValueType::SHORT_STRING : ValueType::LONG_STRING;
        new_pos = (end < len) ? end + 1 : end;
        break;
    }
    case '{':
        observed = ValueType::OBJECT;
        new_pos = train_scan_object(model, data, pos, len, ctx);
        break;
    case '[':
        observed = ValueType::ARRAY;
        new_pos = train_scan_array(model, data, pos, len, ctx);
        break;
    case 't':
        observed = ValueType::BOOL_TRUE;
        new_pos = pos + 4; // "true"
        break;
    case 'f':
        observed = ValueType::BOOL_FALSE;
        new_pos = pos + 5; // "false"
        break;
    case 'n':
        observed = ValueType::NULL_VALUE;
        new_pos = pos + 4; // "null"
        break;
    default: {
        // Number: scan to determine INTEGER vs FLOAT
        size_t npos = pos;
        bool is_float = false;
        if (npos < len && data[npos] == '-')
            ++npos;
        while (npos < len && data[npos] >= '0' && data[npos] <= '9')
            ++npos;
        if (npos < len && data[npos] == '.') {
            is_float = true;
            ++npos;
            while (npos < len && data[npos] >= '0' && data[npos] <= '9')
                ++npos;
        }
        if (npos < len && (data[npos] == 'e' || data[npos] == 'E')) {
            is_float = true;
            ++npos;
            if (npos < len && (data[npos] == '+' || data[npos] == '-'))
                ++npos;
            while (npos < len && data[npos] >= '0' && data[npos] <= '9')
                ++npos;
        }
        observed = is_float ? ValueType::FLOAT : ValueType::INTEGER;
        new_pos = npos;
        break;
    }
    }

    model.observe(ctx, observed);
    ctx.last_value_type = observed;
    return new_pos;
}

} // anonymous namespace

/// Train the model from newline-delimited sample documents.
/// Each line is parsed by a lightweight recursive scanner that observes
/// value-type transitions without building a DOM.
void TransitionModel::train_from_samples(const uint8_t* data, size_t length, size_t /*num_docs*/) {
    // Parse each newline-delimited document
    size_t pos = 0;
    while (pos < length) {
        pos = skip_ws(data, pos, length);
        if (pos >= length)
            break;

        ParserContext root_ctx{};
        root_ctx.key_hash = 0;
        root_ctx.nesting_depth = 0;
        root_ctx.array_index = 0;
        root_ctx.last_value_type = ValueType::NULL_VALUE;
        root_ctx.in_array = false;

        pos = train_scan_value(*this, data, pos, length, root_ctx);
    }
}

// ─── Serialization ───────────────────────────────────────────────────────────
//
// Binary format (little-endian):
//   [4 bytes] magic: "STM1"
//   [4 bytes] num_buckets (always MAX_CONTEXTS)
//   For each bucket:
//     [4 bytes] context_signature
//     [4 bytes] total
//     [18 bytes] counts (9 * uint16_t)
//   Total: 8 + 4096 * 26 = 106504 bytes

/// Serialize the entire transition table to a compact binary blob.
/// The format is versioned (magic "STM1") for forward compatibility.
std::vector<uint8_t> TransitionModel::serialize() const {
    constexpr size_t BUCKET_SIZE = 4 + 4 + NUM_TYPES * 2;
    constexpr size_t HEADER_SIZE = 8;
    std::vector<uint8_t> out(HEADER_SIZE + MAX_CONTEXTS * BUCKET_SIZE);

    uint8_t* p = out.data();

    // Header
    std::memcpy(p, "STM1", 4);
    p += 4;
    uint32_t n = static_cast<uint32_t>(MAX_CONTEXTS);
    std::memcpy(p, &n, 4);
    p += 4;

    // Buckets
    for (size_t i = 0; i < MAX_CONTEXTS; ++i) {
        const auto& b = table_[i];
        std::memcpy(p, &b.context_signature, 4);
        p += 4;
        std::memcpy(p, &b.total, 4);
        p += 4;
        std::memcpy(p, b.counts, NUM_TYPES * 2);
        p += NUM_TYPES * 2;
    }

    return out;
}

/// Deserialize a model from the binary format produced by serialize().
/// Returns a default (empty) model if the blob is malformed or too short.
TransitionModel TransitionModel::deserialize(const uint8_t* data, size_t length) {
    constexpr size_t BUCKET_SIZE = 4 + 4 + TransitionModel::NUM_TYPES * 2;
    constexpr size_t HEADER_SIZE = 8;
    constexpr size_t EXPECTED = HEADER_SIZE + MAX_CONTEXTS * BUCKET_SIZE;

    TransitionModel model;

    if (length < EXPECTED)
        return model;
    if (std::memcmp(data, "STM1", 4) != 0)
        return model;

    const uint8_t* p = data + HEADER_SIZE;
    for (size_t i = 0; i < MAX_CONTEXTS; ++i) {
        auto& b = model.table_[i];
        std::memcpy(&b.context_signature, p, 4);
        p += 4;
        std::memcpy(&b.total, p, 4);
        p += 4;
        std::memcpy(b.counts, p, TransitionModel::NUM_TYPES * 2);
        p += TransitionModel::NUM_TYPES * 2;
    }

    return model;
}

// ─── Merge ───────────────────────────────────────────────────────────────────
//
// Combines per-thread models after parallel NDJSON processing.
// For each bucket: if empty locally, take other's data; if same context,
// add counts (saturating at uint16 max); if different contexts, keep the
// one with more observations (lossy semantics).

void TransitionModel::merge(const TransitionModel& other) noexcept {
    for (size_t i = 0; i < MAX_CONTEXTS; ++i) {
        const auto& ob = other.table_[i];
        if (ob.total == 0)
            continue;

        auto& b = table_[i];
        if (b.total == 0) {
            // Empty local bucket — take the other's data wholesale
            b = ob;
        } else if (b.context_signature == ob.context_signature) {
            // Same context — add counts
            for (size_t j = 0; j < NUM_TYPES; ++j) {
                uint32_t sum = static_cast<uint32_t>(b.counts[j]) + ob.counts[j];
                b.counts[j] = static_cast<uint16_t>(std::min<uint32_t>(sum, UINT16_MAX));
            }
            b.total += ob.total;
        }
        // Different signatures: keep the one with more observations (lossy)
        else if (ob.total > b.total) {
            b = ob;
        }
    }

    // Merge stats
    stats_.total_predictions += other.stats_.total_predictions;
    stats_.correct_predictions += other.stats_.correct_predictions;
    stats_.speculations_attempted += other.stats_.speculations_attempted;
    stats_.speculations_hit += other.stats_.speculations_hit;
}

} // namespace strata::speculative

/**
 * @file schema_filter.cpp
 * @brief Implementation of the schema validation Bloom filter.
 */

#include "strata/bloom/schema_filter.h"

#include <algorithm>

namespace strata {
namespace bloom {

namespace {
struct StrViewCmp {
    bool operator()(const std::string& a, std::string_view b) const noexcept { return a < b; }
    bool operator()(std::string_view a, const std::string& b) const noexcept { return a < b; }
};
} // namespace

// ─── Construction from flat key list ─────────────────────────────────────────

SchemaFilter::SchemaFilter(std::span<const std::string_view> allowed_keys) {
    all_keys_.reserve(allowed_keys.size());
    for (auto key : allowed_keys) {
        global_filter_.insert(key);
        all_keys_.emplace_back(key);
    }
    // Sort for binary search in exact validation.
    std::sort(all_keys_.begin(), all_keys_.end());
    all_keys_.erase(std::unique(all_keys_.begin(), all_keys_.end()), all_keys_.end());
}

// ─── Construction from JSON Schema ───────────────────────────────────────────

SchemaFilter::SchemaFilter(std::string_view json_schema) { parse_schema(json_schema); }

void SchemaFilter::parse_schema(std::string_view js) {
    // Strategy: find every occurrence of "properties" : { ... } in the schema.
    // For each one, extract the top-level keys of that object.
    // Depth estimation: count how many "properties" keywords precede this one
    // in the nesting chain.

    constexpr std::string_view kPropKey = "\"properties\"";
    size_t search_pos = 0;

    while (search_pos < js.size()) {
        auto prop_pos = js.find(kPropKey, search_pos);
        if (prop_pos == std::string_view::npos)
            break;

        // Advance past the keyword to find ':' then '{'.
        size_t pos = prop_pos + kPropKey.size();

        // Skip whitespace and colon.
        while (pos < js.size() && (js[pos] == ' ' || js[pos] == '\t' || js[pos] == '\n' ||
                                   js[pos] == '\r' || js[pos] == ':'))
            ++pos;

        if (pos >= js.size() || js[pos] != '{') {
            search_pos = prop_pos + 1;
            continue;
        }

        // Estimate schema depth: count "properties" occurrences in the text
        // before this one that are still "open" (their braces aren't closed).
        // Simple heuristic: count nesting of "properties" keywords.
        uint8_t depth = 0;
        {
            // Depth = number of "properties" keywords before this one in the text.
            // Root properties is depth 0, nested is depth 1, etc.
            size_t dp = 0;
            size_t sp = 0;
            while ((sp = js.find(kPropKey, sp)) != std::string_view::npos && sp < prop_pos) {
                ++dp;
                sp += kPropKey.size();
            }
            depth = static_cast<uint8_t>(dp);
        }

        ++pos; // skip '{'

        // Extract immediate keys from this properties object.
        // Track brace depth: keys at depth 1 are the property names.
        int brace_depth = 1;
        while (pos < js.size() && brace_depth > 0) {
            char c = js[pos];

            if (c == '{') {
                ++brace_depth;
                ++pos;
            } else if (c == '}') {
                --brace_depth;
                ++pos;
            } else if (c == '"') {
                // Read the string.
                ++pos; // skip opening '"'
                size_t str_start = pos;
                while (pos < js.size() && js[pos] != '"') {
                    if (js[pos] == '\\')
                        ++pos; // skip escaped char
                    ++pos;
                }
                size_t str_end = pos;
                if (pos < js.size())
                    ++pos; // skip closing '"'

                // If at brace_depth 1 and followed by ':', this is a property key.
                if (brace_depth == 1) {
                    // Check if ':' follows (skip whitespace).
                    size_t check = pos;
                    while (check < js.size() && (js[check] == ' ' || js[check] == '\t' ||
                                                 js[check] == '\n' || js[check] == '\r'))
                        ++check;

                    if (check < js.size() && js[check] == ':') {
                        std::string_view key = js.substr(str_start, str_end - str_start);
                        global_filter_.insert(key);
                        all_keys_.emplace_back(key);
                        if (depth < MAX_DEPTH) {
                            depth_filters_[depth].insert(key);
                            depth_keys_[depth].emplace_back(key);
                            if (depth >= max_depth_)
                                max_depth_ = depth + 1;
                        }
                    }
                }
            } else {
                ++pos;
            }
        }

        search_pos = prop_pos + 1; // continue searching for more "properties"
    }

    // Sort and deduplicate all key lists.
    std::sort(all_keys_.begin(), all_keys_.end());
    all_keys_.erase(std::unique(all_keys_.begin(), all_keys_.end()), all_keys_.end());
    for (uint8_t d = 0; d < max_depth_; ++d) {
        std::sort(depth_keys_[d].begin(), depth_keys_[d].end());
        depth_keys_[d].erase(std::unique(depth_keys_[d].begin(), depth_keys_[d].end()),
                             depth_keys_[d].end());
    }
}

// ─── Key addition at specific depth ──────────────────────────────────────────

void SchemaFilter::add_keys_at_depth(std::span<const std::string_view> keys, uint8_t depth) {
    for (auto key : keys) {
        global_filter_.insert(key);
        all_keys_.emplace_back(key);
        if (depth < MAX_DEPTH) {
            depth_filters_[depth].insert(key);
            depth_keys_[depth].emplace_back(key);
            if (depth >= max_depth_)
                max_depth_ = depth + 1;
        }
    }
    // Re-sort all key lists.
    std::sort(all_keys_.begin(), all_keys_.end());
    all_keys_.erase(std::unique(all_keys_.begin(), all_keys_.end()), all_keys_.end());
    for (uint8_t d = 0; d < max_depth_; ++d) {
        std::sort(depth_keys_[d].begin(), depth_keys_[d].end());
        depth_keys_[d].erase(std::unique(depth_keys_[d].begin(), depth_keys_[d].end()),
                             depth_keys_[d].end());
    }
}

// ─── Validation ──────────────────────────────────────────────────────────────

uint16_t SchemaFilter::short_hash(std::string_view key) noexcept {
    // Use wyhash with a fixed seed and truncate to 16 bits.
    auto h = detail::wyhash(reinterpret_cast<const uint8_t*>(key.data()), key.size(),
                            0x1234567890abcdefULL);
    return static_cast<uint16_t>(h ^ (h >> 16) ^ (h >> 32) ^ (h >> 48));
}

SchemaFilter::ValidationResult SchemaFilter::validate(std::string_view key) const noexcept {
    uint16_t kh = short_hash(key);

    if (!global_filter_.maybe_contains(key)) {
        return {false, false, kh};
    }

    // Bloom says maybe — exact check needed.
    bool exact = std::binary_search(all_keys_.begin(), all_keys_.end(), key, StrViewCmp{});

    return {exact, !exact, kh};
}

uint64_t SchemaFilter::validate_batch(const std::string_view* keys, size_t count) const noexcept {
    // Process keys sequentially using the Bloom filter.
    // Each bit in the result corresponds to a key that passed (might be valid).
    // count must be ≤ 64.
    if (count > 64)
        count = 64;

    uint64_t mask = 0;
    for (size_t i = 0; i < count; ++i) {
        if (global_filter_.maybe_contains(keys[i])) {
            mask |= (uint64_t{1} << i);
        }
    }
    return mask;
}

SchemaFilter::ValidationResult SchemaFilter::validate_at_depth(std::string_view key,
                                                               uint8_t depth) const noexcept {
    uint16_t kh = short_hash(key);

    // If we have a depth-specific filter and depth is in range, use it.
    if (depth < max_depth_) {
        if (!depth_filters_[depth].maybe_contains(key)) {
            return {false, false, kh};
        }
        // Bloom positive — do exact check against per-depth key list.
        bool exact = std::binary_search(depth_keys_[depth].begin(), depth_keys_[depth].end(), key,
                                        StrViewCmp{});
        return {exact, !exact, kh};
    }

    // Fall back to global filter for depths beyond our per-depth filters.
    if (!global_filter_.maybe_contains(key)) {
        return {false, false, kh};
    }

    return {true, true, kh};
}

} // namespace bloom
} // namespace strata

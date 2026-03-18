/**
 * @file key_filter.cpp
 * @brief Implementation of the selective key extraction filter.
 */

#include "strata/bloom/key_filter.h"

#include <algorithm>

namespace strata {
namespace bloom {

namespace {
/// Heterogeneous comparator for binary_search with string/string_view mix.
struct StrViewCmp {
    bool operator()(const std::string& a, std::string_view b) const noexcept { return a < b; }
    bool operator()(std::string_view a, const std::string& b) const noexcept { return a < b; }
};
} // namespace

KeyFilter::KeyFilter(std::span<const std::string_view> desired_keys) {
    desired_keys_.reserve(desired_keys.size());
    for (auto sv : desired_keys) {
        bloom_.insert(sv);
        desired_keys_.emplace_back(sv);
    }
    // Sort for binary search in exact_match().
    std::sort(desired_keys_.begin(), desired_keys_.end());
    // Remove duplicates.
    desired_keys_.erase(std::unique(desired_keys_.begin(), desired_keys_.end()),
                        desired_keys_.end());
}

KeyFilter KeyFilter::from_paths(std::span<const std::string_view> paths) {
    // Extract the leaf field name from each JSONPath expression.
    // Examples:
    //   "$.user.name"        → "name"
    //   "$.items[*].price"   → "price"
    //   "$.data"             → "data"
    //   "name"               → "name" (bare field name)
    std::vector<std::string_view> keys;
    keys.reserve(paths.size());

    for (auto path : paths) {
        // Find the last '.' that isn't inside brackets.
        auto pos = path.rfind('.');
        if (pos != std::string_view::npos) {
            auto leaf = path.substr(pos + 1);
            // Strip trailing array subscript like "[*]" or "[0]"
            auto bracket = leaf.find('[');
            if (bracket != std::string_view::npos) {
                leaf = leaf.substr(0, bracket);
            }
            if (!leaf.empty()) {
                keys.push_back(leaf);
            }
        } else {
            // No dot — treat the whole thing as a field name.
            // Strip leading "$" if present.
            if (!path.empty() && path[0] == '$') {
                path = path.substr(1);
            }
            if (!path.empty()) {
                keys.push_back(path);
            }
        }
    }

    return KeyFilter(keys);
}

KeyFilter::Action KeyFilter::check(std::string_view key) const noexcept {
    ++stats_.total_checks;

    if (desired_keys_.empty()) {
        // No filter set — parse everything.
        return Action::PARSE;
    }

    if (!bloom_.maybe_contains(key)) {
        ++stats_.bloom_rejects;
        return Action::SKIP;
    }

    ++stats_.bloom_positives;
    return Action::UNCERTAIN;
}

bool KeyFilter::exact_match(std::string_view key) const noexcept {
    // Binary search on sorted desired_keys_.
    return std::binary_search(desired_keys_.begin(), desired_keys_.end(), key, StrViewCmp{});
}

bool KeyFilter::should_parse(std::string_view key) const noexcept {
    ++stats_.total_checks;

    if (desired_keys_.empty()) {
        return true; // No filter — parse everything.
    }

    if (!bloom_.maybe_contains(key)) {
        ++stats_.bloom_rejects;
        return false;
    }

    ++stats_.bloom_positives;

    // Bloom says maybe — do exact check.
    bool found = std::binary_search(desired_keys_.begin(), desired_keys_.end(), key, StrViewCmp{});

    if (found) {
        ++stats_.exact_matches;
    } else {
        ++stats_.false_positives;
    }

    return found;
}

} // namespace bloom
} // namespace strata

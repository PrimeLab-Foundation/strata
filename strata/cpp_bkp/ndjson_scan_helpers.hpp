/**
 * ndjson_scan_helpers.hpp - Utilities for fast NDJSON scanning and value extraction
 *
 * This header provides lightweight parsing helpers for common NDJSON query patterns.
 * These functions implement fast-path scanning for specific JSONPath expressions without
 * full DOM parsing, optimized for throughput-critical NDJSON processing.
 *
 * Design goals:
 * - Zero-copy: Operate on input buffer directly
 * - Fast-path: Specialized for common patterns ($.id, $..price, etc.)
 * - Fallback-friendly: Work alongside generic JSONPath executor
 * - Testable: Extracted from inline lambdas for unit testing
 */

#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace strata {
namespace ndjson {

/// View into a buffer region (offset + length)
struct View {
    size_t offset;
    size_t length;
};

// ============================================================================
// Low-Level Scanning Primitives
// ============================================================================

/**
 * Check if character is JSON whitespace
 */
[[nodiscard]] inline constexpr bool is_space(char c) noexcept {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/**
 * Scan past a JSON string in the buffer
 *
 * @param p    Buffer pointer
 * @param n    Buffer length
 * @param i    Starting index (must point to opening quote)
 * @return     Index after closing quote, or n if unterminated
 *
 * @pre p[i] == '"'
 * @note Handles escape sequences correctly
 */
[[nodiscard]] inline size_t scan_string(const char* p, size_t n, size_t i) noexcept {
    // Precondition: p[i] == '"'
    bool esc = false;
    ++i; // Move past opening quote

    for (; i < n; ++i) {
        char c = p[i];
        if (esc) {
            esc = false;
            continue;
        }
        if (c == '\\') {
            esc = true;
            continue;
        }
        if (c == '"') {
            return i + 1; // After closing quote
        }
    }
    return n; // Unterminated string
}

/**
 * Scan a complete JSON value and return its span
 *
 * Handles objects, arrays, strings, and primitives.
 * Does not validate JSON correctness (assumes well-formed input).
 *
 * @param p  Buffer pointer
 * @param n  Buffer length
 * @param i  Starting index (whitespace is skipped)
 * @return   Pair of (start_index, end_index) where end is one past last char
 *
 * @note Returns (i, i) if no value found
 * @note End index may be > n for unterminated values
 */
[[nodiscard]] inline std::pair<size_t, size_t> scan_value(const char* p, size_t n,
                                                          size_t i) noexcept {
    // Skip leading whitespace
    while (i < n && is_space(p[i])) {
        ++i;
    }

    size_t start = i;
    if (i >= n) {
        return {start, start};
    }

    char c = p[i];

    // String
    if (c == '"') {
        size_t end = scan_string(p, n, i);
        return {start, end};
    }

    // Object
    if (c == '{') {
        size_t depth = 0;
        bool in_str = false;
        bool esc = false;

        for (size_t j = i; j < n; ++j) {
            char ch = p[j];

            if (in_str) {
                if (esc) {
                    esc = false;
                } else if (ch == '\\') {
                    esc = true;
                } else if (ch == '"') {
                    in_str = false;
                }
                continue;
            }

            if (ch == '"') {
                in_str = true;
            } else if (ch == '{') {
                ++depth;
            } else if (ch == '}') {
                --depth;
                if (depth == 0) {
                    return {start, j + 1};
                }
            }
        }
        return {start, n}; // Unterminated object
    }

    // Array
    if (c == '[') {
        size_t depth = 0;
        bool in_str = false;
        bool esc = false;

        for (size_t j = i; j < n; ++j) {
            char ch = p[j];

            if (in_str) {
                if (esc) {
                    esc = false;
                } else if (ch == '\\') {
                    esc = true;
                } else if (ch == '"') {
                    in_str = false;
                }
                continue;
            }

            if (ch == '"') {
                in_str = true;
            } else if (ch == '[') {
                ++depth;
            } else if (ch == ']') {
                --depth;
                if (depth == 0) {
                    return {start, j + 1};
                }
            }
        }
        return {start, n}; // Unterminated array
    }

    // Primitive (number, true, false, null)
    size_t j = i;
    for (; j < n; ++j) {
        char ch = p[j];
        if (ch == ',' || ch == '}' || ch == ']' || ch == '\n' || ch == '\r') {
            break;
        }
        if (is_space(ch)) {
            break;
        }
    }
    return {start, j};
}

// ============================================================================
// High-Level Query Helpers
// ============================================================================

/**
 * Find all values for a given key at any depth in the JSON
 *
 * Example: find_key_values(buffer, len, "price", results)
 * Matches: {"price": 10}, {"items": [{"price": 20}]}, etc.
 *
 * @param p        Buffer pointer
 * @param n        Buffer length
 * @param key      Key name to search for
 * @param results  Output vector of Views (appended to)
 *
 * @note Searches at any nesting depth (recursive descent)
 * @note Results are Views relative to buffer start (p)
 */
inline void find_key_values_any_depth(const char* p, size_t n, std::string_view key,
                                      std::vector<View>& results) {
    bool in_str = false;
    bool esc = false;
    size_t i = 0;

    while (i < n) {
        char c = p[i];

        if (in_str) {
            if (esc) {
                esc = false;
            } else if (c == '\\') {
                esc = true;
            } else if (c == '"') {
                in_str = false;
            }
            ++i;
            continue;
        }

        if (c == '"') {
            // Start of a potential key
            size_t key_start = i + 1;
            size_t j = scan_string(p, n, i);
            size_t key_end = j - 1; // Before closing quote

            std::string_view found_key(p + key_start, key_end - key_start);
            i = j;

            // Skip whitespace after key
            while (i < n && is_space(p[i])) {
                ++i;
            }

            // Check for colon (key-value pair)
            if (i < n && p[i] == ':') {
                ++i; // Move past colon

                // Check if this is the key we're looking for
                if (found_key == key) {
                    auto [val_start, val_end] = scan_value(p, n, i);
                    if (val_end > val_start) {
                        results.push_back(View{val_start, val_end - val_start});
                    }
                }
                continue;
            }
        }

        ++i;
    }
}

/**
 * Parse simple integer from buffer range
 *
 * @param p      Buffer pointer
 * @param start  Start index
 * @param end    End index (exclusive)
 * @param out    Output integer
 * @return       true if parsed successfully, false otherwise
 *
 * @note Skips leading whitespace
 * @note Handles negative numbers
 * @note Does not validate numeric correctness (assumes valid JSON)
 */
[[nodiscard]] inline bool parse_int(const char* p, size_t start, size_t end, int& out) noexcept {
    // Skip leading whitespace
    while (start < end && is_space(p[start])) {
        ++start;
    }

    if (start >= end) {
        return false;
    }

    int sign = 1;
    if (p[start] == '-') {
        sign = -1;
        ++start;
    }

    int value = 0;
    bool any_digit = false;

    for (size_t i = start; i < end; ++i) {
        char ch = p[i];
        if (ch >= '0' && ch <= '9') {
            any_digit = true;
            value = value * 10 + (ch - '0');
        } else {
            break; // Stop at first non-digit
        }
    }

    if (!any_digit) {
        return false;
    }

    out = sign * value;
    return true;
}

} // namespace ndjson
} // namespace strata

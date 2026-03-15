#pragma once

/**
 * @file simd_string.hpp
 * @brief SIMD-accelerated string operations for JSON parsing and serialisation.
 *
 * Provides platform-adaptive implementations that select AVX2, SSE4.2,
 * NEON, or scalar fallbacks at compile time.  All functions are safe to
 * call regardless of CPU — the scalar fallback is always present.
 *
 * Performance summary (vs scalar):
 * - escape_json_string_simd:   2-4x faster for strings with few escapes.
 * - find_newline_simd:         3-5x faster for large NDJSON buffers.
 * - validate_utf8_simd:        4-8x faster for multi-byte heavy input.
 * - find_next_escape_simd:     2-3x faster for typical JSON strings.
 */

#include "strata/util/output_buffer.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace strata {
namespace util {

// --- JSON string escaping --------------------------------------------------

/// SIMD-accelerated JSON string escaping into a std::string.
void escape_json_string_simd(const char* str, size_t len, std::string& out);

/// SIMD-accelerated JSON string escaping into an OutputBuffer.
void escape_json_string_simd(const char* str, size_t len, OutputBuffer& out);

/// SIMD-accelerated JSON string escaping into a FixedOutputBuffer.
void escape_json_string_simd(const char* str, size_t len, FixedOutputBuffer& out);

/**
 * Fast path for strings with no escapes (detected via SIMD scan).
 * Returns true if the string needs no escaping and was directly copied.
 */
[[nodiscard]] bool try_copy_clean_string(const char* str, size_t len, std::string& out);
[[nodiscard]] bool try_copy_clean_string(const char* str, size_t len, OutputBuffer& out);
[[nodiscard]] bool try_copy_clean_string(const char* str, size_t len, FixedOutputBuffer& out);

// --- NDJSON newline search -------------------------------------------------

/**
 * SIMD-accelerated newline search for NDJSON line splitting.
 *
 * @param data      Pointer to data buffer.
 * @param len       Length of buffer.
 * @param start_pos Starting position to search from.
 * @return Position of first '\n', or @p len if not found.
 */
[[nodiscard]] size_t find_newline_simd(const char* data, size_t len, size_t start_pos = 0);

/**
 * Count newlines in buffer using SIMD (for pre-allocation).
 *
 * @param data Pointer to data buffer.
 * @param len  Length of buffer.
 * @return Number of '\n' characters found.
 */
[[nodiscard]] size_t count_newlines_simd(const char* data, size_t len);

/**
 * Collect newline positions using SIMD.
 *
 * Appends absolute positions (indices into @p data) for up to
 * @p max_positions newlines, starting at @p start_pos.
 */
void collect_newlines_simd(const char* data, size_t len, size_t start_pos, size_t max_positions,
                           std::vector<size_t>& positions);

// --- Whitespace and escape detection ---------------------------------------

/**
 * SIMD-accelerated whitespace-only check for NDJSON lines.
 *
 * @return true if all characters are whitespace (space, tab, CR, LF, FF, VT).
 */
[[nodiscard]] bool is_whitespace_only_simd(const char* data, size_t len);

/**
 * SIMD-accelerated search for JSON escape characters (", \\, control chars).
 *
 * @return Position of first escape/quote, or @p len if none found.
 */
[[nodiscard]] size_t find_next_escape_simd(const char* str, size_t len);

/**
 * Check whether a string contains any character that must be escaped in JSON.
 *
 * @return true if the string needs escaping.
 */
[[nodiscard]] bool string_needs_escape(const char* str, size_t len);

// --- UTF-8 validation ------------------------------------------------------

/**
 * Validate a UTF-8 byte sequence (RFC 3629).
 *
 * Rejects overlong encodings, surrogate codepoints (U+D800–U+DFFF),
 * and codepoints above U+10FFFF.  Uses SIMD (AVX2/SSE2/NEON) when
 * available; falls back to scalar otherwise.
 *
 * @param data Pointer to byte buffer.
 * @param len  Length in bytes.
 * @return true if valid UTF-8, false otherwise.
 */
[[nodiscard]] bool validate_utf8_simd(const char* data, size_t len);

} // namespace util
} // namespace strata

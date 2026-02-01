#pragma once

#include "strata/util/output_buffer.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace strata {
namespace util {

/**
 * SIMD-accelerated JSON string escaping.
 *
 * Uses AVX2 (32 bytes) or SSE4.2 (16 bytes) for parallel character scanning.
 * Falls back to scalar processing when SIMD is not available.
 *
 * Performance: 2-4x faster than scalar for strings with few escapes,
 * 20-50% faster for strings with many escapes.
 */
void escape_json_string_simd(const char* str, size_t len, std::string& out);

void escape_json_string_simd(const char* str, size_t len, OutputBuffer& out);
void escape_json_string_simd(const char* str, size_t len, FixedOutputBuffer& out);

/**
 * Fast path for strings with no escapes (detected via SIMD scan).
 * Returns true if string needs no escaping and was directly copied.
 */
bool try_copy_clean_string(const char* str, size_t len, std::string& out);

bool try_copy_clean_string(const char* str, size_t len, OutputBuffer& out);
bool try_copy_clean_string(const char* str, size_t len, FixedOutputBuffer& out);

/**
 * SIMD-accelerated newline search for NDJSON parsing.
 *
 * Uses AVX2/SSE4.2/NEON for parallel scanning of '\n' characters.
 * Falls back to scalar processing when SIMD is not available.
 *
 * Performance: 3-5x faster than scalar for large buffers.
 *
 * @param data Pointer to data buffer
 * @param len Length of buffer
 * @param start_pos Starting position to search from
 * @return Position of first newline, or len if not found
 */
size_t find_newline_simd(const char* data, size_t len, size_t start_pos = 0);

/**
 * Count newlines in buffer using SIMD (for pre-allocation).
 *
 * @param data Pointer to data buffer
 * @param len Length of buffer
 * @return Number of newlines found
 */
size_t count_newlines_simd(const char* data, size_t len);

/**
 * Collect newline positions in buffer using SIMD.
 *
 * Appends absolute positions (indices into data) for up to max_positions
 * newlines, starting at start_pos.
 */
void collect_newlines_simd(const char* data, size_t len, size_t start_pos, size_t max_positions,
                           std::vector<size_t>& positions);

/**
 * SIMD-accelerated whitespace-only check for NDJSON lines.
 *
 * Returns true if all characters are whitespace (space, tab, CR, LF, FF, VT).
 */
bool is_whitespace_only_simd(const char* data, size_t len);

/**
 * SIMD-accelerated search for JSON escape characters (", \, control chars).
 * Returns position of first escape/quote, or len if none found.
 */
size_t find_next_escape_simd(const char* str, size_t len);

/**
 * Validate UTF-8 byte sequence (RFC 3629).
 *
 * Rejects overlong encodings, surrogate codepoints (U+D800–U+DFFF),
 * and codepoints above U+10FFFF. Uses SIMD (AVX2/SSE2/NEON) when
 * available; falls back to scalar otherwise.
 *
 * @param data Pointer to byte buffer
 * @param len Length in bytes
 * @return true if valid UTF-8, false otherwise
 */
bool validate_utf8_simd(const char* data, size_t len);

} // namespace util
} // namespace strata

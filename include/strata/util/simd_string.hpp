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
 * SIMD-accelerated whitespace skip for JSON parsing.
 *
 * Skips JSON whitespace characters: space, tab, LF, CR.
 * Returns position of first non-whitespace character.
 */
size_t skip_whitespace_simd(const char* data, size_t len, size_t start_pos = 0);

/**
 * SIMD-accelerated search for JSON escape characters (", \, control chars).
 * Returns position of first escape/quote, or len if none found.
 */
size_t find_next_escape_simd(const char* str, size_t len);

/**
 * Single-pass string escaping/copying with SIMD acceleration.
 *
 * Combines escape detection and string output in a single operation:
 * - First finds the position of the first escape character using SIMD
 * - If no escapes: copies the string directly (fast path)
 * - If escapes found: copies clean prefix, then escapes from that position
 *
 * This avoids the double-scan pattern of try_copy_clean_string + escape_json_string_simd.
 *
 * @param str Input string data
 * @param len Length of input string
 * @param out Output buffer for the quoted JSON string (function adds quotes)
 */
void escape_or_copy_string_simd(const char* str, size_t len, std::string& out);
void escape_or_copy_string_simd(const char* str, size_t len, OutputBuffer& out);
void escape_or_copy_string_simd(const char* str, size_t len, FixedOutputBuffer& out);

/**
 * SIMD-accelerated search for next JSON structural character
 * ('{', '}', '[', ']', ',', ':') skipping over whitespace.
 *
 * @param data Pointer to buffer
 * @param len Buffer length
 * @param start_pos Starting index to search from
 * @return Index of first non-whitespace byte; len if none remain.
 *         Caller can read data[pos] to inspect the structural char; if the
 *         returned byte is not structural, the input contained an unexpected
 *         non-whitespace character.
 */
size_t find_next_structural_simd(const char* data, size_t len, size_t start_pos = 0);

/**
 * Collect positions of JSON structural characters using SIMD classification.
 *
 * Structural characters: '{', '}', '[', ']', ',', ':', '"'
 * Fills positions with absolute indices into data.
 */
void collect_structural_positions_simd(const char* data, size_t len,
                                       std::vector<size_t>& positions);

/**
 * Check if buffer contains only ASCII characters (bytes < 0x80).
 * Uses SIMD for fast scanning of large buffers.
 *
 * @param data Pointer to byte buffer
 * @param len Length in bytes
 * @return true if all bytes are ASCII (< 0x80), false otherwise
 */
bool is_ascii_only_simd(const char* data, size_t len);

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

/**
 * Validate UTF-8 lazily - only perform full validation if non-ASCII detected.
 *
 * For ASCII-only content (most common in JSON), this is faster than full validation.
 * Uses SIMD to first check for high bytes, then validates only if needed.
 *
 * @param data Pointer to byte buffer
 * @param len Length in bytes
 * @return true if valid (ASCII-only or valid UTF-8), false if invalid UTF-8
 */
bool validate_utf8_lazy(const char* data, size_t len);

} // namespace util
} // namespace strata

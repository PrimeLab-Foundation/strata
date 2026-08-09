#pragma once

/**
 * @file scan.hpp
 * @brief Byte-level scanning primitives shared by the parser.
 *
 * All three are scalar today. They are deliberately *not* named for an
 * implementation strategy: vectorised versions replace the bodies during the
 * performance milestone, behind the same names and with identical observable
 * behaviour (docs/context/styleguide.md, the SIMD/scalar-twin rule). The
 * previous implementation named these `simd_*` and then shipped scalar code
 * inside them, which the architecture notes flag as misleading.
 */

#include <cstddef>

namespace strata::util {

/**
 * Validate that @p data is well-formed UTF-8.
 *
 * Strict: rejects overlong encodings, truncated sequences, surrogate halves
 * (U+D800–U+DFFF) and anything above U+10FFFF. This is the only UTF-8 check
 * the engine performs, and for `bytes` input it is the only validator there
 * is (docs/context/api.md).
 */
[[nodiscard]] bool validate_utf8(const char* data, size_t len) noexcept;

/**
 * Advance past JSON whitespace (space, tab, CR, LF) starting at @p pos.
 *
 * @return Index of the first non-whitespace byte, or @p len at end of input.
 */
[[nodiscard]] size_t skip_whitespace(const char* data, size_t len, size_t pos) noexcept;

/**
 * Find the first byte that ends a string's "plain" run.
 *
 * Scans for a double quote, a backslash, or a control character below 0x20 —
 * every byte that forces the parser off its zero-copy path.
 *
 * @return Offset of that byte, or @p len when the run reaches the end.
 */
[[nodiscard]] size_t find_next_escape(const char* data, size_t len) noexcept;

/**
 * The scalar definition of @ref find_next_escape.
 *
 * `find_next_escape` uses SIMD where the target has it, and the styleguide
 * requires every such function to keep a scalar twin with identical
 * observable behaviour. Exposed so that requirement is *checked* rather than
 * asserted: `tests/cpp/test_scan.cpp` runs the two against each other over
 * every alignment and every byte value.
 */
[[nodiscard]] size_t find_next_escape_scalar(const char* data, size_t len) noexcept;

} // namespace strata::util

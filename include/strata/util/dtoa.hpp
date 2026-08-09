#pragma once

/**
 * @file dtoa.hpp
 * @brief Rendering doubles as the JSON text that reads back identically.
 *
 * The output is the *shortest* decimal that round-trips to the same double,
 * laid out the way Python renders floats: fixed notation while the decimal
 * exponent stays in a comfortable range, scientific outside it, and always a
 * fraction so a float never reads back as an integer.
 *
 * Named for what it produces, not for an algorithm. The previous
 * implementation shipped three overlapping converters whose names had come
 * loose from their contents — a `dragonbox.cpp` that was really Ryu, a
 * `ryu_dtoa.cpp` that was really a `std::to_chars` wrapper, and a `fast_dtoa`
 * with no callers at all (docs/architecture/SKILL.md, dtoa naming warning).
 * There is one converter here, and swapping its internals for a faster one is
 * a change no caller can observe.
 */

#include <cstddef>
#include <cstdint>

namespace strata::util {

/// Longest output format_double() can produce, plus room to spare.
inline constexpr size_t kDoubleBufferSize = 40;

/**
 * Render @p value into @p out as shortest round-trip JSON text.
 *
 * Non-finite values are *not* handled here: JSON has no NaN or infinity, and
 * choosing what to emit in their place is the serializer's policy, not this
 * function's. Callers must filter them first.
 *
 * @param out Buffer of at least @ref kDoubleBufferSize bytes. Not terminated.
 * @return Number of bytes written, or 0 if the buffer was too small.
 */
[[nodiscard]] size_t format_double(double value, char* out, size_t capacity) noexcept;

/// Longest output format_int64() can produce ("-9223372036854775808").
inline constexpr size_t kInt64BufferSize = 20;

/**
 * Render @p value as decimal digits into @p out. Not terminated.
 *
 * Two digits per step through a 200-byte pair table — the classic itoa that
 * beats `to_chars`' generic machinery by roughly half on the short integers
 * JSON is made of. Behaviour is identical to `to_chars`; the digit-for-digit
 * equivalence is pinned by test over boundaries and a large random sweep.
 *
 * @param out Buffer of at least @ref kInt64BufferSize bytes.
 * @return Number of bytes written.
 */
[[nodiscard]] size_t format_int64(int64_t value, char* out) noexcept;

} // namespace strata::util

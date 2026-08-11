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
#include <cstdint>
#include <cstring>

// Compile-time SIMD selection (docs/context/styleguide.md). There is no
// runtime dispatch: the extension is built with -march=native, and a wheel
// built for a baseline target simply uses the scalar twin.
#if defined(__ARM_NEON)
#include <arm_neon.h>
#define STRATA_ESCAPE_SCAN_SIMD 1
#elif defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64)
// MSVC never defines __SSE2__; x64 implies it.
#include <emmintrin.h>
#define STRATA_ESCAPE_SCAN_SIMD 1
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

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
 * The scalar twin of @ref validate_utf8 (docs/context/styleguide.md): one
 * byte at a time, no bulk-ASCII tier. `tests/cpp/test_scan.cpp` checks the
 * two agree over adversarial inputs. The multi-byte sequence rules are one
 * shared definition inside scan.cpp, so only the ASCII traversal can differ.
 */
[[nodiscard]] bool validate_utf8_scalar(const char* data, size_t len) noexcept;

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

/**
 * Copy bytes from @p src to @p dst until the first escape-forcing byte.
 *
 * The single-pass form of scan-then-memcpy: the serializer's clean-string
 * fast path reads each byte once instead of twice. Stops at exactly the byte
 * @ref find_next_escape would report; bytes at and beyond it are unspecified
 * in @p dst (the vectorized form stores whole blocks before checking).
 *
 * @param dst Must hold at least `len` bytes **rounded up to a full 16-byte
 *            block** — the caller over-reserves; src is never over-read.
 * @return Number of clean bytes copied (== `len` when nothing escapes).
 */

/// The scalar twin of @ref copy_until_escape, checked against it by test.
[[nodiscard]] size_t copy_until_escape_scalar(const char* src, size_t len, char* dst) noexcept;

// ---------------------------------------------------------------------------
// The vectorized scanners live here, in the header, so the serializer and the
// parser inline them: as out-of-line functions the call alone cost several
// nanoseconds per string, which for ten-byte strings rivaled the scan itself.
// The scalar twins stay in scan.cpp as the out-of-line reference the
// equivalence suite runs against.
// ---------------------------------------------------------------------------

namespace detail {

/// Index of the lowest set bit. Precondition: @p value != 0.
/// MSVC has no __builtin_ctz*; the intrinsic pair is its spelling.
[[nodiscard]] inline unsigned trailing_zeros64(uint64_t value) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    unsigned long index = 0;
    _BitScanForward64(&index, value);
    return static_cast<unsigned>(index);
#else
    return static_cast<unsigned>(__builtin_ctzll(value));
#endif
}

[[nodiscard]] inline unsigned trailing_zeros32(uint32_t value) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    unsigned long index = 0;
    _BitScanForward(&index, value);
    return static_cast<unsigned>(index);
#else
    return static_cast<unsigned>(__builtin_ctz(value));
#endif
}

#if defined(STRATA_ESCAPE_SCAN_SIMD)

/// Bytes examined per SIMD step.
inline constexpr size_t kEscapeBlock = 16;

/**
 * Offset of the first byte in a 16-byte block that a JSON string cannot carry
 * verbatim, or detail::kEscapeBlock if the whole block is clean.
 *
 * "Cannot carry verbatim" is exactly the scalar twin's rule: a quote, a
 * backslash, or a control byte below 0x20. UTF-8 continuation bytes are all
 * >= 0x80 and so are never flagged, which is what lets multi-byte text be
 * copied through untouched.
 */
[[nodiscard]] inline size_t first_escape_in_block(const char* data) noexcept {
#if defined(__ARM_NEON)
    const uint8x16_t block = vld1q_u8(reinterpret_cast<const uint8_t*>(data));
    const uint8x16_t interesting =
        vorrq_u8(vorrq_u8(vceqq_u8(block, vdupq_n_u8('"')), vceqq_u8(block, vdupq_n_u8('\\'))),
                 vcltq_u8(block, vdupq_n_u8(0x20)));
    // NEON has no movemask. Narrowing each 16-bit lane by 4 leaves one nibble
    // per input byte, so the whole block fits in a 64-bit word and the first
    // match is a count-trailing-zeros away.
    const uint64_t mask =
        vget_lane_u64(vreinterpret_u64_u8(vshrn_n_u16(vreinterpretq_u16_u8(interesting), 4)), 0);
    if (mask == 0)
        return detail::kEscapeBlock;
    return static_cast<size_t>(detail::trailing_zeros64(mask)) >> 2;
#else
    const __m128i block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
    const __m128i quote = _mm_cmpeq_epi8(block, _mm_set1_epi8('"'));
    const __m128i backslash = _mm_cmpeq_epi8(block, _mm_set1_epi8('\\'));
    // No unsigned compare before SSE4, so: min(b, 0x1F) == b iff b <= 0x1F.
    const __m128i control = _mm_cmpeq_epi8(_mm_min_epu8(block, _mm_set1_epi8(0x1F)), block);
    const int mask = _mm_movemask_epi8(_mm_or_si128(_mm_or_si128(quote, backslash), control));
    if (mask == 0)
        return detail::kEscapeBlock;
    return static_cast<size_t>(trailing_zeros32(static_cast<uint32_t>(mask)));
#endif
}

#endif // STRATA_ESCAPE_SCAN_SIMD

#if defined(__LITTLE_ENDIAN__) || defined(_WIN32) ||                                               \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define STRATA_ESCAPE_SCAN_SWAR 1

/**
 * Eight bytes at a time, in a register — the tier between the 16-byte SIMD
 * block and the byte loop. Most JSON strings are shorter than one SIMD block
 * (keys, names, identifiers), so without this the common case was scanned a
 * byte at a time.
 *
 * The formulas are the classic exact ones: `haszero` marks 0x80 in each byte
 * that is zero, never elsewhere; XOR against a repeated constant turns
 * has-this-value into has-zero; the borrow trick marks bytes below 0x20.
 * Little-endian only, where the first matching byte is the lowest set 0x80
 * and one count-trailing-zeros finds it.
 */
[[nodiscard]] inline uint64_t escape_mask_word(uint64_t word) noexcept {
    constexpr uint64_t kOnes = 0x0101010101010101ULL;
    constexpr uint64_t kHighs = 0x8080808080808080ULL;
    const uint64_t quotes = (word ^ (kOnes * '"'));
    const uint64_t backslashes = (word ^ (kOnes * '\\'));
    const uint64_t quote_mask = (quotes - kOnes) & ~quotes & kHighs;
    const uint64_t backslash_mask = (backslashes - kOnes) & ~backslashes & kHighs;
    const uint64_t control_mask = (word - (kOnes * 0x20)) & ~word & kHighs;
    return quote_mask | backslash_mask | control_mask;
}

/// The same exact formulas, four bytes wide — the tier for 4..7-byte strings,
/// which are common as keys and short values and were walked byte-by-byte.
[[nodiscard]] inline uint32_t escape_mask_word32(uint32_t word) noexcept {
    constexpr uint32_t kOnes = 0x01010101U;
    constexpr uint32_t kHighs = 0x80808080U;
    const uint32_t quotes = (word ^ (kOnes * '"'));
    const uint32_t backslashes = (word ^ (kOnes * '\\'));
    const uint32_t quote_mask = (quotes - kOnes) & ~quotes & kHighs;
    const uint32_t backslash_mask = (backslashes - kOnes) & ~backslashes & kHighs;
    const uint32_t control_mask = (word - (kOnes * 0x20)) & ~word & kHighs;
    return quote_mask | backslash_mask | control_mask;
}

#endif

} // namespace detail

inline size_t find_next_escape(const char* data, size_t len) noexcept {
    size_t pos = 0;
#if defined(STRATA_ESCAPE_SCAN_SIMD)
    // Two blocks per iteration halves the loop overhead on long spans — the
    // parser scans the whole remaining input through here.
    while (pos + 2 * detail::kEscapeBlock <= len) {
        const size_t first = detail::first_escape_in_block(data + pos);
        if (first != detail::kEscapeBlock)
            return pos + first;
        const size_t second = detail::first_escape_in_block(data + pos + detail::kEscapeBlock);
        if (second != detail::kEscapeBlock)
            return pos + detail::kEscapeBlock + second;
        pos += 2 * detail::kEscapeBlock;
    }
    if (pos + detail::kEscapeBlock <= len) {
        const size_t hit = detail::first_escape_in_block(data + pos);
        if (hit != detail::kEscapeBlock)
            return pos + hit;
        pos += detail::kEscapeBlock;
    }
    // Overlapped final block: everything before `pos` is proven clean, so a
    // block re-reading those bytes reports its first hit at an index >= pos
    // automatically. One vector op replaces the word-and-byte tail — without
    // ever reading past the end (the block ends exactly at `len`).
    if (len >= detail::kEscapeBlock && pos < len) {
        const size_t base = len - detail::kEscapeBlock;
        const size_t hit = detail::first_escape_in_block(data + base);
        return hit != detail::kEscapeBlock ? base + hit : len;
    }
#endif
#if defined(STRATA_ESCAPE_SCAN_SWAR)
    for (; pos + 8 <= len; pos += 8) {
        uint64_t word;
        std::memcpy(&word, data + pos, 8); // the only sanctioned type pun
        const uint64_t mask = detail::escape_mask_word(word);
        if (mask != 0)
            return pos + (static_cast<size_t>(detail::trailing_zeros64(mask)) >> 3);
    }
    // The same overlap trick one size down, for 8..15-byte strings.
    if (len >= 8 && pos < len) {
        uint64_t word;
        std::memcpy(&word, data + len - 8, 8);
        const uint64_t mask = detail::escape_mask_word(word);
        return mask != 0 ? (len - 8) + (static_cast<size_t>(detail::trailing_zeros64(mask)) >> 3)
                         : len;
    }
    // ...and one size further down, for 4..7-byte strings.
    if (len >= 4 && pos < len) {
        uint32_t head;
        std::memcpy(&head, data, 4);
        const uint32_t head_mask = detail::escape_mask_word32(head);
        if (head_mask != 0)
            return static_cast<size_t>(detail::trailing_zeros32(head_mask)) >> 3;
        uint32_t tail;
        std::memcpy(&tail, data + len - 4, 4);
        const uint32_t tail_mask = detail::escape_mask_word32(tail);
        return tail_mask != 0
                   ? (len - 4) + (static_cast<size_t>(detail::trailing_zeros32(tail_mask)) >> 3)
                   : len;
    }
#endif
    // Under eight bytes; the twin finishes the job. Reading past the end to
    // fill a block would be faster and is what the previous implementation
    // did — it relied on CPython's allocation slack, which is a promise
    // CPython never made and which ASan rightly flags.
    return pos + find_next_escape_scalar(data + pos, len - pos);
}

inline size_t copy_until_escape(const char* src, size_t len, char* dst) noexcept {
    size_t pos = 0;
#if defined(STRATA_ESCAPE_SCAN_SIMD)
    // Whole blocks are stored before the mask is inspected — the contract
    // gives the destination block-rounded room, so a store past the first
    // escape byte is scratch the caller never reads. Two blocks per
    // iteration; then one plain block; then an overlapped final block, whose
    // re-read bytes are proven clean and whose re-stored bytes are identical.
    while (pos + 2 * detail::kEscapeBlock <= len) {
        const size_t first = detail::first_escape_in_block(src + pos);
        std::memcpy(dst + pos, src + pos, detail::kEscapeBlock);
        if (first != detail::kEscapeBlock)
            return pos + first;
        const size_t second = detail::first_escape_in_block(src + pos + detail::kEscapeBlock);
        std::memcpy(dst + pos + detail::kEscapeBlock, src + pos + detail::kEscapeBlock,
                    detail::kEscapeBlock);
        if (second != detail::kEscapeBlock)
            return pos + detail::kEscapeBlock + second;
        pos += 2 * detail::kEscapeBlock;
    }
    if (pos + detail::kEscapeBlock <= len) {
        const size_t hit = detail::first_escape_in_block(src + pos);
        std::memcpy(dst + pos, src + pos, detail::kEscapeBlock);
        if (hit != detail::kEscapeBlock)
            return pos + hit;
        pos += detail::kEscapeBlock;
    }
    if (len >= detail::kEscapeBlock && pos < len) {
        const size_t base = len - detail::kEscapeBlock;
        const size_t hit = detail::first_escape_in_block(src + base);
        std::memcpy(dst + base, src + base, detail::kEscapeBlock);
        return hit != detail::kEscapeBlock ? base + hit : len;
    }
#endif
#if defined(STRATA_ESCAPE_SCAN_SWAR)
    for (; pos + 8 <= len; pos += 8) {
        uint64_t word;
        std::memcpy(&word, src + pos, 8);
        std::memcpy(dst + pos, &word, 8);
        const uint64_t mask = detail::escape_mask_word(word);
        if (mask != 0)
            return pos + (static_cast<size_t>(detail::trailing_zeros64(mask)) >> 3);
    }
    if (len >= 8 && pos < len) {
        uint64_t word;
        std::memcpy(&word, src + len - 8, 8);
        std::memcpy(dst + len - 8, &word, 8);
        const uint64_t mask = detail::escape_mask_word(word);
        return mask != 0 ? (len - 8) + (static_cast<size_t>(detail::trailing_zeros64(mask)) >> 3)
                         : len;
    }
    if (len >= 4 && pos < len) {
        uint32_t head;
        std::memcpy(&head, src, 4);
        std::memcpy(dst, &head, 4);
        const uint32_t head_mask = detail::escape_mask_word32(head);
        if (head_mask != 0)
            return static_cast<size_t>(detail::trailing_zeros32(head_mask)) >> 3;
        uint32_t tail;
        std::memcpy(&tail, src + len - 4, 4);
        std::memcpy(dst + len - 4, &tail, 4);
        const uint32_t tail_mask = detail::escape_mask_word32(tail);
        return tail_mask != 0
                   ? (len - 4) + (static_cast<size_t>(detail::trailing_zeros32(tail_mask)) >> 3)
                   : len;
    }
#endif
    return pos + copy_until_escape_scalar(src + pos, len - pos, dst + pos);
}

} // namespace strata::util

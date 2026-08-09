/**
 * @file scan.cpp
 * @brief The parser's byte-scanning primitives.
 *
 * UTF-8 validation and whitespace skipping are scalar; the escape scan is
 * SIMD where the target has it, with a scalar twin that tests check it
 * against (docs/context/styleguide.md).
 */

#include "strata/util/scan.hpp"

#include <cstdint>
#include <cstring>

// Compile-time SIMD selection (docs/context/styleguide.md). There is no
// runtime dispatch: the extension is built with -march=native, and a wheel
// built for a baseline target simply uses the scalar twin.
#if defined(__ARM_NEON)
#include <arm_neon.h>
#define STRATA_ESCAPE_SCAN_SIMD 1
#elif defined(__SSE2__)
#include <emmintrin.h>
#define STRATA_ESCAPE_SCAN_SIMD 1
#endif

namespace strata::util {

namespace {

[[nodiscard]] inline bool is_continuation(unsigned char byte) noexcept {
    return (byte & 0xC0) == 0x80;
}

#if defined(STRATA_ESCAPE_SCAN_SIMD)

/// Bytes examined per SIMD step.
constexpr size_t kEscapeBlock = 16;

/**
 * Offset of the first byte in a 16-byte block that a JSON string cannot carry
 * verbatim, or kEscapeBlock if the whole block is clean.
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
        return kEscapeBlock;
    return static_cast<size_t>(__builtin_ctzll(mask)) >> 2;
#else
    const __m128i block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
    const __m128i quote = _mm_cmpeq_epi8(block, _mm_set1_epi8('"'));
    const __m128i backslash = _mm_cmpeq_epi8(block, _mm_set1_epi8('\\'));
    // No unsigned compare before SSE4, so: min(b, 0x1F) == b iff b <= 0x1F.
    const __m128i control = _mm_cmpeq_epi8(_mm_min_epu8(block, _mm_set1_epi8(0x1F)), block);
    const int mask = _mm_movemask_epi8(_mm_or_si128(_mm_or_si128(quote, backslash), control));
    if (mask == 0)
        return kEscapeBlock;
    return static_cast<size_t>(__builtin_ctz(static_cast<unsigned>(mask)));
#endif
}

#endif // STRATA_ESCAPE_SCAN_SIMD

} // namespace

bool validate_utf8(const char* data, size_t len) noexcept {
    const auto* bytes = reinterpret_cast<const unsigned char*>(data);
    size_t pos = 0;

    while (pos < len) {
        const unsigned char lead = bytes[pos];

        if (lead < 0x80) { // ASCII
            ++pos;
            continue;
        }

        // 0x80-0xBF are continuations with no lead; 0xC0/0xC1 would only ever
        // encode an overlong ASCII byte; 0xF5+ is beyond U+10FFFF.
        if (lead < 0xC2 || lead > 0xF4)
            return false;

        size_t extra;
        unsigned char second_min = 0x80;
        unsigned char second_max = 0xBF;

        if (lead <= 0xDF) {
            extra = 1;
        } else if (lead <= 0xEF) {
            extra = 2;
            if (lead == 0xE0)
                second_min = 0xA0; // reject overlong E0 80..9F
            else if (lead == 0xED)
                second_max = 0x9F; // reject surrogates ED A0..BF
        } else {
            extra = 3;
            if (lead == 0xF0)
                second_min = 0x90; // reject overlong F0 80..8F
            else if (lead == 0xF4)
                second_max = 0x8F; // reject above U+10FFFF
        }

        if (pos + extra >= len)
            return false; // truncated sequence

        const unsigned char second = bytes[pos + 1];
        if (second < second_min || second > second_max)
            return false;

        for (size_t offset = 2; offset <= extra; ++offset) {
            if (!is_continuation(bytes[pos + offset]))
                return false;
        }

        pos += extra + 1;
    }

    return true;
}

size_t skip_whitespace(const char* data, size_t len, size_t pos) noexcept {
    while (pos < len) {
        const char c = data[pos];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
        ++pos;
    }
    return pos;
}

size_t find_next_escape_scalar(const char* data, size_t len) noexcept {
    for (size_t pos = 0; pos < len; ++pos) {
        const unsigned char c = static_cast<unsigned char>(data[pos]);
        if (c == '"' || c == '\\' || c < 0x20)
            return pos;
    }
    return len;
}

#if defined(__LITTLE_ENDIAN__) || defined(_WIN32) ||                                               \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define STRATA_ESCAPE_SCAN_SWAR 1

namespace {

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

} // namespace
#endif

size_t find_next_escape(const char* data, size_t len) noexcept {
    size_t pos = 0;
#if defined(STRATA_ESCAPE_SCAN_SIMD)
    for (; pos + kEscapeBlock <= len; pos += kEscapeBlock) {
        const size_t hit = first_escape_in_block(data + pos);
        if (hit != kEscapeBlock)
            return pos + hit;
    }
#endif
#if defined(STRATA_ESCAPE_SCAN_SWAR)
    for (; pos + 8 <= len; pos += 8) {
        uint64_t word;
        std::memcpy(&word, data + pos, 8); // the only sanctioned type pun
        const uint64_t mask = escape_mask_word(word);
        if (mask != 0)
            return pos + (static_cast<size_t>(__builtin_ctzll(mask)) >> 3);
    }
#endif
    // Whatever remains is shorter than a word; the twin finishes the job.
    // Reading past the end to fill one more block would be faster and is what
    // the previous implementation did for short strings — it relied on
    // CPython's allocation slack, which is a promise CPython never made and
    // which ASan rightly flags.
    return pos + find_next_escape_scalar(data + pos, len - pos);
}

} // namespace strata::util

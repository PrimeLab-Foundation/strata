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
 *
 * The digit machinery in `detail` is shared by the double layout in dtoa.cpp
 * and by `format_int64`, which lives here in the header: the serializer's
 * scalar-array runs call it once per element, and on builds without LTO an
 * out-of-line itoa was a cross-TU call in the hottest loop the dumps side has.
 * `format_double` deliberately does NOT follow it into the header — see its
 * declaration below for the measured reason.
 */

#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#include <stdlib.h> // _byteswap_uint64
#endif

namespace strata::util {

/// Longest output format_double() can produce, plus room to spare.
inline constexpr size_t kDoubleBufferSize = 40;

/// Longest output format_int64() can produce ("-9223372036854775808").
inline constexpr size_t kInt64BufferSize = 20;

namespace detail {

/// "00" "01" ... "99", so digits are peeled two at a time.
inline constexpr char kDigitPairs[] = "00010203040506070809"
                                      "10111213141516171819"
                                      "20212223242526272829"
                                      "30313233343536373839"
                                      "40414243444546474849"
                                      "50515253545556575859"
                                      "60616263646566676869"
                                      "70717273747576777879"
                                      "80818283848586878889"
                                      "90919293949596979899";

/// The two ASCII digits of @p value (< 100), in reading order, at @p out.
///
/// Byte-for-byte from the table, so it says the same thing on either endian.
inline void write_digit_pair(char* out, uint32_t value) noexcept {
    std::memcpy(out, &kDigitPairs[value * 2], 2);
}

inline constexpr uint64_t kPow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL,
};

/// Index of the highest set bit. Precondition: @p value != 0.
[[nodiscard]] inline unsigned highest_set_bit(uint64_t value) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    unsigned long index = 0;
    _BitScanReverse64(&index, value);
    return static_cast<unsigned>(index);
#else
    return static_cast<unsigned>(63 - __builtin_clzll(value));
#endif
}

/// Number of decimal digits in @p value (1 for zero).
[[nodiscard]] inline size_t decimal_digit_count(uint64_t value) noexcept {
    if (value == 0)
        return 1;
    // floor(log10) from floor(log2): multiply by log10(2) in fixed point,
    // then correct by comparing against the exact power.
    const auto bits = static_cast<size_t>(highest_set_bit(value));
    size_t digits = (bits * 1233) >> 12;
    digits += static_cast<size_t>(digits + 1 <= 19 && value >= kPow10[digits + 1]);
    return digits + 1;
}

/// Store a digit word: byte k of @p word (bits 8k..8k+7) lands at out[k].
/// The digit-word helpers below build their words in that fixed layout, so
/// a big-endian target swaps once here and every byte-index computation on
/// the word stays the same on both.
inline void store_digit_word(char* out, uint64_t word) noexcept {
    if constexpr (std::endian::native == std::endian::big) {
#if defined(_MSC_VER) && !defined(__clang__)
        word = _byteswap_uint64(word);
#else
        word = __builtin_bswap64(word);
#endif
    }
    std::memcpy(out, &word, 8);
}

/// Exactly eight digits of @p value (< 10^8), zero-padded, as one word:
/// byte k holds the k-th digit from the left, as ASCII.
///
/// SWAR, no table: the value splits into two 4-digit halves in two 32-bit
/// lanes, each lane into its 2-digit pairs in 16-bit lanes, and each pair
/// into its digits in bytes — three multiply-shift steps on the whole word
/// at once, ending in one OR with '0' bytes. The reciprocal constants are
/// exact over their lane ranges: `(n * 10486) >> 20 == n / 100` for
/// n < 10000 and `(n * 103) >> 10 == n / 10` for n < 100, and no lane's
/// product reaches its neighbour (10486 * 9999 < 2^27, 103 * 99 < 2^14).
/// Beyond the op count, the point is *one* 8-byte store where the pair
/// table needed four 2-byte ones: a following wide load (the point
/// insertion shift, the trailing-zero scan) cannot forward from several
/// narrow stores and stalled until they retired. The pair-table body stays
/// as `eight_digits_scalar`, the twin the exhaustive test pins this to.
[[nodiscard]] inline uint64_t eight_digits_word(uint32_t value) noexcept {
    const uint64_t high = value / 10000;       // the four leading digits
    const uint64_t low = value - high * 10000; // the four trailing digits
    uint64_t lanes = high | (low << 32);       // two 4-digit lanes, leading first
    const uint64_t hundreds = ((lanes * 10486) >> 20) & 0x0000007F0000007FULL;
    const uint64_t units = lanes - hundreds * 100;
    lanes = hundreds | (units << 16); // four 2-digit lanes, in digit order
    const uint64_t tens = ((lanes * 103) >> 10) & 0x000F000F000F000FULL;
    const uint64_t ones = lanes - tens * 10;
    return (tens | (ones << 8)) | 0x3030303030303030ULL;
}

/// The pair-table twin of @ref eight_digits_word: same eight ASCII digits,
/// via the divide-by-hundred chain. Reference only — `tests/cpp` runs the
/// two against each other over every value below 10^8.
inline void eight_digits_scalar(uint32_t value, char* out) noexcept {
    const uint32_t high = value / 10000;
    const uint32_t low = value % 10000;
    const uint32_t pair0 = (high / 100) * 2;
    const uint32_t pair1 = (high % 100) * 2;
    const uint32_t pair2 = (low / 100) * 2;
    const uint32_t pair3 = (low % 100) * 2;
    std::memcpy(out + 0, kDigitPairs + pair0, 2);
    std::memcpy(out + 2, kDigitPairs + pair1, 2);
    std::memcpy(out + 4, kDigitPairs + pair2, 2);
    std::memcpy(out + 6, kDigitPairs + pair3, 2);
}

/// Exactly eight digits of @p value (< 10^8), zero-padded.
inline void write_8_digits(uint32_t value, char* out) noexcept {
    store_digit_word(out, eight_digits_word(value));
}

/// The low @p len digits of @p value (< 10^len), as one eight-byte store.
///
/// The digit word holds the k-th digit from the left in byte k, so a group of
/// @p len digits is that word with its 8-len leading zero bytes shifted off.
/// The store spills 8-len scratch bytes past the group; every caller below
/// writes the next group over exactly that span, so the spill is always
/// overwritten and the digits never reach past out[len - 1].
///
/// Precondition: 1 <= @p len <= 8, @p value < 10^len, and eight writable
/// bytes at @p out.
inline void write_head_digits(uint32_t value, char* out, size_t len) noexcept {
    assert(len >= 1 && len <= 8 && value < kPow10[len]);
    store_digit_word(out, eight_digits_word(value) >> ((8 - len) * 8));
}

/// Exactly @p len digits of @p value, zero-padded on the left.
///
/// Groups of eight, head first, each group one `eight_digits_word` store: the
/// divides become multiply-shifts, the two words are independent once the
/// split is done, and the last store ends exactly at out[len - 1].
///
/// The head used to take the pair-table loop below at every width, so a
/// 16-digit significand — 71% of the full-precision doubles a `random()`
/// payload produces, which is what mixed.json's floats are — paid four
/// *chained* 32-bit divides for its leading eight digits while its trailing
/// eight went through the word in one step. What still takes the loop is only
/// what a word cannot pay for: one to eight digits at the top level, and the
/// one to four digits above two full groups at seventeen and beyond.
///
/// Precondition: @p len writable bytes at @p out, and — for the nine-to-
/// sixteen widths the head word serves — @p value < 10^len. Every caller
/// passes `decimal_digit_count(value)`, so the head is a genuine digit group;
/// a wider value would overflow the word's eight-digit split.
inline void write_digits_fixed(uint64_t value, char* out, size_t len) noexcept {
    // One 64-bit split at most: a double's significand is at most 17 digits,
    // so after peeling the low eight the rest always fits 32 bits, where the
    // constant divisions are materially cheaper — this function was 29% of a
    // pure 17-digit float dump before the narrowing, rivalling Dragonbox
    // itself. (Values past 16 digits with a >32-bit remainder — possible for
    // arbitrary u64 input, not for our callers — take a second 64-bit peel.)
    if (len > 8) {
        const uint64_t high = value / 100000000;
        const auto low = static_cast<uint32_t>(value - high * 100000000);
        const size_t head = len - 8; // at least one digit, by the branch
        if (head <= 8) {
            // Nine to sixteen digits: two words, head first, so the head's
            // spill past its own digits is exactly where the low group lands.
            write_head_digits(static_cast<uint32_t>(high), out, head);
            write_8_digits(low, out + head);
            return;
        }
        // Seventeen digits or more: the low two groups are words, and the top
        // one to four digits take the pair loop below — a chain that short
        // beats the word's fixed cost and its overlapping store (measured:
        // +1.2 ns per value with a word there, on the 17-digit bucket alone).
        write_8_digits(low, out + head);
        const uint64_t top =
            high > 0xFFFFFFFFULL ? high / 100000000 : static_cast<uint32_t>(high) / 100000000U;
        write_8_digits(static_cast<uint32_t>(high - top * 100000000), out + head - 8);
        value = top;
        len = head - 8;
    }
    // One to eight digits: the pair table, two at a time. Reached by the short
    // forms — the integral and scientific layouts — where the word's fixed
    // cost loses to a one- or two-step divide chain, and kept as the twin the
    // exhaustive reference test pins the grouped path against.
    auto rest = static_cast<uint32_t>(value);
    size_t index = len;
    while (index >= 2) {
        const uint32_t pair = (rest % 100) * 2;
        rest /= 100;
        out[index - 1] = kDigitPairs[pair + 1];
        out[index - 2] = kDigitPairs[pair];
        index -= 2;
    }
    if (index == 1)
        out[0] = static_cast<char>('0' + rest % 10);
}

/// Fill digits of @p value backwards, ending at @p end; returns the first byte.
[[nodiscard]] inline char* fill_u64_backwards(uint64_t value, char* end) noexcept {
    char* cursor = end;
    while (value >= 100) {
        const size_t pair = static_cast<size_t>(value % 100) * 2;
        value /= 100;
        *--cursor = kDigitPairs[pair + 1];
        *--cursor = kDigitPairs[pair];
    }
    if (value >= 10) {
        const size_t pair = static_cast<size_t>(value) * 2;
        *--cursor = kDigitPairs[pair + 1];
        *--cursor = kDigitPairs[pair];
    } else {
        *--cursor = static_cast<char>('0' + value);
    }
    return cursor;
}

/// Shift sixteen bytes right by @p gap places (loads before stores, so the
/// ranges may overlap). Spelled as two u64 moves because a `memmove` call --
/// which is what the extension build turned the "constant-size" form into --
/// profiled at 12% of users.json serialization.
inline void shift16_right(char* from, size_t gap) noexcept {
    uint64_t low;
    uint64_t high;
    std::memcpy(&low, from, 8);
    std::memcpy(&high, from + 8, 8);
    std::memcpy(from + gap, &low, 8);
    std::memcpy(from + gap + 8, &high, 8);
}

} // namespace detail

/**
 * Render @p value into @p out as shortest round-trip JSON text.
 *
 * Non-finite values are *not* handled here: JSON has no NaN or infinity, and
 * choosing what to emit in their place is the serializer's policy, not this
 * function's. Callers must filter them first.
 *
 * Deliberately out of line, unlike `format_int64`: a header-inline variant
 * with the micro-decimal tier at the call site was measured 6% *slower* on
 * arrays mixing short and long-form floats — the tier's hit-or-miss branch
 * is data-random there and mispredicts at the call site, where the single
 * out-of-line call is perfectly predictable (docs/performance/SKILL.md,
 * wave 10 negative result). LTO/PGO builds — every benchmarked build —
 * still inline it where the profile says it pays.
 *
 * @param out Buffer of at least @ref kDoubleBufferSize bytes. Not terminated.
 * @return Number of bytes written, or 0 if the buffer was too small.
 */
[[nodiscard]] size_t format_double(double value, char* out, size_t capacity) noexcept;

/**
 * Render @p value as decimal digits into @p out. Not terminated.
 *
 * Digits come two at a time out of a 200-byte pair table — the classic itoa
 * that beats `to_chars`' generic machinery by roughly half on the short
 * integers JSON is made of. Three shapes read that table, chosen by
 * magnitude: a four-byte word below 10^4, straight-line pairs up to 10^10,
 * and above that the count-then-back-fill loop, which lands the digits where
 * they belong without copying anything twice. (A grouped
 * fixed-width write via `detail::write_digits_fixed` was measured against
 * this body and lost on every length that matters — +24% on 1–4 digit
 * values, +4% at 9–10 digits — winning only past 14 digits, which JSON
 * integers essentially never have. Splitting nine-plus-digit values into a
 * back-filled head and the low eight as one `eight_digits_word` store was
 * measured too, and also lost: 9–10-digit lists 8.8 -> 9.0 ns per element,
 * short lists +5% for the extra branch — the divider hides the pair chain
 * at these widths, and the word's ~18 ops are not cheaper than five pair
 * steps.) Behaviour is identical to `to_chars`;
 * the digit-for-digit equivalence is pinned by test over boundaries and a
 * large random sweep.
 *
 * @param out Buffer of at least @ref kInt64BufferSize bytes.
 * @return Number of bytes written.
 */
[[nodiscard]] inline size_t format_int64(int64_t value, char* out) noexcept {
    auto magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude; // wraps correctly for INT64_MIN
    }
    char* cursor = out + written;
    // Values under 10^4 -- ids, counts, ages, the bulk of real integer data --
    // take one straight-line shape: the digit count from three compares, two
    // pair lookups into a 4-byte word, shifted and stored whole (the four
    // bytes land inside the kInt64BufferSize the contract gives). Everything
    // else takes the digit count and the backwards pair loop. Measured in
    // experiments/itoa on a quiet arm64 machine (ns per value, five runs
    // within 2%): 1-3 digits 1.78 -> 1.70, 4-6 digits 2.44 -> 2.47, mixed
    // 1-7-digit lists 2.18 -> 2.18, 8 digits 3.08 -> 3.00. A wider word tier
    // for 5-8 digits was measured and rejected: the eight-digit word's fixed
    // cost lost 18% on 4-6-digit values and 8% on mixed widths there
    // (docs/decisions.md 2026-09-03).
    static_assert(std::endian::native == std::endian::little,
                  "the 4-byte digit word assumes little-endian byte order");
    if (magnitude < 10000) {
        const auto v = static_cast<uint32_t>(magnitude);
        const size_t digits = v >= 1000 ? 4 : v >= 100 ? 3 : v >= 10 ? 2 : 1;
        const uint32_t high = v / 100;
        const uint32_t low = v - high * 100;
        char pairs[4];
        std::memcpy(pairs, &detail::kDigitPairs[high * 2], 2);
        std::memcpy(pairs + 2, &detail::kDigitPairs[low * 2], 2);
        uint32_t word;
        std::memcpy(&word, pairs, 4);
        word >>= (4 - digits) * 8;
        std::memcpy(cursor, &word, 4);
        return written + digits;
    }
    // 10^4 up to 10^10 -- timestamps, ids, quantities in cents -- take the
    // same pair table with the loop unrolled by digit count: every divisor is
    // a compile-time multiply-shift, the pairs are independent, and no step
    // waits on the previous one's quotient. The loop's trip count is data, so
    // its exit branch mispredicts on width-mixed data and each iteration's
    // divide feeds the next; straight-line code has neither problem. Measured
    // in experiments/itoa on the Neoverse-N2 profile leg (ns per value,
    // median of 30) as variant `switch` against the loop: 4-6 digits
    // 4.17 -> 3.87, 7 digits 5.21 -> 4.14, 8 digits 5.13 -> 4.14, 9-10 digits
    // 6.44 -> 5.04. Below 10^4 the word tier above still wins (1-3 digits
    // 2.39 against this shape's 2.91), which is why it stays in front.
    if (magnitude < 100000000ULL) {
        // Five to eight digits: 32-bit arithmetic throughout.
        const auto v = static_cast<uint32_t>(magnitude);
        if (v < 1000000U) {
            const uint32_t high = v / 10000;
            const uint32_t rest = v - high * 10000;
            const uint32_t middle = rest / 100;
            const uint32_t low = rest - middle * 100;
            if (v < 100000U) {
                cursor[0] = static_cast<char>('0' + high);
                detail::write_digit_pair(cursor + 1, middle);
                detail::write_digit_pair(cursor + 3, low);
                return written + 5;
            }
            detail::write_digit_pair(cursor + 0, high);
            detail::write_digit_pair(cursor + 2, middle);
            detail::write_digit_pair(cursor + 4, low);
            return written + 6;
        }
        const uint32_t high = v / 1000000;
        const uint32_t rest = v - high * 1000000;
        const uint32_t upper = rest / 10000;
        const uint32_t tail = rest - upper * 10000;
        const uint32_t middle = tail / 100;
        const uint32_t low = tail - middle * 100;
        if (v < 10000000U) {
            cursor[0] = static_cast<char>('0' + high);
            detail::write_digit_pair(cursor + 1, upper);
            detail::write_digit_pair(cursor + 3, middle);
            detail::write_digit_pair(cursor + 5, low);
            return written + 7;
        }
        detail::write_digit_pair(cursor + 0, high);
        detail::write_digit_pair(cursor + 2, upper);
        detail::write_digit_pair(cursor + 4, middle);
        detail::write_digit_pair(cursor + 6, low);
        return written + 8;
    }
    if (magnitude < 10000000000ULL) {
        // Nine or ten digits: one 64-bit split off the top, then the leading
        // one or two digits from the table and the low eight as the SWAR
        // word. The word's store ends exactly on the last digit at both
        // widths (1 + 8 and 2 + 8), so nothing spills past the number.
        //
        // The doc comment above records a *losing* measurement of a split
        // that looks like this one -- a back-filled head plus the word, which
        // cost 9-10-digit lists 8.8 -> 9.0 ns and short lists 5%. Two things
        // differ. That variant hung the split off the short path, so every
        // one- to four-digit value paid its branch; this one is reached only
        // after two magnitude tests those values never fall through, and it
        // takes the head from the pair table instead of the loop. The reason
        // it is here at all is the deficit the shape it replaces leaves on
        // the arm64 leg -- ints at 7 digits 1.37x of orjson, at 8 digits
        // 1.39x, at 9-10 digits 1.51x, against 0.90x at 1-3 digits.
        const auto high = static_cast<uint32_t>(magnitude / 100000000ULL); // 1..99
        const auto low =
            static_cast<uint32_t>(magnitude - static_cast<uint64_t>(high) * 100000000ULL);
        if (high < 10) {
            cursor[0] = static_cast<char>('0' + high);
            detail::write_8_digits(low, cursor + 1);
            return written + 9;
        }
        detail::write_digit_pair(cursor + 0, high);
        detail::write_8_digits(low, cursor + 2);
        return written + 10;
    }
    // Eleven digits and up: the backwards pair loop, unchanged. This is where
    // the measured variant stopped -- past 10^10 the leading group no longer
    // fits the 32-bit split, and JSON integers that wide are rare enough that
    // the extra code would be read once and never again.
    const size_t digits = detail::decimal_digit_count(magnitude);
    (void)detail::fill_u64_backwards(magnitude, cursor + digits);
    return written + digits;
}

} // namespace strata::util

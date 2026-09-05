#pragma once

/**
 * @file fast_parse.hpp
 * @brief Unified JSON number scanning and conversion.
 *
 * One pass validates the token against JSON's grammar and classifies it; the
 * conversion then runs over the exact span that was validated.
 *
 * Integers are exact at any size. A literal that does not fit `int64_t` is
 * reported as @ref NumberKind::BigInt with its span, leaving the decision of
 * how to represent it to the handler — the DOM widens to double, the Python
 * builder constructs an arbitrary-precision int. The previous implementation
 * accumulated only the first 19 digits and then silently returned that
 * truncation as the value, which is how it mis-parsed 20-digit integers
 * (docs/context/api.md: "do not reproduce").
 *
 * Conversion uses `std::from_chars`, which is correctly rounded and, unlike
 * `strtod`, immune to the process locale's decimal separator. Where the
 * standard library lacks the floating-point overload, @ref from_chars_double
 * provides a strtod_l twin with identical observable behavior.
 */

#include <bit>
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <system_error>

/// Keeps a function out of its callers: the hot heads that must stay small.
#if defined(_MSC_VER) && !defined(__clang__)
#define STRATA_NOINLINE __declspec(noinline)
#elif defined(__clang__) || defined(__GNUC__)
#define STRATA_NOINLINE [[gnu::noinline]]
#else
#define STRATA_NOINLINE
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h> // _umul128: MSVC's spelling of the 64x64->128 multiply
#endif

// A placement the measurements depend on is declared, not left to heuristics
// (docs/performance/SKILL.md, wave 12): the same source read +13% on 17-digit
// float lists when the fraction digit-run came out as a separate function.
// Both compiler families get their own spelling so every leg builds the
// measured shape.
#if defined(_MSC_VER) && !defined(__clang__)
#define STRATA_ALWAYS_INLINE __forceinline
#elif defined(__clang__) || defined(__GNUC__)
#define STRATA_ALWAYS_INLINE [[gnu::always_inline]] inline
#else
#define STRATA_ALWAYS_INLINE inline
#endif

// Floating-point std::from_chars reached libc++ only at LLVM 20; Apple SDKs
// through Xcode 16 ship the integral overloads alone, so those builds take
// the strtod_l twin below. STRATA_FORCE_STRTOD_FALLBACK compiles the twin on
// a machine whose library does have from_chars, so both branches run under
// the same suites locally (the scalar-twin rule applied to a library gap).
#if !defined(STRATA_FORCE_STRTOD_FALLBACK) && defined(__cpp_lib_to_chars) &&                       \
    __cpp_lib_to_chars >= 201611L
#define STRATA_HAS_FP_FROM_CHARS 1
#else
#define STRATA_HAS_FP_FROM_CHARS 0
#endif

#if !STRATA_HAS_FP_FROM_CHARS
#if !defined(__APPLE__)
#error "no floating-point std::from_chars and no Apple strtod_l fallback for this toolchain"
#endif
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <xlocale.h>
#endif

namespace strata::util {

/// How a validated number token should be represented.
enum class NumberKind {
    Int64,  ///< Integral and representable as int64_t; use ParsedNumber::int_value.
    BigInt, ///< Integral but outside int64_t; the caller reads the raw span.
    Double, ///< Has a fraction or an exponent; use ParsedNumber::double_value.
};

/// Outcome of parsing one number token.
struct ParsedNumber {
    NumberKind kind = NumberKind::Int64;
    int64_t int_value = 0;
    double double_value = 0.0;
    size_t consumed = 0; ///< Bytes of input the token occupies.
};

namespace detail {

[[nodiscard]] constexpr bool is_digit(char c) noexcept { return c >= '0' && c <= '9'; }

/// 64x64 -> 128 multiply, spelled per compiler.
struct U64Pair {
    uint64_t hi;
    uint64_t lo;
};

[[nodiscard]] inline U64Pair mul_128(uint64_t a, uint64_t b) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    U64Pair result;
    result.lo = _umul128(a, b, &result.hi);
    return result;
#else
    const auto product = static_cast<unsigned __int128>(a) * b;
    return {static_cast<uint64_t>(product >> 64), static_cast<uint64_t>(product)};
#endif
}

inline constexpr long kEiselLemireMinExp10 = -348;
inline constexpr long kEiselLemireMaxExp10 = 347;

#include "eisel_lemire_table.inc"

/**
 * Eisel–Lemire: the correctly rounded double for mantissa × 10^exp10, or
 * false when only the slow path can decide.
 *
 * One (sometimes two) 64×64 multiplies of the normalized mantissa against a
 * 128-bit power of ten give enough product bits to read the rounded result
 * directly — the insight of Eisel and Lemire ("Number parsing at a gigabyte
 * per second"); the structure here follows Go strconv's port, whose table
 * convention the generated .inc matches. Every ambiguity refuses rather than
 * guesses: a truncated table entry that could flip the round bit, an exact
 * halfway needing round-to-even, and any subnormal or out-of-range result
 * all return false, and the caller's from_chars fallback — exact everywhere,
 * merely slower — settles them. So a true return is provably the same bits
 * from_chars would produce, which is what keeps this a pure fast path rather
 * than a semantics change.
 *
 * Preconditions: @p mantissa is non-zero and holds every significant digit
 * (the caller skips this path for tokens past 19 digits).
 */
[[nodiscard]] inline bool eisel_lemire_double(uint64_t mantissa, long exp10, bool negative,
                                              double& out) noexcept {
    if (exp10 < kEiselLemireMinExp10 || exp10 > kEiselLemireMaxExp10)
        return false;

    const int shift = std::countl_zero(mantissa);
    const uint64_t w = mantissa << shift;
    const uint64_t* power = kEiselLemirePow10[exp10 - kEiselLemireMinExp10];

    // 217706/2^16 approximates log2(10); with |exp10| <= 348 the estimate is
    // exact for the whole table. 1087 = IEEE bias 1023 + the 64 bits the
    // product's high word sits above the binary point.
    uint64_t exp2 =
        static_cast<uint64_t>((217706 * exp10 >> 16) + 1087) - static_cast<uint64_t>(shift);

    U64Pair x = mul_128(w, power[0]);
    if ((x.hi & 0x1FF) == 0x1FF && x.lo + w < w) {
        // The truncated high product cannot separate the round bit from a
        // carry out of the discarded low half: widen with the second table
        // word before giving up.
        const U64Pair y = mul_128(w, power[1]);
        uint64_t merged_hi = x.hi;
        uint64_t merged_lo = x.lo + y.hi;
        if (merged_lo < x.lo)
            ++merged_hi;
        if ((merged_hi & 0x1FF) == 0x1FF && merged_lo + 1 == 0 && y.lo + w < w)
            return false;
        x.hi = merged_hi;
        x.lo = merged_lo;
    }

    // The product's high word carries the answer in its top bits; keep 54 of
    // them (53 mantissa + 1 round bit), from bit 63 or 62 depending on
    // whether the multiply of two normalized operands stayed normalized.
    const uint64_t msb = x.hi >> 63;
    uint64_t out_mantissa = x.hi >> (msb + 9);
    exp2 -= 1 ^ msb;

    if (x.lo == 0 && (x.hi & 0x1FF) == 0 && (out_mantissa & 3) == 1) {
        // Exactly halfway between two doubles: round-to-even needs digits
        // the product no longer has.
        return false;
    }

    out_mantissa = (out_mantissa + (out_mantissa & 1)) >> 1; // round to nearest
    if ((out_mantissa >> 53) > 0) {
        out_mantissa >>= 1;
        ++exp2;
    }

    // Rejects biased exponents 0 (subnormal territory — exactness there needs
    // the slow path) and 0x7FF (infinity), including the wrapped negatives.
    if (exp2 - 1 >= 0x7FF - 1)
        return false;

    uint64_t bits = (exp2 << 52) | (out_mantissa & 0xFFFFFFFFFFFFFULL);
    if (negative)
        bits |= 0x8000000000000000ULL;
    out = std::bit_cast<double>(bits);
    return true;
}

#if !STRATA_HAS_FP_FROM_CHARS
/// Process-lifetime "C" locale for strtod_l; never freed by design. Darwin
/// treats a null locale_t as the C locale, so even a failed newlocale still
/// parses correctly.
[[nodiscard]] inline locale_t c_locale() noexcept {
    static const locale_t locale = newlocale(LC_ALL_MASK, "C", static_cast<locale_t>(nullptr));
    return locale;
}
#endif

/// The eight bytes of @p chunk as digit values, one per byte: ASCII '0'
/// subtracted from all of them at once.
///
/// A byte that is not a digit comes out as whatever the subtraction leaves,
/// and may borrow — but a borrow only ever travels *upward*, from a byte
/// into later ones, so every byte below the first non-digit holds exactly
/// its own digit's value whatever follows it. That is the property the
/// leading-run helpers below rest on, and it is why they can all share this
/// single subtraction: it is the same expression @ref leading_digit_count
/// evaluates, so the value of a run costs no subtraction of its own.
[[nodiscard]] inline uint64_t digit_values(uint64_t chunk) noexcept {
    return chunk - 0x3030303030303030ULL;
}

/// The number spelled by the eight per-byte digit values in @p values (first
/// digit in the low byte, i.e. the word loaded from the text on a
/// little-endian target), in three multiplies instead of eight serial
/// multiply-adds (fast_float's in-register reduction). Byte order note: the
/// pair/quad folding assumes the first digit sits in the low byte —
/// little-endian targets, which every supported platform is; the per-digit
/// loops beside the call sites remain the definition and the fallback.
[[nodiscard]] inline uint32_t eight_digit_value_word(uint64_t values) noexcept {
    constexpr uint64_t kMask = 0x000000FF000000FFULL;
    constexpr uint64_t kMul1 = 100 + (1000000ULL << 32);
    constexpr uint64_t kMul2 = 1 + (10000ULL << 32);
    values = (values * 10) + (values >> 8);
    values = (((values & kMask) * kMul1) + (((values >> 16) & kMask) * kMul2)) >> 32;
    return static_cast<uint32_t>(values);
}

/// The numeric value of the eight ASCII digits held in @p chunk.
[[nodiscard]] inline uint32_t eight_digit_word_value(uint64_t chunk) noexcept {
    return eight_digit_value_word(digit_values(chunk));
}

// The word helpers below read the text's first byte in the word's low byte
// (the run length is a count of trailing zero bits, the value a shift up):
// little-endian by construction, like every SWAR tier in scan.hpp. Every
// supported platform is; a big-endian build stops here instead of parsing
// numbers wrongly and silently.
static_assert(std::endian::native == std::endian::little,
              "fast_parse.hpp's digit-run word helpers assume a little-endian target");

/// How many of the eight bytes in @p chunk, from the first, are ASCII digits
/// (0..8). The digit test is fast_float's: adding 0x46 carries into bit 7
/// exactly for bytes above '9', subtracting 0x30 borrows for bytes below
/// '0'. Its carry and borrow only ever travel *upward* (from a byte into
/// later ones), so every byte below the first flagged non-digit is exactly
/// classified, which is all a leading count needs.
[[nodiscard]] inline unsigned leading_digit_count(uint64_t chunk) noexcept {
    const uint64_t non_digit =
        ((chunk + 0x4646464646464646ULL) | digit_values(chunk)) & 0x8080808080808080ULL;
    return static_cast<unsigned>(std::countr_zero(non_digit)) >> 3; // 64 >> 3 == 8 when none
}

/// The value of the first @p count digits of @p chunk (1..8): the run is
/// shifted up so it sits in the high bytes, and the eight-digit reduction
/// then weights it by the powers of ten the vacated low bytes stand for.
///
/// Subtracting '0' *before* the alignment shift is what makes this short.
/// The shift brings in zero bytes, which are already exactly the value a
/// '0' digit carries, so the padding word the previous shape built — two
/// shifts and an or, every one of them downstream of @p count — is gone,
/// and the one subtraction left is @ref leading_digit_count's own, off
/// @p count's dependency chain entirely. Bytes above the run are discarded
/// by the shift and their borrows never reach it (see @ref digit_values).
[[nodiscard]] inline uint32_t leading_digit_value(uint64_t chunk, unsigned count) noexcept {
    assert(count >= 1 && count <= 8);
    return eight_digit_value_word(digit_values(chunk) << ((8 - count) * 8)); // shift 0..56
}

/// The byte at @p index (0..7) of a word already loaded from the input,
/// instead of a second load of that same byte.
///
/// Every byte a number head reads behind a digit run — the terminator that
/// proves the run ends, the first byte that enforces the lone-zero rule —
/// lies inside the word the run's count came from, because a count that
/// leaves room for a terminator is below eight. Reading it out of the word
/// keeps the check on the arithmetic pipes: the address it would load from
/// is not known any earlier than the shift amount is.
[[nodiscard]] inline char word_byte(uint64_t chunk, unsigned index) noexcept {
    assert(index < 8);
    return static_cast<char>((chunk >> (index * 8)) & 0xFFULL);
}

/// How many of the first @p count digit bytes of @p chunk are '0'.
[[nodiscard]] inline unsigned leading_zero_digits(uint64_t chunk, unsigned count) noexcept {
    assert(count >= 1 && count <= 8);
    const uint64_t run = ~0ULL >> ((8 - count) * 8);
    const uint64_t non_zero = (chunk ^ 0x3030303030303030ULL) & run;
    const unsigned first = static_cast<unsigned>(std::countr_zero(non_zero)) >> 3;
    return first < count ? first : count;
}

/// Powers of ten a digit-run step multiplies by (10^0 .. 10^8).
inline constexpr uint64_t kRunPow10[9] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
};

/// The powers of ten that are exact doubles (5^22 < 2^53): Clinger's window,
/// shared by the full scanner and the parser's short-number head.
inline constexpr double kClingerPow10[23] = {
    1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11,
    1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22,
};

/// Significant digits a number keeps exactly; the rest only set `truncated`.
inline constexpr size_t kMaxSignificant = 19;

/**
 * The short-number head's long-fraction step: the double for
 * `int_value . <8..19 fraction digits>` when it can be settled without the
 * full scanner, or false when the scanner must decide.
 *
 * @p fraction points at the first fraction digit and @p first_word holds
 * the eight bytes there, which the caller proved are all digits; @p available
 * is the byte count from @p fraction to the end of input. Up to two more
 * words extend the run: a second word with fewer than eight leading digits
 * ends it (8..15 fraction digits); a full second word and a third with
 * fewer than eight end it at 16..23, of which the significant-digit bound
 * of 19 (kMaxSignificant -- the accumulator's own limit, so the mantissa
 * holds every digit and is never truncated) keeps 16..19 for a fraction
 * behind a lone zero. The conversion is the scanner's own, step for step:
 * a mantissa at or below 2^53 divides by an exact power of ten (Clinger),
 * anything larger goes through Eisel-Lemire, whose refusals fall back to the
 * scanner along with an exponent behind the digits -- so a true return is
 * bit-identical to parse_number_unified on the same bytes. Twenty-four
 * readable bytes are required past @p fraction: the three words, and the
 * byte after any run they can end.
 *
 * Out of line on purpose: the head is a hot, non-inlined function whose
 * every added byte of code taxes the integers that take neither branch
 * (measured on the second integer word: +2-4% on 5-7-digit integers), and
 * these fractions are the rarest shape it serves.
 */
[[nodiscard]] STRATA_NOINLINE inline bool
parse_long_fraction(const char* fraction, size_t available, uint64_t first_word, uint32_t int_value,
                    unsigned int_digits, bool negative, double& out,
                    size_t& fraction_digits) noexcept {
    if (available < 24)
        return false;
    uint64_t second_word;
    std::memcpy(&second_word, fraction + 8, 8);
    const unsigned second_count = leading_digit_count(second_word);
    // A lone zero before the point is not a significant digit.
    const unsigned significant = int_value == 0 ? 0u : int_digits;
    uint64_t mantissa = int_value * kRunPow10[8] + leading_digit_value(first_word, 8);
    size_t digits;
    if (second_count < 8) {
        digits = 8 + second_count;
        if (second_count != 0) {
            mantissa =
                mantissa * kRunPow10[second_count] + leading_digit_value(second_word, second_count);
        }
    } else {
        uint64_t third_word;
        std::memcpy(&third_word, fraction + 16, 8);
        const unsigned third_count = leading_digit_count(third_word);
        if (third_count == 8)
            return false;
        digits = 16 + third_count;
        mantissa = mantissa * kRunPow10[8] + leading_digit_value(second_word, 8);
        if (third_count != 0) {
            mantissa =
                mantissa * kRunPow10[third_count] + leading_digit_value(third_word, third_count);
        }
    }
    if (significant + digits > kMaxSignificant)
        return false;
    const char next = fraction[digits];
    if (next == 'e' || next == 'E')
        return false;
    fraction_digits = digits;
    if (mantissa <= (uint64_t{1} << 53)) {
        const double magnitude = static_cast<double>(mantissa) / kClingerPow10[digits];
        out = negative ? -magnitude : magnitude;
        return true;
    }
    return eisel_lemire_double(mantissa, -static_cast<long>(digits), negative, out);
}

/// The digit accumulator shared by the integer and fraction parts: the first
/// nineteen significant digits, how many were kept, the zeros seen before the
/// first significant digit (a fraction's leading zeros only shift the
/// exponent), and whether digits past the cap were dropped.
struct DigitAccumulator {
    uint64_t mantissa = 0;
    size_t significant = 0;
    size_t leading_zeros = 0;
    bool truncated = false;
};

/// The per-digit body: one byte of the run into @p acc.
inline void accumulate_digit(char c, DigitAccumulator& acc) noexcept {
    if (acc.mantissa == 0 && c == '0') {
        ++acc.leading_zeros; // carries no significance, only shifts the exponent
    } else if (acc.significant < kMaxSignificant) {
        acc.mantissa = acc.mantissa * 10 + static_cast<uint64_t>(c - '0');
        ++acc.significant;
    } else {
        acc.truncated = true;
    }
}

/// The scalar twin of @ref consume_digit_run: the per-digit body alone.
/// Exposed so the equivalence is *checked* (tests/cpp) rather than asserted.
inline size_t consume_digit_run_scalar(const char* str, size_t len, size_t& pos,
                                       DigitAccumulator& acc) noexcept {
    const size_t start = pos;
    while (pos < len && is_digit(str[pos])) {
        accumulate_digit(str[pos], acc);
        ++pos;
    }
    return pos - start;
}

/**
 * Advance past the run of ASCII digits at `str[pos..len)`, accumulating it
 * into @p acc; returns the number of digits consumed.
 *
 * Up to eight digits per step while eight bytes are readable: one word
 * load, the run's length from the digit mask, its value from the padded
 * eight-digit reduction, its leading zeros (only while the mantissa is still
 * zero) from a second mask — no branch that depends on where the run ends.
 * The per-digit loop it replaced mispredicted its exit on every number of
 * random width, which on number-dense documents (a 64-element array of
 * 1..10-digit ints, a column of 5-decimal floats) was the largest single
 * cost the parser paid: homogeneous number lists read 1.33–1.55x behind
 * orjson before this, bools and nulls through the same array loop at parity.
 * The per-digit body remains the definition: it takes the last seven bytes
 * of the input, and every digit past the nineteen-significant cap.
 *
 * Forced inline (STRATA_ALWAYS_INLINE): left to the heuristics, the fraction
 * instantiation came out as a separate function — a prologue and epilogue per
 * fraction, measured as +13% on 17-digit float lists against the loop it
 * replaced.
 *
 * @tparam kLeadingZerosPossible False for an integer part, whose first digit
 *         is never '0' past a lone zero; true for a fraction.
 */
template <bool kLeadingZerosPossible>
STRATA_ALWAYS_INLINE size_t consume_digit_run(const char* str, size_t len, size_t& pos,
                                              DigitAccumulator& acc) noexcept {
    const size_t start = pos;
    while (pos + 8 <= len) {
        uint64_t chunk;
        std::memcpy(&chunk, str + pos, 8);
        const unsigned count = leading_digit_count(chunk);
        if (count == 0)
            return pos - start;
        // An integer part starts with a non-zero digit (the lone-zero rule is
        // enforced by the caller), so only a fraction can hold zeros ahead of
        // the first significant digit, and only when its run starts with one;
        // the integer instantiation folds the second mask away entirely.
        const unsigned zeros = kLeadingZerosPossible && acc.mantissa == 0 && (chunk & 0xFF) == '0'
                                   ? leading_zero_digits(chunk, count)
                                   : 0;
        const unsigned kept = count - zeros;
        if (acc.significant + kept > kMaxSignificant)
            break; // the cap: the per-digit body decides digit by digit
        acc.mantissa = acc.mantissa * kRunPow10[count] + leading_digit_value(chunk, count);
        acc.significant += kept;
        acc.leading_zeros += zeros;
        pos += count;
        // A run shorter than the word ended inside it; a full word may or may
        // not continue — one byte tells, cheaper than another word step.
        if (count < 8 || pos >= len || !is_digit(str[pos]))
            return pos - start;
    }
    // Sequenced explicitly: the tail advances `pos` by reference, and the
    // operands of `+` are unsequenced — MSVC evaluated the call first and
    // doubled the count (0.1 parsed as 0.01 on the Windows leg).
    const size_t by_word = pos - start;
    return by_word + consume_digit_run_scalar(str, len, pos, acc);
}

} // namespace detail

/// The two std::from_chars_result fields the number call sites consume.
struct FromCharsResult {
    const char* ptr;
    std::errc ec;
};

#if STRATA_HAS_FP_FROM_CHARS

/// Correctly rounded text→double over [first, last), via std::from_chars.
[[nodiscard]] inline FromCharsResult from_chars_double(const char* first, const char* last,
                                                       double& value) noexcept {
    const auto result = std::from_chars(first, last, value);
    return {result.ptr, result.ec};
}

#else

/**
 * strtod_l twin of the from_chars branch, observably identical.
 *
 * Each difference strtod would introduce is neutralized: leading whitespace
 * and '+' are rejected up front (strtod accepts both, from_chars neither);
 * the token is copied and NUL-terminated so the parse can never read past
 * @p last (the input need not be NUL-terminated); the "C" locale is passed
 * explicitly so the process locale's decimal separator cannot leak in; and
 * ERANGE is folded to from_chars semantics — a denormal result (flagged by
 * some libcs) is a representable value and therefore success, only ±infinity
 * and total underflow report result_out_of_range. Hex floats and inf/nan
 * words cannot arise: every caller hands over a span already scanned as
 * sign/digits/point/exponent only. Allocation failure on an oversized token
 * reports not_enough_memory — an error, never a fabricated value.
 */
[[nodiscard]] inline FromCharsResult from_chars_double(const char* first, const char* last,
                                                       double& value) noexcept {
    if (first == last || *first == '+' || *first == ' ' || (*first >= '\t' && *first <= '\r'))
        return {first, std::errc::invalid_argument};

    constexpr size_t kStackCapacity = 127;
    char stack_buffer[kStackCapacity + 1];
    std::unique_ptr<char, void (*)(void*)> heap_buffer(nullptr, std::free);
    const size_t length = static_cast<size_t>(last - first);
    char* token = stack_buffer;
    if (length > kStackCapacity) {
        heap_buffer.reset(static_cast<char*>(std::malloc(length + 1)));
        if (heap_buffer == nullptr)
            return {first, std::errc::not_enough_memory};
        token = heap_buffer.get();
    }
    std::memcpy(token, first, length);
    token[length] = '\0';

    errno = 0;
    char* token_end = nullptr;
    const double parsed = strtod_l(token, &token_end, detail::c_locale());
    if (token_end == token)
        return {first, std::errc::invalid_argument};

    const char* const ptr = first + (token_end - token);
    if (errno == ERANGE && (parsed == HUGE_VAL || parsed == -HUGE_VAL || parsed == 0.0))
        return {ptr, std::errc::result_out_of_range};
    value = parsed;
    return {ptr, std::errc{}};
}

#endif

/**
 * Validate and convert one JSON number at the start of @p str.
 *
 * Enforces JSON's grammar exactly: an optional `-`, an integer part with no
 * leading zeros, an optional fraction that must have at least one digit after
 * the point, and an optional exponent that must have at least one digit. Any
 * trailing bytes are left for the caller to reject or consume.
 *
 * @return false if the leading bytes are not a valid JSON number, leaving
 *         @p out untouched.
 */
[[nodiscard]] inline bool parse_number_unified(const char* str, size_t len,
                                               ParsedNumber& out) noexcept {
    using detail::is_digit;

    if (len == 0)
        return false;

    size_t pos = 0;
    const bool negative = str[0] == '-';
    if (negative)
        ++pos;

    if (pos >= len || !is_digit(str[pos]))
        return false;

    // The validation scan doubles as the conversion: significant digits
    // accumulate into the mantissa as they are checked, so the common cases
    // below never re-read the span. `from_chars` re-scanning the token was
    // the previous conversion strategy, and on number-dense documents that
    // second pass was a measurable share of the whole parse.
    detail::DigitAccumulator acc;

    // Integer part. A leading zero may only stand alone.
    const size_t int_start = pos;
    if (str[pos] == '0') {
        ++pos;
        if (pos < len && is_digit(str[pos]))
            return false;
    } else {
        (void)detail::consume_digit_run<false>(str, len, pos, acc);
    }
    const size_t int_digits = pos - int_start;
    const bool int_part_is_zero = (int_digits == 1 && str[int_start] == '0');

    bool is_double = false;
    size_t fraction_digits = 0;

    if (pos < len && str[pos] == '.') {
        ++pos;
        if (pos >= len || !is_digit(str[pos]))
            return false;
        fraction_digits = detail::consume_digit_run<true>(str, len, pos, acc);
        is_double = true;
    }
    // Zeros before the first significant digit can only come from the
    // fraction: an integer part is "0" alone or starts with a non-zero digit.
    const size_t fraction_leading_zeros = acc.leading_zeros;
    const uint64_t mantissa = acc.mantissa;
    const size_t significant = acc.significant;
    const bool truncated = acc.truncated;

    long exponent = 0;
    if (pos < len && (str[pos] == 'e' || str[pos] == 'E')) {
        ++pos;
        bool exponent_negative = false;
        if (pos < len && (str[pos] == '-' || str[pos] == '+')) {
            exponent_negative = (str[pos] == '-');
            ++pos;
        }
        if (pos >= len || !is_digit(str[pos]))
            return false;
        while (pos < len && is_digit(str[pos])) {
            if (exponent < 1000000) // saturate; the magnitude is all that matters
                exponent = exponent * 10 + (str[pos] - '0');
            ++pos;
        }
        if (exponent_negative)
            exponent = -exponent;
        is_double = true;
    }

    out.consumed = pos;
    const char* const first = str;
    const char* const last = str + pos;

    if (!is_double) {
        // Up to 18 digits always fit int64 (999999999999999999 < 2^63); the
        // 19-digit boundary cases go to from_chars, which also classifies
        // anything larger as BigInt.
        if (!truncated && significant <= 18) {
            out.kind = NumberKind::Int64;
            out.int_value =
                negative ? -static_cast<int64_t>(mantissa) : static_cast<int64_t>(mantissa);
            return true;
        }
        const auto result = std::from_chars(first, last, out.int_value);
        if (result.ec == std::errc::result_out_of_range) {
            out.kind = NumberKind::BigInt;
            out.int_value = 0;
            return true;
        }
        if (result.ec != std::errc{} || result.ptr != last)
            return false;
        out.kind = NumberKind::Int64;
        return true;
    }

    out.kind = NumberKind::Double;

    // A zero mantissa is exactly ±0.0 whatever the exponent says; settling
    // it here keeps the fast paths below free of the one input that breaks
    // their normalization (countl_zero of zero, a full-width shift).
    if (!truncated && mantissa == 0) {
        out.double_value = negative ? -0.0 : 0.0;
        return true;
    }

    // Exact-arithmetic fast path (Clinger): when the mantissa is exactly
    // representable (<= 2^53) and the scale is a power of ten that is itself
    // exact (10^0..10^22 -- 5^22 < 2^53), one IEEE multiply or divide of two
    // exact operands is correctly rounded by definition. This covers every
    // ordinary decimal ("-750.347674"); the digit-heavy and huge-exponent
    // remainder falls through to from_chars, which is exact everywhere.
    const long effective_exponent = exponent - static_cast<long>(fraction_digits);
    if (!truncated && mantissa <= (uint64_t{1} << 53) && effective_exponent >= -22 &&
        effective_exponent <= 22) {
        const double magnitude = static_cast<double>(mantissa);
        const double value = effective_exponent < 0
                                 ? magnitude / detail::kClingerPow10[-effective_exponent]
                                 : magnitude * detail::kClingerPow10[effective_exponent];
        out.double_value = negative ? -value : value;
        return true;
    }

    // Eisel–Lemire covers what Clinger's exact window cannot — above all the
    // full-precision 17-digit doubles real data is full of — with a provably
    // correct refuse-don't-guess contract (see eisel_lemire_double). What it
    // refuses falls through to from_chars, exact everywhere.
    if (!truncated &&
        detail::eisel_lemire_double(mantissa, effective_exponent, negative, out.double_value))
        return true;

    const auto result = from_chars_double(first, last, out.double_value);
    if (result.ec == std::errc::result_out_of_range) {
        // Outside the representable range in one direction or the other.
        // Decide which from the decimal magnitude: a value only overflows once
        // it is far above 1, and only underflows once it is far below, so the
        // sign of the estimate is unambiguous here.
        const long magnitude = (int_part_is_zero ? -static_cast<long>(fraction_leading_zeros)
                                                 : static_cast<long>(int_digits)) +
                               exponent;
        out.double_value = magnitude > 0 ? std::numeric_limits<double>::infinity() : 0.0;
        if (negative)
            out.double_value = -out.double_value;
        return true;
    }
    if (result.ec != std::errc{} || result.ptr != last)
        return false;
    return true;
}

} // namespace strata::util

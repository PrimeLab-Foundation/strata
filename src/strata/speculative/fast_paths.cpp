// =============================================================================
// strata/speculative/fast_paths.cpp
//
// Specialized fast-path parsers for the speculative parsing engine.
// Each parser assumes a specific value type and bails immediately if
// the assumption is wrong, keeping the miss penalty under 10%.
// =============================================================================

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <strata/speculative/fast_paths.h>
#include <string>

// Use fast_float for high-performance double parsing if available via
// the project's third_party copy; otherwise fall back to strtod.
#if __has_include(<strata/third_party/fast_float/fast_float.h>)
#include <strata/third_party/fast_float/fast_float.h>
#define STRATA_HAS_FAST_FLOAT 1
#else
#define STRATA_HAS_FAST_FLOAT 0
#endif

namespace strata::speculative {

// ─── SWAR 8-digit parser ─────────────────────────────────────────────────────
//
// Classic SWAR technique for parsing up to 8 ASCII digits in parallel:
//
// 1. Load 8 bytes into a uint64_t
// 2. Subtract '0' (0x30) from each byte in parallel
// 3. Check if any byte > 9 (meaning non-digit) by testing high bits
//    after subtracting, using the carry-out technique
// 4. If all 8 are digits, use the SWAR multiply-accumulate reduction:
//    - Pair adjacent digits: d0*10+d1, d2*10+d3, d4*10+d5, d6*10+d7
//    - Pair adjacent pairs:  p0*100+p1, p2*100+p3
//    - Pair adjacent quads:  q0*10000+q1
//    This gives us the 8-digit number in a single register.
//
// On x86-64 and ARM64, unaligned uint64_t loads are fine (naturally aligned
// to 1 byte). We always have at least `remaining` bytes available.

std::pair<uint64_t, int> FastPaths::parse_8_digits(uint64_t word) noexcept {
    // Subtract '0' from each byte
    const uint64_t sub = word - 0x3030303030303030ULL;

    // Check for non-digits: if any byte had value < '0' or > '9', after
    // subtraction it will have bit 7 set (value >= 0x80) or value > 9.
    // We check: (sub + 0x7676767676767676) & 0x8080808080808080
    // A byte with value 0-9 after sub won't trigger the high bit.
    // A byte with value > 9 (or wrapped around from < '0') will.
    const uint64_t check = (sub + 0x7676767676767676ULL) | sub;
    const uint64_t non_digits = check & 0x8080808080808080ULL;

    if (non_digits == 0) {
        // All 8 bytes are digits [0-9]. Apply SWAR reduction.
        // We need to handle endianness: on little-endian, byte 0 is the
        // least significant byte, but it's the FIRST (leftmost) digit.
        //
        // For little-endian: the first digit is in the lowest byte.
        // d[0] = sub & 0xFF, d[1] = (sub >> 8) & 0xFF, etc.
        //
        // We'll use the standard SWAR multiply technique:
        uint64_t v = sub;

        // Step 1: Combine pairs of digits (8 → 4 values, each 0-99)
        // d[0]*10+d[1], d[2]*10+d[3], d[4]*10+d[5], d[6]*10+d[7]
        // On little-endian, d[0] is low byte, d[1] is next byte.
        // We want d[0]*10 + d[1].
        // Mask odd bytes: v_odd = (v >> 8) & 0x00FF00FF00FF00FF
        // Mask even bytes: v_even = v & 0x00FF00FF00FF00FF
        // pairs = v_even * 10 + v_odd
        uint64_t v_even = v & 0x00FF00FF00FF00FFULL;
        uint64_t v_odd = (v >> 8) & 0x00FF00FF00FF00FFULL;
        v = v_even * 10 + v_odd; // 4 values in 16-bit lanes

        // Step 2: Combine pairs of 2-digit values (4 → 2, each 0-9999)
        uint64_t v_lo = v & 0x0000FFFF0000FFFFULL;
        uint64_t v_hi = (v >> 16) & 0x0000FFFF0000FFFFULL;
        v = v_lo * 100 + v_hi; // 2 values in 32-bit lanes

        // Step 3: Combine pairs of 4-digit values (2 → 1, 0-99999999)
        uint64_t v_lower = v & 0x00000000FFFFFFFFULL;
        uint64_t v_upper = v >> 32;
        uint64_t result = v_lower * 10000 + v_upper;

        return {result, 8};
    }

    // Not all 8 digits — count how many leading digits we have.
    // Find the position of the first non-digit byte.
    // non_digits has bit 7 set in each byte that's not a digit.
    // Count trailing zero bits / 8 gives byte position (little-endian).
#if defined(__GNUC__) || defined(__clang__)
    int first_non_digit = __builtin_ctzll(non_digits) / 8;
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanForward64(&idx, non_digits);
    int first_non_digit = static_cast<int>(idx) / 8;
#else
    int first_non_digit = 0;
    uint64_t tmp = non_digits;
    while ((tmp & 0x80) == 0 && first_non_digit < 8) {
        tmp >>= 8;
        ++first_non_digit;
    }
#endif

    if (first_non_digit == 0)
        return {0, 0};

    // Parse the leading digits conventionally (small count, so a loop is fine).
    uint64_t result = 0;
    for (int i = 0; i < first_non_digit; ++i) {
        result = result * 10 + ((sub >> (i * 8)) & 0xFF);
    }
    return {result, first_non_digit};
}

// ─── Integer fast-path ───────────────────────────────────────────────────────
//
// Expects: optional '-', then digits, terminated by structural char or whitespace.
// Rejects: '.', 'e', 'E' (those are floats), leading zeros except "0" itself.

ParseResult FastPaths::parse_integer(const uint8_t* data, size_t remaining) noexcept {
    if (remaining == 0)
        return {false, 0, {}};

    size_t pos = 0;
    bool negative = false;

    // Optional negative sign
    if (data[0] == '-') {
        if (remaining < 2)
            return {false, 0, {}};
        negative = true;
        pos = 1;
    }

    // Must start with a digit
    uint8_t first = data[pos];
    if (first < '0' || first > '9')
        return {false, 0, {}};

    // Leading zero check: "0" must be followed by non-digit
    if (first == '0') {
        if (pos + 1 < remaining) {
            uint8_t next = data[pos + 1];
            if (next >= '0' && next <= '9')
                return {false, 0, {}}; // "01" etc.
            if (next == '.' || next == 'e' || next == 'E')
                return {false, 0, {}}; // float
        }
        return {true, pos + 1,
                JsonValue(JsonValue::Variant(static_cast<double>(negative ? 0 : 0)))};
    }

    // Parse digits using SWAR when we have enough bytes
    uint64_t value = 0;
    size_t digit_start = pos;

    while (pos + 8 <= remaining) {
        uint64_t word;
        std::memcpy(&word, data + pos, 8);
        auto [chunk_val, count] = parse_8_digits(word);

        if (count == 0)
            break;

        // Check for float indicators in the consumed range
        // (the SWAR parser wouldn't have consumed '.' or 'e', but let's verify
        // the first non-digit isn't a float indicator)
        if (count < 8) {
            uint8_t terminator = data[pos + count];
            if (terminator == '.' || terminator == 'e' || terminator == 'E') {
                return {false, 0, {}}; // actually a float, bail
            }
        }

        // Accumulate: value = value * 10^count + chunk_val
        // Pre-computed powers of 10 for the shift
        static constexpr uint64_t pow10[] = {1,      10,      100,      1000,     10000,
                                             100000, 1000000, 10000000, 100000000};

        // Overflow check: if value > MAX/pow10[count], multiplication overflows
        if (value > std::numeric_limits<uint64_t>::max() / pow10[count]) {
            return {false, 0, {}}; // too large, fall back to generic
        }
        value = value * pow10[count] + chunk_val;
        pos += count;

        if (count < 8)
            break; // hit a non-digit
    }

    // Handle remaining digits one at a time (fewer than 8 bytes left)
    while (pos < remaining && data[pos] >= '0' && data[pos] <= '9') {
        uint8_t d = data[pos] - '0';
        if (value > (std::numeric_limits<uint64_t>::max() - d) / 10) {
            return {false, 0, {}}; // overflow
        }
        value = value * 10 + d;
        ++pos;
    }

    // Verify we consumed at least one digit after the sign
    if (pos == digit_start)
        return {false, 0, {}};

    // Verify termination: next char must be structural or whitespace (or end of input)
    if (pos < remaining) {
        uint8_t term = data[pos];
        if (term == '.' || term == 'e' || term == 'E') {
            return {false, 0, {}}; // actually a float
        }
        if (!is_structural_or_ws(term)) {
            return {false, 0, {}}; // unexpected character
        }
    }

    // Convert to double (JSON numbers are IEEE 754 doubles in our model)
    double dval;
    if (negative) {
        // Check int64_t range
        if (value > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1) {
            return {false, 0, {}}; // can't represent as negative int64
        }
        dval = -static_cast<double>(value);
    } else {
        dval = static_cast<double>(value);
    }

    return {true, pos, JsonValue(JsonValue::Variant(dval))};
}

// ─── Float fast-path ─────────────────────────────────────────────────────────
//
// Scan the number to find its extent, then use fast_float or strtod.

ParseResult FastPaths::parse_float(const uint8_t* data, size_t remaining) noexcept {
    if (remaining == 0)
        return {false, 0, {}};

    // Quick check: must start with digit or '-'
    if (data[0] != '-' && (data[0] < '0' || data[0] > '9')) {
        return {false, 0, {}};
    }

    // Scan to find extent of number
    size_t pos = 0;
    bool has_dot = false;
    bool has_exp = false;

    if (data[pos] == '-')
        ++pos;
    if (pos >= remaining || data[pos] < '0' || data[pos] > '9')
        return {false, 0, {}};

    while (pos < remaining && data[pos] >= '0' && data[pos] <= '9')
        ++pos;

    if (pos < remaining && data[pos] == '.') {
        has_dot = true;
        ++pos;
        if (pos >= remaining || data[pos] < '0' || data[pos] > '9')
            return {false, 0, {}};
        while (pos < remaining && data[pos] >= '0' && data[pos] <= '9')
            ++pos;
    }

    if (pos < remaining && (data[pos] == 'e' || data[pos] == 'E')) {
        has_exp = true;
        ++pos;
        if (pos < remaining && (data[pos] == '+' || data[pos] == '-'))
            ++pos;
        if (pos >= remaining || data[pos] < '0' || data[pos] > '9')
            return {false, 0, {}};
        while (pos < remaining && data[pos] >= '0' && data[pos] <= '9')
            ++pos;
    }

    // Must have at least a decimal point or exponent to be a float
    if (!has_dot && !has_exp) {
        return {false, 0, {}}; // actually an integer
    }

    // Parse the number
    const char* start = reinterpret_cast<const char*>(data);
    const char* end_ptr = start + pos;
    double val = 0.0;

#if STRATA_HAS_FAST_FLOAT
    auto result = fast_float::from_chars(start, end_ptr, val);
    if (result.ec != std::errc{} || result.ptr != end_ptr) {
        return {false, 0, {}};
    }
#else
    // Fallback: copy to null-terminated buffer for strtod
    char buf[64];
    size_t n = std::min(pos, size_t{63});
    std::memcpy(buf, data, n);
    buf[n] = '\0';
    char* send = nullptr;
    val = std::strtod(buf, &send);
    if (send != buf + n)
        return {false, 0, {}};
#endif

    return {true, pos, JsonValue(JsonValue::Variant(val))};
}

// ─── Short string fast-path (≤32 bytes, no escapes) ─────────────────────────
//
// Strategy:
// 1. Verify opening quote
// 2. Scan byte-by-byte (or with SIMD if available) for '"' and '\'
// 3. If closing quote found within 32 bytes and no backslash → success
// 4. Copy content directly to string (no escape processing needed)

ParseResult FastPaths::parse_short_string(const uint8_t* data, size_t remaining) noexcept {
    if (remaining < 2 || data[0] != '"')
        return {false, 0, {}};

    // Scan for closing quote and escape characters.
    // We only check up to 33 bytes (opening quote + 32 content + closing quote).
    const size_t limit = std::min(remaining, size_t{34}); // 1 + 32 + 1

    for (size_t i = 1; i < limit; ++i) {
        if (data[i] == '\\') {
            // Escape found — this is a long string (or at least needs escape processing)
            return {false, 0, {}};
        }
        if (data[i] == '"') {
            // Found closing quote. String content is data[1..i-1], length = i-1.
            size_t slen = i - 1;
            if (slen > 32)
                return {false, 0, {}}; // shouldn't happen given our limit

            std::string s(reinterpret_cast<const char*>(data + 1), slen);
            return {true, i + 1, JsonValue(JsonValue::Variant(std::move(s)))};
        }
    }

    // No closing quote within 32 bytes — this is a long string
    return {false, 0, {}};
}

// ─── Long string fast-path ───────────────────────────────────────────────────
//
// Handles strings > 32 bytes and/or strings with escape sequences.
// Uses a single pass: scan for '\' and '"', process escapes inline.

ParseResult FastPaths::parse_long_string(const uint8_t* data, size_t remaining) noexcept {
    if (remaining < 2 || data[0] != '"')
        return {false, 0, {}};

    // Build the output string, processing escapes as we go.
    // Use the arena string builder for efficiency.
    std::string result;
    result.reserve(64); // reasonable starting size

    size_t pos = 1; // skip opening quote
    while (pos < remaining) {
        uint8_t c = data[pos];

        if (c == '"') {
            // Closing quote — done
            return {true, pos + 1, JsonValue(JsonValue::Variant(std::move(result)))};
        }

        if (c == '\\') {
            // Escape sequence
            if (pos + 1 >= remaining)
                return {false, 0, {}};

            uint8_t esc = data[pos + 1];
            switch (esc) {
            case '"':
                result += '"';
                pos += 2;
                break;
            case '\\':
                result += '\\';
                pos += 2;
                break;
            case '/':
                result += '/';
                pos += 2;
                break;
            case 'b':
                result += '\b';
                pos += 2;
                break;
            case 'f':
                result += '\f';
                pos += 2;
                break;
            case 'n':
                result += '\n';
                pos += 2;
                break;
            case 'r':
                result += '\r';
                pos += 2;
                break;
            case 't':
                result += '\t';
                pos += 2;
                break;
            case 'u': {
                // \uXXXX — parse 4 hex digits
                if (pos + 5 >= remaining)
                    return {false, 0, {}};
                uint16_t cp = 0;
                for (int j = 0; j < 4; ++j) {
                    uint8_t h = data[pos + 2 + j];
                    cp <<= 4;
                    if (h >= '0' && h <= '9')
                        cp |= (h - '0');
                    else if (h >= 'a' && h <= 'f')
                        cp |= (h - 'a' + 10);
                    else if (h >= 'A' && h <= 'F')
                        cp |= (h - 'A' + 10);
                    else
                        return {false, 0, {}};
                }
                // Encode as UTF-8
                if (cp < 0x80) {
                    result += static_cast<char>(cp);
                } else if (cp < 0x800) {
                    result += static_cast<char>(0xC0 | (cp >> 6));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                } else {
                    // Check for surrogate pair
                    if (cp >= 0xD800 && cp <= 0xDBFF) {
                        // High surrogate — expect \uDCxx-\uDFxx
                        if (pos + 11 >= remaining || data[pos + 6] != '\\' ||
                            data[pos + 7] != 'u') {
                            return {false, 0, {}};
                        }
                        uint16_t lo = 0;
                        for (int j = 0; j < 4; ++j) {
                            uint8_t h = data[pos + 8 + j];
                            lo <<= 4;
                            if (h >= '0' && h <= '9')
                                lo |= (h - '0');
                            else if (h >= 'a' && h <= 'f')
                                lo |= (h - 'a' + 10);
                            else if (h >= 'A' && h <= 'F')
                                lo |= (h - 'A' + 10);
                            else
                                return {false, 0, {}};
                        }
                        if (lo < 0xDC00 || lo > 0xDFFF)
                            return {false, 0, {}};
                        uint32_t full_cp =
                            0x10000 + ((static_cast<uint32_t>(cp - 0xD800) << 10) | (lo - 0xDC00));
                        result += static_cast<char>(0xF0 | (full_cp >> 18));
                        result += static_cast<char>(0x80 | ((full_cp >> 12) & 0x3F));
                        result += static_cast<char>(0x80 | ((full_cp >> 6) & 0x3F));
                        result += static_cast<char>(0x80 | (full_cp & 0x3F));
                        pos += 12; // skip both \uXXXX sequences
                        break;     // from switch
                    }
                    result += static_cast<char>(0xE0 | (cp >> 12));
                    result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                }
                if (!(cp >= 0xD800 && cp <= 0xDBFF)) {
                    pos += 6; // skip \uXXXX
                }
                break;
            }
            default:
                return {false, 0, {}}; // invalid escape
            }
        } else {
            // Regular character — scan ahead for the next special character
            // to batch-copy a run of plain characters.
            size_t run_start = pos;
            while (pos < remaining && data[pos] != '"' && data[pos] != '\\') {
                ++pos;
            }
            result.append(reinterpret_cast<const char*>(data + run_start), pos - run_start);
        }
    }

    // Reached end of input without closing quote
    return {false, 0, {}};
}

// ─── Bool/null literal parsers ───────────────────────────────────────────────
//
// These use 4/5-byte integer comparisons for branchless validation.
// The idea: load 4 bytes, compare against the expected literal as a uint32_t.
// If equal, success. One comparison instead of 4 branches.

ParseResult FastPaths::parse_bool_true(const uint8_t* data, size_t remaining) noexcept {
    if (remaining < 4)
        return {false, 0, {}};

    // "true" as a little-endian uint32_t: 't'=0x74, 'r'=0x72, 'u'=0x75, 'e'=0x65
    uint32_t word;
    std::memcpy(&word, data, 4);

    // Expected value depends on endianness. Use char-level comparison that
    // the compiler will optimize into a single 32-bit compare.
    static constexpr uint8_t expected[] = {'t', 'r', 'u', 'e'};
    uint32_t expected_word;
    std::memcpy(&expected_word, expected, 4);

    if (word != expected_word)
        return {false, 0, {}};

    // Verify termination
    if (remaining > 4 && !is_structural_or_ws(data[4]))
        return {false, 0, {}};

    return {true, 4, JsonValue(JsonValue::Variant(true))};
}

ParseResult FastPaths::parse_bool_false(const uint8_t* data, size_t remaining) noexcept {
    if (remaining < 5)
        return {false, 0, {}};

    // Check "false" — first 4 bytes as uint32_t, then 5th byte
    uint32_t word;
    std::memcpy(&word, data, 4);

    static constexpr uint8_t expected[] = {'f', 'a', 'l', 's'};
    uint32_t expected_word;
    std::memcpy(&expected_word, expected, 4);

    if (word != expected_word || data[4] != 'e')
        return {false, 0, {}};

    if (remaining > 5 && !is_structural_or_ws(data[5]))
        return {false, 0, {}};

    return {true, 5, JsonValue(JsonValue::Variant(false))};
}

ParseResult FastPaths::parse_null(const uint8_t* data, size_t remaining) noexcept {
    if (remaining < 4)
        return {false, 0, {}};

    uint32_t word;
    std::memcpy(&word, data, 4);

    static constexpr uint8_t expected[] = {'n', 'u', 'l', 'l'};
    uint32_t expected_word;
    std::memcpy(&expected_word, expected, 4);

    if (word != expected_word)
        return {false, 0, {}};

    if (remaining > 4 && !is_structural_or_ws(data[4]))
        return {false, 0, {}};

    return {true, 4, JsonValue(JsonValue::Variant(nullptr))};
}

// ─── Container openers ───────────────────────────────────────────────────────

ParseResult FastPaths::parse_object_open(const uint8_t* data, size_t remaining) noexcept {
    if (remaining > 0 && data[0] == '{') {
        return {true, 1, JsonValue()}; // value unused — coordinator handles object
    }
    return {false, 0, {}};
}

ParseResult FastPaths::parse_array_open(const uint8_t* data, size_t remaining) noexcept {
    if (remaining > 0 && data[0] == '[') {
        return {true, 1, JsonValue()}; // value unused — coordinator handles array
    }
    return {false, 0, {}};
}

// ─── Dispatch table ──────────────────────────────────────────────────────────

const std::array<FastPaths::ParseFn, FastPaths::NUM_TYPES>& FastPaths::dispatch_table() noexcept {
    static const std::array<ParseFn, NUM_TYPES> table = {
        &FastPaths::parse_integer,      // INTEGER      = 0
        &FastPaths::parse_float,        // FLOAT        = 1
        &FastPaths::parse_short_string, // SHORT_STRING = 2
        &FastPaths::parse_long_string,  // LONG_STRING  = 3
        &FastPaths::parse_bool_true,    // BOOL_TRUE    = 4
        &FastPaths::parse_bool_false,   // BOOL_FALSE   = 5
        &FastPaths::parse_null,         // NULL_VALUE   = 6
        &FastPaths::parse_object_open,  // OBJECT       = 7
        &FastPaths::parse_array_open,   // ARRAY        = 8
    };
    return table;
}

} // namespace strata::speculative

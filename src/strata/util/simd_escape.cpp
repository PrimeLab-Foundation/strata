/**
 * @file simd_escape.cpp
 * @brief SIMD-accelerated JSON string escaping.
 *
 * Implements fast string escaping for JSON serialization using SIMD instructions.
 * The escape_json_string() function handles:
 * - Quote (") → \"
 * - Backslash (\) → \\
 * - Control characters (0x00-0x1F) → \uXXXX or \n, \r, \t, etc.
 *
 * SIMD Strategy:
 * - Process 16/32 bytes at a time looking for characters that need escaping
 * - Use vector comparison to find bytes < 0x20 or == '"' or == '\\'
 * - Fast path: if no escaping needed, copy entire chunk
 * - Slow path: escape individual characters as needed
 *
 * Platform support:
 * - AVX2 (256-bit): x86_64 with AVX2
 * - SSE4.2 (128-bit): x86_64 without AVX2
 * - NEON (128-bit): ARM64
 * - Scalar fallback: All other platforms
 *
 * Performance notes:
 * - Typical JSON strings have few characters needing escape
 * - SIMD allows scanning 16-32 bytes in ~1 cycle
 * - ~3-5x faster than byte-by-byte scanning
 *
 * @see simd_string.hpp for public API
 */

#include "strata/util/simd_string.hpp"
#include <cstdio>
#include <vector>

// Detect SIMD support
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#if defined(__AVX2__)
#define STRATA_HAS_AVX2 1
#include <immintrin.h>
#elif defined(__SSE4_2__)
#define STRATA_HAS_SSE42 1
#include <emmintrin.h>
#include <nmmintrin.h>
#endif
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define STRATA_HAS_NEON 1
#include <arm_neon.h>
#endif

namespace strata {
namespace util {

// Characters that need escaping: 0x00-0x1F, '"' (0x22), '\\' (0x5C)
// We check for: c < 0x20 || c == '"' || c == '\\'

#ifdef STRATA_HAS_AVX2

// AVX2 implementation (32 bytes at a time)
static inline bool has_escape_chars_avx2(const char* str, size_t len) {
    const __m256i threshold = _mm256_set1_epi8(0x20);
    const __m256i quote = _mm256_set1_epi8('"');
    const __m256i backslash = _mm256_set1_epi8('\\');

    size_t i = 0;

    // Process 32 bytes at a time
    for (; i + 32 <= len; i += 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(str + i));

        // Check for control characters (< 0x20)
        __m256i control = _mm256_cmpgt_epi8(threshold, chunk);

        // Check for '"' and '\\'
        __m256i is_quote = _mm256_cmpeq_epi8(chunk, quote);
        __m256i is_backslash = _mm256_cmpeq_epi8(chunk, backslash);

        // Combine all checks
        __m256i needs_escape = _mm256_or_si256(control, _mm256_or_si256(is_quote, is_backslash));

        // If any byte needs escaping, movemask will be non-zero
        if (_mm256_movemask_epi8(needs_escape) != 0) {
            return true;
        }
    }

    // Check remaining bytes (scalar)
    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return true;
        }
    }

    return false;
}

// Find next escape position using AVX2
static inline size_t find_next_escape_avx2(const char* str, size_t len) {
    const __m256i threshold = _mm256_set1_epi8(0x20);
    const __m256i quote = _mm256_set1_epi8('"');
    const __m256i backslash = _mm256_set1_epi8('\\');

    size_t i = 0;

    // Process 32 bytes at a time
    for (; i + 32 <= len; i += 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(str + i));

        __m256i control = _mm256_cmpgt_epi8(threshold, chunk);
        __m256i is_quote = _mm256_cmpeq_epi8(chunk, quote);
        __m256i is_backslash = _mm256_cmpeq_epi8(chunk, backslash);
        __m256i needs_escape = _mm256_or_si256(control, _mm256_or_si256(is_quote, is_backslash));

        uint32_t mask = _mm256_movemask_epi8(needs_escape);
        if (mask != 0) {
            // Find first set bit (position of first escape)
            return i + __builtin_ctz(mask);
        }
    }

    // Scalar tail
    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return i;
        }
    }

    return len;
}

#elif defined(STRATA_HAS_SSE42)

// SSE4.2 implementation (16 bytes at a time)
static inline bool has_escape_chars_sse(const char* str, size_t len) {
    const __m128i threshold = _mm_set1_epi8(0x20);
    const __m128i quote = _mm_set1_epi8('"');
    const __m128i backslash = _mm_set1_epi8('\\');

    size_t i = 0;

    for (; i + 16 <= len; i += 16) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str + i));

        __m128i control = _mm_cmplt_epi8(chunk, threshold);
        __m128i is_quote = _mm_cmpeq_epi8(chunk, quote);
        __m128i is_backslash = _mm_cmpeq_epi8(chunk, backslash);
        __m128i needs_escape = _mm_or_si128(control, _mm_or_si128(is_quote, is_backslash));

        if (_mm_movemask_epi8(needs_escape) != 0) {
            return true;
        }
    }

    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return true;
        }
    }

    return false;
}

static inline size_t find_next_escape_sse(const char* str, size_t len) {
    const __m128i threshold = _mm_set1_epi8(0x20);
    const __m128i quote = _mm_set1_epi8('"');
    const __m128i backslash = _mm_set1_epi8('\\');

    size_t i = 0;

    for (; i + 16 <= len; i += 16) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str + i));

        __m128i control = _mm_cmplt_epi8(chunk, threshold);
        __m128i is_quote = _mm_cmpeq_epi8(chunk, quote);
        __m128i is_backslash = _mm_cmpeq_epi8(chunk, backslash);
        __m128i needs_escape = _mm_or_si128(control, _mm_or_si128(is_quote, is_backslash));

        uint16_t mask = _mm_movemask_epi8(needs_escape);
        if (mask != 0) {
            return i + __builtin_ctz(mask);
        }
    }

    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return i;
        }
    }

    return len;
}

#elif defined(STRATA_HAS_NEON)

// ARM NEON implementation (16 bytes at a time)
static inline bool has_escape_chars_neon(const char* str, size_t len) {
    const uint8x16_t threshold = vdupq_n_u8(0x20);
    const uint8x16_t quote = vdupq_n_u8('"');
    const uint8x16_t backslash = vdupq_n_u8('\\');

    size_t i = 0;

    for (; i + 16 <= len; i += 16) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(str + i));

        uint8x16_t control = vcltq_u8(chunk, threshold);
        uint8x16_t is_quote = vceqq_u8(chunk, quote);
        uint8x16_t is_backslash = vceqq_u8(chunk, backslash);
        uint8x16_t needs_escape = vorrq_u8(control, vorrq_u8(is_quote, is_backslash));

        // Check if any lane is non-zero
        uint64x2_t combined = vreinterpretq_u64_u8(needs_escape);
        if (vgetq_lane_u64(combined, 0) != 0 || vgetq_lane_u64(combined, 1) != 0) {
            return true;
        }
    }

    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return true;
        }
    }

    return false;
}

static inline size_t find_next_escape_neon(const char* str, size_t len) {
    const uint8x16_t threshold = vdupq_n_u8(0x20);
    const uint8x16_t quote = vdupq_n_u8('"');
    const uint8x16_t backslash = vdupq_n_u8('\\');

    size_t i = 0;

    for (; i + 16 <= len; i += 16) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(str + i));

        uint8x16_t control = vcltq_u8(chunk, threshold);
        uint8x16_t is_quote = vceqq_u8(chunk, quote);
        uint8x16_t is_backslash = vceqq_u8(chunk, backslash);
        uint8x16_t needs_escape = vorrq_u8(control, vorrq_u8(is_quote, is_backslash));

        // Use bit manipulation to find the first non-zero byte index
        uint64x2_t res64 = vreinterpretq_u64_u8(needs_escape);
        uint64_t low = vgetq_lane_u64(res64, 0);
        if (low) {
            return i + (static_cast<size_t>(__builtin_ctzll(low)) >> 3);
        }
        uint64_t high = vgetq_lane_u64(res64, 1);
        if (high) {
            return i + 8 + (static_cast<size_t>(__builtin_ctzll(high)) >> 3);
        }
    }

    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return i;
        }
    }

    return len;
}

#endif

// Scalar fallback for platforms without SIMD
[[maybe_unused]] static inline bool has_escape_chars_scalar(const char* str, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return true;
        }
    }
    return false;
}

[[maybe_unused]] static inline size_t find_next_escape_scalar(const char* str, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x20 || c == '"' || c == '\\') {
            return i;
        }
    }
    return len;
}

// Escape a single character
static inline void escape_char(unsigned char c, std::string& out) {
    switch (c) {
    case '"':
        out.append("\\\"");
        break;
    case '\\':
        out.append("\\\\");
        break;
    case '\b':
        out.append("\\b");
        break;
    case '\f':
        out.append("\\f");
        break;
    case '\n':
        out.append("\\n");
        break;
    case '\r':
        out.append("\\r");
        break;
    case '\t':
        out.append("\\t");
        break;
    default:
        if (c < 0x20) {
            char buf[7];
            snprintf(buf, sizeof(buf), "\\u%04x", c);
            out.append(buf);
        } else {
            out.push_back(c);
        }
    }
}

// Public API
bool try_copy_clean_string(const char* str, size_t len, std::string& out) {
    out.push_back('"');

    bool has_escapes;

#ifdef STRATA_HAS_AVX2
    has_escapes = has_escape_chars_avx2(str, len);
#elif defined(STRATA_HAS_SSE42)
    has_escapes = has_escape_chars_sse(str, len);
#elif defined(STRATA_HAS_NEON)
    has_escapes = has_escape_chars_neon(str, len);
#else
    has_escapes = has_escape_chars_scalar(str, len);
#endif

    if (!has_escapes) {
        // Fast path: copy entire string
        out.append(str, len);
        out.push_back('"');
        return true;
    }

    return false;
}

void escape_json_string_simd(const char* str, size_t len, std::string& out) {
    out.push_back('"');

    size_t pos = 0;
    while (pos < len) {
        // Find next character that needs escaping
        size_t next_escape;

#ifdef STRATA_HAS_AVX2
        next_escape = find_next_escape_avx2(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_SSE42)
        next_escape = find_next_escape_sse(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_NEON)
        next_escape = find_next_escape_neon(str + pos, len - pos) + pos;
#else
        next_escape = find_next_escape_scalar(str + pos, len - pos) + pos;
#endif

        // Copy clean chunk
        if (next_escape > pos) {
            out.append(str + pos, next_escape - pos);
        }

        // Escape the character if we found one
        if (next_escape < len) {
            escape_char(static_cast<unsigned char>(str[next_escape]), out);
            pos = next_escape + 1;
        } else {
            break;
        }
    }

    out.push_back('"');
}

size_t find_next_escape_simd(const char* str, size_t len) {
#ifdef STRATA_HAS_AVX2
    return find_next_escape_avx2(str, len);
#elif defined(STRATA_HAS_SSE42)
    return find_next_escape_sse(str, len);
#elif defined(STRATA_HAS_NEON)
    return find_next_escape_neon(str, len);
#else
    return find_next_escape_scalar(str, len);
#endif
}

// OutputBuffer overloads (streaming serializer)
static inline void escape_char_buffer(unsigned char c, OutputBuffer& out) {
    switch (c) {
    case '"':
        out.append("\\\"", 2);
        break;
    case '\\':
        out.append("\\\\", 2);
        break;
    case '\b':
        out.append("\\b", 2);
        break;
    case '\f':
        out.append("\\f", 2);
        break;
    case '\n':
        out.append("\\n", 2);
        break;
    case '\r':
        out.append("\\r", 2);
        break;
    case '\t':
        out.append("\\t", 2);
        break;
    default:
        if (c < 0x20) {
            static const char hex[] = "0123456789abcdef";
            char buf[6] = {'\\', 'u', '0', '0', hex[c >> 4], hex[c & 0x0F]};
            out.append(buf, sizeof(buf));
        } else {
            out.push_back(static_cast<char>(c));
        }
    }
}

static inline void escape_char_fixed(unsigned char c, FixedOutputBuffer& out) {
    switch (c) {
    case '"':
        out.append("\\\"", 2);
        break;
    case '\\':
        out.append("\\\\", 2);
        break;
    case '\b':
        out.append("\\b", 2);
        break;
    case '\f':
        out.append("\\f", 2);
        break;
    case '\n':
        out.append("\\n", 2);
        break;
    case '\r':
        out.append("\\r", 2);
        break;
    case '\t':
        out.append("\\t", 2);
        break;
    default:
        if (c < 0x20) {
            static const char hex[] = "0123456789abcdef";
            char buf[6] = {'\\', 'u', '0', '0', hex[c >> 4], hex[c & 0x0F]};
            out.append(buf, sizeof(buf));
        } else {
            out.push_back(static_cast<char>(c));
        }
    }
}

bool try_copy_clean_string(const char* str, size_t len, OutputBuffer& out) {
    bool has_escapes;

#ifdef STRATA_HAS_AVX2
    has_escapes = has_escape_chars_avx2(str, len);
#elif defined(STRATA_HAS_SSE42)
    has_escapes = has_escape_chars_sse(str, len);
#elif defined(STRATA_HAS_NEON)
    has_escapes = has_escape_chars_neon(str, len);
#else
    has_escapes = has_escape_chars_scalar(str, len);
#endif

    if (has_escapes) {
        return false;
    }

    out.reserve(out.size() + len + 2);
    out.push_back('"');
    out.append(str, len);
    out.push_back('"');
    return true;
}

bool try_copy_clean_string(const char* str, size_t len, FixedOutputBuffer& out) {
    bool has_escapes;

#ifdef STRATA_HAS_AVX2
    has_escapes = has_escape_chars_avx2(str, len);
#elif defined(STRATA_HAS_SSE42)
    has_escapes = has_escape_chars_sse(str, len);
#elif defined(STRATA_HAS_NEON)
    has_escapes = has_escape_chars_neon(str, len);
#else
    has_escapes = has_escape_chars_scalar(str, len);
#endif

    if (has_escapes) {
        return false;
    }

    out.push_back('"');
    out.append(str, len);
    out.push_back('"');
    return true;
}

void escape_json_string_simd(const char* str, size_t len, OutputBuffer& out) {
    out.reserve(out.size() + len + 2);
    out.push_back('"');

    size_t pos = 0;
    while (pos < len) {
        // Find next character that needs escaping
        size_t next_escape;

#ifdef STRATA_HAS_AVX2
        next_escape = find_next_escape_avx2(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_SSE42)
        next_escape = find_next_escape_sse(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_NEON)
        next_escape = find_next_escape_neon(str + pos, len - pos) + pos;
#else
        next_escape = find_next_escape_scalar(str + pos, len - pos) + pos;
#endif

        // Copy clean chunk
        if (next_escape > pos) {
            out.append(str + pos, next_escape - pos);
        }

        // Escape the character if we found one
        if (next_escape < len) {
            escape_char_buffer(static_cast<unsigned char>(str[next_escape]), out);
            pos = next_escape + 1;
        } else {
            break;
        }
    }

    out.push_back('"');
}

void escape_json_string_simd(const char* str, size_t len, FixedOutputBuffer& out) {
    out.push_back('"');

    size_t pos = 0;
    while (pos < len) {
        size_t next_escape;

#ifdef STRATA_HAS_AVX2
        next_escape = find_next_escape_avx2(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_SSE42)
        next_escape = find_next_escape_sse(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_NEON)
        next_escape = find_next_escape_neon(str + pos, len - pos) + pos;
#else
        next_escape = find_next_escape_scalar(str + pos, len - pos) + pos;
#endif

        if (next_escape > pos) {
            out.append(str + pos, next_escape - pos);
        }

        if (next_escape < len) {
            escape_char_fixed(static_cast<unsigned char>(str[next_escape]), out);
            pos = next_escape + 1;
        } else {
            break;
        }
    }

    out.push_back('"');
}

// Single-pass string escaping/copying - avoids double-scan pattern
// Uses has_escape_chars for fast boolean check, then find_next_escape only when escapes exist
void escape_or_copy_string_simd(const char* str, size_t len, OutputBuffer& out) {
    // Fast boolean check first (faster than find_next_escape for clean strings)
    bool has_escapes;
#ifdef STRATA_HAS_AVX2
    has_escapes = has_escape_chars_avx2(str, len);
#elif defined(STRATA_HAS_SSE42)
    has_escapes = has_escape_chars_sse(str, len);
#elif defined(STRATA_HAS_NEON)
    has_escapes = has_escape_chars_neon(str, len);
#else
    has_escapes = has_escape_chars_scalar(str, len);
#endif

    // Reserve space for string + quotes (+ some margin for escapes if needed)
    out.reserve(out.size() + len + 2 + (has_escapes ? len / 8 : 0));
    out.push_back('"');

    if (!has_escapes) {
        // Fast path: no escapes, copy entire string directly
        out.append(str, len);
    } else {
        // Escape path: use find_next_escape to process chunks
        size_t pos = 0;
        while (pos < len) {
            size_t next_escape;
#ifdef STRATA_HAS_AVX2
            next_escape = find_next_escape_avx2(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_SSE42)
            next_escape = find_next_escape_sse(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_NEON)
            next_escape = find_next_escape_neon(str + pos, len - pos) + pos;
#else
            next_escape = find_next_escape_scalar(str + pos, len - pos) + pos;
#endif

            // Copy clean chunk
            if (next_escape > pos) {
                out.append(str + pos, next_escape - pos);
            }

            // Escape the character if we found one
            if (next_escape < len) {
                escape_char_buffer(static_cast<unsigned char>(str[next_escape]), out);
                pos = next_escape + 1;
            } else {
                break;
            }
        }
    }

    out.push_back('"');
}

void escape_or_copy_string_simd(const char* str, size_t len, FixedOutputBuffer& out) {
    // Fast boolean check first (faster than find_next_escape for clean strings)
    bool has_escapes;
#ifdef STRATA_HAS_AVX2
    has_escapes = has_escape_chars_avx2(str, len);
#elif defined(STRATA_HAS_SSE42)
    has_escapes = has_escape_chars_sse(str, len);
#elif defined(STRATA_HAS_NEON)
    has_escapes = has_escape_chars_neon(str, len);
#else
    has_escapes = has_escape_chars_scalar(str, len);
#endif

    out.push_back('"');

    if (!has_escapes) {
        // Fast path: no escapes, copy entire string directly
        out.append(str, len);
    } else {
        // Escape path: use find_next_escape to process chunks
        size_t pos = 0;
        while (pos < len) {
            size_t next_escape;
#ifdef STRATA_HAS_AVX2
            next_escape = find_next_escape_avx2(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_SSE42)
            next_escape = find_next_escape_sse(str + pos, len - pos) + pos;
#elif defined(STRATA_HAS_NEON)
            next_escape = find_next_escape_neon(str + pos, len - pos) + pos;
#else
            next_escape = find_next_escape_scalar(str + pos, len - pos) + pos;
#endif

            // Copy clean chunk
            if (next_escape > pos) {
                out.append(str + pos, next_escape - pos);
            }

            // Escape the character if we found one
            if (next_escape < len) {
                escape_char_fixed(static_cast<unsigned char>(str[next_escape]), out);
                pos = next_escape + 1;
            } else {
                break;
            }
        }
    }

    out.push_back('"');
}

} // namespace util
} // namespace strata

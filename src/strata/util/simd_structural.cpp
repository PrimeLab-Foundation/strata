#include "strata/util/simd_string.hpp"

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

// Whitespace set for JSON: space, tab, LF, CR
static inline bool is_ws_scalar(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static inline bool is_structural_scalar(unsigned char c) {
    switch (c) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ',':
    case ':':
    case '"':
        return true;
    default:
        return false;
    }
}

[[maybe_unused]] static inline void append_structurals_from_mask(
    uint32_t mask, size_t base, std::vector<size_t>& positions) {
    while (mask != 0) {
        unsigned int bit = __builtin_ctz(mask);
        positions.push_back(base + static_cast<size_t>(bit));
        mask &= (mask - 1);
    }
}

#ifdef STRATA_HAS_AVX2

size_t find_next_structural_simd(const char* data, size_t len, size_t start_pos) {
    const __m256i space = _mm256_set1_epi8(' ');
    const __m256i tab = _mm256_set1_epi8('\t');
    const __m256i lf = _mm256_set1_epi8('\n');
    const __m256i cr = _mm256_set1_epi8('\r');

    const __m256i lbrace = _mm256_set1_epi8('{');
    const __m256i rbrace = _mm256_set1_epi8('}');
    const __m256i lbrack = _mm256_set1_epi8('[');
    const __m256i rbrack = _mm256_set1_epi8(']');
    const __m256i comma = _mm256_set1_epi8(',');
    const __m256i colon = _mm256_set1_epi8(':');

    size_t i = start_pos;
    for (; i + 32 <= len; i += 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i));

        __m256i is_ws = _mm256_or_si256(_mm256_cmpeq_epi8(chunk, space),
                                        _mm256_or_si256(_mm256_cmpeq_epi8(chunk, tab),
                                                        _mm256_or_si256(_mm256_cmpeq_epi8(chunk, lf),
                                                                        _mm256_cmpeq_epi8(chunk, cr))));

        __m256i is_struct =
            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, lbrace),
                            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, rbrace),
                                            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, lbrack),
                                                            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, rbrack),
                                                                            _mm256_or_si256(
                                                                                _mm256_cmpeq_epi8(chunk, comma),
                                                                                _mm256_cmpeq_epi8(chunk, colon))))));

        uint32_t ws_mask = static_cast<uint32_t>(_mm256_movemask_epi8(is_ws));
        uint32_t struct_mask = static_cast<uint32_t>(_mm256_movemask_epi8(is_struct));
        uint32_t non_ws_mask = ~ws_mask;

        if (non_ws_mask == 0) {
            continue; // all whitespace in this block
        }

        unsigned int bit = __builtin_ctz(non_ws_mask);
        size_t pos = i + static_cast<size_t>(bit);

        // Fast-path: if the first non-whitespace is structural, we can return immediately.
        if (struct_mask & (1u << bit)) {
            return pos;
        }
        return pos;
    }

    // Tail (scalar)
    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        if (!is_ws_scalar(c)) {
            return i;
        }
    }
    return len;
}

void collect_structural_positions_simd(const char* data, size_t len,
                                       std::vector<size_t>& positions) {
    positions.clear();
    if (len == 0) {
        return;
    }

    const __m256i lbrace = _mm256_set1_epi8('{');
    const __m256i rbrace = _mm256_set1_epi8('}');
    const __m256i lbrack = _mm256_set1_epi8('[');
    const __m256i rbrack = _mm256_set1_epi8(']');
    const __m256i comma = _mm256_set1_epi8(',');
    const __m256i colon = _mm256_set1_epi8(':');
    const __m256i quote = _mm256_set1_epi8('"');

    size_t i = 0;
    for (; i + 32 <= len; i += 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i));

        __m256i is_struct =
            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, lbrace),
                            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, rbrace),
                                            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, lbrack),
                                                            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, rbrack),
                                                                            _mm256_or_si256(
                                                                                _mm256_cmpeq_epi8(chunk, comma),
                                                                                _mm256_or_si256(
                                                                                    _mm256_cmpeq_epi8(chunk, colon),
                                                                                    _mm256_cmpeq_epi8(chunk, quote)))))));

        uint32_t mask = static_cast<uint32_t>(_mm256_movemask_epi8(is_struct));
        if (mask != 0) {
            append_structurals_from_mask(mask, i, positions);
        }
    }

    for (; i < len; ++i) {
        if (is_structural_scalar(static_cast<unsigned char>(data[i]))) {
            positions.push_back(i);
        }
    }
}

#elif defined(STRATA_HAS_SSE42)

size_t find_next_structural_simd(const char* data, size_t len, size_t start_pos) {
    const __m128i space = _mm_set1_epi8(' ');
    const __m128i tab = _mm_set1_epi8('\t');
    const __m128i lf = _mm_set1_epi8('\n');
    const __m128i cr = _mm_set1_epi8('\r');
    const __m128i lbrace = _mm_set1_epi8('{');
    const __m128i rbrace = _mm_set1_epi8('}');
    const __m128i lbrack = _mm_set1_epi8('[');
    const __m128i rbrack = _mm_set1_epi8(']');
    const __m128i comma = _mm_set1_epi8(',');
    const __m128i colon = _mm_set1_epi8(':');

    size_t i = start_pos;
    for (; i + 16 <= len; i += 16) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + i));

        __m128i is_ws = _mm_or_si128(_mm_cmpeq_epi8(chunk, space),
                                     _mm_or_si128(_mm_cmpeq_epi8(chunk, tab),
                                                  _mm_or_si128(_mm_cmpeq_epi8(chunk, lf),
                                                               _mm_cmpeq_epi8(chunk, cr))));

        __m128i is_struct =
            _mm_or_si128(_mm_cmpeq_epi8(chunk, lbrace),
                         _mm_or_si128(_mm_cmpeq_epi8(chunk, rbrace),
                                      _mm_or_si128(_mm_cmpeq_epi8(chunk, lbrack),
                                                   _mm_or_si128(_mm_cmpeq_epi8(chunk, rbrack),
                                                                _mm_or_si128(_mm_cmpeq_epi8(chunk, comma),
                                                                             _mm_cmpeq_epi8(chunk, colon))))));

        uint16_t ws_mask = static_cast<uint16_t>(_mm_movemask_epi8(is_ws));
        uint16_t struct_mask = static_cast<uint16_t>(_mm_movemask_epi8(is_struct));
        uint16_t non_ws_mask = static_cast<uint16_t>(~ws_mask);
        if (non_ws_mask == 0) {
            continue;
        }
        unsigned int bit = __builtin_ctz(non_ws_mask);
        if (struct_mask & (1u << bit)) {
            return i + static_cast<size_t>(bit);
        }
        return i + static_cast<size_t>(bit);
    }

    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        if (!is_ws_scalar(c)) {
            return i;
        }
    }
    return len;
}

void collect_structural_positions_simd(const char* data, size_t len,
                                       std::vector<size_t>& positions) {
    positions.clear();
    if (len == 0) {
        return;
    }

    const __m128i lbrace = _mm_set1_epi8('{');
    const __m128i rbrace = _mm_set1_epi8('}');
    const __m128i lbrack = _mm_set1_epi8('[');
    const __m128i rbrack = _mm_set1_epi8(']');
    const __m128i comma = _mm_set1_epi8(',');
    const __m128i colon = _mm_set1_epi8(':');
    const __m128i quote = _mm_set1_epi8('"');

    size_t i = 0;
    for (; i + 16 <= len; i += 16) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + i));

        __m128i is_struct =
            _mm_or_si128(_mm_cmpeq_epi8(chunk, lbrace),
                         _mm_or_si128(_mm_cmpeq_epi8(chunk, rbrace),
                                      _mm_or_si128(_mm_cmpeq_epi8(chunk, lbrack),
                                                   _mm_or_si128(_mm_cmpeq_epi8(chunk, rbrack),
                                                                _mm_or_si128(_mm_cmpeq_epi8(chunk, comma),
                                                                             _mm_or_si128(
                                                                                 _mm_cmpeq_epi8(chunk, colon),
                                                                                 _mm_cmpeq_epi8(chunk, quote)))))));

        uint32_t mask = static_cast<uint32_t>(_mm_movemask_epi8(is_struct));
        if (mask != 0) {
            append_structurals_from_mask(mask, i, positions);
        }
    }

    for (; i < len; ++i) {
        if (is_structural_scalar(static_cast<unsigned char>(data[i]))) {
            positions.push_back(i);
        }
    }
}

#elif defined(STRATA_HAS_NEON)

size_t find_next_structural_simd(const char* data, size_t len, size_t start_pos) {
    const uint8x16_t space = vdupq_n_u8(' ');
    const uint8x16_t tab = vdupq_n_u8('\t');
    const uint8x16_t lf = vdupq_n_u8('\n');
    const uint8x16_t cr = vdupq_n_u8('\r');
    const uint8x16_t lbrace = vdupq_n_u8('{');
    const uint8x16_t rbrace = vdupq_n_u8('}');
    const uint8x16_t lbrack = vdupq_n_u8('[');
    const uint8x16_t rbrack = vdupq_n_u8(']');
    const uint8x16_t comma = vdupq_n_u8(',');
    const uint8x16_t colon = vdupq_n_u8(':');

    size_t i = start_pos;
    for (; i + 16 <= len; i += 16) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data + i));
        uint8x16_t is_ws = vorrq_u8(vorrq_u8(vceqq_u8(chunk, space), vceqq_u8(chunk, tab)),
                                    vorrq_u8(vceqq_u8(chunk, lf), vceqq_u8(chunk, cr)));

        uint8x16_t is_struct =
            vorrq_u8(vorrq_u8(vceqq_u8(chunk, lbrace), vceqq_u8(chunk, rbrace)),
                     vorrq_u8(vceqq_u8(chunk, lbrack),
                              vorrq_u8(vceqq_u8(chunk, rbrack),
                                       vorrq_u8(vceqq_u8(chunk, comma), vceqq_u8(chunk, colon)))));

        uint8x16_t non_ws = vmvnq_u8(is_ws);
        uint8_t tmp[16];
        vst1q_u8(tmp, non_ws);
        uint8_t tmp_struct[16];
        vst1q_u8(tmp_struct, is_struct);

        for (int j = 0; j < 16; ++j) {
            if (tmp[j]) {
                // Touch struct flag to avoid unused warning and keep parity with x86 paths.
                (void)tmp_struct[j];
                return i + static_cast<size_t>(j);
            }
        }
    }

    for (; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        if (!is_ws_scalar(c)) {
            return i;
        }
    }
    return len;
}

void collect_structural_positions_simd(const char* data, size_t len,
                                       std::vector<size_t>& positions) {
    positions.clear();
    if (len == 0) {
        return;
    }

    const uint8x16_t lbrace = vdupq_n_u8('{');
    const uint8x16_t rbrace = vdupq_n_u8('}');
    const uint8x16_t lbrack = vdupq_n_u8('[');
    const uint8x16_t rbrack = vdupq_n_u8(']');
    const uint8x16_t comma = vdupq_n_u8(',');
    const uint8x16_t colon = vdupq_n_u8(':');
    const uint8x16_t quote = vdupq_n_u8('"');

    size_t i = 0;
    for (; i + 16 <= len; i += 16) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data + i));
        uint8x16_t is_struct =
            vorrq_u8(vorrq_u8(vceqq_u8(chunk, lbrace), vceqq_u8(chunk, rbrace)),
                     vorrq_u8(vceqq_u8(chunk, lbrack),
                              vorrq_u8(vceqq_u8(chunk, rbrack),
                                       vorrq_u8(vceqq_u8(chunk, comma),
                                                vorrq_u8(vceqq_u8(chunk, colon),
                                                         vceqq_u8(chunk, quote))))));

        uint8_t tmp[16];
        vst1q_u8(tmp, is_struct);
        for (int j = 0; j < 16; ++j) {
            if (tmp[j]) {
                positions.push_back(i + static_cast<size_t>(j));
            }
        }
    }

    for (; i < len; ++i) {
        if (is_structural_scalar(static_cast<unsigned char>(data[i]))) {
            positions.push_back(i);
        }
    }
}

#else

size_t find_next_structural_simd(const char* data, size_t len, size_t start_pos) {
    for (size_t i = start_pos; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        if (!is_ws_scalar(c)) {
            return i;
        }
    }
    return len;
}

void collect_structural_positions_simd(const char* data, size_t len,
                                       std::vector<size_t>& positions) {
    positions.clear();
    if (len == 0) {
        return;
    }

    for (size_t i = 0; i < len; ++i) {
        if (is_structural_scalar(static_cast<unsigned char>(data[i]))) {
            positions.push_back(i);
        }
    }
}

#endif

} // namespace util
} // namespace strata

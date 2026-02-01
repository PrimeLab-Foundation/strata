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

// ============================================================================
// UTF-8 validation (RFC 3629: reject overlong, surrogates, > U+10FFFF)
// ============================================================================

// Byte classification: 0=ASCII, 1=continuation, 2=2B lead, 3=3B lead, 4=4B lead, 5=invalid
static constexpr unsigned char kUtf8Ascii = 0;
static constexpr unsigned char kUtf8Cont = 1;
static constexpr unsigned char kUtf8Lead2 = 2;
static constexpr unsigned char kUtf8Lead3 = 3;
static constexpr unsigned char kUtf8Lead4 = 4;
static constexpr unsigned char kUtf8Invalid = 5;

static inline unsigned char utf8_byte_type(unsigned char b) {
    if (b <= 0x7F)
        return kUtf8Ascii;
    if (b >= 0x80 && b <= 0xBF)
        return kUtf8Cont;
    if (b >= 0xC2 && b <= 0xDF)
        return kUtf8Lead2;
    if (b >= 0xE0 && b <= 0xEF)
        return kUtf8Lead3;
    if (b >= 0xF0 && b <= 0xF4)
        return kUtf8Lead4;
    return kUtf8Invalid;
}

// Scalar UTF-8 validator with full second-byte checks (E0/ED/F0/F4)
[[maybe_unused]] static bool validate_utf8_scalar(const char* data, size_t len) {
    size_t i = 0;
    int expected_cont = 0;
    unsigned char lead_byte = 0;

    while (i < len) {
        unsigned char b = static_cast<unsigned char>(data[i]);
        unsigned char t = utf8_byte_type(b);

        if (expected_cont > 0) {
            if (t != kUtf8Cont)
                return false;
            if (expected_cont == 2) {
                // First continuation after 3-byte lead: E0 -> A0-BF, ED -> 80-9F
                if (lead_byte == 0xE0 && b < 0xA0)
                    return false;
                if (lead_byte == 0xED && b > 0x9F)
                    return false;
            } else if (expected_cont == 3) {
                // First continuation after 4-byte lead: F0 -> 90-BF, F4 -> 80-8F
                if (lead_byte == 0xF0 && b < 0x90)
                    return false;
                if (lead_byte == 0xF4 && b > 0x8F)
                    return false;
            }
            expected_cont--;
            i++;
            continue;
        }

        switch (t) {
        case kUtf8Ascii:
            i++;
            break;
        case kUtf8Cont:
            return false;
        case kUtf8Lead2:
            expected_cont = 1;
            lead_byte = b;
            i++;
            break;
        case kUtf8Lead3:
            expected_cont = 2;
            lead_byte = b;
            i++;
            break;
        case kUtf8Lead4:
            expected_cont = 3;
            lead_byte = b;
            i++;
            break;
        default:
            return false;
        }
    }
    return expected_cont == 0;
}

#ifdef STRATA_HAS_AVX2

static bool validate_utf8_avx2(const char* data, size_t len) {
    size_t i = 0;
    int expected_cont = 0;
    unsigned char lead_byte = 0;
    unsigned char type_buf[32];

    for (; i + 32 <= len; i += 32) {
        // Classify 32 bytes (state machine needs types + raw bytes for E0/ED/F0/F4)
        for (int j = 0; j < 32; ++j) {
            type_buf[j] = utf8_byte_type(static_cast<unsigned char>(data[i + j]));
        }

        for (int j = 0; j < 32; ++j) {
            unsigned char t = type_buf[j];
            unsigned char b = static_cast<unsigned char>(data[i + j]);

            if (expected_cont > 0) {
                if (t != kUtf8Cont)
                    return false;
                if (expected_cont == 2) {
                    if (lead_byte == 0xE0 && b < 0xA0)
                        return false;
                    if (lead_byte == 0xED && b > 0x9F)
                        return false;
                } else if (expected_cont == 3) {
                    if (lead_byte == 0xF0 && b < 0x90)
                        return false;
                    if (lead_byte == 0xF4 && b > 0x8F)
                        return false;
                }
                expected_cont--;
                continue;
            }
            switch (t) {
            case kUtf8Ascii:
                break;
            case kUtf8Cont:
                return false;
            case kUtf8Lead2:
                expected_cont = 1;
                lead_byte = b;
                break;
            case kUtf8Lead3:
                expected_cont = 2;
                lead_byte = b;
                break;
            case kUtf8Lead4:
                expected_cont = 3;
                lead_byte = b;
                break;
            default:
                return false;
            }
        }
    }

    for (; i < len; ++i) {
        unsigned char b = static_cast<unsigned char>(data[i]);
        unsigned char t = utf8_byte_type(b);
        if (expected_cont > 0) {
            if (t != kUtf8Cont)
                return false;
            if (expected_cont == 2) {
                if (lead_byte == 0xE0 && b < 0xA0)
                    return false;
                if (lead_byte == 0xED && b > 0x9F)
                    return false;
            } else if (expected_cont == 3) {
                if (lead_byte == 0xF0 && b < 0x90)
                    return false;
                if (lead_byte == 0xF4 && b > 0x8F)
                    return false;
            }
            expected_cont--;
            continue;
        }
        switch (t) {
        case kUtf8Ascii:
            break;
        case kUtf8Cont:
            return false;
        case kUtf8Lead2:
            expected_cont = 1;
            lead_byte = b;
            break;
        case kUtf8Lead3:
            expected_cont = 2;
            lead_byte = b;
            break;
        case kUtf8Lead4:
            expected_cont = 3;
            lead_byte = b;
            break;
        default:
            return false;
        }
    }
    return expected_cont == 0;
}

#elif defined(STRATA_HAS_SSE42)

static bool validate_utf8_sse(const char* data, size_t len) {
    size_t i = 0;
    int expected_cont = 0;
    unsigned char lead_byte = 0;

    for (; i + 16 <= len; i += 16) {
        for (int j = 0; j < 16; ++j) {
            unsigned char b = static_cast<unsigned char>(data[i + j]);
            unsigned char t = utf8_byte_type(b);
            if (expected_cont > 0) {
                if (t != kUtf8Cont)
                    return false;
                if (expected_cont == 2) {
                    if (lead_byte == 0xE0 && b < 0xA0)
                        return false;
                    if (lead_byte == 0xED && b > 0x9F)
                        return false;
                } else if (expected_cont == 3) {
                    if (lead_byte == 0xF0 && b < 0x90)
                        return false;
                    if (lead_byte == 0xF4 && b > 0x8F)
                        return false;
                }
                expected_cont--;
                continue;
            }
            switch (t) {
            case kUtf8Ascii:
                break;
            case kUtf8Cont:
                return false;
            case kUtf8Lead2:
                expected_cont = 1;
                lead_byte = b;
                break;
            case kUtf8Lead3:
                expected_cont = 2;
                lead_byte = b;
                break;
            case kUtf8Lead4:
                expected_cont = 3;
                lead_byte = b;
                break;
            default:
                return false;
            }
        }
    }
    for (; i < len; ++i) {
        unsigned char b = static_cast<unsigned char>(data[i]);
        unsigned char t = utf8_byte_type(b);
        if (expected_cont > 0) {
            if (t != kUtf8Cont)
                return false;
            if (expected_cont == 2) {
                if (lead_byte == 0xE0 && b < 0xA0)
                    return false;
                if (lead_byte == 0xED && b > 0x9F)
                    return false;
            } else if (expected_cont == 3) {
                if (lead_byte == 0xF0 && b < 0x90)
                    return false;
                if (lead_byte == 0xF4 && b > 0x8F)
                    return false;
            }
            expected_cont--;
            continue;
        }
        switch (t) {
        case kUtf8Ascii:
            break;
        case kUtf8Cont:
            return false;
        case kUtf8Lead2:
            expected_cont = 1;
            lead_byte = b;
            break;
        case kUtf8Lead3:
            expected_cont = 2;
            lead_byte = b;
            break;
        case kUtf8Lead4:
            expected_cont = 3;
            lead_byte = b;
            break;
        default:
            return false;
        }
    }
    return expected_cont == 0;
}

#elif defined(STRATA_HAS_NEON)

static bool validate_utf8_neon(const char* data, size_t len) {
    size_t i = 0;
    int expected_cont = 0;
    unsigned char lead_byte = 0;

    for (; i + 16 <= len; i += 16) {
        for (int j = 0; j < 16; ++j) {
            unsigned char b = static_cast<unsigned char>(data[i + j]);
            unsigned char t = utf8_byte_type(b);
            if (expected_cont > 0) {
                if (t != kUtf8Cont)
                    return false;
                if (expected_cont == 2) {
                    if (lead_byte == 0xE0 && b < 0xA0)
                        return false;
                    if (lead_byte == 0xED && b > 0x9F)
                        return false;
                } else if (expected_cont == 3) {
                    if (lead_byte == 0xF0 && b < 0x90)
                        return false;
                    if (lead_byte == 0xF4 && b > 0x8F)
                        return false;
                }
                expected_cont--;
                continue;
            }
            switch (t) {
            case kUtf8Ascii:
                break;
            case kUtf8Cont:
                return false;
            case kUtf8Lead2:
                expected_cont = 1;
                lead_byte = b;
                break;
            case kUtf8Lead3:
                expected_cont = 2;
                lead_byte = b;
                break;
            case kUtf8Lead4:
                expected_cont = 3;
                lead_byte = b;
                break;
            default:
                return false;
            }
        }
    }
    for (; i < len; ++i) {
        unsigned char b = static_cast<unsigned char>(data[i]);
        unsigned char t = utf8_byte_type(b);
        if (expected_cont > 0) {
            if (t != kUtf8Cont)
                return false;
            if (expected_cont == 2) {
                if (lead_byte == 0xE0 && b < 0xA0)
                    return false;
                if (lead_byte == 0xED && b > 0x9F)
                    return false;
            } else if (expected_cont == 3) {
                if (lead_byte == 0xF0 && b < 0x90)
                    return false;
                if (lead_byte == 0xF4 && b > 0x8F)
                    return false;
            }
            expected_cont--;
            continue;
        }
        switch (t) {
        case kUtf8Ascii:
            break;
        case kUtf8Cont:
            return false;
        case kUtf8Lead2:
            expected_cont = 1;
            lead_byte = b;
            break;
        case kUtf8Lead3:
            expected_cont = 2;
            lead_byte = b;
            break;
        case kUtf8Lead4:
            expected_cont = 3;
            lead_byte = b;
            break;
        default:
            return false;
        }
    }
    return expected_cont == 0;
}

#endif

bool validate_utf8_simd(const char* data, size_t len) {
#ifdef STRATA_HAS_AVX2
    return validate_utf8_avx2(data, len);
#elif defined(STRATA_HAS_SSE42)
    return validate_utf8_sse(data, len);
#elif defined(STRATA_HAS_NEON)
    return validate_utf8_neon(data, len);
#else
    return validate_utf8_scalar(data, len);
#endif
}

} // namespace util
} // namespace strata

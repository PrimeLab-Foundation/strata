 #include "strata/util/simd_numbers.hpp"

 #include <cstdint>
 #include <cstring>
 #include <limits>

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

 static inline bool is_digit_scalar(unsigned char c) { return c >= '0' && c <= '9'; }

 // ============================================================================
 // SWAR (SIMD Within A Register) helpers for fast integer parsing
 // ============================================================================


 // Parse 8 ASCII digits to uint64_t using SWAR
 // Assumes all 8 bytes are valid digits '0'-'9'
 // Returns value in range [0, 99999999]
 static inline uint64_t parse_8_digits_swar(uint64_t chunk) {
     // Mask to extract lower 4 bits of each byte (digit value)
     const uint64_t mask_lo = 0x0F0F0F0F0F0F0F0FULL;

     // Extract digit values (0-9) from ASCII
     uint64_t digits = chunk & mask_lo;

     // Combine pairs: d0*10 + d1, d2*10 + d3, etc.
     // Magic: multiply by (256*10 + 1) = 2561, then shift right 8
     // This gives: high_byte*10 + low_byte in each 16-bit word

     // Step 1: Combine pairs of digits into bytes
     // d0*10 + d1, d2*10 + d3, d4*10 + d5, d6*10 + d7
     uint64_t pairs = ((digits * 2561) >> 8) & 0x00FF00FF00FF00FFULL;

     // Step 2: Combine pairs into 16-bit values
     // (d0*10+d1)*100 + (d2*10+d3), etc.
     uint64_t quads = ((pairs * 6553601) >> 16) & 0x0000FFFF0000FFFFULL;

     // Step 3: Combine into final 32-bit values
     // ((d0..d3)*10000 + (d4..d7))
     uint64_t result = ((quads * 42949672960001ULL) >> 32) & 0x00000000FFFFFFFFULL;

     return result;
 }

 // Parse 4 ASCII digits to uint32_t using SWAR
 // Assumes all 4 bytes are valid digits '0'-'9'
 // Returns value in range [0, 9999]
 static inline uint32_t parse_4_digits_swar(uint32_t chunk) {
     // Mask to extract lower 4 bits of each byte
     const uint32_t mask_lo = 0x0F0F0F0FU;

     // Extract digit values
     uint32_t digits = chunk & mask_lo;

     // Combine pairs: multiply by 2561 = (256*10 + 1), shift right 8
     uint32_t pairs = ((digits * 2561U) >> 8) & 0x00FF00FFU;

     // Combine quads: multiply by 6553601 = (65536*100 + 1), shift right 16
     uint32_t result = ((pairs * 6553601U) >> 16) & 0x0000FFFFU;

     return result;
 }

size_t count_digits_simd(const char* str, size_t len) {
     if (len == 0) {
         return 0;
     }

 #ifdef STRATA_HAS_AVX2
     const __m256i lower = _mm256_set1_epi8('0' - 1);
     const __m256i upper = _mm256_set1_epi8('9' + 1);
     size_t pos = 0;

     while (pos + 32 <= len) {
         __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(str + pos));
         __m256i ge_lower = _mm256_cmpgt_epi8(chunk, lower); // chunk >= '0'
         __m256i lt_upper = _mm256_cmpgt_epi8(upper, chunk); // chunk <= '9'
         __m256i is_digit = _mm256_and_si256(ge_lower, lt_upper);

         uint32_t mask = static_cast<uint32_t>(_mm256_movemask_epi8(is_digit));
         if (mask != 0xFFFFFFFFu) {
             uint32_t inv = ~mask;
             unsigned int bit = __builtin_ctz(inv);
             return pos + static_cast<size_t>(bit);
         }

         pos += 32;
     }

     while (pos < len && is_digit_scalar(static_cast<unsigned char>(str[pos]))) {
         ++pos;
     }
     return pos;

 #elif defined(STRATA_HAS_SSE42)
     const __m128i lower = _mm_set1_epi8('0' - 1);
     const __m128i upper = _mm_set1_epi8('9' + 1);
     size_t pos = 0;

     while (pos + 16 <= len) {
         __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str + pos));
         __m128i ge_lower = _mm_cmpgt_epi8(chunk, lower);
         __m128i lt_upper = _mm_cmpgt_epi8(upper, chunk);
         __m128i is_digit = _mm_and_si128(ge_lower, lt_upper);

         uint16_t mask = static_cast<uint16_t>(_mm_movemask_epi8(is_digit));
         if (mask != 0xFFFFu) {
             uint16_t inv = static_cast<uint16_t>(~mask);
             unsigned int bit = __builtin_ctz(inv);
             return pos + static_cast<size_t>(bit);
         }

         pos += 16;
     }

     while (pos < len && is_digit_scalar(static_cast<unsigned char>(str[pos]))) {
         ++pos;
     }
     return pos;

 #elif defined(STRATA_HAS_NEON)
     const uint8x16_t lower = vdupq_n_u8('0');
     const uint8x16_t upper = vdupq_n_u8('9');
     size_t pos = 0;

     while (pos + 16 <= len) {
         uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(str + pos));
         uint8x16_t ge_lower = vcgeq_u8(chunk, lower);
         uint8x16_t le_upper = vcleq_u8(chunk, upper);
         uint8x16_t is_digit = vandq_u8(ge_lower, le_upper);

         uint8_t mask[16];
         vst1q_u8(mask, is_digit);
         for (int i = 0; i < 16; ++i) {
             if (mask[i] == 0) {
                 return pos + static_cast<size_t>(i);
             }
         }

         pos += 16;
     }

     while (pos < len && is_digit_scalar(static_cast<unsigned char>(str[pos]))) {
         ++pos;
     }
     return pos;

 #else
     size_t pos = 0;
     while (pos < len && is_digit_scalar(static_cast<unsigned char>(str[pos]))) {
         ++pos;
     }
     return pos;
 #endif
 }

 // ============================================================================
 // SWAR-accelerated integer parsing
 // ============================================================================

 bool parse_uint_simd(const char* str, size_t len, uint64_t& result, size_t& consumed) {
     if (len == 0) {
         return false;
     }

     // Check first character is a digit
     if (str[0] < '0' || str[0] > '9') {
         return false;
     }

     // Check for leading zero (not allowed in JSON except for "0" itself)
     if (str[0] == '0') {
         // After '0', must not be another digit
         if (len > 1 && str[1] >= '0' && str[1] <= '9') {
             return false;  // Leading zero not allowed
         }
         result = 0;
         consumed = 1;
         return true;
     }

     // Count digits using SIMD
     size_t digit_count = count_digits_simd(str, len);
     if (digit_count == 0) {
         return false;
     }

     // Check for overflow: uint64_t max is 18446744073709551615 (20 digits)
     // We can safely parse up to 19 digits, then check the 20th
     if (digit_count > 20) {
         return false;  // Definitely overflow
     }

     uint64_t value = 0;

     // Fast paths for small numbers (most common in JSON)
     if (digit_count == 1) {
         // Single digit: direct conversion
         value = static_cast<uint64_t>(str[0] - '0');
     } else if (digit_count == 2) {
         // 2 digits: inline multiply
         value = static_cast<uint64_t>(str[0] - '0') * 10 +
                 static_cast<uint64_t>(str[1] - '0');
     } else if (digit_count == 3) {
         // 3 digits: inline multiply
         value = static_cast<uint64_t>(str[0] - '0') * 100 +
                 static_cast<uint64_t>(str[1] - '0') * 10 +
                 static_cast<uint64_t>(str[2] - '0');
     } else if (digit_count == 4) {
         // 4 digits: use SWAR
         uint32_t chunk;
         std::memcpy(&chunk, str, 4);
         value = parse_4_digits_swar(chunk);
    } else if (digit_count <= 8) {
        // 5-8 digits: use SWAR for first 4 digits, scalar for remainder
        uint32_t chunk;
        std::memcpy(&chunk, str, 4);
        value = parse_4_digits_swar(chunk);
        // Parse remaining digits
        for (size_t i = 4; i < digit_count; ++i) {
            value = value * 10 + static_cast<uint64_t>(str[i] - '0');
        }
     } else if (digit_count <= 16) {
         // 9-16 digits: parse first 8 with SWAR, then remaining
         uint64_t chunk1;
         std::memcpy(&chunk1, str, 8);
         uint64_t first8 = parse_8_digits_swar(chunk1);

         size_t remaining = digit_count - 8;
         if (remaining >= 4) {
             uint32_t chunk2;
             std::memcpy(&chunk2, str + 8, 4);
             uint64_t next4 = parse_4_digits_swar(chunk2);
             value = first8 * 10000 + next4;

             // Parse remaining 0-4 digits
             for (size_t i = 12; i < digit_count; ++i) {
                 value = value * 10 + static_cast<uint64_t>(str[i] - '0');
             }
         } else {
             value = first8;
             for (size_t i = 8; i < digit_count; ++i) {
                 value = value * 10 + static_cast<uint64_t>(str[i] - '0');
             }
         }
     } else {
         // 17-20 digits: need overflow checking
         // Parse first 8 digits
         uint64_t chunk1;
         std::memcpy(&chunk1, str, 8);
         value = parse_8_digits_swar(chunk1);

         // Parse next 8 digits
         uint64_t chunk2;
         std::memcpy(&chunk2, str + 8, 8);
         uint64_t next8 = parse_8_digits_swar(chunk2);

         // Combine with overflow check
         // value * 10^8 + next8
        value = value * 100000000ULL + next8;

         // Parse remaining digits (17-20) with overflow checking
         for (size_t i = 16; i < digit_count; ++i) {
             uint64_t digit = static_cast<uint64_t>(str[i] - '0');
             if (value > (std::numeric_limits<uint64_t>::max() - digit) / 10) {
                 return false;  // Overflow
             }
             value = value * 10 + digit;
         }
     }

     result = value;
     consumed = digit_count;
     return true;
 }

 bool parse_int_simd(const char* str, size_t len, int64_t& result, size_t& consumed) {
     if (len == 0) {
         return false;
     }

     size_t pos = 0;
     bool negative = false;

     // Handle sign
     if (str[pos] == '-') {
         negative = true;
         ++pos;
     }

     if (pos >= len) {
         return false;
     }

     // Parse unsigned portion
     uint64_t unsigned_val;
     size_t unsigned_consumed;
     if (!parse_uint_simd(str + pos, len - pos, unsigned_val, unsigned_consumed)) {
         return false;
     }

     // Check overflow for signed range
     const uint64_t max_positive = static_cast<uint64_t>(std::numeric_limits<int64_t>::max());
     const uint64_t max_negative = max_positive + 1;  // |INT64_MIN|

     if (negative) {
         if (unsigned_val > max_negative) {
             return false;  // Overflow
         }
         if (unsigned_val == max_negative) {
             result = std::numeric_limits<int64_t>::min();
         } else {
             result = -static_cast<int64_t>(unsigned_val);
         }
     } else {
         if (unsigned_val > max_positive) {
             return false;  // Overflow
         }
         result = static_cast<int64_t>(unsigned_val);
     }

     consumed = pos + unsigned_consumed;
     return true;
 }

 } // namespace util
 } // namespace strata

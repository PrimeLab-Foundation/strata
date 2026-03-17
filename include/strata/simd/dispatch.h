#pragma once

/**
 * @file dispatch.h
 * @brief Compile-time and runtime SIMD backend dispatch for the structural indexer.
 *
 * Provides:
 *   - Compile-time detection via preprocessor (#ifdef __AVX512F__, __AVX2__, etc.)
 *   - Runtime cpuid-based feature detection with cached function pointers
 *   - Scalar fallback for all operations (no SIMD at all)
 *   - Backends: AVX-512, AVX2(+BMI2), SVE2, NEON, WASM SIMD, RVV, Scalar
 */

#include "strata/simd/index_builder.h"

#include <cstdint>

namespace strata {
namespace simd {

/// Available SIMD backends (ordered roughly by throughput on target hardware).
enum class Backend {
    SCALAR,    ///< No SIMD — portable C++ fallback
    AVX2,      ///< x86 AVX2 (vpshufb, PCLMUL, ctz-loop extraction)
    AVX2_BMI2, ///< x86 AVX2 + BMI2 (adds PEXT for fast bit extraction)
    AVX512,    ///< x86 AVX-512BW (64-byte classify, native kmask)
    NEON,      ///< ARM NEON (vqtbl1q_u8, PMULL prefix-XOR)
    SVE2,      ///< ARM SVE2 (svtbl, scalable 128–2048-bit vectors)
    WASM_SIMD, ///< WebAssembly SIMD128 (i8x16.swizzle, native bitmask)
    RVV,       ///< RISC-V Vector Extension 1.0 (vluxei8 gather)
};

/**
 * Detect the best available backend at runtime.
 *
 * On x86-64: uses cpuid to check AVX2, BMI2, PCLMUL support.
 * On ARM: returns NEON if __ARM_NEON is defined at compile time.
 * Otherwise: returns SCALAR.
 *
 * The result is computed once and cached in a function-local static.
 */
[[nodiscard]] Backend detect_backend() noexcept;

/**
 * Return the name of a backend as a string (for diagnostics).
 */
[[nodiscard]] const char* backend_name(Backend b) noexcept;

/**
 * Build a structural index for the document using the compile-time-selected backend.
 *
 * @param data  Pointer to the raw JSON document bytes.
 * @param len   Length of the document in bytes.
 * @return Structural index (positions + document length).
 */
[[nodiscard]] IndexBuilder::StructuralIndex index_document(const uint8_t* data, size_t len);

} // namespace simd
} // namespace strata

#pragma once

/**
 * @file dispatch.h
 * @brief Compile-time and runtime SIMD backend dispatch for the structural indexer.
 *
 * Provides:
 *   - Compile-time detection via preprocessor (#ifdef __AVX2__, __BMI2__, __PCLMUL__)
 *   - Runtime cpuid-based feature detection with cached function pointers
 *   - Scalar fallback for all operations (no SIMD at all)
 *   - ARM NEON stubs (interface only, implementation deferred)
 */

#include "strata/simd/index_builder.h"

#include <cstdint>

namespace strata {
namespace simd {

/// Available SIMD backends.
enum class Backend {
    SCALAR,    ///< No SIMD — portable C++ fallback
    AVX2,      ///< AVX2 without BMI2 (vpshufb, PCLMUL, ctz-loop extraction)
    AVX2_BMI2, ///< AVX2 + BMI2 (adds PEXT for fast bit extraction)
    NEON,      ///< ARM NEON (stub — not yet implemented)
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
 * Build a structural index for the document using the best available backend.
 *
 * This is the primary entry point for users who don't want to manage
 * StructuralIndexer / IndexBuilder manually.  It detects the backend once
 * (on first call) and dispatches to the appropriate implementation.
 *
 * @param data  Pointer to the raw JSON document bytes.
 * @param len   Length of the document in bytes.
 * @return Structural index (positions + document length).
 */
[[nodiscard]] IndexBuilder::StructuralIndex index_document(const uint8_t* data, size_t len);

} // namespace simd
} // namespace strata
